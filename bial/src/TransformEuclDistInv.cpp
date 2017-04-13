/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2014/Jan/07
 * @brief Inverse Euclidean distance transform
 */

#ifndef BIALTRANSFORMEUCLDISTINV_C
#define BIALTRANSFORMEUCLDISTINV_C

#include "TransformEuclDistInv.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_TransformEuclDistInv )
#define BIAL_EXPLICIT_TransformEuclDistInv
#endif

#if defined ( BIAL_EXPLICIT_TransformEuclDistInv ) || ( BIAL_IMPLICIT_BIN )

#include "AdjacencyRound.hpp"
#include "AdjacencyIterator.hpp"
#include "GrowingBucketQueue.hpp"
#include "Image.hpp"
#include "IntensityLocals.hpp"
#include "TransformEuclDist.hpp"

#include "FileImage.hpp"

namespace Bial {

  template< class D >
  Image< D > Transform::InverseEDT( const Image< D > &border, const Image< D > &mask, int root_dist ) {
    try {
      COMMENT( "root_dist: " << root_dist, 1 );
      Image< float > value = Transform::EDT( border, mask );
      Write( value, "value.nii.gz" );
      Image< int > root( mask );
      Adjacency adj = AdjacencyType::HyperSpheric( 1.8, mask.Dims( ) );
      COMMENT( "Seeds are local maxima from value map. May be a pixel in a plateau.", 2 );
      Vector< bool > seeds( Intensity::LocalMaxima( value, mask, adj ) );
      GrowingBucketQueue queue( value.size( ), 256, false );
      for( size_t pxl = 0; pxl < mask.size( ); ++pxl ) {
        if( seeds[ pxl ] == true ) {
          queue.Insert( pxl, value[ pxl ] );
        }
      }
      Image< float > dist( value );
      size_t adj_pxl;
      size_t adj_size = adj.size( );
      AdjacencyIterator adj_itr( border, adj );
      while( !queue.Empty( ) ) {
        size_t pxl = queue.Remove( );
        if( value[ pxl ] >= root_dist ) {
          root[ pxl ] = pxl;
        }
        queue.Finished( pxl );
        for( size_t adj_idx = 0; adj_idx < adj_size; ++adj_idx ) {
          if( adj_itr.AdjIdx( pxl, adj_idx, adj_pxl ) ) {
            if( ( mask[ adj_pxl ] != 0 ) && ( queue.State( adj_pxl ) != BucketState::REMOVED ) &&
                ( dist[ pxl ] > dist[ adj_pxl ] ) ) {
              if( queue.State( adj_pxl ) == BucketState::NOT_VISITED ) {
                queue.Insert( adj_pxl, dist[ adj_pxl ] );
              }
              dist[ adj_pxl ] = dist[ pxl ];
              root[ adj_pxl ] = root[ pxl ];
            }
          }
        }
      }
      return( root );
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

#ifdef BIAL_EXPLICIT_TransformEuclDistInv

  template Image< int > Transform::InverseEDT( const Image< int > &border, const Image< int > &mask, int root_dist );
  template Image< llint > Transform::InverseEDT( const Image< llint > &border, const Image< llint > &mask, 
                                                 int root_dist );
  template Image< float > Transform::InverseEDT( const Image< float > &border, const Image< float > &mask,
                                                 int root_dist );
  template Image< double > Transform::InverseEDT( const Image< double > &border, const Image< double > &mask,
                                                  int root_dist );

#endif

}

#endif

#endif
