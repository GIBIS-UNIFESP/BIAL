/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Dec/04
 * @brief Observed agreement metric.
 */

#ifndef BIALSTATISTICSOBSAGREE_C
#define BIALSTATISTICSOBSAGREE_C

#include "StatisticsObsAgree.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_StatisticsObsAgree )
#define BIAL_EXPLICIT_StatisticsObsAgree
#endif
#if defined ( BIAL_EXPLICIT_StatisticsObsAgree ) || ( BIAL_IMPLICIT_BIN )

#include "Image.hpp"
#include "StatisticsPosNeg.hpp"

namespace Bial {

  template< template< class D > class C, class D >
  float Statistics::ObservedAgreement( const C< D > &src_label, const C< D > &tgt_label ) {
    try {
      if( src_label.size( ) != tgt_label.size( ) ) {
        std::string msg( BIAL_ERROR( "Input data dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      size_t TP, TN, FP, FN;
      std::tie( TP, TN, FP, FN ) = Statistics::PositiveNegative( src_label, tgt_label );
      return( ( 1.0 * TP + TN ) / ( 1.0 * TP + TN + FP + FN ) );
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
  float Statistics::ObservedAgreement( const Image< D > &src_label, const Image< D > &tgt_label, float dist ) {
    try {
      if( src_label.size( ) != tgt_label.size( ) ) {
        std::string msg( BIAL_ERROR( "Input data dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      size_t TP, TN, FP, FN;
      std::tie( TP, TN, FP, FN ) = Statistics::PositiveNegative( src_label, tgt_label, dist );
      return( ( 1.0 * TP + TN ) / ( 1.0 * TP + TN + FP + FN ) );
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

#ifdef BIAL_EXPLICIT_StatisticsObsAgree

  template float Statistics::ObservedAgreement( const Image< int > &src_label, const Image< int > &tgt_label );
  template float Statistics::ObservedAgreement( const Image< llint > &src_label, const Image< llint > &tgt_label );
  template float Statistics::ObservedAgreement( const Image< float > &src_label, const Image< float > &tgt_label );
  template float Statistics::ObservedAgreement( const Image< double > &src_label, const Image< double > &tgt_label );

  template float Statistics::ObservedAgreement( const Vector< int > &src_label, const Vector< int > &tgt_label );
  template float Statistics::ObservedAgreement( const Vector< llint > &src_label, const Vector< llint > &tgt_label );
  template float Statistics::ObservedAgreement( const Vector< float > &src_label, const Vector< float > &tgt_label );
  template float Statistics::ObservedAgreement( const Vector< double > &src_label, const Vector< double > &tgt_label );

  template float Statistics::ObservedAgreement( const Image< int > &src_label, const Image< int > &tgt_label, 
                                                float dist );
  template float Statistics::ObservedAgreement( const Image< llint > &src_label, const Image< llint > &tgt_label, 
                                                float dist );
  template float Statistics::ObservedAgreement( const Image< float > &src_label, const Image< float > &tgt_label, 
                                                float dist );
  template float Statistics::ObservedAgreement( const Image< double > &src_label, const Image< double > &tgt_label, 
                                                float dist );

#endif

}

#endif

#endif
