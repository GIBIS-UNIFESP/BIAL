/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2014/Jan/07
 * @brief Compute matrix determinant.
 */

#ifndef BIALMATRIXDETERMINANT_C
#define BIALMATRIXDETERMINANT_C

#include "MatrixDeterminant.hpp"
#include "Matrix.hpp"

namespace Bial {

  template< class D >
  double MatrixOp::Determinant( const Matrix< D > &mat ) {
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
      if( dim_0 == 1 ) {
        return( mat( 0 ) );
      }
      if( dim_0 == 2 ) {
        return( mat( 0 ) * mat( 3 ) - mat( 1 ) * mat( 2 ) );
      }
      if( dim_0 == 3 ) {
        return( mat( 0, 0 ) * mat( 1, 1 ) * mat( 2, 2 ) -
                mat( 0, 0 ) * mat( 2, 1 ) * mat( 1, 2 ) -
                mat( 1, 0 ) * mat( 0, 1 ) * mat( 2, 2 ) +
                mat( 1, 0 ) * mat( 2, 1 ) * mat( 0, 2 ) +
                mat( 2, 0 ) * mat( 0, 1 ) * mat( 1, 2 ) -
                mat( 2, 0 ) * mat( 1, 1 ) * mat( 0, 2 ) );
      }
      COMMENT( "For more than three dimensions.", 0 );
      COMMENT( "Setting minor matrix.", 0 );
      size_t minor_dim_0 = dim_0 - 1;
      Matrix< D > minor_mat( minor_dim_0, minor_dim_0 );
      for( size_t i = 0; i < minor_dim_0; ++i ) {
        for( size_t j = 0; j < minor_dim_0; ++j ) {
          minor_mat( i, j ) = mat( i + 1, j + 1 );
        }
      }
      COMMENT( "Computing determinant.", 0 );
      double det = 0.0;
      for( size_t i = 0; i < dim_0; ++i ) {
        for( size_t j = 0; j < dim_0; ++j ) {
          det += pow( -1.0, i + j + 2.0 ) * mat( j, i ) * MatrixOp::Determinant( minor_mat );
          if( j < minor_dim_0 ) {
            for( size_t k = 0; k < minor_dim_0; k++ ) {
              if( k < i )
                minor_mat( j, k ) = mat( j, k );
              else
                minor_mat( j, k ) = mat( j, k + 1 );
            }
          }
        }
        for( size_t k = 0; k < minor_dim_0; ++k ) {
          for( size_t l = 0; l < minor_dim_0; ++l ) {
            if( k <= i ) 
              minor_mat( l, k ) = mat( l + 1, k );
            else
              minor_mat( l, k ) = mat( l + 1, k + 1 );
          }
        }
      }
      return( det / dim_0 );
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

  template double MatrixOp::Determinant( const Matrix< int > &mat );
  template double MatrixOp::Determinant( const Matrix< llint > &mat );
  template double MatrixOp::Determinant( const Matrix< float > &mat );
  template double MatrixOp::Determinant( const Matrix< double > &mat );

}

#endif
