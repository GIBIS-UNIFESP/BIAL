 /* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Jul/12 
 * @brief: Segmentation of object border.
 */

#ifndef BIALSEGMENTATIONBORDER_C
#define BIALSEGMENTATIONBORDER_C

#include "SegmentationBorder.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_SegmentationBorder )
#define BIAL_EXPLICIT_SegmentationBorder
#endif
#if defined ( BIAL_EXPLICIT_SegmentationBorder ) || ( BIAL_IMPLICIT_BIN )

#include "Adjacency.hpp"
#include "AdjacencyIterator.hpp"
#include "Image.hpp"

namespace Bial {

  template< class D >
  Image< D > Segmentation::BorderPixels( const Image< D > &img, const Adjacency &adj ) {
    try {
      Image< D > res( img );
      AdjacencyIterator adj_itr( img, adj );
      size_t adj_size = adj.size( );
      size_t adj_pxl;
      res.Set( 0 );
      COMMENT( "Setting border pixels.", 0 );
      for( size_t pxl = 0; pxl < res.size( ); ++pxl ) {
        for( size_t idx = 0; idx < adj_size; ++idx ) {
          if( ( adj_itr.*adj_itr.AdjIdx )( pxl, idx, adj_pxl ) ) {
            if( img[ pxl ] != img[ adj_pxl ] ) {
              res[ pxl ] = img[ pxl ];
              break;
            }
          }
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

#ifdef BIAL_EXPLICIT_SegmentationBorder

  template Image< int > Segmentation::BorderPixels( const Image< int > &img, const Adjacency &adj );
  template Image< llint > Segmentation::BorderPixels( const Image< llint > &img, const Adjacency &adj );
  template Image< float > Segmentation::BorderPixels( const Image< float > &img, const Adjacency &adj );
  template Image< double > Segmentation::BorderPixels( const Image< double > &img, const Adjacency &adj );

#endif

}

#endif

#endif
