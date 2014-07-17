/* 
 * File:   MatrixRealSingleElementSlidingMove.h
 * Author: nl
 *
 * Created on 13 juillet 2014, 17:55
 */

#ifndef MATRIXREALSINGLEELEMENTSLIDINGMOVE_H
#define	MATRIXREALSINGLEELEMENTSLIDINGMOVE_H

#include "SimpleMove.h"
#include "StochasticNode.h"

#include "RealMatrix.h"

#include <ostream>
#include <string>


namespace RevBayesCore {
    
    /**
     * @brief Scaling move of a single element randomly picked from a vector.
     *
     * 
     * This move randomly picks an element of a vector of positive real numbers,
     * proposes a scaling factor and then Slidings the value.
     * The actual scaling factor is computed by sf = exp( lambda * ( u - 0.5 ) )
     * where u ~ Uniform(0,1).
     * It generally makes more sense to apply the scaling move on a vector of positive
     * real numbers but technically it works on negative numbers too. However, 
     * the move will never change the sign of the value and thus is incomplete if applied
     * to variable defined on the whole real line.
     *
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @copyright GPL version 3
     *
     */
    class MatrixRealSingleElementSlidingMove : public SimpleMove {
        
    public:
        MatrixRealSingleElementSlidingMove(StochasticNode<MatrixReal>* n, double l, bool t, double w);                         //!< Constructor
        
        // Basic utility functions
        MatrixRealSingleElementSlidingMove*         clone(void) const;                                                                  //!< Clone this object.
        const std::string&                          getMoveName(void) const;                                                            //!< Get the name of the move for summary printing.
        void                                        swapNode(DagNode *oldN, DagNode *newN);                                             //!< Swap the variable if it was replaced.
        
    protected:
        
        double                                  performSimpleMove(void);                                                            //!< Perform move
        void                                    printParameterSummary(std::ostream &o) const;
        void                                    rejectSimpleMove(void);
        void                                    acceptSimpleMove(void);
        void                                    tune(void);
        void                                    touch( DagNode *toucher );
        
    private:
        
        StochasticNode<MatrixReal >*                variable;

        double                                      lambda;                                                                             //!< The Sliding parameter of the move (larger lambda -> larger proposals).
        //!< The two indices of the last modified element.
        size_t                                      indexa;    
        size_t                                      indexb;  
        double                                      storedValue;                                                                        //!< The stored value of the last modified element.
        
    };
    
}


#endif	/* MATRIXREALSINGLEELEMENTSLIDINGMOVE_H */

