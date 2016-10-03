/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2012/Jun/27 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test with Filteirng class. */

#include "FileImage.hpp"
#include "GradientCanny.hpp"
#include "Image.hpp"
#include "Matrix.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 6 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <output image> <lower threshold> <higher threshold> <sigma>" <<
    endl;
    cout << "\t\t<lower threshold>, <higher threshold>: Hysteresis thresholds From 0.0 to 1.0." << endl;
    cout << "\t\t<sigma>: Standard deviation of Gaussian filter. From 0.0 to +oo." << endl;
    return( 0 );
  }
  try {
    COMMENT( "Reading parameters.", 0 );
    Image< int > img( Read< int >( argv[ 1 ] ) );
    float low_thres = atof( argv[ 3 ] );
    float hig_thres = atof( argv[ 4 ] );
    float sigma = atof( argv[ 5 ] );

    COMMENT( "Running Canny.", 0 );
    Image< int > res( Gradient::Canny( img, low_thres, hig_thres, sigma ) );

    COMMENT( "Writing result.", 0 );
    Write( res, argv[ 2 ], argv[ 1 ] );

  }
  catch( std::ios_base::failure &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while writing to file." ) );
    throw( std::ios_base::failure( msg ) );
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

  return( 0 );
}
