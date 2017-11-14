 /* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Jul/12 
 * @brief: Segmentation of 3D surface.
 */

#ifndef BIALSEGMENTATIONSURFACE_C
#define BIALSEGMENTATIONSURFACE_C

#include "SegmentationSurface.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_SegmentationSurface )
#define BIAL_EXPLICIT_SegmentationSurface
#endif
#if defined ( BIAL_EXPLICIT_SegmentationSurface ) || ( BIAL_IMPLICIT_BIN )

#include "AdjacencyRound.hpp"
#include "AdjacencyIterator.hpp"
#include "Image.hpp"

namespace Bial {

  template< class D >
  Image< D > Segmentation::SurfaceBorderPixels( const Image< D > &img ) {
    try {
      if( img.Dims( ) != 3 ) {
        std::string msg( BIAL_ERROR( "Required 3D image. Given: " + std::to_string( img.Dims( ) ) + "D." ) );
        throw( std::logic_error( msg ) );
      }
      Image< D > res( img );
      res.Set( 0 );
      Adjacency adj( AdjacencyType::Ellipsoid( 1.5 * img.PixelSize( 0 ), 1.5 * img.PixelSize( 1 ),
                                               1.5 * img.PixelSize( 2 ) ) );
      AdjacencyIterator adj_itr( img, adj );
      size_t adj_size = adj.size( );
      size_t adj_pxl;
      COMMENT( "Setting border pixels.", 0 );
      for( size_t pxl = 0; pxl < res.size( ); ++pxl ) {
        if( img[ pxl ] != 0 ) {
          size_t adjs = 0;
          for( size_t idx = 1; idx < adj_size; ++idx ) {
            if( adj_itr.AdjIdx( pxl, idx, adj_pxl ) ) {
              if( img[ adj_pxl ] != 0 )
                ++adjs;
            }
          }
          if( adjs < 3 ) {
            res[ pxl ] = img[ pxl ];
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

#ifdef BIAL_EXPLICIT_SegmentationSurface

  template Image< int > Segmentation::SurfaceBorderPixels( const Image< int > &img );
  template Image< llint > Segmentation::SurfaceBorderPixels( const Image< llint > &img );
  template Image< float > Segmentation::SurfaceBorderPixels( const Image< float > &img );
  template Image< double > Segmentation::SurfaceBorderPixels( const Image< double > &img );

#endif

}

#endif

#endif
