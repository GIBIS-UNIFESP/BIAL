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
  Tool( guiImage, viewer ), m_seeds( guiImage->getSize( ) ), m_res( guiImage->getDim( ) ),
  m_cache( guiImage->getDim( ) ), m_transf( guiImage->getTransform( 0 ) ) {
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
    std::make_shared< LiveWireMethod >( m_pointIdxs, m_grayImg, m_grad, m_seeds ),
    std::make_shared< RiverBedMethod >( m_pointIdxs, m_grayImg, m_grad, m_seeds ),
    std::make_shared< LazyWalkMethod >( m_pointIdxs, m_grayImg, m_grad, m_seeds )
  };

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
  for( QGraphicsEllipseItem *pt : m_points ) {
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

  m_cache.Set( 0 );
  for( auto method : m_methods ) {
    method->updateCache( m_res );
  }
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
/*
 *  if( !m_costVisible && !m_predVisible ) {
 *    return( QPixmap( ) );
 *  }
 */
  if( !needUpdate[ axis ] ) {
    return( m_pixmaps[ axis ] );
  }
  const Bial::FastTransform &transf = guiImage->getTransform( axis );
  QImage res( xsz, ysz, QImage::Format_ARGB32 );
  res.fill( qRgba( 0, 0, 0, 128 ) );
  if( m_gradVisible ) {
#pragma omp parallel for firstprivate(axis, xsz, ysz)
    for( size_t y = 0; y < ysz; ++y ) {
      QRgb *scanLine = ( QRgb* ) res.scanLine( y );
      for( size_t x = 0; x < xsz; ++x ) {
        Bial::Point3D pos = transf( x, y, guiImage->currentSlice( axis ) );
        QRgb color = scanLine[ x ];
        scanLine[ x ] = qRgba( 0, m_grad( pos.x, pos.y, pos.z ), 0, qAlpha( color ) );
      }
    }
  }
//  if( m_costVisible && ( m_cost.size( ) == m_pred.size( ) ) ) {
//#pragma omp parallel for firstprivate(axis, xsz, ysz)
//    for( size_t y = 0; y < ysz; ++y ) {
//      QRgb *scanLine = ( QRgb* ) res.scanLine( y );
//      for( size_t x = 0; x < xsz; ++x ) {
//        Bial::Point3D pos = transf( x, y, guiImage->currentSlice( axis ) );
//        int cst = m_cost( pos.x, pos.y, pos.z );
//        QRgb color = scanLine[ x ];
//        scanLine[ x ] = qRgba( cst, qGreen( color ), qBlue( color ), qAlpha( color ) );
//      }
//    }
//  }
  for( size_t y = 0; y < ysz; ++y ) {
    QRgb *scanLine = ( QRgb* ) res.scanLine( y );
    for( size_t x = 0; x < xsz; ++x ) {
      Bial::Point3D pos = transf( x, y, guiImage->currentSlice( axis ) );
      switch( m_res( pos.x, pos.y, pos.z ) ) {
          case LiveWireMethod::Type:
          scanLine[ x ] = qRgba( 0, 255, 0, 255 );
          break;
          case RiverBedMethod::Type:
          scanLine[ x ] = qRgba( 0, 0, 255, 255 );
          break;
          case LazyWalkMethod::Type:
          scanLine[ x ] = qRgba( 0, 255, 255, 255 );
          break;
          default:
          break;
      }
      if( m_seeds[ m_res.Position( pos.x, pos.y, pos.z ) ] ) {
        scanLine[ x ] = qRgba( 255, 255, 255, 255 );
      }
    }
  }
  m_pixmaps[ axis ] = QPixmap::fromImage( res );
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
  m_seeds.Set( false );
  for( size_t pt : m_pointIdxs ) {
    if( pt < m_seeds.size( ) ) {
      m_seeds[ pt ] = true;
    }
  }
  for( auto method : m_methods ) {
    method->run( );
  }
  needUpdate[ axis ] = true;
  emit guiImage->imageUpdated( );
}


void LiveWireTool::updatePath( QPointF pt ) {
  if( m_points.isEmpty( ) ) {
    return;
  }
  m_res = m_cache;
  for( auto method : m_methods ) {
    method->updatePath( m_res, toPxIndex( pt ) );
  }
}
