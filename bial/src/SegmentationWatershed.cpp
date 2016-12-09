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

namespace Bial {

  template< class D >
  Image< int > Segmentation::Watershed( Image< D > &gradient, float radius ) {
    try {
      Adjacency spheric = AdjacencyType::HyperSpheric( radius, gradient.Dims( ) );
      Vector< bool > local_minima = Intensity::LocalMinima( gradient, spheric );
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
      D min_val = gradient.Minimum( );
      size_t value_range = gradient.Maximum( ) - gradient.Minimum( ) + 1;
      if( seeds.size( ) != gradient.size( ) ) {
        std::string msg( BIAL_ERROR( "Gradient image and seed vector must have the same number of elements." ) );
        throw( std::logic_error( msg ) );
      }
      Adjacency spheric = AdjacencyType::HyperSpheric( 1.0, gradient.Dims( ) );
      Image< int > label( gradient );
      label.Set( 0.0 );
      MaxPathFunction< Image, D > max_function( gradient, 1.0 );
      size_t size = gradient.size( );
      COMMENT( "Setting seeds. Image size: " << size, 0 );
      for( size_t pxl = 0; pxl < size; ++pxl ) {
        if( seeds[ pxl ] )
          gradient[ pxl ] += 1.0;
        else
          gradient[ pxl ] = std::numeric_limits< D >::max( );
      }
      COMMENT( "Running IFT.", 0 );
      //ImageIFT< D > ift( gradient, spheric, &max_function, &seeds, &label, static_cast< Image< int >* >( nullptr ),
      //                   true, static_cast< D >( 1.0 ), true );
      //ImageIFT< D > ift( gradient, spheric, &max_function, min_val, value_range, &seeds, &label,
      //                   static_cast< Image< int >* >( nullptr ), true, true );
      //ift.Run( );

      FastIncreasingFifoBucketQueue queue( size, min_val, value_range );
      COMMENT( "Initializing data in path function.", 1 );
      max_function.Initialize( gradient, &label, nullptr, true );
      COMMENT( "Initializing the maps.", 1 );
      
      COMMENT( "Inserting seeds.", 1 );
      for( size_t it = 0; it < size; ++it ) {
        if( seeds[ it ] )
          queue.Insert( it, gradient( it ) );
      }

      COMMENT( "Running Image IFT. Queue: " << ( queue.Empty( ) ? "empty" : "not empty" ), 0 );
      while( !queue.Empty( ) ) {
        COMMENT( "Initializing removed data.", 4 );
        int index = queue.Remove( );
        bool capable = max_function.RemoveLabel( index, queue.State( index ) );
        COMMENT( "Index: " << index << ", value: " << gradient[ index ] << ", is capable: " <<
                 ( capable ? "true" : "false" ), 4 );
        queue.Finished( index );
        if( capable ) {
          for( AdjacencyIterator adj = begin( spheric, gradient, index ); *adj < size; ++adj ) {
            size_t adj_index = *adj;
            if( queue.State( adj_index ) != BucketState::REMOVED ) { // Verificar se acelera.
              COMMENT( "Conquering: " << adj_index, 4 );
              D previous_value = gradient[ adj_index ];
              COMMENT( "previous_value: " << previous_value, 4 );
              if( max_function.Propagate( index, adj_index ) ) {
                COMMENT( "propagated!", 4 );
                queue.Update( adj_index, previous_value, gradient[ adj_index ] );
                COMMENT( "queue updated!", 4 );
                label( adj_index ) = label( index );
                COMMENT( "function updated!", 4 );
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
    try {
      if( ( obj_seeds.size( ) == 0 ) || ( bkg_seeds.size( ) == 0 ) ) {
        std::string msg( BIAL_ERROR( "Object seeds and background seeds should not be empty vectors. Given: " +
                                     std::to_string( obj_seeds.size( ) ) + ", " +
                                     std::to_string( bkg_seeds.size( ) ) ) );
        throw( std::logic_error( msg ) );
      }
      D min_val = gradient.Minimum( );
      size_t value_range = gradient.Maximum( ) - gradient.Minimum( ) + 1;
      Adjacency spheric = AdjacencyType::HyperSpheric( 1.0, gradient.Dims( ) );
      Image< int > label( gradient );
      size_t size = gradient.size( );
      Vector< bool > seeds( size, false );
      label.Set( 0.0 );
      MaxPathFunction< Image, D > max_function( gradient, 1.0 );
      for( size_t elm = 0; elm < obj_seeds.size( ); ++elm ) {
        label[ obj_seeds[ elm ] ] = 1;
        seeds[ obj_seeds[ elm ] ] = true;
        gradient[ obj_seeds[ elm ] ] += 1.0;
      }
      for( size_t elm = 0; elm < bkg_seeds.size( ); ++elm ) {
        label[ bkg_seeds[ elm ] ] = 0;
        seeds[ bkg_seeds[ elm ] ] = true;
        gradient[ bkg_seeds[ elm ] ] += 1.0;
      }
      for( size_t elm = 0; elm < size; ++elm ) {
        if( !seeds[ elm ] )
          gradient[ elm ] = std::numeric_limits< D >::max( );
      }
      //ImageIFT< D > ift( gradient, spheric, &max_function, &seeds, &label, static_cast< Image< int >* >( nullptr ),
      //                   false, static_cast< D >( 1.0 ), true );
      ImageIFT< D > ift( gradient, spheric, &max_function, min_val, value_range, &seeds, &label,
                         static_cast< Image< int >* >( nullptr ), true, true );
      ift.Run( );
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
