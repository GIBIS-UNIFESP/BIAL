/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Dec/04
 * @brief Kappa metric.
 */

#ifndef BIALSTATISTICSKAPPA_C
#define BIALSTATISTICSKAPPA_C

#include "StatisticsKappa.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_StatisticsKappa )
#define BIAL_EXPLICIT_StatisticsKappa
#endif
#if defined ( BIAL_EXPLICIT_StatisticsKappa ) || ( BIAL_IMPLICIT_BIN )

#include "Image.hpp"
#include "StatisticsPosNeg.hpp"

namespace Bial {

  template< template< class D > class C, class D >
  float Statistics::Kappa( const C< D > &src_label, const C< D > &tgt_label ) {
    try {
      if( src_label.size( ) != tgt_label.size( ) ) {
        std::string msg( BIAL_ERROR( "Input data dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      size_t TP, TN, FP, FN;
      std::tie( TP, TN, FP, FN ) = Statistics::PositiveNegative( src_label, tgt_label );
      float term1 = 2.0 * TP * TN - FP * FN;
      float term2 = ( 1.0 * TP + FP ) * ( 1.0 * FP + TN );
      float term3 = ( 1.0 * FN + TN ) * ( 1.0 * TP + FN );
      return( term1 / ( term2 + term3 ) );
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
  float Statistics::Kappa( const Image< D > &src_label, const Image< D > &tgt_label, float dist ) {
    try {
      if( src_label.size( ) != tgt_label.size( ) ) {
        std::string msg( BIAL_ERROR( "Input data dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      size_t TP, TN, FP, FN;
      std::tie( TP, TN, FP, FN ) = Statistics::PositiveNegative( src_label, tgt_label, dist );
      float term1 = 2.0 * TP * TN - FP * FN;
      float term2 = ( 1.0 * TP + FP ) * ( 1.0 * FP + TN );
      float term3 = ( 1.0 * FN + TN ) * ( 1.0 * TP + FN );
      return( term1 / ( term2 + term3 ) );
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

#ifdef BIAL_EXPLICIT_StatisticsKappa

  template float Statistics::Kappa( const Image< int > &src_label, const Image< int > &tgt_label );
  template float Statistics::Kappa( const Image< llint > &src_label, const Image< llint > &tgt_label );
  template float Statistics::Kappa( const Image< float > &src_label, const Image< float > &tgt_label );
  template float Statistics::Kappa( const Image< double > &src_label, const Image< double > &tgt_label );

  template float Statistics::Kappa( const Vector< int > &src_label, const Vector< int > &tgt_label );
  template float Statistics::Kappa( const Vector< llint > &src_label, const Vector< llint > &tgt_label );
  template float Statistics::Kappa( const Vector< float > &src_label, const Vector< float > &tgt_label );
  template float Statistics::Kappa( const Vector< double > &src_label, const Vector< double > &tgt_label );

  template float Statistics::Kappa( const Image< int > &src_label, const Image< int > &tgt_label, float dist );
  template float Statistics::Kappa( const Image< llint > &src_label, const Image< llint > &tgt_label, float dist );
  template float Statistics::Kappa( const Image< float > &src_label, const Image< float > &tgt_label, float dist );
  template float Statistics::Kappa( const Image< double > &src_label, const Image< double > &tgt_label, float dist );

#endif

}

#endif

#endif
