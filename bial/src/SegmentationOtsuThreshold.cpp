 /* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Jul/12 
 * @brief: Segmentation by Otsu thresholding. 
 */

#ifndef BIALSEGMENTATIONOTSUTHRESHOLD_C
#define BIALSEGMENTATIONOTSUTHRESHOLD_C

#include "SegmentationOtsuThreshold.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_SegmentationOtsuThreshold )
#define BIAL_EXPLICIT_SegmentationOtsuThreshold
#endif
#if defined ( BIAL_EXPLICIT_SegmentationOtsuThreshold ) || ( BIAL_IMPLICIT_BIN )

#include "Image.hpp"
#include "Histogram.hpp"
#include "Signal.hpp"
#include "SignalOtsuThreshold.hpp"

namespace Bial {

  template< class D >
  D Segmentation::OtsuThreshold( const Image< D > &img ) {
    try {
      Signal histogram;
      if( static_cast< D >( 1.5 ) == static_cast< D >( 1.0 ) ) {
        histogram = SignalType::Histogram( img );
      }
      else {
        histogram = SignalType::Histogram( img, ( img.Maximum( ) - img.Minimum( ) ) / 10000.0 );
      }
      return( histogram.Data( SignalOp::OtsuThreshold( histogram ) ) );
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
  Image< D > Segmentation::Otsu( const Image< D > &img ) {
    try {
      D limiar = Segmentation::OtsuThreshold( img );
      Image< D > res( img );
      for( size_t pxl = 0; pxl < res.Size( ); ++pxl ) {
        if( res( pxl ) > limiar ) {
          res( pxl ) = 1;
        }
        else {
          res( pxl ) = 0;
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

#ifdef BIAL_EXPLICIT_SegmentationOtsuThreshold

  template int Segmentation::OtsuThreshold( const Image< int > &img );
  template Image< int > Segmentation::Otsu( const Image< int > &img );
  template llint Segmentation::OtsuThreshold( const Image< llint > &img );
  template Image< llint > Segmentation::Otsu( const Image< llint > &img );
  template float Segmentation::OtsuThreshold( const Image< float > &img );
  template Image< float > Segmentation::Otsu( const Image< float > &img );
  template double Segmentation::OtsuThreshold( const Image< double > &img );
  template Image< double > Segmentation::Otsu( const Image< double > &img );

#endif

}

#endif

#endif
