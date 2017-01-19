/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/21 
 * @brief Computes image gradients. 
 */

#ifndef BIALGRADIENTSOBEL_C
#define BIALGRADIENTSOBEL_C

#include "GradientSobel.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_GradientSobel )
#define BIAL_EXPLICIT_GradientSobel
#endif
#if defined ( BIAL_EXPLICIT_GradientSobel ) || ( BIAL_IMPLICIT_BIN )

#include "Correlation.hpp"
#ifdef BIAL_DEBUG
#include "FileImage.hpp"
#endif
#include "Image.hpp"
#include "KernelSobel.hpp"

namespace Bial {

  template< class D >
  Image< D > Gradient::DirectionalSobel( const Image< D > &img, size_t direction ) {
    try {
      size_t dimensions = img.Dims( );
      if( direction > dimensions ) {
        std::string msg( BIAL_ERROR(
				    "Gradient direction should be smaller than the number of dimenions of the image." +
				    " Given direction: " + std::to_string( direction ) + ", dimensions : " +
				    std::to_string( dimensions ) ) );
        throw( std::logic_error( msg ) );
      }
      Kernel krn( KernelType::NormalizedSobel( dimensions, direction ) );
      return( Correlation( img, krn ) );
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
  void Gradient::Sobel( const Image< D > &img, Image< D > *magnitude, Image< int > *direction ) {
    try {
      IF_DEBUG( ( magnitude != nullptr ) && ( img.Dim( ) != magnitude->Dim( ) ) ) {
        std::string msg( BIAL_ERROR( "input image and gradient magnitude size do not match." ) );
        throw( std::logic_error( msg ) );
      }
      IF_DEBUG( ( direction != nullptr ) && ( img.Dim( ) != direction->Dim( ) ) ) {
        std::string msg( BIAL_ERROR( "input image and gradient direction size do not match." ) );
        throw( std::logic_error( msg ) );
      }
      const double TAN_22_5 = std::tan( M_PI / 8.0 ); // 0.414213562;
      size_t dimensions = img.Dims( );

      Vector< Image< D > > dir_sobel;
      for( size_t dir = 0; dir < dimensions; ++dir ) {
        Kernel krn( KernelType::NormalizedSobel( dimensions, dir ) );
        dir_sobel.push_back( Correlation( img, krn ) );
      }
      COMMENT( "Computing Sobel magnitude.", 1 );
      if( magnitude != nullptr ) {
        for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
          ( *magnitude )[ pxl ] = 0.0;
          for( size_t dms = 0; dms < dimensions; ++dms ) {
            ( *magnitude )[ pxl ] += dir_sobel( dms )[ pxl ] * dir_sobel( dms )[ pxl ];
          }
          ( *magnitude )[ pxl ] = std::sqrt( ( *magnitude )[ pxl ] );
        }
      }
      COMMENT( "Computing sobel direction.", 1 );
      if( direction != nullptr ) {
        for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {

          COMMENT( "Finding the maximum gradient direction.", 3 );
          double max_grad_val = 0.0;
          int max_grad_dir = -1; /* Central. */
          for( size_t dms = 0; dms < dimensions; ++dms ) {
            D dir_sbl = ( dir_sobel( dms )[ pxl ] ) > 0 ? dir_sobel( dms )[ pxl ] : -dir_sobel( dms )[ pxl ];
            if( max_grad_val < dir_sbl ) {
              max_grad_val = dir_sbl;
              max_grad_dir = dms;
            }
          }
          COMMENT( "Computing gradient angle.", 3 );
          ( *direction )[ pxl ] = 0;
          if( max_grad_dir == -1 ) {
            continue;
          }
          for( size_t dms = 0; dms < dimensions; ++dms ) {
            D dir_sbl = ( dir_sobel( dms )[ pxl ] > 0 ) ? dir_sobel( dms )[ pxl ] : -dir_sobel( dms )[ pxl ];
            double tangent = dir_sbl / max_grad_val;
            if( tangent > TAN_22_5 ) {
              if( dir_sobel( dms )[ pxl ] > 0 ) {
                ( *direction )[ pxl ] += static_cast< int >( std::pow( 2, dms * 2 ) );
              }
              else {
                ( *direction )[ pxl ] += static_cast< int >( std::pow( 2, dms * 2 + 1 ) );
              }
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

#ifdef BIAL_EXPLICIT_GradientSobel


  template Image< int > Gradient::DirectionalSobel( const Image< int > &img, size_t direction );
  template void Gradient::Sobel( const Image< int > &img, Image< int > *magnitude, Image< int > *direction );

  template Image< llint > Gradient::DirectionalSobel( const Image< llint > &img, size_t direction );
  template void Gradient::Sobel( const Image< llint > &img, Image< llint > *magnitude, Image< int > *direction );

  template Image< float > Gradient::DirectionalSobel( const Image< float > &img, size_t direction );
  template void Gradient::Sobel( const Image< float > &img, Image< float > *magnitude, Image< int > *direction );

  template Image< double > Gradient::DirectionalSobel( const Image< double > &img, size_t direction );
  template void Gradient::Sobel( const Image< double > &img, Image< double > *magnitude, Image< int > *direction );

#endif

}

#endif

#endif
