/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2013/Jul/12 
 * @brief Mean filter.
 */

#ifndef BIALFILTERINGMEAN_C
#define BIALFILTERINGMEAN_C

#include "FilteringMean.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_FilteringMean )
#define BIAL_EXPLICIT_FilteringMean
#endif

#if defined ( BIAL_EXPLICIT_FilteringMean ) || ( BIAL_IMPLICIT_BIN )

#include "AdjacencyRound.hpp"
#include "AdjacencyIterator.hpp"
#ifdef BIAL_DEBUG
#include "FileImage.hpp"
#endif
#include "Image.hpp"

namespace Bial {

  template< class D >
  Image< D > Filtering::Mean( const Image< D > &img, float radius ) {
    Image< D > res( img );
    Adjacency adj = AdjacencyType::HyperSpheric( radius, img.Dims( ) );
    AdjacencyIterator adj_itr( img, adj );
    size_t adj_size = adj.size( );
    size_t img_size = img.size( );
    size_t adj_pxl;
    for( size_t pxl = 0; pxl < img_size; ++pxl ) {
      size_t total_voxels = 0;
      unsigned long long sum = 0;
      for( size_t idx = 0; idx < adj_size; ++idx ) {
        if( adj_itr.AdjIdx( pxl, idx, adj_pxl ) ) {
          ++total_voxels;
          sum += img[ adj_pxl ];
        }
      }
      res[ pxl ] = sum / total_voxels;
    }
    return( res );
  }

  template< class D >
  Image< D > Filtering::Mean( const Image< D > &img, const Image< D > &msk, float radius ) {
    Image< D > res( img );
    Adjacency adj = AdjacencyType::HyperSpheric( radius, img.Dims( ) );
    AdjacencyIterator adj_itr( img, adj );
    size_t adj_size = adj.size( );
    size_t img_size = img.size( );
    size_t adj_pxl;
    for( size_t pxl = 0; pxl < img_size; ++pxl ) {
      if( msk[ pxl ] != 0 ) {
        size_t total_voxels = 0;
        unsigned long long sum = 0;
        for( size_t idx = 0; idx < adj_size; ++idx ) {
          if( ( adj_itr.AdjIdx( pxl, idx, adj_pxl ) ) && ( msk[ adj_pxl ] != 0 ) ) {
            ++total_voxels;
            sum += img[ adj_pxl ];
          }
        }
        res[ pxl ] = sum / total_voxels;
      }
    }
    return( res );
  }

#ifdef BIAL_EXPLICIT_FilteringMean

  template Image< int > Filtering::Mean( const Image< int > &img, float radius );
  template Image< int > Filtering::Mean( const Image< int > &img, const Image< int > &msk, float radius );
  template Image< llint > Filtering::Mean( const Image< llint > &img, float radius );
  template Image< llint > Filtering::Mean( const Image< llint > &img, const Image< llint > &msk, float radius );
  template Image< float > Filtering::Mean( const Image< float > &img, float radius );
  template Image< float > Filtering::Mean( const Image< float > &img, const Image< float > &msk, float radius );
  template Image< double > Filtering::Mean( const Image< double > &img, float radius );
  template Image< double > Filtering::Mean( const Image< double > &img, const Image< double > &msk, float radius );
  
#endif

}


#endif

#endif
