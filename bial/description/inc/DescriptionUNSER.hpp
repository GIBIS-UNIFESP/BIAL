#ifndef UNSER_H
#define UNSER_H

#include "DescriptionFeatureExtractor.hpp"

namespace Bial {

typedef Vector< Features< int > > UNSERfeature;

class UNSER : public FeatureExtractor< int, int > {
private:

  static uchar LinearNormalize( double value, double n );

  static float Mean( const Vector< float > &s );
  static float Contrast( const Vector< float > &d );
  static float Correlation( const Vector< float > &s, float mean, float contrast );
  static float Energy( const Vector< float > &s, const Vector< float > &d );
  static float Entropy( const Vector< float > &s, const Vector< float > &d );
  static float Homogeneity( const Vector< float > &d );
  static float MaximalProbability( const Vector< float > &s );
  static float StandardDeviation( float contrast, float correlation );


public:
  UNSER( FeatureDetector< int > *Fd );

  UNSER( const Vector< std::tuple< Image< int >, Vector< size_t > > > &detected );

  void SetParameters( ParameterInterpreter *interpreter );

  std::string GetParameters( ParameterInterpreter *interpreter );

  UNSERfeature Run( );
};

}

#endif
