#ifndef LAZYWALKMETHOD_H
#define LAZYWALKMETHOD_H
#include "lwmethod.h"


class LazyWalkMethod : public LWMethod {
public:
  enum { Type = LWMethod::Type + 3 };

  LazyWalkMethod( const QVector< size_t > &points, const Bial::Image< int > &grayImg, const Bial::Image< int > &grad );

  int type( ) const;
  void run( const Bial::Vector< bool > &seeds );
};


#endif // LAZYWALKMETHOD_H
