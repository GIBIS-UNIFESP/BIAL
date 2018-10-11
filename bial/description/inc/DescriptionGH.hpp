#include "DescriptionFeatureExtractor.hpp"

#ifndef DESCRIPTIONGH_H
#define DESCRIPTIONGH_H

namespace Bial {

  typedef Vector< Features < int > > GHfeature;

  class GH : public FeatureExtractor< int, int > {
private:
    size_t dim;

public:
    GH( FeatureDetector< int > *Fd );

    GH( const Vector< std::tuple< Image< int >, Vector< size_t > > > &detected );

    void SetParameters( ParameterInterpreter *interpreter );

    std::string GetParameters( ParameterInterpreter *interpreter );

    GHfeature Run( );
  };
}

#endif // DESCRIPTIONGH_H

