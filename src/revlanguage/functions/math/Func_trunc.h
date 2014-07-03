/**
 * @file
 * This file contains the declaration of the RevLanguage trunc function, which
 * is used to created deterministic variable associated with the trunc function.
 * This is the standard arithmetic truncating of real numbers. Thus, the real part is simply dropped
 * which makes this function a floor for positive numbers and a ceil for negative numbers.
 *
 * @brief Declaration and implementation of Func_exp
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-20 04:06:14 +0200 (Fri, 20 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func__add.h 1406 2012-04-20 02:06:14Z hoehna $
 */


#ifndef Func_trunc_H
#define Func_trunc_H

#include "RlFunction.h"

#include <string>

namespace RevLanguage {
    
    template <typename valType, typename retType>
    class Func_trunc :  public Function {
        
    public:
        Func_trunc( void );
        
        // Basic utility functions
        Func_trunc*                                     clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassName(void);                                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevObject*                                      execute(void);                                                                  //!< Execute function
        const ArgumentRules&                            getArgumentRules(void) const;                                                   //!< Get argument rules
        const TypeSpec&                                 getReturnType(void) const;                                                      //!< Get type of return value
        
    };
    
}


#include "DeterministicNode.h"
#include "TruncateFunction.h"
#include "Real.h"
#include "RealPos.h"
#include "TypedDagNode.h"

/** default constructor */
template <typename valType, typename retType>
RevLanguage::Func_trunc<valType, retType>::Func_trunc( void ) : Function( ) {
    
}


/** Clone object */
template <typename valType, typename retType>
RevLanguage::Func_trunc<valType, retType>* RevLanguage::Func_trunc<valType, retType>::clone( void ) const {
    
    return new Func_trunc<valType, retType>( *this );
}


template <typename valType, typename retType>
RevLanguage::RevObject* RevLanguage::Func_trunc<valType, retType>::execute() {
    
    RevBayesCore::TypedDagNode<double>* arg = static_cast<const valType &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TruncateFunction* f = new RevBayesCore::TruncateFunction( arg );
    RevBayesCore::DeterministicNode<int> *detNode = new RevBayesCore::DeterministicNode<int>("", f);
    
    retType* value = new retType( detNode );
    
    return value;
}


/* Get argument rules */
template <typename valType, typename retType>
const RevLanguage::ArgumentRules& RevLanguage::Func_trunc<valType, retType>::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) 
    {
        
        argumentRules.push_back( new ArgumentRule( "x", true, valType::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


template <typename valType, typename retType>
const std::string& RevLanguage::Func_trunc<valType, retType>::getClassName(void) { 
    
    static std::string rbClassName = "Func_trunc";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
template <typename valType, typename retType>
const RevLanguage::TypeSpec& RevLanguage::Func_trunc<valType, retType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( valType::getClassTypeSpec() ), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
template <typename valType, typename retType>
const RevLanguage::TypeSpec& RevLanguage::Func_trunc<valType, retType>::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = retType::getClassTypeSpec();
    
    return returnTypeSpec;
}


template <typename valType, typename retType>
const RevLanguage::TypeSpec& RevLanguage::Func_trunc<valType, retType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


#endif
