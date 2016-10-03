/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2013/Oct/29
 * @brief CIELab color space convertion methods.
 */

#ifndef BIALCOLORLAB_C
#define BIALCOLORLAB_C

#include "ColorLab.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_ColorLab )
#define BIAL_EXPLICIT_ColorLab
#endif

#if defined ( BIAL_EXPLICIT_ColorLab ) || ( BIAL_IMPLICIT_BIN )

#include "Color.hpp"
#include "Image.hpp"
#include "RealColor.hpp"

namespace Bial {

  namespace ColorSpace {

    Image< RealColor > ARGBtoXYZ( const Image< Color > &img ) {
      try {
        COMMENT( "Converting to real image.", 2 );
        size_t chSize = img.Size( );
        Image< RealColor > img_rgb( img );
        COMMENT( "Setting input image to the correct range.", 2 );
        img_rgb /= 255.0f;
        COMMENT( "Converting data.", 2 );
        Image< RealColor > img_XYZ( img_rgb );
        for( size_t pxl = 0; pxl < chSize; ++pxl ) {
          double red = ARGB_XYZ( img_rgb[ pxl ]( 1 ) );
          double green = ARGB_XYZ( img_rgb[ pxl ]( 2 ) );
          double blue = ARGB_XYZ( img_rgb[ pxl ]( 3 ) );
          img_XYZ[ pxl ]( 1 ) = 100.0 * ( red * 0.4124 + green * 0.3576 + blue * 0.1805 );
          img_XYZ[ pxl ]( 2 ) = 100.0 * ( red * 0.2126 + green * 0.7152 + blue * 0.0722 );
          img_XYZ[ pxl ]( 3 ) = 100.0 * ( red * 0.0193 + green * 0.1192 + blue * 0.9505 );
        }
        return( img_XYZ );
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

    Image< Color > XYZtoARGB( const Image< RealColor > &img ) {
      try {
        COMMENT( "Creating new image with correct range.", 2 );
        size_t chSize = img.Size( );
        Image< RealColor > img_XYZ( img );
        COMMENT( "Setting input image to the correct range.", 2 );
        img_XYZ /= 100.0f;
        COMMENT( "Converting data.", 2 );
        for( size_t pxl = 0; pxl < chSize; ++pxl ) {
          double X = img_XYZ[ pxl ]( 1 );
          double Y = img_XYZ[ pxl ]( 2 );
          double Z = img_XYZ[ pxl ]( 3 );
          img_XYZ[ pxl ]( 1 ) = std::abs( XYZ_ARGB( X * 3.2406 + Y * -1.5372 + Z * -0.4986 ) );
          img_XYZ[ pxl ]( 2 ) = std::abs( XYZ_ARGB( X * -0.9689 + Y * 1.8758 + Z * 0.0415 ) );
          img_XYZ[ pxl ]( 3 ) = std::abs( XYZ_ARGB( X * 0.0557 + Y * -0.2040 + Z * 1.0570 ) );
        }
        COMMENT( "Setting resultant image to the correct range.", 2 );
        img_XYZ *= 255.0f;
        Image< Color > img_rgb( img_XYZ.Dim( ) );
        for( size_t pxl = 0; pxl < chSize; ++pxl ) {
          img_rgb[ pxl ]( 1 ) = img_XYZ[ pxl ]( 1 );
          img_rgb[ pxl ]( 2 ) = img_XYZ[ pxl ]( 2 );
          img_rgb[ pxl ]( 3 ) = img_XYZ[ pxl ]( 3 );
        }
        return( img_rgb );
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

    Image< RealColor > ARGBtoCIELab( const Image< Color > &img ) {
      try {
        return( XYZtoCIELab( ARGBtoXYZ( img ) ) );
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
  
    Image< Color > CIELabtoARGB( const Image< RealColor > &img ) {
      try {
        return( XYZtoARGB( CIELabtoXYZ( img ) ) );
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

    Image< RealColor > XYZtoCIELab( const Image< RealColor > &img_XYZ ) {
      try {
        COMMENT( "Creating resultant image.", 2 );
        size_t chSize = img_XYZ.Size( );
        Image< RealColor > img_Lab( img_XYZ );
        COMMENT( "Converting data.", 2 );
        for( size_t pxl = 0; pxl < chSize; pxl++ ) {
          double fX = XYZ_CIELab( img_XYZ[ pxl ]( 1 ) / XwRef );
          double fY = XYZ_CIELab( img_XYZ[ pxl ]( 2 ) / YwRef );
          double fZ = XYZ_CIELab( img_XYZ[ pxl ]( 3 ) / ZwRef );
          img_Lab[ pxl ]( 1 ) = ( 116 * fY ) - 16;
          img_Lab[ pxl ]( 2 ) = 500.0 * ( fX - fY );
          img_Lab[ pxl ]( 3 ) = 200.0 * ( fY - fZ );
        }
        return( img_Lab );
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

    Image< RealColor > CIELabtoXYZ( const Image< RealColor > &img_Lab ) {
      try {
        COMMENT( "Creating resultant image.", 2 );
        size_t chSize = img_Lab.Size( );
        Image< RealColor > img_XYZ( img_Lab );
        COMMENT( "Converting data.", 2 );
        for( size_t pxl = 0; pxl < chSize; pxl++ ) {
          double L = img_Lab[ pxl ]( 1 );
          double a = img_Lab[ pxl ]( 2 );
          double b = img_Lab[ pxl ]( 3 );
          double Y = ( L + 16 ) / 116;
          double X = a / 500 + Y;
          double Z = Y - b / 200;
          img_XYZ[ pxl ]( 1 ) = XwRef * CIELab_XYZ( X );
          img_XYZ[ pxl ]( 2 ) = YwRef * CIELab_XYZ( Y );
          img_XYZ[ pxl ]( 3 ) = ZwRef * CIELab_XYZ( Z );
        }
        return( img_XYZ );
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

    double ARGB_XYZ( double color ) {
      if( color > 0.04045 ) {
        return( pow( ( color + 0.055 ) / 1.055, 2.4 ) );
      }
      else {
        return( color / 12.92 );
      }
    }

    double XYZ_ARGB( double color ) {
      if( color > 0.0031308 ) {
        return( 1.055 * ( pow( color, ( 1.0 / 2.4 ) ) ) - 0.055 );
      }
      else {
        return( 12.92 * color );
      }
    }

    double XYZ_CIELab( double color ) {
      double out;
      if( color > Epsilon ) {
        out = pow( color, 0.33334 ); /* x ^( 1/3 ) */
      }
      else {
        out = 7.787 * color + 0.137931034; /* 7.787 * x + 16/116 */
      }
      return( out );
    }

    double CIELab_XYZ( double color ) {
      double out = std::pow( color, 3 );
      if( out <= Epsilon ) {
        out = ( color - 0.137931034 ) / 7.787;
      }
      return( out );
    }

#ifdef BIAL_EXPLICIT_ColorLab

#endif

  }

}

#endif

#endif
