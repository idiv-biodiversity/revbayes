/**
 * @file
 * This file contains the implementation of the RevLanguage wrapper of the AncestralStateTrace class.
 *
 * @brief Implementation of RlAncestralStateTrace
 *
 * (c) Copyright 2014-
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: RlAncestralStateTrace.h $
 */

#include "RlAncestralStateTrace.h"
#include "AncestralStateTrace.h"

#include "ArgumentRules.h"
#include "MemberProcedure.h"
#include "MethodTable.h"
#include "Natural.h"
#include "RlUtils.h"

using namespace RevLanguage;

AncestralStateTrace::AncestralStateTrace() : WorkspaceToCoreWrapperObject<RevBayesCore::AncestralStateTrace>() {

	ArgumentRules* summarizeArgRules = new ArgumentRules();
    summarizeArgRules->push_back( new ArgumentRule("burnin", Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(0)) );
    
}



AncestralStateTrace::AncestralStateTrace(const RevBayesCore::AncestralStateTrace& m) : WorkspaceToCoreWrapperObject<RevBayesCore::AncestralStateTrace>( new RevBayesCore::AncestralStateTrace( m ) ) {
    
	ArgumentRules* summarizeArgRules = new ArgumentRules();
    summarizeArgRules->push_back( new ArgumentRule("burnin", Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(0)) );

}

AncestralStateTrace::AncestralStateTrace(const AncestralStateTrace& m) : WorkspaceToCoreWrapperObject<RevBayesCore::AncestralStateTrace>( m ) {
    
}



/** Clone object */

AncestralStateTrace* AncestralStateTrace::clone(void) const {
    
	return new AncestralStateTrace(*this);
}



void AncestralStateTrace::constructInternalObject( void ) {
    throw RbException("We do not support a constructor function for AncestralStateTrace.");
}


/* Map calls to member methods */

RevPtr<RevVariable> AncestralStateTrace::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found) {
    
    
    return RevObject::executeMethod( name, args, found );
}


/** Get Rev type of object */

const std::string& AncestralStateTrace::getClassType(void) { 
    
    static std::string revType = "AncestralStateTrace";
    
	return revType; 
}

/** Get class type spec describing type of object */

const TypeSpec& AncestralStateTrace::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( WorkspaceToCoreWrapperObject<RevBayesCore::AncestralStateTrace>::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}



/** Return member rules (no members) */

const MemberRules& AncestralStateTrace::getParameterRules(void) const {
    
    static MemberRules modelMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {        
        rulesSet = true;
    }
    
    return modelMemberRules;
}


/** Get type spec */

const TypeSpec& AncestralStateTrace::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */

void AncestralStateTrace::printValue(std::ostream &o) const {
    
    o << "AncestralStateTrace";
}


/** Set a member variable */

void AncestralStateTrace::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "xxx") {
        
    } 
    else {
        RevObject::setConstParameter(name, var);
    }
}

std::ostream& RevLanguage::operator<<(std::ostream& o, const RevLanguage::AncestralStateTrace& x) {
	//    o << x.getParameterName();
    o << "AncestralStateTrace (";
	//    const std::vector<double>& values = x.getValues();
	//    for (std::vector<double>::const_iterator it = values.begin(); it != values.end(); ++it) {
	//        if ( it != values.begin() ) {
	//            o << ", ";
	//        }
	//        o << *it;
	//    }
    o << ")";
    
    return o;
}