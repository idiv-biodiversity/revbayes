/* 
 * File:   Move_MatrixSingleElementSlide.h
 * Author: nl
 *
 * Created on 13 juillet 2014, 18:13
 */

#ifndef Move_MatrixSingleElementSlide_H
#define Move_MatrixSingleElementSlide_H

#include "SimpleMove.h"
#include "RlMove.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class Move_MatrixSingleElementSlide : public Move {
        
    public:
        
        Move_MatrixSingleElementSlide(void);                                                                                      //!< Default constructor (0.0)
        
        // Basic utility functions
        virtual Move_MatrixSingleElementSlide*     clone(void) const;                                                      //!< Clone object
        void                                        constructInternalObject(void);                                          //!< We construct the a new internal SlidingMove.
        static const std::string&                   getClassName(void);                                                     //!< Get class name
        static const TypeSpec&                      getClassTypeSpec(void);                                                 //!< Get class type spec
        const MemberRules&                          getMemberRules(void) const;                                             //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                      //!< Print value (for user)
        
    protected:
        
        void                                        setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var);     //!< Set member variable
        
        RevPtr<const Variable>                      v;
        RevPtr<const Variable>                      lambda;
        RevPtr<const Variable>                      weight;
        RevPtr<const Variable>                      tune;
        
    };
    
}


#endif	/* MOVE_MATRIXSINGLEELEMENTSLIDE_H */

