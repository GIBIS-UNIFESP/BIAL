#include "Image.hpp"
#include "Vector.hpp"

#ifndef NODETECTOR_H
#define NODETECTOR_H

namespace Bial {

  template< class T >
  class NoDetector : public FeatureDetector< T > {
public:
    NoDetector( Image< T > );
    Vector < std::tuple < Image< T >, Image< int >> > Run( );
  };


  template< class T >
  NoDetector< T >::NoDetector( Image< T > img ) : FeatureDetector< T >( img ) {
  }

  template< class T >
  Vector <  std::tuple < Image< T >, Image< int >> > NoDetector< T >::Run( ) {
    Vector <  std::tuple < Image< T >, Image< int >> > v;
     std::tuple < Image< T >, Image < int >> t;
    Image< int > mask( this->img.size( 0 ), this->img.size( 1 ) );
    for( size_t i = 0; i < mask.size( ); ++i ) {
      mask[ i ] = 1;
    }
    t = std::tie( this->img, mask );
    v.push_back( t );

    return( v );
  }

}


#endif
