//
//  NormalDistribution.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "BernoulliDistribution.h"
#include "ContinuousStochasticNode.h"
#include "Dist_bernoulli.h"
#include "Probability.h"
#include "RealPos.h"

using namespace RevLanguage;

Dist_bernoulli::Dist_bernoulli() : TypedDistribution<Natural>() {
    
}


Dist_bernoulli::~Dist_bernoulli() {
    
}



Dist_bernoulli* Dist_bernoulli::clone( void ) const {
    
    return new Dist_bernoulli(*this);
}


RevBayesCore::BernoulliDistribution* Dist_bernoulli::createDistribution( void ) const {
    
    // get the parameters
    RevBayesCore::TypedDagNode<double>* q   = static_cast<const Probability &>( p->getRevObject() ).getDagNode();
    RevBayesCore::BernoulliDistribution* d  = new RevBayesCore::BernoulliDistribution( q );
    
    return d;
}



/* Get class name of object */
const std::string& Dist_bernoulli::getClassName(void) { 
    
    static std::string rbClassName = "Dist_bernoulli";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_bernoulli::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass; 
}




/** Return member rules (no members) */
const MemberRules& Dist_bernoulli::getMemberRules(void) const {
    
    static MemberRules distBernMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distBernMemberRules.push_back( new ArgumentRule( "p", true, Probability::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return distBernMemberRules;
}


const TypeSpec& Dist_bernoulli::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_bernoulli::printValue(std::ostream& o) const {
    
    o << "Bernoulli(p=";
    if ( p != NULL ) {
        o << p->getName();
    } else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Dist_bernoulli::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
        
    if ( name == "p" ) 
    {
        p = var;
    }
    else
    {
        TypedDistribution<Natural>::setConstMemberVariable(name, var);
    }
}
