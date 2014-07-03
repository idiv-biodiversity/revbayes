#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstantNode.h"
#include "ContinuousStochasticNode.h"
#include "RevObject.h"
#include "RbException.h"
#include "Real.h"
#include "RealPos.h"
#include "RlBoolean.h"
#include "Move_Slide.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Move_Slide::Move_Slide() : Move() {
    
}


/** Clone object */
Move_Slide* Move_Slide::clone(void) const {
    
	return new Move_Slide(*this);
}


void Move_Slide::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    double d = static_cast<const RealPos &>( delta->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    bool t = static_cast<const RlBoolean &>( tune->getRevObject() ).getValue();
    RevBayesCore::TypedDagNode<double>* tmp = static_cast<const Real &>( x->getRevObject() ).getDagNode();
    RevBayesCore::ContinuousStochasticNode *n = static_cast<RevBayesCore::ContinuousStochasticNode *>( tmp );
    value = new RevBayesCore::SlidingMove(n, d, t, w);
    
}


/** Get class name of object */
const std::string& Move_Slide::getClassName(void) { 
    
    static std::string rbClassName = "Move_Slide";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move_Slide::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** Return member rules (no members) */
const MemberRules& Move_Slide::getMemberRules(void) const {
    
    static MemberRules slidingMoveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        slidingMoveMemberRules.push_back( new ArgumentRule( "x", false, Real::getClassTypeSpec() ) );
        slidingMoveMemberRules.push_back( new ArgumentRule( "delta", true, RealPos::getClassTypeSpec() , new RealPos(1.0) ) );
        slidingMoveMemberRules.push_back( new ArgumentRule( "tune"  , true, RlBoolean::getClassTypeSpec(), new RlBoolean( true ) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getMemberRules();
        slidingMoveMemberRules.insert( slidingMoveMemberRules.end(), inheritedRules.begin(), inheritedRules.end() ); 

        rulesSet = true;
    }
    
    return slidingMoveMemberRules;
}

/** Get type spec */
const TypeSpec& Move_Slide::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}




/** Get type spec */
void Move_Slide::printValue(std::ostream &o) const {
    
    o << "Move_Slide";
    
}


/** Set a member variable */
void Move_Slide::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "x" ) 
    {
        x = var;
    }
    else if ( name == "delta" ) 
    {
        delta = var;
    }
    else if ( name == "tune" ) 
    {
        tune = var;
    }
    else 
    {
        Move::setConstMemberVariable(name, var);
    }
    
}
