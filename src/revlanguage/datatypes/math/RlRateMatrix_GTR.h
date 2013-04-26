/**
 * @file
 * This file contains the declaration of the RevLanguage wrapper of the model class.
 *
 * @brief Declaration of RlModel
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-08-06 20:14:22 +0200 (Mon, 06 Aug 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id: Real.h 1746 2012-08-06 18:14:22Z hoehna $
 */

#ifndef RlRateMatrix_GTR_H
#define RlRateMatrix_GTR_H

#include "RateMatrix_GTR.h"
#include "RlModelVariableWrapper.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class RateMatrix_GTR : public RlModelVariableWrapper<RevBayesCore::RateMatrix_GTR> {
        
    public:
        
        RateMatrix_GTR(void);                                                                                                        //!< Default constructor
        RateMatrix_GTR(RevBayesCore::RateMatrix_GTR *m);                                                                                                        //!< Default constructor
        RateMatrix_GTR(RevBayesCore::TypedDagNode<RevBayesCore::RateMatrix_GTR> *d);                                                                                                        //!< Default constructor
        
        // Basic utility functions
        RateMatrix_GTR*                     clone(void) const;                                                                                  //!< Clone object
        static const std::string&           getClassName(void);                                                                                 //!< Get class name
        static const TypeSpec&              getClassTypeSpec(void);                                                                             //!< Get class type spec
        const TypeSpec&                     getTypeSpec(void) const;                                                                            //!< Get language type of the object
        
        // Member method inits
        const MethodTable&                  getMethods(void) const;                                             //!< Get methods
        RbLanguageObject*                   executeMethod(const std::string& name, const std::vector<Argument>& args);  //!< Override to map member methods to internal functions
        
        
    };
    
}

#endif