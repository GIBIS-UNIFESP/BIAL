#include "Image.hpp"
#include <tuple>

#ifndef FEATUREDETECTOR_H
#define FEATUREDETECTOR_H

namespace Bial {

  template< class T >
  class FeatureDetector {
protected:
    Image< T > img;

public:
    FeatureDetector( Image< T > img );
    virtual Vector < std::tuple < Image< T >, Image< int >> > Run( ) = 0;
  };

  template< class T >
  FeatureDetector< T >::FeatureDetector( Image< T > img ) {
    this->img = img;
  }

}

#include "DescriptionNoDetector.hpp"
#endif
