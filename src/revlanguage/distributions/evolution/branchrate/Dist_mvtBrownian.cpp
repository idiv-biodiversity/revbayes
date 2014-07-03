//
//  RlWhiteNoiseProcess.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-22.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//



#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Dist_mvtBrownian.h"
#include "MultivariateBrownianPhyloProcess.h"
#include "PrecisionMatrix.h"
#include "Real.h"
#include "RealSymmetricMatrix.h"
#include "RlTimeTree.h"
#include "StochasticNode.h"
#include "TimeTree.h"
#include "Vector.h"

using namespace RevLanguage;


Dist_mvtBrownian* Dist_mvtBrownian::clone( void ) const {
    return new Dist_mvtBrownian(*this);
}


RevBayesCore::MultivariateBrownianPhyloProcess* Dist_mvtBrownian::createDistribution( void ) const {
    // get the parameters

    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree>* tau = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    
    RevBayesCore::TypedDagNode<RevBayesCore::PrecisionMatrix>* om  = static_cast<const RealSymmetricMatrix&>( omega->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<std::vector<double> >* r  = static_cast<const Vector<Real>&>( rootval->getRevObject() ).getDagNode();
    
    RevBayesCore::MultivariateBrownianPhyloProcess* process    = new RevBayesCore::MultivariateBrownianPhyloProcess( tau, om, r );
    
    return process;

}



/* Get class name of object */
const std::string& Dist_mvtBrownian::getClassName(void) {
    
    static std::string rbClassName = "Dist_multivariateBrownian";
    
	return rbClassName;
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_mvtBrownian::getClassTypeSpec(void) {
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass;
}



/** Return member rules (no members) */
const MemberRules& Dist_mvtBrownian::getMemberRules(void) const {
    
    static MemberRules dist;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        dist.push_back( new ArgumentRule( "tree" , true, TimeTree::getClassTypeSpec() ) );
        dist.push_back( new ArgumentRule( "omega", true, RealSymmetricMatrix::getClassTypeSpec() ) );
        dist.push_back( new ArgumentRule( "rootval", true, Vector<Real>::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return dist;
}


const TypeSpec& Dist_mvtBrownian::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}




/** Print value for user */

 void Dist_mvtBrownian::printValue(std::ostream& o) const {
    
    o << " MultivariateBrownian(";
    
    o << "tau=";
    if ( tree != NULL ) {
        o << tree->getName();
    } else {
        o << "?";
    }

     o << ",";
     
     o << "omega=";
     if ( omega != NULL ) {
         o << omega->getName();
     } else {
         o << "?";
     }

     o << ",";
     
     o << "rootval=";
     if ( rootval != NULL ) {
         o << rootval->getName();
     } else {
         o << "?";
     }

     o << ")";
}



/** Set a member variable */
void Dist_mvtBrownian::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {

    if ( name == "tree" )
    {
        tree = var;
    }
    else if ( name == "omega" )
    {
        omega = var;
    }
    else if ( name == "rootval" )
    {
        rootval = var;
    }
    else {
        Distribution::setConstMemberVariable(name, var);
    }
}

