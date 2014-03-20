#include "Natural.h"
#include "RealPos.h"
#include "RbException.h"
#include "RbUtil.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;
    
/** Default constructor */
RealPos::RealPos( void ) : Real( 1.0 ) {
}


/** Construct from double */
RealPos::RealPos( RevBayesCore::TypedDagNode<double> *x ) : Real( x ) {
    
    if ( x->getValue() < 0.0 )
        throw RbException( "Nonpositive value for " + getClassName() );
}


/** Construct from double */
RealPos::RealPos( double x ) : Real( x ) {

    if ( x < 0.0 )
        throw RbException( "Nonpositive value for " + getClassName() );
}


/** Construct from int */
RealPos::RealPos( int x ) : Real( x ) {

    if ( x < 0 )
        throw RbException( "Nonpositive value for " + getClassName() );
}


/**
 * Generic addition operator.
 * We test if the rhs is of a type that we use for a specialized addition operation.
 *
 * \param[in]   rhs     The right hand side operand of the addition operation.
 *
 * \return              A new object holding the sum.
 */
RbLanguageObject* RealPos::add( const RbLanguageObject& rhs ) const 
{
    
    if ( rhs.getTypeSpec().isDerivedOf( RealPos::getClassTypeSpec() ) )
        return add( static_cast<const RealPos&>( rhs ) );
    
    if ( rhs.getTypeSpec().isDerivedOf( Natural::getClassTypeSpec() ) )
        return add( static_cast<const Natural&>( rhs ) );
    
    return Real::add( rhs );
}


/**
 * Specialized addition operation between two positive real numbers.
 * The return value is also of type positive real number.
 *
 * \param[in]   rhs     The right hand side operand of the addition operation.
 *
 * \return              A new object holding the sum.
 */
RealPos* RealPos::add(const RevLanguage::Natural &rhs) const
{
    
    RealPos *n = new RealPos( value->getValue() + rhs.getValue() );
    
    return n;
}


/**
 * Specialized addition operation between two positive real numbers.
 * The return value is also of type positive real number.
 *
 * \param[in]   rhs     The right hand side operand of the addition operation.
 *
 * \return              A new object holding the sum.
 */
RealPos* RealPos::add(const RevLanguage::RealPos &rhs) const
{
    
    RealPos *n = new RealPos( value->getValue() + rhs.getValue() );
    
    return n;
}


/** Clone object */
RealPos* RealPos::clone( void ) const {

	return  new RealPos( *this );
}


/**
 * Generic division operator.
 * We test if the rhs is of a type that we use for a specialized division operation.
 *
 * \param[in]   rhs     The right hand side operand of the division operation.
 *
 * \return              A new object holding the ratio.
 */
RbLanguageObject* RealPos::divide( const RbLanguageObject& rhs ) const 
{
    
    if ( rhs.getTypeSpec().isDerivedOf( RealPos::getClassTypeSpec() ) )
        return divide( static_cast<const RealPos&>( rhs ) );
    
    if ( rhs.getTypeSpec().isDerivedOf( Natural::getClassTypeSpec() ) )
        return divide( static_cast<const Natural&>( rhs ) );
    
    return Real::divide( rhs );
}


/**
 * Specialized division operation between two positive real numbers.
 * The return value is also of type positive real number.
 *
 * \param[in]   rhs     The right hand side operand of the division operation.
 *
 * \return              A new object holding the ratio.
 */
RealPos* RealPos::divide(const RevLanguage::Natural &rhs) const
{
    
    RealPos *n = new RealPos( value->getValue() / rhs.getValue() );
    
    return n;
}


/**
 * Specialized division operation between two positive real numbers.
 * The return value is also of type positive real number.
 *
 * \param[in]   rhs     The right hand side operand of the division operation.
 *
 * \return              A new object holding the ratio.
 */
RealPos* RealPos::divide(const RevLanguage::RealPos &rhs) const
{
    
    RealPos *n = new RealPos( value->getValue() / rhs.getValue() );
    
    return n;
}


/** Get class name of object */
const std::string& RealPos::getClassName(void) { 
    
    static std::string rbClassName = "RealPos";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& RealPos::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Real::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& RealPos::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/**
 * Generic multiplication operator.
 * We test if the rhs is of a type that we use for a specialized multiplication operation.
 *
 * \param[in]   rhs     The right hand side operand of the multiplication operation.
 *
 * \return              A new object holding the product.
 */
RbLanguageObject* RealPos::multiply( const RbLanguageObject& rhs ) const 
{
    
    if ( rhs.getTypeSpec().isDerivedOf( RealPos::getClassTypeSpec() ) )
        return multiply( static_cast<const RealPos&>( rhs ) );
    
    if ( rhs.getTypeSpec().isDerivedOf( Natural::getClassTypeSpec() ) )
        return multiply( static_cast<const Natural&>( rhs ) );
    
    return Real::multiply( rhs );
}


/**
 * Specialized multiplication operation between two positive real numbers.
 * The return value is also of type positive real number.
 *
 * \param[in]   rhs     The right hand side operand of the multiplication operation.
 *
 * \return              A new object holding the product.
 */
RealPos* RealPos::multiply(const RevLanguage::Natural &rhs) const
{
    
    RealPos *n = new RealPos( value->getValue() * rhs.getValue() );
    
    return n;
}


/**
 * Specialized multiplication operation between two positive real numbers.
 * The return value is also of type positive real number.
 *
 * \param[in]   rhs     The right hand side operand of the multiplication operation.
 *
 * \return              A new object holding the product.
 */
RealPos* RealPos::multiply(const RevLanguage::RealPos &rhs) const
{
    
    RealPos *n = new RealPos( value->getValue() * rhs.getValue() );
    
    return n;
}
