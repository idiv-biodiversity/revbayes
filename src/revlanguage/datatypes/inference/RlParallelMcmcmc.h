//
//  RlParallelMcmcmc.h
//  rb_mlandis
//
//  Created by Michael Landis on 3/25/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__RlParallelMcmcmc__
#define __rb_mlandis__RlParallelMcmcmc__


#include "ParallelMcmcmc.h"
#include "Mcmc.h"
#include "TypedDagNode.h"
#include "WorkspaceToCoreWrapperObject.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class ParallelMcmcmc : public WorkspaceToCoreWrapperObject<RevBayesCore::ParallelMcmcmc> {
        
    public:
        
        ParallelMcmcmc(void);                                                                                                               //!< Default constructor
        
        // Basic utility functions
        virtual ParallelMcmcmc*                     clone(void) const;                                                                      //!< Clone object
        void                                        constructInternalObject(void);                                                          //!< We construct the a new internal MCMC object.
        static const std::string&                   getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                                                 //!< Get class type spec
        const MemberRules&                          getParameterRules(void) const;                                                          //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                                //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                                      //!< Print value (for user)
        
        // Member method inits
        virtual RevPtr<Variable>                    executeMethod(const std::string& name, const std::vector<Argument>& args, bool &f);     //!< Override to map member methods to internal functions
        
    protected:
        
        void                                        setConstParameter(const std::string& name, const RevPtr<const Variable> &var);          //!< Set member variable
        
        RevPtr<const Variable>                      model;
        RevPtr<const Variable>                      moves;
        RevPtr<const Variable>                      monitors;
        RevPtr<const Variable>                      moveSchedule;
        RevPtr<const Variable>                      numChains;
        RevPtr<const Variable>                      numProcessors;
        RevPtr<const Variable>                      startHeat;
        RevPtr<const Variable>                      deltaHeat;
        RevPtr<const Variable>                      sigmaHeat;
        RevPtr<const Variable>                      swapInterval;
        RevPtr<const Variable>                      append;
    };
    
}

#endif /* defined(__rb_mlandis__RlParallelMcmcmc__) */
