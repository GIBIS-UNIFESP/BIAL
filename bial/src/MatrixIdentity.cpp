/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2014/Jan/07
 * @brief Set matrix to identity or computer distance from identity.
 */

#ifndef BIALMATRIXIDENTITY_C
#define BIALMATRIXIDENTITY_C

#include "MatrixIdentity.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_MatrixIdentity )
#define BIAL_EXPLICIT_MatrixIdentity
#endif

#if defined ( BIAL_EXPLICIT_MatrixIdentity ) || ( BIAL_IMPLICIT_BIN )

#include "Matrix.hpp"

namespace Bial {
  
  template< class D >
  D MatrixOp::CompareToIdentity( const Matrix< D > &mat ) {
    D max_diff = 0.0;
    size_t dims = mat.Dims( );
    if( dims != 2 ) {
      std::string msg( BIAL_ERROR( "Invalid number of dimensions. Expected 2." ) );
      throw( std::logic_error( msg ) );
    }
    size_t dim_0 = mat.size( 0 );
    size_t dim_1 = mat.size( 1 );
    if( dim_0 != dim_1 ) {
      std::string msg( BIAL_ERROR( "Matrix must be square." ) );
      throw( std::logic_error( msg ) );
    }
    for( size_t j = 0; j < dim_1; j++ ) {
      for( size_t i = 0; i < dim_0; i++ ) {
        D diff;
        if( i == j ) {
          diff = mat( j * dim_0 + i ) - 1.0;
        }
        else {
          diff = mat( j * dim_0 + i );
        }
        if( diff < 0.0 ) {
          diff = -diff;
        }
        if( max_diff < diff ) {
          max_diff = diff;
        }
      }
    }
    return( max_diff );
  }

  template< class D >
  void MatrixOp::Identity( Matrix< D > &mat ) {
    try {
      size_t dims = mat.Dims( );
      if( dims != 2 ) {
        std::string msg( BIAL_ERROR( "Invalid number of dimensions. Expected 2." ) );
        throw( std::logic_error( msg ) );
      }
      size_t dim_0 = mat.size( 0 );
      size_t dim_1 = mat.size( 1 );
      if( dim_0 != dim_1 ) {
        std::string msg( BIAL_ERROR( "Matrix must be square." ) );
        throw( std::logic_error( msg ) );
      }
      for( size_t j = 0; j < dim_1; ++j ) {
        for( size_t i = 0; i < dim_0; ++i ) {
          mat( i, j ) = static_cast< D >( i == j );
        }
      }
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

#ifdef BIAL_EXPLICIT_MatrixIdentity

  template int MatrixOp::CompareToIdentity( const Matrix< int > &mat );
  template void MatrixOp::Identity( Matrix< int > &mat );
  template llint MatrixOp::CompareToIdentity( const Matrix< llint > &mat );
  template void MatrixOp::Identity( Matrix< llint > &mat );
  template float MatrixOp::CompareToIdentity( const Matrix< float > &mat );
  template void MatrixOp::Identity( Matrix< float > &mat );
  template double MatrixOp::CompareToIdentity( const Matrix< double > &mat );
  template void MatrixOp::Identity( Matrix< double > &mat );

#endif

}

#endif

#endif
