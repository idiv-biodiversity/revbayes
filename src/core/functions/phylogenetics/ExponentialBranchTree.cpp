//
//  ExponentialBranchTree.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-24.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#include "ExponentialBranchTree.h"
#include <stdlib.h>

using namespace RevBayesCore;


// constructor(s)
ExponentialBranchTree::ExponentialBranchTree(const TypedDagNode< TimeTree > *t, const TypedDagNode< MultivariatePhyloProcess > *p, const TypedDagNode<double>* o, const TypedDagNode< int >* i): 

    TypedFunction< std::vector< double > >( new std::vector< double >(p->getValue().getTimeTree()->getNumberOfNodes() -1, 0.0 ) ),
    tau(t), process(p), offset(o), traitindex(i) {
    
    this->addParameter( tau );
    this->addParameter( process );
    if (offset != NULL) {
         this->addParameter( offset );   
    }    
    if (traitindex != NULL) {
        this->addParameter(traitindex) ;
    }
    update();
}

// constructor(s)
/*
ExponentialBranchTree::ExponentialBranchTree(const TypedDagNode< TimeTree > *t,  const TypedDagNode< MatrixReal >* m, const TypedDagNode<double>* o, const TypedDagNode< int >* i): TypedFunction< std::vector< double > >( new std::vector< double >(t->getValue().getNumberOfNodes() -1, 0.0 ) ), tau(t),
mvtnodeval(m), offset(o), traitindex(i) {
    this->addParameter( tau );
    if (mvtnodeval != NULL) {
         this->addParameter( mvtnodeval );   
    }    
    if (offset != NULL) {
         this->addParameter( offset );   
    }    
    if (traitindex != NULL) {
        this->addParameter(traitindex) ;
    }
    update();
}
*/

ExponentialBranchTree::ExponentialBranchTree(const ExponentialBranchTree &n):

    TypedFunction< std::vector< double > >( n ), 
        tau(n.tau), process( n.process ), offset( n.offset ), traitindex( n.traitindex) {
    
}


ExponentialBranchTree* ExponentialBranchTree::clone(void) const {
    return new ExponentialBranchTree( *this );
}


void ExponentialBranchTree::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
    if ( oldP == process ) {
        process = static_cast< const TypedDagNode<MultivariatePhyloProcess> * >( newP );
    }

    if (oldP == offset) {
        offset = static_cast<const TypedDagNode< double >* >(newP);
    }
    
    if (oldP == traitindex) {
        traitindex = static_cast<const TypedDagNode< int >* >(newP);
    }
}

void ExponentialBranchTree::update(void)    {
    
    // get the root
    const TopologyNode& root = process->getValue().getTimeTree()->getRoot();
    recursiveUpdate(root);
}

int ExponentialBranchTree::getTraitIndex()  {
    return traitindex->getValue() - 1;
}

void ExponentialBranchTree::recursiveUpdate(const RevBayesCore::TopologyNode &from)    {

    size_t index = from.getIndex();

    if (! from.isRoot())    {
        // get the index
        
        size_t upindex = from.getParent().getIndex();

        if (getTraitIndex() >= 0)    {
            
            double x1 = process->getValue()[index][getTraitIndex()] + offset->getValue();
            double x2 = process->getValue()[upindex][getTraitIndex()] + offset->getValue();
            if (from.getParent().isRoot())  {
                x2 = offset->getValue();
            }
            double y = 0.5 * (exp(x1) + exp(x2));
        
            // we store this val here
            (*value)[index] = y;
            
        }
        else    {

            std::cerr << "should not be in simple univariate case\n";
            exit(1);
            /*
            double x1 = nodeval->getValue()[index];
            double x2 = nodeval->getValue()[upindex];
            
            double y = 0.5 * (exp(x1) + exp(x2));
        
            // we store this val here
            (*value)[index] = y;
             */
            
        }        
    }
    
    // simulate the val for each child (if any)
    size_t numChildren = from.getNumberOfChildren();
    for (size_t i = 0; i < numChildren; ++i) {
        const TopologyNode& child = from.getChild(i);
        recursiveUpdate(child);
    }
}

