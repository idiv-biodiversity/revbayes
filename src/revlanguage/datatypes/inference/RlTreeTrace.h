/**
 * @file
 * This file contains the declaration of the RevLanguage wrapper of the TreeTrace class.
 *
 * @brief Declaration of RlTreeTrace
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-08-06 20:14:22 +0200 (Mon, 06 Aug 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-09-06, version 1.0
 * @extends RbObject
 *
 * $Id: Real.h 1746 2012-08-06 18:14:22Z hoehna $
 */

#ifndef RlTreeTrace_H
#define RlTreeTrace_H

#include "TreeTrace.h"
#include "TypedDagNode.h"
#include "WorkspaceObject.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    template <typename treeType>
    class TreeTrace : public WorkspaceObject<RevBayesCore::TreeTrace<typename treeType::valueType> > {
        
    public:
        
        TreeTrace(void);                                                                                                        //!< Default constructor
        TreeTrace(const RevBayesCore::TreeTrace<typename treeType::valueType>& x);                                                                                              //!< Copy constructor
        TreeTrace(const TreeTrace& x);                                                                                              //!< Copy constructor
        
        // Basic utility functions
        virtual TreeTrace*                          clone(void) const;                                                          //!< Clone object
        void                                        constructInternalObject(void);                                              //!< We construct the a new internal MCMC object.
        static const std::string&                   getClassName(void);                                                         //!< Get class name
        static const TypeSpec&                      getClassTypeSpec(void);                                                     //!< Get class type spec
        const MemberRules&                          getMemberRules(void) const;                                                 //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                    //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                          //!< Print value (for user)
        
        // Member method inits
        const MethodTable&                          getMethods(void) const;                                                     //!< Get methods
        RevObject*                                  executeMethod(const std::string& name, const std::vector<Argument>& args);  //!< Override to map member methods to internal functions
        
    protected:
        
        void                                        setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var);     //!< Set member variable
                
    };
    
}


#include "ArgumentRules.h"
#include "MemberFunction.h"
#include "MethodTable.h"
#include "Natural.h"
#include "RlUtils.h"
#include "TreeSummary.h"

template <typename treeType>
RevLanguage::TreeTrace<treeType>::TreeTrace() : WorkspaceObject<RevBayesCore::TreeTrace<typename treeType::valueType> >() {
    
}


template <typename treeType>
RevLanguage::TreeTrace<treeType>::TreeTrace(const RevBayesCore::TreeTrace<typename treeType::valueType> &m) : WorkspaceObject<RevBayesCore::TreeTrace<typename treeType::valueType> >( new RevBayesCore::TreeTrace<typename treeType::valueType>( m ) ) {
    
}

template <typename treeType>
RevLanguage::TreeTrace<treeType>::TreeTrace(const TreeTrace &m) : WorkspaceObject<RevBayesCore::TreeTrace<typename treeType::valueType> >( m ) {
    
}


/** Clone object */
template <typename treeType>
RevLanguage::TreeTrace<treeType>* RevLanguage::TreeTrace<treeType>::clone(void) const {
    
	return new TreeTrace(*this);
}


template <typename treeType>
void RevLanguage::TreeTrace<treeType>::constructInternalObject( void ) {
    throw RbException("We do not support a constructor function for TreeTrace.");
}


/* Map calls to member methods */
template <typename treeType>
RevLanguage::RevObject* RevLanguage::TreeTrace<treeType>::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if (name == "summarize") {
        
        int b = static_cast<const Natural &>( args[0].getVariable()->getRevObject() ).getValue();
        RevBayesCore::TreeSummary<typename treeType::valueType> summary = RevBayesCore::TreeSummary<typename treeType::valueType>( *this->value );
        summary.summarize( b );
        summary.printTreeSummary(std::cerr);
        
        return NULL;
    } 
    
    return RevObject::executeMethod( name, args );
}


/** Get class name of object */
template <typename treeType>
const std::string& RevLanguage::TreeTrace<treeType>::getClassName(void) { 
    
    static std::string rbClassName = "TreeTrace<" + treeType::getClassName() + ">";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
template <typename treeType>
const RevLanguage::TypeSpec& RevLanguage::TreeTrace<treeType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( WorkspaceObject<RevBayesCore::TreeTrace<typename treeType::valueType> >::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** Return member rules (no members) */
template <typename treeType>
const RevLanguage::MemberRules& RevLanguage::TreeTrace<treeType>::getMemberRules(void) const {
    
    static MemberRules modelMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
//        modelMemberRules.push_back( new ArgumentRule("model", true, Model::getClassTypeSpec() ) );
//        modelMemberRules.push_back( new ArgumentRule("monitors", true, VectorRbPointer<Monitor>::getClassTypeSpec() ) );
//        modelMemberRules.push_back( new ArgumentRule("moves", true, VectorRbPointer<Move>::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return modelMemberRules;
}


/* Get method specifications */
template <typename treeType>
const RevLanguage::MethodTable& RevLanguage::TreeTrace<treeType>::getMethods(void) const {
    
    static MethodTable methods = MethodTable();
    static bool          methodsSet = false;
    
    if ( methodsSet == false ) {
        
        ArgumentRules* summarizeArgRules = new ArgumentRules();
        summarizeArgRules->push_back( new ArgumentRule("burnin", true, Natural::getClassTypeSpec(), new Natural(0)) );
        methods.addFunction("summarize", new MemberFunction( RlUtils::Void, summarizeArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &RevObject::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}

/** Get type spec */
template <typename treeType>
const RevLanguage::TypeSpec& RevLanguage::TreeTrace<treeType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
template <typename treeType>
void RevLanguage::TreeTrace<treeType>::printValue(std::ostream &o) const {
    
    o << "TreeTrace";
}


/** Set a member variable */
template <typename treeType>
void RevLanguage::TreeTrace<treeType>::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "xxx") {
        
    } 
    else {
        RevObject::setConstMemberVariable(name, var);
    }
}

#endif
