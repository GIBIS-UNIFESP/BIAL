#ifndef LIVEWIREMETHOD_H
#define LIVEWIREMETHOD_H
#include "activeContourMethod.h"

class LiveWireMethod : public ActiveContourMethod {
public:
  enum { Type = ActiveContourMethod::Type + 1 };

  LiveWireMethod( const QVector< size_t > &points, const Bial::Image< int > &grayImg, const Bial::Image< int > &grad );

  int type( ) const;
  void run( const Bial::Vector< size_t > &seeds, const Path &currentPath );

  std::string name( );
};

#endif // LIVEWIREMETHOD_H
