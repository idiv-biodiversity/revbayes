/**
 * @file
 * This file contains the implementation of Func_clear, which is
 * the function used to list the content of the workspace.
 *
 * @brief Implementation of Func_clear
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @interface RbFunction
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id$
 */

#include "Func_clear.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RealPos.h"
#include "TypeSpec.h"
#include "VectorString.h"
#include "Workspace.h"

#include <fstream>
#include <memory>


// Definition of the static type spec member
const TypeSpec Func_clear::typeSpec(Func_clear_name);
const TypeSpec Func_clear::returnTypeSpec(RbVoid_name);

/** Clone object */
Func_clear* Func_clear::clone( void ) const {
    
    return new Func_clear( *this );
}


/** Execute function */
RbLanguageObject* Func_clear::execute( void ) {
    
    auto_ptr<RealPos> test(new RealPos(1.0));
    
    // clear the user workspace
    Workspace::userWorkspace().clear();
    
    return NULL;
}


/** Get argument rules */
const ArgumentRules& Func_clear::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules;
    
    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func_clear::getClass( void ) const {
    
    static VectorString rbClass = VectorString( Func_clear_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec& Func_clear::getReturnType( void ) const {
    
    return returnTypeSpec;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Func_clear::getTypeSpec(void) const {
    return typeSpec;
}

