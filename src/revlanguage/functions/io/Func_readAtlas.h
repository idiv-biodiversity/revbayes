#ifndef Func_readAtlas_H
#define Func_readAtlas_H

#include "Procedure.h"
#include "RbFileManager.h"

#include <map>
#include <string>
#include <vector>


namespace RevLanguage {
    
    class Func_readAtlas :  public Procedure {
        
    public:
        // Basic utility functions
        Func_readAtlas*             clone(void) const;                                                      //!< Clone the object
        static const std::string&   getClassType(void);                                                     //!< Get Rev type
        static const TypeSpec&      getClassTypeSpec(void);                                                 //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                                //!< Get language type of the object
        
        // Regular functions
        RevPtr<RevVariable>            execute(void);                                                          //!< Execute function
        const ArgumentRules&        getArgumentRules(void) const;                                           //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                              //!< Get type of return value
        
    private:
        void                        formatError(RevBayesCore::RbFileManager& fm, std::string& errorStr);                  //!< Format the error string when (mis)reading files
        
    };
    
}

#endif

