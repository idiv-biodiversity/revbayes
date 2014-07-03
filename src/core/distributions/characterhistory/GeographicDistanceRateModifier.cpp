//
//  GeographicDistanceRateModifier.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 8/8/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include <iomanip>
#include <cmath>
#include "CharacterEvent.h"
#include "GeographicArea.h"
#include "GeographicDistanceRateModifier.h"
#include "RbConstants.h"
#include "TimeAtlas.h"
#define EARTHRADIUSKM 6371

using namespace RevBayesCore;

GeographicDistanceRateModifier::GeographicDistanceRateModifier(TimeAtlas* ta, bool uadj, bool uav, bool udd, int idx, double dp, double th, std::string dt )

{
    // initialize
    atlas = ta;
    index = idx;
    distancePower = dp;
    threshhold = th;
    distanceType = dt;
    
    useAreaAdjacency = uadj;
    useAreaAvailable = uav;
    useDistanceDependence = udd;

    // get geography dimensions
    const std::vector<std::vector<GeographicArea*> > tmpAreas = atlas->getAreas();
    numAreas = 0;
    epochs = atlas->getEpochs();
    numEpochs = (unsigned)epochs.size();
    if (numEpochs > 0)
        numAreas = (unsigned)tmpAreas[0].size();
    
    epochOffset = numAreas * numAreas;
    areaOffset = numAreas;
    
    // assign areas for timeslice
    for (size_t i = 0; i < numEpochs; i++)
    {
        for (size_t j = 0; j < numAreas; j++)
        {
            GeographicArea* g = new GeographicArea( *tmpAreas[i][j] );
            areas.push_back(g);
            
            geographicCoordinates.push_back(g->getLatitude());
            geographicCoordinates.push_back(g->getLongitude());
        }
    }
    
    // initialize distance dependence
    geographicDistances.resize(numEpochs * numAreas * numAreas, 0.0);
    geographicDistancePowers.resize(numEpochs * numAreas * numAreas, 1.0);
    if (useDistanceDependence)
        initializeDistances();

//    // initialize provided area dispersal/extinction rates
//    dispersalValues.resize(numEpochs * numAreas * numAreas, 1.0);
//    extinctionValues.resize(numEpochs * numAreas, 1.0);
//    if (useAreaAdjacency && useAreaAvailable)
//        initializeDispersalExtinctionValues();

    // initialize adjacencies between areas
    std::set<size_t> tmpSet;
    for (size_t i = 0; i < numAreas; i++)
        tmpSet.insert(i);
    
    availableAreaVector.resize(numEpochs * numAreas, 1.0);
    adjacentAreaVector.resize(numEpochs * numAreas * numAreas, 1.0);
    availableAreaSet.resize(numEpochs, tmpSet);
    adjacentAreaSet.resize(numEpochs*numAreas, tmpSet);

    if (useAreaAdjacency)
       initializeAdjacentAreas();
    
    // unimplemented, but helps for computations for large N
    //computeAllPairwiseDistanceOrder();
    
    setDistancePower(dp, true);
    update();
}

GeographicDistanceRateModifier::GeographicDistanceRateModifier(const GeographicDistanceRateModifier& g)
{
    
    if (&g != this)
    {
        atlas = g.atlas;
        index = g.index;
        epochs = g.epochs;
        areas = g.areas;
        
        distanceType = g.distanceType;
        geographicCoordinates = g.geographicCoordinates;
        geographicDistances = g.geographicDistances;
        geographicDistancePowers = g.geographicDistancePowers;
        geographicDistanceOrder = g.geographicDistanceOrder;
        
        useAreaAvailable = g.useAreaAvailable;
        useAreaAdjacency = g.useAreaAdjacency;
        useDistanceDependence = g.useDistanceDependence;
        
//        dispersalValues = g.dispersalValues;
//        extinctionValues = g.extinctionValues;
//        adjacentAreaSet.resize(g.adjacentAreaSet.size());
//        availableAreaSet.resize(g.availableAreaSet.size());
//        
//        for (size_t i = 0; i < adjacentAreaSet.size(); i++)
//        {
//            std::cout << i << " " << g.adjacentAreaSet[i].size() << "\n";
//            adjacentAreaSet[i] = g.adjacentAreaSet[i];
//            for (std::set<size_t>::iterator it = adjacentAreaSet[i].begin(); it != adjacentAreaSet[i].end(); it++)
//                std::cout << " " << *it;
//            std::cout << "\n";
//        }
//        
//        for (size_t i = 0; i < availableAreaSet.size(); i++)
//            availableAreaSet[i] = g.availableAreaSet[i];
        
        adjacentAreaSet = g.adjacentAreaSet;
        availableAreaSet = g.availableAreaSet;
        
        adjacentAreaVector = g.adjacentAreaVector;
        availableAreaVector = g.availableAreaVector;
        
        numAreas = g.numAreas;
        numEpochs = g.numEpochs;
        areaOffset = g.areaOffset;
        epochOffset = g.epochOffset;
        threshhold = g.threshhold;
        distancePower = g.distancePower;
        
        setDistancePower(1.0, true);
        update();
    }
}

double GeographicDistanceRateModifier::computeRateModifier(std::vector<CharacterEvent *> currState, CharacterEvent* newState, double age)
{
    unsigned epochIdx = getEpochIndex(age);
    
    double rate = 0.0;
    double sum = 0.0;
    double r = 0.0;
    
    unsigned s = newState->getState();
    unsigned charIdx = newState->getIndex();
    bool areaAvailable = availableAreaVector[epochIdx*numAreas + charIdx] > 0.0;

    // area exists and is lost
    if (areaAvailable && s == 0)
    {
        r = 1.0;
    }
    
    // area does not exist and is lost
    if (!areaAvailable && s == 0)
    {
        r = 1e10;
    }
    
    // area does not exist and is gained
    else if (!areaAvailable && s == 1)
    {
        r = 0.0;
    }

    // area exists and is gained
    if (areaAvailable && s == 1)
    {
        // ignore graph structure for now!
//        return 1.0;
        
        // determine which areas are present and which are absent
        present.clear();
        absent.clear();
        
        std::set<size_t>::iterator it;
        for (it = availableAreaSet[epochIdx].begin(); it != availableAreaSet[epochIdx].end(); it++)
        {
            if (currState[*it]->getState() == 0)
            {
                absent.insert(currState[*it]);
//                std::cout << "a " << absent.size() << "\n";
            }
            else
            {
                present.insert(currState[*it]);
//                std::cout << "p " << present.size() << "\n";
            }
        }
        
        if (present.size() == 0)
            return 1.0;
        
        std::set<CharacterEvent*>::iterator it_p;
        std::set<CharacterEvent*>::iterator it_a;
        for (it_p = present.begin(); it_p != present.end(); it_p++)
        {
            size_t idx_p = (*it_p)->getIndex();
            
            for (it_a = absent.begin(); it_a != absent.end(); it_a++)
            {
                size_t idx_a = (*it_a)->getIndex();
                size_t idx_e = epochIdx * epochOffset + idx_p * areaOffset + idx_a;
                
//                std::cout << "  " << idx_p << " " << idx_a << " " << idx_p << "\n";
                double d = adjacentAreaVector[idx_e];
                if (useDistanceDependence && d != 0.0)
                {
                    d = geographicDistancePowers[idx_e];
                }
                
                sum += d;
                
                if (idx_a == charIdx)
                    rate += d;
            }
        }
        
        r = absent.size() * rate / sum;
//        std::cout << "rateMod " << r << " = " << absent.size() << " * " << rate << " / " << sum << "\n";
    }
    
    return r;
}

double GeographicDistanceRateModifier::computeRateModifier_test(std::vector<CharacterEvent *> currState, CharacterEvent* newState, double age)
{
    if (!useDistanceDependence || distancePower == 0.0 || newState->getState() == 0)
    {
        return 1.0;
    }
    
    unsigned epochIndex = getEpochIndex(age);
    
    // determine which areas are present and which are absent
    present.clear();
    absent.clear();
    for (unsigned i = 0; i < numAreas; i++)
    {
        if (currState[i]->getState() == 0)
            absent.insert(currState[i]);
        else
            present.insert(currState[i]);
    }
    
    if (present.size() == 0)
        return 1.0;
    
    // get sum of distances_ij^beta
    double rate = 0.0;
    double sum = 0.0;
    std::set<CharacterEvent*>::iterator it_p;
    std::set<CharacterEvent*>::iterator it_a;
    for (it_p = present.begin(); it_p != present.end(); it_p++)
    {
        size_t idx_p = (*it_p)->getIndex();
        
        for (it_a = absent.begin(); it_a != absent.end(); it_a++)
        {
            size_t idx_a = (*it_a)->getIndex();
            size_t idx_e = epochIndex*epochOffset + idx_p*areaOffset + idx_a;
            
            double d = geographicDistancePowers[ idx_e  ];
            if  (useAreaAdjacency && dispersalValues[ idx_e ] == 0)
                d = 0.0;
            
            sum += d;
            
            if (idx_a == newState->getIndex())
                rate += d;
        }
    }
    
    // get sum-normalized rate-modifier
    double r = absent.size() * rate / sum;
    
    return r;
}

double GeographicDistanceRateModifier::computeSiteRateModifier(const TopologyNode& node, CharacterEvent* currState, CharacterEvent* newState, double age)
{
    unsigned s = newState->getState();
    unsigned charIdx = newState->getIndex();
    unsigned epochIdx = getEpochIndex(age);
    
    // r == 1 if available, r == 0 if unavailable
    double r = availableAreaVector[ epochIdx*numAreas + charIdx ];

    // force extinction if unavailable and present
    if (r == 0.0 && s == 0)
    {
        
        r = 1e10;
    }
    else if (r == 0.0 && s == 1)
    {
        
        ;
    }
    
    return r;
}

double GeographicDistanceRateModifier::computeSiteRateModifier(const TopologyNode& node, unsigned from, unsigned to, unsigned charIdx, double age)
{
    unsigned epochIdx = getEpochIndex(age);
    
    // r == 1 if available, r == 0 if unavailable
    double r = availableAreaVector[ epochIdx*numAreas + charIdx ];

    // force extinction if unavailable and present
    if (r == 0.0 && to == 0)
    {
      
        r = 1e10;
    }
    else if (r == 0.0 && to == 1)
    {
        
        ;
    }
    
    return r;
}

double GeographicDistanceRateModifier::computeRateModifier(const TopologyNode& node, std::vector<CharacterEvent *> currState, CharacterEvent* newState, double age)
{
    return computeRateModifier(currState, newState, age);
}

double GeographicDistanceRateModifier::computeRateModifier(std::vector<CharacterEvent *> currState, CharacterEvent* newState)
{
    return computeRateModifier(currState, newState, 0.0);
}


GeographicDistanceRateModifier* GeographicDistanceRateModifier::clone(void) const
{
    return new GeographicDistanceRateModifier(*this);
}



void GeographicDistanceRateModifier::update(void)
{
    for (size_t i = 0; i < numEpochs; i++)
    {
        unsigned iOffset = (unsigned)(i*epochOffset);
        for (size_t j = 0; j < numAreas; j++)
        {
            unsigned jOffset = (unsigned)(j*areaOffset);
            for (size_t k = 0; k < j; k++)
            {
                //double d = exp(-distancePower * geographicDistances[ iOffset + jOffset + k ]); //, -distancePower);
                
                double d = 1.0;
                if (distancePower != 0.0)
                    d = pow(geographicDistances[ iOffset + jOffset + k ], -distancePower);

                geographicDistancePowers[ iOffset + jOffset + k ] = d;
                geographicDistancePowers[ iOffset + k*areaOffset + j ] = d;
            }
        }
    }
}

void GeographicDistanceRateModifier::initializeDistances(void)
{
    for (unsigned i = 0; i < numEpochs; i++)
    {
        for (unsigned j = 0; j < numAreas; j++)
        {
            for (unsigned k = j; k < numAreas; k++)
            {
                double d = computePairwiseDistances(i, j, k);
                geographicDistances[epochOffset*i + areaOffset*j + k] = d;
                geographicDistances[epochOffset*i + areaOffset*k + j] = d;
            }
        }
    }
}

//void GeographicDistanceRateModifier::initializeDispersalExtinctionValues(void)
//{
//    for (unsigned i = 0; i < numEpochs; i++)
//    {
//        for (unsigned j = 0; j < numAreas; j++)
//        {
//            const std::vector<double>& dvs = areas[numAreas*i + j]->getDispersalValues();
//            const std::vector<double>& evs = areas[numAreas*i + j]->getExtinctionValues();
//            
//            extinctionValues[numEpochs*i + j] = evs[0];
//            for (unsigned k = 0; k < numAreas; k++)
//            {
//                dispersalValues[epochOffset*i + areaOffset*j + k] = dvs[k];
//            }
//        }
//    }
//}

void GeographicDistanceRateModifier::initializeAdjacentAreas(void)
{
    adjacentAreaSet.resize(numEpochs*numAreas);
    availableAreaSet.resize(numEpochs);
    
    for (size_t i = 0; i < availableAreaSet.size(); i++)
//    {
        availableAreaSet[i].clear();
//        std::set<size_t>::iterator it = availableAreaSet[i].begin();
//        std::cout << i << ": ";
//        for (; it != availableAreaSet[i].end(); it++)
//        {
//            std::cout << *it << " ";
//        }
//        std::cout << "\n";
//    }
    
    for (unsigned i = 0; i < numEpochs; i++)
    {
        for (unsigned j = 0; j < numAreas; j++)
        {
            const std::vector<double>& dvs = areas[numAreas*i + j]->getDispersalValues();
//            const std::vector<double>& evs = areas[numAreas*i + j]->getExtinctionValues();

            for (unsigned k = 0; k < numAreas; k++)
            {
                double d = 0.0;
                if (dvs[k] > 0.0)
                    d = 1.0;
                
                adjacentAreaVector[epochOffset*i + areaOffset*j + k] = d;
                adjacentAreaSet[numAreas*i + j].insert(k);
//                std::cout << epochOffset*i + areaOffset*j + k << " " << i << " " << j << " " << k << " " << d << "\n";
                
                if (j == k)
                {
                    availableAreaVector[numAreas*i + j] = d;
                    if (d > 0.0)
                        availableAreaSet[i].insert(k);
                }
            }
        }
    }
    
//    for (size_t i = 0; i < availableAreaSet.size(); i++)
//    {
//        std::set<size_t>::iterator it = availableAreaSet[i].begin();
//        std::cout << i << ": ";
//        for (; it != availableAreaSet[i].end(); it++)
//        {
//            std::cout << *it << " ";
//        }
//        std::cout << "\n";
//    }
//    
//    std::cout << "\n";
}


void GeographicDistanceRateModifier::setDistancePower(double dp, bool upd)
{
    bool changed = (distancePower != dp);
    if (upd && changed)
    {
        distancePower = dp;
        update();
    }
}

void GeographicDistanceRateModifier::setGeographicDistancePowers(const std::vector<double>& dp)
{
    geographicDistancePowers = dp;
}

const std::vector<double>& GeographicDistanceRateModifier::getGeographicDistancePowers(void) const
{
    return geographicDistancePowers;
}

unsigned GeographicDistanceRateModifier::getEpochIndex(double age)
{
    unsigned index = 0;
    while (age <= epochs[index] && index < numEpochs-1)
    {
        index++;
    };
    return index;
}

const std::vector<double>& GeographicDistanceRateModifier::getEpochs(void) const
{
    return epochs;
}

const std::vector<double>& GeographicDistanceRateModifier::getDispersalValues(void) const
{
    return dispersalValues;
}

const std::vector<double>& GeographicDistanceRateModifier::getExtinctionValues(void) const
{
    return extinctionValues;
}

const std::vector<double>& GeographicDistanceRateModifier::getAdjacentAreaVector(void) const
{
    return adjacentAreaVector;
}

const std::vector<double>& GeographicDistanceRateModifier::getAvailableAreaVector(void) const
{
    return availableAreaVector;
}


double GeographicDistanceRateModifier::computePairwiseDistances(int i, int j, int k)
{
    double d = 0.0;
    double rad = RbConstants::PI/180;
    
    unsigned idx0 = i*numAreas*2 + j*2;
    unsigned idx1 = i*numAreas*2 + k*2;
    
    double lat0 = geographicCoordinates[ idx0 + 0] * rad;
    double lat1 = geographicCoordinates[ idx1 + 0] * rad;
    double lon0 = geographicCoordinates[ idx0 + 1] * rad;
    double lon1 = geographicCoordinates[ idx1 + 1] * rad;

    if (distanceType == "haversine")
    {
        double dlat = lat0 - lat1;
        double dlon = lon0 - lon1;
        double sindlat = sin(dlat/2);
        double sindlon = sin(dlon/2);
        double a = sqrt(sindlat * sindlat + cos(lat0) * cos(lat1) * sindlon * sindlon);
        if (a < 0.9995)
            d = 2 * asin(a) * EARTHRADIUSKM;
        else
            d = 2 * asin(1.0) * EARTHRADIUSKM;
    }
    else if (distanceType == "euclidean")
    {
        double dlat = lat0 - lat1;
        double dlon = lon0 - lon1;
        d = pow(dlat*dlat+dlon*dlon, 0.5);
    }
    
    return d;
}

void GeographicDistanceRateModifier::setInboundDispersal(const std::vector<double> &v)
{
    std::vector<double> r = v;
    for (size_t i = 0; i < numEpochs; i++)
    {
        for (size_t j = 0; j < numAreas; j++)
        {
            // update diagonal with sum of incoming rates per epoch
            double sum = 0.0;
            for (size_t k = 0; k < numAreas; k++)
            {
                double d = v[ epochOffset*i + k*numAreas + j ];
                sum += d;
            }
            inboundDispersalValues[ epochOffset*i + j ] = (sum > 0.0 ? 1.0 : 0.0);
        }
    }
}

void GeographicDistanceRateModifier::print(std::vector<std::vector<double> > m)
{
//    std::vector<std::vector<double> >::iterator it0;
//    std::vector<double>::iterator it1;
//    
//    for (it0 = m.begin(); it0 != m.end(); it0++)
//    {
//        for (it1 = it0->begin(); it1 != it0->end(); it1++)
//        {
//            std::cout << *it1 << " ";
//        }
//        std::cout << "\n";
//    }
}

void GeographicDistanceRateModifier::printAll(void)
{
//    std::cout << "\ngc\n";
//    print(geographicCoordinates);
//        std::cout << "\ngd\n";
//    print(geographicDistances);
//        std::cout << "\ngdp\n";
//    print(geographicDistancePowers);
//    
}
