 /* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Jul/12 
 * @brief: Segmentation by intensity hysteresis.
 */

#ifndef BIALSEGMENTATIONHYSTERESIS_C
#define BIALSEGMENTATIONHYSTERESIS_C

#include "SegmentationHysteresis.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_SegmentationHysteresis )
#define BIAL_EXPLICIT_SegmentationHysteresis
#endif
#if defined ( BIAL_EXPLICIT_SegmentationHysteresis ) || ( BIAL_IMPLICIT_BIN )

#include "AdjacencyRound.hpp"
#include "AdjacencyIterator.hpp"
#include "BucketQueue.hpp"
#include "Image.hpp"

namespace Bial {

  template< class D >
  Image< D > Segmentation::HighHysteresis( const Image< D > &img, const D lower, const D higher ) {
    try {
      if( lower > higher ) {
        std::string msg( BIAL_ERROR( "Lower threshold is greater than the higher threshold. Given: lower: " +
                                     std::to_string( lower ) + ", higher: " + std::to_string( higher ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Computing higher threshold seeds.", 2 );
      Image< D > edge( img );
      BucketQueue queue( img.size( ) );
      for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
        if( img[ pxl ] < higher ) {
          edge[ pxl ] = 0;
        }
        else {
          queue.Insert( pxl, 1 );
          edge[ pxl ] = 1;
        }
      }
      COMMENT( "Propagating to connected lower threshold pixels.", 2 );
      Adjacency adj = AdjacencyType::HyperSpheric( 1.9, img.Dims( ) );
      while( !queue.Empty( ) ) {
        size_t pxl = queue.Remove( );
        AdjacencyIterator idx = begin( adj, img, pxl );
        for( ++idx; *idx != img.size( ); ++idx ) {
          size_t adj_pxl = *idx;
          if( ( img[ adj_pxl ] >= lower ) && ( edge[ adj_pxl ] == 0 ) ) {
            queue.Insert( adj_pxl, 1 );
            edge[ adj_pxl ] = 1;
          }
        }
      }
      return( edge );
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

#ifdef BIAL_EXPLICIT_SegmentationHysteresis

  template Image< int > Segmentation::HighHysteresis( const Image< int > &img, const int lower,
                                                      const int higher );
  template Image< llint > Segmentation::HighHysteresis( const Image< llint > &img, const llint lower, 
                                                        const llint higher );
  template Image< float > Segmentation::HighHysteresis( const Image< float > &img, const float lower, 
                                                        const float higher );
  template Image< double > Segmentation::HighHysteresis( const Image< double > &img, const double lower, 
                                                         const double higher );

#endif

}

#endif

#endif
