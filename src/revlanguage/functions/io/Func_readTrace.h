/**
 * @file
 * This file contains the declaration of Func_readTrace 
 *
 * @brief Declaration of Func_readTrace
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-05-27 16:10:36 +0200 (Sun, 27 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func_readCharacterData.h 1587 2012-05-27 14:10:36Z hoehna $
 */

#ifndef Func_readTrace_H
#define Func_readTrace_H

#include "RlFunction.h"
#include "RbFileManager.h"
#include "RlTreeTrace.h"

#include <map>
#include <string>
#include <vector>


namespace RevLanguage {
    
    class Func_readTrace :  public Function {
        
    public:
        // Basic utility functions
        Func_readTrace*                 clone(void) const;                                                      //!< Clone the object
        static const std::string&       getClassName(void);                                                     //!< Get class name
        static const TypeSpec&          getClassTypeSpec(void);                                                 //!< Get class type spec
        const TypeSpec&                 getTypeSpec(void) const;                                                //!< Get language type of the object
        
        // Regular functions
        RevObject*                      execute(void);                                                          //!< Execute function
        const ArgumentRules&            getArgumentRules(void) const;                                           //!< Get argument rules
        const TypeSpec&                 getReturnType(void) const;                                              //!< Get type of return value
        
    private:
        void                            formatError(RevBayesCore::RbFileManager& fm, std::string& errorStr);                  //!< Format the error string when (mis)reading files
    };
    
}

#endif

