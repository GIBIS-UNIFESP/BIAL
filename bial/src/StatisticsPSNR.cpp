/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Dec/04
 * @brief Dice metric.
 */

#ifndef BIALSTATISTICSMINIMUMSQUAREERROR_C
#define BIALSTATISTICSMINIMUMSQUAREERROR_C

#include "StatisticsPSNR.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_StatisticsPSNR )
#define BIAL_EXPLICIT_StatisticsPSNR
#endif
#if defined ( BIAL_EXPLICIT_StatisticsPSNR ) || ( BIAL_IMPLICIT_BIN )

#include "Image.hpp"
#include "StatisticsMeanSquareError.hpp"

namespace Bial {

  template< template< class D > class C, class D >
  float Statistics::PSNR( const C< D > &src_img, const C< D > &tgt_img ) {
    try {
      if( src_img.size( ) != tgt_img.size( ) ) {
        std::string msg( BIAL_ERROR( "Input data dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      float mse = Statistics::MeanSquareError( src_img, tgt_img );
      D src_max = src_img.Maximum( );
      D tgt_max = tgt_img.Maximum( );
      float max = std::max( src_max, tgt_max );
      float psnr = 20 * std::log10( max ) - 10 * std::log10( mse );
      return( psnr );
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

#ifdef BIAL_EXPLICIT_StatisticsPSNR

  template float Statistics::PSNR( const Image< int > &src_img, const Image< int > &tgt_img );
  template float Statistics::PSNR( const Image< llint > &src_img, const Image< llint > &tgt_img );
  template float Statistics::PSNR( const Image< float > &src_img, const Image< float > &tgt_img );
  template float Statistics::PSNR( const Image< double > &src_img, const Image< double > &tgt_img );

  template float Statistics::PSNR( const Vector< int > &src_img, const Vector< int > &tgt_img );
  template float Statistics::PSNR( const Vector< llint > &src_img, const Vector< llint > &tgt_img );
  template float Statistics::PSNR( const Vector< float > &src_img, const Vector< float > &tgt_img );
  template float Statistics::PSNR( const Vector< double > &src_img, const Vector< double > &tgt_img );

#endif

}

#endif

#endif
