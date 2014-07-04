/**
 * @file
 * This file contains the declaration of RevObject, which is
 * the RevBayes abstract base class for all language objects.
 *
 * @brief Declaration of RbObject
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2009-07-27
 *
 * $Id$
 */

#ifndef RevObject_H
#define RevObject_H

#include "DagNode.h"
#include "RevPtr.h"

#include <ostream>
#include <string>
#include <vector>


namespace RevLanguage {

class Argument;
class ArgumentRule;
class ArgumentRules;
class MethodTable;
class TypeSpec;
class Variable;

typedef ArgumentRules MemberRules;                                                                                                      //!< Member rules type def, for convenience



class RevObject {
    
public:
    virtual                            ~RevObject(void);                                                                                //!< Virtual destructor
    
    // overloaded operators
    bool                                operator==(const RevObject& x) const { return &x == this; }
    bool                                operator!=(const RevObject& x) const { return !operator==(x); }
    bool                                operator<(const RevObject& x) const { return false; }
    
    // Basic operator functions
    virtual RevObject*                  add(const RevObject &rhs) const;                                                                //!< Addition operator used for example in '+=' statements
    virtual void                        decrement(void);                                                                                //!< Decrement operator used for example in 'a--' statements
    virtual RevObject*                  divide(const RevObject &rhs) const;                                                             //!< Division operator used for example in '/=' statements
    virtual void                        increment(void);                                                                                //!< Increment operator used for example in 'a++' statements
    virtual RevObject*                  multiply(const RevObject &rhs) const;                                                           //!< Multiplication operator used for example in '-=' statements
    virtual RevObject*                  subtract(const RevObject &rhs) const;                                                           //!< Subtraction operator used for example in '*=' statements
    
    // Basic utility functions you have to override (also getClassTypeSpec()!)
    virtual RevObject*                  clone(void) const = 0;                                                                          //!< Clone object
    static const std::string&           getClassName(void);                                                                             //!< Get class name
    static const TypeSpec&              getClassTypeSpec(void);                                                                         //!< Get class type spec
    virtual const TypeSpec&             getTypeSpec(void) const = 0;                                                                    //!< Get the object type spec of the instance
    virtual void                        printValue(std::ostream& o) const = 0;                                                          //!< Print value for user
    
    // Basic utility functions you may want to override
    virtual void                        constructInternalObject(void);                                                                  //!< Objects using the ConstructorFunction should overwrite this function for proper initializiation of the internal objects.
    virtual RevObject*                  convertTo(const TypeSpec& type) const;                                                          //!< Convert to type
    virtual RevBayesCore::DagNode*      getDagNode(void) const;                                                                         //!< Get my internal value node (if applicable)
//    virtual size_t                      getDim(void) const;                                                                             //!< Get the number of dimensions of the internal object, e.g., 1 if this is a vector.
    virtual const std::string&          getInternalValueType(void) const;                                                               //!< Get the internal value type (RevBayesCore object or primitive C++ object)
    virtual bool                        hasDagNode(void) const;                                                                         //!< Do I have an internal value node?
    virtual bool                        isConvertibleTo(const TypeSpec& type) const;                                                    //!< Is convertible to type?
    virtual RevObject*                  makeDagReference(void);                                                                         //!< Make an object referencing the dag node of this object
    virtual void                        printStructure(std::ostream& o) const;                                                          //!< Print structure of language object for user
    
    // Functions that wrapper objects containing RB core objects might want to override
    virtual bool                        isConstant(void) const;                                                                         //!< Is this variable and the internally stored deterministic node constant?
    virtual void                        makeConstantValue(void);                                                                        //!< Convert the stored variable to a constant variable (if applicable)
    virtual void                        replaceVariable(RevObject *newObj);                                                                   //!< Replace the internal DAG node and prepare to replace me
    virtual void                        setName(const std::string &n);                                                                  //!< Set the name of the variable (if applicable)
    virtual void                        setDagNode(RevBayesCore::DagNode *newNode);                                                     //!< Set or replace the internal dag node (and keep me)

    // Member variable functions you may want to override
    virtual RevObject*                  executeMethod(const std::string& name, const std::vector<Argument>& args);                      //!< Override to map member methods to internal functions
    virtual RevObject*                  getMember(const std::string& name) const;                                                       //!< Get member variable
    virtual const MemberRules&          getMemberRules(void) const;                                                                     //!< Get member rules
    virtual const MethodTable&          getMethods(void) const;                                                                         //!< Get member methods (const)
    virtual bool                        hasMember(const std::string& name) const;                                                       //!< Has this object a member with name
    virtual void                        setConstMember(const std::string& name, const RevPtr<const Variable> &var);                     //!< Set member variable
    virtual void                        setMember(const std::string& name, const RevPtr<Variable> &var);                                //!< Set member variable
    virtual std::string                 toString(void) const;                                                                           //!< Get this object as a string, i.e., get some info about it.
    
    // Basic utility functions you should not have to override
    const std::string&                  getType(void) const;                                                                            //!< Get the type specification as a string
    bool                                isTypeSpec(const TypeSpec& typeSpec) const;                                                     //!< Does the language type of the object fit type specification typeSpec?

protected:
    RevObject(void) {}                                                                                                                  //!< No objects of this class
    RevObject(const MemberRules& memberRules);                                                                                          //!< Standard constructor

    virtual void                        setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var);             //!< Set member variable
    virtual void                        setMemberVariable(const std::string& name, const RevPtr<Variable> &var);                        //!< Set member variable

    
};
    
}

// Global functions using the class
std::ostream&                           operator<<(std::ostream& o, const RevLanguage::RevObject& x);                                   //!< Overloaded output operator



#endif
