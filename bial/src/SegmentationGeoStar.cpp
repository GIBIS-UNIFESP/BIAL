 /* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Jul/12 
 * @brief: Segmentation with oriented geodesic star algorithm. 
 *         Combines geodesic distance and sum of arcs with orientation.
 */

#ifndef BIALSEGMENTATIONGEOSTAR_C
#define BIALSEGMENTATIONGEOSTAR_C

#include "SegmentationGeoStar.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_SegmentationGeoStar )
#define BIAL_EXPLICIT_SegmentationGeoStar
#endif
#if defined ( BIAL_EXPLICIT_SegmentationGeoStar ) || ( BIAL_IMPLICIT_BIN )

#include "AdjacencyIterator.hpp"
#include "AdjacencyRound.hpp"
#include "GeoSumPathFunction.hpp"
#include "GradientDirectional.hpp"
#include "GrowingBucketQueue.hpp"
#include "Image.hpp"
#include "ImageIFT.hpp"
#include "OrientedExternPathFunction.hpp"
#include "OrientedInternPathFunction.hpp"
#include "RotatingBucketQueue.hpp"

namespace Bial {

  template< class D >
  Image< D > Segmentation::GeodesicStar( const Image< D > &grad, const Vector< size_t > &obj_seeds,
                                         const Vector< size_t > &bkg_seeds, double beta ) {
    try {
      IF_DEBUG( ( beta < 0.0 ) || ( beta > 2.0 ) ) {
        std::string msg( BIAL_ERROR( "Beta must be set between 0.0 and 2.0. Given value: " +
                                     std::to_string( beta ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      IF_DEBUG( ( obj_seeds.size( ) == 0 ) || ( bkg_seeds.size( ) == 0 ) ) {
        std::string msg( BIAL_ERROR( "Must have at last one object and one background seed. Given object seeds: " +
                                     std::to_string( obj_seeds.size( ) ) + ", background seeds: " +
                                     std::to_string( bkg_seeds.size( ) ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Initializing maps.", 0 );
      Image< int > pred( grad );
      Image< D > value( grad );
      size_t size = grad.size( );
      D max_val = 2 * grad.Maximum( );
      size_t x_size = grad.size( 0 );
      size_t y_size = grad.size( 1 );
      size_t z_size = grad.size( 2 );
      size_t x_size_1 = x_size - 1;
      size_t y_size_1 = y_size - 1;
      size_t z_size_1 = z_size - 1;
      size_t xy_size = grad.Displacement( 1 );
      COMMENT( "Pre-computing beta powers in order in speedup the computation.", 0 );
      Vector< double > beta_pows( max_val, 0.0 );
      for( size_t elm = 0; elm < max_val; ++elm )
        beta_pows[ elm ] = std::pow( elm, beta );
      RotatingBucketQueue queue( size, beta_pows[ max_val - 1 ] + 3 );

      COMMENT( "FIRST STEP. Running with foreground seeds.", 0 );
      for( size_t elm = 0; elm < size; ++elm )
        value[ elm ] = std::numeric_limits< D >::max( );
      for( size_t idx = 0; idx < obj_seeds.size( ); ++idx ) {
        size_t elm = obj_seeds( idx );
        value[ elm ] = 0;
        pred[ elm ] = -1;
        queue.Insert( elm, value[ elm ] );
      }
      COMMENT( "Queue size: " << queue.Elements( ), 0 );
      Adjacency adjacency( AdjacencyType::HyperSpheric( 1.5, grad.Dims( ) ) );
      AdjacencyIterator adj_itr( value, adjacency );
      size_t adj_size = adjacency.size( );
      COMMENT( "Distance vector for faster computation.", 0 );
      Vector< double > dists( adj_size, 0.0 );
      for( size_t elm = 0; elm < adj_size; ++elm )
        dists[ elm ] = std::sqrt( adjacency( elm, 0 ) * adjacency( elm, 0 ) +
                                  adjacency( elm, 1 ) * adjacency( elm, 1 ) +
                                  adjacency( elm, 2 ) * adjacency( elm, 2 ) );
      size_t adj_index;
      Vector< size_t > src_coords( 3 );
      Vector< size_t > adj_coords( 3 );
      COMMENT( "Running geodesic star restriction IFT.", 0 );
      while( !queue.Empty( ) ) {
        COMMENT( "Initializing removed data.", 3 );
        size_t src_index = queue.Remove( );
        COMMENT( "Src_Index: " << src_index << ", value: " << value[ src_index ], 3 );
        queue.Finished( src_index );
        COMMENT( "Computing src_index coordinates.", 3 );
        div_t div_index_by_xy = std::div( static_cast< int >( src_index ), static_cast< int >( xy_size ) );
        div_t div_rem_by_x =  std::div( div_index_by_xy.rem, static_cast< int >( x_size ) );
        src_coords[ 0 ] = static_cast< size_t >( div_rem_by_x.rem );
        src_coords[ 1 ] = static_cast< size_t >( div_rem_by_x.quot );
        src_coords[ 2 ] = static_cast< size_t >( div_index_by_xy.quot );
        COMMENT( "Checking if pixel is in the border of the image.", 3 );
        if( ( src_coords[ 0 ] > 0 ) && ( src_coords[ 0 ] < x_size_1 ) && ( src_coords[ 1 ] > 0 ) && 
            ( src_coords[ 1 ] < y_size_1 ) && ( src_coords[ 2 ] > 0 ) && ( src_coords[ 2 ] < z_size_1 ) ) {
          COMMENT( "For pixels that are not in the image border, there is no need for adjacency validation.", 3 );
          for( size_t adj = 0; adj < adj_size; ++adj ) {
            adj_index = adj_itr( src_index, adj );
            // SegmentationGeoStar: Check if the following comparison is worth doing.
            if( ( queue.State( adj_index ) != BucketState::REMOVED ) && ( value[ src_index ] < value[ adj_index ] ) ) {
              COMMENT( "Conquering: " << adj_index, 3 );
              D curr_adj_value = value[ adj_index ];
              COMMENT( "curr_adj_value: " << curr_adj_value, 3 );
              COMMENT( "Computing arc weight.", 3 );
              D arc_weight = grad[ src_index ] + grad[ adj_index ];
              COMMENT( "Suppressing non-zero.", 3 );
              ++arc_weight;
              COMMENT( "Computing spacial distance.", 3 );
              double distance = dists[ adj ];
              COMMENT( "Propagated value.", 3 );
              D prp_value = static_cast< D >( value[ src_index ] + beta_pows[ arc_weight ] - 1.0 + distance );
              COMMENT( "Updating value.", 3 );
              if( curr_adj_value > prp_value ) {
                value[ adj_index ] = prp_value;
                pred[ adj_index ] = src_index;
                queue.Update( adj_index, curr_adj_value, value[ adj_index ] );
              }
            }
          }
        }
        else {
          COMMENT( "For pixels in the image border, adjacency must be validated.", 3 );
          for( size_t adj = 0; adj < adj_size; ++adj ) {
            COMMENT( "Warning: The following if contains an assignment. This is on purpose.", 3 );
            if( ( adj_itr.AdjVct( src_coords, adj, adj_coords ) ) && ( adj_index = adj_itr( src_index, adj ) ) &&
                ( queue.State( adj_index ) != BucketState::REMOVED ) && ( value[ src_index ] < value[ adj_index ] ) ) {
              COMMENT( "Conquering: " << adj_index, 3 );
              D curr_adj_value = value[ adj_index ];
              COMMENT( "Computing arc weight with curr_adj_value: " << curr_adj_value, 3 );
              D arc_weight = grad[ src_index ] + grad[ adj_index ] + 1;
              COMMENT( "Computing spacial distance.", 3 );
              double distance = dists[ adj ];
              COMMENT( "Propagated value.", 3 );
              D prp_value = static_cast< D >( value[ src_index ] + beta_pows[ arc_weight ] - 1.0 + distance );
              COMMENT( "Updating value.", 3 );
              if( curr_adj_value > prp_value ) {
                value[ adj_index ] = prp_value;
                pred[ adj_index ] = src_index;
                queue.Update( adj_index, curr_adj_value, value[ adj_index ] );
              }
            }
          }
        }
      }

      COMMENT( "SECOND STEP. Running with background and foreground seeds.", 0 );
      queue.ResetMinimum( );
      Image< int > label( grad.Dim( ), grad.PixelSize( ) );
      label.Set( -1 );
      COMMENT( "Setting seeds with background.", 0 );
      for( size_t elm = 0; elm < size; ++elm ) {
        value[ elm ] = std::numeric_limits< D >::max( );
        queue.State( elm, BucketState::NOT_VISITED );
      }
      COMMENT( "Inserting obj seeds:", 0 );
      for( size_t idx = 0; idx < obj_seeds.size( ); ++idx ) {
        size_t elm = obj_seeds( idx );
        label( elm ) = 1;
        value[ elm ] = 0;
        queue.Insert( elm, value[ elm ] );
      }
      COMMENT( "Inserting bkg seeds:", 0 );
      for( size_t idx = 0; idx < bkg_seeds.size( ); ++idx ) {
        size_t elm = bkg_seeds( idx );
        label( elm ) = 0;
        pred( elm ) = -1;
        value[ elm ] = 0;
        queue.Insert( elm, value[ elm ] );
      }
      COMMENT( "Running path function.", 0 );
      while( !queue.Empty( ) ) {
        COMMENT( "Initializing removed data.", 3 );
        size_t index = queue.Remove( );
        COMMENT( "Index: " << index << ", value: " << value[ index ], 3 );
        queue.Finished( index );
        for( size_t adj = 0; adj < adj_size; ++adj ) {
          if( ( adj_itr.AdjIdx( index, adj, adj_index ) ) && ( queue.State( adj_index ) != BucketState::REMOVED ) &&
              ( value[ index ] < value[ adj_index ] ) ) {
            COMMENT( "Conquering: " << adj_index, 3 );
            D curr_adj_value = value[ adj_index ];
            COMMENT( "curr_adj_value: " << curr_adj_value << ". Computing arc weight.", 3 );
            double arc_weight = grad( index ) + grad( adj_index ) + 1;
            COMMENT( "Zero weight edges.", 3 );
            if( ( ( label[ index ] != 0 ) && ( pred[ index ] == adj_index ) ) ||
                ( ( label[ index ] == 0 ) && ( pred[ adj_index ] == index ) ) ) {
              arc_weight = 0;
            }
            ++arc_weight;
            COMMENT( "Propagated value.", 3 );
            D prp_value = static_cast< D >( arc_weight );
            COMMENT( "Updating path value.", 3 );
            if( curr_adj_value > prp_value ) {
              value[ adj_index ] = prp_value;
              label[ adj_index ] = label[ index ];
              queue.Update( adj_index, curr_adj_value, value[ adj_index ] );
            }
          }
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


  template< class D >
  Image< D > Segmentation::OrientedGeodesicStar( const Image< D > &image, const Vector< size_t > &obj_seeds,
                                                 const Vector< size_t > &bkg_seeds, double alpha, double beta ) {
    try {
      IF_DEBUG( ( alpha < -1.0 ) || ( alpha > 1.0 ) ) {
        std::string msg( BIAL_ERROR( "Alpha must be set between -1.0 and 1.0. Given value: " + std::to_string( alpha )
                                     + "." ) );
        throw( std::logic_error( msg ) );
      }
      IF_DEBUG( ( beta < 0.0 ) || ( beta > 4.0 ) ) {
        std::string msg( BIAL_ERROR( "Beta must be set between 0.0 and 4.0. Given value: " +
                                     std::to_string( beta ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      IF_DEBUG( ( obj_seeds.size( ) == 0 ) || ( bkg_seeds.size( ) == 0 ) ) {
        std::string msg( BIAL_ERROR( "Must have at last one object and one background seed. Given object seeds: " +
                                     std::to_string( obj_seeds.size( ) ) + ", background seeds: " +
                                     std::to_string( bkg_seeds.size( ) ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Initializing maps.", 0 );
      Adjacency adj( AdjacencyType::HyperSpheric( 1.5, image.Dims( ) ) );
      Image< D > grad( Gradient::Directional( image, adj ) );
      Image< int > pred( image );
      Image< D > value( grad );
      Image< int > label( image.Dim( ), image.PixelSize( ) );
      label.Set( -1 );
      size_t size = image.size( );
      COMMENT( "FIRST STEP. Running with foreground seeds.", 0 );
      GeodesicRestrictionPathFunction< D > geo_path( grad, nullptr, &pred, false, grad, image, adj, alpha, beta );
      GrowingBucketQueue queue( size, 1, geo_path.Increasing( ), true );
      for( size_t elm = 0; elm < size; ++elm )
        grad[ elm ] = std::numeric_limits< D >::max( );
      for( size_t idx = 0; idx < obj_seeds.size( ); ++idx ) {
        size_t elm = obj_seeds( idx );
        label( elm ) = 1;
        grad[ elm ] = 0;
        queue.Insert( elm, grad[ elm ] );
      }
      COMMENT( "Running geodesic star restriction IFT.", 0 );
      ImageIFT< D > ift( grad, adj, &geo_path, &queue );
      ift.Run( );
      COMMENT( "SECOND STEP. Running with background and foreground seeds.", 0 );
      COMMENT( "Setting seeds with background.", 0 );
      for( size_t elm = 0; elm < size; ++elm ) {
        grad[ elm ] = std::numeric_limits< D >::max( );
        queue.State( elm, BucketState::NOT_VISITED );
      }
      COMMENT( "Inserting obj seeds:", 0 );
      for( size_t idx = 0; idx < obj_seeds.size( ); ++idx ) {
        size_t elm = obj_seeds( idx );
        grad[ elm ] = 0;
        queue.Insert( elm, grad[ elm ] );
      }
      COMMENT( "Inserting bkg seeds:", 0 );
      for( size_t idx = 0; idx < bkg_seeds.size( ); ++idx ) {
        size_t elm = bkg_seeds( idx );
        label( elm ) = 0;
        pred( elm ) = -1;
        grad[ elm ] = 0;
        queue.Insert( elm, grad[ elm ] );
      }
      COMMENT( "Running oriented path function.", 0 );
      if( alpha >= 0.0 ) {
        COMMENT( "OrientedExternPathFunction.", 0 );
        OrientedExternPathFunction< D > oriented_path( grad, label, nullptr, false, value, image, &pred, alpha );
        ImageIFT< D > ift( grad, adj, &oriented_path, &queue );
        ift.Run( );
      }
      else {
        COMMENT( "OrientedInternPathFunction.", 0 );
        OrientedInternPathFunction< D > oriented_path( grad, label, nullptr, false, value, image, &pred, -alpha );
        ImageIFT< D > ift( grad, adj, &oriented_path, &queue );
        ift.Run( );
      }
      return( label );




    // try {
    //   COMMENT( "Computing initial values from input gradient image.", 0 );
    //   D max_val = gradient.Maximum( );
    //   size_t size = gradient.size( );
    //   size_t x_size = gradient.size( 0 );
    //   size_t y_size = gradient.size( 1 );
    //   size_t z_size = gradient.size( 2 );
    //   size_t x_size_1 = x_size - 1;
    //   size_t y_size_1 = y_size - 1;
    //   size_t z_size_1 = z_size - 1;
    //   size_t xy_size = gradient.Displacement( 1 );
    //   IF_DEBUG( seeds.size( ) != size ) {
    //     std::string msg( BIAL_ERROR( "Gradient image and seed vector must have the same number of elements." ) );
    //     throw( std::logic_error( msg ) );
    //   }
    //   COMMENT( "Creating adjacency and images.", 0 );
    //   Adjacency spheric( AdjacencyType::HyperSpheric( 1.0, gradient.Dims( ) ) );
    //   Image< int > label( gradient.Dim( ), gradient.PixelSize( ) );
    //   Image< D > value( gradient );
    //   COMMENT( "Setting seeds. Image size: " << size << ". Max val: " << max_val, 0 );
    //   DistanceBucketQueue queue( size, max_val + 2 );
    //   for( size_t pxl = 0; pxl < size; ++pxl ) {
    //     if( seeds[ pxl ] ) {
    //       ++value[ pxl ];
    //       queue.Insert( pxl, value[ pxl ] );
    //     }
    //     else
    //       value[ pxl ] = std::numeric_limits< D >::max( );
    //   }
    //   COMMENT( "Running IFT.", 0 );
    //   AdjacencyIterator adj_itr( gradient, spheric );
    //   size_t adj_size = spheric.size( );
    //   size_t adj_index;
    //   Vector< size_t > src_coords( 3 );
    //   Vector< size_t > adj_coords( 3 );
    //   size_t next_label = 1;
    //   COMMENT( "Running Image IFT. Queue: " << ( queue.Empty( ) ? "empty" : "not empty" ), 0 );
    //   while( !queue.Empty( ) ) {
    //     COMMENT( "Initializing removed data.", 4 );
    //     int index = queue.Remove( );
    //     if( queue.State( index ) == BucketState::INSERTED ) {
    //       value[ index ] = gradient[ index ];
    //       label[ index ] = next_label;
    //       ++next_label;
    //       queue.ResetMinimum( );
    //     }
    //     COMMENT( "Index: " << index << ", value: " << value[ index ], 4 );
    //     queue.Finished( index );
    //     div_t div_index_by_xy = std::div( static_cast< int >( index ), static_cast< int >( xy_size ) );
    //     div_t div_rem_by_x =  std::div( div_index_by_xy.rem, static_cast< int >( x_size ) );
    //     src_coords[ 0 ] = static_cast< size_t >( div_rem_by_x.rem );
    //     src_coords[ 1 ] = static_cast< size_t >( div_rem_by_x.quot );
    //     src_coords[ 2 ] = static_cast< size_t >( div_index_by_xy.quot );
    //     if( ( src_coords[ 0 ] > 0 ) && ( src_coords[ 0 ] < x_size_1 ) && ( src_coords[ 1 ] > 0 ) && 
    //         ( src_coords[ 1 ] < y_size_1 ) && ( src_coords[ 2 ] > 0 ) && ( src_coords[ 2 ] < z_size_1 ) ) {
    //       COMMENT( "For pixels that are not in the image border, there is no need for adjacency validation.", 4 );
    //       for( size_t adj = 0; adj < adj_size; ++adj ) {
    //         adj_index = adj_itr( index, adj );
    //         if( queue.State( adj_index ) != BucketState::REMOVED ) {
    //           COMMENT( "Conquering: " << adj_index, 4 );
    //           D curr_adj_value = value[ adj_index ];
    //           COMMENT( "curr_adj_value: " << curr_adj_value, 4 );
    //           D arc_weight = gradient( adj_index );
    //           D prp_value = std::max( value( index ), arc_weight );
    //           if( curr_adj_value > prp_value ) {
    //             COMMENT( "propagated!", 4 );
    //             value( adj_index ) = prp_value;
    //             label( adj_index ) = label( index );
    //             queue.Update( adj_index, curr_adj_value, prp_value );
    //             COMMENT( "queue updated!", 4 );
    //           }
    //         }
    //       }
    //     }
    //     else {
    //       COMMENT( "For pixels in the image border, adjacency must be validated.", 4 );
    //       for( size_t adj = 0; adj < adj_size; ++adj ) {
    //         adj_index = adj_itr( index, adj );
    //         if( ( adj_itr.AdjVct( src_coords, adj, adj_coords ) ) &&
    //             ( queue.State( adj_index ) != BucketState::REMOVED ) ) {
    //           COMMENT( "Conquering: " << adj_index, 4 );
    //           D curr_adj_value = gradient[ adj_index ];
    //           COMMENT( "curr_adj_value: " << curr_adj_value, 4 );
    //           D arc_weight = gradient( adj_index );
    //           D prp_value = std::max( value( index ), arc_weight );
    //           if( curr_adj_value > prp_value ) {
    //             COMMENT( "propagated!", 4 );
    //             value( adj_index ) = prp_value;
    //             label( adj_index ) = label( index );
    //             queue.Update( adj_index, curr_adj_value, prp_value );
    //             COMMENT( "queue updated!", 4 );
    //           }
    //         }
    //       }
    //     }
    //   }
    //   return( label );
    // }
    // catch( std::bad_alloc &e ) {
    //   std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
    //   throw( std::runtime_error( msg ) );
    // }
    // catch( std::runtime_error &e ) {
    //   std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
    //   throw( std::runtime_error( msg ) );
    // }
    // catch( const std::out_of_range &e ) {
    //   std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
    //   throw( std::out_of_range( msg ) );
    // }
    // catch( const std::logic_error &e ) {
    //   std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
    //   throw( std::logic_error( msg ) );
    // }





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

#ifdef BIAL_EXPLICIT_SegmentationGeoStar

  template 
  Image< int > Segmentation::GeodesicStar( const Image< int > &grad, const Vector< size_t > &obj_seeds,
                                                   const Vector< size_t > &bkg_seeds, double beta );
  template 
  Image< llint > Segmentation::GeodesicStar( const Image< llint > &grad, const Vector< size_t > &obj_seeds,
                                                     const Vector< size_t > &bkg_seeds, double beta );
  template
  Image< float > Segmentation::GeodesicStar( const Image< float > &grad, const Vector< size_t > &obj_seeds,
                                                     const Vector< size_t > &bkg_seeds, double beta );
  template 
  Image< double > Segmentation::GeodesicStar( const Image< double > &grad, const Vector< size_t > &obj_seeds,
                                                      const Vector< size_t > &bkg_seeds, double beta );

  template 
  Image< int > Segmentation::OrientedGeodesicStar( const Image< int > &image, const Vector< size_t > &obj_seeds,
                                                   const Vector< size_t > &bkg_seeds, double alpha, double beta );
  template 
  Image< llint > Segmentation::OrientedGeodesicStar( const Image< llint > &image, const Vector< size_t > &obj_seeds,
                                                     const Vector< size_t > &bkg_seeds, double alpha, double beta );
  template
  Image< float > Segmentation::OrientedGeodesicStar( const Image< float > &image, const Vector< size_t > &obj_seeds,
                                                     const Vector< size_t > &bkg_seeds, double alpha, double beta );
  template 
  Image< double > Segmentation::OrientedGeodesicStar( const Image< double > &image, const Vector< size_t > &obj_seeds,
                                                      const Vector< size_t > &bkg_seeds, double alpha, double beta );

#endif

}

#endif

#endif
