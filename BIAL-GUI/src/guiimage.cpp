#include "ColorChannel.hpp"
#include "ColorRGB.hpp"
#include "Histogram.hpp"
#include "NiftiHeader.hpp"
#include "Signal.hpp"
#include "SignalEqualize.hpp"
#include "gdcm.h"
#include "guiimage.h"
#include "tool.h"
#include <QDebug>
#include <QDebug>
#include <QPixmap>
#include <QRgb>
#include <QTime>
#include <QTime>
#include <QtMath>
GuiImage::GuiImage( QString fname, QObject *parent ) : QObject( parent ), image( GDCM::OpenGImage(
                                                                                   fname.toStdString( ) ) ), m_fileName(
    fname ), m_contrast( 0 ), m_brightness( 0 ) {
  qDebug( ) << "guiimage.";

  COMMENT( "GuiImage 0.", 2 );
  transform.resize( 4 );
  /* Bial::Image< int > bial_img( getIntImage( ) ); // Error here. Set color image. */
  Bial::Vector< size_t > dim( getDim( ) );
  size_t dims( getDims( ) );
  m_equalizeHistogram = false;
  bounding.insert( 0, 4, Bial::BBox( Bial::Point3D( 0, 0, 0 ), Bial::Point3D( dim( 0 ), dim( 1 ), 1 ) ) );
  m_currentSlice.insert( 0, 4, 0 );
  COMMENT( "Getting maximum intensity and histogram.", 2 );
  Bial::MultiImageType img_type( image.Type( ) );
  switch( img_type ) {
      case Bial::MultiImageType::int_img: {
      Bial::Image< int > &img( getIntImage( ) );
      m_fmax = m_max = img.Maximum( );
      histogram = Bial::SignalType::ZeroStartHistogram( img );
      break;
    }
      case Bial::MultiImageType::flt_img: {
      Bial::Image< float > &img( getFltImage( ) );
      m_max = m_fmax = img.Maximum( );
      histogram = Bial::SignalType::ZeroStartHistogram( img );
      break;
    }
      case Bial::MultiImageType::clr_img: {
      Bial::Image< Bial::Color > &clr_img( getClrImage( ) );
      Bial::Image< int > img( Bial::ColorSpace::ARGBtoGraybyBrightness< int >( clr_img ) );
      histogram = Bial::SignalType::ZeroStartHistogram( img );
      Bial::Color clr( clr_img.Maximum( ) );
      m_fmax = m_max = std::max( clr[ 0 ], std::max( clr[ 1 ], clr[ 2 ] ) );
      break;
    }
      case Bial::MultiImageType::rcl_img: {
      /* Supposing HSV. Getting V channel. */
      Bial::Image< Bial::RealColor > &rcl_img( getRclImage( ) );
      Bial::Image< float > img( Bial::ColorSpace::Channel< float >( rcl_img, 2 ) );
      histogram = Bial::SignalType::ZeroStartHistogram( img );
      Bial::RealColor rcl( rcl_img.Maximum( ) );
      m_max = m_fmax = rcl[ 2 ];
      break;
    }
      default:
      std::string msg( BIAL_ERROR( "Accessing non-initialized multi-image." ) );
      throw( std::runtime_error( msg ) );
  }
  COMMENT( "Adjusting slice to appear on canvas.", 2 );
  if( dims == 3 ) {
    COMMENT( "NIfTI image detected.", 2 );
    m_modality = Modality::BW3D;
    if( Bial::NiftiHeader::IsNiftiFile( fname.toStdString( ) ) ) {
      {
        COMMENT( "Generating Axial affine transform.", 2 );
        transform[ 0 ].Rotate( 90.0, Bial::FastTransform::X ).Rotate( 90.0, Bial::FastTransform::Y );
        transform[ 0 ].Scale( 1, -1, -1 );
        updateBoundings( 0 );
      }
      {
        COMMENT( "Generating Coronal affine transform.", 2 );
        transform[ 1 ].Rotate( 180.0, Bial::FastTransform::Z ).Rotate( 90.0, Bial::FastTransform::Y );
        transform[ 1 ].Scale( -1, 1, 1 );
        updateBoundings( 1 );
      }
      {
        COMMENT( "Generating Sagittal affine transform.", 2 );
        transform[ 2 ].Rotate( 180.0, Bial::FastTransform::Z );
        updateBoundings( 2 );
      }
    }
    else {
      {
        COMMENT( "Generating Axial affine transform.", 2 );
        transform[ 0 ].Rotate( 90.0, Bial::FastTransform::X ).Rotate( 90.0, Bial::FastTransform::Y );
        updateBoundings( 0 );
      }
      {
        COMMENT( "Generating Coronal affine transform.", 2 );
        transform[ 1 ].Rotate( 90.0, Bial::FastTransform::Y );
        updateBoundings( 1 );
      }
      {
        COMMENT( "Generating Sagittal affine transform.", 2 );
        transform[ 2 ].Rotate( 180.0, Bial::FastTransform::Z );
        updateBoundings( 2 );
      }
    }
    cachedPixmaps.resize( 3 );
    needUpdate.insert( 0, 3, true );
    for( int view = 0; view < m_currentSlice.size( ); ++view ) {
      setCurrentSlice( view, depth( view ) / 2 );
    }
    m_currentToolPos = -1;
  }
  else if( ( img_type == Bial::MultiImageType::clr_img ) || ( img_type == Bial::MultiImageType::rcl_img ) ) {
    COMMENT( "PPM image detected.", 2 );
    m_modality = Modality::RGB2D;
    Bial::BBox box( Bial::Point3D( 0, 0, 0 ), Bial::Point3D( dim( 0 ), dim( 1 ), 1 ) );
    bounding[ 0 ] = box;
    cachedPixmaps.resize( 4 );
    needUpdate.insert( 0, 4, true );
  }
  else {
    COMMENT( "Gray image detected.", 2 );
    m_modality = Modality::BW2D;
    Bial::BBox box( Bial::Point3D( 0, 0, 0 ), Bial::Point3D( dim( 0 ), dim( 1 ), 1 ) );
    bounding[ 0 ] = box;
    cachedPixmaps.resize( 1 );
    needUpdate.push_back( true );
  }
  COMMENT( "Computing equalization transform.", 2 );
  Bial::Signal levi = histogram;
  levi[ 0 ] = 0;
  Bial::SignalOp::Equalize( levi );
  equalization.resize( levi.size( ) );
  for( size_t val = 0; val < levi.size( ); ++val ) {
    equalization[ val ] = std::round( levi[ val ] );
  }
  COMMENT( "Computing equalized histogram.", 2 );
  equalized = Bial::Signal( histogram.size( ), 0.0, 1.0 );
  for( size_t val = 0; val < equalized.size( ); ++val ) {
    equalized[ equalization[ val ] ] = histogram[ val ];
  }
  COMMENT( "Image " << fileName( ).toStdString( ) << " size = (" << width( 0 ) << ", " << heigth( 0 ) << ", " <<
           depth( 0 ) << ")", 0 );
}

GuiImage::~GuiImage( ) {
  qDeleteAll( tools );
}

Tool* GuiImage::currentTool( ) {
  if( tools.isEmpty( ) ) {
    return( nullptr );
  }
  if( m_currentToolPos >= ( size_t ) tools.size( ) ) {
    m_currentToolPos = 0;
  }
  return( tools.at( m_currentToolPos ) );
}

Modality GuiImage::modality( ) {
  return( m_modality );
}

QString GuiImage::fileName( ) {
  return( m_fileName );
}

QPointF GuiImage::getIntersection( size_t view ) {
  /* TODO: Get intersection between other views and current view. */
/*  size_t slice = currentSlice( view );
 *
 *  std::cout << m_currentSlice[ 0 ] << " " << m_currentSlice[ 1 ] << " " << m_currentSlice[ 2 ] << " " <<
 *  m_currentSlice[ 3 ] << std::endl;
 */
  return( QPointF( ) );
}

QPixmap GuiImage::getSlice( size_t view ) {
  size_t slice = currentSlice( view );
  COMMENT( "GET SLICE: image = " << m_fileName.toStdString( ) << ", axis = " << view << ", slice = " << slice, 2 );
  if( needUpdate[ view ] ) {
    if( slice >= depth( view ) ) {
      throw( std::out_of_range(
               BIAL_ERROR( QString( "Slice is out of range. Expected < %1" ).arg( depth( view ) ).toStdString( ) ) ) );
    }
    const size_t xsize = width( view );
    const size_t ysize = heigth( view );
    QImage res( xsize, ysize, QImage::Format_ARGB32 );
    double factor = 255.0 / ( double ) m_fmax;
    const Bial::FastTransform &transf = transform[ view ];
    switch( image.Type( ) ) {
        case Bial::MultiImageType::int_img: {
        COMMENT( "Generating BW view.", 2 );
        const Bial::Image< int > &img( getIntImage( ) );
#pragma omp parallel for default(none) shared(transf, img, res) firstprivate(slice, factor)
        for( size_t y = 0; y < ysize; ++y ) {
          QRgb *scanLine = ( QRgb* ) res.scanLine( y );
          for( size_t x = 0; x < xsize; ++x ) {
            int pixel = 0;
            int xx, yy, zz;
            transf( x, y, slice, &xx, &yy, &zz );
            pixel = img( xx, yy, zz ); /* Pegar imagem e depois usar aqui dentro. */
            if( m_equalizeHistogram ) {
              pixel = equalization[ pixel ];
            }
            pixel *= factor;
            scanLine[ x ] = qRgb( pixel, pixel, pixel );
          }
        }
        break;
      }
        case Bial::MultiImageType::flt_img: {
        COMMENT( "Generating BW float view.", 2 );
        const Bial::Image< float > &img( getFltImage( ) );
#pragma omp parallel for default(none) shared(transf, img, res) firstprivate(slice, factor)
        for( size_t y = 0; y < ysize; ++y ) {
          QRgb *scanLine = ( QRgb* ) res.scanLine( y );
          for( size_t x = 0; x < xsize; ++x ) {
            int pixel = 0;
            int xx, yy, zz;
            transf( x, y, slice, &xx, &yy, &zz );
            pixel = static_cast< int >( img( xx, yy, zz ) );
            if( m_equalizeHistogram ) {
              pixel = equalization[ pixel ];
            }
            pixel *= factor;
            scanLine[ x ] = qRgb( pixel, pixel, pixel );
          }
        }
        break;
      }
        case Bial::MultiImageType::clr_img: {
        if( needUpdate[ 0 ] ) {
          COMMENT( "Generating RGB view.", 2 );
          const Bial::Image< Bial::Color > &img( getClrImage( ) );
#pragma omp parallel for default(none) shared(transf, img, res) firstprivate(slice, factor)
          for( size_t y = 0; y < ysize; ++y ) {
            QRgb *scanLine = ( QRgb* ) res.scanLine( y );
            for( size_t x = 0; x < xsize; ++x ) {
              int xx, yy, zz;
              transf( x, y, slice, &xx, &yy, &zz );
              size_t pos = img.Position( xx, yy );
              int r = img[ pos ][ 1 ];
              int g = img[ pos ][ 2 ];
              int b = img[ pos ][ 3 ];
              if( m_equalizeHistogram ) {
                r = equalization[ r ];
                g = equalization[ g ];
                b = equalization[ b ];
              }
              scanLine[ x ] = qRgb( r * factor, g * factor, b * factor );
            }
          }
          cachedPixmaps[ 0 ] = QPixmap::fromImage( res );
          needUpdate[ 0 ] = false;
        }
        if( view > 0 ) {
          res = cachedPixmaps[ 0 ].toImage( );
          int r( view == 1 ), g( view == 2 ), b( view == 3 );
          for( size_t y = 0; y < ysize; ++y ) {
            QRgb *scanLine = ( QRgb* ) res.scanLine( y );
            for( size_t x = 0; x < xsize; ++x ) {
              QRgb clr = scanLine[ x ];
              scanLine[ x ] = qRgb( qRed( clr ) * r, qGreen( clr ) * g, qBlue( clr ) * b );
            }
          }
        }
        break;
      }
        case Bial::MultiImageType::rcl_img: {
        if( needUpdate[ 0 ] ) {
          COMMENT( "Generating RGB view.", 2 );
          const Bial::Image< Bial::RealColor > &img( getRclImage( ) );
#pragma omp parallel for default(none) shared(transf, img, res) firstprivate(slice, factor)
          for( size_t y = 0; y < ysize; ++y ) {
            QRgb *scanLine = ( QRgb* ) res.scanLine( y );
            for( size_t x = 0; x < xsize; ++x ) {
              int xx, yy, zz;
              transf( x, y, slice, &xx, &yy, &zz );
              size_t pos = img.Position( xx, yy );
              int r = img[ pos ][ 1 ];
              int g = img[ pos ][ 2 ];
              int b = img[ pos ][ 3 ];
              if( m_equalizeHistogram ) {
                r = equalization[ r ];
                g = equalization[ g ];
                b = equalization[ b ];
              }
              scanLine[ x ] = qRgb( r * factor, g * factor, b * factor );
            }
          }
          cachedPixmaps[ 0 ] = QPixmap::fromImage( res );
          needUpdate[ 0 ] = false;
        }
        if( view > 0 ) {
          res = cachedPixmaps[ 0 ].toImage( );
          int r( view == 1 ), g( view == 2 ), b( view == 3 );
          for( size_t y = 0; y < ysize; ++y ) {
            QRgb *scanLine = ( QRgb* ) res.scanLine( y );
            for( size_t x = 0; x < xsize; ++x ) {
              QRgb clr = scanLine[ x ];
              scanLine[ x ] = qRgb( qRed( clr ) * r, qGreen( clr ) * g, qBlue( clr ) * b );
            }
          }
        }
        break;
      }
        default:
        std::string msg( BIAL_ERROR( "Accessing non-initialized multi-image." ) );
        throw( std::runtime_error( msg ) );
    }
    if( needUpdate[ view ] ) {
      cachedPixmaps[ view ] = QPixmap::fromImage( res );
      needUpdate[ view ] = false;
    }
    /*    qDebug( ) << "Elapsed: " << timer.elapsed( ); */
  }
  if( ( m_contrast != 0 ) || ( m_brightness != 0 ) ) {
    return( applyContrastAndBrightness( cachedPixmaps[ view ] ) );
  }
  return( cachedPixmaps[ view ] );
}

QPixmap GuiImage::applyContrastAndBrightness( QPixmap pixmap ) {
  double contrastLevel = qPow( ( 100.0 + m_contrast ) / 100.0, 2 );

  QImage res = pixmap.toImage( );
  const size_t xsize = res.width( );
  const size_t ysize = res.height( );
  if( image.Type( ) == Bial::MultiImageType::int_img ) {
/* #pragma omp parallel for shared(res) firstprivate(xsize, ysize) */
    for( size_t y = 0; y < ysize; ++y ) {
      QRgb *scanLine = ( QRgb* ) res.scanLine( y );
      for( size_t x = 0; x < xsize; ++x ) {
        int pixel = qRed( scanLine[ x ] ) + m_brightness;
        pixel = qMax( qMin( ( ( ( ( pixel / 255.0 ) - 0.5 ) * contrastLevel ) + 0.5 ) * 255.0, 255.0 ), 0.0 );
        scanLine[ x ] = qRgb( pixel, pixel, pixel );
      }
    }
  }
  if( image.Type( ) == Bial::MultiImageType::clr_img ) {
/* #pragma omp parallel for shared(res) firstprivate(xsize, ysize) */
    for( size_t y = 0; y < ysize; ++y ) {
      QRgb *scanLine = ( QRgb* ) res.scanLine( y );
      for( size_t x = 0; x < xsize; ++x ) {
        int r = qRed( scanLine[ x ] ) + m_brightness;
        int g = qGreen( scanLine[ x ] ) + m_brightness;
        int b = qBlue( scanLine[ x ] ) + m_brightness;
        r = qMax( qMin( ( ( ( ( r / 255.0 ) - 0.5 ) * contrastLevel ) + 0.5 ) * 255.0, 255.0 ), 0.0 );
        g = qMax( qMin( ( ( ( ( g / 255.0 ) - 0.5 ) * contrastLevel ) + 0.5 ) * 255.0, 255.0 ), 0.0 );
        b = qMax( qMin( ( ( ( ( b / 255.0 ) - 0.5 ) * contrastLevel ) + 0.5 ) * 255.0, 255.0 ), 0.0 );
        scanLine[ x ] = qRgb( r, g, b );
      }
    }
  }
  return( QPixmap::fromImage( res ) );
}


size_t GuiImage::width( size_t view = 0 ) {
  return( abs( round( bounding.at( view ).pMax.x ) ) );
}

size_t GuiImage::heigth( size_t view = 0 ) {
  return( abs( round( bounding.at( view ).pMax.y ) ) );
}

size_t GuiImage::depth( size_t view = 0 ) {
  return( abs( round( bounding.at( view ).pMax.z ) ) );
}

void GuiImage::setCurrentSlice( size_t view, size_t slice ) {
  size_t sz = m_currentSlice.size( );
  if( view < sz ) {
    if( ( m_currentSlice[ view ] != slice ) && ( slice < depth( view ) ) ) {
      m_currentSlice[ view ] = slice;
      needUpdate[ view ] = true;
      emit imageUpdated( );
    }
  }
  else {
    throw std::out_of_range( BIAL_ERROR( "Axis out of range." ) );
  }
}

Bial::Point3D GuiImage::getPosition( QPointF pos, size_t view ) {
  Bial::Point3D point( pos.x( ), pos.y( ), ( double ) m_currentSlice[ view ] );
  transform[ view ]( point, &point );
  return( point );
}

Bial::FastTransform GuiImage::getTransform( size_t axis ) {
  return( transform.at( axis ) );
}

Bial::MultiImageType GuiImage::getImageType( ) const {
  return( image.Type( ) );
}

Bial::Image< int > &GuiImage::getIntImage( ) const {
  return( image.IntImage( ) );
}

Bial::Image< float > &GuiImage::getFltImage( ) const {
  return( image.FltImage( ) );
}

Bial::Image< Bial::Color > &GuiImage::getClrImage( ) const {
  return( image.ClrImage( ) );
}

Bial::Image< Bial::RealColor > &GuiImage::getRclImage( ) const {
  return( image.RclImage( ) );
}

size_t GuiImage::getDims( ) const {
  switch( image.Type( ) ) {
      case Bial::MultiImageType::int_img:
      return( getIntImage( ).Dims( ) );
      case Bial::MultiImageType::flt_img:
      return( getFltImage( ).Dims( ) );
      case Bial::MultiImageType::clr_img:
      return( getClrImage( ).Dims( ) );
      case Bial::MultiImageType::rcl_img:
      return( getRclImage( ).Dims( ) );
      default:
      std::string msg( BIAL_ERROR( "Getting dimensions from non-initialized multi-image." ) );
      throw( std::runtime_error( msg ) );
  }
}

Bial::Vector< size_t > GuiImage::getDim( ) const {
  switch( image.Type( ) ) {
      case Bial::MultiImageType::int_img:
      return( getIntImage( ).Dim( ) );
      case Bial::MultiImageType::flt_img:
      return( getFltImage( ).Dim( ) );
      case Bial::MultiImageType::clr_img:
      return( getClrImage( ).Dim( ) );
      case Bial::MultiImageType::rcl_img:
      return( getRclImage( ).Dim( ) );
      default:
      std::string msg( BIAL_ERROR( "Getting dimensions from non-initialized multi-image." ) );
      throw( std::runtime_error( msg ) );
  }
}

void GuiImage::rotate90( size_t view ) {
  Bial::FastTransform transf;
  transf.Rotate( -90.0, Bial::FastTransform::Z );
  transform[ view ] = transf * transform[ view ].Inverse( );
  updateBoundings( view );
  needUpdate[ view ] = true;
  emit imageUpdated( );
}

void GuiImage::flipH( size_t view ) {
  Bial::FastTransform transf;
  transf.Scale( -1, 1, 1 );
  transform[ view ] = transf * transform[ view ].Inverse( );
  updateBoundings( view );
  needUpdate[ view ] = true;
  emit imageUpdated( );
}

void GuiImage::flipV( size_t view ) {
  Bial::FastTransform transf;
  transf.Scale( 1, -1, 1 );
  transform[ view ] = transf * transform[ view ].Inverse( );
  updateBoundings( view );
  needUpdate[ view ] = true;
  emit imageUpdated( );
}

void GuiImage::rotateAll90( ) {
  for( int axis = 0; axis < needUpdate.size( ); ++axis ) {
    rotate90( axis );
  }
}

int GuiImage::max( ) {
  return( m_max );
}

float GuiImage::fmax( ) {
  return( m_fmax );
}

size_t GuiImage::currentSlice( size_t view ) {
  return( m_currentSlice[ view ] );
}

bool GuiImage::getEqualizeHistogram( ) const {
  return( m_equalizeHistogram );
}

void GuiImage::setEqualizeHistogram( bool equalizeHistogram ) {
  m_equalizeHistogram = equalizeHistogram;
  for( int axis = 0; axis < needUpdate.size( ); ++axis ) {
    needUpdate[ axis ] = true;
  }
  emit imageUpdated( );
}

const Bial::Signal &GuiImage::getHistogram( ) const {
  if( m_equalizeHistogram ) {
    return( equalized );
  }
  return( histogram );
}

int GuiImage::getPixel( int x, int y, int z ) {
  int color = 0;
  switch( image.Type( ) ) {
      case Bial::MultiImageType::int_img: {
      const Bial::Image< int > &img( getIntImage( ) );
      if( img.Dims( ) == 2 ) {
        if( img.ValidCoordinate( x, y ) ) {
          color = img( x, y );
        }
      }
      else {
        if( img.ValidCoordinate( x, y, z ) ) {
          color = img( x, y, z );
        }
      }
      break;
    }
      case Bial::MultiImageType::flt_img: {
      Bial::Image< float > &img( getFltImage( ) );
      if( img.Dims( ) == 2 ) {
        if( img.ValidCoordinate( x, y ) ) {
          color = img( x, y );
        }
      }
      else {
        if( img.ValidCoordinate( x, y, z ) ) {
          color = img( x, y, z );
        }
      }
      break;
    }
      case Bial::MultiImageType::clr_img: {
      Bial::Image< Bial::Color > &img( getClrImage( ) );
      if( img.ValidCoordinate( x, y ) ) {
        color = img( x, y ) ( 0 );
      }
      break;
    }
      case Bial::MultiImageType::rcl_img: {
      Bial::Image< Bial::RealColor > &img( getRclImage( ) );
      if( img.ValidCoordinate( x, y ) ) {
        color = img( x, y ) ( 0 );
      }
      break;
    }
      default:
      std::string msg( BIAL_ERROR( "Accessing non-initialized multi-image." ) );
      throw( std::runtime_error( msg ) );
  }
  if( m_equalizeHistogram ) {
    return( equalization[ color ] );
  }
  return( color );
}

size_t GuiImage::currentToolPos( ) const {
  return( m_currentToolPos );
}

void GuiImage::setCurrentToolPos( const size_t &currentToolPos ) {
  if( currentToolPos < static_cast< size_t >( tools.size( ) ) ) {
    m_currentToolPos = currentToolPos;
  }
}

int GuiImage::getContrast( ) const {
  return( m_contrast );
}

void GuiImage::setContrast( int contrast ) {
  m_contrast = contrast;
  for( int axis = 0; axis < needUpdate.size( ); ++axis ) {
    needUpdate[ axis ] = true;
  }
  emit imageUpdated( );
}

int GuiImage::getBrightness( ) const {
  return( m_brightness );
}

void GuiImage::setBrightness( int brightness ) {
  m_brightness = brightness;
  for( int axis = 0; axis < needUpdate.size( ); ++axis ) {
    needUpdate[ axis ] = true;
  }
  emit imageUpdated( );
}

void GuiImage::updateBoundings( size_t axis ) {
  Bial::Point3D start;
  Bial::Vector< size_t > size( getDim( ) );
  Bial::Point3D end( size[ 0 ] - 1, size[ 1 ] - 1, std::max( size[ 2 ] - 1, ( size_t ) 1 ) );
  transform[ axis ]( start, &start );
  transform[ axis ]( end, &end );
  bounding[ axis ] = Bial::BBox( start, end );
  transform[ axis ] = transform[ axis ].Inverse( );
  transform[ axis ].Translate( bounding[ axis ].pMin.x, bounding[ axis ].pMin.y, bounding[ axis ].pMin.z );
  bounding[ axis ] = bounding[ axis ].Normalized( );
}
