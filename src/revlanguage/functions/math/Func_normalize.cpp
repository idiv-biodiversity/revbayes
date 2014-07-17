//
//  Func_normalize.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 9/8/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "ArgumentRule.h"
#include "Ellipsis.h"
#include "Func_normalize.h"
#include "NormalizeVectorFunction.h"
#include "RbUtil.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "Vector.h"


using namespace RevLanguage;

Func_normalize::Func_normalize() : Function() {
    
}

/* Clone object */
Func_normalize* Func_normalize::clone( void ) const {
    
    return new Func_normalize( *this );
}


/** Execute function: We rely on getValue and overloaded push_back to provide functionality */
RevObject* Func_normalize::execute( void ) {
    
    const RevBayesCore::TypedDagNode< std::vector<double> > *params = static_cast< Vector<RealPos> & >( args[0].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::NormalizeVectorFunction *func = new RevBayesCore::NormalizeVectorFunction( params );
    
    DeterministicNode<std::vector<double> > *detNode = new DeterministicNode<std::vector<double> >("", func, this->clone());

    Vector<RealPos> *theNormalizedVector = new Vector<RealPos>( detNode );
    
    return theNormalizedVector;
}


/** Get argument rules */
const ArgumentRules& Func_normalize::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "x", true, Vector<RealPos>::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_normalize::getClassName(void) { 
    
    static std::string rbClassName = "Func_normalize";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
const RevLanguage::TypeSpec& Func_normalize::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
const TypeSpec& Func_normalize::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_normalize::getReturnType( void ) const {
    
    return Vector<RealPos>::getClassTypeSpec();
}
