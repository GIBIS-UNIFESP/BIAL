#include "Image.hpp"
#include "Vector.hpp"

#ifndef NODETECTOR_H
#define NODETECTOR_H

namespace Bial {

  template< class T >
  class NoDetector : public FeatureDetector< T > {
public:
    NoDetector( const Image< T > & );
    Vector< std::tuple < Image< T >, Vector< size_t > > > Run( );
  };


  template< class T >
  NoDetector< T >::NoDetector( const Image< T > &img ) : FeatureDetector< T >( img ) {
  }

  template< class T >
  Vector< std::tuple< Image< T >, Vector< size_t > > > NoDetector< T >::Run( ) {
    Vector< std::tuple< Image< T >, Vector< size_t > > > v;
    std::tuple< Image< T >, Vector< size_t > > t;
    Vector< size_t > mask( this->img.size(  ) );
    for( size_t i = 0; i < mask.size( ); ++i ) {
      mask[ i ] = i;
    }
    t = std::tie( this->img, mask );
    v.push_back( t );

    return( v );
  }

}


#endif
