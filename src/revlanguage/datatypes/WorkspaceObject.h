/**
 * @file
 * This file contains the declaration of WorkspaceObject, which is the
 * the Rev abstract base class for all language objects wrapping core
 * datatypes that are used to control the analysis and manipulate model
 * objects, such as monitors, moves, models. All workspace objects have
 * in common that they cannot be included in model DAGs.
 *
 * @brief Declaration of WorkspaceObject
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-08-03 17:51:49 +0200 (Fri, 03 Aug 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-08-06
 *
 * $Id: RevObject.h 1734 2012-08-03 15:51:49Z hoehna $
 */

#ifndef WorkspaceObject_H
#define WorkspaceObject_H

#include "RevObject.h"
#include "TypedDagNode.h"

namespace RevLanguage {
    
    template <typename rbType>
    class WorkspaceObject : public RevObject {
        
    public:
        virtual                            ~WorkspaceObject(void);                                                  //!< Virtual destructor
        
        WorkspaceObject&                    operator=(const WorkspaceObject& x);                                    //!< Assignment operator
        
        // the value type definition
        typedef rbType valueType;
        
        // Basic utility functions you have to override (also getClassTypeSpec()!)
        virtual WorkspaceObject*            clone(void) const = 0;                                                  //!< Clone object
        virtual void                        constructInternalObject(void) = 0;                                      //!< We construct the a new internal object.
        static const std::string&           getClassName(void);                                                     //!< Get class name
        static const TypeSpec&              getClassTypeSpec(void);                                                 //!< Get class type spec
        virtual const TypeSpec&             getTypeSpec(void) const = 0;                                            //!< Get the type spec of the instance
        virtual void                        printValue(std::ostream& o) const = 0;                                  //!< Print value for user

                
        // getters and setters
        const rbType&                       getValue(void) const;
        void                                setValue(const rbType& x);
        
    protected:
        WorkspaceObject(void);
        WorkspaceObject(rbType *v);
        WorkspaceObject(const WorkspaceObject &v);
        
        rbType*                             value;
    };
    
}


#include "ConstantNode.h"
#include "TypeSpec.h"

template <typename rbType>
RevLanguage::WorkspaceObject<rbType>::WorkspaceObject() : RevObject(), value( NULL ) {
    
}



template <typename rbType>
RevLanguage::WorkspaceObject<rbType>::WorkspaceObject(rbType *v) : RevObject(), value( v ) {
    
}



template <typename rbType>
RevLanguage::WorkspaceObject<rbType>::WorkspaceObject(const WorkspaceObject &v) : RevObject( v ), value( NULL ) {
    
    if ( v.value != NULL )
    {
        value = v.value->clone();
    }
}



template <typename rbType>
RevLanguage::WorkspaceObject<rbType>::~WorkspaceObject() {
    
    delete value;
}


template <typename rbType>
RevLanguage::WorkspaceObject<rbType>& RevLanguage::WorkspaceObject<rbType>::operator=(const WorkspaceObject &v) {
    
    if ( this != &v ) {
        // free the memory
        delete value;
        value = NULL;
        
        // create own copy
        if ( v.value != NULL ) {
            value = v.value->clone();
        }
    }
    
    return *this;
}


template <typename rbType>
const std::string& RevLanguage::WorkspaceObject<rbType>::getClassName(void) { 
    
    static std::string className = "Control variable";
    
	return className; 
}


/** Get class type spec describing type of object */
template <typename rlType>
const RevLanguage::TypeSpec& RevLanguage::WorkspaceObject<rlType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return rbClass; 
}


template <typename rbType>
const rbType& RevLanguage::WorkspaceObject<rbType>::getValue( void ) const {
    
    return *value;
}



template <typename rbType>
void RevLanguage::WorkspaceObject<rbType>::setValue(const rbType &x) {
    
    // free memory
    delete value;
    value = new rbType( x );
}


#endif
