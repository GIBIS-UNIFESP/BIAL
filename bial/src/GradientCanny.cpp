/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/21 
 * @brief Computes Canny gradient. 
 */

#ifndef BIALGRADIENTCANNY_C
#define BIALGRADIENTCANNY_C

#include "GradientCanny.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_GradientCanny )
#define BIAL_EXPLICIT_GradientCanny
#endif
#if defined ( BIAL_EXPLICIT_GradientCanny ) || ( BIAL_IMPLICIT_BIN )

#ifdef BIAL_DEBUG
#include "FileImage.hpp"
#endif
#include "FilteringGaussian.hpp"
#include "GradientSobel.hpp"
#include "Histogram.hpp"
#include "Image.hpp"
#include "SegmentationHysteresis.hpp"
#include "Signal.hpp"
#include "SignalAccumulate.hpp"
#include "SignalNormalize.hpp"

namespace Bial {

  template< class D >
  Image< D > Gradient::Canny( const Image< D > &suppressed_sobel, float lower_threshold, float higher_threshold ) {
    try {
      COMMENT( "Checking if thresholds make sense.", 0 );
      if( ( lower_threshold > higher_threshold ) || ( lower_threshold < 0.0 ) || ( higher_threshold > 1.0 ) ) {
        std::string msg( BIAL_ERROR( "Invalid thresholds. Required: 0.0 <= lower <= higher <= 1.0. Given: lower:" +
                                     std::to_string( lower_threshold ) + ", higher: " +
                                     std::to_string( higher_threshold ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      if( suppressed_sobel.Maximum( ) == 0 ) {
        BIAL_WARNING( "Suppressed sobel is NULL." );
        return( Image< D >( suppressed_sobel ) );
      }
      COMMENT( "Computing normalized accumulated histogram to get low and high intensity thresholds.", 0 );
      Signal hist = SignalType::ZeroStartHistogram( suppressed_sobel );
      hist[ 0 ] = 0.0;
      SignalOp::Normalize( hist );
      SignalOp::Accumulate( hist );
      COMMENT( "Getting lower bin for threshold.", 0 );
      size_t lower_bin = 1;
      size_t bin = 2;
      while( ( bin < hist.size( ) ) &&
             ( ( std::abs( lower_threshold - hist[ bin ] ) <= std::abs( lower_threshold - hist[ lower_bin ] ) ) ) ) {
        lower_bin = bin;
        ++bin;
      }
      COMMENT( "Getting higher bin for threshold.", 0 );
      size_t higher_bin = lower_bin;
      while( ( bin < hist.size( ) ) &&
             ( std::abs( higher_threshold - hist[ bin ] ) <= std::abs( higher_threshold - hist[ higher_bin ] ) ) ) {
        higher_bin = bin;
        ++bin;
      }
      COMMENT( "Getting lower and higher intensities.", 0 );
      D lower_intensity = hist.Data( lower_bin );
      D higher_intensity = hist.Data( higher_bin );
      // std::cout << hist << std::endl;
      // std::cout << "lower_bin: " << lower_bin << ", higher_bin: " << higher_bin << std::endl;
      // std::cout << "lower_intensity: " << lower_intensity << ", higher_intensity: " << higher_intensity << std::endl;

      COMMENT( "Computing the hysteresis and returning the result.", 0 );
      return( Segmentation::HighHysteresis( suppressed_sobel, lower_intensity, higher_intensity ) );
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
  Image< D > Gradient::Canny( const Image< D > &img, float lower_threshold, float higher_threshold, float sigma ) {
    try {
      COMMENT( "Checking if thresholds make sense.", 0 );
      if( ( lower_threshold > higher_threshold ) || ( lower_threshold < 0.0 ) || ( higher_threshold > 1.0 ) ) {
        std::string msg( BIAL_ERROR( "Invalid thresholds. Required: 0.0 <= lower <= higher <= 1.0. Given: lower:" +
                                     std::to_string( lower_threshold ) + ", higher: " +
                                     std::to_string( higher_threshold ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Computing the filtered image.", 0 );
      Image< D > smooth( Filtering::Gaussian( img, 2.0, sigma ) );
      COMMENT( "Computing Sobel, suppressing non-edges pixels.", 0 );
      Image< D > suppressed_sobel( Gradient::NonMaxSobelSuppression( smooth ) );
      // DEBUG_WRITE( suppressed_sobel, "suppressed_sobel", 1 );
      return( Gradient::Canny( suppressed_sobel, lower_threshold, higher_threshold ) );
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
  Image< D > Gradient::NonMaxSobelSuppression( const Image< D > &img ) {
    try {
      Image< D > mag( img );
      Image< int > dir( img );
      Gradient::Sobel( img, &mag, &dir );
      return( Gradient::NonMaxSobelSuppression( mag, dir ) );
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
  Image< D > Gradient::NonMaxSobelSuppression( const Image< D > &magnitude, const Image< int > &direction ) {
    try {
      if( magnitude.Dims( ) != direction.Dims( ) ) {
        std::string msg( BIAL_ERROR( "Magnitude and direction images contain distinct dimensions." ) );
        throw( std::logic_error( msg ) );
      }
      size_t dimensions = magnitude.Dims( );
      Image< D > res( magnitude.Dim( ), magnitude.PixelSize( ) );
      res.Set( 0.0 );
      COMMENT( "Setting displacements.", 1 );
      Vector< size_t > max_coord( magnitude.Dim( ) );
      max_coord -= 1;
      Vector< size_t > coord( max_coord );
      Vector< size_t > direct_adj( coord );
      Vector< size_t > opposed_adj( coord );
      if( dimensions == 2 ) {
        COMMENT( "2D image.", 3 );
        for( coord[ 1 ] = 1; coord[ 1 ] < max_coord[ 1 ]; ++coord[ 1 ] ) {
          for( coord[ 0 ] = 1; coord[ 0 ] < max_coord[ 0 ]; ++coord[ 0 ] ) {
            COMMENT( "Computing adjacents for suppression.", 4 );
            direct_adj[ 0 ] = coord[ 0 ];
            direct_adj[ 1 ] = coord[ 1 ];
            opposed_adj[ 0 ] = coord[ 0 ];
            opposed_adj[ 1 ] = coord[ 1 ];
            if( magnitude( coord[ 0 ], coord[ 1 ] ) != 0 ) {
              int pxl_direction = direction( coord[ 0 ], coord[ 1 ] );
              for( int dms = dimensions - 1; dms >= 0; --dms ) {
                int direct_code = std::pow( 2, dms * 2 );
                int opposed_code = std::pow( 2, dms * 2 + 1 );
                if( pxl_direction >= opposed_code ) {
                  --direct_adj[ dms ];
                  ++opposed_adj[ dms ];
                  pxl_direction -= opposed_code;
                }
                if( pxl_direction >= direct_code ) {
                  ++direct_adj[ dms ];
                  --opposed_adj[ dms ];
                  pxl_direction -= direct_code;
                }
              }
              COMMENT( "If pxl is a maximum do not suppress.", 4 );
              if( ( magnitude( coord ) > magnitude( direct_adj ) ) && 
                  ( magnitude( coord ) > magnitude( opposed_adj ) ) ) {
                res( coord ) = magnitude( coord );
              }
            }
          }
        }
      }
      else { // if( dimensions == 3 ) {
        COMMENT( "3D image.", 3 );
        for( coord[ 2 ] = 1; coord[ 2 ] < max_coord[ 2 ]; ++coord[ 2 ] ) {
          for( coord[ 1 ] = 1; coord[ 1 ] < max_coord[ 1 ]; ++coord[ 1 ] ) {
            for( coord[ 0 ] = 1; coord[ 0 ] < max_coord[ 0 ]; ++coord[ 0 ] ) {
              COMMENT( "Computing adjacents for suppression.", 4 );
              direct_adj[ 0 ] = coord[ 0 ];
              direct_adj[ 1 ] = coord[ 1 ];
              direct_adj[ 2 ] = coord[ 2 ];
              opposed_adj[ 0 ] = coord[ 0 ];
              opposed_adj[ 1 ] = coord[ 1 ];
              opposed_adj[ 2 ] = coord[ 2 ];
              if( magnitude( coord[ 0 ], coord[ 1 ], coord[ 2 ] ) != 0 ) {
                int pxl_direction = direction( coord[ 0 ], coord[ 1 ], coord[ 2 ] );
                for( int dms = dimensions - 1; dms >= 0; --dms ) {
                  int direct_code = std::pow( 2, dms * 2 );
                  int opposed_code = std::pow( 2, dms * 2 + 1 );
                  if( pxl_direction >= opposed_code ) {
                    --direct_adj[ dms ];
                    ++opposed_adj[ dms ];
                    pxl_direction -= opposed_code;
                  }
                  if( pxl_direction >= direct_code ) {
                    ++direct_adj[ dms ];
                    --opposed_adj[ dms ];
                    pxl_direction -= direct_code;
                  }
                }
                COMMENT( "If pxl is a maximum do not suppress.", 4 );
                if( ( magnitude( coord ) > magnitude( direct_adj ) ) && 
                    ( magnitude( coord ) > magnitude( opposed_adj ) ) ) {
                  res( coord ) = magnitude( coord );
                }
              }
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

#ifdef BIAL_EXPLICIT_GradientCanny


  template Image< int > Gradient::Canny( const Image< int > &suppressed_sobel, float lower_threshold, 
                                         float higher_threshold );
  template Image< int > Gradient::Canny( const Image< int > &img, float lower_threshold, float higher_threshold, 
                                         float sigma );
  template Image< int > Gradient::NonMaxSobelSuppression( const Image< int > &img );
  template Image< int > Gradient::NonMaxSobelSuppression( const Image< int > &magnitude, 
                                                          const Image< int > &direction );

  template Image< llint > Gradient::Canny( const Image< llint > &suppressed_sobel, float lower_threshold, 
                                           float higher_threshold );
  template Image< llint > Gradient::Canny( const Image< llint > &img, float lower_threshold, float higher_threshold, 
                                           float sigma );
  template Image< llint > Gradient::NonMaxSobelSuppression( const Image< llint > &img );
  template Image< llint > Gradient::NonMaxSobelSuppression( const Image< llint > &magnitude, 
                                                            const Image< int > &direction );

  template Image< float > Gradient::Canny( const Image< float > &suppressed_sobel, float lower_threshold, 
                                           float higher_threshold );
  template Image< float > Gradient::Canny( const Image< float > &img, float lower_threshold, float higher_threshold, 
                                           float sigma );
  template Image< float > Gradient::NonMaxSobelSuppression( const Image< float > &img );
  template Image< float > Gradient::NonMaxSobelSuppression( const Image< float > &magnitude, 
                                                            const Image< int > &direction );

  template Image< double > Gradient::Canny( const Image< double > &suppressed_sobel, float lower_threshold, 
                                            float higher_threshold );
  template Image< double > Gradient::Canny( const Image< double > &img, float lower_threshold, float higher_threshold, 
                                            float sigma );
  template Image< double > Gradient::NonMaxSobelSuppression( const Image< double > &img );
  template Image< double > Gradient::NonMaxSobelSuppression( const Image< double > &magnitude,
                                                             const Image< int > &direction );

#endif

}

#endif

#endif
