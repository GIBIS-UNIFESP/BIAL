#include "FileImage.hpp"
#include "controller.h"
#include "defaulttool.h"
#include "gdcm.h"
#include <QDebug>
#include <QPointF>
#include <QRgb>
DefaultTool::DefaultTool( GuiImage *guiImage, ImageViewer *viewer ) : Tool( guiImage, viewer ) {
  setObjectName( "DefaultTool" );
}

int DefaultTool::type( ) {
  return( DefaultTool::Type );
}

void DefaultTool::mouseReleased( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {
  COMMENT( "Mouse released.", 2 );
  if( buttons & Qt::LeftButton ) {
    changeOtherSlices( pt, axis );
    updateOverlay( viewer->getScene( axis )->overlayPos( ), axis );
  }
}

void DefaultTool::mouseClicked( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {
  COMMENT( "Mouse clicked.", 2 );
  if( buttons & Qt::LeftButton ) {
    changeOtherSlices( pt, axis );
    updateOverlay( viewer->getScene( axis )->overlayPos( ), axis );
  }
}

void DefaultTool::mouseDragged( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {
  COMMENT( "Mouse dragged.", 2 );
  if( buttons & Qt::LeftButton ) {
    changeOtherSlices( pt, axis );
    updateOverlay( viewer->getScene( axis )->overlayPos( ), axis );
  }
}

void DefaultTool::mouseMoved( QPointF pt, size_t axis ) {
  COMMENT( "Mouse moved.", 2 );
  Q_UNUSED( pt )
  Q_UNUSED( axis )
}

void DefaultTool::changeOtherSlices( QPointF posF, size_t view ) {
  COMMENT( "Changing slice position of other frames based on image position.", 2 );
  if( ( guiImage->modality( ) == Modality::BW3D ) ) {
    Bial::FastTransform transform = guiImage->getTransform( view );
    Bial::Point3D pt =
      transform( ( double ) posF.x( ), ( double ) posF.y( ), ( double ) guiImage->currentSlice( view ) );
    for( size_t other = 0; other < 3; ++other ) {
      if( other != view ) {
        Bial::FastTransform otherTransf = guiImage->getTransform( other ).Inverse( );
        Bial::Point3D otherPt = otherTransf( pt );
        size_t pos = static_cast< size_t >( round( otherPt.z ) );
        if( pos < guiImage->depth( other ) ) {
          guiImage->setCurrentSlice( other, pos );
        }
      }
    }
  }
}

const QVector< Bial::Image< int > > &DefaultTool::labels( ) const {
  return( m_labels );
}

void DefaultTool::addLabel( QString filename ) {
  std::cout << "Loading Label: " << filename.toStdString( ) << std::endl;
  m_labels.append( Bial::Read< int >( filename.toStdString( ) ) );
  m_max.append( m_labels.last().Maximum() );
  setHasLabel( true );
  for( int axis = 0; axis < 4; ++axis ) {
    needUpdate[ axis ] = true;
  }
}


void DefaultTool::updateOverlay( QPointF pt, size_t axis ) {
  /*  qDebug() << "Overlaypos = " << pt; */
  COMMENT( "ImageViewer::updateOverlay", 0 );
  pt.setX( qMin( qMax( pt.x( ), 0.0 ), ( double ) guiImage->width( axis ) ) );
  pt.setY( qMin( qMax( pt.y( ), 0.0 ), ( double ) guiImage->heigth( axis ) ) );
  viewer->getScene( axis )->setOverlayPos( pt );
  Controller *controller = viewer->controller( );

  Bial::FastTransform transform = guiImage->getTransform( axis );
  Bial::Point3D pt3d = transform( ( double ) pt.x( ), ( double ) pt.y( ), ( double ) guiImage->currentSlice( axis ) );
  size_t size = controller->currentFormat( )->getMaximumNumberOfViews( );
  for( size_t other = 0; other < size; ++other ) {
    if( controller->currentFormat( )->overlay( ) ) {
      viewer->getScene( other )->setOverlay( true );
      if( other != axis ) {
        Bial::FastTransform otherTransf = guiImage->getTransform( other ).Inverse( );
        Bial::Point3D otherPt = otherTransf( pt3d );
        viewer->getScene( other )->setOverlayPos( QPointF( otherPt.x, otherPt.y ) );
      }
    }
  }
}

void DefaultTool::sliceChanged( size_t axis, size_t slice ) {
  Q_UNUSED( slice )
  updateOverlay( viewer->getScene( axis )->overlayPos( ), axis );
  needUpdate[ axis ] = true;
}


QPixmap DefaultTool::getLabel( size_t axis ) {
  if( !needUpdate[ axis ] ) {
    return( pixmaps[ axis ] );
  }
  const size_t xsz = guiImage->width( axis );
  const size_t ysz = guiImage->heigth( axis );
  const Bial::FastTransform &transf = guiImage->getTransform( axis );
  QImage res( xsz, ysz, QImage::Format_ARGB32 );
  if( m_labels.isEmpty( ) ) {
    res.fill( qRgba( 0, 0, 0, 0 ) );
  }
  else {
    float factor = 64.0/m_max.last();
    const Bial::Image< int > &img( m_labels.last( ) );
#pragma omp parallel for firstprivate(axis, xsz, ysz)
    for( size_t y = 0; y < ysz; ++y ) {
      QRgb *scanLine = ( QRgb* ) res.scanLine( y );
      for( size_t x = 0; x < xsz; ++x ) {
        int xx, yy, zz;
        transf( x, y, guiImage->currentSlice( axis ), &xx, &yy, &zz );
        int color = img( xx, yy, zz ) * factor;
        scanLine[ x ] = qRgba( 0, 255, 255, color );
      }
    }
  }
  pixmaps[ axis ] = QPixmap::fromImage( res );
  return( pixmaps[ axis ] );
}
