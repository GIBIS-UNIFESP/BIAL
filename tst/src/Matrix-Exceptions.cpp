/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2013/Aug/23 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test with Matrix class and exceptions. */

#include "Matrix.hpp"

using namespace std;
using namespace Bial;

int main( int, char** ) {
  Matrix< double > M1( 10, 8 );
  Matrix< double > M2( 2, 3 );
  Matrix< double > M3( 1, 2 );
  Matrix< double > M4( 3, 3, 4 );

  try {
    M2 += M1;
  }
  catch( logic_error &e ) {
    cerr << e.what( ) << endl;
  }

  try {
    M3 = M1 - M2;
  }
  catch( logic_error &e ) {
    cerr << e.what( ) << endl;
  }

  try {
    M3 = M1 * M2;
  }
  catch( logic_error &e ) {
    cerr << e.what( ) << endl;
  }

  try {
    M1 = M4 * M2;
  }
  catch( logic_error &e ) {
    cerr << e.what( ) << endl;
  }

  try {
    cout << M1.at( 3, 4, 5 );
  }
  catch( logic_error &e ) {
    cerr << e.what( ) << endl;
  }

  try {
    cout << M1.at( 0, 100 );
  }
  catch( logic_error &e ) {
    cerr << e.what( ) << endl;
  }

  return( 0 );
}