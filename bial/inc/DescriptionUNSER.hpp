#ifndef UNSER_H
#define UNSER_H

#include "DescriptionFeatureExtractor.hpp"

namespace Bial {

typedef Vector < Features < int >> UNSERfeature;

class UNSER : public FeatureExtractor< int, int > {
private:

  static uchar LinearNormalize( double value, double n );

  static float Mean( Vector< float > s );
  static float Contrast( Vector< float > d );
  static float Correlation( Vector< float > s, float mean, float contrast );
  static float Energy( Vector< float > s, Vector< float > d );
  static float Entropy( Vector< float > s, Vector< float > d );
  static float Homogeneity( Vector< float > d );
  static float MaximalProbability( Vector< float > s );
  static float StandardDeviation( float contrast, float correlation );


public:
  UNSER( FeatureDetector< int > *Fd );

  UNSER( Vector < std::tuple < Image< int >, Image< int >> > detected );

  void SetParameters( ParameterInterpreter *interpreter );

  std::string GetParameters( ParameterInterpreter *interpreter );

  UNSERfeature Run( );
};

}

#endif
