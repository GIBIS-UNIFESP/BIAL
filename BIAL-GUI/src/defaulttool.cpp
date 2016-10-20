#include "controller.h"
#include "defaulttool.h"
#include <QDebug>
#include <QPointF>

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

QVector<GuiImage *> DefaultTool::labels( ) const {
  return( m_labels );
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
}
