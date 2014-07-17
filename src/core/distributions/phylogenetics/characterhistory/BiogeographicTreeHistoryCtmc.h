//
//  BiogeographicTreeHistoryCtmc.h
//  rb_mlandis
//
//  Created by Michael Landis on 3/29/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__BiogeographicTreeHistoryCtmc__
#define __rb_mlandis__BiogeographicTreeHistoryCtmc__

#include "AbstractTreeHistoryCtmc.h"
#include "RateMap_Biogeography.h"
#include "ContinuousCharacterData.h"
#include "DistributionExponential.h"
#include "RateMap.h"
#include "RbConstants.h"
#include "RbVector.h"
#include "StandardState.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include "TreeChangeEventListener.h"
#include "TypedDistribution.h"

#include "ConstantNode.h"
#include "DiscreteCharacterData.h"
#include "DiscreteCharacterState.h"
#include "RandomNumberFactory.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <cstring>

namespace RevBayesCore {
    
    template<class charType, class treeType>
    class BiogeographicTreeHistoryCtmc : public AbstractTreeHistoryCtmc<charType, treeType> {
        
    public:
        BiogeographicTreeHistoryCtmc(const TypedDagNode< treeType > *t, size_t nChars, size_t nSites, bool useAmbigChar=false, bool forbidExt=true, bool useClado=true);
        BiogeographicTreeHistoryCtmc(const BiogeographicTreeHistoryCtmc &n);                                                                         //!< Copy constructor
        virtual                                            ~BiogeographicTreeHistoryCtmc(void);                                                //!< Virtual destructor
        
        // public member functions
        
        BiogeographicTreeHistoryCtmc*                       clone(void) const;                                                           //!< Create an independent clone
        void                                                initializeValue(void);
        void                                                redrawValue(void);
        
        // These will be migrated to PathRejectionSampleProposal and NodeRejectionSampleProposal
        bool                                                samplePathStart(const TopologyNode& node, const std::set<size_t>& indexSet);
        bool                                                samplePathEnd(const TopologyNode& node, const std::set<size_t>& indexSet);
        bool                                                samplePathHistory(const TopologyNode& node, const std::set<size_t>& indexSet);
        
        // These have been migrated to RateMap_Biogeography and BiogeographyRateMapFunction
        void                                                setRateMap(const TypedDagNode< RateMap > *rm);
        void                                                setRateMap(const TypedDagNode< RbVector< RateMap > > *rm);
        void                                                setRootFrequencies(const TypedDagNode< std::vector< double > > *f);
        void                                                setSiteRates(const TypedDagNode< std::vector< double > > *r);
        void                                                setDistancePower(const TypedDagNode<double>* dp);

        // ambiguous characters at tips
        const std::vector<double>&                          getTipProbs(const TopologyNode& nd);
        const std::vector<std::vector<double> >&            getTipProbs(void);
        void                                                setTipProbs(const AbstractCharacterData* d);
        
        
        // cladogenic state information
        const std::vector<int>&                             getBuddingStates(void);
        int                                                 getBuddingState(const TopologyNode& nd);
        void                                                setBuddingState(const TopologyNode& nd, int s);
        const std::vector<int>&                             getCladogenicStates(void);
        int                                                 getCladogenicState(const TopologyNode& nd);
        void                                                setCladogenicState(const TopologyNode& nd, int s);
        
        // epoch info
        const std::vector<double>&                          getEpochs(void) const;
        
        
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);                     //!< Implementation of swaping parameters
        virtual void                                        simulate(void);
        const bool                                          useCladogenicEvents(void) const;
        
    protected:
        
        virtual double                                      computeRootLikelihood(const TopologyNode &n);
        virtual double                                      computeInternalNodeLikelihood(const TopologyNode &n);
        virtual double                                      computeTipLikelihood(const TopologyNode &node);
        virtual const std::vector<double>&                  getRootFrequencies(void);
        // (not needed)        void                         keepSpecialization(DagNode* affecter);
        // (not needed)        void                         restoreSpecialization(DagNode *restorer);
        virtual void                                        touchSpecialization(DagNode *toucher);
        
        
    private:
        

        
        // helper function
        unsigned                                            getEpochIndex(double age);
        unsigned                                            getEpochIndex(const std::vector<double>& epochs, double age);
        size_t                                              numOn(const std::vector<CharacterEvent*>& s);
        bool                                                historyContainsExtinction(const std::vector<CharacterEvent*>& currState, const std::multiset<CharacterEvent*,CharacterEventCompare>& history);
        
        // members
//        const TypedDagNode< double >*                       homogeneousClockRate;
//        const TypedDagNode< std::vector< double > >*        heterogeneousClockRates;
        const TypedDagNode< std::vector< double > >*        rootFrequencies;
        const TypedDagNode< std::vector< double > >*        siteRates;
        const TypedDagNode< RateMap >*                      homogeneousRateMap;
        const TypedDagNode< RbVector< RateMap > >*          heterogeneousRateMaps;
        std::vector<std::vector<double> >                   tipProbs;
        std::vector<double>                                 epochs;
        
        // flags specifying which model variants we use
        std::vector<int>                                    cladogenicState;
        std::vector<int>                                    buddingState;
        bool                                                branchHeterogeneousClockRates;
        bool                                                branchHeterogeneousSubstitutionMatrices;
        bool                                                rateVariationAcrossSites;
        bool                                                cladogenicEvents;
        bool                                                imperfectTipData;
        bool                                                forbidExtinction;
        int                                                 redrawCount;
        
        virtual void                                        simulate(const TopologyNode& node, DiscreteCharacterData< charType > &taxa);
        
    };
    
}



template<class charType, class treeType>
RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::BiogeographicTreeHistoryCtmc(const TypedDagNode<treeType> *t, size_t nChars, size_t nSites, bool useAmbigChar, bool forbidExt, bool useClado) : AbstractTreeHistoryCtmc<charType, treeType>(  t, nChars, nSites, useAmbigChar ) {
    
    // initialize with default parameters
//    homogeneousClockRate        = new ConstantNode<double>("clockRate", new double(1.0) );
//    heterogeneousClockRates     = NULL;
    rootFrequencies             = NULL;
    siteRates                   = NULL;
    homogeneousRateMap          = NULL; // Define a good standard JC RateMap
    heterogeneousRateMaps       = NULL;
    tipProbs.clear();
    redrawCount                 = 0;
    
    
    // flags specifying which model variants we use
    branchHeterogeneousClockRates               = false;
    branchHeterogeneousSubstitutionMatrices     = false;
    rateVariationAcrossSites                    = false;
    cladogenicEvents                            = useClado;
    imperfectTipData                            = false;
    forbidExtinction                            = forbidExt;
    
    cladogenicState                             = std::vector<int>(this->histories.size(), 0);
    if (cladogenicEvents) {
        for (size_t i = 0; i < cladogenicState.size(); i++) {
            cladogenicState[i] = int(GLOBAL_RNG->uniform01() * 2);
        }
    }
    buddingState                                = std::vector<int>(this->histories.size(), 0);
    epochs                                      = std::vector<double>(1,0.0);
//    epochs                                      = std::vector<double>(1,0.0);

    
    // add the parameters to the parents list
//    this->addParameter( homogeneousClockRate );
//    this->addParameter( distancePower );
    
    // Uncomment this to draw the initial state
    // this->redrawValue();
    
}


template<class charType, class treeType>
RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::BiogeographicTreeHistoryCtmc(const BiogeographicTreeHistoryCtmc &d) : AbstractTreeHistoryCtmc<charType, treeType>( d ) {
    // parameters are automatically copied
    // initialize with default parameters
//    homogeneousClockRate        = d.homogeneousClockRate;
//    heterogeneousClockRates     = d.heterogeneousClockRates;
    rootFrequencies             = d.rootFrequencies;
    siteRates                   = d.siteRates;
    homogeneousRateMap          = d.homogeneousRateMap;
    heterogeneousRateMaps       = d.heterogeneousRateMaps;
//    distancePower               = d.distancePower;
    tipProbs                    = d.tipProbs;
    redrawCount                 = d.redrawCount;
    
    // flags specifying which model variants we use
    cladogenicState                             = d.cladogenicState;
    buddingState                                = d.buddingState;
    epochs                                      = d.epochs;
    branchHeterogeneousClockRates               = d.branchHeterogeneousClockRates;
    branchHeterogeneousSubstitutionMatrices     = d.branchHeterogeneousSubstitutionMatrices;
    rateVariationAcrossSites                    = d.rateVariationAcrossSites;
    cladogenicEvents                            = d.cladogenicEvents;
    imperfectTipData                            = d.imperfectTipData;
    forbidExtinction                            = d.forbidExtinction;
}


template<class charType, class treeType>
RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::~BiogeographicTreeHistoryCtmc( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
    
}


template<class charType, class treeType>
RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>* RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::clone( void ) const {
    
    return new BiogeographicTreeHistoryCtmc<charType, treeType>( *this );
}



template<class charType, class treeType>
double RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::computeRootLikelihood(const TopologyNode &n)
{
    return 0.0;
}

template<class charType, class treeType>
double RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::computeInternalNodeLikelihood(const TopologyNode &node)
{
//    return 0.0;
    size_t nodeIndex = node.getIndex();
    double lnL = 0.0;

    BranchHistory* bh = this->histories[nodeIndex];
    std::vector<CharacterEvent*> currState = bh->getParentCharacters();
    unsigned int n1 = (unsigned)numOn(currState);
	unsigned int n0 = (unsigned)(this->numSites - n1);
    unsigned counts[2] = { n0, n1 };
    
    if (node.isRoot())
    {
        return 0.0;
    }
    else if (counts[1] == 0 && forbidExtinction)
    {
        return RbConstants::Double::neginf;
    }
    else
    {
        const std::multiset<CharacterEvent*,CharacterEventCompare>& history = bh->getHistory();
        std::multiset<CharacterEvent*,CharacterEventCompare>::iterator it_h;
        
//        const treeType& tree = this->tau->getValue();
        double branchLength = node.getBranchLength();
        double currAge = (node.isRoot() ? 1e10 : node.getParent().getAge());
        double startAge = currAge;
        double endAge = node.getAge();
        const RateMap_Biogeography& rm = static_cast<const RateMap_Biogeography&>(homogeneousRateMap->getValue());
        
//        if (nodeIndex==1)
//        {
//            std::vector<double> glr = rm.getHomogeneousGainLossRates();
//            std::cout << "glr " << glr[0] << " " << glr[1] << "\n";
//        }

        // handle stratified/epoch models
        const std::vector<double>& epochs = rm.getEpochs();
        size_t epochIdx = getEpochIndex(epochs, currAge);
        double epochEndAge = epochs[epochIdx];
        
        // stepwise events
        double t = 0.0;
        double dt = 0.0;
        double da = 0.0;
      
        bool useEpoch = true;
        bool debug = !true;
        if (debug) std::cout << "--lnL--\n";
        if (debug) std::cout << "BranchStart  ("  << startAge << " to " << endAge << ")\n" << "  a=" << currAge << "\n";
        for (it_h = history.begin(); it_h != history.end(); it_h++)
        {
            // next event time
            double idx = (*it_h)->getIndex();
            dt = (*it_h)->getTime() - t;
            da = dt * branchLength;
            
            // reject extinction
            unsigned s = (*it_h)->getState();
            if (counts[1] == 1 && s == 0 && forbidExtinction)
            {
                
                return RbConstants::Double::neginf;
                break;
            }

            // if epoch crossed, compute prob no events until boundary then advance epochIdx
            while (useEpoch && currAge - da < epochEndAge)
            {
                // waiting factor
                double sr = rm.getSumOfRates(node, currState, counts, currAge);
                lnL += -sr * (currAge - epochEndAge);
                if (debug) std::cout << "NextEpoch    (" << epochIdx << ":" << epochEndAge << ")\n" << "  a=" << currAge << " lnL=" << lnL << " sr=" << sr << " tr=" << "wait "<< " da=" << currAge-epochEndAge << "\n";
                
                // if before branch end, advance epoch
                if (endAge < epochEndAge)
                {
                    epochIdx++;
                    da -= (currAge - epochEndAge);
                    currAge = epochEndAge;
                    epochEndAge = epochs[epochIdx];
                }
                // otherwise, exit loop
                else
                    break;
                
            }
            
            // lnL for stepwise events for p(x->y)
            double tr = rm.getRate(node, currState, *it_h, counts, currAge);
            double sr = rm.getSumOfRates(node, currState, counts, currAge);
            lnL += -(sr * da) + log(tr);
            if (debug) std::cout << "NextEvent    (s=" << (*it_h)->getState() << " i=" << (*it_h)->getIndex() << " t=" << (*it_h)->getTime() << ")\n" << "  a=" << currAge << " lnL=" << lnL << " sr=" << sr << " tr=" << tr << " da=" << currAge-da << "\n";

            // update counts
            counts[currState[idx]->getState()] -= 1;
            counts[s] += 1;
            
            // update time and state
            currState[idx] = *it_h;
            t += dt;
            currAge -= da;
            
        }

        // lnL for final non-event
        if (useEpoch) {
            while (epochEndAge > endAge)
            {
                // waiting factor
                double sr = rm.getSumOfRates(node, currState, counts, currAge);
                lnL += -sr * (currAge - epochEndAge);
                if (debug) std::cout << "EndEpoch     (" << epochIdx << ":" << epochEndAge << ")\n" << "  a=" << currAge << " lnL=" << lnL << " sr=" << sr << " tr=" << "wait "<< " da=" << currAge-epochEndAge << "\n";

                
                // advance epoch
//                if (epochEndAge > endAge)
                {
                    epochIdx++;
                    currAge = epochEndAge;
                    epochEndAge = epochs[epochIdx];
                }
            }
            double sr = rm.getSumOfRates(node, currState, counts, currAge);
            lnL += -sr * (currAge - endAge);
            if (debug) std::cout << "EndBranch   (" << epochIdx << ":" << endAge << ")\n" << "  a=" << currAge << " lnL=" << lnL << " sr=" << sr << " tr=" << "wait "<< " da=" << currAge-endAge << "\n";


        }
        else
        {
            double sr = rm.getSumOfRates(node, currState, counts, currAge);
            lnL += -sr * ( (1.0 - t) * branchLength );
            if (debug) std::cout << "EndBranch   " << epochIdx << ":" << epochEndAge << ")\n" << "  a=" << currAge << " lnL=" << lnL << " sr=" << sr << " tr=" << "wait "<< " da=" << currAge-epochEndAge << "\n";        }
        if (debug) std::cout << "-------\n\n";

//        if (debug) std::cout << lnL << "\n";
        
        if (lnL != lnL)
        {
            
            std::cout << "nan??\n";
        }
        return lnL;
    }
}



template<class charType, class treeType>
double RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::computeTipLikelihood(const TopologyNode &node)
{
    double lnL = 0.0;
    size_t nodeIndex = node.getIndex();
    
    BranchHistory* bh = this->histories[nodeIndex];
    
    // update tip lnLs for ambiguous characters
    if (this->usingAmbiguousCharacters && node.isTip())
    {
        const std::vector<CharacterEvent*>& tipState = bh->getChildCharacters();
        for (size_t i = 0; i < tipState.size(); i++)
        {
            double v = tipProbs[nodeIndex][i];
            if (tipState[i]->getState() == 0)
                v = 1 - v;
            lnL += std::log(v);
        }
    }
    return lnL;
}



template<class charType, class treeType>
const std::vector<double>& RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::getRootFrequencies( void ) {
    
    if ( branchHeterogeneousSubstitutionMatrices || rootFrequencies != NULL )
    {
        return rootFrequencies->getValue();
    }
    else
    {
        throw RbException("BUG: Reached end of a non-void function in BiogeographicTreeHistoryCtmc.");
    }
    
}

template<class charType, class treeType>
void RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::initializeValue( void )
{
    if (this->dagNode->isClamped())
    {
        std::vector<TopologyNode*> nodes = AbstractTreeHistoryCtmc<charType,treeType>::tau->getValue().getNodes();
        for (size_t i = 0; i < nodes.size(); i++)
        {
            TopologyNode* node = nodes[i];
            if (node->isTip())
            {
                DiscreteTaxonData<StandardState>& d = static_cast< DiscreteTaxonData<StandardState>& >( this->value->getTaxonData( node->getName() ) );
                
                std::vector<CharacterEvent*> tipState;
                
                for (size_t j = 0; j < d.size(); j++)
                {
                    unsigned s = 0;
                    if (!this->usingAmbiguousCharacters)
                        s = (unsigned)d[j].getStateIndex();
                    else if (GLOBAL_RNG->uniform01() < tipProbs[node->getIndex()][j])
                        s = 1;
                        
                    CharacterEvent* evt = new CharacterEvent(j, s, 1.0);
                    tipState.push_back( evt );
                }
                
                this->histories[node->getIndex()]->setChildCharacters(tipState);
                tipState.clear();
            }
        }
        this->tipsInitialized = true;
    }
}

template<class charType, class treeType>
size_t RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::numOn(const std::vector<CharacterEvent*>& s)
{
    size_t n = 0;
    for (size_t i = 0; i < s.size(); i++)
        if (s[i]->getState() == 1)
            n++;
    return n;
}

template<class charType, class treeType>
unsigned RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::getEpochIndex(const std::vector<double>& epochs, double age)
{
    unsigned index = 0;
    while (age <= epochs[index] && index < epochs.size() - 1)
    {
        index++;
    };
    return index;
}

template<class charType, class treeType>
unsigned RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::getEpochIndex(double age)
{
    unsigned index = 0;
    while (age <= epochs[index] && index < epochs.size() - 1)
    {
        index++;
    };
    return index;
}

template<class charType, class treeType>
void RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::redrawValue( void )
{
    if (this->tipsInitialized == false)
        initializeValue();
    
    std::set<size_t> indexSet;
    for (size_t i = 0; i < this->numSites; i++)
        indexSet.insert(i);

    // sample node states
    std::vector<TopologyNode*> nodes = AbstractTreeHistoryCtmc<charType,treeType>::tau->getValue().getNodes();
    for (size_t i = 0; i < nodes.size(); i++)
    {
        TopologyNode* nd = nodes[i];
        
        int samplePathEndCount = 0;
        do
        {
            samplePathEndCount++;
        } while (samplePathEnd(*nd,indexSet) == false && samplePathEndCount < 100);
  
        int samplePathStartCount = 0;
        do
        {
            samplePathStartCount++;
        } while (samplePathStart(*nd,indexSet) == false && samplePathStartCount < 100);
    }
    
    // sample paths
    for (size_t i = 0; i < nodes.size(); i++)
    {
        TopologyNode* nd = nodes[i];

        int samplePathHistoryCount = 0;
        do
        {
            samplePathHistoryCount++;
        } while (samplePathHistory(*nd,indexSet) == false && samplePathHistoryCount < 100);
        
//        this->histories[i]->print();
    }
    
    double lnL = this->computeLnProbability();
    
    if (lnL == RbConstants::Double::neginf)
    {
        for (size_t i = 0; i < nodes.size(); i++)
        {
            this->fireTreeChangeEvent(*nodes[i]);
        }
        redrawValue();
    }
}


template<class charType, class treeType>
bool RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::samplePathStart(const TopologyNode& node, const std::set<size_t>& indexSet)
{
    
    // ignore tips
    if (node.isTip())
        return true;
    
    // sample post-speciation path start states

    // 0: sympatric speciation, iid range inheritance
    // A = L v R, n(L) = n(R) = n(A), L ^ R = A

    // 1: peripatric speciation
    // A = L v R, n(R) = 1, n(L) = n(A), L ^ R = R
    
    // 2: allopatric speciation
    // A = L v R, n(R) = 1, n(L) = n(A) - 1, L ^ R = 0

    // sample bud and trunk lineages
    unsigned budChildIndex = (unsigned)(2 * GLOBAL_RNG->uniform01());
    unsigned trunkChildIndex = (budChildIndex == 0 ? 1 : 0);
    const TopologyNode& budNode = node.getChild(budChildIndex);
    const TopologyNode& trunkNode = node.getChild(trunkChildIndex);
    buddingState[ trunkNode.getIndex() ] = 0;
    buddingState[ budNode.getIndex() ] = 1;

    // sample bud area index
    const std::vector<CharacterEvent*>& nodeState = this->histories[ node.getIndex() ]->getChildCharacters();
    std::vector<unsigned> presentAreas;
    for (size_t i = 0; i < nodeState.size(); i++)
        if (nodeState[i]->getState() == 1)
            presentAreas.push_back(i);

    unsigned budAreaIndex;
    if (presentAreas.size() != 0)
        budAreaIndex = presentAreas[GLOBAL_RNG->uniform01() * presentAreas.size()];
    
    // update child states
    std::vector<CharacterEvent*> budState, trunkState;
    for (size_t i = 0; i < nodeState.size(); i++)
    {
        trunkState.push_back(new CharacterEvent(*nodeState[i]));
        
        // sympatry: iid
        if (cladogenicState[node.getIndex()] == 0 || cladogenicEvents == false)
            budState.push_back(new CharacterEvent(*nodeState[i]));
        
        // peripatry, allopatry: sparse
        else
            budState.push_back(new CharacterEvent(i,0,0.0));
    }
    
    // peripatry: add bud area
    if (cladogenicState[node.getIndex()] == 1)
    {
        budState[budAreaIndex]->setState(1);
    }
    
    // allopatry: add bud area, del trunk area
    else if (cladogenicState[node.getIndex()] == 2)
    {
        trunkState[budAreaIndex]->setState(0);
        budState[budAreaIndex]->setState(1);
    }
    
    this->histories[ trunkNode.getIndex() ]->setParentCharacters( trunkState );
    this->histories[ budNode.getIndex() ]->setParentCharacters( budState );
    
    // sample sub-root state as necessary
    if (node.isRoot())
    {
        TransitionProbabilityMatrix nodeTpMatrix(this->numSites);
        

        
        // for sampling probs
        const std::vector<CharacterEvent*>& nodeChildState = this->histories[node.getIndex() ]->getChildCharacters();

        // to update
        std::vector<CharacterEvent*> nodeParentState = this->histories[node.getIndex()]->getParentCharacters();
        for (std::set<size_t>::iterator it = indexSet.begin(); it != indexSet.end(); it++)
        {
            homogeneousRateMap->getValue().calculateTransitionProbabilities(node, nodeTpMatrix, *it);
            unsigned int desS1 = nodeChildState[*it]->getState();
            
            double u = GLOBAL_RNG->uniform01();
            double g0 = nodeTpMatrix[0][desS1];
            double g1 = nodeTpMatrix[1][desS1];
            
            unsigned int s = 0;
            if (u < g1 / (g0 + g1))
                s = 1;
            
//            std::cout << s;
            
            nodeParentState[*it]->setState(s);
        }
//        std::cout << "\n";
        

        // forbid extinction
        if (numOn(nodeParentState) == 0 && forbidExtinction)
            return false;
//            samplePathStart(node, indexSet);
        else
            this->histories[node.getIndex()]->setParentCharacters(nodeParentState);
        
    }

        
    return true;
}

template<class charType, class treeType>
bool RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::samplePathEnd(const TopologyNode& node, const std::set<size_t>& indexSet)
{
//    double lnP = 0.0;
    
    if (node.isTip())
    {
        if (this->usingAmbiguousCharacters)
        {
            std::vector<CharacterEvent*> childState = this->histories[node.getIndex()]->getChildCharacters();
            for (std::set<size_t>::iterator it = indexSet.begin(); it != indexSet.end(); it++)
            {
                double u = GLOBAL_RNG->uniform01();
                unsigned s = ( u < tipProbs[node.getIndex()][*it] ? 1 : 0);
                childState[*it]->setState(s);
            }
      
            // forbid extinction
            if (numOn(childState) == 0 && forbidExtinction)
                samplePathEnd(node, indexSet);
        }
        else
        {
            ; // do nothing...
        }
    }
    else
    {
        TransitionProbabilityMatrix leftTpMatrix(this->numChars);
        TransitionProbabilityMatrix rightTpMatrix(this->numChars);
        TransitionProbabilityMatrix ancTpMatrix(this->numChars);
        
        const RateMap& rm = homogeneousRateMap->getValue();
        
        // for sampling probs
        const std::vector<CharacterEvent*>& leftChildState  = this->histories[node.getChild(0).getIndex()]->getChildCharacters();
        const std::vector<CharacterEvent*>& rightChildState = this->histories[node.getChild(1).getIndex()]->getChildCharacters();
                
        // to update
        std::vector<CharacterEvent*> nodeChildState = this->histories[node.getIndex()]->getChildCharacters();
        for (std::set<size_t>::iterator it = indexSet.begin(); it != indexSet.end(); it++)
        {
            rm.calculateTransitionProbabilities(node.getChild(0), leftTpMatrix, *it);
            rm.calculateTransitionProbabilities(node.getChild(1), rightTpMatrix, *it);
            rm.calculateTransitionProbabilities(node, ancTpMatrix, *it);
            
            unsigned int desS1 = leftChildState[*it]->getState();
            unsigned int desS2 = rightChildState[*it]->getState();
            unsigned int ancS = (unsigned)(GLOBAL_RNG->uniform01() * 2);
            
            double u = GLOBAL_RNG->uniform01();
            double g0 = leftTpMatrix[0][desS1] * rightTpMatrix[0][desS2] * ancTpMatrix[ancS][0]; // mul by ancTpMatrix[uar][s] to enforce epochs
            double g1 = leftTpMatrix[1][desS1] * rightTpMatrix[1][desS2] * ancTpMatrix[ancS][1];

//            std::cout << desS1 << " " << desS2 << " " << ancS << " " << g0 << " " << g1 << "\n";
            unsigned int s = 0;
            if (u < g1 / (g0 + g1))
                s = 1;
            
//            std::cout << s;
            
            nodeChildState[*it]->setState(s);
            
            
            ;
        }
//        std::cout << "\n";
        
        // forbid extinction
        if (numOn(nodeChildState) == 0 && forbidExtinction)
        {
            return false;
        }
        
    }
    return true;
}





template<class charType, class treeType>
bool RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::samplePathHistory(const TopologyNode& node, const std::set<size_t>& indexSet)
{
    
//    if (redrawCount > 10) {
//        redrawCount = 0;
//        return false;
//    }
    
    if (node.isRoot())
        return true;
    
    // get model parameters
//    const treeType& tree = this->tau->getValue();
    double branchLength = node.getBranchLength();
    const RateMap& rm = homogeneousRateMap->getValue();

    // begin update
    BranchHistory* bh = this->histories[ node.getIndex() ];
    
    // get epoch variables
    double startAge = node.getParent().getAge();
//    double endAge = node.getAge();
    
    // reject sample path history
    std::vector<CharacterEvent*> parentVector = bh->getParentCharacters();
    std::vector<CharacterEvent*> childVector =  bh->getChildCharacters();
    std::multiset<CharacterEvent*,CharacterEventCompare> history;

    for (std::set<size_t>::iterator it = indexSet.begin(); it != indexSet.end(); it++)
    {
        std::set<CharacterEvent*> tmpHistory;
        unsigned int currState = parentVector[*it]->getState();
        unsigned int endState = childVector[*it]->getState();
        do
        {
            // delete previously rejected events
            tmpHistory.clear();
            
            // proceed with rejection sampling
            currState = parentVector[*it]->getState();
            
            double t = 0.0;
            
            double currAge = startAge;
            int epochIdx = getEpochIndex(startAge);
            double epochAge = epochs[epochIdx];
            
            // repeated rejection sampling
            do
            {
                unsigned int nextState = (currState == 1 ? 0 : 1);
                unsigned charIdx = (unsigned)(*it);
                double r = rm.getSiteRate(node, currState, nextState, charIdx, currAge);
                
                double dt = 0.0;
                if (r > 0.0)
                    dt = RbStatistics::Exponential::rv(r * branchLength, *GLOBAL_RNG);
                
                double da = dt * branchLength;
                
                // sample time from next interval (by memorylessness)
                if (currAge - da < epochAge || r == 0.0)
                {
                    t = (startAge - epochAge) / branchLength;
                    currAge = epochAge;
                    epochIdx++;
                    epochAge = epochs[epochIdx];
                }
                else
                {
                    t += dt;
                    currAge -= da;
                    
                    if (t < 1.0)
                    {
                        currState = nextState;
                        CharacterEvent* evt = new CharacterEvent(*it, nextState, t);
                        tmpHistory.insert(evt);
                    }
                    else if (currState != endState)
                    {
                        for (std::set<CharacterEvent*>::iterator it_h = tmpHistory.begin(); it_h != tmpHistory.end(); it_h++)
                            delete *it_h;
                    }
                }
            }
            while(t < 1.0);
        }
        while (currState != endState);
        
        for (std::set<CharacterEvent*>::iterator it = tmpHistory.begin(); it != tmpHistory.end(); it++)
        {
            history.insert(*it);
        }
    }
    
//    bh->print();
    
    if (historyContainsExtinction(parentVector, history) == true && forbidExtinction)
    {
        for (std::multiset<CharacterEvent*,CharacterEventCompare>::iterator it_h = history.begin(); it_h != history.end(); it_h++)
            delete *it_h;
        history.clear();
        bh->clearEvents();
   
        return false;
//        redrawCount++;
//        samplePathHistory(node, indexSet);
    }
    else
    {
        bh->updateHistory(history,indexSet);
        return true;
//        bh->print();
    }

    
    // return hastings ratio
//    this->proposedLnProb = computeLnProposal(*(this->node), *bh);
    
//    return this->storedLnProb - this->proposedLnProb;

    
//    // get model parameters
//    const treeType& tree = this->tau->getValue();
//    double branchLength = node.getBranchLength();
//
//    const RateMap* rm = &homogeneousRateMap->getValue();
// 
//    // begin update
//    BranchHistory* h = this->histories[ node.getIndex() ];
//    
//    // reject sample path history
//    std::vector<CharacterEvent*> parentVector = h->getParentCharacters();
//    std::vector<CharacterEvent*> childVector = h->getChildCharacters();
//    std::multiset<CharacterEvent*,CharacterEventCompare> history;
//    
//    for (std::set<size_t>::iterator it = indexSet.begin(); it != indexSet.end(); it++)
//    {
//        size_t i = *it;
//        std::set<CharacterEvent*> tmpHistory;
//    
//        unsigned int currState = parentVector[i]->getState();
//        unsigned int endState = childVector[i]->getState();
//        do
//        {
//            // delete previously rejected events
//            tmpHistory.clear();
//            
//            currState = parentVector[i]->getState();
//            double currAge = node.getParent().getAge();
//            double t = 0.0;
//            do
//            {
//                unsigned int nextState = (currState == 1 ? 0 : 1);
//                double r = rm->getSiteRate(node, currState, nextState, currAge);
//                
//                double dt = RbStatistics::Exponential::rv(r * branchLength, *GLOBAL_RNG);
//                t += dt;
//                currAge -= dt * branchLength;
//                if (t < 1.0)
//                {
//                    currState = nextState;
//                    CharacterEvent* evt = new CharacterEvent(i , nextState, t);
//                    tmpHistory.insert(evt);
//                }
//                else if (currState != endState)
//                {
//                    for (std::set<CharacterEvent*>::iterator it_h = tmpHistory.begin(); it_h != tmpHistory.end(); it_h++)
//                        delete *it_h;
//                }
//            }
//            while(t < 1.0);
//        }
//        while (currState != endState);
//        
//        for (std::set<CharacterEvent*>::iterator it = tmpHistory.begin(); it != tmpHistory.end(); it++)
//            history.insert(*it);
//    }
//    
//    if (historyContainsExtinction(parentVector, history) == true && forbidExtinction)
//    {
//        for (std::multiset<CharacterEvent*,CharacterEventCompare>::iterator it_h = history.begin(); it_h != history.end(); it_h++)
//            delete *it_h;
//        history.clear();
//        h->clearEvents();
//        //std::cout << node.getIndex() << " do over\n";
//        redrawCount++;
//        samplePathHistory(node, indexSet);
//    }
//    else
//    {
//        h->updateHistory(history,indexSet);
//    }
//    return 0.0;
}

template<class charType, class treeType>
bool RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::historyContainsExtinction(const std::vector<CharacterEvent*>& currState, const std::multiset<CharacterEvent*,CharacterEventCompare>& history)
{
    std::multiset<CharacterEvent*,CharacterEventCompare>::iterator it_h;
    int n = (int)numOn(currState);
    if (n == 0)
        return true;
    
    for (it_h = history.begin(); it_h != history.end(); it_h++)
    {    
        if ( (*it_h)->getState() == 0 )
            n--;
        else if ( (*it_h)->getState() == 1 )
            n++;
        
        if (n <= 0)
            return true;
    }
    
    return false;
}

template<class charType, class treeType>
void RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::setRateMap(const TypedDagNode< RateMap > *rm) {
    
    // remove the old parameter first
    if ( homogeneousRateMap != NULL )
    {
        this->removeParameter( homogeneousRateMap );
        homogeneousRateMap = NULL;
    }
    else
    {
        this->removeParameter( heterogeneousRateMaps );
        heterogeneousRateMaps = NULL;
    }
    
    // set the value
    branchHeterogeneousSubstitutionMatrices = false;
    homogeneousRateMap = rm;
    epochs = static_cast<const RateMap_Biogeography&>(rm->getValue()).getEpochs();

    // add the parameter
    this->addParameter( homogeneousRateMap );
    
    // redraw the current value
    if ( this->dagNode != NULL && !this->dagNode->isClamped() )
    {
        this->redrawValue();
    }
    
}


template<class charType, class treeType>
void RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::setRateMap(const TypedDagNode< RbVector< RateMap > > *rm) {
    
    // remove the old parameter first
    if ( homogeneousRateMap != NULL )
    {
        this->removeParameter( homogeneousRateMap );
        homogeneousRateMap = NULL;
    }
    else
    {
        this->removeParameter( heterogeneousRateMaps );
        heterogeneousRateMaps = NULL;
    }
    
    // set the value
    branchHeterogeneousSubstitutionMatrices = true;
    heterogeneousRateMaps = rm;
    epochs = static_cast<const RateMap_Biogeography&>(rm->getValue()[0]).getEpochs();
    
    // add the parameter
    this->addParameter( heterogeneousRateMaps );
    
    // redraw the current value
    if ( this->dagNode != NULL && !this->dagNode->isClamped() )
    {
        this->redrawValue();
    }
    
}


template<class charType, class treeType>
void RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::setRootFrequencies(const TypedDagNode< std::vector< double > > *f) {
    
    // remove the old parameter first
    if ( rootFrequencies != NULL )
    {
        this->removeParameter( rootFrequencies );
        rootFrequencies = NULL;
    }
    
    if ( f != NULL )
    {
        // set the value
        //        branchHeterogeneousSubstitutionMatrices = true;
        rootFrequencies = f;
        
        // add the parameter
        this->addParameter( rootFrequencies );
    }
    else
    {
        branchHeterogeneousSubstitutionMatrices = false;
    }
    
    // redraw the current value
    if ( this->dagNode != NULL && !this->dagNode->isClamped() )
    {
        this->redrawValue();
    }
}


template<class charType, class treeType>
void RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::setSiteRates(const TypedDagNode< std::vector< double > > *r) {
    
    // remove the old parameter first
    if ( siteRates != NULL )
    {
        this->removeParameter( siteRates );
        siteRates = NULL;
    }
    
    if ( r != NULL )
    {
        // set the value
        rateVariationAcrossSites = true;
        siteRates = r;
        this->numSiteRates = r->getValue().size();
        this->resizeLikelihoodVectors();
    }
    else
    {
        // set the value
        rateVariationAcrossSites = false;
        siteRates = NULL;
        this->numSiteRates = 1;
        this->resizeLikelihoodVectors();
        
    }
    
    // add the parameter
    this->addParameter( siteRates );
    
    // redraw the current value
    if ( this->dagNode != NULL && !this->dagNode->isClamped() )
    {
        this->redrawValue();
    }
}

template<class charType, class treeType>
const std::vector<std::vector<double> >& RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::getTipProbs(void)
{
    return tipProbs;
}

template<class charType, class treeType>
const std::vector<double>& RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::getTipProbs(const TopologyNode& nd)
{
    return tipProbs[nd.getIndex()];
}


template<class charType, class treeType>
void RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::setTipProbs(const AbstractCharacterData* tp)
{
    tipProbs.clear();
    
    size_t numTaxa = tp->getNumberOfTaxa();
    size_t numCharacters = tp->getNumberOfCharacters();
    
    const std::vector<TopologyNode*>& nodes = this->tau->getValue().getNodes();
    
    tipProbs.resize(numTaxa);
    const ContinuousCharacterData* ccdp = static_cast<const ContinuousCharacterData*>(tp);
    for (size_t i = 0; i < nodes.size(); i++)
    {
        TopologyNode* nd = nodes[i];
        if (!nd->isTip())
            continue;
        
        const ContinuousTaxonData* cd = &ccdp->getTaxonData(nd->getName());
        for (size_t j = 0; j < numCharacters; j++)
        {
            double v = cd->getCharacter(j).getMean();
            //tipProbs[nd->getIndex()].push_back(1-v);
            tipProbs[nd->getIndex()].push_back(v);
        }
    }
}

template<class charType, class treeType>
const std::vector<int>& RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::getBuddingStates(void)
{
    return buddingState;
}

template<class charType, class treeType>
const std::vector<int>& RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::getCladogenicStates(void)
{
    return cladogenicState;
}


template<class charType, class treeType>
int RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::getBuddingState(const TopologyNode& nd)
{
    return buddingState[nd.getIndex()];
}

template<class charType, class treeType>
int RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::getCladogenicState(const TopologyNode& nd)
{
    return cladogenicState[nd.getIndex()];
}

template<class charType, class treeType>
void RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::setBuddingState(const TopologyNode& nd, int s)
{
    buddingState[nd.getIndex()] = s;
}

template<class charType, class treeType>
void RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::setCladogenicState(const TopologyNode& nd, int s)
{
    cladogenicState[nd.getIndex()] = s;
}

template<class charType, class treeType>
void RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::simulate(void)
{
    ;
}

template<class charType, class treeType>
//void RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::simulate(const TopologyNode& node, std::vector< DiscreteTaxonData< charType > > &taxa)
void RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::simulate(const TopologyNode& node, DiscreteCharacterData< charType > &taxa)
{
    RandomNumberGenerator* rng = GLOBAL_RNG;
    const treeType& tree = this->tau->getValue();
    
    // get the children of the node
    const std::vector<TopologyNode*>& children = node.getChildren();
    
    // get the sequence of this node
    size_t nodeIndex = node.getIndex();
    const DiscreteTaxonData< charType > &parent = taxa[ nodeIndex ];
    
    // simulate the sequence for each child
    for (std::vector< TopologyNode* >::const_iterator it = children.begin(); it != children.end(); ++it)
    {
        const TopologyNode &child = *(*it);
        BranchHistory bh(this->numChars, this->numSites, nodeIndex);
        
        // get parent BranchHistory state
        std::vector<CharacterEvent*> currState;
        if (node.isRoot())
        {
            int n = 0;
            while (n == 0)
            {
                n = 0;
                for (size_t i = 0; i < this->numSites; i++)
                {
                    unsigned s = (rng->uniform01() > 0.5 ? 1 : 0);
                    currState.push_back(new CharacterEvent(i, s, 0.0));
                    if (s == 1)
                        n++;
                }
            }

        }
        else
        {
            for (size_t i = 0; i < this->numSites; ++i)
            {
                unsigned s = (unsigned)parent.getCharacter(i).getState();
                currState.push_back(new CharacterEvent(i, s, 0.0));
            }
        }
        
        // simulate path
        double bt = tree.getBranchLength(nodeIndex);
        double br = 1.0;
//        if (branchHeterogeneousClockRates)
//            br = heterogeneousClockRates->getValue()[nodeIndex];
//        else
//            br = homogeneousClockRate->getValue();
//        if (bt == 0.0)
//            bt = 100.0;
        double bs = br * bt;
        
        const RateMap* rm;
        if (branchHeterogeneousSubstitutionMatrices)
            rm = &heterogeneousRateMaps->getValue()[nodeIndex];
        else
            rm = &homogeneousRateMap->getValue();

        // simulate to end state
        std::set<CharacterEvent*,CharacterEventCompare> history;
        
        double t = 0.0;
        unsigned int n = (unsigned)numOn(currState);
        do
        {
            // sample next event time
            double r_sum = rm->getSumOfRates(node, currState);
            t += RbStatistics::Exponential::rv( r_sum * bs, *GLOBAL_RNG );
            
            if (t < 1.0)
            {
                // sample next event index
                double u = GLOBAL_RNG->uniform01() * r_sum;
                for (size_t i = 0; i < this->numChars; i++)
                {
                    unsigned int s = (currState[i]->getState() == 0 ? 1 : 0);
                    
                    // forbid extinction
                    if (n == 1 && s == 0)
                        continue;
                    
                    CharacterEvent* evt = new CharacterEvent(i,s,t);
                    double tx_rate = rm->getRate(node, currState, evt);
                    u -= tx_rate;
                    
                    //std::cout << "\tu " << u << ": " << i << " " << s << " " << t << " " << r_sum << " " << n << " " << tx_rate << " " << bt << " " << br << "\n";
                    if (u <= 0.0)
                    {
                        currState[i] = evt;
                        history.insert(evt);
                        
                        // check how many areas on
                        if (s == 0)
                            n--;
                        else
                            n++;
                        break;
                    }
                }
            }
        }
        while (t < 1.0);
        
        
        // update child state
        
        //DiscreteTaxonData< charType > &taxon = taxa.getTaxonData( child.getIndex() ); //taxa[ child.getIndex() ];
        DiscreteTaxonData<charType>* taxon = new DiscreteTaxonData<charType>();
        for ( size_t i = 0; i < this->numSites; ++i )
        {
            // create the character
            charType c;
            c.setToFirstState();
            for (size_t j = 0; j < currState[i]->getState(); j++)
                c++;
            
            // add the character to the sequence
            taxon->addCharacter( c );
        }
        
        if ( child.isTip() )
        {
            if (this->usingAmbiguousCharacters)
            {
                ; // apply noise to data
                // requires consideration -- currently impl. is "model-free"
            }
            taxon->setTaxonName( child.getName() );
            taxa.addTaxonData(*taxon);
        }
        else
        {
            // recursively simulate the sequences
            taxa.addTaxonData(*taxon);
            simulate( child, taxa );
        }
    }
}

template<class charType, class treeType>
void RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::swapParameter(const DagNode *oldP, const DagNode *newP) {
    
    if (oldP == homogeneousRateMap)
    {
        homogeneousRateMap = static_cast<const TypedDagNode< RateMap >* >( newP );
    }
    else if (oldP == heterogeneousRateMaps)
    {
        heterogeneousRateMaps = static_cast<const TypedDagNode< RbVector< RateMap > >* >( newP );
    }
    else if (oldP == rootFrequencies)
    {
        rootFrequencies = static_cast<const TypedDagNode< std::vector< double > >* >( newP );
    }
    else if (oldP == siteRates)
    {
        siteRates = static_cast<const TypedDagNode< std::vector< double > >* >( newP );
    }
    else
    {
        AbstractTreeHistoryCtmc<charType, treeType>::swapParameter(oldP,newP);
    }
    
}

template<class charType, class treeType>
void RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::touchSpecialization( DagNode* affecter ) {
    
    // if the topology wasn't the culprit for the touch, then we just flag everything as dirty
    if ( affecter == rootFrequencies || affecter == rootFrequencies )
    {
        
        const TopologyNode &root = this->tau->getValue().getRoot();
        this->flagNodeDirty( root );
    }
    else
    {
        AbstractTreeHistoryCtmc<charType, treeType>::touchSpecialization( affecter );
    }
    
}

template<class charType, class treeType>
const bool RevBayesCore::BiogeographicTreeHistoryCtmc<charType, treeType>::useCladogenicEvents(void) const
{
    return cladogenicEvents;
}

#endif /* defined(__rb_mlandis__BiogeographicTreeHistoryCtmc__) */
