/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/**
 * @date 2013/Jul/12 
 * @brief Image filtering methods.
 */

#ifndef BIALFILTERINGOPTIMALANISOTROPICDIFFUSION_C
#define BIALFILTERINGOPTIMALANISOTROPICDIFFUSION_C

#include "FilteringOptimalAnisotropicDiffusion.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_FilteringOptimalAnisotropicDiffusion )
#define BIAL_EXPLICIT_FilteringOptimalAnisotropicDiffusion
#endif

#if defined ( BIAL_EXPLICIT_FilteringOptimalAnisotropicDiffusion ) || ( BIAL_IMPLICIT_BIN )

#include "FilteringAnisotropicDiffusion.hpp"
#include "DiffusionFunction.hpp"
#ifdef BIAL_DEBUG
#include "FileImage.hpp"
#endif
#include "GradientCanny.hpp"
#include "Image.hpp"
#include "SegmentationBackground.hpp"
#include "StatisticsStdDev.hpp"

namespace Bial {

  template< class D >
  float Filtering::EdgeRegionKappa( const Image< D > &source, const Image< D > &mask, 
                                    const DiffusionFunction *diff_func, float radius ) {
    try {
      float best_proportion = 1.01;

      COMMENT( "Get initial kappa, minimum standard deviation of flat region, and maximum standard deviation of edge "
               << "region.", 3 );
      Vector< int > edges;
      for( size_t pxl = 0; pxl < mask.size( ); ++pxl ) {
        if( mask[ pxl ] != 0 ) {
          edges.push_back( source[ pxl ] );
        }
      }
      float max_std = Statistics::StandardDeviation( edges );

      COMMENT( "Estimate best edge kappa based on the maximum standard deviation in a binary search.", 3 );
      float kappa = 2 * max_std;
      float step = kappa / 2.0;
      Image< D > filtered = Filtering::AnisotropicDiffusion( source, diff_func, kappa, 1, radius );
      size_t elm = 0;
      for( size_t pxl = 0; pxl < mask.size( ); ++pxl ) {
        if( mask[ pxl ] != 0 ) {
          edges( elm ) = filtered[ pxl ];
          ++elm;
        }
      }
      float best_std = Statistics::StandardDeviation( edges );

      COMMENT( "Binary search for the best kappa.", 3 );
      while( step > 2.0 ) {

        COMMENT( "Searching to the right.", 4 );
        filtered = Filtering::AnisotropicDiffusion( source, diff_func, kappa + step, 1, radius );
        for( size_t pxl = 0, elm = 0; pxl < mask.size( ); ++pxl ) {
          if( mask[ pxl ] != 0 ) {
            edges( elm ) = filtered[ pxl ];
            ++elm;
          }
        }
        float std = Statistics::StandardDeviation( edges );
        if( std::fabs( std - max_std / best_proportion ) <= std::fabs( best_std - max_std / best_proportion ) ) {
          best_std = std;
          kappa = kappa + step;
        }
        else {

          COMMENT( "Searching to the left.", 4 );
          filtered = Filtering::AnisotropicDiffusion( source, diff_func, kappa - step, 1, radius );
          for( size_t pxl = 0, elm = 0; pxl < mask.size( ); ++pxl ) {
            if( mask[ pxl ] != 0 ) {
              edges( elm ) = filtered[ pxl ];
              ++elm;
            }
          }
          std = Statistics::StandardDeviation( edges );
          if( std::fabs( std - max_std / best_proportion ) <= std::fabs( best_std - max_std / best_proportion ) ) {
            best_std = std;
            kappa = kappa - step;
          }
        }
        step /= 2.0;
      }
      COMMENT( "edge std: " << best_std << ", edge kappa: " << kappa, 3 );

      return( kappa );
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
  float Filtering::FlatRegionKappa( const Image< D > &source, const Image< D > &mask, 
                                    const DiffusionFunction *diff_func, float radius, float kappa ) {
    try {
      float best_proportion = 1.01;

      COMMENT( "Get initial kappa, minimum standard deviation of flat region, and maximum standard deviation of edge "
               << "region.", 3 );
      Vector< int > backg;
      for( size_t pxl = 0; pxl < mask.size( ); ++pxl ) {
        if( mask[ pxl ] != 0 ) {
          backg.push_back( source[ pxl ] );
        }
      }
      float min_std = Statistics::StandardDeviation( backg );

      COMMENT( "Estimate best kappa for flat region based on the minimum standard deviation in a binary search.", 3 );
      kappa = kappa / 2.0;
      float step = kappa / 2.0;
      Image< D > filtered = Filtering::AnisotropicDiffusion( source, diff_func, kappa, 1, radius );
      size_t elm = 0;
      for( size_t pxl = 0; pxl < mask.size( ); ++pxl ) {
        if( mask[ pxl ] != 0 ) {
          backg( elm ) = filtered[ pxl ];
          ++elm;
        }
      }
      float best_std = Statistics::StandardDeviation( backg );

      COMMENT( "Binary search for the best kappa.", 3 );
      while( step > 2.0 ) {

        COMMENT( "Searching left.", 4 );
        filtered = Filtering::AnisotropicDiffusion( source, diff_func, kappa - step, 1, radius );
        for( size_t pxl = 0, elm = 0; pxl < mask.size( ); ++pxl ) {
          if( mask[ pxl ] != 0 ) {
            backg( elm ) = filtered[ pxl ];
            ++elm;
          }
        }
        float std = Statistics::StandardDeviation( backg );
        if( ( std < best_std ) ||
            ( std::fabs( std - best_proportion * min_std ) <= std::fabs( best_std - best_proportion * min_std ) ) ) {
          best_std = std;
          kappa = kappa - step;
          if( min_std > std ) {
            min_std = std;
          }
        }
        else {

          COMMENT( "Searching right.", 4 );
          filtered = Filtering::AnisotropicDiffusion( source, diff_func, kappa + step, 1, radius );
          for( size_t pxl = 0, elm = 0; pxl < mask.size( ); ++pxl ) {
            if( mask[ pxl ] != 0 ) {
              backg( elm ) = filtered[ pxl ];
              ++elm;
            }
          }
          std = Statistics::StandardDeviation( backg );
          if( std::fabs( std - best_proportion * min_std ) < std::fabs( best_std - best_proportion * min_std ) ) {
            best_std = std;
            kappa = kappa + step;
          }
        }
        step /= 2.0;
      }
      COMMENT( "flat std: " << best_std << ", flat kappa: " << kappa, 3 );

      return( kappa );
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
  Image< D > Filtering::OptimalAnisotropicDiffusion( Image< D > img, const DiffusionFunction *diff_func, float radius,
                                                     float conservativeness ) {
    try {
      if( ( conservativeness < 0.0 ) || ( conservativeness > 1.0 ) ) {
        std::string msg( BIAL_ERROR( "Invalid conservativeness paramter. Expected: [0.0, 1.0]. Given: " +
                                     std::to_string( conservativeness ) ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Computing canny gradient with 0.8, 0.9 hysteresis.", 0 );
      Image< D > canny = Gradient::Canny( img, 0.8, 0.9 );
      COMMENT( "Segmenting the background for a planar region.", 0 );
      Image< D > backg = Segmentation::Background( img, canny );
      COMMENT( "Filtering with optimum anisotropic diffusion filter.", 0 );
      return( Filtering::OptimalAnisotropicDiffusion( img, diff_func, radius, conservativeness, canny, backg ) );
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
  Image< D > Filtering::OptimalAnisotropicDiffusion( Image< D > img, const DiffusionFunction *diff_func, float radius,
                                                     float conservativeness, const Image< D > &canny, 
                                                     const Image< D > &backg ) {
    try {
      if( img.Dims( ) != canny.Dims( ) ) {
        std::string msg( BIAL_ERROR( "Input image and canny image dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      if( img.Dims( ) != backg.Dims( ) ) {
        std::string msg( BIAL_ERROR( "Input and background image dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      if( ( conservativeness < 0.0 ) || ( conservativeness > 1.0 ) ) {
        std::string msg( BIAL_ERROR( "Invalid conservativeness paramter. Expected: [0.0, 1.0]. Given: " +
                                     std::to_string( conservativeness ) ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Computing initial kappa using edge and flat regions.", 0 );
      float edge_kappa = EdgeRegionKappa( img, canny, diff_func, radius );
      float flat_kappa = FlatRegionKappa( img, backg, diff_func, radius, edge_kappa );
      float init_kappa = ( edge_kappa + flat_kappa ) / 2.0;
      if( edge_kappa < flat_kappa ) {
        init_kappa = edge_kappa + conservativeness * ( flat_kappa - edge_kappa );
      }
      COMMENT( "edge_kappa: " << edge_kappa << ", flat_kappa: " << flat_kappa << ", init_kappa: " << init_kappa, 0 );

      COMMENT( "Returning adaptive filter based on initial kappa value.", 0 );
      return( Filtering::AdaptiveAnisotropicDiffusion( img, diff_func, init_kappa, radius ) );
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

#ifdef BIAL_EXPLICIT_FilteringOptimalAnisotropicDiffusion

  template float Filtering::EdgeRegionKappa( const Image< int > &source, const Image< int > &mask,
                                             const DiffusionFunction *diff_func, float radius );
  template float Filtering::FlatRegionKappa( const Image< int > &source, const Image< int > &mask,
                                  const DiffusionFunction *diff_func, float radius, float kappa );
  template Image< int > Filtering::OptimalAnisotropicDiffusion( Image< int > img, const DiffusionFunction *diff_func,
                                                   float radius, float conservativeness );
  template Image< int > Filtering::OptimalAnisotropicDiffusion( Image< int > img, const DiffusionFunction *diff_func,
                                                   float radius, float conservativeness,
                                                   const Image< int > &canny, const Image< int > &backg );

  template float Filtering::EdgeRegionKappa( const Image< llint > &source, const Image< llint > &mask,
                                             const DiffusionFunction *diff_func, float radius );
  template float Filtering::FlatRegionKappa( const Image< llint > &source, const Image< llint > &mask,
                                  const DiffusionFunction *diff_func, float radius, float kappa );
  template Image< llint > Filtering::OptimalAnisotropicDiffusion( Image< llint > img, const DiffusionFunction *diff_func,
                                                   float radius, float conservativeness );
  template Image< llint > Filtering::OptimalAnisotropicDiffusion( Image< llint > img, const DiffusionFunction *diff_func,
                                                   float radius, float conservativeness,
                                                   const Image< llint > &canny, const Image< llint > &backg );

  template float Filtering::EdgeRegionKappa( const Image< float > &source, const Image< float > &mask,
                                             const DiffusionFunction *diff_func, float radius );
  template float Filtering::FlatRegionKappa( const Image< float > &source, const Image< float > &mask,
                                  const DiffusionFunction *diff_func, float radius, float kappa );
  template Image< float > Filtering::OptimalAnisotropicDiffusion( Image< float > img, const DiffusionFunction *diff_func,
                                                   float radius, float conservativeness );
  template Image< float > Filtering::OptimalAnisotropicDiffusion( Image< float > img, const DiffusionFunction *diff_func,
                                                   float radius, float conservativeness,
                                                   const Image< float > &canny, const Image< float > &backg );

  template float Filtering::EdgeRegionKappa( const Image< double > &source, const Image< double > &mask,
                                             const DiffusionFunction *diff_func, float radius );
  template float Filtering::FlatRegionKappa( const Image< double > &source, const Image< double > &mask,
                                  const DiffusionFunction *diff_func, float radius, float kappa );
  template Image< double > Filtering::OptimalAnisotropicDiffusion( Image< double > img, const DiffusionFunction *diff_func,
                                                   float radius, float conservativeness );
  template Image< double > Filtering::OptimalAnisotropicDiffusion( Image< double > img, const DiffusionFunction *diff_func,
                                                   float radius, float conservativeness,
                                                   const Image< double > &canny, const Image< double > &backg );

#endif

}


#endif

#endif
