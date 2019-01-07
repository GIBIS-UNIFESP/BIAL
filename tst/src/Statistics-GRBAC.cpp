/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2018/Dec/14 */
/* Content: Test file. */
/* Description: Counts the number of crossings between the border of two binary objects. */


#include "FileImage.hpp"
#include "Image.hpp"
#include "StatisticsGRBAC.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  try {
    if( argc < 3 ) {
      cout << "Usage: " << argv[ 0 ] << " <binary image 1> <binary image 2> [<alpha> [<delta>]]" << endl;
      cout << "\t\t<alpha>: weight of FP (higher alpha) and FN (lower alpha). [Range: -1.0 to 1.0. Default: 0.0 (neutral)]." << endl;
      cout << "\t\t<delta>: general weight of error. Lower delta implies in higher error impact to the accuracy. [Range: 1.0 to 3.0. Default: 1.0]." << endl;
      return( 0 );
    }
    Image< int > src( Read< int >( argv[ 1 ] ) );
    Image< int > tgt( Read< int >( argv[ 2 ] ) );
    float alpha = 0.0f;
    if( argc > 3 )
      alpha = atof( argv[ 3 ] );
    float delta = 1.0f;
    if( argc > 4 )
      delta = atof( argv[ 4 ] );
    if( src.Dim( ) != tgt.Dim( ) ) {
      cout << "Error. Source and target images have different sizes." << endl;
      return( 0 );
    }
    if( ( alpha < -1.0f ) || ( alpha > 1.0f ) ) {
      cout << "Error. Alpha should be between -1.0 and 1.0." << endl;
      return( 0 );
    }
    if( ( delta < 1.0f ) || ( delta > 3.0f ) ) {
      cout << "Error. Delta should be between 1.0 and 3.0." << endl;
      return( 0 );
    }
    std::cout << Statistics::GRBAC( src, tgt, alpha, delta ) << std::endl;
    return( 0 );
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
