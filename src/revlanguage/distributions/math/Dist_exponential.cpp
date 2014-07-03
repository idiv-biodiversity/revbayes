#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ContinuousStochasticNode.h"
#include "Dist_exponential.h"
#include "ExponentialDistribution.h"
#include "Real.h"
#include "RealPos.h"

using namespace RevLanguage;


/**
 * Default constructor.
 * 
 * The default constructor does nothing except allocating the object.
 */
Dist_exponential::Dist_exponential() : PositiveContinuousDistribution() 
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of the model. 
 */
Dist_exponential* Dist_exponential::clone( void ) const 
{

    return new Dist_exponential(*this);
}


/**
 * Create a new internal distribution object.
 *
 * This function simply dynamically allocates a new internal distribution object that can be 
 * associated with the variable. The internal distribution object is created by calling its
 * constructor and passing the distribution-parameters (other DAG nodes) as arguments of the 
 * constructor. The distribution constructor takes care of the proper hook-ups.
 *
 * \return A new internal distribution object.
 */
RevBayesCore::ExponentialDistribution* Dist_exponential::createDistribution( void ) const
{
    // get the parameters
    RevBayesCore::TypedDagNode<double>* l       = static_cast<const RealPos &>( lambda->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* o       = static_cast<const RealPos &>( offset->getRevObject() ).getDagNode();
    RevBayesCore::ExponentialDistribution* d    = new RevBayesCore::ExponentialDistribution( l, o );
    
    return d;
}



/**
 * Get class name of object 
 *
 * \return The class' name.
 */
const std::string& Dist_exponential::getClassName(void) 
{ 
    
    static std::string rbClassName = "Dist_exponential";
    
	return rbClassName; 
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Dist_exponential::getClassTypeSpec(void) 
{ 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( PositiveContinuousDistribution::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** 
 * Get the member rules used to create the constructor of this object.
 *
 * The member rules of the exponential distribution are:
 * (1) the rate lambda which must be a positive real.
 *
 * \return The member rules.
 */
const MemberRules& Dist_exponential::getMemberRules(void) const 
{
    
    static MemberRules distExpMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distExpMemberRules.push_back( new ArgumentRule( "lambda", true, RealPos::getClassTypeSpec()   , new Real(1.0) ) );
        distExpMemberRules.push_back( new ArgumentRule( "offset", true, RealPos::getClassTypeSpec()   , new Real(0.0) ) );
        
        rulesSet = true;
    }
    
    return distExpMemberRules;
}


/**
 * Get type-specification on this object (non-static).
 *
 * \return The type spec of this object.
 */
const TypeSpec& Dist_exponential::getTypeSpec( void ) const 
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}



/** 
 * Set a member variable.
 * 
 * Sets a member variable with the given name and store the pointer to the variable.
 * The value of the variable might still change but this function needs to be called again if the pointer to
 * the variable changes. The current values will be used to create the distribution object.
 *
 * \param[in]    name     Name of the member variable.
 * \param[in]    var      Pointer to the variable.
 */
void Dist_exponential::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) 
{
    
    if ( name == "lambda" ) 
    {
        lambda = var;
    }
    else if ( name == "offset" ) 
    {
        offset = var;
    }
    else 
    {
        PositiveContinuousDistribution::setConstMemberVariable(name, var);
    }
    
}
