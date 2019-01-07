/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Dec/04
 * @brief Dice metric.
 */

#ifndef BIALSTATISTICSMEANSQUAREERROR_C
#define BIALSTATISTICSMEANSQUAREERROR_C

#include "StatisticsMeanSquareError.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_StatisticsMeanSquareError )
#define BIAL_EXPLICIT_StatisticsMeanSquareError
#endif
#if defined ( BIAL_EXPLICIT_StatisticsMeanSquareError ) || ( BIAL_IMPLICIT_BIN )

#include "Image.hpp"

namespace Bial {

  template< template< class D > class C, class D >
  float Statistics::MeanSquareError( const C< D > &src_img, const C< D > &tgt_img ) {
    try {
      if( src_img.size( ) != tgt_img.size( ) ) {
        std::string msg( BIAL_ERROR( "Input data dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      C< D > diff( src_img - tgt_img );
      diff = diff * diff;
      float mse = 0;
      for( size_t idx = 0; idx < src_img.size( ); ++idx )
        mse += diff[ idx ];
      mse /= src_img.size( );
      return( mse );
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

#ifdef BIAL_EXPLICIT_StatisticsMeanSquareError

  template float Statistics::MeanSquareError( const Image< int > &src_img, const Image< int > &tgt_img );
  template float Statistics::MeanSquareError( const Image< llint > &src_img, const Image< llint > &tgt_img );
  template float Statistics::MeanSquareError( const Image< float > &src_img, const Image< float > &tgt_img );
  template float Statistics::MeanSquareError( const Image< double > &src_img, const Image< double > &tgt_img );

  template float Statistics::MeanSquareError( const Vector< int > &src_img, const Vector< int > &tgt_img );
  template float Statistics::MeanSquareError( const Vector< llint > &src_img, const Vector< llint > &tgt_img );
  template float Statistics::MeanSquareError( const Vector< float > &src_img, const Vector< float > &tgt_img );
  template float Statistics::MeanSquareError( const Vector< double > &src_img, const Vector< double > &tgt_img );

#endif

}

#endif

#endif
