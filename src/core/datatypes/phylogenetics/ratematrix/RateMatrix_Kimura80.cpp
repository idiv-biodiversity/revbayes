#include "MatrixReal.h"
#include "RateMatrix_Kimura80.h"
#include "RbException.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <string>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateMatrix_Kimura80::RateMatrix_Kimura80(void) : TimeReversibleRateMatrix( 4 ){
    
    kappa = 1.0;
    
    updateMatrix();
    
}


/** Destructor */
RateMatrix_Kimura80::~RateMatrix_Kimura80(void) {
    
}



/** Calculate the transition probabilities */
void RateMatrix_Kimura80::calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const
{
    
    // compute beta
    double beta = 1.0 / (0.5+kappa*0.5);
    
    // calculate the transition probabilities
    
    double xx = - beta * t;
    double aa = exp(xx);
    double bb = exp( (1.0+0.5*(kappa-1.0))* xx);
    double oneminusa = 1 - aa;
    
    P[0][0] = ( ( 0.5+0.5*aa ) + bb) / 2.0;
    P[0][1] = oneminusa / 4.0;
    P[0][2] = ( ( 0.5+0.5*aa ) - bb) / 2.0;
    P[0][3] = oneminusa / 4.0;
    
    P[1][0] = oneminusa / 4.0;
    P[1][1] = ( ( 0.5+0.5*aa ) + bb) / 2.0;
    P[1][2] =  oneminusa / 4.0;
    P[1][3] = ( ( 0.5+0.5*aa ) - bb) / 2.0;
    
    P[2][0] = ( ( 0.5+0.5*aa ) - bb) / 2.0;
    P[2][1] = P[0][1];
    P[2][2] = ( ( 0.5+0.5*aa ) + bb) / 2.0;
    P[2][3] = P[0][3];
    
    P[3][0] = P[1][0];
    P[3][1] = ( ( 0.5+0.5*aa ) - bb) / 2.0;
    P[3][2] = P[1][2];
    P[3][3] = ( ( 0.5+0.5*aa ) + bb) / 2.0;
    
}


RateMatrix_Kimura80* RateMatrix_Kimura80::clone( void ) const {
    
    return new RateMatrix_Kimura80( *this );
}


void RateMatrix_Kimura80::setKappa( double k ) {
    
    kappa = k;
    
    // set flags
    needsUpdate = true;
    
}


void RateMatrix_Kimura80::updateMatrix( void ) {
    
    if ( needsUpdate )
    {
        MatrixReal &m = *theRateMatrix;
        
        // @todo: This is only needed for printing the values of the rate matrix properly to the screen. We should do this more efficiently (Sebastian).
        // We could instead only update the matrix if a print call happened and the matrix was flagged as dirty.
        
        // compute the off-diagonal values
        m[0][1] = 1;
        m[0][2] = kappa;
        m[0][3] = 1;
        
        m[1][0] = 1;
        m[1][2] = 1;
        m[1][3] = kappa;
        
        m[2][0] = kappa;
        m[2][1] = 1;
        m[2][3] = 1;
        
        m[3][0] = 1;
        m[3][1] = kappa;
        m[3][2] = 1;
        
        // set the diagonal values
        setDiagonal();
        
        // rescale
        rescaleToAverageRate( 1.0 );
        
        // clean flags
        needsUpdate = false;
    }
}


