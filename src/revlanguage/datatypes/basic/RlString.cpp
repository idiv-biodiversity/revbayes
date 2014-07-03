/**
 * @file
 * This file contains the implementation of RlString, which is
 * a RevBayes wrapper around a regular RlString.
 *
 * @brief Implementation of RlString
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */
 
#include "ConstantNode.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RlString.h"
#include "TypeSpec.h"

using namespace RevLanguage;

/** Constructor from empty RlString */
RlString::RlString( void ) : ModelObject<std::string>() {
}


/** Constructor from RlString */
RlString::RlString(const std::string& v) : ModelObject<std::string>( new std::string(v) ) {
}


/** Constructor from int */
RlString::RlString(int i) : ModelObject<std::string>() {

    std::ostringstream o;
    o << i;
    value = new RevBayesCore::ConstantNode<std::string>("", new std::string(o.str()) );
}



/** Constructor from RlString */
RlString::RlString(double x) : ModelObject<std::string>() {

    std::ostringstream o;
    o << x;
    value = new RevBayesCore::ConstantNode<std::string>("", new std::string(o.str()) );
}


/* Construct from DAG node */
RlString::RlString( RevBayesCore::TypedDagNode<std::string> *v ) : ModelObject<std::string>( v ) {
}


/**
 * Generic addition operator.
 * We test if the rhs is of a type that we use for a specialized addition operation.
 *
 * \param[in]   rhs     The right hand side operand of the addition operation.
 *
 * \return              A new object holding the sum.
 */
RevObject* RlString::add( const RevObject& rhs ) const 
{
    
    if ( rhs.getTypeSpec() == RlString::getClassTypeSpec() )
        return add( static_cast<const RlString&>( rhs ) );
    
    return ModelObject<std::string>::add( rhs );
}


/**
 * Specialized addition operation between two string values.
 * The return value is also of type string.
 *
 * \param[in]   rhs     The right hand side operand of the addition operation.
 *
 * \return              A new object holding the concatenation.
 */
RlString* RlString::add(const RevLanguage::RlString &rhs) const
{
    
    RlString *n = new RlString( value->getValue() + rhs.getValue() );
    
    return n;
}


/** Clone function */
RlString* RevLanguage::RlString::clone() const {

	return new RlString(*this);
}


/** Get class name of object */
const std::string& RlString::getClassName(void) { 
    
    static std::string rbClassName = "String";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& RlString::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& RlString::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Print value */
void RlString::printValue(std::ostream& o) const {

	o << value->getValue();
}

