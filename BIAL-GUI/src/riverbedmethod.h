#ifndef RIVERBEDMETHOD_H
#define RIVERBEDMETHOD_H

#include <src/lwmethod.h>

class RiverBedMethod : public LWMethod {
public:
  enum { Type = LWMethod::Type + 2 };

  RiverBedMethod( const QVector< size_t > &points, const Bial::Image< int > &grayImg, const Bial::Image< int > &grad );

  int type( );
  void run( const Bial::Vector< bool > &seeds );
};

#endif // RIVERBEDMETHOD_H
