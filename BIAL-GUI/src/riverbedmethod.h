#ifndef RIVERBEDMETHOD_H
#define RIVERBEDMETHOD_H

#include <src/activeContourMethod.h>

class RiverBedMethod : public ActiveContourMethod {
public:
  enum { Type = ActiveContourMethod::Type + 2 };

  RiverBedMethod( const QVector< size_t > &points, const Bial::Image< int > &grayImg, const Bial::Image< int > &grad );

  int type( ) const;
  void run( const Bial::Vector< size_t > &seeds, const Path &currentPath );
  std::string name( );
};

#endif // RIVERBEDMETHOD_H
