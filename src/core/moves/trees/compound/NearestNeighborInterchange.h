/**
 * @file
 * This file contains the declaration of NearestNeighborInterchange, 
 * which randomly draws a node in the tree and exchanges its two neighbors.
 *
 * @brief Declaration of NearestNeighborInterchange
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-07-12 16:14:14 +0200 (Thu, 12 Jul 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-07-12, version 1.0
 *
 * $Id: ScaleMove.h 1677 2012-07-12 14:14:14Z hoehna $
 */

#ifndef NearestNeighborInterchange_H
#define NearestNeighborInterchange_H

#include <ostream>
#include <set>
#include <string>

#include "SimpleMove.h"
#include "StochasticNode.h"
#include "TimeTree.h"
#include "Tree.h"

namespace RevBayesCore {

    class NearestNeighborInterchange : public SimpleMove {
    
    public:
        NearestNeighborInterchange( StochasticNode<TimeTree> *n, double weight);                                            //!<  constructor
    
        // Basic utility functions
        NearestNeighborInterchange*     clone(void) const;                                                                  //!< Clone object
        void                            swapNode(DagNode *oldN, DagNode *newN);
    
    protected:
        const std::string&              getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                          performSimpleMove(void);                                                            //!< Perform move
        void                            rejectSimpleMove(void);
    
    private:
        
        // member variables
        StochasticNode<TimeTree>*       variable;
    
        // stored objects to undo proposal
        TopologyNode*                   storedChoosenNode;
        TopologyNode*                   storedUncle;
        double                          storedChoosenAge;
        double                          storedUnclesAge;

    };
    
}

#endif

