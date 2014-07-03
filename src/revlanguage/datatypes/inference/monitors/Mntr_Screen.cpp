
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstantNode.h"
#include "Mntr_Screen.h"
#include "RbException.h"
#include "RevObject.h"
#include "RlString.h"
#include "ScreenMonitor.h"
#include "TypeSpec.h"
#include "Vector.h"


using namespace RevLanguage;

Mntr_Screen::Mntr_Screen(void) : Monitor() {
    
}


/** Clone object */
Mntr_Screen* Mntr_Screen::clone(void) const {
    
	return new Mntr_Screen(*this);
}


void Mntr_Screen::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate space for a new Mntr_Screen object
    const std::string& sep = static_cast<const RlString &>( separator->getRevObject() ).getValue();
    int g = static_cast<const Natural &>( printgen->getRevObject() ).getValue();
    std::set<RevBayesCore::DagNode *> n;
    for (std::set<RevPtr<const Variable> >::iterator i = vars.begin(); i != vars.end(); ++i) {
        RevBayesCore::DagNode* node = (*i)->getRevObject().getDagNode();
        n.insert( node );
    }
    bool pp = static_cast<const RlBoolean &>( posterior->getRevObject() ).getValue();
    bool l = static_cast<const RlBoolean &>( likelihood->getRevObject() ).getValue();
    bool pr = static_cast<const RlBoolean &>( prior->getRevObject() ).getValue();
    value = new RevBayesCore::ScreenMonitor(n, g, sep, pp, l, pr);
}


/** Get class name of object */
const std::string& Mntr_Screen::getClassName(void) { 
    
    static std::string rbClassName = "Mntr_Screen";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Mntr_Screen::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Monitor::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** Return member rules (no members) */
const MemberRules& Mntr_Screen::getMemberRules(void) const {
    
    static MemberRules filemonitorMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        filemonitorMemberRules.push_back( new Ellipsis( RevObject::getClassTypeSpec() ) );
        filemonitorMemberRules.push_back( new ArgumentRule("printgen", true, Natural::getClassTypeSpec(), new Natural(1) ) );
        filemonitorMemberRules.push_back( new ArgumentRule("separator", true, RlString::getClassTypeSpec(), new RlString(" ") ) );
        filemonitorMemberRules.push_back( new ArgumentRule("posterior", true, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        filemonitorMemberRules.push_back( new ArgumentRule("likelihood", true, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        filemonitorMemberRules.push_back( new ArgumentRule("prior", true, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        
        
        rulesSet = true;
    }
    
    return filemonitorMemberRules;
}

/** Get type spec */
const TypeSpec& Mntr_Screen::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Mntr_Screen::printValue(std::ostream &o) const {
    
    o << "Mntr_Screen";
}


/** Set a member variable */
void Mntr_Screen::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "" ) {
        vars.insert( var );
    }
    else if ( name == "separator" ) {
        separator = var;
    }
    else if ( name == "printgen" ) {
        printgen = var;
    }
    else if ( name == "prior" ) {
        prior = var;
    }
    else if ( name == "posterior" ) {
        posterior = var;
    }
    else if ( name == "likelihood" ) {
        likelihood = var;
    }
    else {
        RevObject::setConstMemberVariable(name, var);
    }
}
