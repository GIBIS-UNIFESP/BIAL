/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Jun/26 
 * @brief Feature vector composed by the coordinates of valid the matrix/image elements/pixels.
 * Valid elements/pixels have non-zero value/intensity.
 */

#ifndef BIALSPATIALFEATURE_C
#define BIALSPATIALFEATURE_C

#include "SpatialFeature.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_SpatialFeature )
#define BIAL_EXPLICIT_SpatialFeature
#endif

#if defined ( BIAL_EXPLICIT_SpatialFeature ) || ( BIAL_IMPLICIT_BIN )

#include "Feature.hpp"
#include "Image.hpp"
#include "Matrix.hpp"

namespace Bial {

  template< class D >
  Feature< D > SpatialFeature( const Matrix< D > &src ) {
    try {
      size_t features = src.Dims( );
      size_t elements = 0; // src.Elements( 1, src.Maximum( ) );
      for( size_t elm = 0; elm < src.size( ); ++elm ) {
        if( src( elm ) != 0 )
          ++elements;
      }
      Feature< D > res( elements, features );
      size_t tgt_elm = 0;
      for( size_t src_elm = 0; src_elm < src.Size( ); ++src_elm ) {
        if( src[ src_elm ] != 0 ) {
          Vector< size_t > coords = src.Coordinates( src_elm );
          for( size_t dms = 0; dms < coords.size( ); ++dms )
            res( tgt_elm, dms ) = coords( dms );
          res.Index( tgt_elm ) = src_elm;
          ++tgt_elm;
        }
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

  template< class D >
  Feature< D > SpatialFeature( const Image< D > &src ) {
    try {
      size_t features = src.Dims( );
      size_t elements = 0; // src.Elements( 1, src.Maximum( ) );
      for( size_t elm = 0; elm < src.size( ); ++elm ) {
        if( src( elm ) != 0 )
          ++elements;
      }
      Feature< D > res( elements, features );
      size_t tgt_elm = 0;
      for( size_t src_elm = 0; src_elm < src.Size( ); ++src_elm ) {
        if( src[ src_elm ] != 0 ) {
          Vector< size_t > coords = src.Coordinates( src_elm );
          for( size_t dms = 0; dms < features; ++dms )
            res( tgt_elm, dms ) = coords( dms );
          res.Index( tgt_elm ) = src_elm;
          ++tgt_elm;
        }
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

#ifdef BIAL_EXPLICIT_SpatialFeature

  template Feature< int > SpatialFeature( const Matrix< int > &src );
  template Feature< llint > SpatialFeature( const Matrix< llint > &src );
  template Feature< float > SpatialFeature( const Matrix< float > &src );
  template Feature< double > SpatialFeature( const Matrix< double > &src );

  template Feature< int > SpatialFeature( const Image< int > &src );
  template Feature< llint > SpatialFeature( const Image< llint > &src );
  template Feature< float > SpatialFeature( const Image< float > &src );
  template Feature< double > SpatialFeature( const Image< double > &src );

#endif

}

#endif

#endif
