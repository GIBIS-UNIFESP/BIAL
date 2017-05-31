#include "DescriptionFeatureExtractor.hpp"

#ifndef CCV_H
#define CCV_H

#define HIGH true
#define LOW false

namespace Bial {


  typedef Vector< Features < int > > CCVfeature;

  class CCV : public FeatureExtractor< Color, int > {
    enum { NIL = -1, MIN_AREA = 1 };
private:
    size_t dim;

public:
    CCV( FeatureDetector< Color > *Fd );

    CCV( const Vector< std::tuple< Image< Color >, Vector< size_t > > > &detected );

    void SetParameters( ParameterInterpreter *interpreter );

    std::string GetParameters( ParameterInterpreter *interpreter );

    CCVfeature Run( );
  };
}
#endif
