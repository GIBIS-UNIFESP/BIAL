/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @file GDCM.hpp
 * @author Lucas Santana Lellis 
 * @date 2014/Dec/3 
 * @version 1.0.00
 * @brief
 * Content: Bial GDCM class
 * <br> Description: Wrapper to the GDCM library 
 */
#include "Common.hpp"
#include "Image.hpp"
#include <gdcmImageReader.h>
#ifndef BIALGDCM_HPP
#define BIALGDCM_HPP

namespace Bial {

  class GDCM {
  private:
    template< class D >
    static Image< D > Convert2D( const gdcm::Image &gimage );
  public:
    template< class D >
    static Image< D > OpenGImage( const std::string &filename );
  };

}
namespace Bial {
  template< class D >
  Image< D > GDCM::OpenGImage( const std::string &filename ) {
    gdcm::ImageReader ir;
    ir.SetFileName( filename.c_str( ) );
    if( !ir.Read( ) ) {
      BIAL_WARNING( "GDCM Image Reader Failed to read file." )
	throw std::runtime_error( BIAL_ERROR( "GDCM Image Reader Failed to read file." ) );
    }
    COMMENT( "Getting image from GDCM ImageReader.", 1 );
    gdcm::Image gimage = ir.GetImage( );
    if( gimage.IsEmpty( ) ) {
      BIAL_WARNING( "GDCM GImage is empty!" );
      throw std::runtime_error( BIAL_ERROR( "GDCM GImage is empty!" ) );
    }
    if( gimage.GetNumberOfDimensions( ) == 2 ) {
      return( Convert2D< D >( gimage ) );
    }
    throw std::runtime_error( BIAL_ERROR( "3D image conversion not implemented yet" ) );
  }

  template< class D >
  Image< D > GDCM::Convert2D( const gdcm::Image &gimage ) {
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
      Image< D > res( spc_dim, pixel_size, 3 );
      const size_t channelSz = res.ChannelSize( );
      for( size_t pxl = 0; pxl < channelSz; ++pxl ) {
        res[ pxl ] = static_cast< D >( *buffer++ );
        res[ pxl + channelSz ] = static_cast< D >( *buffer++ );
        res[ pxl + channelSz * 2 ] = static_cast< D >( *buffer++ );
      }
      return( res );
    }
    else if( gimage.GetPhotometricInterpretation( ) == gdcm::PhotometricInterpretation::MONOCHROME2 ) {
      Image< D > res( spc_dim, pixel_size );
      COMMENT( "MONO2 FORMAT", 1 );
      if( gimage.GetPixelFormat( ) == gdcm::PixelFormat::UINT8 ) {
        COMMENT( "UINT8", 1 )
	  unsigned char *ubuffer = ( unsigned char* ) buffer;
        for( size_t pxl = 0; pxl < res.size( ); ++pxl ) {
          res[ pxl ] = static_cast< D >( *ubuffer++ );
        }
      }
      else if( gimage.GetPixelFormat( ) == gdcm::PixelFormat::INT16 ) {
        COMMENT( "INT16", 1 );
        short *buffer16 = ( short* ) buffer;
        for( size_t pxl = 0; pxl < res.size( ); ++pxl ) {
          res[ pxl ] = static_cast< D >( *buffer16++ );
        }
      }
      else if( gimage.GetPixelFormat( ) == gdcm::PixelFormat::UINT16 ) {
        COMMENT( "UINT16", 1 );
        unsigned short *buffer16 = ( unsigned short* ) buffer;
        for( size_t pxl = 0; pxl < res.size( ); ++pxl ) {
          res[ pxl ] = static_cast< D >( *buffer16++ );
        }
      }
      else {
        BIAL_WARNING( "Unhandled Pixel Format!" << ". Format is: " << gimage.GetPixelFormat( ) << ". Pixel Format is: "
		      << gimage.GetPixelFormat( ) << std::endl );
        throw std::runtime_error( BIAL_ERROR( "Unhandled Pixel Format!" ) );
      }
      return( res );
    }
    else {
      BIAL_WARNING( "Unhandled PhotometricInterpretation" << gimage.GetPhotometricInterpretation( ) )
	throw std::runtime_error( BIAL_ERROR( "Unhandled PhotometricInterpretation" ) );
    }
  }

}

#endif /* BIALGDCM_HPP */
