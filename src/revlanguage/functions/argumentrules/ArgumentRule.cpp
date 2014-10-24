#include "Argument.h"
#include "ArgumentRule.h"
#include "RbException.h"
#include "RbUtil.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;


/**
 * Construct rule without default value; use "" for no label.
 */
ArgumentRule::ArgumentRule(const std::string& argName, const TypeSpec& argTypeSp, EvaluationType et, DagNodeType dt) :
    argTypeSpecs( 1, argTypeSp ),
    defaultVar( NULL ),
    evalType( et ),
    nodeType( dt ),
    label(argName),
    hasDefaultVal( false )
{
    
}



/**
 * Construct rule without default value; use "" for no label.
 */
ArgumentRule::ArgumentRule(const std::string& argName, const TypeSpec& argTypeSp, EvaluationType et, DagNodeType dt, RevObject *defVal) :
    argTypeSpecs( 1, argTypeSp ),
    defaultVar( new Variable( defVal ) ),
    evalType( et ),
    nodeType( dt ),
    label(argName),
    hasDefaultVal( true )
{
    
}


/**
 * Construct rule without default value; use "" for no label.
 */
ArgumentRule::ArgumentRule(const std::string& argName, const std::vector<TypeSpec>& argTypeSp, EvaluationType et, DagNodeType dt) :
    argTypeSpecs( argTypeSp ),
    defaultVar( NULL ),
    evalType( et ),
    nodeType( dt ),
    label(argName),
    hasDefaultVal( false )
{
    
}


/**
 * Construct rule without default value; use "" for no label.
 */
ArgumentRule::ArgumentRule(const std::string& argName, const std::vector<TypeSpec>& argTypeSp, EvaluationType et, DagNodeType dt, RevObject *defVal) :
    argTypeSpecs( argTypeSp ),
    defaultVar( new Variable( defVal ) ),
    evalType( et ),
    nodeType( dt ),
    label(argName),
    hasDefaultVal( true )
{
    
}



ArgumentRule* RevLanguage::ArgumentRule::clone( void ) const
{

    return new ArgumentRule( *this );
}


/**
 * Fit a variable into an argument according to the argument rule. If necessary and
 * appropriate, we do type conversion or type promotion.
 *
 *
 * @todo We need to check whether workspace objects with member variables are
 *       modifiable by the user.
 *
 * @todo To conform to the old code we change the required type of the incoming
 *       variable wrapper here. We need to change this so that we do not change
 *       the wrapper here, but make sure that if the argument variable is inserted
 *       in a member variable or container element slot, that the slot variable
 *       wrapper, which should be unique (not the same as the incoming variable
 *       wrapper), has the right required type.
 */
Argument ArgumentRule::fitArgument( Argument& arg, bool once ) const
{

    RevPtr<Variable> theVar = arg.getVariable();
    if ( evalType == BY_VALUE || theVar->isWorkspaceVariable() )
    {
        once = true;
    }
    
    
    for ( std::vector<TypeSpec>::const_iterator it = argTypeSpecs.begin(); it != argTypeSpecs.end(); ++it )
    {
        if ( theVar->getRevObject().isType( *it ) )
        {
            // For now, change the required type of the incoming variable wrapper
            theVar->setRevObjectTypeSpec( *it );
            
            if ( !isEllipsis() )
                return Argument( theVar, getArgumentLabel(), evalType == BY_CONSTANT_REFERENCE );
            else
                return Argument( theVar, arg.getLabel(), true );
        }
        else if ( once == false &&
                 !theVar->isAssignable() &&
                 theVar->getRevObject().isConvertibleTo( *it, true ) &&
                 (*it).isDerivedOf( theVar->getRevObjectTypeSpec() )
                 )
        {
            // Fit by type promotion. For now, we also modify the type of the incoming variable wrapper.
            RevObject* convertedObject = theVar->getRevObject().convertTo( *it );
            theVar->setRevObject( convertedObject );
            theVar->setRevObjectTypeSpec( *it );
            if ( !isEllipsis() )
            {
                return Argument( theVar, getArgumentLabel(), false );
            }
            else
            {
                return Argument( theVar, arg.getLabel(), false );
            }
            
        }
        else if ( theVar->getRevObject().isConvertibleTo( *it, once ) )
        {
            // Fit by type conversion
            if ( once )
            {
                RevObject* convertedObject = theVar->getRevObject().convertTo( *it );
                Variable*  convertedVar    = new Variable( convertedObject );
                convertedVar->setRevObjectTypeSpec( *it );

                if ( !isEllipsis() )
                {
                    return Argument( convertedVar, getArgumentLabel(), evalType == BY_CONSTANT_REFERENCE );
                }
                else
                {
                    return Argument( convertedVar, arg.getLabel(), true );
                }
                
            }
            else
            {
                RevObject* conversionObject = theVar->getRevObject().convertTo( *it );
                conversionObject->makeConversionValue( theVar );
                Variable*  conversionVar    = new Variable( conversionObject );
                conversionVar->setHiddenVariableState( true );
                conversionVar->setRevObjectTypeSpec( *it );
                
                if ( !isEllipsis() )
                {
                    return Argument( conversionVar, getArgumentLabel(), evalType == BY_CONSTANT_REFERENCE );
                }
                else
                {
                    return Argument( conversionVar, arg.getLabel(), true );
                }
                
            }
        }
    }
    
    throw RbException( "Argument type mismatch fitting a " + theVar->getRevObject().getType() + " argument to formal " +
                        getArgumentTypeSpec()[0].getType() + " " + getArgumentLabel() );
}


ArgumentRule::DagNodeType ArgumentRule::getArgumentDagNodeType( void ) const
{
    // return the internal value
    return nodeType;
}


const std::string& ArgumentRule::getArgumentLabel(void) const
{
    return label;
}


const std::vector<TypeSpec>& ArgumentRule::getArgumentTypeSpec(void) const
{
    return argTypeSpecs;
}



const Variable& ArgumentRule::getDefaultVariable( void ) const {
    
    if ( defaultVar == NULL ) 
    {
        throw RbException("Cannot get default variable \"" + label + "\"");
    }
    
    return *defaultVar;
}


ArgumentRule::EvaluationType ArgumentRule::getEvaluationType( void ) const
{
    
    return evalType;
}


bool ArgumentRule::hasDefault(void) const {
    
    return hasDefaultVal;
}


/**
 * Test if argument is valid. The boolean flag 'once' is used to signal whether the argument matching
 * is done in a static or a dynamic context. If the rule is constant, then the argument matching
 * is done in a static context (evaluate-x§once context) regardless of the setting of the once flag.
 * If the argument is constant, we try type promotion if permitted by the variable required type.
 *
 * @todo See the TODOs for fitArgument(...)
 */
bool ArgumentRule::isArgumentValid(const RevPtr<const Variable> &var, bool once) const
{
    
    if ( var == NULL )
    {
        return false;
    }
    
    if ( evalType == BY_VALUE || var->isWorkspaceVariable() )
    {
        once = true;
    }

    for ( std::vector<TypeSpec>::const_iterator it = argTypeSpecs.begin(); it != argTypeSpecs.end(); ++it )
    {
        if ( var->getRevObject().isType( *it ) )
        {
            return true;
        }
        else if ( var->getRevObject().isConvertibleTo( *it, once ) )
        {
            return true;
        }
        else if ( once == false && !var->isAssignable() &&
                  var->getRevObject().isConvertibleTo( *it, true ) &&
                  (*it).isDerivedOf( var->getRevObjectTypeSpec() )
                )
        {
            return true;
        }
        
    }
    
    return false;
}



bool RevLanguage::ArgumentRule::isEllipsis( void ) const
{
    
    return false;
}
 


/**
 * Print value for user (in descriptions of functions, for instance). We apparently do
 * not use the isConst flag to denote whether an argument is supposed to be passed as
 * a constant currently, so the printing of this modifier is suspended for now.
 *
 * @todo Revise the usage and printing of the isConst flag
 */
void RevLanguage::ArgumentRule::printValue(std::ostream &o) const {

//    if ( isConstant() )
//        o << "const ";

    for ( std::vector<TypeSpec>::const_iterator it = argTypeSpecs.begin(); it != argTypeSpecs.end(); ++it )
    {
        if ( it != argTypeSpecs.begin() )
            o << "|";
        o << (*it).getType();
    }
    o << " " << label;
}

