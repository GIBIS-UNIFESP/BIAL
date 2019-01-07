/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2018/Dec/28
 * @brief General, Robust, Balanced, Adjustable, Coherent, and Consistent metric.
 */

#ifndef BIALSTATISTICSGRBAC_C
#define BIALSTATISTICSGRBAC_C

#include "StatisticsGRBAC.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_StatisticsGRBAC )
#define BIAL_EXPLICIT_StatisticsGRBAC
#endif
#if defined ( BIAL_EXPLICIT_StatisticsGRBAC ) || ( BIAL_IMPLICIT_BIN )

#include "Image.hpp"
#include "StatisticsPosNeg.hpp"

namespace Bial {

  template< template< class D > class C, class D >
  float Statistics::GRBAC( const C< D > &src, const C< D > &tgt, float alpha, float delta ) {
    try {
      float abs_alpha = std::abs( alpha );
      size_t TP, TN, FP, FN;
      std::tie( TP, TN, FP, FN ) = Statistics::PositiveNegative( src, tgt );
      float FPs = static_cast< float >( FP * FP ) / static_cast< float >( TP + FN - FP );
      float GBC = ( delta * TP ) /
	( delta * TP + ( ( 1.0f + alpha ) / ( 1.0f + abs_alpha ) ) * FN + ( ( 1.0f - alpha ) / ( 1.0f + abs_alpha ) ) * FPs );
      return( std::max( 0.0f, GBC ) );
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

  template< template< class D > class C, class D >
  float Statistics::GRBAC( const C< D > &src, const C< D > &tgt, float alpha, float delta, size_t slice ) {
    try {
      float abs_alpha = std::abs( alpha );
      size_t TP, TN, FP, FN;
      std::tie( TP, TN, FP, FN ) = Statistics::PositiveNegative( src, tgt, slice );
      float FPs = static_cast< float >( FP * FP ) / static_cast< float >( TP + FN - FP );
      float GBC = ( delta * TP ) /
	( delta * TP + ( ( 1.0f + alpha ) / ( 1.0f + abs_alpha ) ) * FN + ( ( 1.0f - alpha ) / ( 1.0f + abs_alpha ) ) * FPs );
      return( std::max( 0.0f, GBC ) );
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

#ifdef BIAL_EXPLICIT_StatisticsGRBAC

  template float Statistics::GRBAC( const Image< int > &src, const Image< int > &tgt, float alpha, float delta );
  template float Statistics::GRBAC( const Image< llint > &src, const Image< llint > &tgt, float alpha, float delta );
  template float Statistics::GRBAC( const Image< float > &src, const Image< float > &tgt, float alpha, float delta );
  template float Statistics::GRBAC( const Image< double > &src, const Image< double > &tgt, float alpha, float delta );

  template float Statistics::GRBAC( const Vector< int > &src, const Vector< int > &tgt, float alpha, float delta );
  template float Statistics::GRBAC( const Vector< llint > &src, const Vector< llint > &tgt, float alpha, float delta );
  template float Statistics::GRBAC( const Vector< float > &src, const Vector< float > &tgt, float alpha, float delta );
  template float Statistics::GRBAC( const Vector< double > &src, const Vector< double > &tgt, float alpha, float delta );

  template float Statistics::GRBAC( const Image< int > &src, const Image< int > &tgt, float alpha, float delta, size_t slice );
  template float Statistics::GRBAC( const Image< llint > &src, const Image< llint > &tgt, float alpha, float delta, size_t slice );
  template float Statistics::GRBAC( const Image< float > &src, const Image< float > &tgt, float alpha, float delta, size_t slice );
  template float Statistics::GRBAC( const Image< double > &src, const Image< double > &tgt, float alpha, float delta, size_t slice );

#endif

}

#endif

#endif
