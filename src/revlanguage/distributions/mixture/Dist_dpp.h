/**
 * @file
 * This file contains the declaration of the Dirichlet process prior distribution, which is used create
 * random variables of DPP distributions.
 *
 * @brief Declaration of the Dirichlet distribution
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-20 04:06:14 +0200 (Fri, 20 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since Version 1.0, 2012-08-06
 *
 * $Id: Func__add.h 1406 2012-04-20 02:06:14Z hoehna $
 */

#ifndef Dist_dpp_H
#define Dist_dpp_H

#include "DirichletProcessPriorDistribution.h"
#include "RealPos.h"
#include "RlTypedDistribution.h"
#include "TypeSpec.h"
#include "Vector.h"

namespace RevLanguage {
    
    template <typename valType>
    class Dist_dpp :  public TypedDistribution< Vector<valType> >{
        
    public:
        Dist_dpp( void );
        virtual ~Dist_dpp();
        
        // Basic utility functions
        Dist_dpp*                                       clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassName(void);                                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getMemberRules(void) const;                                                     //!< Get member rules (const)
//        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::DirichletProcessPriorDistribution<typename valType::valueType>*            createDistribution(void) const;
        
    protected:
        
        void                                            setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var);     //!< Set member variable
        
        
    private:
        RevPtr<const Variable>                          concentration;
        RevPtr<const Variable>                          baseDistribution;
        RevPtr<const Variable>                          numElements;
        
    };
    
}


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Real.h"
#include "RealPos.h"
#include "RlSimplex.h"
#include "StochasticNode.h"
#include "TypedDistribution.h"


template <typename valType>
RevLanguage::Dist_dpp<valType>::Dist_dpp() : TypedDistribution< Vector<valType> >(), 
concentration( NULL ),
baseDistribution( NULL ),
numElements( NULL )
{
    
}


template <typename valType>
RevLanguage::Dist_dpp<valType>::~Dist_dpp() {
    
}



template <typename valType>
RevLanguage::Dist_dpp<valType>* RevLanguage::Dist_dpp<valType>::clone( void ) const {
    return new Dist_dpp(*this);
}


template <typename valType>
RevBayesCore::DirichletProcessPriorDistribution<typename valType::valueType>* RevLanguage::Dist_dpp<valType>::createDistribution( void ) const {
	
    // get the parameters
    const Distribution& rlDistribution									= static_cast<const Distribution &>( baseDistribution->getRevObject() );
    RevBayesCore::TypedDistribution<typename valType::valueType>* g0    = static_cast<RevBayesCore::TypedDistribution<typename valType::valueType>* >( rlDistribution.createDistribution() );    
    RevBayesCore::TypedDagNode<double>* cp								= static_cast<const RealPos &>( concentration->getRevObject() ).getDagNode();

    int ne = static_cast<const RealPos &>( numElements->getRevObject() ).getValue();
    RevBayesCore::DirichletProcessPriorDistribution<typename valType::valueType>* d		= new RevBayesCore::DirichletProcessPriorDistribution<typename valType::valueType>(g0, cp, ne);
    
    return d;
}



/* Get class name of object */
template <typename valType>
const std::string& RevLanguage::Dist_dpp<valType>::getClassName(void) { 
    
    static std::string rbClassName = "Dist_dpp";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Dist_dpp<valType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( TypedDistribution< Vector< valType > >::getClassTypeSpec() ) );
    
	return rbClass; 
}




/** Return member rules (no members) */
template <typename valType>
const RevLanguage::MemberRules& RevLanguage::Dist_dpp<valType>::getMemberRules(void) const {
    
    static MemberRules distDPPMemberRules;
    static bool rulesSet = false;

    if ( !rulesSet ) {
        distDPPMemberRules.push_back( new ArgumentRule( "concentration",     true, RealPos::getClassTypeSpec(), new RealPos(1.0) ) );
        distDPPMemberRules.push_back( new ArgumentRule( "baseDistribution",  true, TypedDistribution<valType>::getClassTypeSpec() ) );
        distDPPMemberRules.push_back( new ArgumentRule( "numElements",       true, RealPos::getClassTypeSpec(), new RealPos(1.0) ) );
        
        rulesSet = true;
    }
    
    return distDPPMemberRules;
}


template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Dist_dpp<valType>::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}



/** Set a member variable */
template <typename valType>
void RevLanguage::Dist_dpp<valType>::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "concentration" ) {
        concentration = var;
    }
    else if ( name == "baseDistribution" ) {
        baseDistribution = var;
    }
    else if ( name == "numElements" ) {
        numElements = var;
    }
    else {
        TypedDistribution< Vector< valType > >::setConstMemberVariable(name, var);
    }
}


#endif
