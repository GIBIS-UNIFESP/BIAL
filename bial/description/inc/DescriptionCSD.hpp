#include "DescriptionFeatureExtractor.hpp"

#ifndef CSD_H
#define CDS_H

namespace Bial {

  typedef Vector< Features < int > > CSDfeature;

  class CSD : public FeatureExtractor< Color, int > {
public:
    CSD( FeatureDetector< Color > *Fd );

    CSD( const Vector< std::tuple< Image< Color >, Vector< size_t > > > &detected );

    void SetParameters( ParameterInterpreter *interpreter );

    std::string GetParameters( ParameterInterpreter *interpreter );

    CSDfeature Run( );
  };
}

#endif
