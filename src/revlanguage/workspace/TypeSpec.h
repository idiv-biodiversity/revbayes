/**
 * @file
 * This file contains the declaration of TypeSpec, which is
 * used to hold type specifications.
 *
 * @brief Declaration of TypeSpec
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2010-09-06, version 1.0
 *
 * $Id$
 */

#ifndef TypeSpec_H 
#define TypeSpec_H

#include <string>

namespace RevLanguage {

class TypeSpec {
    
    public:
                                    TypeSpec(const std::string& objType);                                                           //!< Complete constructor (no parent)
                                    TypeSpec(const TypeSpec& base, TypeSpec* elemType);                                             //!< Complete constructor with element type (for Vectors)
                                    TypeSpec(const std::string& objType, TypeSpec* p, TypeSpec* elemType = NULL);                   //!< Complete constructor (with parent)
                                    TypeSpec(const TypeSpec& ts);                                                                   //!< Copy Constructor
        virtual                    ~TypeSpec(void);                                                                                 //!< Destructor
                
                                    // Operators
        TypeSpec&                   operator=(const TypeSpec& x);                                                                   //!< Assignment operator
        bool                        operator==(const TypeSpec& x) const;                                                            //!< Equals operator
        bool                        operator!=(const TypeSpec& x) const { return !operator==(x); }                                  //!< Not equals operator
                                    operator std::string(void) const;                                                               //!< Type conversion to RlString

                                    // Regular functions
        const std::string&          getBaseType(void) const { return baseType; }                                                    //!< Get the element type
        const TypeSpec&             getElementType(void) const { return *elementType; }                                             //!< Get the element type
        const TypeSpec*             getParentType(void) const { return parent; }
        const std::string&          getType(void) const { return type; }                                                            //!< Get object type
        bool                        isDerivedOf(const TypeSpec& x) const;                                                           //!< Test whether the type represented by this object is of the same or derived type of the argument
        void                        setElementType(TypeSpec* et);                                                                   //!< Set the element type of this type spec
        const std::string&          toString(void) const;                                                                           //!< Express as a RlString

    private:
                                    // Member variables
        std::string                 baseType;                                                                                       //!< The base type of the object or objects
        std::string                 type;                                                                                           //!< The full type including base and element types
        TypeSpec*                   elementType;                                                                                    //!< The type of the elements if this is a container
        TypeSpec*                   parent;

};

                                    // Global functions using the class
        std::ostream&               operator<<(std::ostream& o, const TypeSpec& x);                                                 //!< Overloaded output operator
        std::string                 operator+(const std::string& o, const TypeSpec& x);                                             //!< Concatenation to std::string

}

#endif
