#include "Adjacency.hpp"
#include "AdjacencyIterator.hpp"
#include "AdjacencyRound.hpp"
#include "ColorRGB.hpp"
#include "DrawLine.hpp"
#include "File.hpp"
#include "FileImage.hpp"
#include "Geometrics.hpp"
#include "GradientMorphological.hpp"
#include "MaxPathFunction.hpp"
#include "MultiImage.hpp"
#include "RealColor.hpp"
#include "SegmentationFSum.hpp"
#include "SegmentationGeoStar.hpp"
#include "SegmentationWatershed.hpp"
#include "guiimage.h"
#include "segmentationtool.h"
#include <QDebug>
#include <QMessageBox>
#include <QPointF>
#include <algorithm>

double SegmentationTool::getAlpha( ) const {
  return( alpha );
}

void SegmentationTool::setAlpha( double value ) {
  alpha = value;
}

double SegmentationTool::getBeta( ) const {
  return( beta );
}

void SegmentationTool::setBeta( double value ) {
  beta = value;
}

int SegmentationTool::getDrawType( ) const {
  return( drawType );
}

void SegmentationTool::setSeedsVisibility( bool vis ) {
  seedsVisible = vis;
  emit guiImage->imageUpdated( );
}

void SegmentationTool::setMaskVisibility( bool vis ) {
  maskVisible = vis;
  emit guiImage->imageUpdated( );
}

bool SegmentationTool::getSeedsVisible( ) const {
  return( seedsVisible );
}

bool SegmentationTool::getMaskVisible( ) const {
  return( maskVisible );
}

void SegmentationTool::setThickness( int value ) {
  thickness = value;
  COMMENT( "thickness set to" << thickness, 2 );
}

bool SegmentationTool::isInitiated( ) const {
    return( initiated );
}

SegmentationTool::SegmentationTool( GuiImage *guiImage, ImageViewer *viewer ) try :
    Tool( guiImage, viewer ), seeds( guiImage->getDim( ) ), cost( ), pred( 1, 1 ), label( 1, 1 ),
    int_path_func( nullptr ), flt_path_func( nullptr ), adj( ), int_ift( nullptr ), flt_ift( nullptr ) {
  COMMENT( "Initiating segmentation tool.", 0 );
  drawType = 1;
  drawing = false;
  setObjectName( "SegmentationTool" );
  setHasLabel( true );
  alpha = 0;
  beta = 0.5;
  thickness = 0;
  seedsVisible = true;
  maskVisible = true;
  initiated = false;
  pf = -1; // geosum path-function
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


SegmentationTool::~SegmentationTool( ) {
    delete int_path_func;
    delete flt_path_func;
    delete int_ift;
    delete flt_ift;
    int_path_func = nullptr;
    flt_path_func = nullptr;
    int_ift = nullptr;
    flt_ift = nullptr;
}

int SegmentationTool::type( ) {
  return( SegmentationTool::Type );
}

void SegmentationTool::mouseClicked( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {
  timer.start( );
  drawing = true;
  seedsVisible = true;
  if( ( drawType == 1 ) || ( drawType == 2 ) ) {
    switch( buttons ) {
        case Qt::LeftButton:
        drawType = 1;
        break;
        case Qt::RightButton:
        drawType = 2;
        break;
    }
  }
  const Bial::FastTransform &transf = guiImage->getTransform( axis );
  lastPoint = transf( pt.x( ), pt.y( ), ( double ) guiImage->currentSlice( axis ) );
}

void SegmentationTool::mouseMoved( QPointF pt, size_t axis ) {
  if( drawing ) {
    const Bial::FastTransform &transf = guiImage->getTransform( axis );
    Bial::Point3D current = transf( pt.x( ), pt.y( ), ( double ) guiImage->currentSlice( axis ) );
    if( lastPoint == current ) {
      return;
    }
    drawSeed( lastPoint, current );
    lastPoint = current;
    if( timer.elapsed( ) > 30 ) {
      emit guiImage->imageUpdated( );
      timer.start( );
    }
  }
}

void SegmentationTool::mouseReleased( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {
  drawing = false;
  if( drawing ) {
    const Bial::FastTransform &transf = guiImage->getTransform( axis );
    Bial::Point3D current = transf( pt.x( ), pt.y( ), ( double ) guiImage->currentSlice( axis ) );
    drawSeed( lastPoint, current );
  }
  emit guiImage->imageUpdated( );
  Q_UNUSED( buttons );
  Q_UNUSED( axis );
  Q_UNUSED( pt );
}

void SegmentationTool::mouseDragged( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {
  Q_UNUSED( buttons );
  Q_UNUSED( axis );
  Q_UNUSED( pt );
  /* nothing happens */
}

void SegmentationTool::sliceChanged( size_t axis, size_t slice ) {
  Q_UNUSED( slice );
  needUpdate[ axis ] = true;
}

void SegmentationTool::drawSeed( Bial::Point3D last, Bial::Point3D current ) {
  /*  std::cout << last << current << std::endl; */
  Bial::Vector< float > vLast;
  Bial::Vector< float > vCurrent;
  size_t dims = guiImage->getDims( );
  if( dims == 3 ) {
    vLast = { { ( float ) last[ 0 ], ( float ) last[ 1 ], ( float ) last[ 2 ] } };
    vCurrent = { { ( float ) current[ 0 ], ( float ) current[ 1 ], ( float ) current[ 2 ] } };
  }
  else {
    vLast = { { ( float ) last[ 0 ], ( float ) last[ 1 ] } };
    vCurrent = { { ( float ) current[ 0 ], ( float ) current[ 1 ] } };
  }
  Bial::Line imgLine( vLast, vCurrent, Bial::Color( 255, 255, 255, 255 ) );
  imgLine.Draw( seeds );
  Bial::Adjacency adj( Bial::AdjacencyType::HyperSpheric( thickness, dims ) );
  Bial::AdjacencyIterator it( seeds, adj );
  size_t adj_size = adj.size( );
  size_t size = seeds.size( );
  size_t adj_px;
  for( size_t px = 0; px < size; ++px ) {
    if( seeds[ px ] == 255 ) {
      for( size_t idx = 0; idx < adj_size; ++idx ) {
        if( ( it.*it.AdjIdx )( px, idx, adj_px ) )
          seeds[ adj_px ] = drawType;
      }
    }
  }
  for( size_t i = 0; i < needUpdate.size( ); ++i ) {
    needUpdate[ i ] = true;
  }
}

void SegmentationTool::setDrawType( int type ) {
  drawType = type;
}

void SegmentationTool::clearSeeds( ) {
  for( size_t i = 0; i < seeds.Size( ); ++i ) {
    seeds[ i ] = 0;
  }
  for( size_t i = 0; i < needUpdate.size( ); ++i ) {
    needUpdate[ i ] = true;
  }
  emit guiImage->imageUpdated( );
}

void SegmentationTool::Watershed( Bial::Image< int > &img, const Bial::Vector< size_t > &obj_seeds,
                                  const Bial::Vector< size_t > &bkg_seeds ) {
    try {
        if( ( initiated ) && ( pf != 1 ) ) {
            COMMENT( "Was running with other pathfunction. Restart required.", 0 );
            delete int_path_func;
            delete int_ift;
            int_path_func = nullptr;
            int_ift = nullptr;
            initiated = false;
        }
        COMMENT( "Getting initial parameters.", 0 );
        pf = 1;
        size_t size = img.size( );
        Bial::Image< int > *grad;
        if( !initiated )  {
            COMMENT( "Initialize maps and path function.", 0 );
            grad = new Bial::Image< int >( Bial::Gradient::Morphological( img ) );
            pred = Bial::Image< int >( img.Dim( ) );
            label = Bial::Image< int >( img.Dim( ) );
            int_path_func = new Bial::MaxPathFunction< Bial::Image, int >( *grad, 1.0 );
            for( size_t elm = 0; elm < size; ++elm )
                ( *grad )[ elm ] = std::numeric_limits< int >::max( );
        }
        else {
            COMMENT( "Continuing segmentation.", 0 );
            grad = &( cost.IntImage( ) );
        }
        COMMENT( "Initialize seeds.", 0 );
        Bial::Vector< bool > seeds( size, false );
        COMMENT( "Initialize object seeds. Seeds: " << obj_seeds.size( ), 0 );
        for( size_t elm = 0; elm < obj_seeds.size( ); ++elm ) {
            label[ obj_seeds[ elm ] ] = 1;
            seeds[ obj_seeds[ elm ] ] = true;
            ( *grad )[ obj_seeds[ elm ] ] = 0;
        }
        COMMENT( "Initialize background seeds. Seeds: " << bkg_seeds.size( ), 0 );
        for( size_t elm = 0; elm < bkg_seeds.size( ); ++elm ) {
            label[ bkg_seeds[ elm ] ] = 0;
            seeds[ bkg_seeds[ elm ] ] = true;
            ( *grad )[ bkg_seeds[ elm ] ] = 0;
        }
        COMMENT( "Fininished initializing seeds.", 0 );
        if( !initiated )  {
            COMMENT( "Initialize IFT.", 0 );
            cost = Bial::MultiImage( *grad );
            adj = Bial::AdjacencyType::HyperSpheric( 1.0, label.Dims( ) );
            int_ift = new Bial::ImageIFT< int >( cost.IntImage( ), adj, int_path_func, &seeds, &label, &pred, false, 1, true );
            initiated = true;
            delete grad;
        }
        else {
            COMMENT( "Updating seeds for IFT.", 0 );
            int_ift->InsertSeeds( seeds );
        }
        COMMENT( "Running IFT.", 0 );
        int_ift->Run( );
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
}


void SegmentationTool::Watershed( Bial::Image< float > &img, const Bial::Vector< size_t > &obj_seeds,
                                  const Bial::Vector< size_t > &bkg_seeds ) {
    if( ( initiated ) && ( pf != 1 ) ) {
        COMMENT( "Was running with other pathfunction. Restart required.", 0 );
        delete flt_path_func;
        delete flt_ift;
        flt_path_func = nullptr;
        flt_ift = nullptr;
        initiated = false;
    }
    pf = 1;
    size_t size = img.size( );
    Bial::Image< float > grad;
    if( !initiated )  {
      COMMENT( "Initialize maps and path function.", 0 );
      grad = Bial::Image< float >( Bial::Gradient::Morphological( img ) );
      pred = Bial::Image< int >( grad.Dim( ) );
      label = Bial::Image< int >( grad.Dim( ) );
      for( size_t elm = 0; elm < size; ++elm )
          grad[ elm ] = std::numeric_limits< float >::max( );
      flt_path_func = new Bial::MaxPathFunction< Bial::Image, float >( grad, 1.0 );
    }
    COMMENT( "Initialize seeds.", 0 );
    Bial::Vector< bool > seeds( size, false );
    for( size_t elm = 0; elm < obj_seeds.size( ); ++elm ) {
      label[ obj_seeds[ elm ] ] = 1;
      seeds[ obj_seeds[ elm ] ] = true;
      grad[ obj_seeds[ elm ] ] = 0.0f;
    }
    for( size_t elm = 0; elm < bkg_seeds.size( ); ++elm ) {
      label[ bkg_seeds[ elm ] ] = 0;
      seeds[ bkg_seeds[ elm ] ] = true;
      grad[ bkg_seeds[ elm ] ] = 0.0f;
    }
    if( !initiated )  {
        COMMENT( "Initialize IFT.", 0 );
        cost = Bial::MultiImage( grad );
        adj = Bial::AdjacencyType::HyperSpheric( 1.0, grad.Dims( ) );
        flt_ift = new Bial::ImageIFT< float >( cost.FltImage( ), adj, flt_path_func, &seeds, &label, &pred, false, 1.0f, true );
        initiated = true;
    }
    COMMENT( "Running IFT.", 0 );
    int_ift->Run( );
}

Bial::Image< int > SegmentationTool::connect( int pf_type, double alpha, double beta ) {
  Bial::Vector< size_t > obj_seed;
  Bial::Vector< size_t > bkg_seed;
  maskVisible = true;
  for( size_t i = 0; i < seeds.size( ); ++i ) {
    if( seeds[ i ] == 1 ) {
      obj_seed.push_back( i );
    }
    else if( seeds[ i ] == 2 ) {
      bkg_seed.push_back( i );
    }
  }
  Bial::Image< int > img;
  if( ( !obj_seed.empty( ) ) && ( !bkg_seed.empty( ) ) ) {
    switch( guiImage->getImageType( ) ) {
        case Bial::MultiImageType::int_img:
        img = guiImage->getIntImage( );
        break;
        case Bial::MultiImageType::flt_img:
        img = guiImage->getFltImage( );
        break;
        case Bial::MultiImageType::clr_img:
        img = Bial::ColorSpace::ARGBtoGraybyLuminosity< int >( guiImage->getClrImage( ) );
        break;
        case Bial::MultiImageType::rcl_img:
        img = Bial::ColorSpace::ARGBtoGraybyLuminosity< int >( guiImage->getRclImage( ) );
        break;
        default:
        std::string msg( BIAL_ERROR( "Getting image from non initialized multi-image." ) );
        throw( std::runtime_error( msg ) );
    }
    switch( pf_type ) {
    case 0: {
        // OrientedGeodesicStar( obj_seed, bkg_seed, alpha, beta );
        break;
    }
    case 1: {
        COMMENT( "Running Max Path Function IFT.", 0 );
        switch( guiImage->getImageType( ) ) {
          case Bial::MultiImageType::int_img:
            Watershed( guiImage->getIntImage( ), obj_seed, bkg_seed );
            break;
          case Bial::MultiImageType::flt_img:
            Watershed( guiImage->getFltImage( ), obj_seed, bkg_seed );
            break;
          case Bial::MultiImageType::clr_img: {
            Bial::Image< int > img( Bial::ColorSpace::ARGBtoGraybyLuminosity< int >( guiImage->getClrImage( ) ) );
            Watershed( img, obj_seed, bkg_seed );
            break;
          }
          case Bial::MultiImageType::rcl_img: {
            Bial::Image< int > img( Bial::ColorSpace::ARGBtoGraybyLuminosity< int >( guiImage->getRclImage( ) ) );
            Watershed( img, obj_seed, bkg_seed );
            break;
          }
          default:
            qDebug( "Error: Segmented uninitialized image." );
        }
    }
    default: {
        //Bial::Image< int > grad( Bial::Gradient::Morphological( img ) );
        //FSum( obj_seed, bkg_seed );
    }
    }
    mask = Bial::Gradient::Morphological( label );
    emit guiImage->imageUpdated( );
    return( label );
  }
  else {
    throw std::runtime_error( "Seeds Missing" );
  }
}

QPixmap SegmentationTool::getLabel( size_t axis ) {
  const size_t xsz = guiImage->width( axis );
  const size_t ysz = guiImage->heigth( axis );
  if( !seedsVisible && !maskVisible ) {
    return( QPixmap( ) );
  }
  if( !needUpdate[ axis ] ) {
    return( pixmaps[ axis ] );
  }
  const Bial::FastTransform &transf = guiImage->getTransform( axis );
  QImage res( xsz, ysz, QImage::Format_ARGB32 );
  if( !seedsVisible ) {
    res.fill( qRgba( 0, 0, 0, 0 ) );
  }
  else {
#pragma omp parallel for firstprivate(axis, xsz, ysz)
    for( size_t y = 0; y < ysz; ++y ) {
      QRgb *scanLine = ( QRgb* ) res.scanLine( y );
      for( size_t x = 0; x < xsz; ++x ) {
        Bial::Point3D pos = transf( x, y, guiImage->currentSlice( axis ) );
        char pixel = seeds( pos.x, pos.y, pos.z );
        if( pixel == 1 ) {
          scanLine[ x ] = qRgb( 0, 255, 0 );
        }
        else if( pixel == 2 ) {
          scanLine[ x ] = qRgb( 0, 0, 255 );
        }
        else {
          scanLine[ x ] = qRgba( 0, 0, 0, 0 );
        }
      }
    }
  }
  if( maskVisible && ( mask.size( ) == seeds.size( ) ) ) {
#pragma omp parallel for firstprivate(axis, xsz, ysz)
    for( size_t y = 0; y < ysz; ++y ) {
      QRgb *scanLine = ( QRgb* ) res.scanLine( y );
      for( size_t x = 0; x < xsz; ++x ) {
        Bial::Point3D pos = transf( x, y, guiImage->currentSlice( axis ) );
        if( mask( pos.x, pos.y, pos.z ) ) {
          scanLine[ x ] = qRgb( 255, 0, 0 );
        }
      }
    }
  }
  pixmaps[ axis ] = QPixmap::fromImage( res );
  return( pixmaps[ axis ] );
}
