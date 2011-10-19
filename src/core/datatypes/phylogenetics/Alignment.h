/**
 * @file
 * This file contains the declaration of Alignment, which is
 * class that holds a character matrix in RevBayes.
 *
 * @brief Declaration of Alignment
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef Alignment_H
#define Alignment_H

#include "Character.h"
#include "Matrix.h"
#include "Sequence.h"
#include "ValueRule.h"
#include "Vector.h"
#include "VectorString.h"

#include <set>
#include <string>
#include <vector>

class ArgumentRule;
class DAGNode;


const std::string Alignment_name = "Alignment";

class Alignment : public Matrix {

    public:
                                            Alignment(const std::string& characterType);                                //!< Constructor requires character type
                                            Alignment(const Alignment& x);                                              //!< Copy constructor to deal with sequenceTypeRule
        virtual                            ~Alignment(void);                                                            //!< Destructor to deal with sequenceTypeRule

        // Overloaded operators
        Alignment&                          operator=(const Alignment& x);                                              //!< Assignment operator
        const Sequence&                     operator[](size_t i) const;                                                 //!< Subscript operator (const)

        // Basic utility functions
        Alignment*                          clone(void) const;                                                          //!< Clone object
        const VectorString&                 getClass(void) const;                                                       //!< Get class vector   
        const TypeSpec&                     getTypeSpec(void) const;                                                    //!< Get language type of the object
        void                                printValue(std::ostream& o) const;                                          //!< Print value for user
        std::string                         richInfo(void) const;                                                       //!< Complete info

        // Member variable rules
        const MemberRules&                  getMemberRules(void) const;                                                 //!< Get member rules

        // Member method inits
        const MethodTable&                  getMethods(void) const;                                                     //!< Get methods
        RbLanguageObject*                   executeOperation(const std::string& name, Environment& args);               //!< Execute method
    
        // Container functions
        void                                clear();
        void                                setElement(const size_t index, RbLanguageObject* elem);                     //!< Set element with type conversion

        // Matrix functions
        Character*                          getElement(size_t row, size_t col) const;                                   //!< Get element or subcontainer
        void                                setElement(size_t row, size_t col, RbLanguageObject* var);                  //!< set element
        void                                resize(size_t nRows, size_t nCols);                                         //!< Resize to new length vector
        void                                transpose(void);                                                            //!< Transpose the matrix

        // Alignment functions
        void                                addSequence(Sequence* obs);                                                 //!< Add taxon name
        void                                excludeCharacter(size_t i);                                                 //!< Exclude character
        void                                excludeTaxon(size_t i);                                                     //!< Exclude taxon
        void                                excludeTaxon(std::string& s);                                               //!< Exclude taxon
        const Character&                    getCharacter(size_t tn, size_t cn) const;                                   //!< Return a reference to a character element in the character matrix
        const std::string&                  getDataType(void) const;                                                    //!< Returns the data type for the matrix
        const std::string&                  getFileName(void) const;                                                    //!< Returns the name of the file the data came from
        size_t                              getNumberOfCharacters(void) const;                                          //!< Number of characters
        size_t                              getNumberOfStates(void) const;                                              //!< Get the number of states for the characters in this matrix
        size_t                              getNumberOfTaxa(void) const;                                                //!< Number of taxa
        const Sequence&                     getSequence(size_t tn) const;                                               //!< Return a reference to a sequence in the character matrix
        const std::string&                  getTaxonNameWithIndex(size_t idx) const;                                    //!< Returns the idx-th taxon name
        bool                                isCharacterExcluded(size_t i) const;                                        //!< Is the character excluded
        bool                                isTaxonExcluded(size_t i) const;                                            //!< Is the taxon excluded
        bool                                isTaxonExcluded(std::string& s) const;                                      //!< Is the taxon excluded
        Vector*                             makeSiteColumn(size_t cn) const;                                            //!< Return a reference to a sequence in the character matrix
        void                                restoreCharacter(size_t i);                                                 //!< Restore character
        void                                restoreTaxon(size_t i);                                                     //!< Restore taxon
        void                                restoreTaxon(std::string& s);                                               //!< Restore taxon
        void                                setFileName(const std::string fn) { fileName = fn; }                        //!< Set the file name

    private:
        // Utility functions
        size_t                              indexOfTaxonWithName(std::string& s) const;                                 //!< Get the index of the taxon
        bool                                isCharacterConstant(size_t idx) const;                                      //!< Is the idx-th character a constant pattern?
        bool                                isCharacterMissingOrAmbiguous(size_t idx) const;                            //!< Does the character have missing or ambiguous data?
        size_t                              numConstantPatterns(void) const;                                            //!< The number of constant patterns
        size_t                              numMissAmbig(void) const;                                                   //!< The number of patterns with missing or ambiguous characters

        // Member variables
        std::set<size_t>                    deletedTaxa;                                                                //!< Set of deleted taxa
        std::set<size_t>                    deletedCharacters;                                                          //!< Set of deleted characters
        std::string                         fileName;                                                                   //!< The path/filename from where this matrix originated
        std::vector<std::string>            sequenceNames;                                                              //!< names of the sequences
        size_t                              sequenceLength;                                                             //!< The length of each sequence
        std::string                         characterType;                                                              //!< Rule describing sequence type
        TypeSpec                            typeSpec;                                                                   //!< The type of this character matrix including element type
};

#endif
