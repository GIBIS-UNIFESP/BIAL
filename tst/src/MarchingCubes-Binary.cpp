/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Lucas Santana Lellis */
/* Date: 2015/Aug/15 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test marching cubes algorithm. */

#include "AdjacencyRound.hpp"
#include "DrawShape.hpp"
#include "FileImage.hpp"
#include "Image.hpp"
#include "GradientMorphological.hpp"
#include "MarchingCubes.hpp"
#include "MorphologyDilation.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 6 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <input binary image> <output stl file> <isolevel> <radius>" <<
    endl;
    return( 0 );
  }
  try {
    COMMENT( "Loading input image.", 0 );
    Image< int > img( Read< int >( argv[ 1 ] ) );
    Image< int > mask( Read< int >( argv[ 2 ] ) );
    float isolevel = atof( argv[ 4 ] );
    float radius = atof( argv[ 5 ] );
    mask = Gradient::Morphological( mask );

    Adjacency adj = AdjacencyType::Spheric( radius );
    mask = Morphology::DilateBin( mask, adj );
    /* img *= mask; */

    TriangleMesh *mesh = MarchingCubes::Binary( img, mask, isolevel );

    /*
     * mesh->Print(std::cout);
     * std::cout << std::endl << "STL: " << std::endl;
     * mesh->ExportSTLA(std::cout);
     */
    mesh->ExportSTLB( argv[ 3 ] );
    
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
