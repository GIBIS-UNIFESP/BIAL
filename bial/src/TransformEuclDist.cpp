/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2014/Jan/07
 * @brief Euclidean Distance Transform
 */

#ifndef BIALTRANSFORMEUCLDIST_C
#define BIALTRANSFORMEUCLDIST_C

#include "TransformEuclDist.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_TransformEuclDist )
#define BIAL_EXPLICIT_TransformEuclDist
#endif

#if defined ( BIAL_EXPLICIT_TransformEuclDist ) || ( BIAL_IMPLICIT_BIN )

#include "AdjacencyRound.hpp"
#include "Image.hpp"
#include "ImageIFT.hpp"
#include "GeodesicPathFunction.hpp"

namespace Bial {

  template< class D >
  Image< D > Transform::EDT( const Image< D > &border ) {
    try {
      Image< float > value( border );
      value.Set( 0.0f );
      Adjacency adj = AdjacencyType::HyperSpheric( 1.8, border.Dims( ) );
      GeodesicDistancePathFunction< float > path_func( value );
      COMMENT( "Setting seed pixels.", 0 );
      Vector< bool > seed( border.size( ) );
      for( size_t pxl = 0; pxl < border.size( ); ++pxl ) {
        if( border[ pxl ] != 0 ) {
          seed( pxl ) = true;
          value[ pxl ] = 0;
        }
        else
          value[ pxl ] = std::numeric_limits< D >::max( );          
      }
      COMMENT( "Computing distance transform with geodesic path function and image IFT.", 0 );
      ImageIFT< float > ift( value, adj, &path_func, &seed, static_cast< Image< int >* >( nullptr ),
                             static_cast< Image< int >* >( nullptr ), false, 0.001f, true );
      ift.Run( );
      return( value );
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
  Image< D > Transform::EDT( const Image< D > &border, const Image< D > &mask ) {
    try {
      Image< float > value( mask );
      Adjacency adj = AdjacencyType::HyperSpheric( 1.8, mask.Dims( ) );
      COMMENT( "Setting value for mask.", 0 );
      for( size_t pxl = 0; pxl < mask.size( ); ++pxl ) {
        if( mask[ pxl ] == 0 ) {
          value[ pxl ] = 0;
        }
      }
      GeodesicDistancePathFunction< float > path_func( value );
      COMMENT( "Setting seed pixels.", 0 );
      Vector< bool > seed( mask.size( ) );
      for( size_t pxl = 0; pxl < mask.size( ); ++pxl ) {
        if( border[ pxl ] != 0 ) {
          seed( pxl ) = true;
          value[ pxl ] = 0;
        }
        else {
          value[ pxl ] = std::numeric_limits< float >::max( );
          seed( pxl ) = false;
        }
      }
      COMMENT( "Computing distance transform with geodesic path function and image IFT.", 0 );
      ImageIFT< float > ift( value, adj, &path_func, &seed, static_cast< Image< int >* >( nullptr ),
                             static_cast< Image< int >* >( nullptr ), false, 0.001f, true );
      ift.Run( );
      return( value );
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

#ifdef BIAL_EXPLICIT_TransformEuclDist

  template Image< int > Transform::EDT( const Image< int > &border );
  template Image< llint > Transform::EDT( const Image< llint > &border );
  template Image< float > Transform::EDT( const Image< float > &border );
  template Image< double > Transform::EDT( const Image< double > &border );

  template Image< int > Transform::EDT( const Image< int > &border, const Image< int > &mask );
  template Image< llint > Transform::EDT( const Image< llint > &border, const Image< llint > &mask );
  template Image< float > Transform::EDT( const Image< float > &border, const Image< float > &mask );
  template Image< double > Transform::EDT( const Image< double > &border, const Image< double > &mask );

#endif

}

#endif

#endif
