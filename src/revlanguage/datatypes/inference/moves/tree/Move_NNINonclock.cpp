//
//  MoveSlide.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "NearestNeighborInterchange_nonClock.h"
#include "RevObject.h"
#include "RbException.h"
#include "RealPos.h"
#include "Move_NNINonclock.h"
#include "RlTopology.h"
#include "Topology.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Move_NNINonclock::Move_NNINonclock() : Move() {
    
}


/** Clone object */
Move_NNINonclock* Move_NNINonclock::clone(void) const {
    
	return new Move_NNINonclock(*this);
}


void Move_NNINonclock::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    RevBayesCore::TypedDagNode<RevBayesCore::Topology> *tmp = static_cast<const Topology &>( tree->getRevObject() ).getDagNode();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::StochasticNode<RevBayesCore::Topology> *t = static_cast<RevBayesCore::StochasticNode<RevBayesCore::Topology> *>( tmp );
    value = new RevBayesCore::NearestNeighborInterchange_nonClock(t, w);
}


/** Get class name of object */
const std::string& Move_NNINonclock::getClassName(void) { 
    
    static std::string rbClassName = "Move_NNI";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move_NNINonclock::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** Return member rules (no members) */
const MemberRules& Move_NNINonclock::getMemberRules(void) const {
    
    static MemberRules nniMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        nniMemberRules.push_back( new ArgumentRule( "tree", false, Topology::getClassTypeSpec() ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getMemberRules();
        nniMemberRules.insert( nniMemberRules.end(), inheritedRules.begin(), inheritedRules.end() ); 
        
        rulesSet = true;
    }
    
    return nniMemberRules;
}

/** Get type spec */
const TypeSpec& Move_NNINonclock::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Get type spec */
void Move_NNINonclock::printValue(std::ostream &o) const {
    
    o << "NNI(";
    if (tree != NULL) {
        o << tree->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a NearestNeighborInterchange variable */
void Move_NNINonclock::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "tree" ) {
        tree = var;
    }
    else {
        Move::setConstMemberVariable(name, var);
    }
}
