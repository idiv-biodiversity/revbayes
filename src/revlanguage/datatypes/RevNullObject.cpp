/**
 * @file
 * This file contains the declaration of RevNullObject, which is
 * the RevBayes class used for NULL references.
 *
 * @brief Declaration of RevNullObject
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-02-14
 *
 * $Id$
 */


#include "RevNullObject.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

// Declarations
std::ostream& operator<<(std::ostream& o, const RevNullObject& x);

RevNullObject::RevNullObject(void) : RevObject() {
    
}


RevNullObject::RevNullObject(const RevNullObject& n) : RevObject(n) {
    
}


RevNullObject* RevLanguage::RevNullObject::clone(void) const {
    return new RevNullObject(*this);
}


bool RevNullObject::operator==(void *ptr) {
    return ptr == NULL || ptr == this;
}


bool RevNullObject::operator==(const RevNullObject& obj) {
    return true;
}


bool RevNullObject::operator==(const RevObject& obj) {
    return obj.isTypeSpec( getClassTypeSpec() );
}


/** Get class name of object */
const std::string& RevNullObject::getClassName(void) { 
    
    static std::string rbClassName = "NULL";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& RevNullObject::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& RevNullObject::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Print the value. We just print "NULL" */
void RevNullObject::printValue(std::ostream &o) const {
    o << "NULL";
}



/** Make sure we can print the value of the object easily */
std::ostream& operator<<(std::ostream& o, const RevNullObject& x) {
    
    o << "NULL";
    return o;
}

