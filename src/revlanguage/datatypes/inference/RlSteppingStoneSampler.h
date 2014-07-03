#ifndef RlSteppingStoneSampler_H
#define RlSteppingStoneSampler_H

#include "SteppingStoneSampler.h"
#include "TypedDagNode.h"
#include "WorkspaceObject.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    
    /**
     * @brief Rev wrapper of the Stepping-Stone-Sampler class.
     *
     * The stepping stone sampler analyzes the output of a power posterior and
     * computes the marginal likelihood.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since Version 1.0, 2012-06-17
     *
     */
    class SteppingStoneSampler : public WorkspaceObject<RevBayesCore::SteppingStoneSampler> {
        
    public:
        
        SteppingStoneSampler(void);                                                                                         //!< Default constructor
        
        // Basic utility functions
        virtual SteppingStoneSampler*               clone(void) const;                                                      //!< Clone object
        void                                        constructInternalObject(void);                                          //!< We construct the a new internal PowerPosterior object.
        static const std::string&                   getClassName(void);                                                     //!< Get class name
        static const TypeSpec&                      getClassTypeSpec(void);                                                 //!< Get class type spec
        const MemberRules&                          getMemberRules(void) const;                                             //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                      //!< Print value (for user)
        
        // Member method inits
        const MethodTable&                          getMethods(void) const;                                                 //!< Get methods
        RevObject*                                  executeMethod(const std::string& name, const std::vector<Argument>& args);  //!< Override to map member methods to internal functions
        
    protected:
        
        void                                        setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var);     //!< Set member variable
        
        RevPtr<const Variable>                      likelihoodColumnName;
        RevPtr<const Variable>                      powerColumnName;
        RevPtr<const Variable>                      delimmiter;
        RevPtr<const Variable>                      filename;
        
    };
    
}

#endif
