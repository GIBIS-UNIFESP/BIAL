#ifndef LAZYWALKMETHOD_H
#define LAZYWALKMETHOD_H
#include "lwmethod.h"


class LazyWalkMethod : public LWMethod {
public:
  enum { Type = LWMethod::Type + 3 };

  LazyWalkMethod( const QVector< size_t > &points,
                  const Bial::Image< int > &grayImg,
                  const Bial::Image< int > &grad,
                  const Bial::Vector< bool > &seeds );

  int type( );
  void run( );
};


#endif // LAZYWALKMETHOD_H
