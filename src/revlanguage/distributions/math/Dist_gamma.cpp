//
//  NormalDistribution.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Dist_gamma.h"
#include "GammaDistribution.h"
#include "Real.h"
#include "RealPos.h"

using namespace RevLanguage;

Dist_gamma::Dist_gamma() : PositiveContinuousDistribution() {
    
}


Dist_gamma::~Dist_gamma() {
    
}



Dist_gamma* Dist_gamma::clone( void ) const {
    
    return new Dist_gamma(*this);
}


RevBayesCore::GammaDistribution* Dist_gamma::createDistribution( void ) const {
    // get the parameters
    RevBayesCore::TypedDagNode<double>* sh  = static_cast<const RealPos &>( shape->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* r   = static_cast<const RealPos &>( rate->getRevObject() ).getDagNode();
    RevBayesCore::GammaDistribution* d      = new RevBayesCore::GammaDistribution(sh, r);
    
    return d;
}



/* Get class name of object */
const std::string& Dist_gamma::getClassName(void) { 
    
    static std::string rbClassName = "Dist_gamma";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_gamma::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass; 
}




/** Return member rules (no members) */
const MemberRules& Dist_gamma::getMemberRules(void) const {
    
    static MemberRules distGammaMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distGammaMemberRules.push_back( new ArgumentRule( "shape", true, RealPos::getClassTypeSpec() ) );
        distGammaMemberRules.push_back( new ArgumentRule( "rate" , true, RealPos::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return distGammaMemberRules;
}


const TypeSpec& Dist_gamma::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_gamma::printValue(std::ostream& o) const {
    
    o << "gamma(shape=";
    if ( shape != NULL ) 
    {
        o << shape->getName();
    } 
    else 
    {
        o << "?";
    }
    o << ", rate=";
    if ( rate != NULL ) 
    {
        o << rate->getName();
    } 
    else 
    {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Dist_gamma::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "shape" ) 
    {
        shape = var;
    }
    else if ( name == "rate" ) 
    {
        rate = var;
    }
    else  
    {
        Distribution::setConstMemberVariable(name, var);
    }
}
