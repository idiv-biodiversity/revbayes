#include "RbException.h"
#include "RbUtil.h"
#include "SyntaxClassDef.h"
#include "Workspace.h"

#include <sstream>

using namespace RevLanguage;


/** Construct definition from class name, base class name, and variable and function definitions */
SyntaxClassDef::SyntaxClassDef( const std::string &name, const std::string &base, std::list<SyntaxElement*>* defs ) :
    SyntaxElement(),
    className( name ),
    baseClass( base ),
    definitions( defs )
{
}


/** Deep copy constructor */
SyntaxClassDef::SyntaxClassDef(const SyntaxClassDef& x) :
    SyntaxElement( x )
{
    className = className;
    baseClass = baseClass;

    definitions = new std::list<SyntaxElement*>();
    for ( std::list<SyntaxElement*>::const_iterator it = x.definitions->begin(); it != x.definitions->end(); ++it )
        definitions->push_back( (*it)->clone() );
}


/** Destructor deletes members */
SyntaxClassDef::~SyntaxClassDef()
{
    for ( std::list<SyntaxElement*>::const_iterator it = definitions->begin(); it != definitions->end(); ++it )
        delete (*it);
    delete definitions;
}


/** Assignment operator */
SyntaxClassDef& SyntaxClassDef::operator=( const SyntaxClassDef& x )
{
    if ( &x != this )
    {
        SyntaxElement::operator=( x );

        className   = x.className;
        baseClass   = x.baseClass;

        for ( std::list<SyntaxElement*>::const_iterator it = definitions->begin(); it != definitions->end(); ++it )
            delete (*it);

        definitions->clear();
        for ( std::list<SyntaxElement*>::const_iterator it = x.definitions->begin(); it != x.definitions->end(); ++it )
            definitions->push_back( (*it)->clone() );
    }

    return *this;
}


/** Clone syntax element */
SyntaxElement* SyntaxClassDef::clone() const
{
    return new SyntaxClassDef( *this );
}


/** Get semantic value: insert a user-defined class in the user workspace */
RevPtr<RevVariable> SyntaxClassDef::evaluateContent( Environment& env, bool dynamic )
{
    throw RbException( "Sorry, user-defined classes not implemented yet" );

    // No return value 
    return NULL;
}


/** Print info about the syntax element */
void SyntaxClassDef::printValue( std::ostream& o ) const
{
    o << "SyntaxClassDef:" << std::endl;

    o << "className   = " << className;
    o << std::endl;

    o << "baseClass   = " << baseClass;
    o << std::endl;

    o << "defintions  = <" << definitions->size() << " definitions of member variables and functions>" << std::endl;
}

