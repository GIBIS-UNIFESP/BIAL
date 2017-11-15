/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2014/Jan/07
 * @brief Compute the matrix transpose.
 */

#ifndef BIALMATRIXTRANSPOSE_C
#define BIALMATRIXTRANSPOSE_C

#include "MatrixTranspose.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_MatrixTranspose )
#define BIAL_EXPLICIT_MatrixTranspose
#endif

#if defined ( BIAL_EXPLICIT_MatrixTranspose ) || ( BIAL_IMPLICIT_BIN )

#include "Matrix.hpp"

namespace Bial {

  template< class D >
  void MatrixOp::Transpose( Matrix< D > &mat ) {
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
    for( size_t i = 0; i < dim_1; i++ ) {
      for( size_t j = 0; j < i; j++ )
        std::swap( mat[ i * dim_0 + j ], mat[ j * dim_0 + i ] );
    }
  }

  template< class D >
  Matrix< D > MatrixOp::Transpose( const Matrix< D > &mat ) {
    try {
      Matrix< D > res( mat );
      MatrixOp::Transpose( res );
      return( res );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

#ifdef BIAL_EXPLICIT_MatrixTranspose

  template void MatrixOp::Transpose( Matrix< int > &mat );
  template Matrix< int > MatrixOp::Transpose( const Matrix< int > &mat );
  template void MatrixOp::Transpose( Matrix< llint > &mat );
  template Matrix< llint > MatrixOp::Transpose( const Matrix< llint > &mat );
  template void MatrixOp::Transpose( Matrix< float > &mat );
  template Matrix< float > MatrixOp::Transpose( const Matrix< float > &mat );
  template void MatrixOp::Transpose( Matrix< double > &mat );
  template Matrix< double > MatrixOp::Transpose( const Matrix< double > &mat );

#endif

}

#endif

#endif
