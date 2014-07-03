//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Clade.h"
#include "ConstantNode.h"
#include "DeterministicNode.h"
#include "Func_dppMeanFromConc.h"
#include "RbStatisticsHelper.h"
#include "RlClade.h"
#include "RlTimeTree.h"
#include "RateMatrix.h"
#include "RealPos.h"
#include "Topology.h"
#include "TypedDagNode.h"
#include "Vector.h"

using namespace RevLanguage;

/** default constructor */
Func_dppMeanFromConc::Func_dppMeanFromConc( void ) : Function( ) {
    
}


/** Clone object */
Func_dppMeanFromConc* Func_dppMeanFromConc::clone( void ) const {
    
    return new Func_dppMeanFromConc( *this );
}


RevObject* Func_dppMeanFromConc::execute() {
	
    double cp = static_cast<const RealPos &>( args[0].getVariable()->getRevObject() ).getValue();
    double ne = static_cast<const RealPos &>( args[1].getVariable()->getRevObject() ).getValue();
	
	double meanCP = RevBayesCore::RbStatistics::Helper::dppExpectNumTableFromConcParam(cp, ne);
    RevBayesCore::ConstantNode<double> *constNode = new RevBayesCore::ConstantNode<double>("", new double(meanCP));
	
	RealPos* value = new RealPos( constNode );
	return value;
	
}


/* Get argument rules */
const ArgumentRules& Func_dppMeanFromConc::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "concentration", true, RealPos::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "num_elements", true, RealPos::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_dppMeanFromConc::getClassName(void) { 
    
    static std::string rbClassName = "Func_dppMeanFromConc";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_dppMeanFromConc::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
const TypeSpec& Func_dppMeanFromConc::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RealPos::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_dppMeanFromConc::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
