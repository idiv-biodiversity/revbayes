//
//  DistributionFunctionCdf.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 3/8/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#include "DistributionFunctionCdf.h"

#include "ArgumentRule.h"
#include "CummulativeDistributionFunction.h"
#include "DeterministicNode.h"
#include "DistributionFunctionPdf.h"
#include "Probability.h"
#include "ProbabilityDensityFunction.h"
#include "Real.h"
#include "RevObject.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Constructor */
DistributionFunctionCdf::DistributionFunctionCdf( ContinuousDistribution *d ) : Function(), templateObject( d ), templateObjectPositive( NULL ) {
    
    argRules.push_back( new ArgumentRule("x", true, Real::getClassTypeSpec()));
    const ArgumentRules &memberRules = templateObject->getMemberRules();
    for (std::vector<ArgumentRule*>::const_iterator it = memberRules.begin(); it != memberRules.end(); ++it) {
        argRules.push_back( (*it)->clone() );
    }
}

/** Constructor */
DistributionFunctionCdf::DistributionFunctionCdf( PositiveContinuousDistribution *d ) : Function(), templateObject( NULL ), templateObjectPositive( d ) {
    
    argRules.push_back( new ArgumentRule("x", true, RealPos::getClassTypeSpec()));
    const ArgumentRules &memberRules = templateObjectPositive->getMemberRules();
    for (std::vector<ArgumentRule*>::const_iterator it = memberRules.begin(); it != memberRules.end(); ++it) {
        argRules.push_back( (*it)->clone() );
    }
}


/** Constructor */
DistributionFunctionCdf::DistributionFunctionCdf(const DistributionFunctionCdf& obj) : Function(obj), argRules( obj.argRules )  {
    
    if ( obj.templateObject != NULL ) {
        templateObject = obj.templateObject->clone();
    }
    else {
        templateObject = NULL;
    }
    
    if ( obj.templateObjectPositive != NULL ) {
        templateObjectPositive = obj.templateObjectPositive->clone();
    }
    else {
        templateObjectPositive = NULL;
    }
    
}


DistributionFunctionCdf& DistributionFunctionCdf::operator=(const DistributionFunctionCdf &c) {
    
    if (this != &c) {
        Function::operator=(c);
        
        if ( c.templateObject != NULL ) {
            templateObject = c.templateObject->clone();
        }
        else {
            templateObject = NULL;
        }
        
        if ( c.templateObjectPositive != NULL ) {
            templateObjectPositive = c.templateObjectPositive->clone();
        }
        else {
            templateObjectPositive = NULL;
        }
        
        argRules = c.argRules;
    }
    
    return *this;
}


/** Clone the object */
DistributionFunctionCdf* DistributionFunctionCdf::clone(void) const {
    
    return new DistributionFunctionCdf(*this);
}


/** Execute function: we reset our template object here and give out a copy */
RevObject* DistributionFunctionCdf::execute( void ) {
    
    RevBayesCore::ContinuousDistribution *d = NULL;
    
    if ( templateObject != NULL ) {
    
        ContinuousDistribution* copyObject = templateObject->clone();
    
        for ( size_t i = 1; i < args.size(); i++ ) {
        
            if ( args[i].isConstant() ) {
                copyObject->setConstMember( args[i].getLabel(), RevPtr<const Variable>( (Variable*) args[i].getVariable() ) );
            } else {
                copyObject->setMember( args[i].getLabel(), args[i].getReferenceVariable() );
            }
        }
        
        d = copyObject->createDistribution();
        
    }
    else {
        
        
        PositiveContinuousDistribution* copyObject = templateObjectPositive->clone();
        
        for ( size_t i = 1; i < args.size(); i++ ) {
            
            if ( args[i].isConstant() ) {
                copyObject->setConstMember( args[i].getLabel(), RevPtr<const Variable>( (Variable*) args[i].getVariable() ) );
            } else {
                copyObject->setMember( args[i].getLabel(), args[i].getReferenceVariable() );
            }
        }
        
        d = copyObject->createDistribution();
        
    }
    
    
    
    RevBayesCore::TypedDagNode<double>* arg = static_cast<const Probability &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::CummulativeDistributionFunction* f = new RevBayesCore::CummulativeDistributionFunction( arg, d );
    RevBayesCore::DeterministicNode<double> *detNode = new RevBayesCore::DeterministicNode<double>("", f);
    
    Probability* value = new Probability( detNode );
    
    return value;
}


/** Get argument rules */
const ArgumentRules& DistributionFunctionCdf::getArgumentRules(void) const {
    
    return argRules;
}


/** Get class name of object */
const std::string& DistributionFunctionCdf::getClassName(void) { 
    
    static std::string rbClassName = "DistributionFunctionCdf";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& DistributionFunctionCdf::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& DistributionFunctionCdf::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& DistributionFunctionCdf::getReturnType(void) const {
    
    return Probability::getClassTypeSpec();
}
