#include "Adjacency.hpp"
#include "AdjacencyIterator.hpp"
#include "AdjacencyRound.hpp"
#include "riverbedmethod.h"

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

#include "riverbedmethod.h"

#include <FastIncreasingFifoBucketQueue.hpp>
#include <Geometrics.hpp>
#include <LocalMaxPathFunction.hpp>
#include <QDebug>
#include <QMessageBox>
#include <QPointF>
#include <Vector.hpp>
#include <algorithm>
RiverBedMethod::RiverBedMethod( const QVector< size_t > &points, const Bial::Image< int > &grayImg,
                                const Bial::Image< int > &grad ) :
  LWMethod( points, grayImg, grad, QColor( 0, 0, 255 ) ) {

}

int RiverBedMethod::type( ) const {
  return( RiverBedMethod::Type );
}

void RiverBedMethod::run( const Bial::Vector< bool > &seeds ) {
  m_cost.Set( 0 );
  m_pred.Set( 0 );

  COMMENT( "Running RiverBed.", 0 );
  m_cost = m_grad;
  m_pred.Set( 0 );
  size_t size = m_cost.size( ); Bial::Image< int > handicap( m_cost );
  Bial::LocalMaxPathFunction< Bial::Image, int > pf( m_cost, nullptr, &m_pred, false );
  COMMENT( "Computing IFT.", 0 );
  COMMENT( "Weight parameter is the control of the lazy-runner. Set it to a value lower than 1.0 for Live-Wire "
           "similarity, or to a value higher than 1.0 for River Bed similarity.", 1 );
  Bial::Adjacency adj( Bial::AdjacencyType::HyperSpheric( 1.1, m_grayImg.Dims( ) ) );
  Bial::FastIncreasingFifoBucketQueue queue( size, 0, m_grad.Maximum( ) + 1 );
  Bial::ImageIFT< int > ift( m_cost, adj, &pf, &queue );
  ift.InsertSeeds( seeds );
  ift.Run( );

  COMMENT( "Seting pixels for frendly displaying.", 0 );
  for( size_t pxl = 0; pxl < m_pred.size( ); ++pxl ) {
    if( m_pred[ pxl ] < 0 ) {
      m_pred[ pxl ] = pxl;
    }
  }
  m_cost.SetRange( 0, 255 );
}
