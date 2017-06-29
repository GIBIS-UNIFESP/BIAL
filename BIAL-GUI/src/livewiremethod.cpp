#include "Adjacency.hpp"
#include "AdjacencyIterator.hpp"
#include "AdjacencyRound.hpp"
#include "livewiremethod.h"

#include "ColorRGB.hpp"
#include "File.hpp"
#include "FileImage.hpp"
#include "Geometrics.hpp"

#include "MultiImage.hpp"
#include "RealColor.hpp"

#include "SumPathFunction.hpp"
#include "guiimage.h"

#include "AdjacencyRound.hpp"
#include "GradientMorphological.hpp"
#include "GrowingBucketQueue.hpp"
#include "Image.hpp"
#include "ImageIFT.hpp"
#include "IntensityGlobals.hpp"
#include "MaxSumPathFunction.hpp"

#include "livewiremethod.h"

#include <Geometrics.hpp>
#include <QDebug>
#include <QMessageBox>
#include <QPointF>
#include <Vector.hpp>
#include <algorithm>
LiveWireMethod::LiveWireMethod( const QVector< size_t > &points, const Bial::Image< int > &grayImg,
                                const Bial::Image< int > &grad, const Bial::Vector< bool > &seeds ) :
  LWMethod( points, grayImg, grad, seeds ) {

}

int LiveWireMethod::type( ) {
  return( LiveWireMethod::Type );
}

void LiveWireMethod::run( ) {
  m_cost.Set( 0 );
  m_pred.Set( 0 );

  COMMENT( "Running LiveWire.", 0 );
  float weight = 1;

  m_cost = m_grad;
  m_pred.Set( 0 );
  size_t size = m_cost.size( );
  if( weight > 1. ) {
    Bial::Image< double > grad_dbl( m_cost );
    double min = m_cost.Minimum( );
    double max = m_cost.Maximum( );
    COMMENT( "Computing new intensities based on beta.", 0 );
    for( size_t elm = 0; elm < size; ++elm ) {
      grad_dbl[ elm ] = std::pow( grad_dbl[ elm ], weight );
    }
    COMMENT( "Normalizing data into the input range.", 0 );
    grad_dbl.SetRange( min, max );
    m_cost = grad_dbl;
  }
  int delta = 1;
  Bial::Image< int > handicap( m_cost );
  m_cost += delta;
  Bial::MaxSumPathFunction< Bial::Image, int > pf( m_cost, nullptr, &m_pred, false, m_grayImg, handicap, 0.0,
                                                   delta );
  COMMENT( "Computing IFT.", 0 );
  COMMENT( "Weight parameter is the control of the lazy-runner. Set it to a value lower than 1.0 for Live-Wire "
           "similarity, or to a value higher than 1.0 for River Bed similarity.", 1 );
  Bial::Adjacency adj( Bial::AdjacencyType::HyperSpheric( 1.9, m_grayImg.Dims( ) ) );
  Bial::GrowingBucketQueue queue( size, delta, true, true );
  for( size_t elm = 0; elm < size; ++elm ) {
    if( m_seeds[ elm ] ) {
      queue.Insert( elm, m_cost[ elm ] );
    }
    else {
      m_cost( elm ) = std::numeric_limits< int >::max( );
    }
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
