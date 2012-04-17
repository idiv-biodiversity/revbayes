/**
 * @file
 * This file contains the implementation of Dist_norm, which is used to hold
 * parameters and functions related to a normal distribution.
 *
 * @brief Implementation of Dist_norm
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id$
 */

#include "DAGNode.h"
#include "Dist_norm.h"
#include "MemberFunction.h"
#include "RealPos.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "Real.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbUtil.h"
#include "StochasticNode.h"
#include "ValueRule.h"
#include "Workspace.h"
#include "DistributionNormal.h"

#include <cmath>


/** Constructor for parser use */
Dist_norm::Dist_norm( void ) : DistributionContinuous( getMemberRules() ), mu( NULL ), sigma( NULL ) {

}


/**
 * This function calculates the cumulative probability for a
 * normally-distributed random variable.
 *
 * @brief Normal cumulative probability
 *
 * @param q     Quantile
 * @return      Cumulative probability
 *
 * @see Adams, A. G. 1969. Areas under the normal curve. Computer J. 12:197-198.
 */
double Dist_norm::cdf( const RbLanguageObject& value ) {

	double m = static_cast<const Real&   >( mu->getValue()    ).getValue();
    double s = static_cast<const RealPos&>( sigma->getValue() ).getValue();
    double q = static_cast<const Real&   >( value            ).getValue();
	return RbStatistics::Normal::cdf(m, s, q);

}


/** Clone this object */
Dist_norm* Dist_norm::clone( void ) const {

    return new Dist_norm( *this );
}


/** Get class name of object */
const std::string& Dist_norm::getClassName(void) { 
    
    static std::string rbClassName = "Normal distribution";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Dist_norm::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( DistributionContinuous::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Dist_norm::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get member variable rules */
const MemberRules& Dist_norm::getMemberRules( void ) const {

    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;

    if ( !rulesSet ) {

        memberRules.push_back( new ValueRule( "mean", Real::getClassTypeSpec()   , new Real(0.0)    ) );
        memberRules.push_back( new ValueRule( "sd"  , RealPos::getClassTypeSpec(), new RealPos(1.0) ) );

        rulesSet = true;
    }

    return memberRules;
}


/** Get random variable type */
const TypeSpec& Dist_norm::getVariableType( void ) const {

    static TypeSpec varTypeSpec = Real::getClassTypeSpec();
    
    return varTypeSpec;
}


/**
 * This function calculates the natural log of the probability
 * density for a normally-distributed random variable.
 *
 * @brief Natural log of normal probability density
 *
 * @param value Observed value
 * @return      Natural log of the probability density
 */
double Dist_norm::lnPdf(const RbLanguageObject& value) const {
    
    double m = static_cast<const Real&   >( mu->getValue()    ).getValue();
    double s = static_cast<const RealPos&>( sigma->getValue() ).getValue();
    double x = static_cast<const Real&   >( value            ).getValue();


    return RbStatistics::Normal::lnPdf(m, s, x);

}


/**
 * This function calculates the probability density
 * for a normally-distributed random variable.
 *
 * @brief Normal probability density
 *
 * @param value Observed value
 * @return      Probability density
 */
double Dist_norm::pdf( const RbLanguageObject& value ) const {
    
    double m = static_cast<const Real&   >( mu->getValue()    ).getValue();
    double s = static_cast<const RealPos&>( sigma->getValue() ).getValue();
    double x = static_cast<const Real&   >( value            ).getValue();

    return RbStatistics::Normal::pdf(m, s, x);

}


/**
 * This function calculates the quantile for a
 * normally-distributed random variable.
 *
 * @brief Quantile of normal probability density
 *
 * @param p     Cumulative probability of quantile
 * @return      Quantile
 *
 * @see Odeh, R. E. and J. O. Evans. 1974. The percentage points of the normal
 *      distribution. Applied Statistics, 22:96-97.
 * @see Wichura, M. J.  1988. Algorithm AS 241: The percentage points of the
 *      normal distribution. 37:477-484.
 * @see Beasley, JD & S. G. Springer. 1977. Algorithm AS 111: The percentage
 *      points of the normal distribution. 26:118-121.
 */
const Real& Dist_norm::quantile( const double p) {
    
    double m = static_cast<const Real&   >( mu->getValue()    ).getValue();
    double s = static_cast<const RealPos&>( sigma->getValue() ).getValue();
	
    quant.setValue( RbStatistics::Normal::quantile(m, s, p) );
    
    return quant;
}


/**
 * This function generates a normally-distributed
 * random variable.
 *
 * @todo What algorithm is this? It is not Box-Muller, not Ziggurat. It does
 *       not resemble the R code, which uses Ahrens & Dieter and Kinderman & Ramage.
 *       Code for extra normal rv based on guesswork using comments in RbStatistics.cpp.
 *
 * @brief Random draw from normal distribution
 *
 * @return      Random draw
 */
const RbLanguageObject& Dist_norm::rv(void) {

    static bool   availableNormalRv = false;
    static double extraNormalRv;

    if ( availableNormalRv ) {
        availableNormalRv = false;
        randomVariable.setValue( extraNormalRv );
        return randomVariable;
    }
    
    double m = static_cast<const Real&   >( mu->getValue()    ).getValue();
    double s = static_cast<const RealPos&>( sigma->getValue() ).getValue();

    RandomNumberGenerator* rng = GLOBAL_RNG;
    randomVariable.setValue( RbStatistics::Normal::rv(m, s, *rng) );
    
	return randomVariable;

}


/** We catch here the setting of the member variables to store our parameters. */
void Dist_norm::setMemberVariable(std::string const &name, const Variable *var) {
    
    if ( name == "mean" ) {
        mu = var;
    }
    else if ( name == "sd" ) {
        sigma = var;
    }
    else {
        DistributionContinuous::setMemberVariable(name, var);
    }
}
