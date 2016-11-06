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

}

#include "NoDetector.cpp"

#endif
