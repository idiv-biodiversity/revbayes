/**
 * @file
 * This file contains the declaration of Set, a container type
 * that acts as a base class for all constant Sets.
 *
 * This class is a wrapper for the stl-class set and we provide additional RevBayes functionality
 * (e.g. getTypeSpec and getClassTypeSpec for argument checking)
 *
 * Sets are like vectors, but can only contain one instance of the same value.
 * In that sense, sets are unique vectors. Furthermore, the ordering doesn't matter in sets.
 *
 * @brief Declaration of Set
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-02-25 10:17:07 +0100 (Sat, 25 Feb 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 *
 * $Id: Set.h 1247 2012-02-25 09:17:07Z hoehna $
 */

#ifndef Set_H
#define Set_H

#include "Container.h"

#include <iostream>
#include <set>

template <typename setType>
class Set : public Container {
    
public:
    Set(void);                                                                                                      //!< Default constructor with type RbLanguageObject
    Set(const Set<setType>& v);                                                                                     //!< Copy Constructor
    
    virtual                                        ~Set(void);                                                      //!< Virtual destructor 
    
    // Basic utility functions you have to override
    Set*                                            clone(void) const;                                              //!< Clone object
    static const std::string&                       getClassName(void);                                             //!< Get class name
    static const TypeSpec&                          getClassTypeSpec(void);                                         //!< Get class type spec
    const TypeSpec&                                 getTypeSpec(void) const;                                        //!< Get language type of the object
    void                                            printValue(std::ostream& o) const;                              //!< Print value for user
    
    setType&                                        operator[](size_t i);                                           //!< Index op allowing change
    const setType&                                  operator[](size_t i) const;                                     //!< Const index op
    Set<setType>&                                   operator=(const Set<setType>& x);                               //!< Assignment operator
    
    // Set functions
    void                                            clear(void);                                                    //!< Clear
    const setType&                                  find(const setType& x) const;                                   //!< Find the element being equal to that one
    int                                             findIndex(const setType& x) const;                              //!< Find the element being equal to that one
    const RbObject&                                 getElement(size_t index) const;                                 //!< Get element
    RbObject&                                       getElement(size_t index);                                       //!< Get element (non-const to return non-const element)
    const std::set<setType* >&                      getValue(void) const;                                           //!< Get the stl Set of elements
    void                                            insert(setType* x);                                             //!< Add element in front
    size_t                                          size(void) const;                                               //!< get the number of elements in the AbstractSet
    
protected:
    
    // We store internally pointers to our objects. This is necessary because elements can be also of the derived type and we need to be able to make proper copies of the set and all its elements
    std::set<setType* >                             elements;
};

#endif


#include "RbException.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "TypeSpec.h"

/** Set type of elements */
template <typename setType>
Set<setType>::Set(void) : Container( setType::getClassTypeSpec() ) {
}


/** Copy Constructor */
template <typename setType>
Set<setType>::Set(const Set<setType> &v) : Container(v) {
    
    typename std::set<setType* >::const_iterator it;
    // copy all the elements by deep copy
    for ( it = v.elements.begin(); it != v.elements.end(); it++) {
        // We need to type cast because clone return by default RbObject*
        setType *copy = static_cast<setType*>( (*it)->clone() );
        elements.insert( copy );
    }
    
}


/** Destructor. Free the memory of the elements. */
template <typename setType>
Set<setType>::~Set(void) {
    
    // just call clear which will free the memory of the elements
    clear();
}

/** Assignment operator; make sure we get independent elements */
template <typename setType>
Set<setType>& Set<setType>::operator=( const Set<setType>& x ) {
    
    if ( this != &x ) {
        
        // First assign using parent assignment operator. This will test to make sure the containers
        // are of the same type, and throw an error if they are not. By calling it before we destroy
        // our own elements, we can make sure that an assignment error leaves us intact, which it should
        Container::operator=( x );
        
        // just call clear which will free the memory of the objects
        clear();
        
        typename std::set<setType*>::const_iterator i;
        for ( i = x.elements.begin(); i != x.elements.end(); i++ ) {
            setType *element = static_cast<setType>( (*i)->clone() );
            elements.insert( element );
        }
    }
    
    return ( *this );
}

/** Subscript operator */
template <typename setType>
setType& Set<setType>::operator[](size_t index) {
    
    if (index > elements.size())
        throw RbException("Index out of bounds");
    
    // get the iterator to the first element
    typename std::set<setType*>::iterator i;
    i = elements.begin();
    // move the iterator index elements forward
    std::advance(i, index);
    
    return *(*i);
}


/** Subscript const operator */
template <typename setType>
const setType& Set<setType>::operator[](size_t index) const {
    
    if (index > elements.size())
        throw RbException("Index out of bounds");
    
    // get the iterator to the first element
    typename std::set<setType*>::iterator i;
    i = elements.begin();
    // move the iterator index elements forward
    std::advance(i, index);
    
    return *(*i);
}


/** Clear contents of value container */
template <typename setType>
void Set<setType>::clear( void ) {
    
    typename std::set<setType*>::iterator i;
    for ( i = elements.begin(); i != elements.end(); i++) {
        setType* theElement = *i;
        delete theElement;
    }
    
    elements.clear();
}


template <typename setType>
Set<setType>* Set<setType>::clone() const {
    return new Set<setType>(*this);
}


/**
 * Find the given element.
 * We rely on overloaded operator== in the element classes.
 * 
 * \param x the element we are looking for. 
 * \return The reference to the element we found or RbNullObject
 */
template <typename setType>
const setType& Set<setType>::find(const setType& x) const {
    
    // get the iterator to the first element
    typename std::set<setType*>::const_iterator i;
    for ( i = elements.begin(); i != elements.end(); i++) {
        setType& element = *(*i);
        
        // test if the object matches
        // note that we rely on the implemented operator==
        if ( element == x ) {
            return element;
        }
    }
    
    return RbNullObject::getInstance();
}


/**
 * Find the index of the given element.
 * We rely on overloaded operator== in the element classes to check for matches.
 * 
 * \param x the element we are looking for. 
 * \return The index or -1 if we didn't find it.
 */
template <typename setType>
int Set<setType>::findIndex(const setType& x) const {
    
    // get the iterator to the first element
    typename std::set<setType*>::const_iterator i;
    
    // initialize the index
    int index = 0;
    for ( i = elements.begin(); i != elements.end(); i++, index++) {
        const setType& element = *(*i);
        
        // test if the object matches
        // note that we rely on the implemented operator==
        if ( element == x ) {
            return index;
        }
    }
    
    return -1;
}


/** Get class name of object */
template <typename setType>
const std::string& Set<setType>::getClassName(void) { 
    
    static std::string rbClassName = "Set";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
template <typename setType>
const TypeSpec& Set<setType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Container::getClassTypeSpec() ), new TypeSpec( setType::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get element */
template <typename setType>
const RbObject& Set<setType>::getElement(size_t index) const {
    
    return operator[](index);
}


/** Get element */
template <typename setType>
RbObject& Set<setType>::getElement(size_t index) {
    
    return operator[](index);
}


template <typename setType>
const std::set<setType* >& Set<setType>::getValue(void) const {
    return elements;
}


/** Get the type spec of this class. We return a member variable because instances might have different element types. */
template <typename setType>
const TypeSpec& Set<setType>::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = TypeSpec( getClassName(), new TypeSpec( Container::getClassTypeSpec() ), new TypeSpec( setType::getClassTypeSpec() ) );
    
    return typeSpec;
}


/** Print value for user */
template <typename setType>
void Set<setType>::printValue( std::ostream& o ) const {
    
    o << "[ ";
    typename std::set<setType*>::const_iterator i;
    for ( i = elements.begin(); i != elements.end(); i++ ) {
        if ( i != elements.begin() )
            o << ", ";
        if ( (*i) == NULL )
            o << "NULL";
        else 
            (*i)->printValue(o);
    }
    o <<  " ]";
    
}


/** Push an int onto the back of the vector after checking */
template <typename setType>
void Set<setType>::insert( setType *x ) {
    if (x == NULL) {
        elements.push_back( NULL );
    }
    else {
        throw RbException( "Trying to set " + getClassName() + "[] with invalid value" );
    }
}



/** Get the size of the vector */
template <typename setType>
size_t Set<setType>::size( void ) const {
    
    return elements.size();
    
}

