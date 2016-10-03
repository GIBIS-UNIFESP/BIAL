/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2014/Oct/23 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test with plate detection. */

#include "DrawCircle.hpp"
#include "FileImage.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc < 2 ) {
    cout << "Usage: " << argv[ 0 ] << " <txt file> <ground truth>" << endl;
    exit( -1 );
  }
  COMMENT( "Opening resulting image.", 0 );
  ifstream file( argv[ 1 ] );
  if( !file.is_open( ) ) {
    cout << "Could not open file." << endl;
    exit( -1 );
  }
  COMMENT( "Reading the number of circles.", 0 );
  string line;
  int size;
  file.ignore( 15 );
  file >> size;

  COMMENT( "Reading groud truth image.", 0 );
  Image< int > ground_truth( Read< int >( argv[ 2 ] ) );
  /* ground_truth /= 255; */

  COMMENT( "Initializing metric variables.", 0 );
  size_t TP = 0;
  size_t FP = 0;
  size_t FN = 0;

  COMMENT( "Computing metrics.", 0 );
  for( int crc = 0; crc < size; ++crc ) {
    COMMENT( "Reading data from file.", 0 );
    float radius, cx, cy;
    getline( file, line );
    getline( file, line );
    getline( file, line );
    file.ignore( 7 );
    file >> radius;
    getline( file, line );
    file.ignore( 7 );
    file >> cx >> cy;

    COMMENT( "Computing x square dimensions.", 0 );
    size_t xmin = cx - radius;
    size_t xmax = cx + radius;
    float xdisp_init = -radius;

    COMMENT( "Computing y square dimensions.", 0 );
    size_t ymin = cy - radius;
    size_t ymax = cy + radius;
    float ydisp_init = -radius;

    float radius2 = radius * radius;

    COMMENT( "Drawing filled circle.", 0 );
    for( size_t x = xmin, xdisp = xdisp_init; x <= xmax; ++x, ++xdisp ) {
      for( size_t y = ymin, ydisp = ydisp_init; y <= ymax; ++y, ++ydisp ) {
        if( !ground_truth.ValidCoordinate( x, y ) ) {
          ++FP;
        }
        else if( xdisp * xdisp + ydisp * ydisp <= radius2 ) {
          if( ground_truth( x, y ) != 0 ) {
            ++TP;
          }
          else {
            ++FN;
          }
        }
      }
    }
  }
  COMMENT( "Printing result.", 0 );
  cout << "TP = " << TP << endl;
  cout << "FP = " << FP << endl;
  cout << "FN = " << FN << endl;
  return( 0 );
}
