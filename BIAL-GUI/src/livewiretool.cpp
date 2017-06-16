#include "Adjacency.hpp"
#include "AdjacencyIterator.hpp"
#include "AdjacencyRound.hpp"

#include "ColorRGB.hpp"
#include "File.hpp"
#include "FileImage.hpp"
#include "Geometrics.hpp"

#include "MultiImage.hpp"
#include "RealColor.hpp"

#include "EdgeLiveWire.hpp"
#include "SumPathFunction.hpp"
#include "guiimage.h"

#include "livewiretool.h"
#include <QDebug>
#include <QMessageBox>
#include <QPointF>
#include <Vector.hpp>
#include <algorithm>


void LiveWireTool::setPredVisibility( bool vis ) {
  predVisible = vis;
  emit guiImage->imageUpdated( );
}

void LiveWireTool::setCostVisibility( bool vis ) {
  costVisible = vis;
  emit guiImage->imageUpdated( );
}

bool LiveWireTool::getPredVisible( ) const {
  return( predVisible );
}

bool LiveWireTool::getCostVisible( ) const {
  return( costVisible );
}

LiveWireTool::LiveWireTool( GuiImage *guiImage, ImageViewer *viewer ) try :
  Tool( guiImage, viewer ), cost( guiImage->getDim( ) ), pred( guiImage->getDim( ) ) {
  COMMENT( "Initiating segmentation tool.", 0 );

  m_scene = viewer->getScene( 0 );

  setObjectName( "LiveWireTool" );
  costVisible = true;
  predVisible = false;
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

  auto point = m_scene->addEllipse( QRectF( x - 3, y - 3, 6, 6 ), QPen( Qt::red, 2 ),
                                    QBrush( Qt::darkRed ) );
  point->setFlag( QGraphicsItem::ItemIsMovable, true );
  m_points.append( point );

  emit guiImage->imageUpdated( );
}

void LiveWireTool::mouseClicked( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {
  timer.start( );

  const Bial::FastTransform &transf = guiImage->getTransform( axis );
  if( m_scene->itemAt( pt, QTransform( ) )->type( ) != QGraphicsEllipseItem::Type ) {
    addPoint( pt );
  }
}

void LiveWireTool::mouseMoved( QPointF pt, size_t axis ) {
  if( timer.elapsed( ) > 30 ) {
    emit guiImage->imageUpdated( );
    timer.start( );
  }
}

void LiveWireTool::mouseReleased( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {
  emit guiImage->imageUpdated( );
  Q_UNUSED( buttons );
  Q_UNUSED( pt );
  const Bial::FastTransform &transf = guiImage->getTransform( axis );
  runLiveWire( axis, transf );
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
  if( !costVisible && !predVisible ) {
    return( QPixmap( ) );
  }
  if( !needUpdate[ axis ] ) {
    return( pixmaps[ axis ] );
  }
  const Bial::FastTransform &transf = guiImage->getTransform( axis );
  QImage res( xsz, ysz, QImage::Format_ARGB32 );
  if( !predVisible ) {
    res.fill( qRgba( 0, 0, 0, 0 ) );
  }
  else {
#pragma omp parallel for firstprivate(axis, xsz, ysz)
    for( size_t y = 0; y < ysz; ++y ) {
      QRgb *scanLine = ( QRgb* ) res.scanLine( y );
      for( size_t x = 0; x < xsz; ++x ) {
        Bial::Point3D pos = transf( x, y, guiImage->currentSlice( axis ) );
        scanLine[ x ] = qRgba( 0, 0, pred( pos.x, pos.y, pos.z ), 128 );
      }
    }
  }
  if( costVisible && ( cost.size( ) == pred.size( ) ) ) {
#pragma omp parallel for firstprivate(axis, xsz, ysz)
    for( size_t y = 0; y < ysz; ++y ) {
      QRgb *scanLine = ( QRgb* ) res.scanLine( y );
      for( size_t x = 0; x < xsz; ++x ) {
        Bial::Point3D pos = transf( x, y, guiImage->currentSlice( axis ) );
        int cst = cost( pos.x, pos.y, pos.z );
        scanLine[ x ] = qRgba( cst, 0, 0, 128 );
      }
    }
  }
  pixmaps[ axis ] = QPixmap::fromImage( res );
  return( pixmaps[ axis ] );
}

void LiveWireTool::runLiveWire( int axis, const Bial::FastTransform &transf ) {
  Bial::Vector< bool > seed( guiImage->getSize( ) );
  cost.Set( 0 );
  pred.Set( 0 );

  seed.Set( false );
  for( QGraphicsEllipseItem *point : m_points ) {
    float px = point->rect( ).center( ).x( ) + point->pos( ).x( );
    float py = point->rect( ).center( ).y( ) + point->pos( ).y( );
    auto tpoint = transf( px, py, ( double ) guiImage->currentSlice( axis ) );
    seed[ cost.Position( tpoint.x, tpoint.y ) ] = true;
  }
  COMMENT( "Running LiveWire.", 0 );
  float weight = 10.;
  switch( guiImage->getImageType( ) ) {
      case Bial::MultiImageType::int_img: {
      std::tie( cost, pred ) = Bial::Edge::LiveWire( guiImage->getIntImage( ), seed, weight );
      break;
    }
      case Bial::MultiImageType::flt_img: {
      std::tie( cost, pred ) = Bial::Edge::LiveWire( guiImage->getFltImage( ), seed, weight );
      break;
    }
      case Bial::MultiImageType::clr_img: {
      Bial::Image< int > img( Bial::ColorSpace::ARGBtoGraybyLuminosity< int >( guiImage->getClrImage( ) ) );
      std::tie( cost, pred ) = Bial::Edge::LiveWire( img, seed, weight );
      break;
    }
      case Bial::MultiImageType::rcl_img: {
      Bial::Image< int > img( Bial::ColorSpace::ARGBtoGraybyLuminosity< int >( guiImage->getRclImage( ) ) );
      std::tie( cost, pred ) = Bial::Edge::LiveWire( img, seed, weight );
      break;
    }
      case Bial::MultiImageType::none:
      std::cout << "MultiImageType::none" << std::endl;
      return;
  }
  COMMENT( "Seting pixels for frendly displaying.", 0 );
  for( size_t pxl = 0; pxl < pred.size( ); ++pxl ) {
    if( pred[ pxl ] < 0 ) {
      pred[ pxl ] = pxl;
    }
  }
  cost.SetRange( 0, 255 );

  needUpdate[ axis ] = true;
  emit guiImage->imageUpdated( );
}
