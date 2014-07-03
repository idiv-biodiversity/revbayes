/**
 * @file
 * This file contains the implementation of SyntaxConstant, which is
 * used to hold constants in the syntax tree.
 *
 * @brief Implementation of SyntaxConstant
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include <iostream>

#include "ConstantNode.h"
#include "RevObject.h"
#include "SyntaxConstant.h"

using namespace RevLanguage;

/** Construct from value */
SyntaxConstant::SyntaxConstant(RevObject* val) : SyntaxElement(), value(val) {
}


/** Deep copy constructor */
SyntaxConstant::SyntaxConstant(const SyntaxConstant& x) : SyntaxElement(x), value(NULL) {

    value = x.value->clone();
}


/** Destructor deletes value */
SyntaxConstant::~SyntaxConstant(void) {
    
    delete value;
}


/** Assignment operator deletes value and makes a clone of the value */
SyntaxConstant& SyntaxConstant::operator=(const SyntaxConstant& x) {

    if (this != &x) {

        SyntaxElement::operator=(x);
        
        delete value;
        value = x.value->clone();
        
    }

    return (*this);
}


/** Clone syntax element */
SyntaxConstant* SyntaxConstant::clone (void) const {

    return new SyntaxConstant(*this);
}


/** Get semantic value of element */
RevPtr<Variable> SyntaxConstant::evaluateContent( Environment& env ) {

    // We return a clone in case this function is called repeatedly.
    if (value == NULL)
        return RevPtr<Variable>( new Variable( NULL ) );
    else 
        return RevPtr<Variable>( new Variable( value->clone() ) );
}


/** Is the expression constant?
 *  Of course we are!!!
 */
bool SyntaxConstant::isConstExpression(void) const {
    return true;
}


/** Print info about the syntax element */
void SyntaxConstant::printValue(std::ostream& o) const {

    o << "SyntaxConstant: value = ";
    value->printValue(o);
    o << std::endl;
}


/**
 * Replace the syntax variable with name by the constant value. Loops have to do that for their index variables.
 * Don't do anything.
 */
void SyntaxConstant::replaceVariableWithConstant(const std::string& name, const RevObject& c) {
    
    
}

