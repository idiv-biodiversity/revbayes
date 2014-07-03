/**
 * @file
 * This file contains the implementation of RlRnaState, which is
 * a RevBayes wrapper around a regular bool.
 *
 * @brief Implementation of RlRnaState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-09-04 20:14:58 +0200 (Tue, 04 Sep 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id: RlBoolean.cpp 1793 2012-09-04 18:14:58Z hoehna $
 */


#include "ConstantNode.h"
#include "RlRnaState.h"
#include "RbUtil.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
RnaState::RnaState(void) : ModelObject<RevBayesCore::RnaState>() {
    
}

/** Construct from bool */
RnaState::RnaState(const RevBayesCore::RnaState &d) : ModelObject<RevBayesCore::RnaState>( new RevBayesCore::RnaState(d) ) {
    
}


/** Clone object */
RnaState* RnaState::clone(void) const {
    
	return new RnaState(*this);
}


/** Convert to type. The caller manages the returned object. */
RevObject* RnaState::convertTo(const TypeSpec& type) const {
    
    return RevObject::convertTo(type);
}


/** Get class name of object */
const std::string& RnaState::getClassName(void) { 
    
    static std::string rbClassName = "RNA";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& RnaState::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& RnaState::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Is convertible to type? */
bool RnaState::isConvertibleTo(const TypeSpec& type) const {
    
    return RevObject::isConvertibleTo(type);
}

