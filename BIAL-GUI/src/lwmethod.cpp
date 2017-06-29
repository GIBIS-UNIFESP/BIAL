#include "lwmethod.h"

LWMethod::LWMethod( const QVector< size_t > &points,
                    const Bial::Image< int > &grayImg,
                    const Bial::Image< int > &grad,
                    QColor color ) :
  m_points( points ), m_grayImg( grayImg ), m_grad( grad ),
  m_cost( grayImg.Dim( ) ), m_pred( grayImg.Dim( ) ), color( color ) {

}

Path LWMethod::updatePath( size_t pxl ) {
  Path path;
  while( ( int ) pxl != m_pred[ pxl ] ) {
    path.push_back( pxl );
    pxl = m_pred[ pxl ];
  }
  return( path );
}
