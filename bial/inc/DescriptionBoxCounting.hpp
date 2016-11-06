#include "FeatureExtractor.h"

#ifndef BOXCOUNTING_H
#define BOXCOUNTING_H

namespace Bial {

  typedef Vector < Features < double >> BOXCOUNTINGfeature;

  class BOXCOUNTING : public FeatureExtractor< int, double > {

public:
    BOXCOUNTING( FeatureDetector< int > *Fd );

    BOXCOUNTING( Vector < std::tuple < Image< int >, Image< int >> > detected );

    void SetParameters( ParameterInterpreter *interpreter );

    std::string GetParameters( ParameterInterpreter *interpreter );

    BOXCOUNTINGfeature Run( );
  };

}

#endif
