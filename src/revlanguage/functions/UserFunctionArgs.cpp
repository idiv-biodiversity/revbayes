#include "UserFunctionArgs.h"

#include <sstream>

using namespace RevLanguage;

/** Constructor */
UserFunctionArgs::UserFunctionArgs( const UserFunction*  f   )   :
    Function(*f), userFunction(f) {
    
    
}


/** Copy constructor */
UserFunctionArgs::UserFunctionArgs(const UserFunctionArgs &f) :
    Function(f), userFunction(f.userFunction) {
    
}


UserFunctionArgs& UserFunctionArgs::operator=(const UserFunctionArgs &f) {
    
    if ( this != &f ) {
        
        // Call the base class assignment operator
        Function::operator=(f);
        
        // Use the same user function
        userFunction = f.userFunction;
    }
    
    return *this;
}


/** Destructor */
UserFunctionArgs::~UserFunctionArgs() {
    
    // Nothing to do here

}


/** Clone the object */
UserFunctionArgs* UserFunctionArgs::clone(void) const {
    
    return new UserFunctionArgs(*this);
}


/** Execute function: this function should not be called */
RevObject* UserFunctionArgs::execute( void ) {
    
    throw ("Unexpected call to UserFunctionArgs");
}


/** Get class name of object */
const std::string& UserFunctionArgs::getClassName(void) {
    
    static std::string rbClassName = "UserFunctionArgs";
    
	return rbClassName;
}


/** Get class type spec describing type of object */
const TypeSpec& UserFunctionArgs::getClassTypeSpec(void) {
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass;
}


/** Get type spec */
const TypeSpec& UserFunctionArgs::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get argument rules */
const ArgumentRules& UserFunctionArgs::getArgumentRules(void) const {
    
    return userFunction->getArgumentRules();
}


/** Get return type */
const TypeSpec& UserFunctionArgs::getReturnType(void) const {
    
    return userFunction->getReturnType();
}

