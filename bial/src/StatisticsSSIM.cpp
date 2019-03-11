/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Dec/04
 * @brief Dice metric.
 */

#ifndef BIALSTATISTICSSSIM_C
#define BIALSTATISTICSSSIM_C

#include "StatisticsSSIM.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_StatisticsSSIM )
#define BIAL_EXPLICIT_StatisticsSSIM
#endif
#if defined ( BIAL_EXPLICIT_StatisticsSSIM ) || ( BIAL_IMPLICIT_BIN )

#include "Image.hpp"
#include "StatisticsAverage.hpp"
#include "StatisticsStdDev.hpp"
#include "StatisticsCovariance.hpp"

namespace Bial {

  template< template< class D > class C, class D >
  float Statistics::SSIM( const C< D > &src_img, const C< D > &tgt_img ) {
    try {
      size_t size = src_img.Maximum( ) - src_img.Minimum( );
      const float k1 = 0.0001f;
      const float k2 = 0.0009f;
      const float c1 = k1 * size * size;
      const float c2 = k2 * size * size;
      float src_avg = Statistics::Average( src_img );
      float tgt_avg = Statistics::Average( tgt_img );
      float src_std = Statistics::StandardDeviation( src_img );
      float tgt_std = Statistics::StandardDeviation( tgt_img );
      float src_tgt_covar = Statistics::Covariance( src_img, tgt_img );
      return( ( ( 2.0f * src_avg * tgt_avg + c1 ) * ( 2.0f * src_tgt_covar + c2 ) ) /
	      ( ( src_avg * src_avg + tgt_avg * tgt_avg + c1 ) * ( src_std * src_std + tgt_std * tgt_std + c2 ) ) );
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

#ifdef BIAL_EXPLICIT_StatisticsSSIM

  template float Statistics::SSIM( const Image< int > &src_img, const Image< int > &tgt_img );
  template float Statistics::SSIM( const Image< llint > &src_img, const Image< llint > &tgt_img );
  template float Statistics::SSIM( const Image< float > &src_img, const Image< float > &tgt_img );
  template float Statistics::SSIM( const Image< double > &src_img, const Image< double > &tgt_img );

  template float Statistics::SSIM( const Vector< int > &src_img, const Vector< int > &tgt_img );
  template float Statistics::SSIM( const Vector< llint > &src_img, const Vector< llint > &tgt_img );
  template float Statistics::SSIM( const Vector< float > &src_img, const Vector< float > &tgt_img );
  template float Statistics::SSIM( const Vector< double > &src_img, const Vector< double > &tgt_img );

#endif

}

#endif

#endif
