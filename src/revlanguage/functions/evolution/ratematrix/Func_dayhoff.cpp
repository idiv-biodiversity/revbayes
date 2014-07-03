//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "DeterministicNode.h"
#include "Func_dayhoff.h"
#include "Natural.h"
#include "RateMatrix_Dayhoff.h"
#include "Real.h"
#include "RealPos.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_dayhoff::Func_dayhoff( void ) : Function( ) {
    
}


/** Clone object */
Func_dayhoff* Func_dayhoff::clone( void ) const {
    
    return new Func_dayhoff( *this );
}


RevObject* Func_dayhoff::execute() {
    
    
    RevBayesCore::RateMatrix_Dayhoff *rmj = new RevBayesCore::RateMatrix_Dayhoff();
    RateMatrix* value = new RateMatrix( rmj );
    
    return value;
}


/* Get argument rules */
const ArgumentRules& Func_dayhoff::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    
    return argumentRules;
}


const std::string& Func_dayhoff::getClassName(void) { 
    
    static std::string rbClassName = "Func_dayhoff";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_dayhoff::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
const TypeSpec& Func_dayhoff::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RateMatrix::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_dayhoff::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
