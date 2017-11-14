/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2013/Oct/29
 * @brief RGB color space convertion methods.
 */

#ifndef BIALCOLORRGB_C
#define BIALCOLORRGB_C

#include "ColorRGB.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_ColorRGB )
#define BIAL_EXPLICIT_ColorRGB
#endif

#if defined ( BIAL_EXPLICIT_ColorRGB ) || ( BIAL_IMPLICIT_BIN )

#include "Image.hpp"

namespace Bial {

  namespace ColorSpace {

    template< class D >
    Image< Color > GraytoARGB( const Image< D > &img ) {
      try {
        COMMENT( "Creating color image.", 2 );
        Image< Color > res( img.Dim( ) );
        float max = img.Maximum( );
        COMMENT( "Converting to ARGB.", 2 );
        for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
          for( size_t chn = 1; chn < 4; ++chn )
            res( pxl )( chn ) = static_cast< uchar >( img( pxl ) / max * 255.0f );
        }
        return( res );
      }
      catch( std::bad_alloc &e ) {
        std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
        throw( std::runtime_error( msg ) );
      }
      catch( std::runtime_error &e ) {
        std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
        throw( std::runtime_error( msg ) );
      }
      catch( const std::logic_error &e ) {
        std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
        throw( std::logic_error( msg ) );
      }
    }

    template< class D >
    Image< D > ARGBtoGraybyLuminosity( const Image< Color > &img ) {
      try {
        COMMENT( "Creating resulting image.", 2 );
        Image< D > res( img.Dim( ) );
        COMMENT( "Converting to PGM.", 2 );
        for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
          uchar red = img( pxl )( 1 );
          uchar green = img( pxl )( 2 );
          uchar blue = img( pxl )( 3 );
          res( pxl ) = static_cast< D >( std::round( ( 0.21 * red + 0.72 * green + 0.07 * blue ) / 2.0 ) );
        }
        return( res );
      }
      catch( std::bad_alloc &e ) {
        std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
        throw( std::runtime_error( msg ) );
      }
      catch( std::runtime_error &e ) {
        std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
        throw( std::runtime_error( msg ) );
      }
      catch( const std::logic_error &e ) {
        std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
        throw( std::logic_error( msg ) );
      }
    }
  
    template< class D >
    Image< D > ARGBtoGraybyLightness( const Image< Color > &img ) {
      try {
        COMMENT( "Creating resulting image.", 2 );
        Image< D > res( img.Dim( ) );
        COMMENT( "Converting to PGM.", 2 );
        for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
          uchar red = img( pxl )( 1 );
          uchar green = img( pxl )( 2 );
          uchar blue = img( pxl )( 3 );
          res( pxl ) = static_cast< D >( std::round( ( std::max( std::max( red, green ), blue ) +
                                                       std::min( std::min( red, green ), blue ) ) / 2.0 ) );
        }
        return( res );
      }
      catch( std::bad_alloc &e ) {
        std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
        throw( std::runtime_error( msg ) );
      }
      catch( std::runtime_error &e ) {
        std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
        throw( std::runtime_error( msg ) );
      }
      catch( const std::logic_error &e ) {
        std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
        throw( std::logic_error( msg ) );
      }
    }

    template< class D >
    Image< D > ARGBtoGraybyBrightness( const Image< Color > &img ) {
      try {
        COMMENT( "Creating resulting image.", 2 );
        Image< D > res( img.Dim( ) );
        COMMENT( "Converting to PGM.", 2 );
        for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
          D red = img( pxl )( 1 );
          D green = img( pxl )( 2 );
          D blue = img( pxl )( 3 );
          res( pxl ) = static_cast< D >( std::round( ( red + green + blue ) / 3.0 ) );
        }
        return( res );
      }
      catch( std::bad_alloc &e ) {
        std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
        throw( std::runtime_error( msg ) );
      }
      catch( std::runtime_error &e ) {
        std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
        throw( std::runtime_error( msg ) );
      }
      catch( const std::logic_error &e ) {
        std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
        throw( std::logic_error( msg ) );
      }
    }

#ifdef BIAL_EXPLICIT_ColorRGB

    template Image< Color > GraytoARGB( const Image< int > &img );
    template Image< int > ARGBtoGraybyLuminosity( const Image< Color > &img );
    template Image< int > ARGBtoGraybyLightness( const Image< Color > &img );
    template Image< int > ARGBtoGraybyBrightness( const Image< Color > &img );

    template Image< Color > GraytoARGB( const Image< llint > &img );
    template Image< llint > ARGBtoGraybyLuminosity( const Image< Color > &img );
    template Image< llint > ARGBtoGraybyLightness( const Image< Color > &img );
    template Image< llint > ARGBtoGraybyBrightness( const Image< Color > &img );

    template Image< Color > GraytoARGB( const Image< float > &img );
    template Image< float > ARGBtoGraybyLuminosity( const Image< Color > &img );
    template Image< float > ARGBtoGraybyLightness( const Image< Color > &img );
    template Image< float > ARGBtoGraybyBrightness( const Image< Color > &img );

    template Image< Color > GraytoARGB( const Image< double > &img );
    template Image< double > ARGBtoGraybyLuminosity( const Image< Color > &img );
    template Image< double > ARGBtoGraybyLightness( const Image< Color > &img );
    template Image< double > ARGBtoGraybyBrightness( const Image< Color > &img );

#endif

  }

}

#endif

#endif
