#ifndef LAZYWALKMETHOD_H
#define LAZYWALKMETHOD_H
#include "activeContourMethod.h"


class LazyWalkMethod : public ActiveContourMethod {
public:
  enum { Type = ActiveContourMethod::Type + 3 };

  LazyWalkMethod( const QVector< size_t > &points, const Bial::Image< int > &grayImg, const Bial::Image< int > &grad );

  int type( ) const;
  void run( const Bial::Vector< size_t > &seeds, const Path &currentPath );
  std::string name( );
};


#endif // LAZYWALKMETHOD_H
