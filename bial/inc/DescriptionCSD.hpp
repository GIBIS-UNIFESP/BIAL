#include "FeatureExtractor.h"

#ifndef CSD_H
#define CDS_H

namespace Bial {

  typedef Vector < Features < int >> CSDfeature;

  class CSD : public FeatureExtractor< Color, int > {
public:
    CSD( FeatureDetector< Color > *Fd );

    CSD( Vector < std::tuple < Image< Color >, Image< int >> > detected );

    void SetParameters( ParameterInterpreter *interpreter );

    std::string GetParameters( ParameterInterpreter *interpreter );

    CSDfeature Run( );
  };
}

#endif
