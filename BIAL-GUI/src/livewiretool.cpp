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

const Bial::Vector< Bial::Point3D > LiveWireTool::toPoint3DVector( const Path &path ) {
  Bial::Vector< Bial::Point3D > points;
  for( size_t pxl : path ) {
    auto coords = m_grayImg.Coordinates( pxl );
    points.push_back( Bial::Point3D( coords[ 0 ], coords[ 1 ], 0 ) );
  }
  return( points );
}

Bial::Vector< double > LiveWireTool::calcHistogram( const Path &path, const Bial::Image< int > &img, size_t bins ) {
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

Bial::Array< double, NUM_FTR > LiveWireTool::pathDescription( const Path &path, const LWMethod *method ) {
  Bial::Array< double, NUM_FTR > features;
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

LiveWireTool::LiveWireTool( GuiImage *guiImage, ImageViewer *viewer ) try :
  Tool( guiImage, viewer ), m_seeds( guiImage->getSize( ) ),
  m_cache( guiImage->width( 0 ), guiImage->heigth( 0 ), QImage::Format_ARGB32 ),
  m_svm( cv::ml::SVM::create( ) ), m_transf( guiImage->getTransform( 0 ) ) {
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

  setHasLabel( true );

  m_svm->setType( cv::ml::SVM::C_SVC );
  m_svm->setKernel( cv::ml::SVM::LINEAR );
  m_svm->setTermCriteria( cv::TermCriteria( cv::TermCriteria::MAX_ITER, 100, 1e-6 ) );

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

LiveWireTool::~LiveWireTool( ) {
  clear( );
}

int LiveWireTool::type( ) {
  return( LiveWireTool::Type );
}

void LiveWireTool::clear( ) {
  for( auto pt : m_points ) {
    m_scene->removeItem( pt );
  }
  qDeleteAll( m_points );
  m_points.clear( );
  m_pointIdxs.clear( );
  m_cache.fill( QColor( 0, 0, 0, 0 ) );
  m_res.fill( QColor( 0, 0, 0, 0 ) );
  m_seeds.Set( false );
  m_selectedMethods.clear( );
  m_svm->clear( );
  m_drawing = false;
  m_finished = false;
  emit guiImage->imageUpdated( );
}

void LiveWireTool::roboto( ) {
  clear( );
  qDebug( ) << "Hello Mr Roboto!";
  RobotUser mrRoboto( *this );
  qDebug( ) << "Run, roboto, run!";
  mrRoboto.run( );
}

void LiveWireTool::addPoint( QPointF pt ) {
  qDebug( ) << "ADD point: " << pt;
  float x = pt.x( ), y = pt.y( );
  if( ( x < 0 ) || ( y < 0 ) || ( x >= guiImage->width( 0 ) ) ||
      ( y >= guiImage->heigth( 0 ) ) ) {
    return;
  }
  auto point = m_scene->addEllipse( QRectF( x - 3, y - 3, 6, 6 ), QPen( QColor( 0, 255, 0, 128 ), 1 ),
                                    QBrush( QColor( 0, 255, 0, 64 ) ) );
//  point->setFlag( QGraphicsItem::ItemIsMovable, true );
/*  point->setFlag( QGraphicsItem::ItemIsSelectable, true ); */
  m_points.append( point );
  m_pointIdxs.append( toPxIndex( point ) );
  if( m_points.size( ) > 1 ) {
    for( auto method : m_methods ) {
      auto path = method->updatePath( toPxIndex( point ) );
      method->m_paths.push_back( path );
      qDebug( ) << method->type( ) << " " << method->m_paths.size( );
    }
    m_selectedMethods.append( m_currentMethod );
    m_cache.fill( QColor( 0, 0, 0, 0 ) );
    for( int point = 0; point < m_selectedMethods.size( ); ++point ) {
      int m = m_selectedMethods[ point ];
      for( size_t pxl : m_methods[ m ]->m_paths[ point ] ) {
        auto coord = m_grayImg.Coordinates( pxl );
        m_cache.setPixelColor( coord[ 0 ], coord[ 1 ], QColor( 255, 0, 255, 255 ) );
      }
    }
    int num_samples = m_selectedMethods.size( );
    float trainingData[ num_samples ][ NUM_FTR ];
    int labels[ num_samples ];
    for( int sample = 0; sample < num_samples; ++sample ) {
      for( auto method : m_methods ) {
        auto features = pathDescription( method->m_paths[ sample ], method.get( ) );
        for( int ftr = 0; ftr < ( int ) features.size( ); ++ftr ) {
          trainingData[ sample ][ ftr ] = features[ ftr ];
          labels[ sample ] = ( method->type( ) == m_selectedMethods[ sample ] ) ? 1 : -1;
        }
      }
    }
    cv::Mat trainingDataMat( num_samples, NUM_FTR, cv::DataType< float >::type, trainingData );
    cv::Mat labelsMat( num_samples, 1, cv::DataType< int >::type, labels );
    std::cout << "Train result: " << m_svm->train( trainingDataMat, cv::ml::ROW_SAMPLE, labelsMat ) << std::endl;
  }
  emit guiImage->imageUpdated( );
}

void LiveWireTool::finishSegmentation( ) {
  addPoint( m_points.first( )->scenePos( ) + m_points.first( )->rect( ).center( ) );
  m_res = m_cache;
  m_drawing = false;
  m_finished = true;
}

void LiveWireTool::mouseClicked( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {
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
      addPoint( pt );
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
//    if( features[ 5 ] < best_val ) {
//      best_val = features[ 5 ];
//      m_currentMethod = method->type( );
//    }
//    qDebug( ) << m << features[ 5 ];
    std::cout << m << " : " << features << std::endl;
  }
//  if( m_points.size( ) > 1 ) {
//    float testData[ m_methods.size( ) ][ NUM_FTR ];
//    for( int m = 0; m < m_methods.size( ); ++m ) {
//      auto method = m_methods[ m ];
//      auto features = pathDescription( paths[ m ], method.get( ) );
//      for( int ftr = 0; ftr < features.size( ); ++ftr ) {
//        testData[ m ][ ftr ] = features[ ftr ];
//      }
//    }
//    cv::Mat testDataMat( m_methods.size( ), NUM_FTR, cv::DataType< float >::type, testData );
//    cv::Mat responsesMat( m_methods.size( ), 1, cv::DataType< int >::type );
//    std::cout << "SVM PREDICT: " << m_svm->predict( testDataMat, responsesMat ) << std::endl;
//    for( int m = 0; m < m_methods.size( ); ++m ) {
//      std::cout << "LABEL [" << m << "] = " << responsesMat.at< int >( m ) << std::endl;
//    }
//  }
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
  runLiveWire( );
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

void LiveWireTool::runLiveWire( ) {
  if( m_points.size( ) > 0 ) {
    m_seeds.Set( false );
//    for( size_t pxl: m_pointIdxs ) {
//      m_seeds[ pxl ] = true;
//    }
    m_seeds[ m_pointIdxs.last( ) ] = true;

#pragma omp parallel for default(none) firstprivate(m_seeds) shared(m_methods)
    for( int m = 0; m < m_methods.size( ); ++m ) {
      m_methods[ m ]->run( m_seeds );
    }
    needUpdate[ 0 ] = true;

    emit guiImage->imageUpdated( );
  }
}

void LiveWireTool::enter( ) {
  for( auto pt : m_points ) {
    m_scene->addItem( pt );
  }
}

void LiveWireTool::leave( ) {
  for( auto pt : m_points ) {
    m_scene->removeItem( pt );
  }
}
