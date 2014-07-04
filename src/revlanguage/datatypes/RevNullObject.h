/**
 * @file
 * This file contains the declaration of RevNullObject, which is
 * the RevBayes class used for NULL references.
 *
 * @brief Declaration of RevNullObject
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-02-14
 *
 * $Id$
 */

#ifndef RevNullObject_H
#define RevNullObject_H

#include "RevObject.h"

#include <ostream>
#include <string>

namespace RevLanguage {

class RevNullObject : public RevObject {
    
public:
    RevNullObject(void);    
    RevNullObject(const RevNullObject& n);                                                                          //!< Default constructor
    virtual                            ~RevNullObject(void) {}                                                      //!< Virtual destructor
    
    bool                                operator==(void* ptr);                                                      //!< Compare to NULL pointer
    bool                                operator==(const RevNullObject& obj);                                       //!< Compare to Rev NULL object
    bool                                operator==(const RevObject& obj);                                           //!< Compare to Rev object
    
    static RevNullObject&     getInstance(void)                                           //!< Get the memory manager
    {
        static RevNullObject theInstance = RevNullObject();
        return theInstance;
    }

    
    // Basic utility functions you have to override (also getClassTypeSpec()!)
    RevNullObject*                      clone(void) const;                                                      //!< Clone object
    static const std::string&           getClassName(void);                                                     //!< Get class name
    static const TypeSpec&              getClassTypeSpec(void);                                                 //!< Get class type spec
    const TypeSpec&                     getTypeSpec(void) const;                                                //!< Get the type spec for this instance
    void                                printValue(std::ostream& o) const;                                      //!< Print value for user
        
    
};

// Global functions using the class
std::ostream&                           operator<<(std::ostream& o, const RevNullObject& x);                    //!< Overloaded output operator

}

#endif
