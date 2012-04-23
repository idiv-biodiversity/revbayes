/**
 * @file
 * This file contains the declaration of Func_quit, which is
 * the function used to quit the program.
 *
 * @brief Declaration of Func_quit
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @interface RbFunction
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id$
 */

#ifndef Func_quit_H
#define Func_quit_H

#include "RbFunction.h"

class Func_quit :  public RbFunction {

    public:
        // Basic utility functions
        Func_quit*                  clone(void) const;                                          //!< Clone object
        static const std::string&   getClassName(void);                                         //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                     //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object

        // Func_quit functions
        const RbLanguageObject&     execute(void);                                              //!< Execute function
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return val

    protected:

};

#endif
