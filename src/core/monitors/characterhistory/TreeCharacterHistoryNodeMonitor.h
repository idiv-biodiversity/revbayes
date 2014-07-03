//
//  TreeCharacterHistoryNodeMonitor.h
//  rb_mlandis
//
//  Created by Michael Landis on 5/5/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__TreeCharacterHistoryNodeMonitor__
#define __rb_mlandis__TreeCharacterHistoryNodeMonitor__


#include "AbstractCharacterData.h"
#include "AbstractTreeHistoryCtmc.h"
#include "DagNode.h"
#include "Model.h"
#include "Monitor.h"
#include "BranchHistory.h"
#include "RbException.h"
#include "StochasticNode.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

namespace RevBayesCore {
    
    template<class charType, class treeType>
    class TreeCharacterHistoryNodeMonitor : public Monitor {
        
    public:
        // Constructors and Destructors
        TreeCharacterHistoryNodeMonitor(StochasticNode<AbstractCharacterData>* s, TypedDagNode<treeType> *t, unsigned long g, const std::string &fname, const std::string &del, bool pp=true, bool l=true, bool pr=true, bool ap=false, bool sm=true, bool sne=false, bool ste=true);
        
        // new TreeCharacterHistoryNodeMonitor( tau, bh_vector_stochastic, 10, filepath + "rb.tree_chars.txt", "\t"));
        
        TreeCharacterHistoryNodeMonitor(const TreeCharacterHistoryNodeMonitor& f);
        
        // basic methods
        TreeCharacterHistoryNodeMonitor*          clone(void) const;                                                  //!< Clone the object
        
        // Monitor functions
        void                                monitor(unsigned long gen);                                         //!< Monitor at generation gen
        void                                swapNode(DagNode *oldN, DagNode *newN);
        
        // FileMonitor functions
        void                                closeStream(void);                                                  //!< Close stream after finish writing
        void                                openStream(void);                                                   //!< Open the stream for writing
        void                                printHeader(void);                                                  //!< Print header
        
    private:
        std::string                         buildExtendedNewick(void);
        std::string                         buildExtendedNewick(TopologyNode* n);
        std::string                         buildNumEventsStr(TopologyNode* n);
        std::string                         buildNumEventsStr(TopologyNode* n, unsigned state);
        std::string                         buildNumEventsForTreeStr(unsigned state);
        std::string                         buildCharacterHistoryString(TopologyNode* n, std::string brEnd="child");
        
        // the stream to print
        std::fstream                        outStream;
        
        // parameters
        StochasticNode<AbstractCharacterData>* variable;
        TypedDagNode<treeType>*             tree;
        std::set<DagNode *>                 nodeVariables;
        std::string                         filename;
        std::string                         separator;
        bool                                posterior;
        bool                                prior;
        bool                                likelihood;
        bool                                append;
        bool                                showMetadata;
        bool                                showNumEvents;
        bool                                showTreeEvents;
        size_t                              numStates;
        
    };
    
}

/* Constructor */
template<class charType, class treeType>
RevBayesCore::TreeCharacterHistoryNodeMonitor<charType, treeType>::TreeCharacterHistoryNodeMonitor(StochasticNode<AbstractCharacterData>* s, TypedDagNode<treeType>* t, unsigned long g, const std::string &fname, const std::string &del, bool pp, bool l, bool pr, bool ap, bool sm, bool sne, bool ste) :
Monitor(g,s),
outStream(),
variable(s),
tree( t ),
filename( fname ),
separator( del ),
posterior( pp ),
prior( pr ),
likelihood( l ),
append(ap),
showMetadata(sm),
showNumEvents(sne),
showTreeEvents(ste),
numStates(0)
{
    nodes.push_back(s);
    nodes.push_back(t);
    
    numStates = static_cast<const DiscreteCharacterState&>(s->getValue().getCharacter(0,0)).getNumberOfStates();
}

template<class charType, class treeType>
RevBayesCore::TreeCharacterHistoryNodeMonitor<charType, treeType>::TreeCharacterHistoryNodeMonitor(const TreeCharacterHistoryNodeMonitor &m) :
Monitor( m ),
outStream( ),
variable(m.variable),
tree( m.tree ),
nodeVariables( m.nodeVariables ),
showMetadata(m.showMetadata),
showNumEvents(m.showNumEvents),
showTreeEvents(m.showTreeEvents),
numStates(m.numStates)
{    
    filename    = m.filename;
    separator   = m.separator;
    prior       = m.prior;
    posterior   = m.posterior;
    likelihood  = m.likelihood;
    append      = m.append;
}


/* Clone the object */
template<class charType, class treeType>
RevBayesCore::TreeCharacterHistoryNodeMonitor<charType,treeType>* RevBayesCore::TreeCharacterHistoryNodeMonitor<charType, treeType>::clone(void) const {
    
    return new TreeCharacterHistoryNodeMonitor<charType, treeType>(*this);
}

template<class charType, class treeType>
void RevBayesCore::TreeCharacterHistoryNodeMonitor<charType, treeType>::closeStream() {
    outStream.close();
}

template<class charType, class treeType>
std::string RevBayesCore::TreeCharacterHistoryNodeMonitor<charType, treeType>::buildExtendedNewick( void ) {
    //tree->getValue().getRoot().setNewickNeedsRefreshing(true);
    std::string newick = buildExtendedNewick( &tree->getValue().getRoot() );
    return newick;
}

template<class charType, class treeType>
std::string RevBayesCore::TreeCharacterHistoryNodeMonitor<charType, treeType>::buildCharacterHistoryString(TopologyNode* n, std::string infoStr)
{

    AbstractTreeHistoryCtmc<charType, treeType>* p = static_cast< AbstractTreeHistoryCtmc<charType, treeType>* >(&variable->getDistribution());
    const BranchHistory& bh = p->getHistory(*n);
    std::stringstream ss;
    
    if (infoStr=="child")
    {
        std::vector<CharacterEvent*> characters = bh.getChildCharacters();
        
        for (size_t i = 0; i < characters.size(); i++)
        {
            if (i != 0)
                ss << ",";
            ss << characters[i]->getState();
        }
    }
    else if (infoStr=="parent")
    {
        std::vector<CharacterEvent*> characters = bh.getParentCharacters();
        for (size_t i = 0; i < characters.size(); i++)
        {
            if (i != 0)
                ss << ",";
            ss << characters[i]->getState();
        }
    }
    else if (infoStr=="state_into")
    {
        // loop over events
        const std::multiset<CharacterEvent*,CharacterEventCompare>& evts = bh.getHistory();
        std::multiset<CharacterEvent*,CharacterEventCompare>::const_iterator it;
        
        std::vector<unsigned> v(numStates,0);
        for (it = evts.begin(); it != evts.end(); it++)
        {
            unsigned s = (*it)->getState();
            v[s] += 1;
        }

        // loop over states
        for (size_t i = 0; i < v.size(); i++)
        {
            if (i != 0)
                ss << ",";
            ss << i << ":" << v[i];
        }
    }
    else if (infoStr=="state_betw")
    {
        // loop over events
        const std::multiset<CharacterEvent*,CharacterEventCompare>& evts = bh.getHistory();
        std::multiset<CharacterEvent*,CharacterEventCompare>::const_iterator it;
        std::vector<CharacterEvent*> characters = bh.getParentCharacters();
        
        std::vector<unsigned> v(numStates*numStates,0);
        for (it = evts.begin(); it != evts.end(); it++)
        {
            size_t idx = (*it)->getIndex();
            unsigned from = characters[idx]->getState();
            unsigned to = (*it)->getState();
            v[ numStates*from + to ] += 1;
            characters[idx] = (*it);
        }
        
        // loop over states
        for (size_t i = 0; i < numStates; i++)
        {
            if (i != 0)
                ss << ",";
            ss << i << ":{";
            for (size_t j = 0; j < numStates; j++)
            {
                if (j != 0)
                    ss << ",";
                ss << j << ":" << v[numStates*i + j];
            }
            ss << "}";
        }
    
    }
    
    return ss.str();

}

/* Build newick string */
template<class charType, class treeType>
std::string RevBayesCore::TreeCharacterHistoryNodeMonitor<charType, treeType>::buildExtendedNewick( TopologyNode* n ) {
    // create the newick string
    std::stringstream o;
    
    // extended data is only found on admixture nodes
    std::string additionalInfo = "";
    
    // loop over admixture nodes per branch
    std::stringstream characterStream;
    
    double br = 1.0;
    
    if (showMetadata)
    {
        characterStream << "[";
        
        // character history
        characterStream << "&ch={" << buildCharacterHistoryString(n,"child") << "}";
        characterStream << ",&pa={" << buildCharacterHistoryString(n,"parent") << "}";
        
        // # events
        characterStream << ",&state_into={" << buildCharacterHistoryString(n,"state_into") << "}";
        characterStream << ",&state_betw={" << buildCharacterHistoryString(n,"state_betw") << "}";
        
        // ... whatever else
        characterStream << "]";
        
        additionalInfo = characterStream.str();
    }
    
    // test whether this is a internal or external node
    if (n->isTip()) {
        // this is a tip so we just return the name of the node
        o << n->getName() << additionalInfo << ":" << n->getBranchLength();
    }
    
    else {
        o << "(";
        for (size_t i=0; i<(n->getNumberOfChildren()-1); i++) {
            o << buildExtendedNewick( &n->getChild(i) ) << ",";
        }
        o << buildExtendedNewick( &n->getChild(n->getNumberOfChildren()-1) ) << ")" << additionalInfo << ":" << n->getBranchLength() * br;
    }
    
    return o.str();
}

/* Build newick string */
template<class charType, class treeType>
std::string RevBayesCore::TreeCharacterHistoryNodeMonitor<charType, treeType>::buildNumEventsStr(TopologyNode* nd)
{
    
    AbstractTreeHistoryCtmc<charType, treeType>* p = static_cast< AbstractTreeHistoryCtmc<charType, treeType>* >(&variable->getDistribution());
    BranchHistory* bh = &p->getHistory(*nd);
    
    std::stringstream ss;
    double eventRatio = (double)bh->getNumEvents() / 1.0; //nd->getBranchLength();
    
    ss << eventRatio;
    return ss.str();
    
}



/* Build newick string */
template<class charType, class treeType>
std::string RevBayesCore::TreeCharacterHistoryNodeMonitor<charType, treeType>::buildNumEventsStr(TopologyNode* nd, unsigned state)
{
    
    AbstractTreeHistoryCtmc<charType, treeType>* p = static_cast< AbstractTreeHistoryCtmc<charType, treeType>* >(&variable->getDistribution());
    BranchHistory* bh = &p->getHistory(*nd);
    
    const std::multiset<CharacterEvent*,CharacterEventCompare>& evts = bh->getHistory();
    std::multiset<CharacterEvent*,CharacterEventCompare>::const_iterator it;
    
    std::stringstream ss;
    
    unsigned v = 0;
    for (it = evts.begin(); it != evts.end(); it++)
    {
        if ( (*it)->getState() == state )
            v++;
    }
    double eventRatio = (double)v / 1.0;//nd->getBranchLength();

    ss << eventRatio;
    return ss.str();
    
}

template<class charType, class treeType>
std::string RevBayesCore::TreeCharacterHistoryNodeMonitor<charType, treeType>::buildNumEventsForTreeStr(unsigned state)
{
    AbstractTreeHistoryCtmc<charType, treeType>* p = static_cast< AbstractTreeHistoryCtmc<charType, treeType>* >(&variable->getDistribution());
    
    const std::vector<TopologyNode*>& nds = tree->getValue().getNodes();
    unsigned v = 0;
    std::stringstream ss;
    
    for (size_t i = 0; i < nds.size(); i++)
    {
        BranchHistory* bh = &p->getHistory(*nds[i]);
        
        const std::multiset<CharacterEvent*,CharacterEventCompare>& evts = bh->getHistory();
        std::multiset<CharacterEvent*,CharacterEventCompare>::const_iterator it;
        
        
        for (it = evts.begin(); it != evts.end(); it++)
        {
            if ( (*it)->getState() == state )
                v++;
        }
        
    }
    double eventRatio = (double)v / 1.0;//nd->getBranchLength();
    ss << eventRatio;
    return ss.str();
}


/** Monitor value at generation gen */
template<class charType, class treeType>
void RevBayesCore::TreeCharacterHistoryNodeMonitor<charType, treeType>::monitor(unsigned long gen) {
    
    // get the printing frequency
    unsigned long samplingFrequency = printgen;
    
    if (gen % samplingFrequency == 0) {
        // print the iteration number first
        outStream << gen;
        
        if ( posterior ) {
            // add a separator before every new element
            outStream << separator;
            
            const std::vector<DagNode*> &n = model->getDagNodes();
            double pp = 0.0;
            for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it) {
                pp += (*it)->getLnProbability();
            }
            outStream << pp;
        }
        
        if ( likelihood ) {
            // add a separator before every new element
            outStream << separator;
            
            const std::vector<DagNode*> &n = model->getDagNodes();
            double pp = 0.0;
            for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it) {
                if ( (*it)->isClamped() ) {
                    pp += (*it)->getLnProbability();
                }
            }
            outStream << pp;
        }
        
        if ( prior ) {
            // add a separator before every new element
            outStream << separator;
            
            const std::vector<DagNode*> &n = model->getDagNodes();
            double pp = 0.0;
            for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it) {
                if ( !(*it)->isClamped() ) {
                    pp += (*it)->getLnProbability();
                }
            }
            outStream << pp;
        }
        
        if (showNumEvents)
        {
            for (size_t s = 0; s < numStates; s++)
            {
                outStream << separator << buildNumEventsForTreeStr(s);
            }
            for (size_t i = 0; i < tree->getValue().getNumberOfNodes(); i++)
            {
                for (size_t s = 0; s < numStates; s++)
                {
                    TopologyNode* nd = &tree->getValue().getNode(i);
                    if (nd->isRoot())
                        continue;
                    outStream << separator << buildNumEventsStr(nd, s);
                }
            }
        }
        
        if (showTreeEvents)
        {
            // add a separator before the tree
            outStream << separator;
            outStream << buildExtendedNewick();
            
        }
        
        outStream << std::endl;
        
    }
}


/** open the file stream for printing */
template<class charType, class treeType>
void RevBayesCore::TreeCharacterHistoryNodeMonitor<charType, treeType>::openStream(void) {
    
    // open the stream to the file
    if (append)
        outStream.open( filename.c_str(), std::fstream::out | std::fstream::app);
    else
        outStream.open( filename.c_str(), std::fstream::out);
}

/** Print header for monitored values */
template<class charType, class treeType>
void RevBayesCore::TreeCharacterHistoryNodeMonitor<charType, treeType>::printHeader() {
    
    // print one column for the iteration number
    outStream << "Iteration";
    
    if ( posterior ) {
        // add a separator before every new element
        outStream << separator;
        outStream << "Posterior";
    }
    
    if ( likelihood ) {
        // add a separator before every new element
        outStream << separator;
        outStream << "Likelihood";
    }
    
    if ( prior ) {
        // add a separator before every new element
        outStream << separator;
        outStream << "Prior";
    }
    
    if (showNumEvents)
    {
        for (size_t s = 0; s < numStates; s++)
        {
            outStream << separator << "t" << s;
        }
        for (size_t i = 0; i < tree->getValue().getNumberOfNodes(); i++)
        {
            for (size_t s = 0; s < numStates; s++)
            {
                TopologyNode* nd = &tree->getValue().getNode(i);
                if (nd->isRoot())
                    continue;
                outStream << separator << "b" << nd->getIndex() << "_s" << s;
            }
        }
    }
    
    if (showTreeEvents)
    {
        // add a separator tree
        outStream << separator << "Tree";
    }
    
    // end line of stream
    outStream << std::endl;
}

template<class charType, class treeType>
void RevBayesCore::TreeCharacterHistoryNodeMonitor<charType, treeType>::swapNode(DagNode *oldN, DagNode *newN) {
    
    bool found = false;
    if ( oldN == tree )
    {
        tree = static_cast< TypedDagNode< treeType > *>( newN );
        found = true;
    }
    else if (oldN == variable)
    {
        variable = static_cast<StochasticNode<AbstractCharacterData>* >(newN);
    }

    /*
     if (found == false)
     {
     // error catching
     if ( nodeVariables.find(oldN) == nodeVariables.end() ) {
     throw RbException("Cannot replace DAG node in this monitor because the monitor doesn't hold this DAG node.");
     }
     
     nodeVariables.erase( oldN );
     nodeVariables.insert( newN );
     }
     */
    
    // delegate to base class
    Monitor::swapNode(oldN, newN);
}


#endif /* defined(__rb_mlandis__TreeCharacterHistoryNodeMonitor__) */
