/**
 * @file
 * This file contains the implementation of Func_range, which is
 * the function used to compute range (colon) vectors.
 *
 * @brief Implementation of Func_range
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-05-04 18:03:37 +0200 (Fri, 04 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @interface RbFunction
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id: Func_source.cpp 1485 2012-05-04 16:03:37Z hoehna $
 */

#include "Argument.h"
#include "ArgumentRule.h"
#include "Func_range.h"
#include "Integer.h"
#include "RbException.h"
#include "TypeSpec.h"
#include "Vector.h"

using namespace RevLanguage;

/** Default constructor */
Func_range::Func_range( void ) : Function() {
    
}


/** Clone object */
Func_range* Func_range::clone( void ) const {
    
    return new Func_range( *this );
}


/** Execute function */
RevObject* Func_range::execute( void ) {
    
    int f = static_cast<const Integer &>( args[0].getVariable()->getRevObject() ).getValue();
    int l = static_cast<const Integer &>( args[1].getVariable()->getRevObject() ).getValue();
    
    Vector<Integer> *range = new Vector<Integer>();
    if (f < l) {
        for ( int i = f; i <= l; i++ )
            range->push_back( Integer(i) );
    }
    else {
        for ( int i = f; i >= l; i-- )
            range->push_back( Integer(i) );
    }
    
    return range;
}


/** Get argument rules */
const ArgumentRules& Func_range::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if (!rulesSet) {
        
        argumentRules.push_back( new ArgumentRule( "first", true, Integer::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "last",  true, Integer::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_range::getClassName(void) { 
    
    static std::string rbClassName = "Func_range";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_range::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_range::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_range::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = Vector<Integer>::getClassTypeSpec();
    
    return returnTypeSpec;
}

