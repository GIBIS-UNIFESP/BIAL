#include "Adjacency.hpp"
#include "AdjacencyIterator.hpp"
#include "AdjacencyRound.hpp"

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

#include "livewiretool.h"
#include <Geometrics.hpp>
#include <QDebug>
#include <QMessageBox>
#include <QPointF>
#include <Vector.hpp>
#include <algorithm>


void LiveWireTool::setPredVisibility( bool vis ) {
  m_predVisible = vis;
  emit guiImage->imageUpdated( );
}

void LiveWireTool::setCostVisibility( bool vis ) {
  m_costVisible = vis;
  emit guiImage->imageUpdated( );
}

bool LiveWireTool::getPredVisible( ) const {
  return( m_predVisible );
}

bool LiveWireTool::getCostVisible( ) const {
  return( m_costVisible );
}

LiveWireTool::LiveWireTool( GuiImage *guiImage, ImageViewer *viewer ) try :
  Tool( guiImage, viewer ), m_cost( guiImage->getDim( ) ), m_pred( guiImage->getDim( ) ),
  m_seeds( guiImage->getDim( ) ), m_res( guiImage->getDim( ) ),
  m_transf( guiImage->getTransform( 0 ) ) {
  switch( guiImage->getImageType( ) ) {
      case Bial::MultiImageType::int_img: {
      m_grayImg = guiImage->getIntImage( );
      break;
    }
      case Bial::MultiImageType::flt_img: {
      m_grayImg = guiImage->getFltImage( );
      break;
    }
      case Bial::MultiImageType::clr_img: {
      m_grayImg = Bial::ColorSpace::ARGBtoGraybyLuminosity< int >( guiImage->getClrImage( ) );
      break;
    }
      case Bial::MultiImageType::rcl_img: {
      m_grayImg = Bial::ColorSpace::ARGBtoGraybyLuminosity< int >( guiImage->getRclImage( ) );
      break;
    }
      case Bial::MultiImageType::none:
      throw std::runtime_error( BIAL_ERROR( "Unsupported image type." ) );
  }
  m_grad = Bial::Gradient::Morphological( m_grayImg );
  Bial::Intensity::Complement( m_grad );
  COMMENT( "Initiating segmentation tool.", 0 );

  m_scene = viewer->getScene( 0 );

  setObjectName( "LiveWireTool" );
  m_costVisible = true;
  m_predVisible = false;
  setHasLabel( true );
  COMMENT( "Finished constructor for segmentation tool.", 0 );
}
catch( std::bad_alloc &e ) {
  std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
  throw( std::runtime_error( msg ) );
}
catch( std::runtime_error &e ) {
  std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
  throw( std::runtime_error( msg ) );
}
catch( const std::out_of_range &e ) {
  std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
  throw( std::out_of_range( msg ) );
}
catch( const std::logic_error &e ) {
  std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
  throw( std::logic_error( msg ) );
}

LiveWireTool::~LiveWireTool( ) {
  for( QGraphicsEllipseItem *pt : m_points ) {
    m_scene->removeItem( pt );
    delete pt;
  }
}

int LiveWireTool::type( ) {
  return( LiveWireTool::Type );
}

void LiveWireTool::addPoint( QPointF pt ) {
  float x = pt.x( ), y = pt.y( );

  auto point = m_scene->addEllipse( QRectF( x - 3, y - 3, 6, 6 ), QPen( QColor( 0, 255, 0, 128 ), 1 ),
                                    QBrush( QColor( 0, 255, 0, 64 ) ) );
  point->setFlag( QGraphicsItem::ItemIsMovable, true );
/*  point->setFlag( QGraphicsItem::ItemIsSelectable, true ); */
  m_points.append( point );

  emit guiImage->imageUpdated( );
}

void LiveWireTool::mouseClicked( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {
  timer.start( );
  QGraphicsItem *item = m_scene->itemAt( pt, QTransform( ) );
  if( buttons & Qt::LeftButton ) {
    if( ( item == NULL ) || ( ( item->type( ) != QGraphicsEllipseItem::Type ) ) ) {
      addPoint( pt );
    }
  }
  else if( buttons & Qt::RightButton ) {
    if( item && ( item->type( ) == QGraphicsEllipseItem::Type ) ) {
      m_points.removeAll( dynamic_cast< QGraphicsEllipseItem* >( item ) );
      m_scene->removeItem( item );
      delete item;
    }
  }
}


Bial::Point3D LiveWireTool::toPoint3D( QGraphicsEllipseItem *item ) {
  float px = item->rect( ).center( ).x( ) + item->pos( ).x( );
  float py = item->rect( ).center( ).y( ) + item->pos( ).y( );
//  return( transf( px, py, ( double ) guiImage->currentSlice( axis ) ) );
  return( m_transf( px, py, 0 ) );
}

size_t LiveWireTool::toPxIndex( QGraphicsEllipseItem *item ) {
  const Bial::Point3D &point = toPoint3D( item );
  if( m_grayImg.ValidCoordinate( point.x, point.y ) ) {
    return( m_grayImg.Position( point.x, point.y ) );
  }
  else {
    return( m_grayImg.size( ) );
  }
}

Bial::Point3D LiveWireTool::toPoint3D( const QPointF &qpoint ) {
  return( m_transf( qpoint.x( ), qpoint.y( ), 0 ) );
}

size_t LiveWireTool::toPxIndex( const QPointF &qpoint ) {
  const Bial::Point3D &point = toPoint3D( qpoint );
  if( m_grayImg.ValidCoordinate( point.x, point.y ) ) {
    return( m_grayImg.Position( point.x, point.y ) );
  }
  else {
    return( m_grayImg.size( ) );
  }
}

void LiveWireTool::mouseMoved( QPointF pt, size_t axis ) {
  if( timer.elapsed( ) > 30 ) {

    updatePath( pt );

    emit guiImage->imageUpdated( );
    timer.start( );
  }
}

void LiveWireTool::mouseReleased( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {
  emit guiImage->imageUpdated( );
  Q_UNUSED( buttons );
  Q_UNUSED( pt );
  runLiveWire( axis );
}

void LiveWireTool::mouseDragged( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {
  Q_UNUSED( buttons );
  Q_UNUSED( axis );
  Q_UNUSED( pt );
  /* nothing happens */
}

void LiveWireTool::sliceChanged( size_t axis, size_t slice ) {
  Q_UNUSED( slice );
  needUpdate[ axis ] = true;
}

QPixmap LiveWireTool::getLabel( size_t axis ) {
  const size_t xsz = guiImage->width( axis );
  const size_t ysz = guiImage->heigth( axis );
  if( !m_costVisible && !m_predVisible ) {
    return( QPixmap( ) );
  }
  if( !needUpdate[ axis ] ) {
    return( m_pixmaps[ axis ] );
  }
  const Bial::FastTransform &transf = guiImage->getTransform( axis );
  QImage res( xsz, ysz, QImage::Format_ARGB32 );
  res.fill( qRgba( 0, 0, 0, 128 ) );
  if( m_predVisible ) {
#pragma omp parallel for firstprivate(axis, xsz, ysz)
    for( size_t y = 0; y < ysz; ++y ) {
      QRgb *scanLine = ( QRgb* ) res.scanLine( y );
      for( size_t x = 0; x < xsz; ++x ) {
        Bial::Point3D pos = transf( x, y, guiImage->currentSlice( axis ) );
        QRgb color = scanLine[ x ];
        scanLine[ x ] = qRgba( 0, 0, m_pred( pos.x, pos.y, pos.z ), qAlpha( color ) );
      }
    }
  }
  if( m_costVisible && ( m_cost.size( ) == m_pred.size( ) ) ) {
#pragma omp parallel for firstprivate(axis, xsz, ysz)
    for( size_t y = 0; y < ysz; ++y ) {
      QRgb *scanLine = ( QRgb* ) res.scanLine( y );
      for( size_t x = 0; x < xsz; ++x ) {
        Bial::Point3D pos = transf( x, y, guiImage->currentSlice( axis ) );
        int cst = m_cost( pos.x, pos.y, pos.z );
        QRgb color = scanLine[ x ];
        scanLine[ x ] = qRgba( cst, qGreen( color ), qBlue( color ), qAlpha( color ) );
      }
    }
  }
  for( size_t y = 0; y < ysz; ++y ) {
    QRgb *scanLine = ( QRgb* ) res.scanLine( y );
    for( size_t x = 0; x < xsz; ++x ) {
      Bial::Point3D pos = transf( x, y, guiImage->currentSlice( axis ) );
      if( m_res( pos.x, pos.y, pos.z ) > 0 ) {
        scanLine[ x ] = qRgba( 0, 255, 0, 255 );
      }
      if( m_seeds( pos.x, pos.y, pos.z ) ) {
        scanLine[ x ] = qRgba( 255, 255, 255, 255 );
      }
    }
  }
  m_pixmaps[ axis ] = QPixmap::fromImage( res );
  return( m_pixmaps[ axis ] );
}


///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
//////////                                             ///////////
//////////              L I V E W I R E                ///////////
//////////                                             ///////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

void LiveWireTool::runLiveWire( int axis ) {
  m_cost.Set( 0 );
  m_pred.Set( 0 );
  m_seeds.Set( false );
  for( QGraphicsEllipseItem *item : m_points ) {
    const Bial::Point3D &point = toPoint3D( item );
    if( m_pred.ValidCoordinate( point.x, point.y, point.z ) ) {
      m_seeds[ m_cost.Position( point.x, point.y ) ] = true;
    }
  }
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

  needUpdate[ axis ] = true;
  emit guiImage->imageUpdated( );
}

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
//////////                                             ///////////
//////////                 D   S   P                   ///////////
//////////                                             ///////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////


void LiveWireTool::updatePath( QPointF pt ) {
  // ############################################################ //
  // DSP algorithm - Live wire on the fly article - A. X. Falcao  //
  // ############################################################ //
  if( m_points.isEmpty( ) ) {
    return;
  }
  size_t v_start = toPxIndex( m_points.last( ) );
  size_t v_end = toPxIndex( pt );
  m_res.Set( 0 );
  m_res[ v_end ] = 1;
  if( ( v_start >= m_grayImg.size( ) ) || ( v_end >= m_grayImg.size( ) ) ) {
    return;
  }
  std::vector< std::deque< size_t > > queue;

  QList< size_t > proc_points;
//  while( proc_points.last( ) != v_start ) {

//  }
}
