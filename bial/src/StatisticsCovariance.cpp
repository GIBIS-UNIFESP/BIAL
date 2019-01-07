/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Dec/04
 * @brief Intensity standard deviation
 */

#ifndef BIALSTATISTICSCOVARIANCE_C
#define BIALSTATISTICSCOVARIANCE_C

#include "StatisticsCovariance.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_StatisticsCovariance )
#define BIAL_EXPLICIT_StatisticsCovariance
#endif
#if defined ( BIAL_EXPLICIT_StatisticsCovariance ) || ( BIAL_IMPLICIT_BIN )

#include "Image.hpp"
#include "StatisticsAverage.hpp"

namespace Bial {

  template< template< class D > class C, class D >
  float Statistics::Covariance( const C< D > &src_data, const C< D > &tgt_data ) {
    try {
      float src_avg = Statistics::Average( src_data );
      float tgt_avg = Statistics::Average( tgt_data );
      if( src_data.size( ) != tgt_data.size( ) ) {
        std::string msg( BIAL_ERROR( "Source and target data must have the same size. " ) );
        throw( std::logic_error( msg ) );
      }
      C< D > src_less_avg( src_data - src_avg );
      C< D > tgt_less_avg( tgt_data - tgt_avg );
      C< D > src_mul_tgt( ( src_data - src_avg ) * ( tgt_data - tgt_avg ) );
      float sum = 0.0;
      for( size_t elm = 0; elm < src_data.size( ); ++elm )
        sum += src_mul_tgt[ elm ];
      return( sum / src_data.size( ) );
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

#ifdef BIAL_EXPLICIT_StatisticsCovariance

  template float Statistics::Covariance( const Image< int > &src_data, const Image< int > &tgt_data );
  template float Statistics::Covariance( const Image< llint > &src_data, const Image< llint > &tgt_data );
  template float Statistics::Covariance( const Image< float > &src_data, const Image< float > &tgt_data );
  template float Statistics::Covariance( const Image< double > &src_data, const Image< double > &tgt_data );

  template float Statistics::Covariance( const Vector< int > &src_data, const Vector< int > &tgt_data );
  template float Statistics::Covariance( const Vector< llint > &src_data, const Vector< llint > &tgt_data );
  template float Statistics::Covariance( const Vector< float > &src_data, const Vector< float > &tgt_data );
  template float Statistics::Covariance( const Vector< double > &src_data, const Vector< double > &tgt_data );

#endif

}

#endif

#endif
