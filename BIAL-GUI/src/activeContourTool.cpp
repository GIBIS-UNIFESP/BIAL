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

#include "activeContourTool.h"
#include "lazywalkmethod.h"
#include "linepathmethod.h"
#include "livewiremethod.h"
#include "riverbedmethod.h"

#include <Geometrics.hpp>
#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <QPointF>
#include <Vector.hpp>
#include <algorithm>

const Bial::Vector< Bial::Point3D > ActiveContourTool::toPoint3DVector( const Path &path ) {
  Bial::Vector< Bial::Point3D > points;
  for( size_t pxl : path ) {
    auto coords = m_grayImg.Coordinates( pxl );
    points.push_back( Bial::Point3D( coords[ 0 ], coords[ 1 ], 0 ) );
  }
  return( points );
}

Bial::Vector< double > ActiveContourTool::calcHistogram( const Path &path, const Bial::Image< int > &img,
                                                         size_t bins ) {
  double max = img.Maximum( );
  Bial::Vector< double > hist( bins );
  for( size_t pxl: path ) {
    if( pxl < img.size( ) ) {
      size_t bin = std::min( static_cast< size_t >( std::round( bins * ( img[ pxl ] / max ) ) ), bins - 1 );
      hist[ bin ] += 1.0 / path.size( );
    }
  }
  return( hist );
}

FeatureData ActiveContourTool::pathDescription( const Path &path, const ActiveContourMethod *method ) {
  FeatureData features;
  features.Set( 0 );
  features[ method->type( ) ] = 1.0;
  if( path.empty( ) ) {
    return( features );
  }
  const Bial::Vector< Bial::Point3D > points = toPoint3DVector( path );
  double imgSz = m_grayImg.size( );
  double perimeter = points.size( ) / imgSz;
  double distance = Bial::Distance( points.front( ), points.back( ) ) / imgSz;
  features[ 3 ] = perimeter;
  features[ 4 ] = distance;
  features[ 5 ] = perimeter / distance;
//  auto grad_hist = calcHistogram( path, m_grad, 4 );
  auto grad_hist = calcHistogram( path, method->m_cost, 4 );
  for( size_t i = 0; i < grad_hist.size( ); ++i ) {
    features[ i + 6 ] = grad_hist[ i ];
  }
  return( features );
}

ActiveContourTool::ActiveContourTool( GuiImage *guiImage, ImageViewer *viewer ) try :
  Tool( guiImage, viewer ),
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
    std::make_shared< LazyWalkMethod >( m_pointIdxs, m_grayImg, m_grad ),
    std::make_shared< LinePathMethod >( m_pointIdxs, m_grayImg, m_grad )
  };
  m_currentMethod = LiveWireMethod::Type;

  setObjectName( "LiveWireTool" );

  setHasLabel( true );

  clear( );
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

ActiveContourTool::~ActiveContourTool( ) {
  clear( );
}

Bial::Image< int > ActiveContourTool::getResult( ) {
  Bial::Image< int > result( m_grayImg.Dim( ) );
  for( size_t pxl: m_currentPath ) {
    result[ pxl ] = 255;
  }
  return( result );
}

int ActiveContourTool::type( ) {
  return( ActiveContourTool::Type );
}

void ActiveContourTool::clear( ) {
  timer.start( );
  for( auto pt : m_points ) {
    m_scene->removeItem( pt );
  }
  qDeleteAll( m_points );
  m_points.clear( );
  m_pointIdxs.clear( );
  m_cache.fill( QColor( 0, 0, 0, 0 ) );
  m_res.fill( QColor( 0, 0, 0, 0 ) );
  m_selectedMethods.clear( );
  m_drawing = false;
  m_finished = false;
  m_currentPath.clear( );
  emit guiImage->imageUpdated( );
}

void ActiveContourTool::addPoint( QPointF pt ) {
  float x = pt.x( ), y = pt.y( );
  if( ( x < 0 ) || ( y < 0 ) || ( x >= guiImage->width( 0 ) ) ||
      ( y >= guiImage->heigth( 0 ) ) ) {
    qDebug( ) << "Point is outside of the image:" << pt;
    return;
  }
  QGraphicsItem *item = m_scene->itemAt( pt, QTransform( ) );
  if( item && ( item->type( ) == QGraphicsEllipseItem::Type ) ) {
    qDebug( ) << "There is another item in this position " << pt;
  }
  auto point = m_scene->addEllipse( QRectF( x - 2, y - 2, 4, 4 ), QPen( QColor( 0, 255, 0, 128 ), 1 ),
                                    QBrush( QColor( 0, 255, 0, 64 ) ) );
//  point->setFlag( QGraphicsItem::ItemIsMovable, true );
/*  point->setFlag( QGraphicsItem::ItemIsSelectable, true ); */
  m_points.append( point );
  m_pointIdxs.append( toPxIndex( point ) );
  if( m_points.size( ) > 1 ) {
#pragma omp parallel for
    for( int m = 0; m < m_methods.size( ); ++m ) {
      auto method = m_methods[ m ];
      Path path = method->updatePath( toPxIndex( point ) );
      if( method->type( ) == m_currentMethod ) {
        m_currentPath.insert( m_currentPath.end( ), path.begin( ), path.end( ) );
      }
      method->m_paths.push_back( path );
    }
    m_selectedMethods.push_back( m_currentMethod );

    m_cache.fill( QColor( 0, 0, 0, 0 ) );
    for( size_t pxl: m_currentPath ) {
      auto coord = m_grayImg.Coordinates( pxl );
      m_cache.setPixelColor( coord[ 0 ], coord[ 1 ], QColor( 255, 0, 255, 255 ) );
    }
  }
  m_res = m_cache;
  emit guiImage->imageUpdated( );
}

void ActiveContourTool::finishSegmentation( ) {
  addPoint( m_points.first( )->scenePos( ) + m_points.first( )->rect( ).center( ) );
  m_drawing = false;
  m_finished = true;
  m_res = m_cache;
}

void ActiveContourTool::mouseClicked( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {
  float x = pt.x( ), y = pt.y( );
  if( ( x < 0 ) || ( y < 0 ) || ( x >= guiImage->width( 0 ) ) ||
      ( y >= guiImage->heigth( 0 ) ) ) {
    return;
  }
  timer.start( );
  QGraphicsItem *item = m_scene->itemAt( pt, QTransform( ) );
  if( m_finished ) {
    return;
  }
  if( buttons & Qt::LeftButton ) {
    qDebug( ) << "Left click, method = " << m_currentMethod;
    if( ( item == NULL ) || ( ( item->type( ) != QGraphicsEllipseItem::Type ) ) ) {
      addPoint( m_lastPoint );
      m_drawing = true;
    }
    else if( item == m_points.first( ) ) {
      finishSegmentation( );
    }
  }
  else if( buttons & Qt::RightButton ) {
    m_currentMethod = ( m_currentMethod + 1 ) % m_methods.size( );
    updatePath( pt );
  }
  else if( buttons & Qt::MidButton ) {
    if( m_drawing ) {
      m_drawing = false;
    }
  }
  emit guiImage->imageUpdated( );
}


const Bial::Vector< int > &ActiveContourTool::getSelectedMethods( ) const {
  return( m_selectedMethods );
}

QVector< std::shared_ptr< ActiveContourMethod > > ActiveContourTool::getMethods( ) const {
  return( m_methods );
}

Path ActiveContourTool::getCurrentPath( ) const {
  return( m_currentPath );
}

int ActiveContourTool::getCurrentMethod( ) const {
  return( m_currentMethod );
}

void ActiveContourTool::setCurrentMethod( int currentMethod ) {
  m_currentMethod = currentMethod;
}

QImage ActiveContourTool::getRes( ) const {
  return( m_res );
}

void ActiveContourTool::setRes( const QImage &res ) {
  m_res = res;
}

QImage ActiveContourTool::getCache( ) const {
  return( m_cache );
}

void ActiveContourTool::setCache( const QImage &cache ) {
  m_cache = cache;
}

Bial::Point3D ActiveContourTool::toPoint3D( QGraphicsEllipseItem *item ) {
  float px = item->rect( ).center( ).x( ) + item->pos( ).x( );
  float py = item->rect( ).center( ).y( ) + item->pos( ).y( );
/*  return( transf( px, py, ( double ) guiImage->currentSlice( axis ) ) ); */
  return( m_transf( px, py, 0 ) );
}

size_t ActiveContourTool::toPxIndex( QGraphicsEllipseItem *item ) {
  const Bial::Point3D &point = toPoint3D( item );
  if( m_grayImg.ValidCoordinate( point.x, point.y ) ) {
    return( m_grayImg.Position( point.x, point.y ) );
  }
  else {
    return( m_grayImg.size( ) );
  }
}

Bial::Point3D ActiveContourTool::toPoint3D( const QPointF &qpoint ) {
  return( m_transf( qpoint.x( ), qpoint.y( ), 0 ) );
}

size_t ActiveContourTool::toPxIndex( const QPointF &qpoint ) {
  const Bial::Point3D &point = toPoint3D( qpoint );
  if( m_grayImg.ValidCoordinate( point.x, point.y ) ) {
    return( m_grayImg.Position( point.x, point.y ) );
  }
  else {
    return( m_grayImg.size( ) );
  }
}

void ActiveContourTool::updatePath( QPointF pt ) {
  float x = pt.x( ), y = pt.y( );
  if( ( x < 0 ) || ( y < 0 ) || ( x >= guiImage->width( 0 ) ) ||
      ( y >= guiImage->heigth( 0 ) ) ) {
    return;
  }
  m_res = m_cache;
  std::vector< Path > paths;
//  double best_val = std::numeric_limits< double >::max( );
  for( auto method : m_methods ) {
    Path path = method->updatePath( toPxIndex( pt ) );
    paths.push_back( path );
    int m = method->type( );
    auto features = pathDescription( path, method.get( ) );
    std::cout << m << " : " << features << std::endl;
  }
  for( int m = 0; m < m_methods.size( ); ++m ) {
    if( m != m_currentMethod ) {
      QColor clr = m_methods[ m ]->color;
      clr.setAlpha( 100 );
      for( size_t pxl : paths[ m ] ) {
        auto coords = m_grayImg.Coordinates( pxl );
        m_res.setPixelColor( coords[ 0 ], coords[ 1 ], clr );
      }
    }
  }
  for( size_t pxl : paths[ m_currentMethod ] ) {
    auto coords = m_grayImg.Coordinates( pxl );
    m_res.setPixelColor( coords[ 0 ], coords[ 1 ], Qt::red );
  }
}

void ActiveContourTool::mouseMoved( QPointF pt, size_t axis ) {
  if( timer.elapsed( ) > 30 ) {
    if( m_drawing ) {
      updatePath( pt );
    }
    m_lastPoint = pt;
    emit guiImage->imageUpdated( );
    timer.start( );
    qApp->processEvents( );
  }
}

void ActiveContourTool::mouseReleased( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {
  emit guiImage->imageUpdated( );
  Q_UNUSED( buttons );
  Q_UNUSED( pt );
  runLiveWire( );
}

void ActiveContourTool::mouseDragged( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {
  Q_UNUSED( buttons );
  Q_UNUSED( axis );
  Q_UNUSED( pt );
  /* nothing happens */
}

void ActiveContourTool::sliceChanged( size_t axis, size_t slice ) {
  Q_UNUSED( slice );
  needUpdate[ axis ] = true;
}

QPixmap ActiveContourTool::getLabel( size_t axis ) {
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

void ActiveContourTool::runLiveWire( ) {
  if( m_points.size( ) > 0 ) {
    const Bial::Vector< size_t > m_seeds = { m_pointIdxs.last( ) };
//#pragma omp parallel for default(none) firstprivate(m_seeds, m_currentPath) shared(m_methods)
    for( int m = 0; m < m_methods.size( ); ++m ) {
      m_methods[ m ]->run( m_seeds, m_currentPath );
    }
    needUpdate[ 0 ] = true;


    emit guiImage->imageUpdated( );
  }
}

void ActiveContourTool::enter( ) {
  for( auto pt : m_points ) {
    m_scene->addItem( pt );
  }
}

void ActiveContourTool::leave( ) {
  for( auto pt : m_points ) {
    m_scene->removeItem( pt );
  }
}
