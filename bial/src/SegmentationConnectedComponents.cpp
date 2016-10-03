/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/21 
 * @brief Find the connected components and select some among them.
 */

#ifndef BIALSEGMENTATIONCONNECTEDCOMPONENTS_C
#define BIALSEGMENTATIONCONNECTEDCOMPONENTS_C

#include "SegmentationConnectedComponents.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_SegmentationConnectedComponents )
#define BIAL_EXPLICIT_SegmentationConnectedComponents
#endif
#if defined ( BIAL_EXPLICIT_SegmentationConnectedComponents ) || ( BIAL_IMPLICIT_BIN )

#include "Adjacency.hpp"
#ifdef BIAL_DEBUG
#include "FileImage.hpp"
#endif
#include "Histogram.hpp"
#include "Image.hpp"
#include "ImageIFT.hpp"
#include "ConnPathFunction.hpp"
#include "Signal.hpp"

namespace Bial {

  template< class D >
  Image< int > Segmentation::ConnectedComponents( const Image< D > &input, const Adjacency &adj ) {
    try {
      Image< int > label( input.Dim( ), input.PixelSize( ) );
      Image< D > value( input );
      Image< D > handicap( input.Dim( ) );
      ConnPathFunction< Image, D > connection_function( handicap, value );
      value.Set( 1.0 );
      ImageIFT< D > ift( value, adj, &connection_function, static_cast< Vector< bool >* >( nullptr ), &label,
                         static_cast< Image< int >* >( nullptr ), true, static_cast< D >( 1 ), false );
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

  Image< int > Segmentation::RemoveSmallComponents( const Image< int > &label, float fraction ) {
    try {
      Image< int > res( label );
      COMMENT( "Computing the size of each component.", 1 );
      Signal hist( SignalType::Histogram( label ) );
      hist[ 0 ] = 0;
      double max_size = hist.MaximumFrequency( );
      double min_size = max_size * fraction;

      COMMENT( "Fraction: " << fraction << ", min size: " << min_size << ", max size: " << max_size << ".", 2 );
      COMMENT( "Removing components with less than min_size.", 1 );
      for( size_t pxl = 0; pxl < label.size( ); ++pxl ) {
        if( hist[ label[ pxl ] ] < min_size )
          res[ pxl ] = 0;
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

#ifdef BIAL_EXPLICIT_SegmentationConnectedComponents

  template Image< int > Segmentation::ConnectedComponents( const Image< int > &input, const Adjacency &adj );
  template Image< int > Segmentation::ConnectedComponents( const Image< llint > &input, const Adjacency &adj );
  template Image< int > Segmentation::ConnectedComponents( const Image< float > &input, const Adjacency &adj );
  template Image< int > Segmentation::ConnectedComponents( const Image< double > &input, const Adjacency &adj );

#endif

}

#endif

#endif
