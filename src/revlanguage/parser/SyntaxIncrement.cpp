#include "RbException.h"
#include "RbUtil.h"
#include "RbOptions.h"
#include "SyntaxIncrement.h"
#include "Workspace.h"

#include <iostream>
#include <list>
#include <sstream>

using namespace RevLanguage;


/**
 * Construct from variable expression.
 *
 * \param[in]   v   The variable expression
 */
SyntaxIncrement::SyntaxIncrement( SyntaxElement* v, bool post ) : SyntaxElement(),
    postIncrement( post ),
    variable( v )
{
}


/**
 * Deep copy constructor. It really should not be possible for the
 * parser to return a NULL variable statement, so we do not have
 * to check for a NULL pointer.
 */
SyntaxIncrement::SyntaxIncrement( const SyntaxIncrement& x ) : SyntaxElement(x),
    postIncrement( x.postIncrement )
{
    variable   = x.variable->clone();
}


/**
 * Destructor deletes operands
 */
SyntaxIncrement::~SyntaxIncrement()
{
    delete variable;
}


/**
 * Assignment operator performing deep assignment.
 */
SyntaxIncrement& SyntaxIncrement::operator=( const SyntaxIncrement& x )
{
    if ( this != &x )
    {
        SyntaxElement::operator=( x );
        
        postIncrement = x.postIncrement;
        
        delete variable;
        variable = x.variable->clone();
    }
    
    return ( *this );
}


/**
 * The clone function is a convenience function to create proper copies of inherited objects.
 * That is, a.clone() will create a clone of type 'b' if the 'a' instance is of derived type 'b'.
 *
 * \return A new copy of myself
 */
SyntaxIncrement* SyntaxIncrement::clone () const
{
    return new SyntaxIncrement( *this );
}


/**
 * Evaluate the content of this syntax element. This will perform
 * an increment assignment operation.
 */
RevPtr<RevVariable> SyntaxIncrement::evaluateContent( Environment& env, bool dynamic )
{
#ifdef DEBUG_PARSER
    printf( "Evaluating increment assignment\n" );
#endif
    
    RevPtr<RevVariable> retVar;
    
    // Get variable. We use standard evaluation because the variable is
    // implicitly on both sides (lhs and rhs) of this type of statement
    RevPtr<RevVariable> theVariable = variable->evaluateContent( env );
    if ( theVariable == NULL )
        throw RbException( "Invalid NULL variable returned by variable expression in increment assignment" );
    
    // Make sure that the variable is constant
    if ( !theVariable->getRevObject().isConstant() )
        throw RbException( "Invalid increment assignment to dynamic variable" );
    
    // Get a non-const reference to the lhs value object
    RevObject& lhs_value = theVariable->getRevObject();
    
    if ( postIncrement )
    {
        retVar = new RevVariable( lhs_value.clone() );
    }
    
    // Increment the lhs value. This will not change the control variable status.
    lhs_value.increment();
    
    if ( !postIncrement )
    {
        retVar = new RevVariable( lhs_value.clone() );
    }
    
#ifdef DEBUG_PARSER
    env.printValue(std::cerr);
#endif
    
    // No further assignment with this type of statement
    return retVar;
}


/**
 * Is the syntax element safe for use in a function (as
 * opposed to a procedure)? The assignment is safe
 * if the variable expression is safe, and the
 * increment is not to an external variable.
 */
bool SyntaxIncrement::isFunctionSafe( const Environment& env, std::set<std::string>& localVars ) const
{
    // Check variable
    if ( !variable->isFunctionSafe( env, localVars ) )
        return false;
    
    // Check whether assignment is to external variable (not function safe)
    // We are not allowed to add the variable to the local variables, it must
    // exist before the increment is attempted
    if ( variable->retrievesExternVar( env, localVars, false ) )
        return false;
    
    // All tests passed
    return true;
}


/**
 * Print info about the syntax element
 */
void SyntaxIncrement::printValue(std::ostream& o) const
{
    o << "SyntaxIncrement:" << std::endl;
    o << "variable      = ";
    variable->printValue(o);
    o << std::endl;
    o << "expression    = '++'" << std::endl;
}


