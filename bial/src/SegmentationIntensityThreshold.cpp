 /* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Jul/12 
 * @brief: Segmentation by intensity thresholding.
 */

#ifndef BIALSEGMENTATIONINTENSITYTHRESHOLD_C
#define BIALSEGMENTATIONINTENSITYTHRESHOLD_C

#include "SegmentationIntensityThreshold.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_SegmentationIntensityThreshold )
#define BIAL_EXPLICIT_SegmentationIntensityThreshold
#endif
#if defined ( BIAL_EXPLICIT_SegmentationIntensityThreshold ) || ( BIAL_IMPLICIT_BIN )

#include "Image.hpp"

namespace Bial {

  template< class D >
  void Segmentation::IntensityThreshold( Image< D > &img, const D Tmin, const D Tmax ) {
    try {
      if( Tmin > Tmax ) {
        std::string msg( BIAL_ERROR( "Lower threshold is greater than the higher threshold. Given: lower: " +
                                     std::to_string( Tmin ) + ", higher: " + std::to_string( Tmax ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
        if( ( img[ pxl ] < Tmin ) || ( img[ pxl ] > Tmax ) ) {
          img[ pxl ] = 0.0;
        }
        else {
          img[ pxl ] = 1.0;
        }
      }
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

#ifdef BIAL_EXPLICIT_SegmentationIntensityThreshold

  template void Segmentation::IntensityThreshold( Image< int > &img, const int Tmin, const int Tmax );
  template void Segmentation::IntensityThreshold( Image< llint > &img, const llint Tmin, const llint Tmax );
  template void Segmentation::IntensityThreshold( Image< float > &img, const float Tmin, const float Tmax );
  template void Segmentation::IntensityThreshold( Image< double > &img, const double Tmin, const double Tmax );

#endif

}

#endif

#endif
