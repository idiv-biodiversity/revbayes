/**
 * @file
 * This file contains the declaration of RbFunction_pchisq, the
 * dnorm() function.
 *
 * @brief Declaration of RbFunction_pchisq
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author RevBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since Version 1.0, 2009-08-26
 *
 * $Id$
 */

#ifndef RbFunction_pchisq_H
#define RbFunction_pchisq_H

#include "RbFunction.h"
#include <iostream>
#include <string>
#include <vector>

class ArgumentRule;
class RbDouble;
class RbObject;

/** This is the class for the pchisq() function, which takes a single
 *  scalar real or int.
 */
class RbFunction_pchisq :  public RbFunction {

    public:
									RbFunction_pchisq(void);                                        //!< Default constructor, allocate workspace
                                    RbFunction_pchisq(const RbFunction_pchisq& s);                  //!< Copy constructor
								   ~RbFunction_pchisq(void);                                        //!< Destructor, delete workspace

        RbObject*                   clone(void) const ;                                             //!< clone this object
        const StringVector&         getClass(void) const;                                           //!< Get class
        std::string                 toString(void) const;                                           //!< General info on object

        const RbObject*             executeOperation(const std::vector<DAGNode*>& arguments) const; //!< Get result
		const ArgumentRule**        getArgumentRules(void) const;                                   //!< Get the number of argument rules
        const std::string           getReturnType(void) const;                                      //!< Get return type

    protected:
        RbDouble*                   value;                                                          //!< Workspace for result
};

#endif



/**
 * @file
 * This file contains the declaration of RbFunction_pchisq, the
 * pchisq() function.
 *
 * @brief Declaration of RbFunction_pchisq
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author Fredrik Ronquist and the REvBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since Version 1.0, 2009-08-26
 *
 * $Id$
 */

#ifndef RbFunction_pchisq_H
#define RbFunction_pchisq_H

#include "ArgumentRule.h"
#include "RbDouble.h"
#include "RbFunction.h"
#include "RbObject.h"
#include <iostream>
#include <string>
#include <vector>

class RbDumpState;

/** This is the class for the pchisq() function, which takes a single
 *  scalar real or int.
 *
 *  @package    functions
 *  @implements RbFunction, RbStandardFunction
 */
class RbFunction_pchisq :  public RbFunction {

    public:
        static const StringVector   rbClass;            //!< Static class attribute

            RbFunction_pchisq(void);                              //!< Default constructor, allocate workspace
            RbFunction_pchisq(const RbFunction_pchisq& s);      //!< Copy constructor
	        ~RbFunction_pchisq();                             //!< Destructor, delete workspace

        // implemented abstract/virtual functions from base classes
        RbObject*           clone(void) const ;                                 //!< clone this object
        void                print(std::ostream &c) const;                       //!< Print the value to ostream c
        void                dump(std::ostream& c);                              //!< Dump to ostream c
        void                resurrect(const RbDumpState& x);                    //!< Resurrect from dumped state
        bool                equals(const RbObject* o) const;                    //!< Comparison

        const StringVector& getClass() const { return rbClass; }        //!< Get class
        void                printValue(std::ostream& o) const;          //!< Print value (for user)
        std::string         toString(void) const;                       //!< General info on object

        // Type conversion
        bool                isConvertibleTo(const std::string& type) const;
        RbObject*           convertTo(const std::string& type) const;
        virtual const int           getNumberOfRules() const;                   //!< Get number of argument rules for the function

        RbObject*                 executeOperation(const std::vector<DAGNode*>& arguments);              //!< Get result

        // overloaded operators
        RbObject&           operator=(const RbObject& o);
        RbFunction_pchisq&   operator=(const RbFunction_pchisq& o);

    protected:
        RbDouble*                     value;              //!< Workspace for result
};

#endif
