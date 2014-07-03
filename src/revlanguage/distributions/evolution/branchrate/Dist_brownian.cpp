//
//  Dist_brownian.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-22.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//



#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "BrownianPhyloProcess.h"
#include "Dist_brownian.h"
#include "Real.h"
#include "RlTimeTree.h"
#include "StochasticNode.h"
#include "TimeTree.h"
#include "Vector.h"

using namespace RevLanguage;


Dist_brownian* Dist_brownian::clone( void ) const {
    return new Dist_brownian(*this);
}


RevBayesCore::BrownianPhyloProcess* Dist_brownian::createDistribution( void ) const {
    // get the parameters

    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree>* tau = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    
    RevBayesCore::TypedDagNode<double>* s  = static_cast<const Real&>( sigma->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* r  = static_cast<const Real&>( rootval->getRevObject() ).getDagNode();
    
    RevBayesCore::BrownianPhyloProcess* d    = new RevBayesCore::BrownianPhyloProcess( tau, s, r );
    
    return d;

}



/* Get class name of object */
const std::string& Dist_brownian::getClassName(void) {
    
    static std::string rbClassName = "Dist_brownian";
    
	return rbClassName;
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_brownian::getClassTypeSpec(void) {
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass;
}



/** Return member rules (no members) */
const MemberRules& Dist_brownian::getMemberRules(void) const {
    
    static MemberRules dist;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        dist.push_back( new ArgumentRule( "tree" , true, TimeTree::getClassTypeSpec() ) );
        dist.push_back( new ArgumentRule( "sigma", true, RealPos::getClassTypeSpec() ) );
        dist.push_back( new ArgumentRule( "rootval", true, Real::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return dist;
}


const TypeSpec& Dist_brownian::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}




/** Print value for user */

 void Dist_brownian::printValue(std::ostream& o) const {
    
    o << " brownian(";
    
    o << "tau=";
    if ( tree != NULL ) {
        o << tree->getName();
    } else {
        o << "?";
    }

     o << ",";
     
     o << "sigma=";
     if ( sigma != NULL ) {
         o << sigma->getName();
     } else {
         o << "?";
     }

     o << ",";
     
     o << "rootval=";
     if ( rootval != NULL ) {
         o << rootval->getName();
     } else {
         o << "?";
     }

     o << ")";
}



/** Set a member variable */
void Dist_brownian::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {

    if ( name == "tree" )
    {
        tree = var;
    }
    else if ( name == "sigma" )
    {
        sigma = var;
    }
    else if ( name == "rootval" )
    {
        rootval = var;
    }
    else {
        Distribution::setConstMemberVariable(name, var);
    }
}

