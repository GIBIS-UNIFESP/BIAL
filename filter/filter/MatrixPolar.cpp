/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2014/Jan/07
 * @brief Functions to compute polar matrix.
 */

#ifndef BIALMATRIXPOLAR_C
#define BIALMATRIXPOLAR_C

#include "MatrixPolar.hpp"
#include "Matrix.hpp"
#include "MatrixDeterminant.hpp"
#include "MatrixInverse.hpp"

namespace Bial {

  template< class D >
  double MatrixOp::MinimumRowNorm( const Matrix< D > &mat ) {
    try {
      size_t dims = mat.Dims( );
      if( dims != 2 ) {
        std::string msg( BIAL_ERROR( "Invalid number of dimensions. Expected 2." ) );
        throw( std::logic_error( msg ) );
      }
      double smallest = mat( 0 );
      size_t dim_0 = mat.size( 0 );
      size_t dim_1 = mat.size( 1 );
      for( size_t j = 1; j < dim_1; ++j ) {
        smallest += mat( j * dim_0 );
      }
      for( size_t i = 1; i < dim_0; ++i ) {
        double rn = mat( i );
        for( size_t j = 1; j < dim_1; ++j ) {
          rn += mat( i + j * dim_0 );
        }
        if( smallest > rn ) {
          smallest = rn;
        }
      }
      return( smallest );
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
  double MatrixOp::MinimumColNorm( const Matrix< D > &mat ) {
    try {
      size_t dims = mat.Dims( );
      if( dims != 2 ) {
        std::string msg( BIAL_ERROR( "Invalid number of dimensions. Expected 2." ) );
        throw( std::logic_error( msg ) );
      }
      double smallest = mat( 0 );
      size_t dim_0 = mat.size( 0 );
      size_t dim_1 = mat.size( 1 );
      for( size_t i = 1; i < dim_0; ++i ) {
        smallest += mat( i );
      }
      for( size_t j = 1; j < dim_1; ++j ) {
        double rn = mat( j * dim_0 );
        for( size_t i = 1; i < dim_0; ++i ) {
          rn += mat( i + j * dim_0 );
        }
        if( smallest > rn ) {
          smallest = rn;
        }
      }
      return( smallest );
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
  void MatrixOp::Polar( Matrix< D > &mat ) {
    try {
      size_t dims = mat.Dims( );
      size_t dim_0 = mat.size( 0 );
      size_t dim_1 = mat.size( 1 );
      if( ( dims != 2 ) || ( dim_0 != 3 ) || ( dim_1 != 3 ) ) {
        std::string msg( BIAL_ERROR( "Expected 3x3 Matrix." ) );
        throw( std::logic_error( msg ) );
      }
      Matrix< D > mat_copy = mat;
      COMMENT( "Force matrix to be nonsingular.", 0 );
      float det = MatrixOp::Determinant( mat_copy );
      COMMENT( "Perturb matrix.", 0 );
      while( det == 0.0 ) {
        det = 0.00001 * ( 0.001 + MatrixOp::MinimumRowNorm( mat_copy ) );
        mat_copy( 0, 0 ) += det;
        mat_copy( 1, 1 ) += det;
        mat_copy( 2, 2 ) += det;
        det = MatrixOp::Determinant( mat_copy );
      }
      int k = 0;
      while( 1 ) {
        Matrix< D > mat_copy_inv( mat_copy );
        MatrixOp::Inverse( mat_copy_inv );
        float det_inv;
        float dif = 1.0;
        if( dif > 0.3 ) {
          COMMENT( "Far from convergence.", 3 );
          float alp = sqrt( MatrixOp::MinimumRowNorm( mat_copy ) * MatrixOp::MinimumColNorm( mat_copy ) );
          float bet = sqrt( MatrixOp::MinimumRowNorm( mat_copy_inv ) * MatrixOp::MinimumColNorm( mat_copy_inv ) );
          det = sqrt( bet / alp );
          det_inv = 1.0 / det;
        }
        else {
          COMMENT( "Close to convergence.", 3 );
          det = det_inv = 1.0;
        }
        mat( 0, 0 ) = 0.5 * ( det * mat_copy( 0, 0 ) + det_inv * mat_copy_inv( 0, 0 ) );
        mat( 0, 1 ) = 0.5 * ( det * mat_copy( 0, 1 ) + det_inv * mat_copy_inv( 1, 0 ) );
        mat( 0, 2 ) = 0.5 * ( det * mat_copy( 0, 2 ) + det_inv * mat_copy_inv( 2, 0 ) );
        mat( 1, 0 ) = 0.5 * ( det * mat_copy( 1, 0 ) + det_inv * mat_copy_inv( 0, 1 ) );
        mat( 1, 1 ) = 0.5 * ( det * mat_copy( 1, 1 ) + det_inv * mat_copy_inv( 1, 1 ) );
        mat( 1, 2 ) = 0.5 * ( det * mat_copy( 1, 2 ) + det_inv * mat_copy_inv( 2, 1 ) );
        mat( 2, 0 ) = 0.5 * ( det * mat_copy( 2, 0 ) + det_inv * mat_copy_inv( 0, 2 ) );
        mat( 2, 1 ) = 0.5 * ( det * mat_copy( 2, 1 ) + det_inv * mat_copy_inv( 1, 2 ) );
        mat( 2, 2 ) = 0.5 * ( det * mat_copy( 2, 2 ) + det_inv * mat_copy_inv( 2, 2 ) );
        dif = std::abs( mat( 0, 0 ) - mat_copy( 0, 0 ) ) + std::abs( mat( 0, 1 ) - mat_copy( 0, 1 ) )
          + std::abs( mat( 0, 2 ) - mat_copy( 0, 2 ) ) + std::abs( mat( 1, 0 ) - mat_copy( 1, 0 ) )
          + std::abs( mat( 1, 1 ) - mat_copy( 1, 1 ) ) + std::abs( mat( 1, 2 ) - mat_copy( 1, 2 ) )
          + std::abs( mat( 2, 0 ) - mat_copy( 2, 0 ) ) + std::abs( mat( 2, 1 ) - mat_copy( 2, 1 ) )
          + std::abs( mat( 2, 2 ) - mat_copy( 2, 2 ) );
        k = k + 1;
        if( ( k > 100 ) || ( dif < 3.e-6 ) ) {
          COMMENT( "Convergence or exhaustion: k: " << k << ", dif: " << dif, 0 );
          break;
        }
        mat_copy = mat;
      }
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
  Matrix< D > MatrixOp::Polar( const Matrix< D > &mat ) {
    try {
      Matrix< D > res( mat );
      MatrixOp::Polar( res );
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
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template double MatrixOp::MinimumRowNorm( const Matrix< int > &mat );
  template double MatrixOp::MinimumColNorm( const Matrix< int > &mat );
  template void MatrixOp::Polar( Matrix< int > &mat );
  template Matrix< int > MatrixOp::Polar( const Matrix< int > &mat );

  template double MatrixOp::MinimumRowNorm( const Matrix< llint > &mat );
  template double MatrixOp::MinimumColNorm( const Matrix< llint > &mat );
  template void MatrixOp::Polar( Matrix< llint > &mat );
  template Matrix< llint > MatrixOp::Polar( const Matrix< llint > &mat );

  template double MatrixOp::MinimumRowNorm( const Matrix< float > &mat );
  template double MatrixOp::MinimumColNorm( const Matrix< float > &mat );
  template void MatrixOp::Polar( Matrix< float > &mat );
  template Matrix< float > MatrixOp::Polar( const Matrix< float > &mat );

  template double MatrixOp::MinimumRowNorm( const Matrix< double > &mat );
  template double MatrixOp::MinimumColNorm( const Matrix< double > &mat );
  template void MatrixOp::Polar( Matrix< double > &mat );
  template Matrix< double > MatrixOp::Polar( const Matrix< double > &mat );

}

#endif
