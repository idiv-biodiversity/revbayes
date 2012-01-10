/**
 * @file
 * This file contains the implementation of FileMonitor, used to save
 * information to file about the monitoring of a variable DAG node.
 *
 * @brief Implementation of FileMonitor
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id$
 */

#include "DagNodeContainer.h"
#include "Ellipsis.h"
#include "Integer.h"
#include "ObjectMonitor.h"
#include "RbException.h"
#include "RbUtil.h"
#include "ValueRule.h"
#include "VariableNode.h"
#include "VectorString.h"


#include <sstream>


// Definition of the static type spec member
const TypeSpec ObjectMonitor::typeSpec(ObjectMonitor_name);

/** Constructor */
ObjectMonitor::ObjectMonitor(void) : Monitor(getMemberRules() ) {
    
}

/** Copy Constructor */
ObjectMonitor::ObjectMonitor(const ObjectMonitor &x) : Monitor(x) {
    
    // shallow copy
    values = x.values;
}

ObjectMonitor::~ObjectMonitor() {
    
}


/** Clone the object */
ObjectMonitor* ObjectMonitor::clone(void) const {
    
    return new ObjectMonitor(*this);
}


/** Get class vector describing type of object */
const VectorString& ObjectMonitor::getClass() const {
    
    static VectorString rbClass = VectorString(ObjectMonitor_name) + ConstantMemberObject::getClass();
    return rbClass;
}

/** Return member rules */
RbPtr<const MemberRules> ObjectMonitor::getMemberRules( void ) const {
    
    static RbPtr<MemberRules> memberRules( new MemberRules() );
    static bool        rulesSet = false;
    
    if (!rulesSet) 
    {
       // memberRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "filename"  , TypeSpec(RbString_name)         ) ) );
        memberRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "printgen"  , TypeSpec(Integer_name)          ) ) );
       // memberRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "separator" , TypeSpec(RbString_name), RbPtr<RbLanguageObject>(new RbString("\t") ) ) ) );
        memberRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "variable"  , TypeSpec(RbLanguageObject_name) ) ) );
        memberRules->push_back( RbPtr<ArgumentRule>( new Ellipsis (               TypeSpec(RbLanguageObject_name) ) ) );
        rulesSet = true;
    }
    
    return RbPtr<const MemberRules>( memberRules );
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& ObjectMonitor::getTypeSpec(void) const {
    return typeSpec;
}


/** Monitor value unconditionally */
void ObjectMonitor::monitor(void) {
    
    for (std::vector<RbPtr<VariableNode> >::const_iterator it=nodes.begin(); it!=nodes.end(); it++) {
        
        // save the value        
        RbPtr< RbLanguageObject> temp = (*it)->getValue()->clone();
        values[(*it)->getName()].push_back( RbPtr<RbObject> (temp) );
    }
    
}


/** Monitor value at generation gen */
void ObjectMonitor::monitor(int gen) {
    
    // get the sampling frequency
    int samplingFrequency = dynamic_cast<const Integer*>( (const RbObject*)getMemberValue("printgen") )->getValue();

    if (gen % samplingFrequency == 0) {
        
        for (std::vector<RbPtr<VariableNode> >::const_iterator it=nodes.begin(); it!=nodes.end(); it++) {
            
            // save the value
            RbPtr< RbLanguageObject> temp = (*it)->getValue()->clone();
            values[(*it)->getName()].push_back( RbPtr<RbObject> (temp) );
        }
        
    }
}


/** Print value for user */
void ObjectMonitor::printValue(std::ostream& o) const {
    
    // get the printing frequency
    int samplingFrequency = dynamic_cast<const Integer*>( (const RbObject*)getMemberValue("printgen") )->getValue();
    
    o << "Monitor: interval = " << samplingFrequency;
}



/** Complete info about object */
std::string ObjectMonitor::richInfo(void) const {
    
    // get the sampling frequency
    int samplingFrequency = dynamic_cast<const Integer*>( (const RbObject*)getMemberValue("printgen") )->getValue();
    
    std::ostringstream o;
    o << "Monitor: interval = " << samplingFrequency;
    
    return o.str();
}

void ObjectMonitor::setMemberVariable(std::string const &name, RbPtr<Variable> var) {
    
    // catch setting of the variables 
    if (name == "variable" || name == "") {
        RbPtr<DAGNode> theNode = var->getDagNode();
        if (theNode->getValue()->isType(DagNodeContainer_name)) {
            RbPtr<DagNodeContainer> theContainer( static_cast<DagNodeContainer*>( (RbLanguageObject*)theNode->getValue() ) );
            for (size_t i = 0; i < theContainer->size(); i++) {
                theNode = static_cast<VariableSlot*>( (RbObject*)theContainer->getElement(i) )->getDagNode();
                if (theNode->isType(VariableNode_name)) {
                    nodes.push_back( RbPtr<VariableNode>( static_cast<VariableNode*>( (DAGNode*)theNode ) ) );
                    values[RbPtr<VariableNode>( static_cast<VariableNode*>( (DAGNode*)theNode ) )->getName()] = Vector();
                    //                } else {
                    //                    throw RbException("Cannot monitor a constant node!");
                }
            }
        }
        else {
            if (theNode->isType(VariableNode_name)) {
                nodes.push_back( RbPtr<VariableNode>( static_cast<VariableNode*>( (DAGNode*)theNode ) ) );
                values[RbPtr<VariableNode>( static_cast<VariableNode*>( (DAGNode*)theNode ) )->getName()] = Vector();
                //            } else {
                //                throw RbException("Cannot monitor a constant node!");
            }
        }
    } 
    else {
        // call parent class to set member variable
        ConstantMemberObject::setMemberVariable( name, var );
    }
}


/** returns the values contained in the values vector for variable with name varName */
RbPtr<Vector> ObjectMonitor::getValues(RbString varName) {
    std::map<RbString,Vector>::iterator it = values.find(varName);
    if (it != values.end()) {
        RbPtr<Vector> toReturn = RbPtr<Vector> ( it->second.clone() );
        return( toReturn );
    }
    
    return RbPtr<Vector>::getNullPtr();
}