 /* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Jul/12 
 * @brief: Image background segmentation. Segments a large piece of dark pixels in the background.
 */

#ifndef BIALSEGMENTATIONBACKGROUND_C
#define BIALSEGMENTATIONBACKGROUND_C

#include "SegmentationBackground.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_SegmentationBackground )
#define BIAL_EXPLICIT_SegmentationBackground
#endif
#if defined ( BIAL_EXPLICIT_SegmentationBackground ) || ( BIAL_IMPLICIT_BIN )

#include "AdjacencyRound.hpp"
#include "DiffusionFunction.hpp"
#include "Feature.hpp"
#include "FilteringAnisotropicDiffusion.hpp"
#include "GradientCanny.hpp"
#include "Image.hpp"
#include "Matrix.hpp"
#include "MedianFeature.hpp"
#include "OPFSpatialClustering.hpp"
#include "SortingSort.hpp"
#include "StatisticsStdDev.hpp"

namespace Bial {

  template< class D >
  Image< D > Segmentation::Background( const Image< D > &img ) {
    try {
      COMMENT( "Computing canny edges.", 0 );
      Image< D > canny = Gradient::Canny( img, 0.8, 0.9 );
      return( Segmentation::Background( img, canny ) );
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
  Image< D > Segmentation::Background( const Image< D > &img, const Image< D > &canny ) {
    try {
      COMMENT( "Defining minimum background cluster size for a reasonable background estimation.", 0 );
      const size_t minimum_size = 0.01 * img.size( );
      Vector< int > edges;
      for( size_t pxl = 0; pxl < canny.size( ); ++pxl ) {
        if( canny[ pxl ] != 0 ) {
          edges.push_back( img[ pxl ] );
        }
      }
      float edge_kappa = 2 * Statistics::StandardDeviation( edges );
      COMMENT( "Filtering image.", 0 );
      RobustDiffusionFunction diff_func( 1.0 );
      Image< D > filtered = Filtering::AdaptiveAnisotropicDiffusion( img, &diff_func, edge_kappa, 1.01 );
      COMMENT( "Computing median features.", 0 );
      Feature< D > feats = MedianFeature( filtered, AdjacencyType::HyperSpheric( 1.1, img.Dims( ) ), 0.5 );
      COMMENT( "Features: " << feats, 4 );
      /*
       * COMMENT( "Clustering image to separate background.", 0 );
       * Image< int > label( filtered );
       * size_t nlabels = OPF::SpectralClustering( feats, 1, 50 );
       * for( size_t elm = 0; elm < feat.Elements( ); ++elm )
       *   label[ feat.Index( elm ) ] = feat.Label( elm );
       */
      COMMENT( "Clustering image to separate background.", 0 );
      Image< int > label( filtered );
      size_t nlabels = OPF::SpatialClustering( feats, label, AdjacencyType::HyperSpheric( 4.0, label.Dims( ) ), 0.01 );
      COMMENT( "Computing average intensity of each cluster.", 0 );
      Vector< double > cluster_average( nlabels, 0.0 );
      Vector< size_t > cluster_size( nlabels, 0 );
      for( size_t pxl = 0; pxl < label.size( ); ++pxl ) {
        ++cluster_size( label[ pxl ] );
        cluster_average( label[ pxl ] ) += img[ pxl ];
      }
      for( size_t lbl = 0; lbl < nlabels; ++lbl ) {
        cluster_average( lbl ) /= cluster_size( lbl );
      }
      COMMENT( "Sorting clusters by their average intensities.", 0 );
      Vector< size_t > cluster_order = Sorting::Sort( cluster_average );
      Sorting::Sort( cluster_size, cluster_order );
      COMMENT( "Computing standard deviation of the first cluster.", 0 );
      Vector< D > dark_pxl;
      for( size_t pxl = 0; pxl < label.size( ); ++pxl ) {
        if( label[ pxl ] == 0 ) {
          dark_pxl.push_back( filtered[ pxl ] );
        }
      }
      float best_stddev = Statistics::StandardDeviation( dark_pxl );
      size_t last_dark_cluster = 0;
      COMMENT( "Computing standard deviation of the next cluster.", 0 );
      Vector< D > next_pxl( dark_pxl );
      for( size_t pxl = 0; pxl < label.size( ); ++pxl ) {
        if( label[ pxl ] == 1 ) {
          next_pxl.push_back( filtered[ pxl ] );
        }
      }
      float next_stddev = Statistics::StandardDeviation( next_pxl );
      COMMENT( "Finding the last dark cluster by minimum number of elements and decreasing standard deviation.", 0 );
      while( ( dark_pxl.size( ) < minimum_size ) || ( next_stddev < best_stddev ) ) {
        COMMENT( "dark_pxl.size( ): " << dark_pxl.size( ) << ", minimum_size: " << minimum_size << "next_stddev: "
                                      << next_stddev << ", best_stddev: " << best_stddev << ".", 3 );
        ++last_dark_cluster;
        dark_pxl = next_pxl;
        best_stddev = next_stddev;
        for( size_t pxl = 0; pxl < label.size( ); ++pxl ) {
          if( label[ pxl ] == static_cast< int >( last_dark_cluster + 1 ) ) {
            next_pxl.push_back( filtered[ pxl ] );
          }
        }
        next_stddev = Statistics::StandardDeviation( next_pxl );
      }
      COMMENT( "Labeling the clusters according to their classes.", 0 );
      for( size_t pxl = 0; pxl < label.size( ); ++pxl ) {
        if( label[ pxl ] <= static_cast< int >( last_dark_cluster ) ) {
          label[ pxl ] = 1;
        }
        else {
          label[ pxl ] = 0;
        }
      }
      return( label );
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

#ifdef BIAL_EXPLICIT_SegmentationBackground

  template Image< int > Segmentation::Background( const Image< int > &img );
  template Image< int > Segmentation::Background( const Image< int > &img, const Image< int > &canny );
  template Image< llint > Segmentation::Background( const Image< llint > &img );
  template Image< llint > Segmentation::Background( const Image< llint > &img, const Image< llint > &canny );
  template Image< float > Segmentation::Background( const Image< float > &img );
  template Image< float > Segmentation::Background( const Image< float > &img, const Image< float > &canny );
  template Image< double > Segmentation::Background( const Image< double > &img );
  template Image< double > Segmentation::Background( const Image< double > &img, const Image< double > &canny );

#endif

}

#endif

#endif
