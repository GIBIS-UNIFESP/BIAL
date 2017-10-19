#ifndef LINEPATHMETHOD_H
#define LINEPATHMETHOD_H

#include "activeContourMethod.h"



class LinePathMethod : public ActiveContourMethod {
public:
  enum { Type = ActiveContourMethod::Type + 4 };
  LinePathMethod( const QVector< size_t > &points, const Bial::Image< int > &grayImg, const Bial::Image< int > &grad );

  // LWMethod interface
public:
  int type( ) const;
  void run( const Bial::Vector< size_t > &seeds, const Path & );
  Path updatePath( size_t pxl );
  Path linePath( Path path );
  std::string name( );
};

#endif // LINEPATHMETHOD_H
