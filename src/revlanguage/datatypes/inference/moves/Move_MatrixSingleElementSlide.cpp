/* 
 * File:   Move_MatrixSingleElementSlide.cpp
 * Author: nl
 * 
 * Created on 13 juillet 2014, 18:13
 */

#include "Move_MatrixSingleElementSlide.h"

#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "RlBoolean.h"
#include "ContinuousStochasticNode.h"

#include "Natural.h"
#include "RbException.h"
#include "Real.h"
#include "RealPos.h"
#include "RevObject.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "MatrixRealSingleElementSlidingMove.h"


using namespace RevLanguage;

Move_MatrixSingleElementSlide::Move_MatrixSingleElementSlide() : Move() {
    
}

/** Clone object */
Move_MatrixSingleElementSlide* Move_MatrixSingleElementSlide::clone(void) const {
    
	return new Move_MatrixSingleElementSlide(*this);
}


void Move_MatrixSingleElementSlide::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    double l = static_cast<const RealPos &>( lambda->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::TypedDagNode<RevBayesCore::MatrixReal >* tmp = static_cast<const RealMatrix &>( v->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode<RevBayesCore::MatrixReal > *n = static_cast<RevBayesCore::StochasticNode<RevBayesCore::MatrixReal> *>( tmp );
    bool t = static_cast<const RlBoolean &>( tune->getRevObject() ).getValue();
    value = new RevBayesCore::MatrixRealSingleElementSlidingMove(n, l, t, w);
}


/** Get class name of object */
const std::string& Move_MatrixSingleElementSlide::getClassName(void) { 
    
    static std::string rbClassName = "Move_MatrixSingleElementSlide";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move_MatrixSingleElementSlide::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** Return member rules (no members) */
const MemberRules& Move_MatrixSingleElementSlide::getMemberRules(void) const {
    
    static MemberRules scalingMoveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        scalingMoveMemberRules.push_back( new ArgumentRule( "x", false, RealMatrix::getClassTypeSpec() ) );
        scalingMoveMemberRules.push_back( new ArgumentRule( "lambda", true, RealPos::getClassTypeSpec() , new Real(1.0) ) );
        scalingMoveMemberRules.push_back( new ArgumentRule( "tune"  , true, RlBoolean::getClassTypeSpec(), new RlBoolean( true ) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getMemberRules();
        scalingMoveMemberRules.insert( scalingMoveMemberRules.end(), inheritedRules.begin(), inheritedRules.end() ); 
        
        rulesSet = true;
    }
    
    return scalingMoveMemberRules;
}

/** Get type spec */
const TypeSpec& Move_MatrixSingleElementSlide::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Move_MatrixSingleElementSlide::printValue(std::ostream &o) const {
    
    o << "Move_MatrixSingleElementSlide(";
    if (v != NULL) {
        o << v->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Move_MatrixSingleElementSlide::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "x" ) {
        v = var;
    }
    else if ( name == "lambda" ) {
        lambda = var;
    }
    else if ( name == "weight" ) {
        weight = var;
    }
    else if ( name == "tune" ) {
        tune = var;
    }
    else {
        Move::setConstMemberVariable(name, var);
    }
}

