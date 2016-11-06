#include "Adjacency.hpp"
#include "AdjacencyIterator.hpp"
#include "AdjacencyRound.hpp"
#include "ColorRGB.hpp"
#include "DrawLine.hpp"
#include "File.hpp"
#include "FileImage.hpp"
#include "Geometrics.hpp"
#include "GradientMorphological.hpp"
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

SegmentationTool::SegmentationTool( GuiImage *guiImage, ImageViewer *viewer ) : Tool( guiImage, viewer ), seeds(
    guiImage->getDim( ) ) {
  drawType = 1;
  drawing = false;
  setObjectName( "SegmentationTool" );
  setHasLabel( true );
  alpha = 0;
  beta = 0.5;
  thickness = 0;
  seedsVisible = true;
  maskVisible = true;
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
  Bial::Adjacency adj = Bial::AdjacencyType::HyperSpheric( thickness, dims );
  size_t size = seeds.size( );
  for( size_t px = 0; px < size; px++ ) {
    if( seeds[ px ] == 255 ) {
      Bial::AdjacencyIterator it( adj, seeds, px );
      for( it.begin( ); *it < seeds.size( ); ++it ) {
        seeds[ *it ] = drawType;
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

Bial::Image< int > SegmentationTool::segmentationOGS( int pf_type, double alpha, double beta ) {
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
    Bial::Image< int > res( 1, 1 );
    switch( pf_type ) {
    case 0: {
        res = Bial::Segmentation::OrientedGeodesicStar( img, obj_seed, bkg_seed, alpha, beta );
        break;
    }
    case 1: {
        Bial::Image< int > grad( Bial::Gradient::Morphological( img ) );
        res = Bial::Segmentation::Watershed( grad, obj_seed, bkg_seed );
        break;
    }
    default: {
        Bial::Image< int > grad( Bial::Gradient::Morphological( img ) );
        res = Bial::Segmentation::FSum( grad, obj_seed, bkg_seed );
    }
    }
    mask = Bial::Gradient::Morphological( res );
    emit guiImage->imageUpdated( );
    return( res );
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
