#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Move_SingleElementScale.h"
#include "Natural.h"
#include "RbException.h"
#include "Real.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RlBoolean.h"
#include "SingleElementScaleMove.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "Vector.h"


using namespace RevLanguage;

Move_SingleElementScale::Move_SingleElementScale() : Move() {
    
}


/** Clone object */
Move_SingleElementScale* Move_SingleElementScale::clone(void) const {
    
	return new Move_SingleElementScale(*this);
}


void Move_SingleElementScale::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    double l = static_cast<const RealPos &>( lambda->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::TypedDagNode<std::vector<double> >* tmp = static_cast<const Vector<RealPos> &>( x->getRevObject() ).getDagNode();
    const std::set<const RevBayesCore::DagNode*> &p = tmp->getParents();
    std::vector< RevBayesCore::StochasticNode<double> *> n;
    for (std::set<const RevBayesCore::DagNode*>::const_iterator it = p.begin(); it != p.end(); ++it) 
    {
        const RevBayesCore::StochasticNode<double> *theNode = static_cast< const RevBayesCore::StochasticNode<double>* >( *it );
        n.push_back( const_cast< RevBayesCore::StochasticNode<double>* >( theNode ) );
    }
    bool t = static_cast<const RlBoolean &>( tune->getRevObject() ).getValue();
    value = new RevBayesCore::SingleElementScaleMove(n, l, t, w);
}


/** Get class name of object */
const std::string& Move_SingleElementScale::getClassName(void) {
    
    static std::string rbClassName = "Move_SingleElementScale";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move_SingleElementScale::getClassTypeSpec(void) {
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** Return member rules (no members) */
const MemberRules& Move_SingleElementScale::getMemberRules(void) const {
    
    static MemberRules scalingMoveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        scalingMoveMemberRules.push_back( new ArgumentRule( "x", false, Vector<RealPos>::getClassTypeSpec() ) );
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
const TypeSpec& Move_SingleElementScale::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Move_SingleElementScale::printValue(std::ostream &o) const {
    
    o << "SingleElementScale(";
    if (x != NULL) {
        o << x->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Move_SingleElementScale::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "x" ) {
        x = var;
    }
    else if ( name == "lambda" ) {
        lambda = var;
    }
    else if ( name == "tune" ) {
        tune = var;
    }
    else {
        Move::setConstMemberVariable(name, var);
    }
}
