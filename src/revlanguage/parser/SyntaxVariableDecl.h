/**
 * @file
 * This file contains the declaration of SyntaxVariableDecl, which is
 * used to hold assignment expressions in the syntax tree. These
 * can be left-arrow, equation or tilde assignments.
 *
 * @brief Declaration of SyntaxVariableDecl
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef SyntaxVariableDecl_H
#define SyntaxVariableDecl_H

#include "SyntaxElement.h"
#include "SyntaxVariable.h"

#include <iostream>
#include <list>
#include <string>

namespace RevLanguage {

class SyntaxVariableDecl : public SyntaxElement {
    
    public:
        SyntaxVariableDecl(const std::string &typeName, std::list<SyntaxElement*>* lengths, const std::string &referenceChar,const std::string &varName);   //!< Basic constructor
        SyntaxVariableDecl(const SyntaxVariableDecl& x);                                                                                                    //!< Copy constructor
        
        virtual                    ~SyntaxVariableDecl();                                                                                                   //!< Destructor
        
        // Assignment operator
        SyntaxVariableDecl&         operator=(const SyntaxVariableDecl& x);                                                                                 //!< Assignment operator
        
        // Basic utility functions
        SyntaxVariableDecl*         clone() const;                                                                                                          //!< Clone object
        void                        printValue(std::ostream& o) const;                                                                                      //!< Print info about object
        
        // Regular functions
        RevPtr<Variable>            evaluateContent(Environment& env);                                                                                      //!< Get semantic value
        void                        replaceVariableWithConstant(const std::string& name, const RevObject& c);                                               //!< Replace the syntax variable with name by the constant value. Loops have to do that for their index variables.
        
    protected:
        std::string                 elementTypeName;                                                                //!< Element type of the variable
        std::list<SyntaxElement*>*  lengthExpr;                                                                     //!< Lengths in different dimensions
        std::string                 referenceSymbol;                                                                //!< Is reference? ("&" or "")
        std::string                 variableName;                                                                   //!< Variable name
    
};
    
}

#endif

