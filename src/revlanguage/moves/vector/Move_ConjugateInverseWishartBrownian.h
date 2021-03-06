#ifndef Move_ConjugateInverseWishartBrownian_H
#define Move_ConjugateInverseWishartBrownian_H

#include "SimpleMove.h"
#include "RlMove.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    /**
     * @brief Rev Wrapper of a scaling move on a uniformly chosen element of a real valued vector.
     *
     * This class is the RevLanguage wrapper of SingleElementScale.
     *
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @copyright GPL version 3
     * @since 2013-11-17, version 1.0
     */
    class Move_ConjugateInverseWishartBrownian : public Move {
        
    public:
        
        Move_ConjugateInverseWishartBrownian(void);                                                                                                                   //!< Default constructor
        
        // Basic utility functions
        virtual Move_ConjugateInverseWishartBrownian*       clone(void) const;                                                                              //!< Clone the object
        void                                                constructInternalObject(void);                                                                  //!< We construct the a new internal move.
        static const std::string&                           getClassType(void);                                                                             //!< Get Rev type
        static const TypeSpec&                              getClassTypeSpec(void);                                                                         //!< Get class type spec
        const MemberRules&                                  getParameterRules(void) const;                                                                     //!< Get member rules (const)
        virtual const TypeSpec&                             getTypeSpec(void) const;                                                                        //!< Get language type of the object
        virtual void                                        printValue(std::ostream& o) const;                                                              //!< Print value (for user)
        
    protected:
        
        void                                                setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);             //!< Set member variable
        
        RevPtr<const RevVariable>                           x;
        RevPtr<const RevVariable>                           kappa;
        RevPtr<const RevVariable>                           df;                                                                                             //!< The variable telling if to tune or not.
        
    };
    
}

#endif
