#include "Adjacency.hpp"
#include "AdjacencyIterator.hpp"
#include "AdjacencyRound.hpp"
#include "lazywalkmethod.h"

#include "ColorRGB.hpp"
#include "File.hpp"
#include "FileImage.hpp"
#include "Geometrics.hpp"

#include "MultiImage.hpp"
#include "RealColor.hpp"

#include "guiimage.h"

#include "AdjacencyRound.hpp"
#include "GradientMorphological.hpp"
#include "GrowingBucketQueue.hpp"
#include "Image.hpp"
#include "ImageIFT.hpp"
#include "IntensityGlobals.hpp"

#include "lazywalkmethod.h"

#include <FastIncreasingFifoBucketQueue.hpp>
#include <Geometrics.hpp>
#include <LocalMaxPathFunction.hpp>
#include <MaxPathFunction.hpp>
#include <QDebug>
#include <QMessageBox>
#include <QPointF>
#include <Vector.hpp>
#include <algorithm>
LazyWalkMethod::LazyWalkMethod( const QVector< size_t > &points, const Bial::Image< int > &grayImg,
                                const Bial::Image< int > &grad ) :
  ActiveContourMethod( points, grayImg, grad, QColor( 0, 255, 255 ) ) {

}

int LazyWalkMethod::type( ) const {
  return( LazyWalkMethod::Type );
}

void LazyWalkMethod::run( const Bial::Vector< size_t > &seeds, const Path &currentPath ) {
  setLastPt( seeds.back( ) );
  m_cost.Set( 0 );
  m_pred.Set( 0 );

  COMMENT( "Running LazyWalk.", 0 );
  m_cost = m_grad;
  m_pred.Set( 0 );
  size_t size = m_cost.size( );

  int delta = 1;
  Bial::Image< int > handicap( m_cost );
  m_cost += delta;
  Bial::MaxPathFunction< Bial::Image, int > pf( m_cost, nullptr, &m_pred, false, handicap );
  COMMENT( "Computing IFT.", 0 );
  COMMENT( "Weight parameter is the control of the lazy-runner. Set it to a value lower than 1.0 for Live-Wire "
           "similarity, or to a value higher than 1.0 for River Bed similarity.", 1 );
  Bial::Adjacency adj( Bial::AdjacencyType::HyperSpheric( 1.9, m_grayImg.Dims( ) ) );
  //Bial::FastIncreasingFifoBucketQueue queue( size, 0, m_grad.Maximum( ) + 1 );
  Bial::GrowingBucketQueue queue( size, 1, true, true );
  for( size_t seed : seeds ) {
    queue.Insert( seed, m_cost[ seed ] );
  }
  for( size_t elm = 0; elm < size; ++elm ) {
    m_cost( elm ) = std::numeric_limits< int >::max( );
  }
  for( size_t pxl : currentPath ) {
    m_cost[ pxl ] = std::numeric_limits< int >::max( );
  }
  Bial::ImageIFT< int > ift( m_cost, adj, &pf, &queue );
  ift.Run( );

  COMMENT( "Seting pixels for frendly displaying.", 0 );
  for( size_t pxl = 0; pxl < m_pred.size( ); ++pxl ) {
    if( m_pred[ pxl ] < 0 ) {
      m_pred[ pxl ] = pxl;
    }
  }
  m_cost.SetRange( 0, 255 );
}

std::string LazyWalkMethod::name( ) {
  return( "LazyWalk" );
}
