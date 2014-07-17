#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "BiogeographicTreeHistoryCtmc.h"
#include "ConstantNode.h"
//#include "TreeCharacterHistoryNhxMonitor.h"
#include "TreeCharacterHistoryNodeMonitor.h"
#include "Mntr_CharacterHistoryNewickFile.h"
#include "OptionRule.h"
#include "RbException.h"
#include "RevObject.h"
#include "RlAbstractCharacterData.h"
#include "RlString.h"
#include "RlTimeTree.h"
#include "StandardState.h"
#include "TimeTree.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "Vector.h"


using namespace RevLanguage;

Mntr_CharacterHistoryNewickFile::Mntr_CharacterHistoryNewickFile(void) : Monitor() {
    
}


/** Clone object */
Mntr_CharacterHistoryNewickFile* Mntr_CharacterHistoryNewickFile::clone(void) const {
    
	return new Mntr_CharacterHistoryNewickFile(*this);
}


void Mntr_CharacterHistoryNewickFile::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    const std::string& fn = static_cast<const RlString &>( filename->getRevObject() ).getValue();
    const std::string& sep = static_cast<const RlString &>( separator->getRevObject() ).getValue();
    int g = static_cast<const Natural &>( printgen->getRevObject() ).getValue();
   
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree> *t = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    std::set<RevBayesCore::TypedDagNode<std::vector<double> > *> n;
    for (std::set<RevPtr<const Variable> >::iterator i = vars.begin(); i != vars.end(); ++i) {
        RevBayesCore::TypedDagNode<std::vector<double> >* node = static_cast< const Vector<Real> & >((*i)->getRevObject()).getDagNode();
        n.insert( node );
    }
    
    RevBayesCore::TypedDagNode<RevBayesCore::AbstractCharacterData>* ctmc_tdn   = static_cast<const RevLanguage::AbstractCharacterData&>( ctmc->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode<RevBayesCore::AbstractCharacterData>* ctmc_sn  = static_cast<RevBayesCore::StochasticNode<RevBayesCore::AbstractCharacterData>* >(ctmc_tdn);
    
    bool pp = static_cast<const RlBoolean &>( posterior->getRevObject() ).getValue();
    bool l = static_cast<const RlBoolean &>( likelihood->getRevObject() ).getValue();
    bool pr = static_cast<const RlBoolean &>( prior->getRevObject() ).getValue();
//    bool sc = static_cast<const RlBoolean &>( counts->getRevObject() ).getValue();
//    bool se = static_cast<const RlBoolean &>( events->getRevObject() ).getValue();
    bool sm = false; // show metadata disabled for now
    bool ap = static_cast<const RlBoolean &>( append->getRevObject() ).getValue();

    
    std::string ms = static_cast<const RlString&>( style->getRevObject() ).getValue();
    bool sc = false;
    if (ms == "counts")
        sc = true;
    bool se = !sc;
    
    std::string mt = static_cast<const RlString&>( type->getRevObject() ).getValue();
    if (mt == "std")
        ; // value = XXXXXX
    else if (mt == "biogeo")
        value = new RevBayesCore::TreeCharacterHistoryNodeMonitor<RevBayesCore::StandardState,RevBayesCore::TimeTree>(ctmc_sn, t, size_t(g), fn, sep, pp, l, pr, ap, sm, sc, se);
}


/** Get class name of object */
const std::string& Mntr_CharacterHistoryNewickFile::getClassName(void) {
    
    static std::string rbClassName = "Mntr_CharacterHistoryNewickFile";
    
	return rbClassName;
}

/** Get class type spec describing type of object */
const TypeSpec& Mntr_CharacterHistoryNewickFile::getClassTypeSpec(void) {
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Monitor::getClassTypeSpec() ) );
    
	return rbClass;
}



/** Return member rules (no members) */
const MemberRules& Mntr_CharacterHistoryNewickFile::getMemberRules(void) const {
    
    static MemberRules Mntr_CharacterHistoryNewickFileMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        Mntr_CharacterHistoryNewickFileMemberRules.push_back( new ArgumentRule("filename", true, RlString::getClassTypeSpec() ) );
        Mntr_CharacterHistoryNewickFileMemberRules.push_back( new ArgumentRule("ctmc", true, AbstractCharacterData::getClassTypeSpec() ) );
        Mntr_CharacterHistoryNewickFileMemberRules.push_back( new ArgumentRule("tree", true, TimeTree::getClassTypeSpec() ) );
        Mntr_CharacterHistoryNewickFileMemberRules.push_back( new ArgumentRule("printgen", true, Natural::getClassTypeSpec(), new Natural(1) ) );
        Mntr_CharacterHistoryNewickFileMemberRules.push_back( new ArgumentRule("separator", true, RlString::getClassTypeSpec(), new RlString(" ") ) );
        Mntr_CharacterHistoryNewickFileMemberRules.push_back( new ArgumentRule("posterior", true, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        Mntr_CharacterHistoryNewickFileMemberRules.push_back( new ArgumentRule("likelihood", true, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        Mntr_CharacterHistoryNewickFileMemberRules.push_back( new ArgumentRule("prior", true, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
//        Mntr_CharacterHistoryNewickFileMemberRules.push_back( new ArgumentRule("counts", true, RlBoolean::getClassTypeSpec(), new RlBoolean(false) ) );
//        Mntr_CharacterHistoryNewickFileMemberRules.push_back( new ArgumentRule("events", true, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        Mntr_CharacterHistoryNewickFileMemberRules.push_back( new ArgumentRule("append", true, RlBoolean::getClassTypeSpec(), new RlBoolean(true) ) );
        
        Vector<RlString> options_style;
        //        options.push_back( RlString("std") );
        options_style.push_back( RlString("events") );
        options_style.push_back( RlString("counts") );
        Mntr_CharacterHistoryNewickFileMemberRules.push_back( new OptionRule( "style", new RlString("events"), options_style ) );

        
        Vector<RlString> options;
//        options.push_back( RlString("std") );
        options.push_back( RlString("biogeo") );
        Mntr_CharacterHistoryNewickFileMemberRules.push_back( new OptionRule( "type", new RlString("biogeo"), options ) );

        
        
        rulesSet = true;
    }
    
    return Mntr_CharacterHistoryNewickFileMemberRules;
}

/** Get type spec */
const TypeSpec& Mntr_CharacterHistoryNewickFile::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Mntr_CharacterHistoryNewickFile::printValue(std::ostream &o) const {
    
    o << "Mntr_CharacterHistoryNewickFile";
}


/** Set a member variable */
void Mntr_CharacterHistoryNewickFile::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "" ) {
        vars.insert( var );
    }
    else if ( name == "filename" ) {
        filename = var;
    }
    else if ( name == "tree" ) {
        tree = var;
    }
    else if ( name == "ctmc" ) {
        ctmc = var;
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
    else if ( name == "type" ) {
        type = var;
    }
    else if ( name == "style" ) {
        style = var;
    }
    else if ( name == "append" ) {
        append = var;
    }
    else {
        RevObject::setConstMemberVariable(name, var);
    }
}
