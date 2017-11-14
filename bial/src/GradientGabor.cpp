/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/21 
 * @brief Computes Gabor gradient.
 */

#ifndef BIALGRADIENTGABOR_C
#define BIALGRADIENTGABOR_C

#include "GradientGabor.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_GradientGabor )
#define BIAL_EXPLICIT_GradientGabor
#endif
#if defined ( BIAL_EXPLICIT_GradientGabor ) || ( BIAL_IMPLICIT_BIN )

#include "Correlation.hpp"
#ifdef BIAL_DEBUG
#include "FileImage.hpp"
#endif
#include "Image.hpp"
#include "KernelGabor.hpp"

namespace Bial {

  template< class D >
  void Gradient::Gabor( const Image< D > &img, float sigma, Image< D > *magnitude, Image< int > *direction ) {
    try {
      const double TAN_22_5 = 0.414213562;
      size_t dimensions = img.Dims( );
      Vector< Image< D > > dir_gabor;
      for( size_t dir = 0; dir < dimensions; ++dir ) {
        Kernel krn( KernelType::NormalizedGabor( sigma, dimensions, dir ) );
        dir_gabor.push_back( Correlation( img, krn ) );
      }
      COMMENT( "Computing Gabor magnitude.", 1 );
      if( magnitude != nullptr ) {
        for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
          ( *magnitude )[ pxl ] = 0.0;
          for( size_t dms = 0; dms < dimensions; ++dms )
            ( *magnitude )[ pxl ] += dir_gabor( dms )[ pxl ] * dir_gabor( dms )[ pxl ];
          ( *magnitude )[ pxl ] = std::sqrt( ( *magnitude )[ pxl ] );
        }
      }
      COMMENT( "Computing gabor direction.", 1 );
      if( direction != nullptr ) {
        for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
          COMMENT( "Finding the maximum gradient direction.", 3 );
          double max_grad_val = 0.0;
          int max_grad_dir = -1; /* Central. */
          for( size_t dms = 0; dms < dimensions; ++dms ) {
            D dir_gbr = ( dir_gabor( dms )[ pxl ] > 0 ) ? dir_gabor( dms )[ pxl ] : -dir_gabor( dms )[ pxl ];
            if( max_grad_val < dir_gbr ) {
              max_grad_val = dir_gbr;
              max_grad_dir = dms;
            }
          }
          COMMENT( "Computing gradient angle.", 3 );
          ( *direction )[ pxl ] = 0;
          if( max_grad_dir == -1 )
            continue;
          for( size_t dms = 0; dms < dimensions; ++dms ) {
            D dir_gbr = ( dir_gabor( dms )[ pxl ] > 0 ) ? dir_gabor( dms )[ pxl ] : -dir_gabor( dms )[ pxl ];
            double tangent = dir_gbr / max_grad_val;
            if( tangent > TAN_22_5 ) {
              if( dir_gabor( dms )[ pxl ] > 0 )
                ( *direction )[ pxl ] += static_cast< int >( std::pow( 2, dms * 2 ) );
              else
                ( *direction )[ pxl ] += static_cast< int >( std::pow( 2, dms * 2 + 1 ) );
            }
          }
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

#ifdef BIAL_EXPLICIT_GradientGabor


  template void Gradient::Gabor( const Image< int > &img, float sigma, Image< int > *magnitude, 
                                 Image< int > *direction );
  template void Gradient::Gabor( const Image< llint > &img, float sigma, Image< llint > *magnitude, 
                                 Image< int > *direction );
  template void Gradient::Gabor( const Image< float > &img, float sigma, Image< float > *magnitude, 
                                 Image< int > *direction );
  template void Gradient::Gabor( const Image< double > &img, float sigma, Image< double > *magnitude, 
                                 Image< int > *direction );

#endif

}

#endif

#endif
