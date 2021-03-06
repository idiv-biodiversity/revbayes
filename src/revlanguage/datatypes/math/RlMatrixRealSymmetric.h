#ifndef RLMatrixRealSymmetric_H
#define RLMatrixRealSymmetric_H

#include <iostream>



#include <iostream>
#include <vector>

#include "ModelObject.h"
#include "RlMatrixReal.h"
#include "TypedDagNode.h"


namespace RevLanguage {
    
    /**
     * The RevLanguage wrapper of the symmetrix matrix real.
     *
     * The RevLanguage wrapper of the symmetric matrix real simply
     * manages the interactions through the Rev with our core.
     * That is, the internal move object can be constructed and hooked up
     * in a DAG-nove (variable) that it works on.
     * See the SymmetricMatrixReal.h for more details.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Nicolas Lartillot and Sebastian Hoehna)
     * @since 2014-03-27, version 1.0
     *
     */
    class MatrixRealSymmetric : public MatrixReal  {
        
    public:
        
        MatrixRealSymmetric(void);
        MatrixRealSymmetric(RevBayesCore::MatrixReal* m);
        MatrixRealSymmetric(RevBayesCore::TypedDagNode<RevBayesCore::MatrixReal> *mat);                                                            //!< Construct from DAG node
        
        
        // the value type definition
        virtual MatrixRealSymmetric*    clone(void) const;                                                      //!< Clone object
        virtual RevObject*              convertTo(const TypeSpec& type) const;                                  //!< Convert to type
//        void                            constructInternalObject(void);                                          //!< We construct the a new internal SlidingMove.
        static const std::string&       getClassType(void);                                                     //!< Get Rev type
        static const TypeSpec&          getClassTypeSpec(void);                                                 //!< Get class type spec
//        const MemberRules&              getParameterRules(void) const;                                             //!< Get member rules (const)
        virtual const TypeSpec&         getTypeSpec(void) const;                                                //!< Get language type of the object
        void                            printValue(std::ostream& o) const;                                      //!< Print value (for user)
        
    };

}

#endif /* defined(__revbayes__MatrixRealSymmetric__) */
