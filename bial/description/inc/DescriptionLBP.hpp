#include "DescriptionFeatureExtractor.hpp"

#ifndef LBP_H
#define LBP_H

namespace Bial {

  typedef Vector< Features < int > > LBPfeature;

  class LBP : public FeatureExtractor< int, int > {
private:
    static uchar Uniformity( uchar s[ 8 ] );

    static uchar Signal( int x );

public:
    LBP( FeatureDetector< int > *Fd );

    LBP( const Vector< std::tuple< Image< int >, Vector< size_t > > > &detected );

    void SetParameters( ParameterInterpreter *interpreter );

    std::string GetParameters( ParameterInterpreter *interpreter );

    LBPfeature Run( );
  };

}
#endif
