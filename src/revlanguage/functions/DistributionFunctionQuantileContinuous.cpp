//
//  DistributionFunctionQuantileContinuous.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 3/8/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#include "DistributionFunctionQuantileContinuous.h"

#include "ArgumentRule.h"
#include "DeterministicNode.h"
#include "DistributionFunctionPdf.h"
#include "Probability.h"
#include "ProbabilityDensityFunction.h"
#include "QuantileFunction.h"
#include "Real.h"
#include "RevObject.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Constructor */
DistributionFunctionQuantileContinuous::DistributionFunctionQuantileContinuous( ContinuousDistribution *d ) : TypedFunction<Real>(),
    templateObject( d )
{
    
    argRules.push_back( new ArgumentRule("p", Probability::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    const ArgumentRules &memberRules = templateObject->getParameterRules();
    for (std::vector<ArgumentRule*>::const_iterator it = memberRules.begin(); it != memberRules.end(); ++it)
    {
        argRules.push_back( (*it)->clone() );
    }
}


/** Constructor */
DistributionFunctionQuantileContinuous::DistributionFunctionQuantileContinuous(const DistributionFunctionQuantileContinuous& obj) : TypedFunction<Real>(obj),
    argRules( obj.argRules )
{
    
    if ( obj.templateObject != NULL )
    {
        templateObject = obj.templateObject->clone();
    }
    else
    {
        templateObject = NULL;
    }
    
}



DistributionFunctionQuantileContinuous::~DistributionFunctionQuantileContinuous( void )
{
    delete templateObject;
}

DistributionFunctionQuantileContinuous& DistributionFunctionQuantileContinuous::operator=(const DistributionFunctionQuantileContinuous &c) {
    
    if (this != &c) {
        Function::operator=(c);
        
        delete templateObject;
        if ( c.templateObject != NULL ) {
            templateObject = c.templateObject->clone();
        }
        else {
            templateObject = NULL;
        }
        
        
        argRules = c.argRules;
    }
    
    return *this;
}


/** Clone the object */
DistributionFunctionQuantileContinuous* DistributionFunctionQuantileContinuous::clone(void) const {
    
    return new DistributionFunctionQuantileContinuous(*this);
}


RevBayesCore::TypedFunction<double>* DistributionFunctionQuantileContinuous::createFunction( void ) const
{
    
    RevBayesCore::TypedFunction<double>* f = NULL;
    RevBayesCore::TypedDagNode<double>* arg = static_cast<const Probability &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    
    ContinuousDistribution* copyObject = templateObject->clone();
    
    for ( size_t i = 1; i < args.size(); i++ )
    {
    
        if ( args[i].isConstant() )
        {
            copyObject->setConstParameter( args[i].getLabel(), RevPtr<const RevVariable>( (const RevVariable*) args[i].getVariable() ) );
        }
        else
        {
            copyObject->setParameter( args[i].getLabel(), args[i].getReferenceVariable() );
        }
    }
        
    RevBayesCore::ContinuousDistribution *d = copyObject->createDistribution();
    f = new RevBayesCore::QuantileFunction( arg, d );
    
    
    // return the value
    return f;
}


/** Get argument rules */
const ArgumentRules& DistributionFunctionQuantileContinuous::getArgumentRules(void) const {
    
    return argRules;
}


/** Get Rev type of object */
const std::string& DistributionFunctionQuantileContinuous::getClassType(void) { 
    
    static std::string revType = "DistributionFunctionQuantileContinuous";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& DistributionFunctionQuantileContinuous::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}

/** Get type spec */
const TypeSpec& DistributionFunctionQuantileContinuous::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
