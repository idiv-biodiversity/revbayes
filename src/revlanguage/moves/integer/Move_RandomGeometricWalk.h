#ifndef RlMove_RandomGeometricWalk_H
#define RlMove_RandomGeometricWalk_H

#include "RandomGeometricWalkMove.h"
#include "RlMove.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    
    /**
     * The RevLanguage wrapper of the random-geometric-walk move.
     *
     * The RevLanguage wrapper of the random-geometric-walk move simply
     * manages the interactions through the Rev with our core.
     * That is, the internal move object can be constructed and hooked up
     * in a DAG-nove (variable) that it works on.
     * See the Move_RandomGeometricWalk.h for more details.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     *
     */
    class Move_RandomGeometricWalk : public Move {
        
    public:
        
        Move_RandomGeometricWalk(void);                                                                                                          //!< Default constructor
        
        // Basic utility functions
        virtual Move_RandomGeometricWalk*            clone(void) const;                                                                          //!< Clone object
        void                                        constructInternalObject(void);                                                              //!< We construct the a new internal SlidingMove.
        static const std::string&                   getClassType(void);                                                                         //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                                                     //!< Get class type spec
        const MemberRules&                          getParameterRules(void) const;                                                                 //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                                    //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                                          //!< Print value (for user)
        
    protected:
        
        void                                        setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);         //!< Set member variable
        
        RevPtr<const RevVariable>                      x;                                                                                          //!< The variable on which the move works
        RevPtr<const RevVariable>                      p;                                                                                          //!< The tuning parameter
        RevPtr<const RevVariable>                      tune;                                                                                       //!< If autotuning should be used.

    };
    
}

#endif
