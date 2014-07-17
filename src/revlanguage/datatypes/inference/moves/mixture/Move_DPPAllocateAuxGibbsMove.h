#ifndef Move_DPPAllocateAuxGibbsMove_H
#define Move_DPPAllocateAuxGibbsMove_H

#include "MoveOld.h"
#include "RlMove.h"
#include "SimpleMove.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    /**
     * @brief Rev Wrapper of a scaling move on all elements of a real valued vector.
     *
     * This class is the RevLanguage wrapper of Move_DPPAllocateAuxGibbsMove.
     *
     * @author The RevBayes Development Core Team (Tracy Heath)
     * @copyright GPL version 3
     * @since 2013-11-17, version 1.0
     */
    template <typename valType>
    class Move_DPPAllocateAuxGibbsMove : public Move {
        
    public:
        
        Move_DPPAllocateAuxGibbsMove(void);                                                                                                                   //!< Default constructor
        
        // Basic utility functions
        virtual Move_DPPAllocateAuxGibbsMove*                        clone(void) const;                                                                              //!< Clone the object
        void                                        constructInternalObject(void);                                                                  //!< We construct the a new internal move.
        static const std::string&                   getClassName(void);                                                                             //!< Get class name
        static const TypeSpec&                      getClassTypeSpec(void);                                                                         //!< Get class type spec
        const MemberRules&                          getMemberRules(void) const;                                                                     //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                                        //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                                              //!< Print value (for user)
        
    protected:
        
        void                                        setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var);             //!< Set member variable
        
        RevPtr<const Variable>                      x;                                                                                              //!< The variable holding the real valued vector.
        RevPtr<const Variable>                      nAux;                                                                                         //!< The variable for the tuning parameter.
        
    };
    
}

#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "DPPAllocateAuxGibbsMove.h"
#include "Natural.h"
#include "RbException.h"
#include "Real.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RlBoolean.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "Vector.h"
#include "Integer.h"


using namespace RevLanguage;

template <class valType>
Move_DPPAllocateAuxGibbsMove<valType>::Move_DPPAllocateAuxGibbsMove() : Move() {
    
}


/** Clone object */
template <class valType>
Move_DPPAllocateAuxGibbsMove<valType>* Move_DPPAllocateAuxGibbsMove<valType>::clone(void) const {
    
	return new Move_DPPAllocateAuxGibbsMove<valType>(*this);
}


template <class valType>
void Move_DPPAllocateAuxGibbsMove<valType>::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new vector-scale move
    int na = static_cast<const Integer &>( nAux->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::TypedDagNode<std::vector<typename valType::valueType> >* tmp = static_cast<const Vector<valType> &>( x->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode< std::vector<typename valType::valueType> > *sn = static_cast<RevBayesCore::StochasticNode<std::vector<typename valType::valueType> > *>( tmp );
    
    value = new RevBayesCore::DPPAllocateAuxGibbsMove<typename valType::valueType>(sn, na, w);
}


/** Get class name of object */
template <class valType>
const std::string& Move_DPPAllocateAuxGibbsMove<valType>::getClassName(void) { 
    
    static std::string rbClassName = "Move_DPPAllocateAuxGibbsMove";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
template <class valType>
const RevLanguage::TypeSpec& Move_DPPAllocateAuxGibbsMove<valType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** Return member rules (no members) */
template <class valType>
const MemberRules& Move_DPPAllocateAuxGibbsMove<valType>::getMemberRules(void) const {
    
    static MemberRules dppMove;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        dppMove.push_back( new ArgumentRule( "x", false, Vector<valType>::getClassTypeSpec() ) );
        dppMove.push_back( new ArgumentRule( "numAux", true, Integer::getClassTypeSpec() , new Integer(4) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getMemberRules();
        dppMove.insert( dppMove.end(), inheritedRules.begin(), inheritedRules.end() ); 
        
        rulesSet = true;
    }
    
    return dppMove;
}

/** Get type spec */
template <class valType>
const RevLanguage::TypeSpec& Move_DPPAllocateAuxGibbsMove<valType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
template <class valType>
void Move_DPPAllocateAuxGibbsMove<valType>::printValue(std::ostream &o) const {
    
    o << "Move_DPPAllocateAuxGibbsMove(";
    if (x != NULL) {
        o << x->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
template <class valType>
void Move_DPPAllocateAuxGibbsMove<valType>::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "x" ) {
        x = var;
    }
    else if ( name == "numAux" ) {
        nAux = var;
    }
    else {
        Move::setConstMemberVariable(name, var);
    }
}


#endif
