#include "lwmethod.h"

LWMethod::LWMethod( const QVector< size_t > &points,
                    const Bial::Image< int > &grayImg,
                    const Bial::Image< int > &grad,
                    const Bial::Vector< bool > &m_seeds ) :
  m_points( points ), m_grayImg( grayImg ), m_grad( grad ), m_seeds( m_seeds ),
  m_cost( grayImg.Dim( ) ), m_pred( grayImg.Dim( ) ) {

}

void LWMethod::updateCache( Bial::Image< int > &img ) {
  const int _type = type( );
  for( int p = 0; p < m_points.size( ); ++p ) {
    size_t v_end = m_points[ ( p + 1 ) % m_points.size( ) ];
    size_t pxl = m_pred[ v_end ];
    while( ( int ) pxl != m_pred[ pxl ] ) {
      img[ pxl ] = _type;
      pxl = m_pred[ pxl ];
    }
  }
}

void LWMethod::updatePath( Bial::Image< int > &img, size_t v_end ) {
  size_t pxl = v_end;
  const int _type = type( );
  while( ( int ) pxl != m_pred[ pxl ] ) {
    img[ pxl ] = _type;
    pxl = m_pred[ pxl ];
  }
}
