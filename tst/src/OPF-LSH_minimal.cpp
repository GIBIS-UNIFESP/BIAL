/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2013/Sep/21 */
/* Content: Test file. */
/* Description: Test with OPF class and LSH. */

#include "AdjacencyRound.hpp"
#include "BasicDefinitions.hpp"
#include "ColorFeature.hpp"
#include "Feature.hpp"
#include "FileImage.hpp"
#include "GlobalVars.hpp"
#include "Image.hpp"
#include "LSH.hpp"
#include "Vector.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char *argv[] ) {
  if( argc != 2 ) {
    cout << "Usage: " << argv[ 0 ] << " <input color image>" << endl;
    return( 0 );
  }
  Image< int > img( Read< int >( argv[ 1 ] ) );
  Adjacency adj( AdjacencyType::HyperSpheric( 0.1, img.Dims( ) ) );
  Feature< int > feature = ColorFeature< int >( img, adj );
  Vector< float > radius( 1, 5.0 );

  LSH_Init( &feature( 0, 0 ), feature.Elements( ), feature.Features( ), &radius[ 0 ], radius.size( ) );

  Vector< int > index( feature.Elements( ) - 1 );
  Vector< float > distance( feature.Elements( ) - 1 );
  size_t adjs = LSH_Query( 0, 1.0, &( index[ 0 ] ), &( distance[ 0 ] ) );
  std::cout << "# adjacents: " << adjs << std::endl;
  std::cout << "Index(distance):" << std::endl;
  for( size_t elm = 0; elm < adjs; ++elm ) {
    std::cout << index[ elm ] << ", " << distance[ elm ] << "; ";
  }
  std::cout << std::endl;

  adjs = LSH_Query( 1000, 1.0, &( index[ 0 ] ), &( distance[ 0 ] ) );
  std::cout << "# adjacents: " << adjs << std::endl;
  std::cout << "Index(distance):" << std::endl;
  for( size_t elm = 0; elm < adjs; ++elm ) {
    std::cout << index[ elm ] << ", " << distance[ elm ] << "; ";
  }
  std::cout << std::endl;

  LSH_Free( );

  return( 0 );
}
