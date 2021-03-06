#include "Func_pomo.h"
//
//  Func_pomo.cpp
//  RevBayesCore
//
//  Created by Bastien Boussau on 22/8/14.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "PomoRateMatrixFunction.h"
#include "Func_pomo.h"
#include "Natural.h"
#include "RateMatrix_Pomo.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_pomo::Func_pomo( void ) : TypedFunction<RateMatrix>( ) {
    
}


/** Clone object */
Func_pomo* Func_pomo::clone( void ) const {
    
    return new Func_pomo( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RateMatrix >* Func_pomo::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::RateMatrix >* q = static_cast<const RateMatrix &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    //RevBayesCore::TypedDagNode< double >* root_pol = static_cast<const double &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* fit = static_cast<const ModelVector<RealPos> &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< int >* n = static_cast<const Natural &>( this->args[2].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::PomoRateMatrixFunction* f = new RevBayesCore::PomoRateMatrixFunction( n, q, fit );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_pomo::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "mutationRates", RateMatrix::getClassTypeSpec()       , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "fitness"      , ModelVector<Real>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "virtualNe"    , Natural::getClassTypeSpec()          , ArgumentRule::BY_CONSTANT_REFERENCE ) );

        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_pomo::getClassType(void) {
    
    static std::string revType = "Func_pomo";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_pomo::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


const TypeSpec& Func_pomo::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
