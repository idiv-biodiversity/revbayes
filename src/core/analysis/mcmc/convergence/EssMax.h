#ifndef EssMax_H
#define EssMax_H

#include <vector>

#include "BurninEstimatorContinuous.h"

namespace RevBayesCore {

    class EssMax : public BurninEstimatorContinuous {
    
    public:
      EssMax(std::size_t b=10, double f=0.5);
    
        // implementen functions from convergence diagnostic
      std::size_t      estimateBurnin(const std::vector<double>& values);
    
    private:
    
      std::size_t      blockSize;                                                                                          //!< first window
        double      frac;
    
    };
    
}

#endif
