//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "DeterministicNode.h"
#include "Func_jc.h"
#include "JcRateMatrixFunction.h"
#include "Natural.h"
#include "RateMatrix_JC.h"
#include "Real.h"
#include "RealPos.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_jc::Func_jc( void ) : Function( ) {
    
}


/** Clone object */
Func_jc* Func_jc::clone( void ) const {
    
    return new Func_jc( *this );
}


RevObject* Func_jc::execute() {
    
    int ns = static_cast<const Natural &>( this->args[0].getVariable()->getRevObject() ).getValue();
    RevBayesCore::JcRateMatrixFunction* f = new RevBayesCore::JcRateMatrixFunction( size_t(ns) );
    RevBayesCore::DeterministicNode<RevBayesCore::RateMatrix> *detNode = new RevBayesCore::DeterministicNode<RevBayesCore::RateMatrix>("", f);
    
    RateMatrix* value = new RateMatrix( detNode );
    
    return value;
}


/* Get argument rules */
const ArgumentRules& Func_jc::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "numStates", true, Natural::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_jc::getClassName(void) { 
    
    static std::string rbClassName = "Func_jc";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_jc::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
const TypeSpec& Func_jc::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RateMatrix::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_jc::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
