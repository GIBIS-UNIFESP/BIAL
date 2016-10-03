/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2014/Jan/07
 * @brief Functions to compute matrix inverse.
 */

#ifndef BIALMATRIXINVERSE_C
#define BIALMATRIXINVERSE_C

#include "MatrixInverse.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_MatrixInverse )
#define BIAL_EXPLICIT_MatrixInverse
#endif

#if defined ( BIAL_EXPLICIT_MatrixInverse ) || ( BIAL_IMPLICIT_BIN )

#include "Matrix.hpp"
#include "MatrixTranspose.hpp"
#include "MatrixDeterminant.hpp"

namespace Bial {

  template< class D >
  Matrix< D > MatrixOp::Cofactor( const Matrix< D > &mat ) {
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
      Matrix< D > cofactor( dim_0, dim_0 );
      Matrix< D > minor_mat( dim_0 - 1, dim_0 - 1 );
      size_t minor_size_0 = minor_mat.size( 0 );
      COMMENT( "Setting first minor matrix.", 3 );
      for( size_t i = 0; i < minor_size_0; ++i ) {
        for( size_t j = 0; j < minor_size_0; ++j ) {
          minor_mat( i, j ) = mat( i + 1, j + 1 );
        }
      }
      double det;
      for( size_t i = 0; i < dim_0; ++i ) {
        for( size_t j = 0; j < dim_0; ++j ) {
          det = MatrixOp::Determinant( minor_mat );
          cofactor( j, i ) = pow( -1.0, j + i + 2.0 ) * det;
          if( j < minor_size_0 ) {
            for( size_t k = 0; k < minor_size_0; k++ ) {
              if( k < i ) {
                minor_mat( j, k ) = mat( j, k );
              }
              else {
                minor_mat( j, k ) = mat( j, k + 1 );
              }
            }
          }
        }
        for( size_t k = 0; k < minor_size_0; ++k ) {
          for( size_t l = 0; l < minor_size_0; ++l ) {
            if( k <= i ) {
              minor_mat( l, k ) = mat( l + 1, k );
            }
            else {
              minor_mat( l, k ) = mat( l + 1, k + 1 );
            }
          }
        }
      }
      return( cofactor );
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
  void MatrixOp::Inverse( Matrix< D > &mat ) {
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
      COMMENT( "Avoiding matrices with 0 in any eigenvector element.", 2 );
      for( size_t i = 0; i < dim_0; ++i ) {
        if( mat( i, i ) == 0.0 ) {
          MatrixOp::Inverse3( mat );
          return;
        }
      }
      COMMENT( "Normalizing row 0.", 2 );
      for( size_t i = 1; i < dim_0; ++i ) {
        mat( i ) /= mat( 0 );
      }
      for( size_t i = 1; i < dim_0; ++i ) {
        COMMENT( "Do a column of L", 3 );
        for( size_t j = i; j < dim_0; ++j ) {
          D sum = 0.0;
          for( size_t k = 0; k < i; ++k ) {
            sum += mat( j * dim_0 + k ) * mat( k * dim_0 + i );
          }
          mat( j * dim_0 + i ) -= sum;
        }
        if( i == dim_0 - 1 ) {
          continue;
        }
        COMMENT( "Do a row of U.", 3 );
        for( size_t j = i + 1; j < dim_0; ++j ) {
          D sum = 0.0;
          for( size_t k = 0; k < i; ++k ) {
            sum += mat( i * dim_0 + k ) * mat( k * dim_0 + j );
          }
          mat( i * dim_0 + j ) = ( mat( i * dim_0 + j ) - sum ) /
            mat( i * dim_0 + i );
        }
      }
      COMMENT( "Invert L.", 2 );
      for( size_t i = 0; i < dim_0; ++i ) {
        for( size_t j = i; j < dim_0; ++j ) {
          D x = 1.0;
          if( i != j ) {
            x = 0.0;
            for( size_t k = i; k < j; k++ ) {
              x -= mat( j * dim_0 + k ) * mat( k * dim_0 + i );
            }
          }
          mat( j * dim_0 + i ) = x / mat( j * dim_0 + j );
        }
      }
      COMMENT( "Invert U.", 2 );
      for( size_t i = 0; i < dim_0; ++i ) {
        for( size_t j = i; j < dim_0; ++j ) {
          if( i == j ) {
            continue;
          }
          D sum = 0.0;
          for( size_t k = i; k < j; ++k ) {
            if( i == k ) {
              sum += mat( k * dim_0 + j );
            }
            else {
              sum += mat( k * dim_0 + j ) * mat( i * dim_0 + k );
            }
          }
          mat( i * dim_0 + j ) = -sum;
        }
      }
      COMMENT( "Invert the other elements.", 2 );
      for( size_t i = 0; i < dim_0; ++i ) {
        for( size_t j = 0; j < dim_0; ++j ) {
          D sum = 0.0;
          if( i > j ) {
            for( size_t k = i; k < dim_0; ++k ) {
              if( j == k ) {
                sum += mat( k * dim_0 + i );
              }
              else {
                sum += mat( j * dim_0 + k ) * mat( k * dim_0 + i );
              }
            }
          }
          else {
            for( size_t k = j; k < dim_0; ++k ) {
              if( j == k ) {
                sum += mat( k * dim_0 + i );
              }
              else {
                sum += mat( j * dim_0 + k ) * mat( k * dim_0 + i );
              }
            }
          }
          mat( j * dim_0 + i ) = sum;
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

  template< class D >
  Matrix< D > MatrixOp::Inverse( const Matrix< D > &mat ) {
    try {
      Matrix< D > res( mat );
      MatrixOp::Inverse( res );
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

  template< class D >
  void MatrixOp::Inverse2( Matrix< D > &mat ) {
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
      COMMENT( "Avoiding matrices with 0 in any eigenvector element.", 0 );
      for( size_t i = 0; i < dim_0; ++i ) {
        if( mat( i, i ) == 0.0 ) {
          MatrixOp::Inverse3( mat );
          return;
        }
      }
      Matrix< D > same( mat );
      for( size_t i = 0; i < dim_1; ++i ) {
        for( size_t j = 0; j < dim_0; ++j ) {
          if( i == j )
            mat( i * dim_0 + j ) = 1.0;
          else
            mat( i * dim_0 + j ) = 0.0;
        }
      }
      for( size_t k = 0; k < dim_0; ++k ) {
        D m = same[ k * dim_0 + k ];
        same[ k * dim_0 + k ] = 1.0;
        for( size_t j = 0; j < dim_0; ++j ) {
          if( j != k ) {
            same[ k * dim_0 + j ] = same[ k * dim_0 + j ] / m;
          }
          mat( k * dim_0 + j ) = mat( k * dim_0 + j ) / m;
        }
        for( size_t i = 0; i < dim_0; ++i ) {
          if( i != k ) {
            m = same[ i * dim_0 + k ] / same[ k * dim_0 + k ];
            same[ i * dim_0 + k ] = 0.0;
            for( size_t j = 0; j < dim_0; ++j ) {
              if( j != k ) {
                same[ i * dim_0 + j ] = same[ i * dim_0 + j ] - m * same[ k * dim_0 + j ];
              }
              mat( i * dim_0 + j ) = mat( i * dim_0 + j ) - m * mat( k * dim_0 + j );
            }
          }
        }
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
  Matrix< D > MatrixOp::Inverse2( const Matrix< D > &mat ) {
    try {
      Matrix< D > res( mat );
      MatrixOp::Inverse2( res );
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

  template< class D >
  void MatrixOp::Inverse3( Matrix< D > &mat ) {
    try {
      Matrix< D > cofactor( MatrixOp::Cofactor( mat ) );
      Matrix< D > adjoint( MatrixOp::Transpose( const_cast< const Matrix< D > & >( cofactor ) ) );
      double det = MatrixOp::Determinant( mat );
      size_t size = mat.size( );
      for( size_t i = 0; i < size; ++i ) {
        mat[ i ] = adjoint[ i ] / det;
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
  Matrix< D > MatrixOp::Inverse3( const Matrix< D > &mat ) {
    try {
      Matrix< D > cofactor = MatrixOp::Cofactor( mat );
      return( MatrixOp::Transpose( const_cast< const Matrix< D > & >( cofactor ) ) * ( 1 / MatrixOp::Determinant( mat ) ) );
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

#ifdef BIAL_EXPLICIT_MatrixInverse

  template Matrix< int > MatrixOp::Cofactor( const Matrix< int > &mat );
  template void MatrixOp::Inverse( Matrix< int > &mat );
  template Matrix< int > MatrixOp::Inverse( const Matrix< int > &mat );
  template void MatrixOp::Inverse2( Matrix< int > &mat );
  template Matrix< int > MatrixOp::Inverse2( const Matrix< int > &mat );
  template void MatrixOp::Inverse3( Matrix< int > &mat );
  template Matrix< int > MatrixOp::Inverse3( const Matrix< int > &mat );

  template Matrix< llint > MatrixOp::Cofactor( const Matrix< llint > &mat );
  template void MatrixOp::Inverse( Matrix< llint > &mat );
  template Matrix< llint > MatrixOp::Inverse( const Matrix< llint > &mat );
  template void MatrixOp::Inverse2( Matrix< llint > &mat );
  template Matrix< llint > MatrixOp::Inverse2( const Matrix< llint > &mat );
  template void MatrixOp::Inverse3( Matrix< llint > &mat );
  template Matrix< llint > MatrixOp::Inverse3( const Matrix< llint > &mat );

  template Matrix< float > MatrixOp::Cofactor( const Matrix< float > &mat );
  template void MatrixOp::Inverse( Matrix< float > &mat );
  template Matrix< float > MatrixOp::Inverse( const Matrix< float > &mat );
  template void MatrixOp::Inverse2( Matrix< float > &mat );
  template Matrix< float > MatrixOp::Inverse2( const Matrix< float > &mat );
  template void MatrixOp::Inverse3( Matrix< float > &mat );
  template Matrix< float > MatrixOp::Inverse3( const Matrix< float > &mat );

  template Matrix< double > MatrixOp::Cofactor( const Matrix< double > &mat );
  template void MatrixOp::Inverse( Matrix< double > &mat );
  template Matrix< double > MatrixOp::Inverse( const Matrix< double > &mat );
  template void MatrixOp::Inverse2( Matrix< double > &mat );
  template Matrix< double > MatrixOp::Inverse2( const Matrix< double > &mat );
  template void MatrixOp::Inverse3( Matrix< double > &mat );
  template Matrix< double > MatrixOp::Inverse3( const Matrix< double > &mat );
  
#endif

}

#endif

#endif
