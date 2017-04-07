/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2015/Jan/29 */
/* Content: Canny edge detector file. */
/* Description: Test with Gradient class. */

#include "AdjacencyRound.hpp"
#include "AdjacencyIterator.hpp"
#include "FileImage.hpp"
#include "FilteringGaussian.hpp"
#include "GradientCanny.hpp"
#include "GradientSobel.hpp"
#include "GrowingBucketQueue.hpp"
#include "Image.hpp"
#include "Matrix.hpp"
#include "SegmentationHysteresis.hpp"
#include "SegmentationIntensityThreshold.hpp"
#include "Signal.hpp"
#include "SignalKeyPoints.hpp"
#include "SignalMedianFilter.hpp"

using namespace std;
using namespace Bial;

std::mutex gradient_auto_canny_mutex;

template< class D >
void DetectHighThresEdges( Image< float > &SM_HCI, Image< D > &suppressed_sobel, D max_grad, float low_thres, 
                           float high_thres, size_t low_elm, size_t high_elm ) {
  COMMENT( "Computing DeltaG_{high}(I) and DeltaG_{low,high}(I).", 3 );
  Image< float > delta_g_high( suppressed_sobel );
  delta_g_high /= max_grad;
  COMMENT( "Hysteresis:", 3 );
  Image< float > delta_g_high_low( Segmentation::HighHysteresis( delta_g_high, low_thres, high_thres ) );
  COMMENT( "Thresholding:", 3 );
  Segmentation::IntensityThreshold( delta_g_high, high_thres, 1.0f );
  gradient_auto_canny_mutex.lock( );
  SM_HCI += ( delta_g_high_low - delta_g_high );
  gradient_auto_canny_mutex.unlock( );
}

template< class D >
Image< D > AutoCanny( const Image< D > &img, float lower_threshold, float higher_threshold, float sigma ) {
  try {
    COMMENT( "Checking if thresholds make sense.", 1 );
    if( ( lower_threshold > higher_threshold ) || ( lower_threshold < 0.0f ) || ( higher_threshold > 1.0f ) ) {
      std::string msg( BIAL_ERROR( "Invalid thresholds. Required: 0.0 <= lower <= higher <= 1.0. Given: lower:" +
                                   std::to_string( lower_threshold ) + ", higher: " +
                                   std::to_string( higher_threshold ) + "." ) );
      throw( std::logic_error( msg ) );
    }
    COMMENT( "Computing the filtered image.", 0 );
    Image< D > smooth = Filtering::Gaussian( img, 2.0, sigma );
    COMMENT( "Computing sobel gradient.", 0 );
    Image< D > magnitude( smooth );
    Image< int > direction( smooth );
    Gradient::Sobel( smooth, &magnitude, &direction );
    size_t dimensions = magnitude.Dims( );
    size_t pixels = magnitude.size( );
    COMMENT( "Computing suppressed sobel.", 0 );
    Image< D > suppressed_sobel( magnitude.Dim( ), magnitude.PixelSize( ) );
    suppressed_sobel.Set( 0.0 );
    COMMENT( "Setting displacements.", 1 );
    Vector< size_t > max_coord( magnitude.Dim( ) );
    Vector< size_t > coord( max_coord );
    Vector< size_t > direct_adj( coord );
    Vector< size_t > opposed_adj( coord );
    if( dimensions == 2 ) {
      COMMENT( "2D image.", 3 );
      for( coord[ 1 ] = 1; coord[ 1 ] < max_coord[ 1 ] - 1; ++coord[ 1 ] ) {
        for( coord[ 0 ] = 1; coord[ 0 ] < max_coord[ 0 ] - 1; ++coord[ 0 ] ) {
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
            if( ( magnitude( coord ) > magnitude( direct_adj ) ) && 
                ( magnitude( coord ) > magnitude( opposed_adj ) ) ) {
              COMMENT( "If pxl is a maximum do not suppress.", 4 );
              suppressed_sobel( coord ) = magnitude( coord );
            }
            // else {
            //   COMMENT( "If pxl is a maximum in other direction, put in the unknown.", 4 );
            //   COMMENT( "This is the begining of the implementation of Ding 01 - PR. I did not like the results." <<
            //            "Seems to insert a lot of noise and a few interesting edge points.", 4 );
            //   direct_adj[ 0 ] = coord[ 0 ] + 1;
            //   opposed_adj[ 0 ] = coord[ 0 ] - 1;
            //   direct_adj[ 1 ] = coord[ 1 ];
            //   opposed_adj[ 1 ] = coord[ 1 ];
            //   if( ( magnitude( coord ) > magnitude( direct_adj ) ) && 
            //       ( magnitude( coord ) > magnitude( opposed_adj ) ) ) {
            //     suppressed_sobel( coord ) = magnitude( coord );
            //     continue;
            //   }
            //   direct_adj[ 1 ] = coord[ 1 ] + 1;
            //   opposed_adj[ 1 ] = coord[ 1 ] - 1;
            //   if( ( magnitude( coord ) > magnitude( direct_adj ) ) && 
            //       ( magnitude( coord ) > magnitude( opposed_adj ) ) ) {
            //     suppressed_sobel( coord ) = magnitude( coord );
            //     continue;
            //   }
            //   direct_adj[ 0 ] = coord[ 0 ];
            //   opposed_adj[ 0 ] = coord[ 0 ];
            //   if( ( magnitude( coord ) > magnitude( direct_adj ) ) && 
            //       ( magnitude( coord ) > magnitude( opposed_adj ) ) ) {
            //     suppressed_sobel( coord ) = magnitude( coord );
            //     continue;
            //   }
            //   direct_adj[ 0 ] = coord[ 0 ] - 1;
            //   opposed_adj[ 0 ] = coord[ 0 ] + 1;
            //   if( ( magnitude( coord ) > magnitude( direct_adj ) ) && 
            //       ( magnitude( coord ) > magnitude( opposed_adj ) ) ) {
            //     suppressed_sobel( coord ) = magnitude( coord );
            //   }
            // }
          }
        }
      }
    }
    else { // if( dimensions == 3 ) {
      COMMENT( "3D image.", 3 );
      for( coord[ 2 ] = 1; coord[ 2 ] < max_coord[ 2 ] - 1; ++coord[ 2 ] ) {
        for( coord[ 1 ] = 1; coord[ 1 ] < max_coord[ 1 ] - 1; ++coord[ 1 ] ) {
          for( coord[ 0 ] = 1; coord[ 0 ] < max_coord[ 0 ] - 1; ++coord[ 0 ] ) {
            COMMENT( "Computing adjacents for suppression.", 4 );
            direct_adj[ 0 ] = coord[ 0 ];
            direct_adj[ 1 ] = coord[ 1 ];
            direct_adj[ 2 ] = coord[ 2 ];
            opposed_adj[ 0 ] = coord[ 0 ];
            opposed_adj[ 1 ] = coord[ 1 ];
            opposed_adj[ 2 ] = coord[ 2 ];
            if( magnitude( coord[ 0 ], coord[ 1 ], coord[ 2 ] ) != 0 ) {
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
                suppressed_sobel( coord ) = magnitude( coord );
              }
            }
          }
        }
      }
    }
    COMMENT( "Implementation of Medina 11 - PR. Automatic hysteresis detection.", 0 );
    const size_t THRES_NUMBER = 25;
    D max_grad = suppressed_sobel.Maximum( );
    COMMENT( "max_grad: " << max_grad, 0 );
    float delta_threshold = ( higher_threshold - lower_threshold ) / THRES_NUMBER;
    COMMENT( "delta_threshold: " << delta_threshold, 0 );
    COMMENT( "Computing SM_{H_{C(I)}} from all DeltaG_{low,high}(I).", 0 );
    float low_thres = lower_threshold;
    COMMENT( "low_thres: " << low_thres, 0 );
    Image< float > SM_HCI( suppressed_sobel.Dim( ) );
    size_t thres_combinations = 0;
    try {
      Vector< std::thread > threads;
      size_t nthreads = 0;
      const size_t MAX_THREADS = 12;
      for( size_t low_elm = 0; low_elm < THRES_NUMBER; ++low_elm, low_thres += delta_threshold ) {
        float high_thres = low_thres + delta_threshold;
        COMMENT( "high_thres: " << high_thres, 3 );
        for( size_t high_elm = low_elm + 1; high_elm < THRES_NUMBER; ++high_elm, high_thres += delta_threshold ) {
          if( nthreads < MAX_THREADS ) {
            threads.push_back( std::thread( &DetectHighThresEdges< D >, std::ref( SM_HCI ), std::ref( suppressed_sobel ),
                                            max_grad, low_thres, high_thres, low_elm, high_elm ) );
            ++nthreads;
          }
          else {
            for( size_t thd = 0; thd < nthreads; ++thd ) {
              threads( thd ).join( );
              ++thres_combinations;
            }
            threads = Vector< std::thread >( );
            nthreads = 0;
          }
        }
      }
      for( size_t thd = 0; thd < nthreads; ++thd ) {
        threads( thd ).join( );
        ++thres_combinations;
      }
    }
    catch( std::exception &e ) {
      std::cout << "Warning: Failed to run in multi-thread. Exception: " << e.what( ) << std::endl;
      for( size_t low_elm = 0; low_elm < THRES_NUMBER; ++low_elm, low_thres += delta_threshold ) {
        float high_thres = low_thres + delta_threshold;
        COMMENT( "high_thres: " << high_thres, 3 );
        for( size_t high_elm = low_elm + 1; high_elm < THRES_NUMBER; ++high_elm, high_thres += delta_threshold ) {
          DetectHighThresEdges( SM_HCI, suppressed_sobel, max_grad, low_thres, high_thres, low_elm, high_elm );
          ++thres_combinations;
        }
      }
    }
    COMMENT( "thres_combinations: " << thres_combinations, 0 );
    DEBUG_WRITE( SM_HCI, "SM_HCI", 0 );
    COMMENT( "Computing prob of SM_HCI", 0 );
    Image< float > prob_sm_hci( SM_HCI );
    prob_sm_hci /= thres_combinations;
    COMMENT( "Computing prob_F_I(x)", 0 );
    Signal prob_f_ix( max_grad + 1, 0, 1 );
    for( size_t val = 0; val <= max_grad; ++val ) {
      COMMENT( "Computing prob_x of SM_HCI and the number of ones in it and computing F_I(x) and the number of xs in " <<
               "F_I(x).", 3 );
      Image< float > prob_x_sm_hci( prob_sm_hci );
      // Aqui se utiliza um limiar de intensidade do gradiente. Mas, prob_x_sm_hci é gerado pela frequência de limiares.
      Segmentation::IntensityThreshold( prob_x_sm_hci, static_cast< float >( val ) / max_grad, 1.0f ); 
      size_t ones_prob_x_sm_hci = 0;
      size_t vals_f_ix = 0;
      for( size_t pxl = 0; pxl < pixels; ++pxl ) {
        if( prob_x_sm_hci[ pxl ] > 0.1f ) {
          ++ones_prob_x_sm_hci;
          if( suppressed_sobel[ pxl ] == val )
            ++vals_f_ix;
        }
      }
      COMMENT( "Computing prob in F_I(x)", 3 );
      if( ones_prob_x_sm_hci > 0 ) {
        //cout << "val: " << val << ", ones_prob_x_sm_hci: " << ones_prob_x_sm_hci << ", vals_f_ix: " << vals_f_ix << endl;
        prob_f_ix[ val ] = static_cast< double >( vals_f_ix ) / ones_prob_x_sm_hci;
      }
    }
    COMMENT( "Filtering the histogram.", 0 );
    // Signal median_hist = hist.RemoveEmptyBins( );
    // Vector< size_t > hist_index = hist.MapBins( median_hist );
    prob_f_ix[ 0 ] = 0; /* Remove background. */
    SignalOp::MedianFilter( prob_f_ix, 2 + ( prob_f_ix.Bins( ) / 1000 ) );
    COMMENT( "Finding first and last non-zero bins in signal. max_grad: " << max_grad, 0 );
    size_t last_init_zero = 1;
    size_t first_end_zero = max_grad;
    for( size_t elm = 2; elm < max_grad + 1; ++elm ) {
      if( prob_f_ix[ elm ] > 0.0 ) {
        last_init_zero = elm - 1;
        break;
      }
    }
    for( size_t elm = max_grad - 1; elm < max_grad; --elm ) {
      if( prob_f_ix[ elm ] > 0.0 ) {
        first_end_zero = elm + 1;
        break;
      }
    }
    COMMENT( "last_init_zero: " << last_init_zero << ", first_end_zero: " << first_end_zero, 0 );
    COMMENT( "Getting first and last peaks of prob_F_I(x) as lower and higher thresholds.", 0 );
    D lower_intensity = SignalOp::FirstRightPeak( prob_f_ix, last_init_zero, first_end_zero, 0 );
    D higher_intensity = SignalOp::FirstLeftPeak( prob_f_ix, first_end_zero, last_init_zero, 0 );
    COMMENT( "lower_intensity: " << lower_intensity << ", higher_intensity: " << higher_intensity, 0 );
    cout << "lower_intensity: " << lower_intensity << ", higher_intensity: " << higher_intensity << endl;
    // lower_threshold = lower_intensity / max_grad;
    // higher_threshold = higher_intensity / max_grad;
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
Image< D > AutoScaleCanny( const Image< D > &smooth, size_t window_scale, float lower_threshold,
                                     float higher_threshold ) {
  try {
    COMMENT( "Computing suppressed Sobel.", 0 );
    Image< D > suppressed_sobel = Gradient::NonMaxSobelSuppression( smooth );
    DEBUG_WRITE( suppressed_sobel, "suppressed_sobel", 1 );
    COMMENT( "Creating resultant image.", 0 );
    Image< D > canny_scale( smooth.Dim( ) );
    COMMENT( "Getting image dimensions.", 0 );
    Vector< size_t > full_size = smooth.Dim( );
    size_t dims = smooth.Dims( );
    if( dims == 2 )
      full_size.push_back( 1 );
    Vector< size_t > dim_size( full_size ); /* Maximum position to slide window on. */
    COMMENT( "Creating window with correct dimensions.", 0 );
    Vector< size_t > window_side( 3, 1 );
    for( size_t dms = 0; dms < 3; ++dms ) {
      window_side( dms ) = ( full_size( dms ) + window_scale - 1 ) / window_scale;
      dim_size( dms ) = full_size( dms ) - window_side( dms ) + 1;
    }
    COMMENT( "xwin: " << window_side( 0 ) << ", ywin: " << window_side( 1 ) << ", zwin: " << window_side( 2 ), 1 );
    COMMENT( "Creating histogram to compute lower and higher thresholds for each subregion.", 0 );
    Signal histogram( suppressed_sobel.Maximum( ) + 1, 0.0, 1.0 );
    COMMENT( "Factor of the window size by which displacement over the image occurs.", 0 );
    size_t window_factor = 2;
    COMMENT( "Union of local hysteresis.", 0 );
    Image< D > edge( window_side );
    Adjacency adj( AdjacencyType::HyperSpheric( 1.9, dims ) );
    AdjacencyIterator adj_itr( smooth, adj );
    size_t adj_size = adj.size( );
    size_t adj_pxl;
    for( size_t z_src = 0; z_src < dim_size( 2 ); ) {
      for( size_t y_src = 0; y_src < dim_size( 1 ); ) {
        for( size_t x_src = 0; x_src < dim_size( 0 ); ) {
          COMMENT( "Implementation of Medina 11 - PR. Automatic hysteresis detection.", 0 );
          COMMENT( "Creating window histogram and getting the maximum value inside the window.", 4 );
          D max_grad = 0;
          for( size_t bin = 0; bin < histogram.size( ); ++bin )
            histogram[ bin ] = 0.0;
          for( size_t z_wdw = 0; z_wdw < window_side( 2 ); ++z_wdw ) {
            for( size_t y_wdw = 0; y_wdw < window_side( 1 ); ++y_wdw ) {
              for( size_t x_wdw = 0; x_wdw < window_side( 0 ); ++x_wdw ) {
                D grad_value = suppressed_sobel( x_src + x_wdw, y_src + y_wdw, z_src + z_wdw );
                ++histogram[ grad_value ];
                if( max_grad < grad_value )
                  max_grad = grad_value;
              }
            }
          }
          if( max_grad != 0 ) {
            const size_t THRES_NUMBER = 25;
            COMMENT( "max_grad: " << max_grad, 1 );
            float delta_threshold = ( higher_threshold - lower_threshold ) / THRES_NUMBER;
            COMMENT( "delta_threshold: " << delta_threshold, 1 );
            COMMENT( "Computing SM_{H_{C(I)}} from all DeltaG_{low,high}(I).", 1 );
            float low_thres = lower_threshold;
            COMMENT( "low_thres: " << low_thres, 1 );
            Image< float > SM_HCI( suppressed_sobel.Dim( ) );
            size_t thres_combinations = 0;
            try {
              Vector< std::thread > threads;
              size_t nthreads = 0;
              const size_t MAX_THREADS = 12;
              for( size_t low_elm = 0; low_elm < THRES_NUMBER; ++low_elm, low_thres += delta_threshold ) {
                float high_thres = low_thres + delta_threshold;
                COMMENT( "high_thres: " << high_thres, 3 );
                for( size_t high_elm = low_elm + 1; high_elm < THRES_NUMBER; ++high_elm, high_thres += delta_threshold ) {
                  if( nthreads < MAX_THREADS ) {
                    threads.push_back( std::thread( &DetectHighThresEdges< D >, std::ref( SM_HCI ), std::ref( suppressed_sobel ),
                                                    max_grad, low_thres, high_thres, low_elm, high_elm ) );
                    ++nthreads;
                  }
                  else {
                    for( size_t thd = 0; thd < nthreads; ++thd ) {
                      threads( thd ).join( );
                      ++thres_combinations;
                    }
                    threads = Vector< std::thread >( );
                    nthreads = 0;
                  }
                }
              }
              for( size_t thd = 0; thd < nthreads; ++thd ) {
                threads( thd ).join( );
                ++thres_combinations;
              }
            }
            catch( std::exception &e ) {
              std::cout << "Warning: Failed to run in multi-thread. Exception: " << e.what( ) << std::endl;
              for( size_t low_elm = 0; low_elm < THRES_NUMBER; ++low_elm, low_thres += delta_threshold ) {
                float high_thres = low_thres + delta_threshold;
                COMMENT( "high_thres: " << high_thres, 3 );
                for( size_t high_elm = low_elm + 1; high_elm < THRES_NUMBER; ++high_elm, high_thres += delta_threshold ) {
                  DetectHighThresEdges( SM_HCI, suppressed_sobel, max_grad, low_thres, high_thres, low_elm, high_elm );
                  ++thres_combinations;
                }
              }
            }
            // for( size_t low_elm = 0; low_elm < THRES_NUMBER; ++low_elm, low_thres += delta_threshold ) {
            //   float high_thres = low_thres + delta_threshold;
            //   COMMENT( "high_thres: " << high_thres, 1 );
            //   for( size_t high_elm = low_elm + 1; high_elm < THRES_NUMBER; ++high_elm, high_thres += delta_threshold ) {
            //     COMMENT( "Computing DeltaG_{high}(I) and DeltaG_{low,high}(I).", 1 );
            //     Image< float > delta_g_high( suppressed_sobel );
            //     delta_g_high /= max_grad;
            //     COMMENT( "Hysteresis:", 1 );
            //     Image< float > delta_g_high_low( Segmentation::HighHysteresis( delta_g_high, low_thres, high_thres ) );
            //     COMMENT( "Thresholding:", 1 );
            //     Segmentation::IntensityThreshold( delta_g_high, high_thres, 1.0f );
            //     SM_HCI += ( delta_g_high_low - delta_g_high );
            //     ++thres_combinations;
            //   }
            // }
            COMMENT( "thres_combinations: " << thres_combinations, 1 );
            DEBUG_WRITE( SM_HCI, "SM_HCI", 2 );
            COMMENT( "Computing prob of SM_HCI", 1 );
            Image< float > prob_sm_hci( SM_HCI );
            prob_sm_hci /= thres_combinations;
            COMMENT( "Computing prob_F_I(x)", 2 );
            Signal prob_f_ix( max_grad + 1, 0, 1 );
            for( size_t val = 0; val <= max_grad; ++val ) {
              COMMENT( "Computing prob_x of SM_HCI and the number of ones in it and computing F_I(x) and the number of xs in " <<
                       "F_I(x).", 4 );
              Image< float > prob_x_sm_hci( prob_sm_hci );
              // Aqui se utiliza um limiar de intensidade do gradiente. Mas, prob_x_sm_hci é gerado pela frequência de limiares.
              Segmentation::IntensityThreshold( prob_x_sm_hci, static_cast< float >( val ) / max_grad, 1.0f ); 
              size_t ones_prob_x_sm_hci = 0;
              size_t vals_f_ix = 0;
              for( size_t z_wdw = 0; z_wdw < window_side( 2 ); ++z_wdw ) {
                for( size_t y_wdw = 0; y_wdw < window_side( 1 ); ++y_wdw ) {
                  for( size_t x_wdw = 0; x_wdw < window_side( 0 ); ++x_wdw ) {
                    if( prob_x_sm_hci( x_src + x_wdw, y_src + y_wdw, z_src + z_wdw ) > 0.1f ) {
                      ++ones_prob_x_sm_hci;
                      if( suppressed_sobel( x_src + x_wdw, y_src + y_wdw, z_src + z_wdw ) == val )
                        ++vals_f_ix;
                    }
                  }
                }
              }
              COMMENT( "Computing prob in F_I(x)", 4 );
              if( ones_prob_x_sm_hci > 0 ) {
                //cout << "val: " << val << ", ones_prob_x_sm_hci: " << ones_prob_x_sm_hci << ", vals_f_ix: " << vals_f_ix << endl;
                prob_f_ix[ val ] = static_cast< double >( vals_f_ix ) / ones_prob_x_sm_hci;
              }
            }
            //cout << prob_f_ix << endl;
            COMMENT( "Filtering the histogram.", 0 );
            prob_f_ix[ 0 ] = 0; /* Remove background. */
            SignalOp::MedianFilter( prob_f_ix, 2 + ( prob_f_ix.Bins( ) / 1000 ) );
            COMMENT( "Finding first and last non-zero bins in signal. max_grad: " << max_grad, 2 );
            size_t last_init_zero = 1;
            size_t first_end_zero = max_grad;
            for( size_t elm = 2; elm < max_grad + 1; ++elm ) {
              if( prob_f_ix[ elm ] > 0.0 ) {
                last_init_zero = elm - 1;
                break;
              }
            }
            for( size_t elm = max_grad - 1; elm < max_grad; --elm ) {
              if( prob_f_ix[ elm ] > 0.0 ) {
                first_end_zero = elm + 1;
                break;
              }
            }
            COMMENT( "last_init_zero: " << last_init_zero << ", first_end_zero: " << first_end_zero, 2 );
            COMMENT( "Getting first and last peaks of prob_F_I(x) as lower and higher thresholds.", 2 );
            D lower_intensity = SignalOp::FirstRightPeak( prob_f_ix, last_init_zero, first_end_zero, 2 );
            D higher_intensity = SignalOp::FirstLeftPeak( prob_f_ix, first_end_zero, last_init_zero, 2 );
            COMMENT( "lower_intensity: " << lower_intensity << ", higher_intensity: " << higher_intensity, 2 );
            cout << "lower_intensity: " << lower_intensity << ", higher_intensity: " << higher_intensity << endl;
            COMMENT( "Computing the hysteresis.", 2 );
            edge.Set( 0.0 );
            GrowingBucketQueue queue( edge.size( ) );
            COMMENT( "Avoid getting all window pixels as edges.", 4 );
            if( lower_intensity != 0 ) {
              COMMENT( "Computing higher threshold seeds.", 4 );
              for( size_t z_wdw = 0; z_wdw < window_side( 2 ); ++z_wdw ) {
                for( size_t y_wdw = 0; y_wdw < window_side( 1 ); ++y_wdw ) {
                  for( size_t x_wdw = 0; x_wdw < window_side( 0 ); ++x_wdw ) {
                    COMMENT( "Getting local and global coordinates.", 4 );
                    //size_t wdw_coord = x_wdw + y_wdw*suppressed_sobel.size( 0 ) +
                    //z_wdw*suppressed_sobel.Displacement( 1 );
                    size_t edge_coord = x_wdw + y_wdw * edge.size( 0 ) + z_wdw * edge.Displacement( 1 );
                    COMMENT( "Verifying if pixel is an edge pixel and adding it to the queue.", 4 );
                    if( suppressed_sobel( x_src + x_wdw, y_src + y_wdw, z_src + z_wdw ) >= higher_intensity ) {
                      edge[ edge_coord ] = 1;
                      canny_scale( x_src + x_wdw, y_src + y_wdw, z_src + z_wdw ) = 1;
                      queue.Insert( edge_coord, 1 );
                    }
                  }
                }
              }
            }
            COMMENT( "If lower and higher intensities are equal, there is no need for hysteresis.", 4 );
            if( lower_intensity != higher_intensity ) {
              COMMENT( "Propagating to connected lower threshold pixels.", 4 );
              while( !queue.Empty( ) ) {
                COMMENT( "Removing  pixel from queue.", 4 );
                size_t pxl = queue.Remove( );
                COMMENT( "Searching for adjacents that must also be set as edge pixels.", 4 );
                for( size_t idx = 1; idx < adj_size; ++idx ) {
                  if( adj_itr.AdjIdx( pxl, idx, adj_pxl ) ) {
                    COMMENT( "Getting adjacent local and global coordinates.", 4 );
                    Vector< size_t > img_coord = edge.Coordinates( adj_pxl );
                    img_coord( 0 ) += x_src;
                    img_coord( 1 ) += y_src;
                    img_coord( 2 ) += z_src;
                    COMMENT( "Verifying if adjacent pixel is an edge pixel and adding it to the queue.", 4 );
                    if( ( suppressed_sobel( img_coord ) >= lower_intensity ) && ( edge[ adj_pxl ] == 0 ) ) {
                      edge[ adj_pxl ] = 1;
                      canny_scale( img_coord ) = 1;
                      queue.Insert( adj_pxl, 1 );
                    }
                  }
                }
              }
            }
          }
          COMMENT( "Ensuring the last step in x to include the edges in the extremity of the image.", 4 );
          if( ( x_src >= dim_size( 0 ) - std::max( 1, static_cast< int >( window_side( 0 ) / window_factor ) ) ) &&
              ( x_src < dim_size( 0 ) - 1 ) )
            x_src = dim_size( 0 ) - 1;
          else
            x_src += std::max( 1, static_cast< int >( window_side( 0 ) / window_factor ) );
        }
        COMMENT( "Ensuring the last step in y to include the edges in the extremity of the image.", 4 );
        if( ( y_src >= dim_size( 1 ) - std::max( 1, static_cast< int >( window_side( 1 ) / window_factor ) ) ) &&
            ( y_src < dim_size( 1 ) - 1 ) )
          y_src = dim_size( 1 ) - 1;
        else
          y_src += std::max( 1, static_cast< int >( window_side( 1 ) / window_factor ) );
      }
      COMMENT( "Ensuring the last step in z to include the edges in the extremity of the image.", 4 );
      if( ( z_src >= dim_size( 2 ) - std::max( 1, static_cast< int >( window_side( 2 ) / window_factor ) ) ) &&
          ( z_src < dim_size( 2 ) - 1 ) )
        z_src = dim_size( 2 ) - 1;
      else
        z_src += std::max( 1, static_cast< int >( window_side( 2 ) / window_factor ) );
    }
    return( canny_scale );
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
Image< D > AutoMultiScaleCanny( const Image< D > &img, float lower_threshold, float higher_threshold,
                                          float sigma, size_t levels ) {
  try {
    COMMENT( "Checking if thresholds make sense.", 1 );
    if( ( lower_threshold > higher_threshold ) || ( lower_threshold < 0.0f ) || ( higher_threshold > 1.0f ) ) {
      std::string msg( BIAL_ERROR( "Invalid thresholds. Required: 0.0 <= lower <= higher <= 1.0. Given: lower:" +
                                   std::to_string( lower_threshold ) + ", higher: " +
                                   std::to_string( higher_threshold ) + "." ) );
      throw( std::logic_error( msg ) );
    }
    COMMENT( "Getting image maximum dimension.", 0 );
    Vector< size_t > full_size = img.Dim( );
    COMMENT( "Computing global Canny with medium and higher thresholds.", 0 );
    Image< D > global = AutoCanny( img, lower_threshold, higher_threshold, sigma );
    COMMENT( "Computing the filtered image.", 0 );
    Image< D > smooth = Filtering::Gaussian( img, 2.0, sigma );
    COMMENT( "Computing sobel gradient.", 0 );
    for( size_t scl = 1; scl < levels; ++scl ) {
      COMMENT( "Computing scale Canny with medium and higher thresholds for scale: " << scl, 0 );
      Image< D > local = AutoScaleCanny( smooth, std::pow( 2, scl ), lower_threshold, higher_threshold );
      for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
        if( ( local[ pxl ] == 1 ) && ( global[ pxl ] == 0 ) )
          global[ pxl ] = scl + 1;
      }
    }
    return( global );
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

int main( int argc, char **argv ) {
  if( argc < 6 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <output image> <lower threshold> <higher threshold> <sigma> " <<
      "[<multi_scale_level>]" << endl;
    cout << "\t\t<lower threshold>, <higher threshold>: Hysteresis thresholds From 0.0 to 1.0." << endl;
    cout << "\t\t<sigma>: Standard deviation of Gaussian filter. From 0.0 to +oo." << endl;
    cout << "\t\t<multi_scale_level>: number of sub levels. 1 for standard method." << endl;
    return( 0 );
  }
  try {
    COMMENT( "Reading parameters.", 0 );
    Image< int > img( Read< int >( argv[ 1 ] ) );
    float low_thres = atof( argv[ 3 ] );
    float hig_thres = atof( argv[ 4 ] );
    float sigma = atof( argv[ 5 ] );
    size_t levels = 1;
    if( argc == 7 )
      levels = atoi( argv[ 6 ] );
    if( levels <= 1 ) {
      COMMENT( "Running Canny.", 0 );
      Image< int > res( AutoCanny( img, low_thres, hig_thres, sigma ) );
      COMMENT( "Writing result.", 0 );
      Write( res, argv[ 2 ], argv[ 1 ] );
    }
    else {
      COMMENT( "Running Multi-Scale Canny.", 0 );
      Image< int > res( AutoMultiScaleCanny( img, low_thres, hig_thres, sigma, levels ) );
      COMMENT( "Writing result.", 0 );
      Write( res, argv[ 2 ], argv[ 1 ] );
    }
  }
  catch( std::ios_base::failure &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while writing to file." ) );
    throw( std::ios_base::failure( msg ) );
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

  return( 0 );
}
