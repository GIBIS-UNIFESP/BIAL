#include "Adjacency.hpp"
#include "AdjacencyIterator.hpp"
#include "AdjacencyRound.hpp"

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
#include "IntensityGlobals.hpp"

#include "lazywalkmethod.h"
#include "livewiremethod.h"
#include "livewiretool.h"
#include "riverbedmethod.h"

#include <Geometrics.hpp>
#include <QDebug>
#include <QMessageBox>
#include <QPointF>
#include <Vector.hpp>
#include <algorithm>


void LiveWireTool::setGradVisibility( bool vis ) {
  m_gradVisible = vis;
  emit guiImage->imageUpdated( );
}

void LiveWireTool::setCostVisibility( bool vis ) {
  m_costVisible = vis;
  emit guiImage->imageUpdated( );
}

bool LiveWireTool::getGradVisible( ) const {
  return( m_gradVisible );
}

bool LiveWireTool::getCostVisible( ) const {
  return( m_costVisible );
}

LiveWireTool::LiveWireTool( GuiImage *guiImage, ImageViewer *viewer ) try :
  Tool( guiImage, viewer ), m_seeds( guiImage->getSize( ) ),
  m_cache( guiImage->width( 0 ), guiImage->heigth( 0 ), QImage::Format_ARGB32 ),
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

  m_methods = {
    std::make_shared< LiveWireMethod >( m_pointIdxs, m_grayImg, m_grad ),
    std::make_shared< RiverBedMethod >( m_pointIdxs, m_grayImg, m_grad ),
    std::make_shared< LazyWalkMethod >( m_pointIdxs, m_grayImg, m_grad )
  };
  m_currentMethod = LiveWireMethod::Type;

  setObjectName( "LiveWireTool" );
  m_costVisible = true;
  m_gradVisible = false;
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
  for( auto pt : m_points ) {
    m_scene->removeItem( pt );
    delete pt;
  }
}

int LiveWireTool::type( ) {
  return( LiveWireTool::Type );
}

void LiveWireTool::updatePointIdxs( ) {
  m_pointIdxs = QVector< size_t >( m_points.size( ) );
  for( int p = 0; p < m_points.size( ); ++p ) {
    m_pointIdxs[ p ] = toPxIndex( m_points[ p ] );
  }
}

void LiveWireTool::addPoint( QPointF pt ) {
  float x = pt.x( ), y = pt.y( );
  if( ( pt.x( ) < 0 ) || ( pt.y( ) < 0 ) || ( pt.x( ) > guiImage->width( 0 ) ) ||
      ( pt.y( ) > guiImage->heigth( 0 ) ) ) {
    return;
  }
  auto point = m_scene->addEllipse( QRectF( x - 3, y - 3, 6, 6 ), QPen( QColor( 0, 255, 0, 128 ), 1 ),
                                    QBrush( QColor( 0, 255, 0, 64 ) ) );
//  point->setFlag( QGraphicsItem::ItemIsMovable, true );
/*  point->setFlag( QGraphicsItem::ItemIsSelectable, true ); */
  m_points.append( point );
  updatePointIdxs( );
  m_cache.fill( Qt::transparent );
  for( auto method : m_methods ) {
    method->updateCache( );
  }
  //TODO : dfsjoiasjdfioasjdoisfjioajdsfoijasiofjdoisajiofdj PAÇOCA
  emit guiImage->imageUpdated( );
}

void LiveWireTool::mouseClicked( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {
  timer.start( );
  QGraphicsItem *item = m_scene->itemAt( pt, QTransform( ) );
  if( buttons & Qt::LeftButton ) {
    if( ( item == NULL ) || ( ( item->type( ) != QGraphicsEllipseItem::Type ) ) ) {
      addPoint( pt );
    }
    m_drawing = true;
  }
  else if( buttons & Qt::RightButton ) {
    m_currentMethod = ( m_currentMethod + 1 ) % m_methods.size( );
    updatePath( pt );
  }
  else if( buttons & Qt::MidButton ) {
    m_drawing = false;
  }
  emit guiImage->imageUpdated( );
}


Bial::Point3D LiveWireTool::toPoint3D( QGraphicsEllipseItem *item ) {
  float px = item->rect( ).center( ).x( ) + item->pos( ).x( );
  float py = item->rect( ).center( ).y( ) + item->pos( ).y( );
/*  return( transf( px, py, ( double ) guiImage->currentSlice( axis ) ) ); */
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

void LiveWireTool::updatePath( QPointF pt ) {
  m_res = m_cache;
  for( auto method : m_methods ) {
    if( method->type( ) != m_currentMethod ) {
      for( size_t pxl : method->updatePath( toPxIndex( pt ) ) ) {
        auto coords = m_grayImg.Coordinates( pxl );
        QColor clr = method->color;
        clr.setAlpha( 100 );
        m_res.setPixelColor( coords[ 0 ], coords[ 1 ], clr );
      }
    }
  }
  auto current_method = m_methods[ m_currentMethod ];
  QColor clr = Qt::red;
  for( size_t pxl : current_method->updatePath( toPxIndex( pt ) ) ) {
    auto coords = m_grayImg.Coordinates( pxl );
    m_res.setPixelColor( coords[ 0 ], coords[ 1 ], clr );
  }
}

void LiveWireTool::mouseMoved( QPointF pt, size_t axis ) {
  if( timer.elapsed( ) > 30 ) {
    if( m_drawing ) {
      updatePath( pt );
    }
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
  if( !needUpdate[ axis ] ) {
    return( m_pixmaps[ axis ] );
  }
  m_pixmaps[ axis ] = QPixmap::fromImage( m_res );
  return( m_pixmaps[ axis ] );
}

/*
 * /////////////////////////////////////////////////////////////////
 * /////////////////////////////////////////////////////////////////
 * ////////                                             ///////////
 * ////////              L I V E W I R E                ///////////
 * ////////                                             ///////////
 * /////////////////////////////////////////////////////////////////
 * /////////////////////////////////////////////////////////////////
 */

void LiveWireTool::runLiveWire( int axis ) {
  if( m_points.size( ) > 0 ) {
    m_seeds.Set( false );
    m_seeds[ m_pointIdxs.last( ) ] = true;
//    for( size_t pt : m_pointIdxs ) {
//      if( pt < m_seeds.size( ) ) {
//        m_seeds[ pt ] = true;
//      }
//    }
    for( auto method : m_methods ) {
      method->run( m_seeds );
    }
    needUpdate[ axis ] = true;

    emit guiImage->imageUpdated( );
  }
}
