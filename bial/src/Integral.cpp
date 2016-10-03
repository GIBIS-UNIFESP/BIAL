/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Aug/27 
 * @brief Integral image construction and properties.
 */

#ifndef BIALINTEGRAL_C
#define BIALINTEGRAL_C

#include "Integral.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_Integral )
#define BIAL_EXPLICIT_Integral
#endif
#if defined ( BIAL_EXPLICIT_Integral ) || ( BIAL_IMPLICIT_BIN )

namespace Bial {

  template< class D >
  Image< double > Integral::IntegralImage( const Image< D > &img ) {
    try {
      Image< double > integral( img.Dim( ), img.PixelSize( ) );
      Vector< size_t > window_size( 3, 1 );
      for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
        COMMENT( "Window greater coordinates in N-dimensional space.", 4 );
        Vector< size_t > window_end = integral.Coordinates( pxl );
        integral[ pxl ] = img[ pxl ] - Integral::WindowIntegralValue( integral, window_end, window_size );
      }
      return( integral );
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

  double Integral::WindowIntegralValue( const Image< double > &integral, const Vector< size_t > &window_end,
                                        const Vector< size_t > &window_size ) {
    try {
      COMMENT( "Computing window restricted to image domain.", 3 );
      size_t dimensions = window_end.size( );
      Vector< size_t > domain_begin( window_end );
      Vector< size_t > domain_end( window_end );
      for( size_t dms = 0; dms < dimensions; ++dms ) {
        COMMENT( "Re-assigning window end in the case it exceeds image domain.", 3 );
        if( window_end( dms ) >= integral.size( dms ) ) {
          domain_end( dms ) = integral.size( dms ) - 1;
        }
        if( window_size( dms ) > window_end( dms ) ) {
          COMMENT( "Lower extremity exceeds image domain => domain begin is 0.", 3 );
          domain_begin( dms ) = 0;
        }
        else if( window_end( dms ) >= integral.size( dms ) ) {
          COMMENT( "Higher extremity exceeds image domain => Use domain begin is image_size - window_size.", 3 );
          domain_begin( dms ) = integral.size( dms ) - window_size( dms );
        }
        else {
          COMMENT( "Both extremeties in image domain => Use domain begin is window_end + 1 - window_size.", 3 );
          domain_begin( dms ) = window_end( dms ) + 1 - window_size( dms );
        }
      }
      if( dimensions == 2 ) {
        COMMENT( "For 2D image.", 3 );
        double sum = integral( domain_end( 0 ), domain_end( 1 ) );
        if( domain_begin( 0 ) != 0 ) {
          sum -= integral( domain_begin( 0 ) - 1, domain_end( 1 ) );
          if( domain_begin( 1 ) != 0 ) {
            sum += integral( domain_begin( 0 ) - 1, domain_begin( 1 ) - 1 );
          }
        }
        if( domain_begin( 1 ) != 0 ) {
          sum -= integral( domain_end( 0 ), domain_begin( 1 ) - 1 );
        }
        return( sum );
      }
      else { /* if( dimensions == 3 ) { */
        COMMENT( "For 3D image.", 3 );
        double sum = integral( domain_end( 0 ), domain_end( 1 ), domain_end( 2 ) );
        if( domain_begin( 0 ) != 0 ) {
          sum -= integral( domain_begin( 0 ) - 1, domain_end( 1 ), domain_end( 2 ) );
          if( domain_begin( 1 ) != 0 ) {
            sum += integral( domain_begin( 0 ) - 1, domain_begin( 1 ) - 1, domain_end( 2 ) );
          }
          if( domain_begin( 2 ) != 0 ) {
            sum += integral( domain_begin( 0 ) - 1, domain_end( 1 ), domain_begin( 2 ) - 1 );
          }
          if( ( domain_begin( 1 ) != 0 ) && ( domain_begin( 2 ) != 0 ) ) {
            sum -= integral( domain_begin( 0 ) - 1, domain_begin( 1 ) - 1, domain_begin( 2 ) - 1 );
          }
        }
        if( domain_begin( 1 ) != 0 ) {
          sum -= integral( domain_end( 0 ), domain_begin( 1 ) - 1, domain_end( 2 ) );
          if( domain_begin( 2 ) != 0 ) {
            sum += integral( domain_end( 0 ), domain_begin( 1 ) - 1, domain_begin( 2 ) - 1 );
          }
        }
        if( domain_begin( 2 ) != 0 ) {
          sum -= integral( domain_end( 0 ), domain_end( 1 ), domain_begin( 2 ) - 1 );
        }
        return( sum );
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

  double Integral::WindowIntegralValue( const Image< double > &integral, const Vector< size_t > &window_end,
                                        const Vector< float > &window_size_mm ) {
    try {
      Vector< size_t > window_size = Integral::PixelWindow( integral.PixelSize( ), window_size_mm );
      return( Integral::WindowIntegralValue( integral, window_end, window_size ) );
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

  double Integral::WindowVolume( const Vector< size_t > &window_size ) {
    try {
      double volume = window_size( 0 );
      for( size_t dms = 1; dms < window_size.size( ); ++dms ) {
        volume *= window_size( dms );
      }
      return( volume );
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

  template< class D >
  double Integral::WindowVolume( const Image< D > &img, const Vector< float > &window_size_mm ) {
    try {
      Vector< size_t > window_size = Integral::PixelWindow( img.PixelSize( ), window_size_mm );
      return( Integral::WindowVolume( window_size ) );
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

  Vector< size_t > Integral::PixelWindow( const Vector< float > &pixel_size,
                                          const Vector< float > &window_size_mm ) {
    try {
      Vector< size_t > window_size( window_size_mm.size( ) );
      for( size_t dms = 0; dms < window_size_mm.size( ); ++dms ) {
        window_size( dms ) = static_cast< size_t >( window_size_mm( dms ) / pixel_size( dms ) + 0.5 ); 
      }
      return( window_size );
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

  double Integral::WindowDensity( const Image< double > &integral, const Vector< size_t > &window_end,
                                  const Vector< size_t > &window_size ) {
    try {
      COMMENT( "Computing volume.", 2 );
      double volume = Integral::WindowVolume( integral, window_size );

      COMMENT( "Computing density.", 2 );
      double intensity = Integral::WindowIntegralValue( integral, window_end, window_size );
      if( volume == 0.0 ) {
        return( 0.0 );
      }
      return( intensity / volume );
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

  double Integral::WindowDensity( const Image< double > &integral, const Vector< size_t > &window_end,
                                  const Vector< float > &window_size_mm ) {
    try {
      Vector< size_t > window_size = Integral::PixelWindow( integral.PixelSize( ), window_size_mm );
      return( Integral::WindowDensity( integral, window_end, window_size ) );
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
      std::string msg( e.what( ) + std::string( "\n" ) + 
                       BIAL_ERROR( "Image, window end, and/or window size dimensions do not match." ) );
      throw( std::logic_error( msg ) );
    }
  }

#ifdef BIAL_EXPLICIT_Integral

  template Image< double > Integral::IntegralImage( const Image< int > &img );
  template double Integral::WindowVolume( const Image< int > &img, const Vector< float > &window_size_mm );
  template Image< double > Integral::IntegralImage( const Image< llint > &img );
  template double Integral::WindowVolume( const Image< llint > &img, const Vector< float > &window_size_mm );
  template Image< double > Integral::IntegralImage( const Image< float > &img );
  template double Integral::WindowVolume( const Image< float > &img, const Vector< float > &window_size_mm );
  template Image< double > Integral::IntegralImage( const Image< double > &img );
  template double Integral::WindowVolume( const Image< double > &img, const Vector< float > &window_size_mm );

#endif

}

#endif

#endif

