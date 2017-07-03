#include "linepathmethod.h"

#include <DrawLine.hpp>

LinePathMethod::LinePathMethod( const QVector< size_t > &points, const Bial::Image< int > &grayImg,
                                const Bial::Image< int > &grad ) :
  ActiveContourMethod( points, grayImg, grad, QColor( 255, 255, 0 ) ) {

}

int LinePathMethod::type( ) const {
  return( Type );
}

void LinePathMethod::run( const Bial::Vector< size_t > &seeds, const Path & ) {
  setLastPt( seeds.back( ) );
}

Path LinePathMethod::updatePath( size_t pxl ) {
  Path path;
  auto coord1 = m_cost.Coordinates( lastPt( ) );
  auto coord2 = m_cost.Coordinates( pxl );

  Bial::Line line( coord1, coord2 );
  auto pixels = line.Pixels( );
  for( auto px :pixels ) {
    path.push_back( m_cost.Position( px ) );
  }
  return( path );
}

Path LinePathMethod::linePath( Path path ) {

  return( path );
}

std::string LinePathMethod::name( ) {
  return( "LinePathMethod" );
}
