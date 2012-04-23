/**
 * @file
 * This file contains the declaration of Func_reference, which is
 * the function used to create a reference of a variable to another one. The expression
 * b := a
 * creates a references of 'a' stored in 'b'. If 'a' is changed, so is 'b'.
 * Other names for a reference could be lookup function, alias function, etc. They all would be synonyms.
 *
 * @brief Declaration of Func_reference for creating a reference to another variable
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @interface RbFunction
 * @package functions
 * @since Version 1.0, 2011-11-07
 *
 * $Id$
 */

#ifndef Func_reference_H
#define Func_reference_H

#include "RbFunction.h"


class Func_reference :  public RbFunction {
    
public:
    // Basic utility functions
    Func_reference*             clone(void) const;                                          //!< Clone object
    static const std::string&   getClassName(void);                                         //!< Get class name
    static const TypeSpec&      getClassTypeSpec(void);                                     //!< Get class type spec
    const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object
    
    // Func_reference functions
    const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
    const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return val

protected:
    const RbLanguageObject&     executeFunction(const std::vector<const RbObject*>& args);  //!< Execute function
    

};

#endif
