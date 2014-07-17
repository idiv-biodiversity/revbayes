/**
 * @file
 * This file contains the declaration of Move, which is the abstract
 * base class for moves used in the mcmc inference.
 *
 * @brief Declaration of the base class Move
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-08-17, version 1.0
 *
 * $Id$
 */

#ifndef MoveSchedule_H
#define MoveSchedule_H

#include "Move.h"
#include "RbVector.h"

#include <vector>

namespace RevBayesCore {
    
    class MoveSchedule {
        
    public:
        virtual                                                ~MoveSchedule(void);                                                                             //!< Destructor
        
        // pure virtual public methods
        virtual MoveSchedule*                                   clone(void) const = 0;
        virtual double                                          getNumberMovesPerIteration(void) const = 0;
        virtual Move&                                           nextMove(unsigned long g) = 0;
        
        // public methods
        void                                                    tune(void);                                                                                     //!< The the moves to achieve better performance.
        
    protected:
        MoveSchedule(RbVector<Move> *m);                                                                                                                  //!< Default constructor
                
        // Hidden member variables
        RbVector<Move>*                                         moves;
        
    };
    
}

#endif
