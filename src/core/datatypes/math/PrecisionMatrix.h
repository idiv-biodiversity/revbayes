//
//  PrecisionMatrix.h
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-26.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#ifndef __revbayes__PrecisionMatrix__
#define __revbayes__PrecisionMatrix__

#include <iostream>

#include "MatrixReal.h"
#include "EigenSystem.h"

namespace RevBayesCore {
    
    class EigenSystem;
    
    class PrecisionMatrix : public MatrixReal {
        
    public:
                                            PrecisionMatrix(void);                                              //!< Default constructor required by revlanguae use
                                            PrecisionMatrix(size_t n);                                          //!< Construct rate matrix with n states
                                            PrecisionMatrix(const PrecisionMatrix& m);                          //!< Copy constructor
                                            PrecisionMatrix(const MatrixReal& m);                          //!< Copy constructor
        
        virtual                            ~PrecisionMatrix(void) {}                                            //!< Destructor
                
        // public methods
        // could not make them constant
        // because potentially need to update the eigensystem
        // should implement some update flags here
        bool                                isPositive() const;
        double                              getLogDet() const;
        
        size_t                              getDim() const;
        
        void                                touch();
        void                                update() const;
        
        bool                                isUpdated() const {return eigenflag;}
        
        
        
        virtual PrecisionMatrix*            clone(void) const;
        
        void                                updateMatrix(void);
        //!< Update the rate entries of the matrix (is needed if stationarity freqs or similar have changed)
        
        const MatrixReal&                   getInverse(void) const;
        
        void                                drawNormalSamplePrecision(std::vector<double>& v) const;
        void                                drawNormalSampleCovariance(std::vector<double>& v) const;

    protected:
        
        
    private:
        
        mutable EigenSystem                 eigensystem;
        mutable bool                        eigenflag;

        mutable MatrixReal                  inverse;
        
    };
    
    std::ostream&                       operator<<(std::ostream& o, const PrecisionMatrix& x);                                           //!< Overloaded output operator
  
}

#endif /* defined(__revbayes__PrecisionMatrix__) */
