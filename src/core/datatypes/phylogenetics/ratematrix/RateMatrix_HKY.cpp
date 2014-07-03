/**
 * @file
 * This file contains the implementation of RateMatrix_HKY, which is
 * class that holds a rate matrix in RevBayes.
 *
 * @brief Implementation of RateMatrix_HKY
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-12-11 14:46:24 +0100 (Tue, 11 Dec 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 * @interface Mcmc
 * @package distributions
 *
 * $Id: RateMatrix_HKY.cpp 1921 2012-12-11 13:46:24Z hoehna $
 */

#include "EigenSystem.h"
#include "MatrixComplex.h"
#include "MatrixReal.h"
#include "RateMatrix_HKY.h"
#include "RbException.h"
#include "RbMathMatrix.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateMatrix_HKY::RateMatrix_HKY(void) : TimeReversibleRateMatrix( 4 ){
    
    kappa = 1.0;
    
    updateMatrix();
    
}


/** Copy constructor */
RateMatrix_HKY::RateMatrix_HKY(const RateMatrix_HKY& m) : TimeReversibleRateMatrix( m ) {
    
    kappa = m.kappa;
    
}


/** Destructor */
RateMatrix_HKY::~RateMatrix_HKY(void) {
    
}


RateMatrix_HKY& RateMatrix_HKY::operator=(const RateMatrix_HKY &r) {
    
    if (this != &r) {
        TimeReversibleRateMatrix::operator=( r );
        
        kappa = r.kappa;
    }
    
    return *this;
}



/** Calculate the transition probabilities */
void RateMatrix_HKY::calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const {
    
    // notation:
    double pi_A = stationaryFreqs[0];
    double pi_C = stationaryFreqs[1];
    double pi_G = stationaryFreqs[2];
    double pi_T = stationaryFreqs[3];
    
    // compute auxilliary variables
    double pi_AG = pi_A + pi_G;
    double pi_CT = pi_C + pi_T;
    
    // compute beta
    double beta = 1.0 / (2.0*pi_AG*pi_CT+2.0*kappa*((pi_A*pi_G)+(pi_C*pi_T))); 
    
    // calculate the transition probabilities
    
    double xx = - beta * t;
    double aa = exp(xx);
    double bbR = exp( (1.0+pi_AG*(kappa-1.0))* xx);
    double bbY = exp( (1.0+pi_CT*(kappa-1.0))* xx);
    double oneminusa = 1 - aa;
    
    P[0][0] = (pi_A * ( pi_AG+pi_CT*aa ) + pi_G*bbR) / pi_AG;
    P[0][1] = pi_C * oneminusa;
    P[0][2] = (pi_G * ( pi_AG+pi_CT*aa ) - pi_G*bbR) / pi_AG;
    P[0][3] = pi_T * oneminusa;
    
    P[1][0] = pi_A * oneminusa;
    P[1][1] = (pi_C * ( pi_CT+pi_AG*aa ) + pi_T*bbY) / pi_CT;
    P[1][2] = pi_G * oneminusa;
    P[1][3] = (pi_T * ( pi_CT+pi_AG*aa ) - pi_T*bbY) / pi_CT;
    
    P[2][0] = (pi_A * ( pi_AG+pi_CT*aa ) - pi_A*bbR) / pi_AG;
    P[2][1] = P[0][1];
    P[2][2] = (pi_G * ( pi_AG+pi_CT*aa ) + pi_A*bbR) / pi_AG;
    P[2][3] = P[0][3];
    
    P[3][0] = P[1][0];
    P[3][1] = (pi_C * ( pi_CT+pi_AG*aa ) - pi_C*bbY) / pi_CT;
    P[3][2] = P[1][2];
    P[3][3] = (pi_T * ( pi_CT+pi_AG*aa ) + pi_C*bbY) / pi_CT;
	
}


RateMatrix_HKY* RateMatrix_HKY::clone( void ) const {
    
    return new RateMatrix_HKY( *this );
}


void RateMatrix_HKY::setKappa( double k ) {
    
    kappa = k;
    
    // set flags
    needsUpdate = true;
    
}


void RateMatrix_HKY::updateMatrix( void ) {
    
    if ( needsUpdate ) 
    {
        MatrixReal &m = *theRateMatrix;
        
        // @todo: This is only needed for printing the values of the rate matrix properly to the screen. We should do this more efficiently (Sebastian).
        // We could instead only update the matrix if a print call happened and the matrix was flagged as dirty.
        
        // compute the off-diagonal values
        m[0][1] = stationaryFreqs[1];
        m[0][2] = kappa*stationaryFreqs[2];
        m[0][3] = stationaryFreqs[3];
        
        m[1][0] = stationaryFreqs[0];
        m[1][2] = stationaryFreqs[2];
        m[1][3] = kappa*stationaryFreqs[3];
        
        m[2][0] = kappa*stationaryFreqs[0];
        m[2][1] = stationaryFreqs[2];
        m[2][3] = stationaryFreqs[3];
        
        m[3][0] = stationaryFreqs[0];
        m[3][1] = kappa*stationaryFreqs[2];
        m[3][2] = stationaryFreqs[3];
        
        // set the diagonal values
        setDiagonal();
        
        // rescale 
        rescaleToAverageRate( 1.0 );
        
        // clean flags
        needsUpdate = false;
    }
}


