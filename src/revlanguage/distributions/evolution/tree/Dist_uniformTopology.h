#ifndef Dist_uniformTopology_H
#define Dist_uniformTopology_H

#include "UniformTopologyDistribution.h"
#include "RlTopology.h"
#include "RlTypedDistribution.h"

namespace RevLanguage {
    
    /**
     * The RevLanguage wrapper of the uniform topology distribution
     *
     * The RevLanguage wrapper of the uniform topology distribution connects
     * the variables/parameters of the distribution and creates the internal UniformTopologyDistribution object.
     * Please read the UniformTopologyDistribution.h for more info.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-01-26, version 1.0
     *
     */
    class Dist_uniformTopology :  public TypedDistribution<Topology> {
        
    public:
        Dist_uniformTopology( void );                                                                                                            //!< Default constructor
        
        // Basic utility functions
        Dist_uniformTopology*                    clone(void) const;                                                                              //!< Clone the object
        static const std::string&                       getClassName(void);                                                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                                        //!< Get the type spec of the instance
        const MemberRules&                              getMemberRules(void) const;                                                                     //!< Get member rules (const)
        
        
        // Distribution functions you have to override
        RevBayesCore::UniformTopologyDistribution*      createDistribution(void) const;                                                                 //!< Create the internal distribution object
        
    protected:
        
        void                                            setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var);             //!< Set member variable
        
        
    private:
        
        RevPtr<const Variable>                          numTaxa;                                                                                        //!< Number of taxa of the topology
        RevPtr<const Variable>                          taxonNames;                                                                                     //!< Taxon names
        
    };
    
}

#endif
