#include "RlAbstractDiscreteCharacterData.h"
#include "ArgumentRule.h"
#include "MemberFunction.h"
#include "Natural.h"
#include "RlBoolean.h"
#include "RlString.h"
#include "RlTaxonData.h"
#include "Vector.h"


using namespace RevLanguage;

AbstractDiscreteCharacterData::AbstractDiscreteCharacterData(void) : AbstractCharacterData() 
{
    
}


AbstractDiscreteCharacterData::AbstractDiscreteCharacterData( RevBayesCore::AbstractDiscreteCharacterData *v) : AbstractCharacterData( v )
{
    
}


AbstractDiscreteCharacterData::AbstractDiscreteCharacterData( RevBayesCore::TypedDagNode<RevBayesCore::AbstractCharacterData> *d) : AbstractCharacterData( d ) 
{

}


AbstractDiscreteCharacterData* AbstractDiscreteCharacterData::clone() const {
    
    return new AbstractDiscreteCharacterData( *this );
}


/* Get class name of object */
const std::string& AbstractDiscreteCharacterData::getClassName(void) { 
    
    static std::string rbClassName = "AbstractDiscreteCharacterData";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& AbstractDiscreteCharacterData::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( AbstractCharacterData::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get the type spec of this class. We return a member variable because instances might have different element types. */
const TypeSpec& AbstractDiscreteCharacterData::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}


const RevBayesCore::AbstractDiscreteCharacterData& RevLanguage::AbstractDiscreteCharacterData::getValue( void ) const 
{
    
    return static_cast<const RevBayesCore::AbstractDiscreteCharacterData&>( this->dagNode->getValue() );
}



