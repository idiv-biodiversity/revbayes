/**
 * @file
 * This file contains the declaration of the RevLanguage F81 function, which
 * is used to create deterministic variable associated with the F81 function.
 *
 * @brief Declaration and implementation of Func_f81
 *
 * (c) Copyright under GPL version 3
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: Func__add.h 1406 2012-04-20 02:06:14Z hoehna $
 */


#ifndef Func_f81_H
#define Func_f81_H

#include "RlFunction.h"

#include <map>
#include <string>

namespace RevLanguage {
    
    class Func_f81 :  public Function {
        
    public:
        Func_f81( void );
        
        // Basic utility functions
        Func_f81*                                       clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassName(void);                                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevObject*                                      execute(void);                                                                  //!< Execute function
        const ArgumentRules&                            getArgumentRules(void) const;                                                   //!< Get argument rules
        const TypeSpec&                                 getReturnType(void) const;                                                      //!< Get type of return value
        
    };
    
}

#endif
