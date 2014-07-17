/* 
 * File:   MultivariatePhyloProcess.h
 * Author: nl
 *
 * Created on 16 juillet 2014, 20:17
 */

#ifndef MULTIVARIATEPHYLOPROCESS_H
#define	MULTIVARIATEPHYLOPROCESS_H


#include "MatrixReal.h"
#include "TimeTree.h"

#include <vector>
#include <string>

namespace RevBayesCore {
    
    class MultivariatePhyloProcess : public MatrixReal {
    
    public:
        MultivariatePhyloProcess(void);                                                                                             //!< Default constructor
        MultivariatePhyloProcess(const TimeTree* intree, size_t indim);                                                                                             //!< Default constructor
        MultivariatePhyloProcess(const MultivariatePhyloProcess& t);                                                                        //!< Copy constructor
        virtual                                    ~MultivariatePhyloProcess(void);                                                                                                        //!< Destructor
    
        MultivariatePhyloProcess&                           operator=(const MultivariatePhyloProcess& t);
    
        // Basic utility functions
        MultivariatePhyloProcess*                   clone(void) const;                                                      //!< Clone object
    
        const TimeTree*                             getTimeTree() const {return tree;}
        size_t                                      getDim() const {return getNumberOfColumns();}
        

        // MultivariatePhyloProcess functions
        double                                      getRootVal(int k) const;
        double                                      getMean(int k) const;
        double                                      getStdev(int k) const;
        void                                        recursiveGetStats(int k, const TopologyNode& from, double& e1, double& e2, int& n) const;
        
    protected:
        // void                                        resizeElementVectors(size_t n);
    
    private:

        const TimeTree*                     tree;

    };

}

// Global functions using the class
// std::ostream&                       operator<<(std::ostream& o, const RevBayesCore::MultivariatePhyloProcess& x);                                         //!< Overloaded output operator

#endif	/* MULTIVARIATEPHYLOPROCESS_H */

