

#include "DagNode.h"
#include "DynamicNode.h"
#include "RbException.h"
#include "RbOptions.h"

using namespace RevBayesCore;

DagNode::DagNode( const std::string &n ) : 
        children(),
        name( n ), 
        parents(),
        touchedElements() {

}


DagNode::DagNode( const DagNode &n ) : 
        children(),
        name( n.name ),  
        parents( n.parents ),
        touchedElements( n.touchedElements ) {

    // add myself as a new child of all my parents
    for (std::set<const DagNode*>::iterator it = parents.begin(); it != parents.end(); ++it) {
        (*it)->addChild( this );
    }
}


DagNode::~DagNode( void ) {
    // we do not release anything here
    // children and parents need to be clean somewhere else!
    while ( !children.empty() ) {
        ( *children.begin() )->removeParent( this );
    }
    while ( !parents.empty() ) {
        const DagNode *theParent = ( *parents.begin() );
        theParent->removeChild( this );
        parents.erase( theParent );
    }

}

void DagNode::addChild(DagNode *child) const {
    children.insert( child );
}


/* Add parent node */
void DagNode::addParent( const DagNode *newParent ) {
    
    parents.insert( newParent );
    newParent->addChild( this );
}


/* Add the index of an element that has been touched */
void DagNode::addTouchedElementIndex(size_t i) {
    touchedElements.insert( i );
}


void DagNode::clearTouchedElementIndices( void ) {
    touchedElements.clear();
}


/** Clone the graph downstream from this node: clone children */
DagNode* DagNode::cloneDownstreamDag( std::map<const DagNode*, DagNode* >& newNodes ) const {
    
    if ( newNodes.find( this ) != newNodes.end() )
        return ( newNodes[ this ] );
    
    // Get pristine copy
    DagNode* copy = clone();
    
    // Add this node to the map
    newNodes[ this ] = copy;
    
    /* Make sure the children clone themselves */
    for( std::set<DagNode*>::const_iterator i = this->children.begin(); i != this->children.end(); i++ ) {
        DagNode *child = (*i)->cloneDownstreamDag( newNodes );
        child->swapParent(this, copy);
    }
    
    return copy;
}


void DagNode::collectDownstreamGraph(std::set<RevBayesCore::DagNode *> &nodes) {
    
    // for efficiency we check for multiple calls
    if ( nodes.find( this ) == nodes.end() ) {
        // first, add myself
        nodes.insert( this );
        
        // now, perform a recursive call
        for (std::set<DagNode*>::iterator it = children.begin(); it != children.end(); ++it) {
            (*it)->collectDownstreamGraph( nodes );
        }
    }
}


/**
 * Get all affected nodes this DAGNode.
 * This means we call getAffected() of all children. getAffected() is pure virtual.
 */
void DagNode::getAffectedNodes(std::set<DagNode *> &affected) {
    
    // get all my affected children
    for ( std::set<DagNode*>::iterator i = children.begin(); i != children.end(); i++ )
        (*i)->getAffected(affected, this);
}


const std::set<DagNode*>& DagNode::getChildren( void ) const {
    return children;
}


DagNode* DagNode::getFirstChild( void ) const {
    return *children.begin();
}


const std::string& DagNode::getName( void ) const {
    return name;
}


size_t DagNode::getNumberOfChildren( void ) const {
    return children.size();
}


const std::set<const DagNode*>& DagNode::getParents( void ) const {
    return parents;
}


/* Get the indices of all touched elements */
const std::set<size_t>& DagNode::getTouchedElementIndices( void ) const {
    return touchedElements;
}


bool DagNode::isClamped( void ) const {
    return false;
}


bool DagNode::isConstant( void ) const {
    return false;
}


bool DagNode::isStochastic( void ) const {
    return false;
}

/**
 * Keep the value of the node.
 * This function delegates the call to keepMe() and calls keepAffected() too.
 */
void DagNode::keep(void) {
    
    // keep myself first
    keepMe( this );
    
    // next, keep all my children
    keepAffected();
}

/**
 * Tell affected variable nodes to keep the current value.
 */
void DagNode::keepAffected() {
    
    // keep all my children
    for ( std::set<DagNode*>::iterator i = children.begin(); i != children.end(); i++ )
        (*i)->keepMe( this );
}

void DagNode::removeChild(DagNode *child) const {
    
    // test if we even have this node as a child
    if (children.find(child) != children.end()) {
        // we do not own our children! See addChildNode for explanation
        
        // remove the child from our list
        children.erase(child);
    }
}


/* Remove parent node */
void DagNode::removeParent( const DagNode *p ) {
    
    parents.erase( p );
    p->removeChild( this );
}


/** 
 * Replace the DAG node. We call replace for all children so that they get a new parent. We do not change the parents.
 */
void DagNode::replace( DagNode *n ) {
    
    // replace myself at all my children
    while ( !children.empty() ) {
        DagNode *child = *children.begin();
        child->swapParent( this, n);
    }
}


/**
 * Restore this DAGNode.
 * This means we call restoreMe() and restoreAffected(). There is a default implementation of restoreAffected()
 * which call restoreMe() of all children of this node. restoreMe() is pure virtual.
 */
void DagNode::restore(void) {
    
    // first restore myself
    restoreMe( this );
    
    // next, restore all my children
    restoreAffected();
}


/**
 * Restore all affected nodes this DAGNode.
 * This means we call restoreMe() of all children. restoreMe() is pure virtual.
 */
void DagNode::restoreAffected(void) {
    
    // next, restore all my children
    for ( std::set<DagNode *>::iterator i = children.begin(); i != children.end(); i++ )
        (*i)->restoreMe( this );
}


void DagNode::setName(std::string const &n) {
    name = n;
}


/* Swap parent node */
void DagNode::swapParent( const DagNode *oldParent, const DagNode *newParent ) {
    
    if ( parents.find( oldParent ) == parents.end() )
        throw RbException( "Node '" + oldParent->getName() + "' is not a parent" );
    
    oldParent->removeChild( this );
    if ( newParent != NULL ) {
        newParent->addChild( this );   
        addParent( newParent );
    }
    removeParent( oldParent );
    
    // delegate a call to swap the parameters in the internal implementation too.
    // this provides a hook to catch swaps of hard coded pointers to parameters.
    swapParameter(oldParent, newParent);
    
    this->touch();
}


/**
 * Touch the DAG node.
 *
 * This function should be called if the value of the variable has changed or if you want this node to be reevaluated. 
 * The function will automatically call the touchMe() which is implemented differently in the different DAG node types.
 *
 * Since the DAG node was touch and possibly changed, we tell affected variable nodes to keep the current value.
 */
void DagNode::touch() {
    // first touch myself
    touchMe( this );
    
    // next, touch all my children
    touchAffected();
}


/**
 * Tell affected variable nodes to touch themselves (i.e. that they've been touched).
 */
void DagNode::touchAffected() {
    
    // touch all my children
    for ( std::set<DagNode*>::iterator i = children.begin(); i != children.end(); i++ )
        (*i)->touchMe( this );
}