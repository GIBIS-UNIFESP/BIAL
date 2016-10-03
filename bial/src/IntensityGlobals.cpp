/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Aug/27 
 * @brief Image intensity manipulation functions.
 */

#ifndef BIALINTENSITYGLOBALS_C
#define BIALINTENSITYGLOBALS_C

#include "IntensityGlobals.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_IntensityGlobals )
#define BIAL_EXPLICIT_IntensityGlobals
#endif
#if defined ( BIAL_EXPLICIT_IntensityGlobals ) || ( BIAL_IMPLICIT_BIN )

namespace Bial {

  template< class D >
  D Intensity::Minimum( const Image< D > &img, const Image< D > &msk ) {
    try {
      if( img.Dims( ) != msk.Dims( ) ) {
        std::string msg( BIAL_ERROR( "Image dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      D min = std::numeric_limits< D >::max( );
      for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
        if( ( msk[ pxl ] != 0.0 ) && ( min > img[ pxl ] ) ) {
          min = img[ pxl ];
        }
      }
      return( min );
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
  D Intensity::Maximum( const Image< D > &img, const Image< D > &msk ) {
    try {
      if( img.Dims( ) != msk.Dims( ) ) {
        std::string msg( BIAL_ERROR( "Image dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      D max = std::numeric_limits< D >::lowest( );
      for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
        if( ( msk[ pxl ] != 0.0 ) && ( max < img[ pxl ] ) ) {
          max = img[ pxl ];
        }
      }
      return( max );
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
  void Intensity::Complement( Image< D > &img ) {
    try {
      D max = img.Maximum( );
      for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
        img[ pxl ] = max - img[ pxl ];
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

  template< class D >
  void Intensity::Complement( Image< D > &img, const Image< D > &msk ) {
    try {
      if( img.Dims( ) != msk.Dims( ) ) {
        std::string msg( BIAL_ERROR( "Image dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      D max = img.Maximum( );
      for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
        if( msk[ pxl ] != 0.0 ) {
          img[ pxl ] = max - img[ pxl ];
        }
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


#ifdef BIAL_EXPLICIT_IntensityGlobals

  template int Intensity::Minimum( const Image< int > &img, const Image< int > &msk );
  template int Intensity::Maximum( const Image< int > &img, const Image< int > &msk );
  template void Intensity::Complement( Image< int > &img, const Image< int > &msk );
  template void Intensity::Complement( Image< int > &img );

  template llint Intensity::Minimum( const Image< llint > &img, const Image< llint > &msk );
  template llint Intensity::Maximum( const Image< llint > &img, const Image< llint > &msk );
  template void Intensity::Complement( Image< llint > &img, const Image< llint > &msk );
  template void Intensity::Complement( Image< llint > &img );

  template float Intensity::Minimum( const Image< float > &img, const Image< float > &msk );
  template float Intensity::Maximum( const Image< float > &img, const Image< float > &msk );
  template void Intensity::Complement( Image< float > &img, const Image< float > &msk );
  template void Intensity::Complement( Image< float > &img );

  template double Intensity::Minimum( const Image< double > &img, const Image< double > &msk );
  template double Intensity::Maximum( const Image< double > &img, const Image< double > &msk );
  template void Intensity::Complement( Image< double > &img, const Image< double > &msk );
  template void Intensity::Complement( Image< double > &img );

#endif

}

#endif

#endif


