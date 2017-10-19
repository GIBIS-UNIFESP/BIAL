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
#include "AdjacencyIterator.hpp"
#include "DistanceBucketQueue.hpp"
#include "GeodesicPathFunction.hpp"
#include "GrowingBucketQueue.hpp"
#include "Image.hpp"
#include "ImageIFT.hpp"

namespace Bial {

  template< class D >
  Image< D > Transform::EDT( const Image< D > &border ) {
    try {
      COMMENT( "Creating images for processing.", 0 );
      size_t size = border.size( );
      Image< D > value( border.Dim( ), border.PixelSize( ) );
      COMMENT( "Square value kept for faster comparison between values.", 0 );
      Image< int > sqr_value( border.Dim( ), border.PixelSize( ) );
      Image< int > root( border.Dim( ), border.PixelSize( ) );
      COMMENT( "Creating adjacency.", 0 );
      Adjacency adj( AdjacencyType::HyperSpheric( 1.8f, border.Dims( ) ) );
      AdjacencyIterator adj_itr( value, adj );
      size_t adj_size = adj.size( );
      COMMENT( "Setting seed pixels.", 0 );
      COMMENT( "Creating bucket queue with the longest distance in the image.", 0 );
      DistanceBucketQueue queue( size, border.size( 0 ) * border.size( 0 ) + border.size( 1 ) * border.size( 1 ) + 
                                 border.size( 2 ) * border.size( 2 ) + 1 );
      for( size_t pxl = 0; pxl < size; ++pxl ) {
        if( border[ pxl ] != 0 ) {
          root[ pxl ] = pxl;
          queue.Insert( pxl, sqr_value[ pxl ] );
        }
        else
          sqr_value[ pxl ] = std::numeric_limits< D >::max( ); // Mudado de D para float. Voltei aqora...
      }
      COMMENT( "Running Image IFT. Queue: " << ( queue.Empty( ) ? "empty" : "not empty" ), 0 );
      Vector< size_t > adj_coords( 3 );
      size_t adj_index;
      if( border.Dims( ) == 3 ) {
        while( !queue.Empty( ) ) {
          COMMENT( "Initializing removed data.", 4 );
          size_t src_index = queue.Remove( );
          Vector< size_t > src_coords( border.Coordinates( src_index ) );
          Vector< size_t > root_coords( border.Coordinates( root[ src_index ] ) );
          size_t src_value = sqr_value[ src_index ];
          COMMENT( "Index: " << src_index << ", sqr_value: " << src_value, 4 );
          queue.Finished( src_index );
          size_t previous_value;
          for( size_t adj_pos = 0; adj_pos < adj_size; ++adj_pos ) {
            if( ( adj_itr.AdjVct( src_coords, adj_pos, adj_coords ) ) && 
                ( adj_index = src_index + adj_itr.Displacement( adj_pos ) ) && 
                ( previous_value = sqr_value[ adj_index ] ) && ( src_value < previous_value ) ) {
              COMMENT( "Conquering: " << adj_index << " with previous_sqr_value: " << previous_value, 4 );
              D prp_value = ( root_coords[ 0 ] - adj_coords[ 0 ] ) * ( root_coords[ 0 ] - adj_coords[ 0 ] ) +
                ( root_coords[ 1 ] - adj_coords[ 1 ] ) * ( root_coords[ 1 ] - adj_coords[ 1 ] ) + 
                ( root_coords[ 2 ] - adj_coords[ 2 ] ) * ( root_coords[ 2 ] - adj_coords[ 2 ] );
              COMMENT( "Updating value.", 3 );
              if( previous_value > prp_value ) {
                sqr_value[ adj_index ] = prp_value;
                root[ adj_index ] = root[ src_index ];
                queue.Update( adj_index, previous_value, sqr_value[ adj_index ] );
              }
            }
          }
        }
      }
      // Optimized 2D version. It is around 6% faster.
      else {
        while( !queue.Empty( ) ) {
          COMMENT( "Initializing removed data.", 4 );
          size_t src_index = queue.Remove( );
          Vector< size_t > src_coords( border.Coordinates2( src_index ) );
          Vector< size_t > root_coords( border.Coordinates2( root[ src_index ] ) );
          size_t src_value = sqr_value[ src_index ];
          COMMENT( "Index: " << src_index << ", sqr_value: " << src_value, 4 );
          queue.Finished( src_index );
          size_t previous_value;
          for( size_t adj_pos = 0; adj_pos < adj_size; ++adj_pos ) {
            if( ( adj_itr.AdjVct2( src_coords, adj_pos, adj_coords ) ) && 
                ( adj_index = src_index + adj_itr.Displacement( adj_pos ) ) && 
                ( previous_value = sqr_value[ adj_index ] ) && ( src_value < previous_value ) ) {
              COMMENT( "Conquering: " << adj_index << " with previous_sqr_value: " << previous_value, 4 );
              D prp_value = ( root_coords[ 0 ] - adj_coords[ 0 ] ) * ( root_coords[ 0 ] - adj_coords[ 0 ] ) +
                ( root_coords[ 1 ] - adj_coords[ 1 ] ) * ( root_coords[ 1 ] - adj_coords[ 1 ] );
              COMMENT( "Updating value.", 3 );
              if( previous_value > prp_value ) {
                sqr_value[ adj_index ] = prp_value;
                root[ adj_index ] = root[ src_index ];
                queue.Update( adj_index, previous_value, sqr_value[ adj_index ] );
              }
            }
          }
        }
      }
      for( size_t pxl = 0; pxl < size; ++pxl )
        value[ pxl ] = std::sqrt( sqr_value[ pxl ] );
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
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR(
                         "Image, window end, and/or window size dimensions do not match." ) );
      throw( std::logic_error( msg ) );
    }
  }


  template< class D >
  Image< D > Transform::EDT( const Image< D > &border, const Image< D > &mask ) {
    try {
      COMMENT( "Creating images for processing.", 0 );
      size_t size = border.size( );
      Image< D > value( border.Dim( ), border.PixelSize( ) );
      COMMENT( "Square value kept for faster comparison between values.", 0 );
      Image< int > sqr_value( border.Dim( ), border.PixelSize( ) );
      Image< int > root( border.Dim( ), border.PixelSize( ) );
      COMMENT( "Creating adjacency.", 0 );
      Adjacency adj( AdjacencyType::HyperSpheric( 1.8f, border.Dims( ) ) );
      AdjacencyIterator adj_itr( value, adj );
      size_t adj_size = adj.size( );
      COMMENT( "Setting seed pixels.", 0 );
      COMMENT( "Creating bucket queue with the longest distance in the image.", 0 );
      DistanceBucketQueue queue( size, border.size( 0 ) * border.size( 0 ) + border.size( 1 ) * border.size( 1 ) + 
                                 border.size( 2 ) * border.size( 2 ) + 1 );
      for( size_t pxl = 0; pxl < size; ++pxl ) {
        if( mask[ pxl ] != 0 ) {
          if( border[ pxl ] != 0 ) {
            root[ pxl ] = pxl;
            queue.Insert( pxl, sqr_value[ pxl ] );
          }
          else
            sqr_value[ pxl ] = std::numeric_limits< D >::max( ); // Mudado de D para float. Voltei aqora...
        }
      }
      COMMENT( "Running Image IFT. Queue: " << ( queue.Empty( ) ? "empty" : "not empty" ), 0 );
      Vector< size_t > adj_coords( 3 );
      size_t adj_index;
      if( border.Dims( ) == 3 ) {
        while( !queue.Empty( ) ) {
          COMMENT( "Initializing removed data.", 4 );
          size_t src_index = queue.Remove( );
          Vector< size_t > src_coords( border.Coordinates( src_index ) );
          Vector< size_t > root_coords( border.Coordinates( root[ src_index ] ) );
          size_t src_value = sqr_value[ src_index ];
          COMMENT( "Index: " << src_index << ", sqr_value: " << src_value, 4 );
          queue.Finished( src_index );
          size_t previous_value;
          for( size_t adj_pos = 0; adj_pos < adj_size; ++adj_pos ) {
            if( ( adj_itr.AdjVct( src_coords, adj_pos, adj_coords ) ) && 
                ( adj_index = src_index + adj_itr.Displacement( adj_pos ) ) && 
                ( previous_value = sqr_value[ adj_index ] ) && ( src_value < previous_value ) ) {
              COMMENT( "Conquering: " << adj_index << " with previous_sqr_value: " << previous_value, 4 );
              D prp_value = ( root_coords[ 0 ] - adj_coords[ 0 ] ) * ( root_coords[ 0 ] - adj_coords[ 0 ] ) +
                ( root_coords[ 1 ] - adj_coords[ 1 ] ) * ( root_coords[ 1 ] - adj_coords[ 1 ] ) + 
                ( root_coords[ 2 ] - adj_coords[ 2 ] ) * ( root_coords[ 2 ] - adj_coords[ 2 ] );
              COMMENT( "Updating value.", 3 );
              if( previous_value > prp_value ) {
                sqr_value[ adj_index ] = prp_value;
                root[ adj_index ] = root[ src_index ];
                queue.Update( adj_index, previous_value, sqr_value[ adj_index ] );
              }
            }
          }
        }
      }
      // Optimized 2D version. It is around 6% faster.
      else {
        while( !queue.Empty( ) ) {
          COMMENT( "Initializing removed data.", 4 );
          size_t src_index = queue.Remove( );
          Vector< size_t > src_coords( border.Coordinates2( src_index ) );
          Vector< size_t > root_coords( border.Coordinates2( root[ src_index ] ) );
          size_t src_value = sqr_value[ src_index ];
          COMMENT( "Index: " << src_index << ", sqr_value: " << src_value, 4 );
          queue.Finished( src_index );
          size_t previous_value;
          for( size_t adj_pos = 0; adj_pos < adj_size; ++adj_pos ) {
            if( ( adj_itr.AdjVct2( src_coords, adj_pos, adj_coords ) ) && 
                ( adj_index = src_index + adj_itr.Displacement( adj_pos ) ) && 
                ( previous_value = sqr_value[ adj_index ] ) && ( src_value < previous_value ) ) {
              COMMENT( "Conquering: " << adj_index << " with previous_sqr_value: " << previous_value, 4 );
              D prp_value = ( root_coords[ 0 ] - adj_coords[ 0 ] ) * ( root_coords[ 0 ] - adj_coords[ 0 ] ) +
                ( root_coords[ 1 ] - adj_coords[ 1 ] ) * ( root_coords[ 1 ] - adj_coords[ 1 ] );
              COMMENT( "Updating value.", 3 );
              if( previous_value > prp_value ) {
                sqr_value[ adj_index ] = prp_value;
                root[ adj_index ] = root[ src_index ];
                queue.Update( adj_index, previous_value, sqr_value[ adj_index ] );
              }
            }
          }
        }
      }
      for( size_t pxl = 0; pxl < size; ++pxl )
        value[ pxl ] = std::sqrt( sqr_value[ pxl ] );
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
