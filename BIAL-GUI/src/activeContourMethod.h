#ifndef LWMETHOD_H
#define LWMETHOD_H

#include "Common.hpp"
#include "Image.hpp"
#include <EuclideanDistanceFunction.hpp>
#include <QColor>
#include <QVector>

typedef Bial::Vector< size_t > Path;

class ActiveContourMethod {
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
  size_t m_lastPt;
  Path cachedPath;
  Bial::EuclideanDistanceFunction df;

  size_t pred( size_t pxl ) const;

  virtual int type( ) const = 0;

  ActiveContourMethod( const QVector< size_t > &points,
                       const Bial::Image< int > &grayImg,
                       const Bial::Image< int > &grad,
                       QColor color );

  virtual void run( const Bial::Vector< size_t > &seeds, const Path &currentPath ) = 0;

  virtual Path updatePath( size_t pxl );
  virtual Path linePath( Path path );


  virtual std::string name( ) = 0;

  const QColor color;
  size_t lastPt( ) const;
  void setLastPt( const size_t &lastPt );
};


#endif // LWMETHOD_H
