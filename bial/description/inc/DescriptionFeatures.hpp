#include "Vector.hpp"

#ifndef FEATURES_H
#define FEATURES_H

namespace Bial {

  template< class T >
  class Features {
private:
    Vector< T > item;

public:
    Features( );
    Features( size_t n );
    T &operator[]( size_t i );
    const T &operator[]( size_t i ) const;
    size_t size( ) const;
    void clear( );
    void push_back( T i );
    void resize( size_t n );
    T* data( );
  };
}

namespace Bial {

  template< class T >
  Features< T >::Features( ) {
  }

  template< class T >
  Features< T >::Features( size_t n ) {
    this->item.clear( ); 
    this->item.resize( n );
  }

  template< class T >
  T &Features< T >::operator[]( size_t i ) {
    return( item[ i ] );
  }

  template< class T >
  const T &Features< T >::operator[]( size_t i ) const {
    return( item[ i ] );
  }

  template< class T >
  size_t Features< T >::size( ) const {
    return( item.size( ) );
  }

  template< class T >
  void Features< T >::clear( ) {
    item.clear( );
  }

  template< class T >
  void Features< T >::push_back( T i ) {
    item.push_back( i );
  }

  template< class T >
  void Features< T >::resize( size_t n ) {
    item.resize( n );
  }

  template< class T >
  T*Features< T >::data( ) {
    return( item.data( ) );
  }

}


#endif
