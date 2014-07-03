//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "DeterministicNode.h"
#include "Func_mtMam.h"
#include "Natural.h"
#include "RateMatrix_MtMam.h"
#include "Real.h"
#include "RealPos.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_mtMam::Func_mtMam( void ) : Function( ) {
    
}


/** Clone object */
Func_mtMam* Func_mtMam::clone( void ) const {
    
    return new Func_mtMam( *this );
}


RevObject* Func_mtMam::execute() {
    
    
    RevBayesCore::RateMatrix_MtMam *rmj = new RevBayesCore::RateMatrix_MtMam();
    RateMatrix* value = new RateMatrix( rmj );
    
    return value;
}


/* Get argument rules */
const ArgumentRules& Func_mtMam::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    
    return argumentRules;
}


const std::string& Func_mtMam::getClassName(void) { 
    
    static std::string rbClassName = "Func_mtMam";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_mtMam::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
const TypeSpec& Func_mtMam::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RateMatrix::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_mtMam::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
