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
#include "GrowingBucketQueue.hpp"
#include "Image.hpp"
#include "ImageIFT.hpp"
#include "GeodesicPathFunction.hpp"

namespace Bial {

  template< class D >
  Image< D > Transform::EDT( const Image< D > &border ) {
    try {
      size_t size = border.size( );
      Image< float > value( border.Dim( ), border.PixelSize( ) );
      Adjacency adj = AdjacencyType::HyperSpheric( 1.8, border.Dims( ) );
      GeodesicDistancePathFunction< float > path_func( value, nullptr, nullptr, false, adj, value );
      GrowingBucketQueue queue( size, 0.001f, true, true );
      COMMENT( "Setting seed pixels.", 0 );
      for( size_t pxl = 0; pxl < size; ++pxl ) {
        if( border[ pxl ] != 0 ) {
          value[ pxl ] = 0;
          queue.Insert( pxl, value[ pxl ] );
        }
        else
          value[ pxl ] = std::numeric_limits< float >::max( ); // Mudado de D para float.
      }
      COMMENT( "Computing distance transform with geodesic path function and image IFT.", 0 );
      ImageIFT< float > ift( value, adj, &path_func, &queue );
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
      size_t size = border.size( );
      Image< float > value( mask );
      Adjacency adj = AdjacencyType::HyperSpheric( 1.8, border.Dims( ) );
      GeodesicDistancePathFunction< float > path_func( value, nullptr, nullptr, false, adj, value );
      GrowingBucketQueue queue( size, 0.001f, true, true );
      COMMENT( "Setting seed pixels.", 0 );
      for( size_t pxl = 0; pxl < size; ++pxl ) {
        if( mask[ pxl ] != 0 ) {
          if( border[ pxl ] != 0 ) {
            value[ pxl ] = 0;
            queue.Insert( pxl, value[ pxl ] );
          }
          else
            value[ pxl ] = std::numeric_limits< float >::max( ); // Mudado de D para float.
        }
      }
      COMMENT( "Computing distance transform with geodesic path function and image IFT.", 0 );
      ImageIFT< float > ift( value, adj, &path_func, &queue );
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
