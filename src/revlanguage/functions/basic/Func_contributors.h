/**
 * @file
 * This file contains the declaration of Func_contributors, which is
 * used to provide the user with information on contributors.
 *
 * @brief Declaration of Func_contributors
 *
 * @author Fredrik Ronquist
 * @license GPL version 3
 */

#ifndef Func_contributors_H
#define Func_contributors_H

#include "RlFunction.h"

namespace RevLanguage {
    
    class Func_contributors : public Function {
        
    public:
        Func_contributors( void );
        
        // Basic utility functions
        Func_contributors*                      clone(void) const;                                                          //!< Clone object
        static const std::string&               getClassName(void);                                                         //!< Get class name
        static const TypeSpec&                  getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                         getTypeSpec(void) const;                                                    //!< Get language type of the object
        
        // Func_contributors functions
        const ArgumentRules&                    getArgumentRules(void) const;                                               //!< Get argument rules
        const TypeSpec&                         getReturnType(void) const;                                                  //!< Get type of return val
        bool                                    throws(void) const { return false; }                                         //!< Function may throw exceptions
        
        RevObject*                              execute(void);                                                              //!< Execute function
        
    };
    
}

#endif
