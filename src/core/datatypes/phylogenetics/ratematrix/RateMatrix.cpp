#include "RateMatrix.h"
#include "RbMathMatrix.h"

#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateMatrix::RateMatrix(size_t n) :
    numStates( n )
{

}





/** Destructor */
RateMatrix::~RateMatrix(void)
{
    
}


size_t RateMatrix::getNumberOfStates( void ) const {
    return numStates;
}



size_t RateMatrix::size( void ) const {
    return numStates;
}



std::ostream& RevBayesCore::operator<<(std::ostream& o, const RateMatrix& x) {
    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    o << "[ ";
    o << std::fixed;
    o << std::setprecision(4);
    
    // print the RbMatrix with each column of equal width and each column centered on the decimal
    for (size_t i=0; i < x.size(); i++) {
        if (i == 0)
            o << "[ ";
        else 
            o << "  ";
        
        for (size_t j = 0; j < x.size(); ++j) 
        {
            if (j != 0)
                o << ", ";
            o << x[i][j];
        }
        o <<  " ]";
        
        if (i == x.size()-1)
            o << " ]";
        else 
            o << " ,\n";
        
    }
    
    o.setf(previousFlags);
    o.precision(previousPrecision);
    
    return o;
}

