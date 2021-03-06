#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "RbException.h"
#include "Signals.h"
#include "SyntaxElement.h"
#include "TypeSpec.h"
#include "UserFunctionDef.h"
#include "UserProcedure.h"

#include <sstream>

using namespace RevLanguage;

/** Basic constructor */
UserProcedure::UserProcedure( UserFunctionDef* def ) :
    Function(),
    functionDef( def )
{
}


/** Clone the object */
UserProcedure* UserProcedure::clone(void) const
{
    return new UserProcedure(*this);
}


/**
 * Execute the procedure. We simply execute the code; a procedure is never part of
 * a deterministic variable, so we need not worry about dynamic, multiple execution
 * of the procedure from a deterministic variable.
 *
 * Arguments are added to our environment as aliases, so we can both modify them and
 * assign to them.
 */
RevPtr<RevVariable> UserProcedure::execute( void )
{
    // Clear signals
    Signals::getSignals().clearFlags();
    
    // Set initial return value
    RevPtr<RevVariable> retVar = NULL;
    
    // Create new evaluation frame with function base class execution environment as parent
    Environment* procedureFrame = new Environment( getEnvironment(), "UserProcedureEnvironment" );
    
    // Add the arguments to our environment as alias variables (modifiable and assignable)
    for (std::vector<Argument>::iterator it = args.begin(); it != args.end(); ++it)
    {
        procedureFrame->addAlias( it->getLabel(), it->getVariable() );
    }
    
    // Execute code
    const std::list<SyntaxElement*>& code = functionDef->getCode();
    for ( std::list<SyntaxElement*>::const_iterator i=code.begin(); i!=code.end(); i++ ) {
        
        SyntaxElement* theSyntaxElement = *i;
        retVar = theSyntaxElement->evaluateContent( *procedureFrame );
        
        if ( Signals::getSignals().isSet( Signals::RETURN ) )
        {
            Signals::getSignals().clearFlags();
            break;
        }
    }
    
    // Delete evaluation frame
    delete procedureFrame;
    
    // Return the return value
    return retVar;
}


/** Get Rev type (static) */
const std::string& UserProcedure::getClassType(void)
{
    static std::string revType = "UserProcedure";
    
	return revType;
}


/** Get Rev type spec (static) */
const TypeSpec& UserProcedure::getClassTypeSpec(void)
{
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), &Function::getClassTypeSpec() );
    
	return revTypeSpec;
}


/** Get Rev type spec (from an instance) */
const TypeSpec& UserProcedure::getTypeSpec( void ) const
{
    return getClassTypeSpec();
}


/** Get argument rules */
const ArgumentRules& UserProcedure::getArgumentRules(void) const
{
    return functionDef->getArgumentRules();
}


/** Get return type */
const TypeSpec& UserProcedure::getReturnType(void) const
{
    return functionDef->getReturnType();
}

