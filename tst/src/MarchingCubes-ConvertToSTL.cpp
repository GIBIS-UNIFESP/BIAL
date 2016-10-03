/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Lucas Santana Lellis */
/* Date: 2015/Aug/15 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test marching cubes algorithm. */

#include "Adjacency.hpp"
#include "DrawShape.hpp"
#include "FileImage.hpp"
#include "Image.hpp"
#include "MarchingCubes.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 4 ) {
    cout << "Usage: " << argv[ 0 ] << " <input file name> <output file name> <isolevel>" << endl;
    return( 0 );
  }
  try {
    COMMENT( "Loading input image.", 0 );
    Image< int > img( Read< int >( argv[ 1 ] ) );
    float isolevel = atof( argv[ 3 ] );

    TriangleMesh *mesh = MarchingCubes::exec( img, isolevel );

    /*
     * mesh->Print(std::cout);
     * std::cout << std::endl << "STL: " << std::endl;
     * mesh->ExportSTLA(std::cout);
     */
    mesh->ExportSTLB( argv[ 2 ] );
    mesh->Print( cout << endl );
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
