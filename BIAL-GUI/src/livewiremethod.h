#ifndef LIVEWIREMETHOD_H
#define LIVEWIREMETHOD_H
#include "lwmethod.h"

class LiveWireMethod : public LWMethod {
public:
  enum { Type = LWMethod::Type + 1 };

  LiveWireMethod(const QVector< size_t > &points,
                  const Bial::Image< int > &grayImg,
                  const Bial::Image< int > &grad);

  int type( );
  void run(const Bial::Vector<bool> &seeds);
};

#endif // LIVEWIREMETHOD_H
