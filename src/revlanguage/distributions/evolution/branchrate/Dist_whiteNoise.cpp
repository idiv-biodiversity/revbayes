//
//  RlWhiteNoiseProcess.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-22.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//



#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Dist_whiteNoise.h"
#include "Real.h"
#include "RealPos.h"
#include "RlTimeTree.h"
#include "StochasticNode.h"
#include "TimeTree.h"
#include "Vector.h"
#include "WhiteNoisePhyloProcess.h"

using namespace RevLanguage;

Dist_whiteNoise::Dist_whiteNoise() : TypedDistribution< Vector<RealPos> >() {
    
}


Dist_whiteNoise::~Dist_whiteNoise() {
    
}



Dist_whiteNoise* Dist_whiteNoise::clone( void ) const {
    return new Dist_whiteNoise(*this);
}


RevBayesCore::WhiteNoisePhyloProcess* Dist_whiteNoise::createDistribution( void ) const {
    // get the parameters

    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree>* tau = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    
    RevBayesCore::TypedDagNode<double>* s  = static_cast<const RealPos &>( sigma->getRevObject() ).getDagNode();
    
    RevBayesCore::WhiteNoisePhyloProcess* d    = new RevBayesCore::WhiteNoisePhyloProcess( tau, s );
    
    return d;

}



/* Get class name of object */
const std::string& Dist_whiteNoise::getClassName(void) {
    
    static std::string rbClassName = "Dist_whiteNoise";
    
	return rbClassName;
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_whiteNoise::getClassTypeSpec(void) {
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass;
}



/** Return member rules (no members) */
const MemberRules& Dist_whiteNoise::getMemberRules(void) const {
    
    static MemberRules dist;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        dist.push_back( new ArgumentRule( "tree" , true, TimeTree::getClassTypeSpec() ) );
        dist.push_back( new ArgumentRule( "sigma", true, RealPos::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return dist;
}


const TypeSpec& Dist_whiteNoise::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}




/** Print value for user */

 void Dist_whiteNoise::printValue(std::ostream& o) const {
    
    o << " whitenoise(";
    
    o << "tau=";
    if ( tree != NULL ) {
        o << tree->getName();
    } else {
        o << "?";
    }
    
    o << "sigma=";
    if ( sigma != NULL ) {
        o << sigma->getName();
    } else {
        o << "?";
    }
    o << ")";
}



/** Set a member variable */
void Dist_whiteNoise::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {

    if ( name == "tree" )
    {
        tree = var;
    }
    else if ( name == "sigma" )
    {
        sigma = var;
    }
    else {
        Distribution::setConstMemberVariable(name, var);
    }
}

