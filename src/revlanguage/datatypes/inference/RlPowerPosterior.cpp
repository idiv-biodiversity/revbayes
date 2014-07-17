
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstantNode.h"
#include "DistributionBeta.h"
#include "PowerPosteriorMcmc.h"
#include "Model.h"
#include "RevObject.h"
#include "RbException.h"
#include "Real.h"
#include "RevNullObject.h"
#include "RlPowerPosterior.h"
#include "RlModel.h"
#include "RlMonitor.h"
#include "RlMove.h"
#include "TypeSpec.h"
#include "VectorRbPointer.h"
#include "Vector.h"


using namespace RevLanguage;

PowerPosterior::PowerPosterior() : WorkspaceObject<RevBayesCore::PowerPosteriorMcmc>() {
    
}


/** Clone object */
PowerPosterior* PowerPosterior::clone(void) const {
    
	return new PowerPosterior(*this);
}


void PowerPosterior::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    const RevBayesCore::Model&                  mdl     = static_cast<const Model &>( model->getRevObject() ).getValue();
    const VectorRbPointer<Move>&                rlmvs   = static_cast<const VectorRbPointer<Move> &>( moves->getRevObject() );
    RevBayesCore::RbVector<RevBayesCore::Move>  mvs     = rlmvs.getValue();
    const std::string&                          fn      = static_cast<const RlString &>( filename->getRevObject() ).getValue();

    value = new RevBayesCore::PowerPosteriorMcmc(mdl, mvs, fn);
    
    std::vector<double> beta;
    if ( powers->getRevObject() != RevNullObject::getInstance() )
    {
        beta = static_cast<const Vector<RealPos> &>( powers->getRevObject() ).getValue();
    }
    else if( cats->getRevObject() != RevNullObject::getInstance() )
    {
        int k = static_cast<const Natural &>( cats->getRevObject() ).getValue();
        for (int i = k; i >= 0; --i)
        {
            double b = RevBayesCore::RbStatistics::Beta::quantile(0.3,1.0,i / double(k));
            beta.push_back( b );
        }
    }
    else
    {
        int k     = 100;
        for (int i = k; i >= 0; --i)
        {
            double b = RevBayesCore::RbStatistics::Beta::quantile(0.3,1.0,i / double(k));
            beta.push_back( b );
        }
    }
    
    value->setPowers( beta );
    value->setSampleFreq( 100 );
}


/* Map calls to member methods */
RevObject* PowerPosterior::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if (name == "run") {
        // get the member with give index
        int gen = static_cast<const Natural &>( args[0].getVariable()->getRevObject() ).getValue();
        value->run( size_t(gen) );
        
        return NULL;
    } 
    else if (name == "burnin") {
        // get the member with give index
        int gen = static_cast<const Natural &>( args[0].getVariable()->getRevObject() ).getValue();
        int tuningInterval = static_cast<const Natural &>( args[1].getVariable()->getRevObject() ).getValue();
        value->burnin( size_t(gen), size_t(tuningInterval) );
        
        return NULL;
    } 
    
    return RevObject::executeMethod( name, args );
}


/** Get class name of object */
const std::string& PowerPosterior::getClassName(void) { 
    
    static std::string rbClassName = "PowerPosterior";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& PowerPosterior::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( WorkspaceObject<RevBayesCore::PowerPosteriorMcmc>::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** Return member rules (no members) */
const MemberRules& PowerPosterior::getMemberRules(void) const {
    
    static MemberRules modelMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        modelMemberRules.push_back( new ArgumentRule("model", true, Model::getClassTypeSpec() ) );
        modelMemberRules.push_back( new ArgumentRule("moves", true, VectorRbPointer<Move>::getClassTypeSpec() ) );
        modelMemberRules.push_back( new ArgumentRule("filename", true, RlString::getClassTypeSpec() ) );
        modelMemberRules.push_back( new ArgumentRule("powers", true, Vector<RealPos>::getClassTypeSpec(), NULL ) );
        modelMemberRules.push_back( new ArgumentRule("cats", true, Natural::getClassTypeSpec(), NULL ) );
        
        rulesSet = true;
    }
    
    return modelMemberRules;
}


/* Get method specifications */
const MethodTable& PowerPosterior::getMethods(void) const {
    
    static MethodTable methods = MethodTable();
    static bool          methodsSet = false;
    
    if ( methodsSet == false ) {
        ArgumentRules* runArgRules = new ArgumentRules();
        runArgRules->push_back( new ArgumentRule("generations", true, Natural::getClassTypeSpec()) );
        methods.addFunction("run", new MemberFunction( RlUtils::Void, runArgRules) );
        
        ArgumentRules* burninArgRules = new ArgumentRules();
        burninArgRules->push_back( new ArgumentRule("generations", true, Natural::getClassTypeSpec()) );
        burninArgRules->push_back( new ArgumentRule("tuningInterval", true, Natural::getClassTypeSpec()) );
        methods.addFunction("burnin", new MemberFunction( RlUtils::Void, burninArgRules) );
        
        
        // necessary call for proper inheritance
        methods.setParentTable( &RevObject::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}

/** Get type spec */
const TypeSpec& PowerPosterior::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void PowerPosterior::printValue(std::ostream &o) const {
    
    o << "PowerPosterior";
}


/** Set a member variable */
void PowerPosterior::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "model")
    {
        model = var;
    } 
    else if ( name == "moves")
    {
        moves = var;
    }
    else if ( name == "filename")
    {
        filename = var;
    }
    else if ( name == "cats")
    {
        cats = var;
    }
    else if ( name == "powers")
    {
        powers = var;
    }
    else
    {
        RevObject::setConstMemberVariable(name, var);
    }
}
