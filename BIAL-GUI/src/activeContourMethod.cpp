#include "DistanceFunction.hpp"
#include "activeContourMethod.h"

#include <DrawLine.hpp>

ActiveContourMethod::ActiveContourMethod( const QVector< size_t > &points,
                                          const Bial::Image< int > &grayImg,
                                          const Bial::Image< int > &grad,
                                          QColor color ) :
  m_points( points ), m_grayImg( grayImg ), m_grad( grad ),
  m_cost( grayImg.Dim( ) ), m_pred( grayImg.Dim( ) ), color( color ) {

}

Path ActiveContourMethod::linePath( Path path ) {
  if( !path.empty( ) ) {
    auto coord1 = m_cost.Coordinates( path.front( ) );
    auto coord2 = m_cost.Coordinates( path.back( ) );
    Bial::Line line( coord1, coord2 );
    auto pixels = line.Pixels( );
    path.clear( );
    for( auto px :pixels ) {
      path.push_back( m_cost.Position( px ) );
    }
  }
  return( path );
}

//Path LWMethod::linePath( Path path ) {
//  double maxDist = 30.;
//  double proportion = 3.;
//  if( !path.empty( ) ) {
//    double dist = df.Distance( &coord1[ 0 ], &coord2[ 0 ], 2 );
//    if( ( dist < maxDist ) && ( dist < ( path.size( ) / proportion ) ) ) {
//      Bial::Line line( coord1, coord2 );
//      auto pixels = line.Pixels( );
//      path.clear( );
//      for( auto px :pixels ) {
//        path.push_back( m_cost.Position( px ) );
//      }
//    }
//  }
// return( path );
//}

Path ActiveContourMethod::updatePath( const size_t end_pxl ) {
  Path path;
  size_t pxl = end_pxl;
  if( !cachedPath.empty( ) ) {
    if( m_pred[ pxl ] == static_cast< int >( cachedPath.front( ) ) ) {
      path = cachedPath;
      path.push_back( pxl );
      cachedPath = path;
      return( path );
    }
  }
  while( ( int ) pxl != m_pred[ pxl ] ) {
    path.push_back( pxl );
    pxl = m_pred[ pxl ];
  }
  cachedPath = path;
  return( path );
}

size_t ActiveContourMethod::lastPt( ) const {
  return( m_lastPt );
}

void ActiveContourMethod::setLastPt( const size_t &lastPt ) {
  m_lastPt = lastPt;
}

size_t ActiveContourMethod::pred( size_t pxl ) const {
  return( m_pred[ pxl ] );
}
