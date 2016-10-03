/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2013/Jul/12 
 * @brief Gaussian filter.
 */

#ifndef BIALFILTERINGGAUSSIAN_C
#define BIALFILTERINGGAUSSIAN_C

#include "FilteringGaussian.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_FilteringGaussian )
#define BIAL_EXPLICIT_FilteringGaussian
#endif

#if defined ( BIAL_EXPLICIT_FilteringGaussian ) || ( BIAL_IMPLICIT_BIN )

#include "Correlation.hpp"
#ifdef BIAL_DEBUG
#include "FileImage.hpp"
#endif
#include "Image.hpp"
#include "KernelGaussian.hpp"

namespace Bial {

  template< class D >
  Image< D > Filtering::Gaussian( const Image< D > &img, float radius, float std_dev ) {
    try {
      if( radius <= 0.0 ) {
        std::string msg( BIAL_ERROR( "Radius must be greater than 0.0. Given: " + std::to_string( radius ) ) );
        throw( std::logic_error( msg ) );
      }
      if( std_dev <= 0.0 ) {
        std::string msg( BIAL_ERROR( "Standard deviation must be greater than 0.0. Given: " + std::to_string( std_dev ) ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Kernel generation.", 1 );
      Kernel krn = KernelType::Gaussian( img.Dims( ), radius, std_dev );
      COMMENT( "Running correlation.", 1 );
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

#ifdef BIAL_EXPLICIT_FilteringGaussian

  template Image< int > Filtering::Gaussian( const Image< int > &img, float radius, float std_dev );
  template Image< llint > Filtering::Gaussian( const Image< llint > &img, float radius, float std_dev );
  template Image< float > Filtering::Gaussian( const Image< float > &img, float radius, float std_dev );
  template Image< double > Filtering::Gaussian( const Image< double > &img, float radius, float std_dev );

#endif

}


#endif

#endif
