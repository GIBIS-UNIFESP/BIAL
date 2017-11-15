#include "FileImage.hpp"
#include "controller.h"
#include "defaulttool.h"
#include "gdcm.h"
#include <QDebug>
#include <QPointF>
#include <QRgb>

DefaultTool::DefaultTool( GuiImage *guiImage, ImageViewer *viewer ) : Tool( guiImage, viewer ), m_max( 0 ), labelType(
    LabelType::multilabel ), m_factor( 64.0 ) {
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

LabelType DefaultTool::getLabelType( ) const {
  return( labelType );
}

void DefaultTool::setLabelType( const LabelType &value ) {
  labelType = value;
  for( int axis = 0; axis < 4; ++axis ) {
    needUpdate[ axis ] = true;
  }
  emit guiImage->imageUpdated( );
}

void DefaultTool::setTranslucentFactor( float value ) {
  m_factor = value;
  for( int axis = 0; axis < 4; ++axis ) {
    needUpdate[ axis ] = true;
  }
  emit guiImage->imageUpdated( );
}

float DefaultTool::getFactor( ) const {
  return( m_factor );
}

bool validLabel( const Bial::Image< int > &label, GuiImage *guiImage, int max ) {
  bool valid = true;
  if( max == 0 ) {
    valid = false;
    BIAL_WARNING( "Label image should have valid pixels (pixel > 0)." );
  }
  else {
    if( label.Dims( ) == guiImage->getDims( ) ) {
      for( size_t dim = 0; dim < label.Dims( ); ++dim ) {
        valid &= ( label.Dim( ).at( dim ) == guiImage->getDim( ).at( dim ) );
      }
    }
    else {
      BIAL_WARNING( "Label image should have same dimensions. " << label.Dims( ) << " != " << guiImage->getDims( ) );
      valid = false;
    }
  }
  return( valid );
}

bool DefaultTool::addLabel( QString filename ) {
  /* TODO: Verify if the image maximum value is bigger than 0, and verify if image dimensions matchs. */
  std::cout << "Loading Label: " << filename.toStdString( ) << std::endl;
  Bial::Image< int > label = Bial::Read< int >( filename.toStdString( ) );
  int max = label.Maximum( );
  if( !validLabel( label, guiImage, max ) ) {
    return( false );
  }
  m_label = std::move( label );
  m_max = std::move( max );
  setHasLabel( true );
  for( int axis = 0; axis < 4; ++axis ) {
    needUpdate[ axis ] = true;
  }
  emit guiImage->imageUpdated( );
  return( true );
}

void DefaultTool::removeLabel( ) {
  setHasLabel( false );
  m_label = Bial::Image< int >( );
  m_max = 0.0;
  for( int axis = 0; axis < 4; ++axis ) {
    pixmaps[ axis ] = QPixmap( );
    needUpdate[ axis ] = false;
  }
  emit guiImage->imageUpdated( );
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

QRgb mapcolor( double v, double vmin, double vmax, int alpha ) {
  double r, g, b;
  r = g = b = 1.0;
  double dv;
  if( v < vmin ) {
    v = vmin;
  }
  if( v > vmax ) {
    v = vmax;
  }
  dv = vmax - vmin;
  if( v < ( vmin + 0.25 * dv ) ) {
    r = 0;
    g = 4 * ( v - vmin ) / dv;
  }
  else if( v < ( vmin + 0.5 * dv ) ) {
    r = 0;
    b = 1 + 4 * ( vmin + 0.25 * dv - v ) / dv;
  }
  else if( v < ( vmin + 0.75 * dv ) ) {
    r = 4 * ( v - vmin - 0.5 * dv ) / dv;
    b = 0;
  }
  else {
    g = 1 + 4 * ( vmin + 0.75 * dv - v ) / dv;
    b = 0;
  }
  int ir, ig, ib;
  ir = qMin( ( int ) ( 255 * r ), 255 );
  ig = qMin( ( int ) ( 255 * g ), 255 );
  ib = qMin( ( int ) ( 255 * b ), 255 );
  return( qRgba( ir, ig, ib, alpha ) );
}

QPixmap DefaultTool::getLabel( size_t axis ) {
  if( !needUpdate[ axis ] || !hasLabel( ) ) {
    return( pixmaps[ axis ] );
  }
  const size_t xsz = guiImage->width( axis );
  const size_t ysz = guiImage->heigth( axis );
  Bial::FastTransform transf = guiImage->getTransform( axis );
  QImage res( xsz, ysz, QImage::Format_ARGB32 );
  if( labelType == LabelType::none ) {
    res.fill( qRgba( 0, 0, 0, 0 ) );
  }
  else if( labelType == LabelType::translucent ) {
    float factor = m_factor / m_max;
#pragma omp parallel for firstprivate(axis, xsz, ysz, transf, factor) shared(res)
    for( size_t y = 0; y < ysz; ++y ) {
      QRgb *scanLine = ( QRgb* ) res.scanLine( y );
      for( size_t x = 0; x < xsz; ++x ) {
        int xx, yy, zz;
        transf( x, y, guiImage->currentSlice( axis ), &xx, &yy, &zz );
        int color = m_label( xx, yy, zz ) * factor;
        scanLine[ x ] = qRgba( 0, 255, 255, color );
      }
    }
  }
  else if( labelType == LabelType::solid ) {
    float factor = 255.0 / m_max;
    int alpha = qMin( static_cast< int >( m_factor ), 255 );
#pragma omp parallel for firstprivate(axis, xsz, ysz, alpha, transf, factor) shared(res)
    for( size_t y = 0; y < ysz; ++y ) {
      QRgb *scanLine = ( QRgb* ) res.scanLine( y );
      for( size_t x = 0; x < xsz; ++x ) {
        int xx, yy, zz;
        transf( x, y, guiImage->currentSlice( axis ), &xx, &yy, &zz );
        int color = m_label( xx, yy, zz ) * factor;
        if( color != 0 )
          scanLine[ x ] = qRgba( color, color, color, alpha );
        else
          scanLine[ x ] = qRgba( 0, 0, 0, alpha );
      }
    }
  }
  else if( labelType == LabelType::multilabel ) {
    res.fill( qRgba( 0, 0, 0, 0 ) );
    int alpha = qMin( static_cast< int >( m_factor ), 255 );
#pragma omp parallel for firstprivate(axis, xsz, ysz, alpha, transf) shared(res)
    for( size_t y = 0; y < ysz; ++y ) {
      QRgb *scanLine = ( QRgb* ) res.scanLine( y );
      for( size_t x = 0; x < xsz; ++x ) {
        int xx, yy, zz;
        transf( x, y, guiImage->currentSlice( axis ), &xx, &yy, &zz );
        int color = m_label( xx, yy, zz );
        if( color != 0 ) {
          scanLine[ x ] = mapcolor( color, 0, m_max, alpha );
        }
      }
    }
  }
  pixmaps[ axis ] = QPixmap::fromImage( res );
  return( pixmaps[ axis ] );
}
