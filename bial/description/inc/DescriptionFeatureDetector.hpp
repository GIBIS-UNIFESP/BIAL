#include "Common.hpp"
#include "Image.hpp"

#ifndef FEATUREDETECTOR_H
#define FEATUREDETECTOR_H

namespace Bial {

  template< class T >
  class FeatureDetector {
protected:
    const Image< T > &img;

public:
    FeatureDetector( const Image< T > &img );
    virtual Vector< std::tuple< Image< T >, Vector< size_t > > > Run( ) = 0;
  };

  template< class T >
  FeatureDetector< T >::FeatureDetector( const Image< T > &img ) : img( img ) {
  }

}

#include "DescriptionNoDetector.hpp"
#endif
