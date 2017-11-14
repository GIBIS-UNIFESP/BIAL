/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2013/Oct/29
 * @brief HSI color space convertion methods.
 */

#ifndef BIALCOLORHSI_C
#define BIALCOLORHSI_C

#include "ColorHSI.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_ColorHSI )
#define BIAL_EXPLICIT_ColorHSI
#endif

#if defined ( BIAL_EXPLICIT_ColorHSI ) || ( BIAL_IMPLICIT_BIN )

#include "Color.hpp"
#include "Image.hpp"
#include "RealColor.hpp"

namespace Bial {

  namespace ColorSpace {

    RealColor ARGBtoAHSI( const Color &pxl ) {
      RealColor res;
      double red = pxl( 1 ) / 255.0;
      double green = pxl( 2 ) / 255.0;
      double blue = pxl( 3 ) / 255.0;
      double rmg = red - green;
      double rmb = red - blue;
      double gmb = green - blue;
      double sum = red + green + blue;

      COMMENT( "Computing the hue.", 3 );
      //std::cout << "rmg: " << rmg << " rmb: " << rmb << " gmb: " << gmb << std::endl;
      double theta = std::acos( 0.5 * ( rmg + rmb ) / ( 0.0000001 + std::sqrt( rmg * rmg + rmb * gmb ) ) );
      //std::cout << "theta: " << theta << std::endl;
      if( blue > green )
        res( 1 ) = 2 * M_PI - theta;
      else
        res( 1 ) = theta;
      COMMENT( "Computing the saturation.", 3 );
      res( 2 ) = 1.0 - 3.0 * std::min( red, std::min( green, blue ) ) / ( 0.00000001 + sum );
      COMMENT( "Computing intensity.", 3 );
      res( 3 ) = sum / 3.0;
      return( res );
    }

    Color AHSItoARGB( const RealColor &pxl ) {
      double hue = pxl( 1 );
      double saturation = pxl( 2 );
      double intensity = pxl( 3 );
      double red;
      double green;
      double blue;
      if( hue <= 2.0 / 3.0 * M_PI ) {
        COMMENT( "First sector 0 <= hue < 120", 3 );
        double coshue = std::cos( hue );
        double coshue60 = 0.000000001 + std::cos( M_PI / 3.0 - hue ); // 60 - hue
        blue = intensity * ( 1.0 - saturation );
        red = intensity * ( 1.0 + saturation * coshue / coshue60 );
        green = 3.0 * intensity - ( red + blue );
      }
      else if( hue <= 4.0 / 3.0 * M_PI ) {
        COMMENT( "First sector 120 <= hue < 240", 3 );
        hue -= 2.0 / 3.0 * M_PI; // hue - 120
        double coshue = std::cos( hue );
        double coshue60 = 0.000000001 + std::cos( M_PI / 3.0 - hue ); // 60 - hue
        red = intensity * ( 1.0 - saturation );
        green = intensity * ( 1.0 + saturation * coshue / coshue60 );
        blue = 3.0 * intensity - ( red + green );
      }
      else {
        COMMENT( "First sector 240 <= hue < 360", 3 );
        hue -= 4.0 / 3.0 * M_PI; // hue - 240
        double coshue = std::cos( hue );
        double coshue60 = 0.000000001 + std::cos( M_PI / 3.0 - hue ); // 60 - hue
        green = intensity * ( 1.0 - saturation );
        blue = intensity * ( 1.0 + saturation * coshue / coshue60 );
        red = 3.0 * intensity - ( green + blue );
      }
      Color res;
      res( 1 ) = std::min( 255.0, ( red * 255.0 ) + 0.5 );
      res( 2 ) = std::min( 255.0, ( green * 255.0 ) + 0.5 );
      res( 3 ) = std::min( 255.0, ( blue * 255.0 ) + 0.5 );
      return( res );
    }

    Image< RealColor > ARGBtoAHSI( const Image< Color > &img ) {
      try {
        Image< RealColor > res( img );
        size_t size = img.size( );
        for( size_t pxl = 0; pxl < size; ++pxl ) {
          res( pxl ) = ARGBtoAHSI( img( pxl ) );
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
      catch( const std::out_of_range &e ) {
        std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
        throw( std::out_of_range( msg ) );
      }
      catch( const std::logic_error &e ) {
        std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
        throw( std::logic_error( msg ) );
      }
    }

    Image< Color > AHSItoARGB( const Image< RealColor > &img ) {
      try {
        Image< Color > res( img );
        size_t size = img.size( );
        for( size_t pxl = 0; pxl < size; ++pxl ) {
          res( pxl ) = AHSItoARGB( img( pxl ) );
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
      catch( const std::out_of_range &e ) {
        std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
        throw( std::out_of_range( msg ) );
      }
      catch( const std::logic_error &e ) {
        std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
        throw( std::logic_error( msg ) );
      }
    }

#ifdef BIAL_EXPLICIT_ColorHSI

#endif

  }

}

#endif

#endif
