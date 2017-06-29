#include "Common.hpp"
#include "FileImage.hpp"
#include "NiftiHeader.hpp"
#include "gdcm.h"
#include "guiimage.h"

#include <QFileInfo>
#include <QImageReader>

#ifdef LIBGDCM
#include <gdcmImageReader.h>

Bial::Image< int > Convert2D( const gdcm::Image &gimage ) {
  if( gimage.IsEmpty( ) ) {
    BIAL_WARNING( "GDCM GImage is empty!" )
    throw std::runtime_error( BIAL_ERROR( "GDCM GImage is empty!" ) );
  }
  std::vector< char > vbuffer( gimage.GetBufferLength( ) );
  char *buffer = &vbuffer[ 0 ];
  gimage.GetBuffer( buffer );
  const std::vector< size_t > spc_dim = { gimage.GetDimension( 0 ), gimage.GetDimension( 1 ) };
  const std::vector< float > pixel_size = { static_cast< float >( gimage.GetSpacing( )[ 0 ] ),
                                            static_cast< float >( gimage.GetSpacing( )[ 1 ] ) };
  if( gimage.GetPhotometricInterpretation( ) == gdcm::PhotometricInterpretation::RGB ) {
    if( gimage.GetPixelFormat( ) != gdcm::PixelFormat::UINT8 ) {
      throw std::runtime_error( BIAL_ERROR( "Unhandled pixel format!" ) );
    }
    Bial::Image< int > res( spc_dim, pixel_size, 3 );
    const size_t channelSz = res.size( 0 ) * res.size( 1 );
    for( size_t pxl = 0; pxl < channelSz; ++pxl ) {
      res[ pxl ] = static_cast< int >( *buffer++ );
      res[ pxl + channelSz ] = static_cast< int >( *buffer++ );
      res[ pxl + channelSz * 2 ] = static_cast< int >( *buffer++ );
    }
    return( res );
  }
  else if( gimage.GetPhotometricInterpretation( ) == gdcm::PhotometricInterpretation::MONOCHROME2 ) {
    Bial::Image< int > res( spc_dim, pixel_size );
    COMMENT( "MONO2 FORMAT", 1 );
    if( gimage.GetPixelFormat( ) == gdcm::PixelFormat::UINT8 ) {
      COMMENT( "UINT8", 1 )
      unsigned char *ubuffer = ( unsigned char* ) buffer;
      for( size_t pxl = 0; pxl < res.size( ); ++pxl ) {
        res[ pxl ] = static_cast< int >( *ubuffer++ );
      }
    }
    else if( gimage.GetPixelFormat( ) == gdcm::PixelFormat::INT16 ) {
      COMMENT( "INT16", 1 );
      short *buffer16 = ( short* ) buffer;
      for( size_t pxl = 0; pxl < res.size( ); ++pxl ) {
        res[ pxl ] = static_cast< int >( *buffer16++ );
      }
    }
    else if( gimage.GetPixelFormat( ) == gdcm::PixelFormat::UINT16 ) {
      COMMENT( "UINT16", 1 );
      unsigned short *buffer16 = ( unsigned short* ) buffer;
      for( size_t pxl = 0; pxl < res.size( ); ++pxl ) {
        res[ pxl ] = static_cast< int >( *buffer16++ );
      }
    }
    else {
      BIAL_WARNING( "Unhandled Pixel Format!"
                    << ". Format is: " << gimage.GetPixelFormat( ) << ". Pixel Format is: " << gimage.GetPixelFormat( )
                    << std::endl );
      throw std::runtime_error( BIAL_ERROR( "Unhandled Pixel Format!" ) );
    }
    return( res );
  }
  else {
    BIAL_WARNING( "Unhandled PhotometricInterpretation" << gimage.GetPhotometricInterpretation( ) )
    throw std::runtime_error( BIAL_ERROR( "Unhandled PhotometricInterpretation" ) );
  }
}

Bial::Image< int > GDCM::OpenGImage( const std::string &filename ) {
  gdcm::ImageReader ir;
  ir.SetFileName( filename.c_str( ) );
  try {
    return( Bial::Read< int >( filename ) );
  }
  catch( std::bad_alloc &e ) {
    BIAL_WARNING( e.what( ) );
  }
  catch( std::runtime_error &e ) {
    BIAL_WARNING( e.what( ) );
  }
  catch( std::out_of_range &e ) {
    BIAL_WARNING( e.what( ) );
  }
  catch( std::logic_error &e ) {
    BIAL_WARNING( e.what( ) );
  }
  if( !ir.Read( ) ) {
    BIAL_WARNING( "Could not read " << filename << " with gdcm." );
    throw std::runtime_error( BIAL_ERROR( "GDCM CAN'T READ THIS IMAGE" ) );
  }
  COMMENT( "Getting image from GDCM ImageReader.", 1 );
  gdcm::Image gimage = ir.GetImage( );
  if( gimage.IsEmpty( ) ) {
    BIAL_WARNING( "GDCM GImage is empty!" );
    throw std::runtime_error( BIAL_ERROR( "GDCM CAN'T READ THIS IMAGE" ) );
  }
  if( gimage.GetNumberOfDimensions( ) == 2 ) {
    return( Convert2D( gimage ) );
  }
  else {
    BIAL_WARNING( "3D image conversion not implemented yet." );
    throw std::runtime_error( BIAL_ERROR( "3D image conversion not implemented yet." ) );
  }
  return( Bial::Image< int >( ) );
}

#else

Bial::MultiImage GDCM::OpenGImage( const std::string &filename ) {
  try {
    QFileInfo fileInfo( QString::fromStdString( filename ) );
    QRegExp regexpr3D( ".nii|.img|.scn" );
    if( fileInfo.completeSuffix( ).contains( regexpr3D ) ) {
      Bial::NiftiHeader hdr( filename );
      Bial::NiftiType type( hdr.DataType( ) );
      switch( type ) {
          case Bial::NiftiType::INT8:
          case Bial::NiftiType::INT16:
          case Bial::NiftiType::INT32:
          case Bial::NiftiType::UINT8:
          case Bial::NiftiType::UINT16:
          case Bial::NiftiType::UINT32:
          return( Bial::MultiImage( Bial::Read< int >( filename ) ) );
          case Bial::NiftiType::FLOAT32:
          return( Bial::MultiImage( Bial::Read< float >( filename ) ) );
          default:
          std::string msg( BIAL_ERROR( "Could not open Nifti image. Unsupported data type." ) );
          throw( std::runtime_error( msg ) );
      }
    }
    QImageReader imageReader( fileInfo.absoluteFilePath( ) );
    if( imageReader.format( ).isEmpty( ) ) {
      QRegExp regexprColorBial( ".ppm|.pnm" );
      if( fileInfo.completeSuffix( ).contains( regexprColorBial ) ) {
        return( Bial::MultiImage( Bial::Read< Bial::Color >( filename ) ) );
      }
      return( Bial::MultiImage( Bial::Read< int >( filename ) ) );
    }
    else {
      QImage qimage = imageReader.read( );
      size_t xsize = qimage.width( ), ysize = qimage.height( );
      Bial::Vector< size_t > dims = { xsize, ysize };
      qimage = qimage.convertToFormat( QImage::Format_Grayscale8 );
      Bial::Image< int > img( dims );
      for( size_t y = 0; y < ysize; ++y ) {
        uchar *scanLine = ( uchar* ) qimage.scanLine( y );
        for( size_t x = 0; x < xsize; ++x ) {
          uchar clr = scanLine[ x ];
          img( x, y ) = clr;
        }
      }
      return( Bial::MultiImage( img ) );
      // FIXME : Enable colored jpg image...
//      if( qimage.isGrayscale( ) ) {
//      }
//      else {
//        qimage = qimage.convertToFormat( QImage::Format_ARGB32 );
//        Bial::Image< Bial::Color > img( dims );
//        for( size_t y = 0; y < ysize; ++y ) {
//          QRgb *scanLine = ( QRgb* ) qimage.scanLine( y );
//          for( size_t x = 0; x < xsize; ++x ) {
//            QRgb clr = scanLine[ x ];
//            img( x, y ) = Bial::Color( qAlpha( clr ), qRed( clr ), qGreen( clr ), qBlue( clr ) );
//          }
//        }
//        return( Bial::MultiImage( img ) );
//      }
    }
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

#endif
