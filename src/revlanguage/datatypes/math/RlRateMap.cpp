//
//  RlRateMap.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 4/5/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#include "RlRateMap.h"

#include "ArgumentRule.h"
#include "MemberFunction.h"
#include "Natural.h"
#include "RlBoolean.h"
#include "RlTaxonData.h"
#include "Vector.h"

using namespace RevLanguage;

RateMap::RateMap(void) : ModelObject<RevBayesCore::RateMap>() {
    
}


RateMap::RateMap( RevBayesCore::RateMap *v) : ModelObject<RevBayesCore::RateMap>( v ) {
    
}


RateMap::RateMap( RevBayesCore::TypedDagNode<RevBayesCore::RateMap> *m) : ModelObject<RevBayesCore::RateMap>( m ) {
    
}


RateMap* RateMap::clone() const {
    return new RateMap( *this );
}


/* Map calls to member methods */
RevObject* RateMap::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
   
    if (name == "nChars")
    {
        int n = (int)this->dagNode->getValue().getNumberOfCharacters();
        return new Natural(n);
    }
    else if (name == "nStates")
    {
        int n = (int)this->dagNode->getValue().getNumberOfStates();
        return new Natural(n);
    }

    return ModelObject<RevBayesCore::RateMap>::executeMethod( name, args );
}


/* Get class name of object */
const std::string& RateMap::getClassName(void) {
    
    static std::string rbClassName = "RateMap";
    
	return rbClassName;
}

/* Get class type spec describing type of object */
const TypeSpec& RateMap::getClassTypeSpec(void) {
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return rbClass;
}


/** Get the methods for this vector class */
/* Get method specifications */
const MethodTable& RateMap::getMethods(void) const {
    
    static MethodTable    methods                     = MethodTable();
    static bool           methodsSet                  = false;
    
    if ( methodsSet == false ) {
        
        // add method for call "x.nChars()" as a function
        ArgumentRules* nCharsArgRules = new ArgumentRules();
        methods.addFunction("nChars",  new MemberFunction( Natural::getClassTypeSpec(), nCharsArgRules) );
        
        // add method for call "x.nStates()" as a function
        ArgumentRules* nStatesArgRules = new ArgumentRules();
        methods.addFunction("nStates",  new MemberFunction( Natural::getClassTypeSpec(), nStatesArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &ModelObject<RevBayesCore::RateMap>::getMethods() );
        methodsSet = true;
    }
    
    
    return methods;
}



/** Get the type spec of this class. We return a member variable because instances might have different element types. */
const TypeSpec& RateMap::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}
