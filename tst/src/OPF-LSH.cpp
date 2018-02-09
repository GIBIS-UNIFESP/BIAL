/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2014/Oct/23 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test with OPF class. */

#include "AdjacencyRound.hpp"
#include "Color.hpp"
#include "ColorFeature.hpp"
#include "Feature.hpp"
#include "FileImage.hpp"
#include "Image.hpp"
#include "LSH.hpp"
#include "MedianFeature.hpp"
#include "OPFSpectralClustering.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char *argv[] ) {
  if( argc != 5 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <output image> <minimum radius> <maximum radius>" << endl;
    cout << "\t<minimum radius> <maximum radius>: In 0.0 <= <minimum radius> <= <maximum radius> <= 1.0." << endl;
    return( 0 );
  }

  Feature< int > feature( 1, 1 );  
  float min_radius = atof( argv[ 3 ] );
  float max_radius = atof( argv[ 4 ] );
  
  string filename( argv[ 1 ] );
  string extension( File::ToLowerExtension( argv[ 1 ], 
                                            static_cast< size_t >( std::max( 0, static_cast< int >( filename.size( ) ) 
                                                                             - 8 ) ) ) );
  
  Image< int > label;
  if( extension.rfind( ".ppm" ) != std::string::npos ) {
    COMMENT( "Full adjacency multi-channel.", 0 );
    Image< Color > img( Read< Color >( argv[ 1 ] ) );
    Adjacency adj( AdjacencyType::HyperSpheric( 0.1, img.Dims( ) ) );
    feature = ColorFeature< int >( img, adj );
    label = Image< int >( img.Dim( ) );
  }
  else {
    COMMENT( "Adjacency based features.", 0 );
    Image< int > img( Read< int >( argv[ 1 ] ) );
    Adjacency adj( AdjacencyType::HyperSpheric( 1.9, img.Dims( ) ) );
    feature = MedianFeature( img, adj );
    label = Image< int >( img.Dim( ) );
  }

  /* size_t nlabels = */
  std::cout << "LSH call!" << std::endl;
  size_t nlabels = OPF::SpectralClustering( feature, min_radius, max_radius );
  label.Set( 0 );
  for( size_t elm = 0; elm < feature.Elements( ); ++elm ) {
    label[ feature.Index( elm ) ] = feature.Label( elm );
  }
  cout << "Labels: " << nlabels << endl;

  Write( label, argv[ 2 ], argv[ 1 ] );

  return( 0 );
}
