/**
 * @file
 * This file contains the declaration of ConstructorFunction, which is used
 * for functions that construct objects using the RevLanguage.
 *
 * @brief Declaration of ConstructorFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-06-01 14:40:37 +0200 (Fri, 01 Jun 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: ConstructorFunction.h 1602 2012-06-01 12:40:37Z hoehna $
 */

#ifndef DistributionFunctionRv_H
#define DistributionFunctionRv_H

#include "RlTypedDistribution.h"
#include "RlFunction.h"

#include <string>
#include <vector>

namespace RevLanguage {
    
    template <class valueType>
    class DistributionFunctionRv :  public Function {
        
    public:
        DistributionFunctionRv(TypedDistribution<valueType> *d);                                                                             //!< Object constructor
        DistributionFunctionRv(const DistributionFunctionRv& obj);                                                                    //!< Copy constructor
        
        // overloaded operators
        DistributionFunctionRv&                operator=(const DistributionFunctionRv& c);
        
        // Basic utility functions
        DistributionFunctionRv*                clone(void) const;                                                              //!< Clone the object
        static const std::string&               getClassName(void);                                                             //!< Get class name
        static const TypeSpec&                  getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                         getTypeSpec(void) const;                                                        //!< Get language type of the object
        
        // Regular functions
        RevObject*                              execute(void);                                                                  //!< Execute the function. This is the function one has to overwrite for single return values.
        const ArgumentRules&                    getArgumentRules(void) const;                                                   //!< Get argument rules
        const TypeSpec&                         getReturnType(void) const;                                                      //!< Get type of return value
        
    protected:
        
        ArgumentRules                           argRules;                                                                       //!< Member rules converted to reference rules
        TypedDistribution<valueType>*           templateObject;                                                                 //!< The template object
        
    };
    
}

#include "ArgumentRule.h"
#include "DeterministicNode.h"
#include "DistributionFunctionRv.h"
#include "ProbabilityDensityFunction.h"
#include "Real.h"
#include "RevObject.h"
#include "TypeSpec.h"

#include <sstream>

/** Constructor */
template <class valueType>
RevLanguage::DistributionFunctionRv<valueType>::DistributionFunctionRv( TypedDistribution<valueType> *d ) : Function(), templateObject( d ) {
    
    argRules.push_back( new ArgumentRule("n", true, Natural::getClassTypeSpec(), new Natural(1)));
    const ArgumentRules &memberRules = templateObject->getMemberRules();
    for (std::vector<ArgumentRule*>::const_iterator it = memberRules.begin(); it != memberRules.end(); ++it) {
        argRules.push_back( (*it)->clone() );
    }
}


/** Constructor */
template <class valueType>
RevLanguage::DistributionFunctionRv<valueType>::DistributionFunctionRv(const DistributionFunctionRv& obj) : Function(obj), argRules( obj.argRules )  {
    
    templateObject = obj.templateObject->clone();
    
}


template <class valueType>
RevLanguage::DistributionFunctionRv<valueType>& RevLanguage::DistributionFunctionRv<valueType>::operator=(const DistributionFunctionRv &c) {
    
    if (this != &c) {
        Function::operator=(c);
        
        templateObject = c.templateObject->clone();
        
        argRules = c.argRules;
    }
    
    return *this;
}


/** Clone the object */
template <class valueType>
RevLanguage::DistributionFunctionRv<valueType>* RevLanguage::DistributionFunctionRv<valueType>::clone(void) const {
    
    return new DistributionFunctionRv<valueType>(*this);
}


/** Execute function: we reset our template object here and give out a copy */
template <class valueType>
RevLanguage::RevObject* RevLanguage::DistributionFunctionRv<valueType>::execute( void ) {
    
    TypedDistribution<valueType>* copyObject = templateObject->clone();
    
    for ( size_t i = 1; i < args.size(); i++ ) {
        
        if ( args[i].isConstant() ) {
            copyObject->setConstMember( args[i].getLabel(), RevPtr<const Variable>( (Variable*) args[i].getVariable() ) );
        } else {
            copyObject->setMember( args[i].getLabel(), args[i].getReferenceVariable() );
        }
    }
    
    
    
    int n = static_cast<const Natural &>( this->args[0].getVariable()->getRevObject() ).getValue();
    Vector<valueType> *values = new Vector<valueType>();
    for (int i = 0;  i < n; ++i) {
        valueType* value = copyObject->createRandomVariable();
        value->makeConstantValue();
        values->push_back( *value );
        delete value;
    }
    return values;
}


/** Get argument rules */
template <class valueType>
const RevLanguage::ArgumentRules& RevLanguage::DistributionFunctionRv<valueType>::getArgumentRules(void) const {
    
    return argRules;
}


/** Get class name of object */
template <class valueType>
const std::string& RevLanguage::DistributionFunctionRv<valueType>::getClassName(void) { 
    
    static std::string rbClassName = "DistributionFunctionRv";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
template <class valueType>
const RevLanguage::TypeSpec& RevLanguage::DistributionFunctionRv<valueType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
template <class valueType>
const RevLanguage::TypeSpec& RevLanguage::DistributionFunctionRv<valueType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
template <class valueType>
const RevLanguage::TypeSpec& RevLanguage::DistributionFunctionRv<valueType>::getReturnType(void) const {
    
    return Real::getClassTypeSpec();
}


#endif
