 /* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Jul/12 
 * @brief: Segmentation with watershed algorithm.
 */

#ifndef BIALSEGMENTATIONWATERSHED_C
#define BIALSEGMENTATIONWATERSHED_C

#include "SegmentationWatershed.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_SegmentationWatershed )
#define BIAL_EXPLICIT_SegmentationWatershed
#endif
#if defined ( BIAL_EXPLICIT_SegmentationWatershed ) || ( BIAL_IMPLICIT_BIN )

#include "AdjacencyIterator.hpp"
#include "AdjacencyRound.hpp"
#include "FastIncreasingFifoBucketQueue.hpp"
#include "Image.hpp"
#include "ImageIFT.hpp"
#include "IntensityLocals.hpp"
#include "MaxPathFunction.hpp"
#include "SimpleBucketQueue.hpp"

namespace Bial {

  template< class D >
  Image< int > Segmentation::Watershed( Image< D > &gradient, float radius ) {
    try {
      Adjacency spheric( AdjacencyType::HyperSpheric( radius, gradient.Dims( ) ) );
      Vector< bool > local_minima( Intensity::LocalMinima( gradient, spheric ) );
      return( Segmentation::Watershed( gradient, local_minima ) );
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
  Image< int > Segmentation::Watershed( Image< D > &gradient, const Vector< bool > &seeds ) {
    try {
      COMMENT( "Computing initial values from input gradient image.", 0 );
      D max_val = gradient.Maximum( );
      size_t size = gradient.size( );
      size_t x_size = gradient.size( 0 );
      size_t y_size = gradient.size( 1 );
      size_t z_size = gradient.size( 2 );
      size_t x_size_1 = x_size - 1;
      size_t y_size_1 = y_size - 1;
      size_t z_size_1 = z_size - 1;
      IF_DEBUG( seeds.size( ) != size ) {
        std::string msg( BIAL_ERROR( "Gradient image and seed vector must have the same number of elements." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Creating images.", 0 );
      Image< int > label( gradient.Dim( ), gradient.PixelSize( ) );
      Image< D > value( gradient );
      COMMENT( "Setting seeds. Image size: " << size << ". Max val: " << max_val, 0 );
      SimpleBucketQueue queue( size, max_val + 2 );
      for( size_t pxl = 0; pxl < size; ++pxl ) {
        if( seeds[ pxl ] ) {
          ++value[ pxl ];
          queue.Insert( pxl, value[ pxl ] );
        }
        else
          value[ pxl ] = std::numeric_limits< D >::max( );
      }
      COMMENT( "Running IFT.", 0 );
      Adjacency spheric( AdjacencyType::HyperSpheric( 1.0, gradient.Dims( ) ) );
      AdjacencyIterator adj_itr( gradient, spheric );
      size_t adj_size = spheric.size( );
      size_t adj_index;
      Vector< size_t > src_vector( 3 );
      Vector< size_t > adj_vector( 3 );
      size_t next_label = 1;
      size_t xy_size = gradient.Displacement( 1 );
      COMMENT( "Running Image IFT. Queue: " << ( queue.Empty( ) ? "empty" : "not empty" ), 0 );
      while( !queue.Empty( ) ) {
        COMMENT( "Initializing removed data.", 4 );
        int index = queue.Remove( );
        if( queue.State( index ) == BucketState::INSERTED ) {
          value[ index ] = gradient[ index ];
          label[ index ] = next_label;
          ++next_label;
          queue.ResetMinimum( );
        }
        COMMENT( "Index: " << index << ", value: " << value[ index ], 4 );
        queue.Finished( index );
        div_t div_index_by_xy = std::div( static_cast< int >( index ), static_cast< int >( xy_size ) );
        div_t div_rem_by_x =  std::div( div_index_by_xy.rem, static_cast< int >( x_size ) );
        src_vector[ 0 ] = static_cast< size_t >( div_rem_by_x.rem );
        src_vector[ 1 ] = static_cast< size_t >( div_rem_by_x.quot );
        src_vector[ 2 ] = static_cast< size_t >( div_index_by_xy.quot );
        if( ( src_vector[ 0 ] > 0 ) && ( src_vector[ 0 ] < x_size_1 ) && ( src_vector[ 1 ] > 0 ) && 
            ( src_vector[ 1 ] < y_size_1 ) && ( src_vector[ 2 ] > 0 ) && ( src_vector[ 2 ] < z_size_1 ) ) {
          COMMENT( "For pixels that are not in the image border, there is no need for adjacency validation.", 4 );
          for( size_t adj = 0; adj < adj_size; ++adj ) {
            adj_index = adj_itr( index, adj );
            if( queue.State( adj_index ) != BucketState::REMOVED ) {
              COMMENT( "Conquering: " << adj_index, 4 );
              D previous_value = value[ adj_index ];
              COMMENT( "previous_value: " << previous_value, 4 );
              D arc_weight = gradient( adj_index );
              D prp_value = std::max( value( index ), arc_weight );
              if( previous_value > prp_value ) {
                COMMENT( "propagated!", 4 );
                value( adj_index ) = prp_value;
                label( adj_index ) = label( index );
                queue.Update( adj_index, previous_value, prp_value );
                COMMENT( "queue updated!", 4 );
              }
            }
          }
        }
        else {
          COMMENT( "For pixels in the image border, adjacency must be validated.", 4 );
          for( size_t adj = 0; adj < adj_size; ++adj ) {
            adj_index = adj_itr( index, adj );
            if( ( adj_itr.AdjVct( src_vector, adj, adj_vector ) ) &&
                ( queue.State( adj_index ) != BucketState::REMOVED ) ) {
              COMMENT( "Conquering: " << adj_index, 4 );
              D previous_value = gradient[ adj_index ];
              COMMENT( "previous_value: " << previous_value, 4 );
              D arc_weight = gradient( adj_index );
              D prp_value = std::max( value( index ), arc_weight );
              if( previous_value > prp_value ) {
                COMMENT( "propagated!", 4 );
                value( adj_index ) = prp_value;
                label( adj_index ) = label( index );
                queue.Update( adj_index, previous_value, prp_value );
                COMMENT( "queue updated!", 4 );
              }
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
  Image< int > Segmentation::Watershed( Image< D > &gradient, const Vector< size_t > &obj_seeds,
                                        const Vector< size_t > &bkg_seeds ) {
    IF_DEBUG( ( obj_seeds.size( ) == 0 ) || ( bkg_seeds.size( ) == 0 ) ) {
      std::string msg( BIAL_ERROR( "Object seeds and background seeds should not be empty vectors. Given: " +
                                   std::to_string( obj_seeds.size( ) ) + ", " +
                                   std::to_string( bkg_seeds.size( ) ) ) );
      throw( std::logic_error( msg ) );
    }
    try {
      COMMENT( "Computing initial values from input gradient image.", 0 );
      D max_val = gradient.Maximum( );
      size_t size = gradient.size( );
      size_t x_size = gradient.size( 0 );
      size_t y_size = gradient.size( 1 );
      size_t z_size = gradient.size( 2 );
      size_t x_size_1 = x_size - 1;
      size_t y_size_1 = y_size - 1;
      size_t z_size_1 = z_size - 1;
      COMMENT( "Creating images.", 0 );
      Image< int > label( gradient.Dim( ), gradient.PixelSize( ) );
      Image< D > value( gradient.Dim( ), gradient.PixelSize( ) );
      COMMENT( "Setting seeds. Image size: " << size << ". Max val: " << max_val, 0 );
      SimpleBucketQueue queue( size, max_val + 2 );
      for( size_t pxl = 0; pxl < size; ++pxl )
        gradient[ pxl ] = std::numeric_limits< D >::max( );
      for( size_t elm = 0; elm < obj_seeds.size( ); ++elm ) {
        size_t obj_elm = obj_seeds[ elm ];
        label[ obj_elm ] = 1;
        ++value[ obj_elm ];
        queue.Insert( obj_elm, value[ obj_elm ] );
      }
      for( size_t elm = 0; elm < bkg_seeds.size( ); ++elm ) {
        size_t bkg_elm = bkg_seeds[ elm ];
        label[ bkg_elm ] = 0;
        ++value[ bkg_elm ];
        queue.Insert( bkg_elm, value[ bkg_elm ] );
      }
      COMMENT( "Running IFT.", 0 );
      size_t xy_size = gradient.Displacement( 1 );
      Adjacency spheric( AdjacencyType::HyperSpheric( 1.0, gradient.Dims( ) ) );
      AdjacencyIterator adj_itr( gradient, spheric );
      size_t adj_size = spheric.size( );
      size_t adj_index;
      Vector< size_t > src_vector( 3 );
      Vector< size_t > adj_vector( 3 );
      COMMENT( "Running Image IFT. Queue: " << ( queue.Empty( ) ? "empty" : "not empty" ), 0 );
      while( !queue.Empty( ) ) {
        COMMENT( "Initializing removed data.", 4 );
        int index = queue.Remove( );
        if( queue.State( index ) == BucketState::INSERTED ) {
          value[ index ] = gradient[ index ];
          queue.ResetMinimum( );
        }
        COMMENT( "Index: " << index << ", value: " << value[ index ], 4 );
        queue.Finished( index );
        div_t div_index_by_xy = std::div( static_cast< int >( index ), static_cast< int >( xy_size ) );
        div_t div_rem_by_x =  std::div( div_index_by_xy.rem, static_cast< int >( x_size ) );
        src_vector[ 0 ] = static_cast< size_t >( div_rem_by_x.rem );
        src_vector[ 1 ] = static_cast< size_t >( div_rem_by_x.quot );
        src_vector[ 2 ] = static_cast< size_t >( div_index_by_xy.quot );
        if( ( src_vector[ 0 ] > 0 ) && ( src_vector[ 0 ] < x_size_1 ) && ( src_vector[ 1 ] > 0 ) && 
            ( src_vector[ 1 ] < y_size_1 ) && ( src_vector[ 2 ] > 0 ) && ( src_vector[ 2 ] < z_size_1 ) ) {
          COMMENT( "For pixels that are not in the image border, there is no need for adjacency validation.", 4 );
          for( size_t adj = 0; adj < adj_size; ++adj ) {
            adj_index = adj_itr( index, adj );
            if( queue.State( adj_index ) != BucketState::REMOVED ) {
              COMMENT( "Conquering: " << adj_index, 4 );
              D previous_value = value[ adj_index ];
              COMMENT( "previous_value: " << previous_value, 4 );
              D arc_weight = gradient( adj_index );
              D prp_value = std::max( value( index ), arc_weight );
              if( previous_value > prp_value ) {
                COMMENT( "propagated!", 4 );
                value( adj_index ) = prp_value;
                label( adj_index ) = label( index );
                queue.Update( adj_index, previous_value, prp_value );
                COMMENT( "queue updated!", 4 );
              }
            }
          }
        }
        else {
          COMMENT( "For pixels in the image border, adjacency must be validated.", 4 );
          for( size_t adj = 0; adj < adj_size; ++adj ) {
            adj_index = adj_itr( index, adj );
            if( ( adj_itr.AdjVct( src_vector, adj, adj_vector ) ) &&
                ( queue.State( adj_index ) != BucketState::REMOVED ) ) {
              COMMENT( "Conquering: " << adj_index, 4 );
              D previous_value = gradient[ adj_index ];
              COMMENT( "previous_value: " << previous_value, 4 );
              D arc_weight = gradient( adj_index );
              D prp_value = std::max( value( index ), arc_weight );
              if( previous_value > prp_value ) {
                COMMENT( "propagated!", 4 );
                value( adj_index ) = prp_value;
                label( adj_index ) = label( index );
                queue.Update( adj_index, previous_value, prp_value );
                COMMENT( "queue updated!", 4 );
              }
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

#ifdef BIAL_EXPLICIT_SegmentationWatershed

  template Image< int > Segmentation::Watershed( Image< int > &gradient, float radius );
  template Image< int > Segmentation::Watershed( Image< int > &gradient, const Vector< bool > &seeds );
  template Image< int > Segmentation::Watershed( Image< int > &gradient, const Vector< size_t > &obj_seeds,
                                                 const Vector< size_t > &bkg_seeds );
  template Image< int > Segmentation::Watershed( Image< llint > &gradient, float radius );
  template Image< int > Segmentation::Watershed( Image< llint > &gradient, const Vector< bool > &seeds );
  template Image< int > Segmentation::Watershed( Image< llint > &gradient, const Vector< size_t > &obj_seeds,
                                                 const Vector< size_t > &bkg_seeds );
  template Image< int > Segmentation::Watershed( Image< float > &gradient, float radius );
  template Image< int > Segmentation::Watershed( Image< float > &gradient, const Vector< bool > &seeds );
  template Image< int > Segmentation::Watershed( Image< float > &gradient, const Vector< size_t > &obj_seeds,
                                                 const Vector< size_t > &bkg_seeds );
  template Image< int > Segmentation::Watershed( Image< double > &gradient, float radius );
  template Image< int > Segmentation::Watershed( Image< double > &gradient, const Vector< bool > &seeds );
  template Image< int > Segmentation::Watershed( Image< double > &gradient, const Vector< size_t > &obj_seeds,
                                                 const Vector< size_t > &bkg_seeds );

#endif

}

#endif

#endif
