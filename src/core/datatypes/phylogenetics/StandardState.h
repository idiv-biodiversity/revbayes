/**
 * @file
 * This file contains the declaration of StandardState, which is
 * the class for the Standard data types in RevBayes.
 *
 * @brief Declaration of StandardState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef StandardState_H
#define StandardState_H

#include "CharacterObservationDiscrete.h"
#include <ostream>
#include <vector>



class StandardState : public CharacterObservationDiscrete {

    public:
        // Basic utility functions you should not have to override
        void                            printValue(std::ostream& o) const;                  //!< Print value (for user)

        // Basic utility functions you have to override
        StandardState*                  clone(void) const;                                  //!< Clone object
        const VectorString&             getClass(void) const;                               //!< Get class vector
        std::string                     richInfo(void) const;                               //!< Complete info about object

        // Discrete character observation functions
        const std::string&              getStateLabels(void) const { return stateLabels; }  //!< Get valid state labels
        const char                      getValue(void) const;                               //!< Get the discrete observation
        void                            setValue(const char s);                             //!< Set the discrete observation

    protected:
                                        StandardState(void);                                //!< Default constructor
                                        StandardState(const StandardState& s);              //!< Copy constructor
                                        StandardState(const char s);                        //!< Constructor with nucleotide observation
        const static std::string        stateLabels;                                        //!< The labels for the possible states
};

#endif
