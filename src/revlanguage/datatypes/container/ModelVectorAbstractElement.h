#ifndef ModelVectorAbstractElement_H
#define ModelVectorAbstractElement_H

#include "ModelContainer.h"
#include "RbConstIterator.h"
#include "RbVector.h"

#include <iostream>
#include <vector>

namespace RevLanguage {
    
    /**
     * @brief ModelVectorAbstractElement: templated class for Rev vectors of abstract model objects
     *
     * We can rely on the template class specialization of ModelContainer for
     * vectors to provide most of the functionality we need.
     *
     * The class is based on a value type of RbVector, which is a vector of pointers
     * to objects. This means that it can be used for abstract internal value types,
     * but it is less efficient for non-abstract value types and should not be used
     * for those.
     */
    template <typename rlType>
    class ModelVectorAbstractElement : public ModelContainer< rlType, 1, RevBayesCore::RbVector<typename rlType::valueType> > {
        
    public:
        
        typedef typename rlType::valueType                                      elementType;
        typedef typename RevBayesCore::RbVector<typename rlType::valueType>     valueType;
        
                                                        ModelVectorAbstractElement(void);                                               //!< Default constructor
                                                        ModelVectorAbstractElement(const valueType& v);                                 //!< Constructor from vector of values
                                                        ModelVectorAbstractElement(RevBayesCore::TypedDagNode<valueType>* n);           //!< Constructor from value node
        
        // Basic utility functions you have to override
        virtual ModelVectorAbstractElement<rlType>*     clone(void) const;                                                              //!< Clone object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        virtual const TypeSpec&                         getTypeSpec(void) const;                                                        //!< Get the object type spec of the instance
        
        // Basic utility function you do not have to override
        void                                            printValue(std::ostream& o) const;                                              //!< Print value for user
        
        // Type conversion functions
        RevObject*                                      convertTo(const TypeSpec& type) const;                                          //!< Convert to requested type
        virtual bool                                    isConvertibleTo(const TypeSpec& type) const;                                    //!< Is this object convertible to the requested type?
        
        // Container functions you may want to override to protect from assignment
        virtual RevPtr<Variable>                        findOrCreateElement(const std::vector<size_t>& oneOffsetIndices);               //!< Find or create element variable
        virtual RevPtr<Variable>                        getElement(size_t oneOffsetIndex);                                              //!< Get element variable (single index)
        
        // Container functions you should not have to override
        RevPtr<Variable>                                getElement(const std::vector<size_t>& oneOffsetIndices);                        //!< Get element variable (generic version)
        void                                            makeCompositeValue();                                                           //!< Convert to a composite vector (or not)
        RevObject*                                      makeIndirectReference(void);                                                    //!< Make an object referencing the dag node of this object
        void                                            setElements(std::vector<RevObject*> elems, const std::vector<size_t>& lengths); //!< Set elements from Rev objects
        
    protected:
        
        // Helper function
        RevPtr<Variable>                                getElementFromValue(size_t oneOffsetIndex) const;                               //!< Get element from value
        
    };
    
}


#include "ArgumentRule.h"
#include "DeterministicNode.h"
#include "MethodTable.h"
#include "RbException.h"
#include "RevPtr.h"
#include "TypeSpec.h"
#include "Variable.h"

#include <algorithm>

using namespace RevLanguage;


/**
 * Default constructor: construct an empty vector. We do not need to store the element type
 * because this can be accessed from our Rev type specification.
 */
template <typename rlType>
ModelVectorAbstractElement<rlType>::ModelVectorAbstractElement( void ) :
    ModelContainer< rlType, 1, RevBayesCore::RbVector<typename rlType::valueType> >()
{
    this->setDagNode( new ContainerNode<rlType, valueType>("") );
}


/**
 * Constructor from a valueType object, that is, from an RbVector containing pointers
 * to the internal value type instances.
 */
template <typename rlType>
ModelVectorAbstractElement<rlType>::ModelVectorAbstractElement( const valueType &v ) :
    ModelContainer< rlType, 1, RevBayesCore::RbVector<typename rlType::valueType> >( v )
{
}


/**
 * Constructor from a DAG node appropriate for the model vector
 * object.
 */
template <typename rlType>
ModelVectorAbstractElement<rlType>::ModelVectorAbstractElement( RevBayesCore::TypedDagNode<valueType> *n ) :
    ModelContainer< rlType, 1, RevBayesCore::RbVector<typename rlType::valueType> >( rlType::getClassTypeSpec(), n )
{
}


/** Get a type-safe clone of the object */
template <typename rlType>
ModelVectorAbstractElement<rlType>* ModelVectorAbstractElement<rlType>::clone() const
{
    return new ModelVectorAbstractElement<rlType>( *this );
}


/**
 * Convert to object of another type. Here we use the setElements function
 * of the Container base class to do generic type conversion in all cases
 * where the elements are individually convertible to the desired element
 * type.
 */
template <typename rlType>
RevObject* ModelVectorAbstractElement<rlType>::convertTo(const TypeSpec &type) const
{
    // First check that we are not asked to convert to our own type
    if ( type == getClassTypeSpec() )
        return this->clone();
    
    // No type conversion for simple containers
    if ( !this->isComposite() )
        throw RbException( "Invalid attempt to do type conversion on a simple vector of abstract elements" );
        
    // test whether we want to convert to another generic model vector
    if ( type.getDim() == 1 && type.getParentType() == getClassTypeSpec().getParentType() )
    {
        // We are both model vectors. Rely on generic code to cover all allowed conversions
        
        // First generate an empty model vector of the desired type
        Container* theConvertedContainer = Workspace::userWorkspace().makeNewEmptyContainer( type.getElementType(), type.getDim() );
        
        // Now generate the vector of elements
        std::vector<RevObject*> theConvertedObjects;
        for ( size_t i = 1; i <= this->size(); ++i )
        {
            RevObject& orgElement = const_cast< ModelVectorAbstractElement<rlType>* >( this )->getElement( i )->getRevObject();
            if ( orgElement.isTypeSpec( *type.getElementTypeSpec() ) )
                theConvertedObjects.push_back( orgElement.clone() );
            else
                theConvertedObjects.push_back( orgElement.convertTo( *type.getElementTypeSpec() ) );
        }
        
        // Set the elements of the converted container, which assumes ownership of the objects
        std::vector<size_t> lengths;
        lengths.push_back( this->size() );
        theConvertedContainer->setElements( theConvertedObjects, lengths );
        
        // Now return the converted container object
        return theConvertedContainer;
    }
    
    // Call the base class if all else fails. This will eventually throw an error if the type conversion is not supported.
    return this->ModelContainer< rlType, 1, std::vector<typename rlType::valueType> >::convertTo( type );
}


/**
 * Find an element for assignment. If we are a stochastic node or
 * deterministic node, we ask the user first if they want to break up
 * the previous association of the variable with a single dynamic DAG
 * node and make it a composite of several individual DAG nodes instead.
 * If we are a constant node, we simply go ahead quietly with the
 * replacement.
 */
template<typename rlType>
RevPtr<Variable> ModelVectorAbstractElement<rlType>::findOrCreateElement( const std::vector<size_t>& oneOffsetIndices )
{
    // First retrieve the variable elements vector, if possible
    ContainerNode<rlType, valueType>* theContainerNode = dynamic_cast< ContainerNode<rlType, valueType>* >( this->dagNode );
    if ( theContainerNode == NULL )
    {
        if ( dynamic_cast< RevBayesCore::ConstantNode<valueType>* >( this->dagNode ) == NULL )
        {
            bool answer = UserInterface::userInterface().ask( "Do you want to convert the container to a composite container" );
            
            if ( answer == true )
                this->makeCompositeValue();
            else
                throw RbException( "Assignment to elements of a simple container not allowed" );
        }
        else
            this->makeCompositeValue();
        
        theContainerNode = static_cast< ContainerNode<rlType, valueType>* >( this->dagNode );
    }
    
    // Check the indices first
    if ( oneOffsetIndices.size() == 0 || oneOffsetIndices[0] == 0 )
        throw RbException( "Assignment to multiple vector elements not supported (yet)" );
    
    // Check that there are no superfluous indices
    if ( oneOffsetIndices.size() > 1 )
        throw RbException( "Unexpected extra indices to a vector" );
    
    // Resize if myIndex is out of range; fill with NULL variables
    for ( size_t it = this->size(); it < oneOffsetIndices[0]; ++it )
        theContainerNode->push_back( NULL );
    
    // Return the assignable element
    return theContainerNode->getElement( oneOffsetIndices[0] - 1 );
}


/**
 * Get Rev type of object. This is the Rev object element type followed by
 * a single set of square brackets. This provides a nice and convenient way
 * of specifying generic types of vectors for all Rev object types.
 */
template <typename rlType>
const std::string& ModelVectorAbstractElement<rlType>::getClassType(void)
{
    static std::string revType = rlType::getClassType() + "[]";
    
	return revType;
}


/**
 * Get class type spec describing type of object. Note that we have to
 * use the special version of the TypeSpec constructor because we derive
 * directly from ModelContainer and have the special "rlType[]" type
 * specification.
 */
template <typename rlType>
const RevLanguage::TypeSpec& ModelVectorAbstractElement<rlType>::getClassTypeSpec(void)
{
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), &ModelContainer<rlType, 1, valueType>::getClassTypeSpec(), &rlType::getClassTypeSpec() );
    
	return revTypeSpec;
}


/**
 * Get an element. We support reference assignment by giving out a smart pointer to the actual
 * element if we are a composite node. We do that by retrieving the elements from our container
 * node, and then returning the appropriate element or elements if a slice is requested.
 *
 * If we are a constant node, we could transform ourselves to a composite conatiner node first
 * and then get the elements from it. However, we opt not to do that here, to keep the vector
 * simple if possible.
 *
 * If we are a simple dynamic node, or a constant node, we simply give out a new temporary variable,
 * a clone of the original element. In this way, we can guard ourselves from attempts by others to
 * modify the element in cases where that would be inappropriate.
 *
 * You can still assign to elements of simple vector nodes through calling the createOrFindElement
 * function. It will ask the user if they want to convert a dynamic node to a composite node. See
 * that function for more information.
 *
 * Note that we use oneOffsetIndices, so substract one before using them to retrieve elements
 * from the internal elements vector. A zero in the first position of the index means that we
 * want to return the entire vector.
 */
template<typename rlType>
RevPtr<Variable> ModelVectorAbstractElement<rlType>::getElement( size_t oneOffsetIndex )
{
    // First check if we want to return a slice
    if ( oneOffsetIndex == 0 )
        return new Variable( this->clone() );
    
    // We want a single element; first check that index is in range
    if ( oneOffsetIndex > this->size() )
        throw RbException( "Index out of range" );
    
    // Check whether we have element variables
    ContainerNode<rlType, valueType>* theContainerNode = dynamic_cast< ContainerNode<rlType, valueType>* >( this->dagNode );
    
    // We need to retrieve the element from the value vector if we do not have a container node
    if ( theContainerNode == NULL )
        throw RbException( "Impossible to reconstruct element from simple container of abstract elements" );
    
    // We are a composite vector with a container node. We retrieve the element from its elements vector
    return theContainerNode->getElement( oneOffsetIndex - 1 );
}


/**
 * Get an element (generic version). For vectors, it makes sense to delegate to the getElement
 * function that takes a single index.
 */
template<typename rlType>
RevPtr<Variable> ModelVectorAbstractElement<rlType>::getElement( const std::vector<size_t>& oneOffsetIndices )
{
    // First check if we want to return a slice
    if ( oneOffsetIndices.size() == 0 )
        return getElement( 0 );
    
    // Check for superfluous indices
    if ( oneOffsetIndices.size() > 1 )
        throw RbException( "Unexpected extra indices to vector" );
    
    // Delegate to single-index getElement function
    return this->getElement( oneOffsetIndices[0] );
}


/**
 * Get element(s) from value vector. Note that we use the language default of
 * one-offset indices, so remember to subtract one before accessing internal
 * objects.
 *
 * Note also that a zero index means that we want the entire vector.
 */
template <typename rlType>
RevPtr<Variable> ModelVectorAbstractElement<rlType>::getElementFromValue( size_t oneOffsetIndex ) const
{
    if ( oneOffsetIndex == 0 )
    {
        ModelVectorAbstractElement<rlType>* newVector = this->clone();
        newVector->makeConstantValue();
        
        return new Variable( newVector );
    }

    if ( oneOffsetIndex > this->size() )
        throw RbException( "Index out of range" );

    return new Variable( new rlType( *( this->getValue()[ oneOffsetIndex - 1 ] ) ) );
}


/** Get the type spec of this class */
template <typename rlType>
const RevLanguage::TypeSpec& ModelVectorAbstractElement<rlType>::getTypeSpec(void) const
{
    return getClassTypeSpec();
}


/**
 * In this function we check whether this type is convertible to some other
 * Rev object type. Here we focus entirely on supporting conversion to
 * other generic vectors with compatible elements. This is not done automatically
 * because of the templating: a vector of RealPos does not inherit from a vector
 * of Real, for example.
 */
template <typename rlType>
bool ModelVectorAbstractElement<rlType>::isConvertibleTo( const TypeSpec& type ) const
{
    // No type conversion for simple vectors of abstract elements
    if ( !this->isComposite() )
        return false;
    
    // Now proceed with the elements we have
    if ( type.getDim() == 1 && type.getParentType() == getClassTypeSpec().getParentType() )
    {
        // We want to convert to another generic model vector
        
        // Simply check whether our elements can convert to the desired element type
        for ( size_t i = 1; i <= this->size(); ++i )
        {
            const RevObject& orgElement = const_cast< ModelVectorAbstractElement<rlType>* >( this )->getElement( i )->getRevObject();
            
            // Test whether this element is already of the desired element type or can be converted to it
            if ( !orgElement.isTypeSpec( *type.getElementTypeSpec() ) && !orgElement.isConvertibleTo( *type.getElementTypeSpec() ) )
                return false;
        }
        
        return true;
    }
    
    return Container::isConvertibleTo( type );
}


/**
 * Convert the vector to a composite value, that is, a vector where each element has
 * its own DAG node controlling its value. Unfortunately, it is not possible to do
 * so for a simple vector of abstract elements, since it does not know what Rev objects
 * correspond to its internal value elements.
 */
template <typename rlType>
void ModelVectorAbstractElement<rlType>::makeCompositeValue( void )
{
    if ( dynamic_cast< ContainerNode<rlType, valueType>* >( this->dagNode ) != NULL )
        return;
    
    throw RbException( "Simple vector of abstract elements cannot be converted to a composite vector" );
}


/**
 * Make indirect reference. This is relevant when we try to make an indirect reference
 * to the object in a dynamic evaluation context. These are statements of the type
 *
 *    a := b
 *
 * where this function is called if we are variable b.
 */
template <typename rlType>
RevObject* ModelVectorAbstractElement<rlType>::makeIndirectReference(void) {
    
    IndirectReferenceNode< ModelVectorAbstractElement<rlType> >* newNode =
    new IndirectReferenceNode< ModelVectorAbstractElement<rlType> >( "", this->getDagNode() );
    
    ModelVectorAbstractElement<rlType>* newObj = this->clone();
    
    newObj->setDagNode( newNode );
    
    return newObj;
}


/**
 * Print the value of the vector, taking the possibility of NA object
 * elements into account, and respecting the formatting of the
 * model object elements. We do this by retrieving the elements, one
 * by one, and printing them using their own printValue implementation.
 * Among other things, this takes care of proper formatting.
 *
 * We make a perfectly safe const cast here, since we only utilize the
 * const printValue function of the element.
 */
template <typename rlType>
void ModelVectorAbstractElement<rlType>::printValue( std::ostream& o ) const
{
    size_t lineLength = 75;
    
    std::ostringstream s, t;
    s << "[ ";
    size_t curLength = 2;
    for ( size_t i = 1; i <= this->size(); ++i )
    {
        RevPtr<Variable> elem = const_cast< ModelVectorAbstractElement<rlType>* >(this)->getElement( i );
        if ( elem->isNAVar() )
            t << "NA";
        else
            elem->getRevObject().printValue( t );
        if ( i != this->size() )
            t << ", ";
        if ( curLength + t.str().size() > lineLength )
        {
            s << std::endl << "  ";
            curLength = 2;
        }
        s << t.str();
        curLength += t.str().size();
        t.str("");
    }
    if ( curLength + 2 > lineLength )
        s << std::endl << "]";
    else
        s << " ]";
    o << s.str();
}


/**
 * Set elements from a vector of Rev objects. We assume that we want
 * a composite container, since simple containers of abstract elements
 * have very limited functionality. This function will typically be called
 * by the type conversion code.
 */
template <typename rlType>
void ModelVectorAbstractElement<rlType>::setElements( std::vector<RevObject*> elems, const std::vector<size_t>& lengths )
{
    this->setDagNode( new ContainerNode<rlType, valueType>( "", elems, lengths ) );
}


#endif
