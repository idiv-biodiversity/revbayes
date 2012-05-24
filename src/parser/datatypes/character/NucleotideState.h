/**
 * @file
 * This file contains the declaration of NucleotideState, which is
 * the abstract base class for nucleotide observations (DNA & RNA) 
 * data types in RevBayes.
 *
 * @brief Declaration of NucleotideState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef NucleotideState_H
#define NucleotideState_H

#include "CharacterStateDiscrete.h"
#include <ostream>
#include <vector>



class NucleotideState : public CharacterStateDiscrete {

    public:
        virtual bool                    operator==(const Character& x) const = 0;            //!< Equality
        virtual bool                    operator!=(const Character& x) const = 0;            //!< Inequality

        // Basic utility functions you should not have to override
        virtual void                    printValue(std::ostream& o) const = 0;               //!< Print value (for user)

        // Basic utility functions you have to override
        virtual NucleotideState*        clone(void) const = 0;                               //!< Clone object
        static const std::string&       getClassName(void);                                  //!< Get class name
        static const TypeSpec&          getClassTypeSpec(void);                              //!< Get class type spec

        // Discrete character observation functions
        void                            addState(const char s);                              //!< Add a character state to the set of character states
        virtual const std::string&      getStateLabels(void) const = 0;                      //!< Get valid state labels
        virtual const char              getState(void) const = 0;                            //!< Get the discrete observation
        void                            setState(const char s);                              //!< Set the discrete observation

    protected:
                                        NucleotideState(void);                               //!< Default constructor
};

#endif
