/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2015/Sep/22
 * @brief BIAL_Superpixel class. Functions to call SLIC superpixel routines.
 */

#ifndef BIALSUPERPIXEL_C
#define BIALSUPERPIXEL_C

#include "Superpixel.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_Superpixel )
#define BIAL_EXPLICIT_Superpixel
#endif

#if defined ( BIAL_EXPLICIT_Superpixel ) || ( BIAL_IMPLICIT_BIN )

#include "AdjacencyRound.hpp"
#include "AdjacencyIterator.hpp"
#include "Color.hpp"
#include "Image.hpp"

namespace Bial {

  Image< int > Superpixel::SuperPixel( Image< Color > &img, int sup_pixel_size, double compactness ) {
    if( sup_pixel_size < 2 ) {
      std::string msg( BIAL_ERROR( "Must specify superpixel with more than 1 pixel. Given: " + 
                                   std::to_string( sup_pixel_size ) + "." ) );
      throw( std::logic_error( msg ) );
    }
    if( ( img.size( ) < 2 ) || ( img.Dims( ) != 2 ) ) {
      std::string msg( BIAL_ERROR( "Input image must be 2D, with more than 1 pixel. Given dimensions: " + 
                                   std::to_string( img.Dims( ) ) + ", pixels: " + std::to_string( img.size( ) ) + "." ) );
      throw( std::logic_error( msg ) );
    }

    COMMENT( "Preparing parameters to run SLIC.", 0 );
    int width = img.size( 1 );
    int height = img.size( 2 );
    Image< int > label( width, height );
    label.Set( -1 ); // For slic initial value.
    int *klabels = label.Data( );
    int numlabels;

    COMMENT( "Running SLIC.", 0 );
    SLIC segment;
    segment.DoSuperpixelSegmentation_ForGivenSuperpixelSize( reinterpret_cast< uint* >( img.Data( ) ), width, height, 
                                                             klabels, numlabels, sup_pixel_size, compactness );
  
    COMMENT( "Draw boundaries around segments.", 0 );
    segment.DrawContoursAroundSegments( reinterpret_cast< uint* >( img.Data( ) ), klabels, width, height, 0xff0000 );
  
    return( label );
  }

  template< class D >
  Image< int > Superpixel::SuperVoxel( Image< D > &img, int sup_pixel_size, double compactness ) {
    if( sup_pixel_size < 2 ) {
      std::string msg( BIAL_ERROR( "Must specify superpixel with more than 1 pixel. Given: " + 
                                   std::to_string( sup_pixel_size ) + "." ) );
      throw( std::logic_error( msg ) );
    }
    if( ( img.size( ) < 2 ) || ( img.Dims( ) != 3 ) ) {
      std::string msg( BIAL_ERROR( "Input image must be 3D, with more than 1 pixel. Given dimensions: " + 
                                   std::to_string( img.Dims( ) ) + ", pixels: " + std::to_string( img.size( ) ) + "." ) );
      throw( std::logic_error( msg ) );
    }
    COMMENT( "Preparing parameters to run SLIC.", 0 );
    int width = img.size( 0 );
    int height = img.size( 1 );
    int depth = img.size( 2 );
    int slc_size = width * height;
    uint **ubuffvec = NULL;
    int **klabels = NULL;
    int numlabels;
    COMMENT( "Converting image to ARGB video format.", 0 );
    uchar *c;
    ubuffvec = new uint*[ depth ];
    klabels = new int*[ depth ];
    for( int slc = 0; slc < depth; ++slc ) {
      ubuffvec[ slc ] = new uint[ slc_size ];
      klabels[ slc ] = new int[ slc_size ];
      for( int pxl = 0; pxl < slc_size; ++pxl ) {
        c = reinterpret_cast< uchar* >( &ubuffvec[ slc ][ pxl ] );
        c[ 3 ] = static_cast< uchar >( img[ pxl + slc * slc_size ] );
        c[ 2 ] = static_cast< uchar >( img[ pxl + slc * slc_size ] );
        c[ 1 ] = static_cast< uchar >( img[ pxl + slc * slc_size ] );
        c[ 0 ] = 0;
      }
    }
    COMMENT( "Running SLIC.", 0 );
    SLIC segment;
    segment.DoSupervoxelSegmentation( ubuffvec, width, height, depth, klabels, numlabels, sup_pixel_size, compactness );
    COMMENT( "Converting result to Image.", 0 );
    Adjacency adj( AdjacencyType::Spheric( 1.1 ) );
    Image< int > label( img.Dim( ), img.PixelSize( ) );
    AdjacencyIterator adj_itr( img, adj );
    size_t adj_size = adj.size( );
    size_t img_size = img.size( );
    size_t adj_pxl;
    for( int slc = 0; slc < depth; ++slc ) {
      for( int pxl = 0; pxl < slc_size; ++pxl )
        label[ pxl + slc * slc_size ] = klabels[ slc ][ pxl ];
    }
    for( size_t pxl = 0; pxl < img_size; ++pxl ) {
      bool border = false;
      for( size_t idx = 0; idx < adj_size; ++idx ) {
        if( ( ( adj_itr.*adj_itr.AdjIdx )( pxl, idx, adj_pxl ) ) && ( label[ pxl ] != label[ adj_pxl ] ) ) {
          border = true;
          break;
        }
      }
      if( border )
        img[ pxl ] = 1;
      else
        img[ pxl ] = 0;
    }
    COMMENT( "Destroying structures.", 0 );
    if( klabels ) {
      for( int slc = 0; slc < depth; ++slc ) {
        delete[ ] klabels[ slc ];
      }
      delete[ ] klabels;
    }
    if( ubuffvec ) {
      for( int slc = 0; slc < depth; ++slc ) {
        delete[ ] ubuffvec[ slc ];
      }
      delete[ ] ubuffvec;
    }
    return( label );
  }

#ifdef BIAL_EXPLICIT_Superpixel

  template Image< int > Superpixel::SuperVoxel( Image< int > &img, int sup_pixel_size, double compactness );
  template Image< int > Superpixel::SuperVoxel( Image< llint > &img, int sup_pixel_size, double compactness );
  template Image< int > Superpixel::SuperVoxel( Image< float > &img, int sup_pixel_size, double compactness );
  template Image< int > Superpixel::SuperVoxel( Image< double > &img, int sup_pixel_size, double compactness );

#endif

}

#endif

#endif
