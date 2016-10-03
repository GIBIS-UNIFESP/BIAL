/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Oct/23 */
/* Content: Test file. */
/* Description: Test with Matrix class. */

#include "Common.hpp"
#include "Matrix.hpp"
#include "MatrixDeterminant.hpp"

using namespace std;
using namespace Bial;

int main( int, char** ) {

  Common::Randomize( false );

  Matrix< double > M1( 2, 2 );
  M1( 0, 0 ) = 1.0;
  M1( 0, 1 ) = 2.0;
  M1( 1, 0 ) = -1.0;
  M1( 1, 1 ) = -3.0;
  cout << "2x2 Matrix: " << endl;
  M1.Print( cout );
  cout << "Determinant: " << MatrixOp::Determinant( M1 ) << endl << endl;

  M1 = Matrix< double >( 3, 3 );
  for( size_t i = 0; i < M1.size( 0 ); ++i ) {
    for( size_t j = 0; j < M1.size( 1 ); ++j ) {
      M1( i, j ) = static_cast< double >( rand( ) ) / static_cast< float >( RAND_MAX ) * 10.0;
    }
  }
  cout << "3x3 Matrix: " << endl;
  M1.Print( cout );
  cout << "Determinant: " << MatrixOp::Determinant( M1 ) << endl << endl;


  M1 = Matrix< double >( 4, 4 );
  for( size_t i = 0; i < M1.size( 0 ); ++i ) {
    for( size_t j = 0; j < M1.size( 1 ); ++j ) {
      M1( i, j ) = static_cast< double >( rand( ) ) / static_cast< float >( RAND_MAX ) * 10.0;
    }
  }
  cout << "4x4 Matrix: " << endl;
  M1.Print( cout );
  cout << endl;
  cout << "Determinant: " << MatrixOp::Determinant( M1 ) << endl << endl;
  for( size_t i = 0; i < M1.size( 0 ); ++i ) {
    for( size_t j = 0; j < M1.size( 1 ); ++j ) {
      if( ( i == 0 ) && ( j == 0 ) ) {
        M1( i, j ) = -3;
      }
      else if( ( i == 1 ) && ( j == 1 ) ) {
        M1( i, j ) = 4;
      }
      else if( ( i == 2 ) && ( j == 2 ) ) {
        M1( i, j ) = -1;
      }
      else if( ( i == 3 ) && ( j == 3 ) ) {
        M1( i, j ) = 5;
      }
      else {
        M1( i, j ) = 2;
      }
    }
  }
  cout << "4x4 Matrix: " << endl;
  M1.Print( cout );
  cout << endl;
  cout << "Determinant: " << MatrixOp::Determinant( M1 ) << endl << endl;


  M1 = Matrix< double >( 5, 5 );
  for( size_t i = 0; i < M1.size( 0 ); i++ ) {
    for( size_t j = 0; j < M1.size( 1 ); j++ ) {
      M1( i, j ) = static_cast< double >( rand( ) ) / static_cast< float >( RAND_MAX ) * 10.0;
    }
  }
  cout << "5x5 Matrix: " << endl;
  M1.Print( cout );
  cout << "Determinant: " << MatrixOp::Determinant( M1 ) << endl << endl;

  return( 0 );

}
