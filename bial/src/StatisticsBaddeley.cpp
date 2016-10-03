/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Dec/04
 * @brief Baddeley metric.
 */

#ifndef BIALSTATISTICSBADDELEY_C
#define BIALSTATISTICSBADDELEY_C

#include "StatisticsBaddeley.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_StatisticsBaddeley )
#define BIAL_EXPLICIT_StatisticsBaddeley
#endif
#if defined ( BIAL_EXPLICIT_StatisticsBaddeley ) || ( BIAL_IMPLICIT_BIN )

#include "Image.hpp"

namespace Bial {

  template< class D >
  float Statistics::Baddeley( const Image< D > &src_distance_img, const Image< D > &tgt_distance_img ) {
    try {
      if( src_distance_img.size( ) != tgt_distance_img.size( ) ) {
        std::string msg( BIAL_ERROR( "Input data dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      } 
      double res = 0.0;
      size_t size = src_distance_img.size( );
      for( size_t pxl = 0; pxl < size; ++pxl ) {
        res += std::pow( src_distance_img[ pxl ] - tgt_distance_img[ pxl ], 2.0 );
      }
      res /= src_distance_img.size( 0 ) * src_distance_img.size( 1 ) * src_distance_img.size( 0 ) * src_distance_img.size( 1 );
      return( std::sqrt( res ) );
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

#ifdef BIAL_EXPLICIT_StatisticsBaddeley

  template float Statistics::Baddeley( const Image< int > &src_distance_img, const Image< int > &tgt_distance_img );
  template float Statistics::Baddeley( const Image< llint > &src_distance_img, const Image< llint > &tgt_distance_img );
  template float Statistics::Baddeley( const Image< float > &src_distance_img, const Image< float > &tgt_distance_img );
  template float Statistics::Baddeley( const Image< double > &src_distance_img, 
                                       const Image< double > &tgt_distance_img );

#endif

}

#endif

#endif
