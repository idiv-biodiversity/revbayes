#include "ConstantNode.h"
#include "RlBoolean.h"
#include "Natural.h"
#include "Integer.h"
#include "RealPos.h"
#include "Real.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RlString.h"
#include "TypeSpec.h"
#include <climits>
#include <sstream>
#include <climits>

using namespace RevLanguage;

/** Default constructor */
Natural::Natural( void ) : Integer( 0 ) {
}


Natural::Natural( RevBayesCore::TypedDagNode<int> *v ) : Integer( v ) {
    
}


/** Construct from int */
Natural::Natural( int x ) : Integer( x ) {

    if ( x < 0 )
        throw RbException( "Negative value for " + getClassName() );
}


/* Construct from unsigned int */
Natural::Natural( unsigned int x ) : Integer( x ) {
        
}


/* Construct from unsigned long */
Natural::Natural( unsigned long x) : Integer( int(x) ) {

    if ( x > INT_MAX )
        throw RbException( "Value out of range for " + getClassName() );

}


/**
 * Generic addition operator.
 * We test if the rhs is of a type that we use for a specialized addition operation.
 *
 * \param[in]   rhs     The right hand side operand of the addition operation.
 *
 * \return              A new object holding the sum.
 */
RevObject* Natural::add( const RevObject& rhs ) const 
{
    
    if ( rhs.getTypeSpec().isDerivedOf( Natural::getClassTypeSpec() ) )
        return add( static_cast<const Natural&>( rhs ) );
    
    if ( rhs.getTypeSpec().isDerivedOf( RealPos::getClassTypeSpec() ) )
        return add( static_cast<const RealPos&>( rhs ) );
    
    return Integer::add( rhs );
}


/**
 * Specialized addition operation between two natural numbers.
 * The return value is also of type natural number.
 *
 * \param[in]   rhs     The right hand side operand of the addition operation.
 *
 * \return              A new object holding the sum.
 */
Natural* Natural::add(const RevLanguage::Natural &rhs) const
{
    
    Natural *n = new Natural( value->getValue() + rhs.getValue() );
    
    return n;
}


/**
 * Specialized addition operation between a natural and a positiove real number.
 * The return value is also of type positive real number.
 *
 * \param[in]   rhs     The right hand side operand of the addition operation.
 *
 * \return              A new object holding the sum.
 */
RealPos* Natural::add(const RevLanguage::RealPos &rhs) const
{
    
    RealPos *n = new RealPos( value->getValue() + rhs.getValue() );
    
    return n;
}


/** Clone object */
Natural* Natural::clone( void ) const {

	return new Natural( *this );
}


/** Convert to type. The caller manages the returned object. */
RevObject* Natural::convertTo( const TypeSpec& type ) const {

    if ( type == RlBoolean::getClassTypeSpec() )
        return new RlBoolean( value->getValue() == 0 );

    if ( type == Real::getClassTypeSpec() )
        return new Real( value->getValue() );

    if ( type == RealPos::getClassTypeSpec() )
        return new RealPos( value->getValue() );

    if ( type == RlString::getClassTypeSpec() ) {

        std::ostringstream o;
        printValue( o );
        return new RlString( o.str() );
    }

    return Integer::convertTo( type );
}


/**
 * Generic division operator.
 * We test if the rhs is of a type that we use for a specialized division operation.
 *
 * \param[in]   rhs     The right hand side operand of the division operation.
 *
 * \return              A new object holding the ratio.
 */
RevObject* Natural::divide( const RevObject& rhs ) const 
{
    
    if ( rhs.getTypeSpec().isDerivedOf( Natural::getClassTypeSpec() ) )
        return divide( static_cast<const Natural&>( rhs ) );
    
    if ( rhs.getTypeSpec().isDerivedOf( RealPos::getClassTypeSpec() ) )
        return divide( static_cast<const RealPos&>( rhs ) );
    
    return Integer::divide( rhs );
}


/**
 * Specialized division operation between two natural numbers.
 * The return value is of type positive real number.
 *
 * \param[in]   rhs     The right hand side operand of the division operation.
 *
 * \return              A new object holding the ratio.
 */
RealPos* Natural::divide(const RevLanguage::Natural &rhs) const
{
    
    RealPos *n = new RealPos( value->getValue() / double( rhs.getValue() ) );
    
    return n;
}


/**
 * Specialized division operation between a natural and a positiove real number.
 * The return value is also of type positive real number.
 *
 * \param[in]   rhs     The right hand side operand of the division operation.
 *
 * \return              A new object holding the ratio.
 */
RealPos* Natural::divide(const RevLanguage::RealPos &rhs) const
{
    
    RealPos *n = new RealPos( value->getValue() / rhs.getValue() );
    
    return n;
}


/** Get class name of object */
const std::string& Natural::getClassName(void) { 
    
    static std::string rbClassName = "Natural";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Natural::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Integer::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Natural::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Is convertible to type? */
bool Natural::isConvertibleTo( const TypeSpec& type ) const {

    if ( type == RlBoolean::getClassTypeSpec() )
        return true;

    if ( type == Real::getClassTypeSpec() )
        return true;

    if ( type == RealPos::getClassTypeSpec() )
        return true;

    if ( type == RlString::getClassTypeSpec() )
        return true;

    return Integer::isConvertibleTo( type );
}


/**
 * Generic multiplication operator.
 * We test if the rhs is of a type that we use for a specialized multiplication operation.
 *
 * \param[in]   rhs     The right hand side operand of the multiplication operation.
 *
 * \return              A new object holding the product.
 */
RevObject* Natural::multiply( const RevObject& rhs ) const 
{
    
    if ( rhs.getTypeSpec().isDerivedOf( Natural::getClassTypeSpec() ) )
        return multiply( static_cast<const Natural&>( rhs ) );
    
    if ( rhs.getTypeSpec().isDerivedOf( RealPos::getClassTypeSpec() ) )
        return multiply( static_cast<const RealPos&>( rhs ) );
    
    return Integer::multiply( rhs );
}


/**
 * Specialized multiplication operation between two natural numbers.
 * The return value is also of type natural number.
 *
 * \param[in]   rhs     The right hand side operand of the multiplication operation.
 *
 * \return              A new object holding the product.
 */
Natural* Natural::multiply(const RevLanguage::Natural &rhs) const
{
    
    Natural *n = new Natural( value->getValue() * rhs.getValue() );
    
    return n;
}


/**
 * Specialized multiplication operation between a natural and a positive real number.
 * The return value is also of type positive real number.
 *
 * \param[in]   rhs     The right hand side operand of the multiplication operation.
 *
 * \return              A new object holding the product.
 */
RealPos* Natural::multiply(const RevLanguage::RealPos &rhs) const
{
    
    RealPos *n = new RealPos( value->getValue() * rhs.getValue() );
    
    return n;
}
