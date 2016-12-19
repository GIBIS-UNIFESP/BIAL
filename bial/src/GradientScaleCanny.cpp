/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/21 
 * @brief Computes multiscale Canny gradient. 
 */

#ifndef BIALGRADIENTSCALECANNY_C
#define BIALGRADIENTSCALECANNY_C

#include "GradientScaleCanny.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_GradientScaleCanny )
#define BIAL_EXPLICIT_GradientScaleCanny
#endif
#if defined ( BIAL_EXPLICIT_GradientScaleCanny ) || ( BIAL_IMPLICIT_BIN )

#include "AdjacencyRound.hpp"
#include "AdjacencyIterator.hpp"
#ifdef BIAL_DEBUG
#include "FileImage.hpp"
#endif
#include "FilteringGaussian.hpp"
#include "GradientCanny.hpp"
#include "GrowingBucketQueue.hpp"
#include "Image.hpp"
#include "SegmentationConnectedComponents.hpp"
#include "Signal.hpp"
#include "SignalAccumulate.hpp"
#include "SignalNormalize.hpp"

namespace Bial {

  template< class D >
  Image< D > Gradient::ScaleCanny( const Image< D > &img, size_t window_scale, float lower_threshold,
                                   float higher_threshold ) {
    try {
      COMMENT( "Checking if thresholds make sense.", 1 );
      if( ( lower_threshold > higher_threshold ) || ( lower_threshold < 0.0 ) || ( higher_threshold > 1.0 ) ) {
        std::string msg( BIAL_ERROR( "Invalid thresholds. Required: 0.0 <= lower <= higher <= 1.0. Given: lower:" +
                                     std::to_string( lower_threshold ) + ", higher: " +
                                     std::to_string( higher_threshold ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Computing suppressed Sobel.", 0 );
      Image< D > suppressed_sobel( Gradient::NonMaxSobelSuppression( img ) );
      DEBUG_WRITE( suppressed_sobel, "suppressed_sobel", 0 );
      COMMENT( "Creating resultant image.", 0 );
      Image< D > canny_scale( img.Dim( ), img.PixelSize( ) );
      COMMENT( "Getting image dimensions.", 0 );
      Vector< size_t > full_size = img.Dim( );
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
      size_t window_factor = 4;
      COMMENT( "Sum of local hysteresis.", 0 );
      Image< D > edge( window_side );
      Vector< size_t > step( window_side / window_factor );
      if( step[ 0 ] < 1 ) step[ 0 ] = 1;
      if( step[ 1 ] < 1 ) step[ 1 ] = 1;
      if( step[ 2 ] < 1 ) step[ 2 ] = 1;
      Adjacency adj( AdjacencyType::HyperSpheric( 1.9, edge.Dims( ) ) );
      AdjacencyIterator adj_itr( edge, adj );
      size_t adj_size = adj.size( );
      Vector< size_t > adj_pxl( 3 );
      size_t hist_size = histogram.size( );
      for( size_t z_src = 0; z_src < dim_size( 2 ); ) {
        for( size_t y_src = 0; y_src < dim_size( 1 ); ) {
          COMMENT( "z_src: " << z_src << ", y_src: " << y_src, 4 );
          for( size_t x_src = 0; x_src < dim_size( 0 ); ) {
            COMMENT( "Creating window histogram.", 4 );
            for( size_t bin = 0; bin < hist_size; ++bin )
              histogram[ bin ] = 0.0;
            for( size_t z_wdw = 0; z_wdw < window_side( 2 ); ++z_wdw ) {
              for( size_t y_wdw = 0; y_wdw < window_side( 1 ); ++y_wdw ) {
                for( size_t x_wdw = 0; x_wdw < window_side( 0 ); ++x_wdw )
                  ++histogram[ suppressed_sobel( x_src + x_wdw, y_src + y_wdw, z_src + z_wdw ) ];
              }
            }
            COMMENT( "Computing normalized accumulated histogram to get low and high intensity thresholds.", 4 );
            histogram[ 0 ] = 0.0;
            SignalOp::Normalize( histogram );
            SignalOp::Accumulate( histogram );
            COMMENT( "Getting lower bin for threshold.", 4 );
            size_t lower_bin = 0;
            for( size_t bin = 1; bin < hist_size; ++bin ) {
              if( std::abs( lower_threshold - histogram[ bin ] ) <=
                  std::abs( lower_threshold - histogram[ lower_bin ] ) )
                lower_bin = bin;
              else
                break;
            }
            COMMENT( "Getting higher bin for threshold.", 4 );
            size_t higher_bin = lower_bin;
            for( size_t bin = lower_bin + 1; bin < hist_size; ++bin ) {
              if( std::abs( higher_threshold - histogram[ bin ] ) <=
                  std::abs( higher_threshold - histogram[ higher_bin ] ) )
                higher_bin = bin;
              else
                break;
            }
            COMMENT( "Getting lower and higher intensities.", 4 );
            D lower_intensity = histogram.Data( lower_bin );
            D higher_intensity = histogram.Data( higher_bin );
            edge.Set( 0.0 );
            GrowingBucketQueue queue( edge.size( ) );
            COMMENT( "Avoid getting all window pixels as edges.", 4 );
            if( lower_intensity != 0 ) {
              COMMENT( "Computing higher threshold seeds.", 4 );
              for( size_t z_wdw = 0; z_wdw < window_side( 2 ); ++z_wdw ) {
                for( size_t y_wdw = 0; y_wdw < window_side( 1 ); ++y_wdw ) {
                  for( size_t x_wdw = 0; x_wdw < window_side( 0 ); ++x_wdw ) {
                    COMMENT( "Getting local and global coordinates.", 5 );
                    //size_t wdw_coord = x_wdw + y_wdw*suppressed_sobel.size( 0 ) +
                    //z_wdw*suppressed_sobel.Displacement( 1 );
                    size_t edge_coord = x_wdw + y_wdw * edge.size( 0 ) + z_wdw * edge.Displacement( 1 );
                    COMMENT( "Verifying if pixel is an edge pixel and adding it to the queue.", 5 );
                    //if( base_grad( x_src + x_wdw, y_src + y_wdw, z_src + z_wdw ) > 0 ) {
                    if( suppressed_sobel( x_src + x_wdw, y_src + y_wdw, z_src + z_wdw ) >= higher_intensity ) {
                      edge[ edge_coord ] = 1;
                      canny_scale( x_src + x_wdw, y_src + y_wdw, z_src + z_wdw ) = 1;
                      queue.Insert( edge_coord, 1 );
                    }
                  }
                }
              }
            }
            COMMENT( "Lower and higher intensities: ." << lower_intensity << ", " << higher_intensity, 4 );
            if( lower_intensity != higher_intensity ) {
              COMMENT( "Propagating to connected lower threshold pixels.", 4 );
              while( !queue.Empty( ) ) {
                COMMENT( "Removing  pixel from queue.", 4 );
                size_t pxl = queue.Remove( );
                Vector< size_t > pxl_coords( edge.Coordinates( pxl ) );
                COMMENT( "Searching for adjacents that must also be set as edge pixels.", 4 );
                for( size_t idx = 1; idx < adj_size; ++idx ) {
                  if( ( adj_itr.*adj_itr.AdjVct )( pxl_coords, idx, adj_pxl ) ) {
                    COMMENT( "Getting adjacent local and global coordinates.", 5 );
                    Vector< size_t > img_coord( adj_pxl );
                    img_coord[ 0 ] += x_src;
                    img_coord[ 1 ] += y_src;
                    img_coord[ 2 ] += z_src;
                    COMMENT( "Verifying if adjacent pixel is an edge pixel and adding it to the queue.", 5 );
                    if( ( suppressed_sobel( img_coord ) >= lower_intensity ) && ( edge( adj_pxl ) == 0 ) ) {
                      edge( adj_pxl ) = 1;
                      canny_scale( img_coord ) = 1;
                      queue.Insert( edge.Position( adj_pxl ), 1 );
                    }
                  }
                }
              }
            }
            COMMENT( "x step to the window.", 4 );
            COMMENT( "Checking the last step, if it requires a smaller displacement to include the edges in " <<
                     "the extremity of the image.", 4 );
            if( ( x_src >= dim_size( 0 ) - step[ 0 ] ) && ( x_src < dim_size( 0 ) - 1 ) ) {
              x_src = dim_size( 0 ) - 1;
              COMMENT( "Other steps.", 4 );
            }
            else
              x_src += step[ 0 ];
          }
          COMMENT( "y step to the window.", 4 );
          COMMENT( "Checking the last step, if it requires a smaller displacement to include the edges in " <<
                   "the extremity of the image.", 4 );
          if( ( y_src >= dim_size( 1 ) - step[ 1 ] ) && ( y_src < dim_size( 1 ) - 1 ) ) {
            y_src = dim_size( 1 ) - 1;
            COMMENT( "Other steps.", 4 );
          }
          else
            y_src += step[ 1 ];
        }
        COMMENT( "z step to the window.", 4 );
        COMMENT( "Checking the last step, if it requires a smaller displacement to include the edges in " <<
                 "the extremity of the image.", 4 );
        if( ( z_src >= dim_size( 2 ) - step[ 2 ] ) && ( z_src < dim_size( 2 ) - 1 ) ) {
          z_src = dim_size( 2 ) - 1;
          COMMENT( "Other steps.", 4 );
        }
        else
          z_src += step[ 2 ];
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
  Image< D > Gradient::SubScaleCanny( const Image< D > &img, const Image< D > &base_grad, size_t window_scale,
                                      float lower_threshold ) {
    try {
      COMMENT( "Computing suppressed Sobel.", 0 );
      Image< D > suppressed_sobel = Gradient::NonMaxSobelSuppression( img );
      DEBUG_WRITE( suppressed_sobel, "suppressed_sobel", 0 );
      COMMENT( "Creating resultant image.", 0 );
      Image< D > canny_scale( img.Dim( ), img.PixelSize( ) );
      COMMENT( "Getting image dimensions.", 0 );
      Vector< size_t > full_size = img.Dim( );
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
      size_t window_factor = 4;
      COMMENT( "Sum of local hysteresis.", 0 );
      Image< D > edge( window_side );
      Vector< size_t > step( window_side / window_factor );
      if( step[ 0 ] < 1 ) step[ 0 ] = 1;
      if( step[ 1 ] < 1 ) step[ 1 ] = 1;
      if( step[ 2 ] < 1 ) step[ 2 ] = 1;
      Adjacency adj( AdjacencyType::HyperSpheric( 1.9, edge.Dims( ) ) );
      AdjacencyIterator adj_itr( edge, adj );
      size_t adj_size = adj.size( );
      Vector< size_t > adj_pxl( 3 );
      size_t hist_size = histogram.size( );
      for( size_t z_src = 0; z_src < dim_size( 2 ); ) {
        for( size_t y_src = 0; y_src < dim_size( 1 ); ) {
          COMMENT( "z_src: " << z_src << ", y_src: " << y_src, 4 );
          for( size_t x_src = 0; x_src < dim_size( 0 ); ) {
            COMMENT( "Creating window histogram.", 4 );
            for( size_t bin = 0; bin < hist_size; ++bin )
              histogram[ bin ] = 0.0;
            for( size_t z_wdw = 0; z_wdw < window_side( 2 ); ++z_wdw ) {
              for( size_t y_wdw = 0; y_wdw < window_side( 1 ); ++y_wdw ) {
                for( size_t x_wdw = 0; x_wdw < window_side( 0 ); ++x_wdw )
                  ++histogram[ suppressed_sobel( x_src + x_wdw, y_src + y_wdw, z_src + z_wdw ) ];
              }
            }
            COMMENT( "Computing normalized accumulated histogram to get low and high intensity thresholds.", 4 );
            histogram[ 0 ] = 0.0;
            SignalOp::Normalize( histogram );
            SignalOp::Accumulate( histogram );
            COMMENT( "Getting lower bin for threshold.", 4 );
            size_t lower_bin = 0;
            for( size_t bin = 1; bin < hist_size; ++bin ) {
              if( std::abs( lower_threshold - histogram[ bin ] ) <=
                  std::abs( lower_threshold - histogram[ lower_bin ] ) )
                lower_bin = bin;
              else
                break;
            }
            // COMMENT( "Getting higher bin for threshold.", 4 );
            // size_t higher_bin = lower_bin;
            // for( size_t bin = lower_bin + 1; bin < histogram.size( ); ++bin ) {
            //   if( std::abs( higher_threshold - histogram[ bin ] ) <=
            //       std::abs( higher_threshold - histogram[ higher_bin ] ) )
            //     higher_bin = bin;
            //   else
            //     break;
            // }
            COMMENT( "Getting lower and higher intensities.", 4 );
            D lower_intensity = histogram.Data( lower_bin );
            // D higher_intensity = histogram.Data( higher_bin );
            edge.Set( 0.0 );
            GrowingBucketQueue queue( edge.size( ) );
            COMMENT( "Avoid getting all window pixels as edges.", 4 );
            if( lower_intensity != 0 ) {
              COMMENT( "Getting seeds from base gradient.", 4 );
              for( size_t z_wdw = 0; z_wdw < window_side( 2 ); ++z_wdw ) {
                for( size_t y_wdw = 0; y_wdw < window_side( 1 ); ++y_wdw ) {
                  for( size_t x_wdw = 0; x_wdw < window_side( 0 ); ++x_wdw ) {
                    COMMENT( "Getting local and global coordinates.", 4 );
                    //size_t wdw_coord = x_wdw + y_wdw*suppressed_sobel.size( 0 ) +
                    //z_wdw*suppressed_sobel.Displacement( 1 );
                    size_t edge_coord = x_wdw + y_wdw * edge.size( 0 ) + z_wdw * edge.Displacement( 1 );
                    COMMENT( "Verifying if pixel is an edge pixel and adding it to the queue.", 4 );
                    if( base_grad( x_src + x_wdw, y_src + y_wdw, z_src + z_wdw ) > 0 ) {
                    //if( suppressed_sobel( x_src + x_wdw, y_src + y_wdw, z_src + z_wdw ) >= higher_intensity ) {
                      edge[ edge_coord ] = 1;
                      canny_scale( x_src + x_wdw, y_src + y_wdw, z_src + z_wdw ) = 1;
                      queue.Insert( edge_coord, 1 );
                    }
                  }
                }
              }
            // }
            // COMMENT( "If lower and higher intensities are equal, there is no need for hysteresis.", 4 );
            // if( lower_intensity != higher_intensity ) {
              COMMENT( "Propagating to connected lower threshold pixels.", 4 );
              while( !queue.Empty( ) ) {
                COMMENT( "Removing  pixel from queue.", 4 );
                size_t pxl = queue.Remove( );
                Vector< size_t > pxl_coords( edge.Coordinates( pxl ) );
                COMMENT( "Searching for adjacents that must also be set as edge pixels.", 4 );
                for( size_t idx = 1; idx < adj_size; ++idx ) {
                  if( ( adj_itr.*adj_itr.AdjVct )( pxl_coords, idx, adj_pxl ) ) {
                    COMMENT( "Getting adjacent local and global coordinates.", 5 );
                    Vector< size_t > img_coord( adj_pxl );
                    img_coord[ 0 ] += x_src;
                    img_coord[ 1 ] += y_src;
                    img_coord[ 2 ] += z_src;
                    COMMENT( "Verifying if adjacent pixel is an edge pixel and adding it to the queue.", 5 );
                    if( ( suppressed_sobel( img_coord ) >= lower_intensity ) && ( edge( adj_pxl ) == 0 ) ) {
                      edge( adj_pxl ) = 1;
                      canny_scale( img_coord ) = 1;
                      queue.Insert( edge.Position( adj_pxl ), 1 );
                    }
                  }
                }
              }
            }
            COMMENT( "x step to the window.", 4 );
            COMMENT( "Checking the last step, if it requires a smaller displacement to include the edges in " <<
                     "the extremity of the image.", 4 );
            if( ( x_src >= dim_size( 0 ) - step[ 0 ] ) && ( x_src < dim_size( 0 ) - 1 ) ) {
              x_src = dim_size( 0 ) - 1;
              COMMENT( "Other steps.", 4 );
            }
            else
              x_src += step[ 0 ];
          }
          COMMENT( "y step to the window.", 4 );
          COMMENT( "Checking the last step, if it requires a smaller displacement to include the edges in " <<
                   "the extremity of the image.", 4 );
          if( ( y_src >= dim_size( 1 ) - step[ 1 ] ) && ( y_src < dim_size( 1 ) - 1 ) ) {
            y_src = dim_size( 1 ) - 1;
            COMMENT( "Other steps.", 4 );
          }
          else
            y_src += step[ 1 ];
        }
        COMMENT( "z step to the window.", 4 );
        COMMENT( "Checking the last step, if it requires a smaller displacement to include the edges in " <<
                 "the extremity of the image.", 4 );
        if( ( z_src >= dim_size( 2 ) - step[ 2 ] ) && ( z_src < dim_size( 2 ) - 1 ) ) {
          z_src = dim_size( 2 ) - 1;
          COMMENT( "Other steps.", 4 );
        }
        else
          z_src += step[ 2 ];
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
  Image< D > Gradient::MultiScaleCanny( const Image< D > &img, float lower_threshold, float higher_threshold,
                                        float sigma, size_t scales ) {
    try {
      COMMENT( "Checking if thresholds make sense.", 1 );
      if( ( lower_threshold > higher_threshold ) || ( lower_threshold < 0.0f ) || ( higher_threshold > 1.0f ) ) {
        std::string msg( BIAL_ERROR( "Invalid thresholds. Required: 0.0 <= lower <= higher <= 1.0. Given: lower:" +
                                     std::to_string( lower_threshold ) + ", higher: " +
                                     std::to_string( higher_threshold ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      //COMMENT( "Getting image maximum dimension.", 0 );
      //Vector< size_t > full_size = img.Dim( );
      COMMENT( "Computing global Canny with medium and higher thresholds.", 0 );
      Image< D > global = Gradient::Canny( img, lower_threshold, higher_threshold, sigma );
      COMMENT( "Computing the filtered image.", 0 );
      Image< D > smooth = Filtering::Gaussian( img, 2.0, sigma );
      for( size_t scl = 1; scl < scales; ++scl ) {
        COMMENT( "Computing scale Canny with medium and higher thresholds for scale: " << scl, 0 );
        Image< D > local = ScaleCanny( smooth, std::pow( 2, scl ), lower_threshold, higher_threshold );
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

  template< class D >
  Image< D > Gradient::MultiSubScaleCanny( const Image< D > &img, float lower_threshold, float higher_threshold,
                                           float sigma, size_t scales ) {
    try {
      COMMENT( "Checking if thresholds make sense.", 1 );
      if( ( lower_threshold > higher_threshold ) || ( lower_threshold < 0.0f ) || ( higher_threshold > 1.0f ) ) {
        std::string msg( BIAL_ERROR( "Invalid thresholds. Required: 0.0 <= lower <= higher <= 1.0. Given: lower:" +
                                     std::to_string( lower_threshold ) + ", higher: " +
                                     std::to_string( higher_threshold ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      //COMMENT( "Getting image maximum dimension.", 0 );
      //Vector< size_t > full_size = img.Dim( );
      COMMENT( "Computing global Canny with medium and higher thresholds.", 0 );
      Image< D > global = Gradient::Canny( img, lower_threshold, higher_threshold, sigma );
      COMMENT( "Computing the filtered image.", 0 );
      Image< D > smooth = Filtering::Gaussian( img, 2.0, sigma );
      for( size_t scl = 1; scl < scales; ++scl ) {
        COMMENT( "Computing scale Canny with medium and higher thresholds for scale: " << scl, 0 );
        Image< D > local = SubScaleCanny( smooth, global, std::pow( 2, scl ), lower_threshold );
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

  template< class D >
  Image< D > Gradient::MultiScaleSizeCanny( const Image< D > &img, float lower_threshold, float higher_threshold,
                                            float sigma, size_t scales, float fraction ) {
    try {
      COMMENT( "Checking if thresholds make sense.", 1 );
      if( ( lower_threshold > higher_threshold ) || ( lower_threshold < 0.0f ) || ( higher_threshold > 1.0f ) ) {
        std::string msg( BIAL_ERROR( "Invalid thresholds. Required: 0.0 <= lower <= higher <= 1.0. Given: lower:" +
                                     std::to_string( lower_threshold ) + ", higher: " +
                                     std::to_string( higher_threshold ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Checking if fraction makes sense.", 1 );
      if( ( fraction <= 0.0f ) || ( fraction > 1.0f ) ) {
        std::string msg( BIAL_ERROR( "Invalid fraction. Required: 0.0 < fraction <= 1.0. Given: "
                                     + std::to_string( fraction ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Computing multscale Canny.", 1 );
      Image< D > mult( MultiScaleCanny( img, lower_threshold, higher_threshold, sigma, scales ) );
      COMMENT( "Computing connected components.", 1 );
      Adjacency spheric( AdjacencyType::HyperSpheric( 1.9, img.Dims( ) ) );
      Image< int > label( Segmentation::ConnectedComponents( mult, spheric ) );
      COMMENT( "Removing components with less than min_size.", 1 );
      Image< int > res( Segmentation::RemoveSmallComponents( label, fraction ) );
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

#ifdef BIAL_EXPLICIT_GradientScaleCanny

  template Image< int > Gradient::ScaleCanny( const Image< int > &img, size_t window_scale, float lower_threshold,
                                            float higher_threshold );
  template Image< int > Gradient::SubScaleCanny( const Image< int > &img, const Image< int > &base_grad, 
                                               size_t window_scale, float lower_threshold );
  template Image< int > Gradient::MultiScaleCanny( const Image< int > &img, float lower_threshold, 
                                                 float higher_threshold, float sigma, size_t scales );
  template Image< int > Gradient::MultiSubScaleCanny( const Image< int > &img, float lower_threshold, 
                                                    float higher_threshold, float sigma, size_t scales );
  template Image< int > Gradient::MultiScaleSizeCanny( const Image< int > &img, float lower_threshold, 
                                                     float higher_threshold, float sigma, size_t scales, 
                                                     float fraction );

  template Image< llint > Gradient::ScaleCanny( const Image< llint > &img, size_t window_scale, float lower_threshold,
                                            float higher_threshold );
  template Image< llint > Gradient::SubScaleCanny( const Image< llint > &img, const Image< llint > &base_grad, 
                                               size_t window_scale, float lower_threshold );
  template Image< llint > Gradient::MultiScaleCanny( const Image< llint > &img, float lower_threshold, 
                                                 float higher_threshold, float sigma, size_t scales );
  template Image< llint > Gradient::MultiSubScaleCanny( const Image< llint > &img, float lower_threshold, 
                                                    float higher_threshold, float sigma, size_t scales );
  template Image< llint > Gradient::MultiScaleSizeCanny( const Image< llint > &img, float lower_threshold, 
                                                     float higher_threshold, float sigma, size_t scales, 
                                                     float fraction );

  template Image< float > Gradient::ScaleCanny( const Image< float > &img, size_t window_scale, float lower_threshold,
                                            float higher_threshold );
  template Image< float > Gradient::SubScaleCanny( const Image< float > &img, const Image< float > &base_grad, 
                                               size_t window_scale, float lower_threshold );
  template Image< float > Gradient::MultiScaleCanny( const Image< float > &img, float lower_threshold, 
                                                 float higher_threshold, float sigma, size_t scales );
  template Image< float > Gradient::MultiSubScaleCanny( const Image< float > &img, float lower_threshold, 
                                                    float higher_threshold, float sigma, size_t scales );
  template Image< float > Gradient::MultiScaleSizeCanny( const Image< float > &img, float lower_threshold, 
                                                     float higher_threshold, float sigma, size_t scales, 
                                                     float fraction );

  template Image< double > Gradient::ScaleCanny( const Image< double > &img, size_t window_scale, float lower_threshold,
                                            float higher_threshold );
  template Image< double > Gradient::SubScaleCanny( const Image< double > &img, const Image< double > &base_grad, 
                                               size_t window_scale, float lower_threshold );
  template Image< double > Gradient::MultiScaleCanny( const Image< double > &img, float lower_threshold, 
                                                 float higher_threshold, float sigma, size_t scales );
  template Image< double > Gradient::MultiSubScaleCanny( const Image< double > &img, float lower_threshold, 
                                                    float higher_threshold, float sigma, size_t scales );
  template Image< double > Gradient::MultiScaleSizeCanny( const Image< double > &img, float lower_threshold, 
                                                     float higher_threshold, float sigma, size_t scales, 
                                                     float fraction );
#endif

}

#endif

#endif
