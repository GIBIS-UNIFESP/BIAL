#ifndef LWMETHOD_H
#define LWMETHOD_H

#include "Common.hpp"
#include "Image.hpp"
#include <QColor>
#include <QVector>

typedef Bial::Vector< size_t > Path;

class LWMethod {
public:
  enum {
    Type = -1,
  };
  const QVector< size_t > &m_points;
  const Bial::Image< int > &m_grayImg;
  const Bial::Image< int > &m_grad;
  Bial::Image< int > m_cost;
  Bial::Image< int > m_pred;
  Bial::Vector< Path > m_paths;

  virtual int type( ) const = 0;

  LWMethod( const QVector< size_t > &points,
            const Bial::Image< int > &grayImg,
            const Bial::Image< int > &grad,
            QColor color );

  virtual void run( const Bial::Vector< bool > &seeds ) = 0;

  Path updatePath( size_t pxl );

  const QColor color;
};
#endif // LWMETHOD_H
