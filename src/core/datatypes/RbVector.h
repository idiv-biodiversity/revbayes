/**
 * @file
 * This file contains the declaration of the RbVector class. 
 * The RbVector is our implementation of the stl vector, actually a wrapper class,
 * that internally stores the values in a stl-vector of pointers so that vectors
 * of abstract base classes can be used as well.
 *
 *
 * @brief Declaration of the RbVector class
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-07-18
 *
 * $Id$
 */

#ifndef RbVector_H
#define RbVector_H

#include "IsAbstract.h"
#include "RbVectorImpl.h"
#include "RbIterator.h"
#include "RbConstIterator.h"
#include "StringUtilities.h"

#include <boost/dynamic_bitset.hpp>


#include <vector>
#include <iostream>
#include <sstream>

namespace RevBayesCore {
    
    template <class valueType>
    class RbVector : public RbVectorImpl<valueType, IsAbstract<valueType>::Is > {
        
    public:
        // constructor(s)
        RbVector();
        RbVector(size_t n);
        RbVector(size_t n, const valueType &v);
        RbVector(const typename RbVectorImpl<valueType, IsAbstract<valueType>::Is >::vectorType &v);
        RbVector(const RbVector<valueType> &v);
        virtual                                            ~RbVector(void);
        
        // public member functions
        RbVector<valueType>*                                clone(void) const;                                                                      //!< Create an independent clone
        void                                                printElement(std::ostream &o, size_t i, std::string sep="\t", int l=-1, bool left=true) const;                                          //!< Print the i-th element
        
        void                                                sort(void);
        
    private:
        
        // private methods
        int                                                 pivot(int l, int r);
        void                                                quicksort(int l, int r);
        void                                                swap( valueType& a, valueType& b);
        
        // private members

    };
    
//    template<>
//    class RbVector<int> : public RbVectorImpl<int, IsAbstract<int>::Is > {
//        
//    public:
//        // constructor(s)
//        RbVector() : RbVectorImpl<int, IsAbstract<int>::Is  >( ) {}
//        RbVector(size_t n) : RbVectorImpl<int, IsAbstract<int>::Is  >( n ) {}
//        RbVector(size_t n, const int &v) : RbVectorImpl<int, IsAbstract<int>::Is  >( n, v ) {}
//        RbVector(const RbVectorImpl<int, IsAbstract<int>::Is >::vectorType &v) : RbVectorImpl<int, IsAbstract<int>::Is  >( v ) {}
//        RbVector(const RbVector<double> &v) : RbVectorImpl<int, IsAbstract<int>::Is  >( ) {  for (size_t i=0; i<v.size(); ++i) push_back( int(v[i]) ); }
//        RbVector(const RbVector<int> &v) : RbVectorImpl<int, IsAbstract<int>::Is  >( v ) {}
//        virtual                                            ~RbVector(void) {}
//        
//        // public member functions
//        RbVector<int>*                                      clone(void) const { return new RbVector<int>( *this ); }                                                                            //!< Create an independent clone
//        void                                                printElement(std::ostream &o, size_t i, std::string sep="\t", int l=-1, bool left=true) const { o << this->operator[](i); }         //!< Print the i-th element
//        
//        void                                                sort(void) { std::sort(this->std::vector<int>::begin(), this->std::vector<int>::end() ); }
//        
//        
//    };
    
    template<>
    class RbVector<double> : public RbVectorImpl<double, IsAbstract<double>::Is > {
        
    public:
        // constructor(s)
        RbVector() : RbVectorImpl<double, IsAbstract<double>::Is  >( ) {}
        RbVector(size_t n) : RbVectorImpl<double, IsAbstract<double>::Is  >( n ) {}
        RbVector(size_t n, const double &v) : RbVectorImpl<double, IsAbstract<int>::Is  >( n, v ) {}
        RbVector(const std::vector<double> &v) : RbVectorImpl<double, IsAbstract<double>::Is  >( v ) {}
        RbVector(const RbVector<int> &v) : RbVectorImpl<double, IsAbstract<double>::Is  >( ) {  for (size_t i=0; i<v.size(); ++i) push_back( double(v[i]) ); }
        RbVector(const RbVector<double> &v) : RbVectorImpl<double, IsAbstract<double>::Is  >( v ) {}
        virtual                                            ~RbVector(void) {}
        
        // public member functions
        RbVector<double>*                                   clone(void) const { return new RbVector<double>( *this ); }                                                                            //!< Create an independent clone
        void                                                printElement(std::ostream &o, size_t i, std::string sep="\t", int l=-1, bool left=true) const { std::stringstream ss; ss << this->operator[](i); std::string s = ss.str(); StringUtilities::fillWithSpaces( s, l, left ); o << s; } //!< Print the i-th element
        
//        StringUtilities::fillWithSpaces( s, columnWidth, false );
        void                                                sort(void) { std::sort(this->std::vector<double>::begin(), this->std::vector<double>::end() ); }
        
        
    };
    
    template <>
    inline void RbVector<int>::printElement(std::ostream& o, size_t idx, std::string sep, int l, bool left) const
    {
        std::stringstream ss;
        ss << this->operator[](idx);
        std::string s = ss.str();
        StringUtilities::fillWithSpaces( s, l, left );
        o << s;
    }
    
    template <>
    inline void RbVector<unsigned int>::printElement(std::ostream& o, size_t idx, std::string sep, int l, bool left) const
    {
        std::stringstream ss;
        ss << this->operator[](idx);
        std::string s = ss.str();
        StringUtilities::fillWithSpaces( s, l, left );
        o << s;
    }
    
    template <>
    inline void RbVector<std::string>::printElement(std::ostream& o, size_t idx, std::string sep, int l, bool left) const
    {
        std::stringstream ss;
        ss << "\"" << this->operator[](idx) << "\"";
        std::string s = ss.str();
        StringUtilities::fillWithSpaces( s, l, left );
        o << s;
    }
    
    template <>
    inline void RbVector<boost::dynamic_bitset<> >::printElement(std::ostream& o, size_t idx, std::string sep, int l, bool left) const
    {
        std::stringstream ss;
        ss << this->operator[](idx);
        std::string s = ss.str();
        StringUtilities::fillWithSpaces( s, l, left );
        o << s;
    }
    
    
    template <class valueType>
    std::ostream&                       operator<<(std::ostream& o, const RbVector<valueType>& x);                            //!< Overloaded output operator

    template <>
    inline std::ostream& operator<<(std::ostream& o, const RevBayesCore::RbVector<std::string>& x) {
        
        size_t lineLength = 75;
        
        o << "[ ";
        size_t curLength = 2;
        
        for (RbConstIterator<std::string> it = x.begin(); it != x.end(); ++it)
        {
            if ( it != x.begin() )
            {
                o << ", ";
            }
            const std::string& v = *it;
            o << "\"" << v << "\"";
        }
        if ( curLength + 2 > lineLength )
            o << std::endl << "]";
        else
            o << " ]";
        
        return o;
    }
}



#include "Cloner.h"
#include "IsAbstract.h"
#include "IsDerivedFrom.h"

template <class valueType>
RevBayesCore::RbVector<valueType>::RbVector() : RbVectorImpl<valueType, IsAbstract<valueType>::Is  >()
{
    
}


template <class valueType>
RevBayesCore::RbVector<valueType>::RbVector(size_t n) : RbVectorImpl<valueType, IsAbstract<valueType>::Is  >(n)
{
    
}


template <class valueType>
RevBayesCore::RbVector<valueType>::RbVector(size_t n, const valueType &v) : RbVectorImpl<valueType, IsAbstract<valueType>::Is  >(n,v)
{
    
}


template <class valueType>
RevBayesCore::RbVector<valueType>::RbVector( const typename RbVectorImpl<valueType, IsAbstract<valueType>::Is >::vectorType &v ) : RbVectorImpl<valueType, IsAbstract<valueType>::Is  >(v)
{
    
}


template <class valueType>
RevBayesCore::RbVector<valueType>::RbVector( const RbVector<valueType> &v ) : RbVectorImpl<valueType, IsAbstract<valueType>::Is  >(v)
{
    
}


template <class valueType>
RevBayesCore::RbVector<valueType>::~RbVector( void )
{

}


template <class valueType>
RevBayesCore::RbVector<valueType>* RevBayesCore::RbVector<valueType>::clone(void) const {
    
    return new RbVector<valueType>( *this );
}

/**
 * Find and return the index of pivot element.
 * @param a - The array.
 * @param first - The start of the sequence.
 * @param last - The end of the sequence.
 * @return - the pivot element
 */
template <class valueType>
int RevBayesCore::RbVector<valueType>::pivot(int first, int last)
{
    int  p = first;
    const valueType& pivotElement = this->operator[](first);
    
    for(int i = first+1 ; i <= last ; i++)
    {
        /* If you want to sort the list in the other order, change "<=" to ">" */
        if(this->operator[](i) <= pivotElement)
        {
            p++;
            
            valueType *temp = Cloner<valueType, IsDerivedFrom<valueType, Cloneable>::Is >::createClone( this->operator[](i) );
            this->operator[](i) = this->operator[](p);
            this->operator[](p) = *temp;
            
            delete temp;
        }
    }
    
    valueType *temp = Cloner<valueType, IsDerivedFrom<valueType, Cloneable>::Is >::createClone( this->operator[](p) );
    this->operator[](p) = this->operator[](first);
    this->operator[](first) = *temp;
    
    delete temp;
    
    return p;
}


template <class valueType>
void RevBayesCore::RbVector<valueType>::printElement(std::ostream& o, size_t idx, std::string sep, int l, bool left) const
{
    
    const valueType &element = this->operator[](idx);

    const Container *c = dynamic_cast< const Container *>( &element );
    if ( c == NULL )
    {
        o << element;
    }
    else
    {
        for (size_t i=0; i<c->size(); ++i)
        {
            if ( i > 0)
            {
                o << sep;
            }
            c->printElement(o, i, sep, l+1, left);
        }
    }
    
}


template <class valueType>
void RevBayesCore::RbVector<valueType>::quicksort(int first, int last)
{
    
    if(first < last)
    {
        int pivotElement = pivot(first, last);
        quicksort(first, pivotElement-1);
        quicksort(pivotElement+1, last);
    }
}


template <class valueType>
void RevBayesCore::RbVector<valueType>::sort(void)
{
    // just delegate to our internal quicksort method.
    quicksort(0, int(this->size())-1);
}


/**
 * Swap the parameters.
 * @param a - The first parameter.
 * @param b - The second parameter.
 */
template <class valueType>
void RevBayesCore::RbVector<valueType>::swap( valueType& a, valueType& b)
{
    valueType *temp = Cloner<valueType, IsDerivedFrom<valueType, Cloneable>::Is >::createClone( a );
    a = b;
    b = *temp;
    
    delete temp;
}


template <class valueType>
std::ostream& RevBayesCore::operator<<(std::ostream& o, const RevBayesCore::RbVector<valueType>& x) {
    
    size_t lineLength = 75;
    
    o << "[ ";
    size_t curLength = 2;

    for (RbConstIterator<valueType> it = x.begin(); it != x.end(); ++it) 
    {
        if ( it != x.begin() ) 
        {
            o << ", ";
        }
        const valueType& v = *it;
        o << v;
    }
    if ( curLength + 2 > lineLength )
        o << std::endl << "]";
    else
        o << " ]";
    
    return o;
}


#endif

