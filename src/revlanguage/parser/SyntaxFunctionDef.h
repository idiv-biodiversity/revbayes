/**
 * @file
 * This file contains the declaration of SyntaxFunctionDef, which is
 * used to hold definitions of functions (user-defined functions).
 *
 * @brief Declaration of SyntaxFunctionDef
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef SyntaxFunctionDef_H
#define SyntaxFunctionDef_H

#include "SyntaxElement.h"
#include "SyntaxFormal.h"
#include "SyntaxVariable.h"
#include "TypeSpec.h"

#include <ostream>
#include <list>
#include <string>

namespace RevLanguage {

class SyntaxFunctionDef : public SyntaxElement {

    public:
        SyntaxFunctionDef(const std::string &type, const std::string &name, std::list<SyntaxFormal*>*   formals, std::list<SyntaxElement*>*  stmts);    //!< Standard constructor
        SyntaxFunctionDef(const SyntaxFunctionDef& x);                                                                                                  //!< Copy constructor
	    
        virtual                        ~SyntaxFunctionDef();                                                                                            //!< Destructor

        // Assignment operator
        SyntaxFunctionDef&              operator=(const SyntaxFunctionDef& x);                                                                          //!< Assignment operator

        // Basic utility functions
        SyntaxFunctionDef*              clone() const;                                                                                                  //!< Clone object
        void                            printValue(std::ostream& o) const;                                                                              //!< Print info about object

        // Regular functions
        RevPtr<Variable>                evaluateContent(Environment& env);                                                                              //!< Get semantic value
        void                            replaceVariableWithConstant(const std::string& name, const RevObject& c);                                       //!< Replace the syntax variable with name by the constant value. Loops have to do that for their index variables.

    protected:
        std::list<SyntaxElement*>*      code;                                                                                                           //!< The list of statements
        std::string                     functionName;                                                                                                   //!< The name of the function
        std::list<SyntaxFormal*>*       formalArgs;                                                                                                     //!< The formal arguments
        TypeSpec                        returnType;                                                                                                     //!< The return type specification of the function
    
};
    
}

#endif

