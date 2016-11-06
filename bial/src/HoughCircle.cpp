/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2015/Apr/07
 * @brief Hough transform for circles
 */

#ifndef BIALHOUGHCIRCLE_C
#define BIALHOUGHCIRCLE_C

#include "HoughCircle.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_HoughCircle )
#define BIAL_EXPLICIT_HoughCircle
#endif
#if defined ( BIAL_EXPLICIT_HoughCircle ) || ( BIAL_IMPLICIT_BIN )

#include "DrawCircle.hpp"
#include "DrawPoint.hpp"
#include "GeometricsScale.hpp"
#include "GradientPosOperations.hpp"
#include "GradientScaleCanny.hpp"
#include "GradientSobel.hpp"

namespace Bial {

  template< class D >
  HoughIncreaseCompare< D >::HoughIncreaseCompare( const Image< D > &_aux ) : aux( _aux ) {
    data = aux.data( );
  }

  template< class D >
  bool HoughIncreaseCompare< D >::operator()( size_t l1, size_t l2 ) const {
    return( ( data[ l1 ] < data[ l2 ] ) || ( ( data[ l1 ] == data[ l2 ] ) && ( l1 < l2 ) ) );
  }

  template< class D >
  HoughDecreaseCompare< D >::HoughDecreaseCompare( const Image< D > &_aux ) : aux( _aux ) {
    data = aux.data( );
  }

  template< class D >
  bool HoughDecreaseCompare< D >::operator()( size_t l1, size_t l2 ) const {
    return( ( data[ l1 ] > data[ l2 ] ) || ( ( data[ l1 ] == data[ l2 ] ) && ( l1 < l2 ) ) );
  }

  template< class D >
  Vector< Circle > Hough::HoughCircles( Image< D > &img, float scale, float min_dist, size_t min_radius, 
                                        size_t max_radius, float canny_threshold, size_t detection_threshold,
                                        int canny_levels, int clean_edges ) {
    try {
      if( ( scale <= 0.0f ) || ( min_dist <= 0.0f ) ) {
        std::string msg( BIAL_ERROR( "scale and min_dist must be positive numbers." ) );
        throw( std::logic_error( msg ) );
      }
      if( max_radius <= min_radius ) {
        std::string msg( BIAL_ERROR( "max_radius must be greater than min_radius. Given: min_radius: " +
                                     std::to_string( min_radius ) + ", max_radius: " + std::to_string( max_radius ) ) );
        throw( std::logic_error( msg ) );
      }
      if( ( canny_levels < 1 ) || ( canny_levels > 4 ) ) {
        std::string msg( BIAL_ERROR( "Canny levels must be between 1 and 4." ) );
        throw( std::logic_error( msg ) );
      }
      size_t max_circles = 9999;
      COMMENT( "Computing canny edge detector.", 0 );
      Image< D > canny( Gradient::MultiScaleCanny( img, canny_threshold / 2.0f, canny_threshold, 0.1f, canny_levels ) );
      COMMENT( "Removing non-circular and small borders.", 0 );
      Image< D > filtered_canny( canny );
      if( clean_edges != 0 ) {
        Gradient::RemoveSmallSegments( canny, 20 );
      }
      if( filtered_canny.Maximum( ) == 0 ) {
        BIAL_WARNING( "No circle centers detected." );
        return( Vector< Circle >( ) );
      }
      COMMENT( "Computing sobel edges in x and y directions.", 0 );
      Image< D > sobel_x( Gradient::DirectionalSobel( img, 0 ) );
      Image< D > sobel_y( Gradient::DirectionalSobel( img, 1 ) );
      scale = std::max( scale, 1.0f );
      float inv_scale = 1.0f / scale;
      COMMENT( "Create vectors of circle for the nonzero pixels in the edge image and the centers of circles" <<
               " which could be detected.", 0 );
      Vector< Point > non_zero_pxl;
      COMMENT( "Accumulate circle evidence for each edge pixel.", 0 );
      const size_t sht_const = 10;
      const size_t mul_const = 1 << 10; /* 1024; */
      Image< D > subimg( Geometrics::Scale( img, inv_scale, true ) );
      Image< int > accum( subimg.size( 0 ) + 2, subimg.size( 1 ) + 2 );
      size_t acc_rows = accum.size( 1 ) - 2;
      size_t acc_cols = accum.size( 0 ) - 2;
      for( size_t y = 0; y < img.size( 1 ); ++y ) {
        for( size_t x = 0; x < img.size( 0 ); ++x ) {
          if( ( filtered_canny( x, y ) == 0 ) || ( ( sobel_x( x, y ) == 0 ) && ( sobel_y( x, y ) == 0 ) ) ) {
            continue;
          }
          float mag = std::sqrt( sobel_x( x, y ) * sobel_x( x, y ) + sobel_y( x, y ) * sobel_y( x, y ) );
          int sbl_x = static_cast< int >( ( sobel_x( x, y ) * inv_scale ) * mul_const / mag + 0.5f );
          int sbl_y = static_cast< int >( ( sobel_y( x, y ) * inv_scale ) * mul_const / mag + 0.5f );
          int x0 = static_cast< int >( ( x * inv_scale ) * mul_const + 0.5f );
          int y0 = static_cast< int >( ( y * inv_scale ) * mul_const + 0.5f );
          COMMENT( "Step from min_radius to max_radius in both directions of the directional gradient.", 4 );
          for( int k1 = 0; k1 < 2; ++k1 ) {
            int x1 = x0 + min_radius * sbl_x;
            int y1 = y0 + min_radius * sbl_y;
            for( size_t rds = min_radius; rds <= max_radius; ++rds ) {
              int x2 = x1 >> sht_const;
              int y2 = y1 >> sht_const;
              if( ( static_cast< unsigned >( x2 ) >= static_cast< unsigned >( acc_cols ) ) ||
                  ( static_cast< unsigned >( y2 ) >= static_cast< unsigned >( acc_rows ) ) ) {
                break;
              }
              ++accum( x2, y2 );
              x1 += sbl_x;
              y1 += sbl_y;
            }
            sbl_x = -sbl_x;
            sbl_y = -sbl_y;
          }
          non_zero_pxl.push_back( { { ( x * inv_scale ) + 0.5f, ( y * inv_scale ) + 0.5f } } );
        }
      }
      Image< D > nz_image( accum.Dim( ) );
      for( size_t pxl = 0; pxl < non_zero_pxl.size( ); ++pxl ) {
        nz_image( non_zero_pxl[ pxl ].coordinate[ 0 ], non_zero_pxl[ pxl ].coordinate[ 1 ] ) = 255;
      }
      size_t non_zero_pxl_count = non_zero_pxl.size( );
      COMMENT( "non_zero_pxl_count: " << non_zero_pxl_count, 3 );
      if( !non_zero_pxl_count ) {
        BIAL_WARNING( "Image with no gradient." );
        return( Vector< Circle >( ) );
      }
      COMMENT( "Find possible circle centers.", 0 );
      Vector< size_t > centers;
      for( size_t y = 1; y < acc_rows - 1; ++y ) {
        for( size_t x = 1; x < acc_cols - 1; ++x ) {
          if( ( accum( x, y ) > static_cast<int>(detection_threshold) ) &&
              ( accum( x, y ) > accum( x - 1, y ) ) && ( accum( x, y ) > accum( x + 1, y ) ) &&
              ( accum( x, y ) > accum( x, y - 1 ) ) && ( accum( x, y ) > accum( x, y + 1 ) ) ) {
            centers.push_back( x + y * accum.size( 0 ) );
          }
        }
      }
      size_t center_count = centers.size( );
      if( !center_count ) {
        BIAL_WARNING( "No circle centers detected." );
        return( Vector< Circle >( ) );
      }
      COMMENT( "Sort candidate centers in descending order of their accumulator values, so that the centers with the "
               << "most supporting pixels appear first.", 0 );
      std::sort( centers.begin( ), centers.end( ), HoughDecreaseCompare< int >( accum ) );
      COMMENT( "centers: " << centers, 3 );
      COMMENT( "For each found possible center, estimate radius and check support.", 0 );
      Vector< float > distance( non_zero_pxl_count );
      float min_dist2 = std::max( min_dist, scale );
      min_dist2 *= min_dist2;
      Vector< Circle > circles;
      float min_radius2 = static_cast< float >( min_radius * min_radius );
      float max_radius2 = static_cast< float >( max_radius * max_radius );
      for( size_t cnt = 0; cnt < center_count; cnt++ ) {
        int cnt_pos = centers[ cnt ];
        size_t y = cnt_pos / ( acc_cols + 2 );
        size_t x = cnt_pos - y * ( acc_cols + 2 );
        COMMENT( "Calculate circle's center in pixels.", 4 );
        COMMENT( "Check distance with previously detected circles.", 4 );
        float scl_x = static_cast< float >( ( x + 0.5f ) * scale );
        float scl_y = static_cast< float >( ( y + 0.5f ) * scale );
        size_t ccl = 0;
        for( ; ccl < circles.size( ); ++ccl ) {
          if( ( circles[ ccl ].center[ 0 ] - scl_x ) * ( circles[ ccl ].center[ 0 ] - scl_x ) +
              ( circles[ ccl ].center[ 1 ] - scl_y ) * ( circles[ ccl ].center[ 1 ] - scl_y ) < min_dist2 ) {
            break;
          }
        }
        if( ccl < circles.size( ) ) {
          continue;
        }
        COMMENT( "Estimate contours of this circle.", 4 );
        size_t non_zero_pxl_count_ccl = 0;
        for( size_t nz_pxl = 0; nz_pxl < non_zero_pxl_count; ++nz_pxl ) {
          float delta_x = scl_x - non_zero_pxl[ nz_pxl ].coordinate[ 0 ] * scale;
          float delta_y = scl_y - non_zero_pxl[ nz_pxl ].coordinate[ 1 ] * scale;
          float radius_2 = delta_x * delta_x + delta_y * delta_y;
          if( ( min_radius2 <= radius_2 ) && ( radius_2 <= max_radius2 ) ) {
            distance[ non_zero_pxl_count_ccl ] = radius_2;
            ++non_zero_pxl_count_ccl;
          }
        }
        if( non_zero_pxl_count_ccl == 0 ) {
          continue;
        }
        size_t start_cont_pxl = non_zero_pxl_count_ccl - 1;
        for( size_t elm = 0; elm < distance.size( ); ++elm ) {
          distance[ elm ] = std::pow( distance[ elm ], 0.5 );
        }
        COMMENT( "Sort non-zero pixels according to their distance from the center.", 0 );
        std::sort( distance.begin( ), distance.begin( ) + non_zero_pxl_count_ccl, std::greater< float >( ) );
        COMMENT( "Computing the best radius to this center according to the countours found.", 0 );
        float best_rds = 0;
        size_t max_cont_size = 0;
        float start_dist = distance[ non_zero_pxl_count_ccl - 1 ];
        float dist_sum = start_dist;
        for( size_t cont_pxl = non_zero_pxl_count_ccl - 2; cont_pxl <= non_zero_pxl_count_ccl - 2; --cont_pxl ) {
          float dst = distance[ cont_pxl ];
          if( dst > max_radius ) {
            break;
          }
          if( dst - start_dist > scale ) {
            float cur_rds = distance[ ( cont_pxl + start_cont_pxl ) / 2 ];
            if( ( ( start_cont_pxl - cont_pxl ) * best_rds >= max_cont_size * cur_rds ) ||
                ( ( best_rds < FLT_EPSILON ) && ( start_cont_pxl - cont_pxl >= max_cont_size ) ) ) {
              best_rds = cur_rds;
              max_cont_size = start_cont_pxl - cont_pxl;
            }
            start_dist = dst;
            start_cont_pxl = cont_pxl;
            dist_sum = 0;
          }
          dist_sum += dst;
        }
        COMMENT( "Check if the circle has enough support.", 0 );
        if( static_cast< size_t >( max_cont_size ) > detection_threshold ) {
          circles.push_back( Circle( { scl_x, scl_y }, static_cast< float >( best_rds ), Color( 255, 255, 255, 255 ), 
                                     false ) );
          if( circles.size( ) > max_circles ) {
            return( circles );
          }
        }
      }
      return( circles );
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

#ifdef BIAL_EXPLICIT_HoughCircle

  template struct HoughIncreaseCompare< int >;
  template struct HoughDecreaseCompare< int >;
  template struct HoughIncreaseCompare< llint >;
  template struct HoughDecreaseCompare< llint >;
  template struct HoughIncreaseCompare< float >;
  template struct HoughDecreaseCompare< float >;
  template struct HoughIncreaseCompare< double >;
  template struct HoughDecreaseCompare< double >;
  template Vector< Circle > Hough::HoughCircles( Image< int > &img, float scale, float min_dist, size_t min_radius,
                                                 size_t max_radius, float canny_threshold, size_t detection_threshold,
                                                 int canny_levels, int clean_edges );
  template Vector< Circle > Hough::HoughCircles( Image< llint > &img, float scale, float min_dist, size_t min_radius,
                                                 size_t max_radius, float canny_threshold, size_t detection_threshold,
                                                 int canny_levels, int clean_edges );
  template Vector< Circle > Hough::HoughCircles( Image< float > &img, float scale, float min_dist, size_t min_radius,
                                                 size_t max_radius, float canny_threshold, size_t detection_threshold,
                                                 int canny_levels, int clean_edges );
  template Vector< Circle > Hough::HoughCircles( Image< double > &img, float scale, float min_dist, size_t min_radius,
                                                 size_t max_radius, float canny_threshold, size_t detection_threshold,
                                                 int canny_levels, int clean_edges );

#endif

}

#endif

#endif
