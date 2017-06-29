#ifndef LWMETHOD_H
#define LWMETHOD_H

#include "Common.hpp"
#include "Image.hpp"
#include <QVector>

class LWMethod {
public:
  enum {
    Type = 1,
    UserType = 65536
  };
  const QVector< size_t > &m_points;
  const Bial::Image< int > &m_grayImg;
  const Bial::Image< int > &m_grad;
  const Bial::Vector< bool > &m_seeds;
  Bial::Image< int > m_cost;
  Bial::Image< int > m_pred;

  virtual int type( ) = 0;

  LWMethod( const QVector< size_t > &points,
            const Bial::Image< int > &grayImg,
            const Bial::Image< int > &grad,
            const Bial::Vector< bool > &m_seeds );

  virtual void run( ) = 0;

  void updateCache( Bial::Image< int > &img );

  void updatePath( Bial::Image< int > &img, size_t v_end );
};
#endif // LWMETHOD_H
