//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "DeterministicNode.h"
#include "Func_cpRev.h"
#include "Natural.h"
#include "RateMatrix_CpRev.h"
#include "Real.h"
#include "RealPos.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_cpRev::Func_cpRev( void ) : Function( ) {
    
}


/** Clone object */
Func_cpRev* Func_cpRev::clone( void ) const {
    
    return new Func_cpRev( *this );
}


RevObject* Func_cpRev::execute() {
    
    
    RevBayesCore::RateMatrix_CpRev *rmj = new RevBayesCore::RateMatrix_CpRev();
    RateMatrix* value = new RateMatrix( rmj );
    
    return value;
}


/* Get argument rules */
const ArgumentRules& Func_cpRev::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    
    return argumentRules;
}


const std::string& Func_cpRev::getClassName(void) { 
    
    static std::string rbClassName = "Func_cpRev";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_cpRev::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
const TypeSpec& Func_cpRev::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RateMatrix::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_cpRev::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
