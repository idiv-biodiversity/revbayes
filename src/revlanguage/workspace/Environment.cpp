#include "Environment.h"
#include "RbException.h"
#include "RlFunction.h"
#include "RbUtil.h"
#include "RbOptions.h"
#include "Variable.h"

#include <cstdio>

using namespace RevLanguage;


/** Construct environment with NULL parent */
Environment::Environment(void) :
    functionTable(),
    numUnnamedVariables(0),
    parentEnvironment(NULL),
    variableTable()
{
    
}


/** Construct environment with parent */
Environment::Environment(Environment* parentEnv) :
    functionTable(&parentEnv->getFunctionTable()),
    numUnnamedVariables(0),
    parentEnvironment(parentEnv),
    variableTable()
{
    
}


/** Copy constructor. We need to make sure we have a deep copy of the variable table, because a shallow copy is the default. */
Environment::Environment(const Environment &x) :
    functionTable( x.functionTable ),
    numUnnamedVariables( x.numUnnamedVariables ),
    parentEnvironment( x.parentEnvironment ),
    variableTable( x.variableTable )
{
    // Make a deep copy of the variable table by making sure we have clones of the variables
    for ( VariableTable::iterator i = variableTable.begin(); i != variableTable.end(); i++ )
    {
        i->second = i->second->clone();
    }
    
}


/** Destructor */
Environment::~Environment()
{
    // Clear the variable table and function table
    clear();
    
}


/** Assignment operator. Ensure we have a deep copy of the variable table. */
Environment& Environment::operator=(const Environment &x)
{
    if (this != &x)
    {
        // Clean up variable and function table
        clear();

        // Copy parent environment pointer
        parentEnvironment = x.parentEnvironment;

        // Make a deep copy of function table by using assignment operator in FunctionTable
        functionTable = x.functionTable;
    }

    return *this;
}


/** Add alias to variable to frame. */
void Environment::addAlias( const std::string& name, const RevPtr<Variable>& theVar )
{
    
    /* Throw an error if the name string is empty. */
    if ( name == "" )
    {
        throw RbException("Invalid attempt to add unnamed reference variable to frame.");
    }
    
    /* Throw an error if the variable exists. Note that we cannot use the function
     existsVariable because that function looks recursively in parent frames, which
     would make it impossible to hide global variables. */
    if ( variableTable.find( name ) != variableTable.end() )
    {
        throw RbException( "Variable " + name + " already exists in frame" );
    }
    
    /* Insert new alias to variable in variable table (we do not and should not name it) */
    variableTable.insert( std::pair<std::string, RevPtr<Variable> >( name, theVar ) );
    
#ifdef DEBUG_WORKSPACE
    printf("Inserted \"%s\" (alias of \"%s\") in frame\n", name.c_str(), theVar->getName() );
#endif
    
}


/* Add function to frame. */
bool Environment::addFunction(const std::string& name, Function* func)
{

    if (existsVariable(name))
    {
        throw RbException("There is already a variable named '" + name + "' in the workspace");
    }
    
    functionTable.addFunction(name, func);

    return true;
}


/** Add an empty (NULL) variable to frame. */
void Environment::addNullVariable( const std::string& name )
{
    // Create a new variable object
    RevPtr<Variable> var = RevPtr<Variable>( new Variable( NULL ) );
    
    // Add the variable to the table
    addVariable( name, var );
}


/** Add reference to variable to frame. */
void Environment::addReference( const std::string& name, const RevPtr<Variable>& theVar )
{
    
    /* Throw an error if the name string is empty. */
    if ( name == "" )
    {
        throw RbException("Invalid attempt to add unnamed reference variable to frame.");
    }
    
    /* Throw an error if the variable exists. Note that we cannot use the function
     existsVariable because that function looks recursively in parent frames, which
     would make it impossible to hide global variables. */
    if ( variableTable.find( name ) != variableTable.end() )
    {
        throw RbException( "Variable " + name + " already exists in frame" );
    }
    
    /* Insert new reference variable in variable table */
    RevPtr<Variable> theRef = new Variable( theVar );
    variableTable.insert( std::pair<std::string, RevPtr<Variable> >( name, theRef ) );
    theRef->setName( name );
    
#ifdef DEBUG_WORKSPACE
    printf("Inserted \"%s\" in frame\n", name.c_str());
#endif
    
}


/** Add variable to frame. */
void Environment::addVariable( const std::string& name, const RevPtr<Variable>& theVar )
{
    
    /* Throw an error if the name string is empty. */
    if ( name == "" )
    {
        throw RbException("Invalid attempt to add unnamed variable to frame.");
    }
    
    /* Throw an error if the variable exists. Note that we cannot use the function
        existsVariable because that function looks recursively in parent frames, which
        would make it impossible to hide global variables. */
    if ( variableTable.find( name ) != variableTable.end() )
    {
        throw RbException( "Variable " + name + " already exists in frame" );
    }
    
    /* Insert new variable in variable table */
    variableTable.insert( std::pair<std::string, RevPtr<Variable> >( name, theVar ) );
    theVar->setName( name );

#ifdef DEBUG_WORKSPACE
    printf("Inserted \"%s\" in frame\n", name.c_str());
#endif

}


/** Add variable to frame from a "naked" Rev object. */
void Environment::addVariable(const std::string& name, RevObject* obj)
{
    // Create a new variable object
    RevPtr<Variable> var = new Variable( obj ) ;

    // Add the variable to the table
    addVariable( name, var );
}


/** Clone the environment (frame) */
Environment* Environment::clone() const
{
    return new Environment(*this);
}


/**
 * Clear the variable table. The function table has its own deep destructor called by assignment or destruction.
 * However, we empty it here in case the clear function is called from some place other than the assignment
 * operator or the destructor.
 */
void Environment::clear(void)
{
#if defined ( DEBUG_MEMORY )
    if ( variableTable.size() > 0 )
    {
        std::cerr << std::endl;
        std::cerr << "Variables to delete:" << std::endl;
        for ( VariableTable::iterator it = variableTable.begin(); it != variableTable.end(); ++it )
        {
            std::cerr << "variable: '" << (it)->second->getName() << "' <" << (it)->second << ">" << std::endl;
            std::cerr << "refCount: " << (it)->second->getReferenceCount() << std::endl;
        }
        std::cerr << std::endl;
    }

    if ( functionTable.size() > 0 )
    {
        std::cerr << std::endl;
        std::cerr << "Functions to delete:" << std::endl;
        for ( FunctionTable::iterator it = functionTable.begin(); it != functionTable.end(); ++it )
        {
            std::cerr << "function: '" << (it)->second->getName() << "' <" << (it)->second << ">" << std::endl;
        }
        std::cerr << std::endl;
    }
#endif

    // Empty the variable table. It is as easy as this because we use smart pointers...
    variableTable.clear();

    // Empty the function table.
    functionTable.clear();
}


/** Erase a variable by name given to it in the frame. */
void Environment::eraseVariable(const std::string& name)
{
    std::map<std::string, RevPtr<Variable> >::iterator it = variableTable.find(name);

    if ( it == variableTable.end() )
    {
        throw RbException(RbException::MISSING_VARIABLE, "Variable " + name + " does not exist in environment");
    }
    else
    {
        // Free the memory for the variable (smart pointer, so happens automatically) and
        // remove the variable from the map of variables
        variableTable.erase(it);
    }
}


/**
 * Erase a variable by its address, which is the value and not the key in the variable
 * map. Assuming that the variable maps are not huge and that we are not doing this kind
 * of operation in performance-critical code, we simply use a linear search of the map
 * to find the variable address here.
 */
void Environment::eraseVariable(const RevPtr<Variable>& var) {
    
    VariableTable::iterator it;
    for ( it=variableTable.begin(); it != variableTable.end(); ++it )
    {
        
        if ( it->second == var )
        {
            break;
        }
        
    }
    
    if ( it == variableTable.end() )
    {
        std::ostringstream msg;
        msg << "Variable with address '" << var << "' does not exist in frame";
        throw RbException( msg );
    }
    
    // Delegate call
    eraseVariable( it->first );
}


///**
// * Execute function to get its value. This will either return a constant value or a deterministic value
// * depending on the return type of the function.
// */
//RevPtr<Variable> Environment::executeFunction(const std::string& name, const std::vector<Argument>& args)
//{
//    
//    return functionTable.executeFunction(name, args);
//}


/** Find whether a function name exists (current and enclosing frames) */
bool Environment::existsFunction(std::string const &name) const
{
    // We delegate the query to the function table
    return functionTable.existsFunction(name);
}


/** Does variable exist in the environment (current frame and enclosing frames)? */
bool Environment::existsVariable(const std::string& name) const
{
    if (variableTable.find(name) == variableTable.end())
    {
        
        if (parentEnvironment != NULL)
        {
            return parentEnvironment->existsVariable(name);
        }
        else
        {
            return false;
        }
        
    }

    return true;
}


/** Does variable exist in the the current frame? */
bool Environment::existsVariableInFrame(const std::string& name) const
{
    if (variableTable.find(name) == variableTable.end())
        return false;
    else
        return true;
}


/** Generate a unique variable name */
std::string Environment::generateUniqueVariableName(void)
{
    std::ostringstream theName;

    theName << "var" << ++numUnnamedVariables;

    return theName.str();
}


/** Return variable table (const) */
const VariableTable& Environment::getVariableTable(void) const
{
    
    return variableTable;
}


/** Return variable table */
VariableTable& Environment::getVariableTable(void) {
    
    return variableTable;
}


/** Get function. This call will throw an error if the name is missing or present multiple times. */
const Function& Environment::getFunction(const std::string& name)
{
    
    return functionTable.getFunction(name);
}


/* Get function. This call will throw an error if the function is missing. */
const Function& Environment::getFunction(const std::string& name, const std::vector<Argument>& args, bool once) const
{
    
    return functionTable.getFunction(name, args, once);
}


/** Return the function table (const) */
const FunctionTable& Environment::getFunctionTable(void) const
{

    return functionTable;
}


/** Return the function table */
FunctionTable& Environment::getFunctionTable(void)
{

    return functionTable;
}


/** Return the object inside a specific variable */
RevObject& Environment::getRevObject(const std::string& name)
{
#ifdef DEBUG_WORKSPACE
    printf("Retrieving \"%s\" object from frame\n", name.c_str());
#endif
    
    return getVariable( name )->getRevObject();
}


/** Return a specific variable (const version) */
const RevObject& Environment::getRevObject(const std::string& name) const
{
#ifdef DEBUG_WORKSPACE
    printf("Retrieving \"%s\" object from frame\n", name.c_str());
#endif
    
    return getVariable( name )->getRevObject();
}


/** Return a specific variable */
RevPtr<Variable>& Environment::getVariable(const std::string& name)
{
    std::map<std::string, RevPtr<Variable> >::iterator it = variableTable.find( name );
    
    if ( variableTable.find(name) == variableTable.end() )
    {
        
        if ( parentEnvironment != NULL )
        {
            return parentEnvironment->getVariable( name );
        }
        else
        {
            throw RbException(RbException::MISSING_VARIABLE, "Variable " + name + " does not exist");
        }
        
    }
    
#ifdef DEBUG_WORKSPACE
    printf("Retrieving \"%s\" from frame\n", name.c_str());
#endif
    
    return it->second;
}


/** Return a specific variable (const version) */
const RevPtr<Variable>& Environment::getVariable(const std::string& name) const
{
    std::map<std::string, RevPtr<Variable> >::const_iterator it = variableTable.find(name);
    
    if ( variableTable.find(name) == variableTable.end() )
    {
        
        if ( parentEnvironment != NULL )
        {
            return const_cast<const Environment*>( parentEnvironment )->getVariable( name );
        }
        else
        {
            throw RbException(RbException::MISSING_VARIABLE, "Variable " + name + " does not exist");
        }
        
    }
    
    return it->second;
}


/**
 * Is the function with the name 'fxnName' a procedure? Simply ask the
 * function table.
 */
bool Environment::isProcedure(const std::string& fxnName) const
{
    return functionTable.isProcedure( fxnName );
}

/**
 * Is Environment same or parent of otherEnvironment? We use this function
 * to decide when a reference from otherEnvironment to a variable in this
 * Environment is safe, and when it is not. The only time we know for sure
 * that it is safe is when this Environment is identical to, or a parent of,
 * otherEnvironment.
 */
bool Environment::isSameOrParentOf(const Environment& otherEnvironment) const
{

    if ( this == &otherEnvironment )
    {
        return true;
    }
    
    if ( otherEnvironment.parentEnvironment == NULL )
    {
        return false;
    }
    
    return isSameOrParentOf( otherEnvironment.parentEnvironment );
}


/** Print workspace */
void Environment::printValue(std::ostream& o) const
{
    o << "Variable table:" << std::endl;

    VariableTable::const_iterator it;
    for ( it = variableTable.begin(); it != variableTable.end(); it++ )
    {
        o << (*it).first << " = ";
        (*it).second->printValue( o );
        o << std::endl;
    }

    o << std::endl;
}

