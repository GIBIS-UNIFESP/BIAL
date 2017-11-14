#include "DescriptionFeatureExtractor.hpp"

#ifndef LAS_H
#define LAS_H

namespace Bial {

  typedef Vector< Features < int > > LASfeature;

  class LAS : public FeatureExtractor< int, int > {
private:
    size_t bins;

    static size_t QuantizeValue( int value );

public:
    LAS( FeatureDetector< int > *Fd );

    LAS( const Vector< std::tuple< Image< int >, Vector< size_t > > > &detected );

    void SetParameters( ParameterInterpreter *interpreter );

    std::string GetParameters( ParameterInterpreter *interpreter );

    LASfeature Run( );
  };

}
#endif
