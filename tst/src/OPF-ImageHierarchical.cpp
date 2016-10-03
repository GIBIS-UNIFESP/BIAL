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
#include "HierarchicalGraph.hpp"
#include "Image.hpp"
#include "LSHGraphAdjacency.hpp"
#include "MedianFeature.hpp"
#include "OPFHierarchicalClustering.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char *argv[] ) {
  if( argc != 6 ) {
    cout << "Usage: " << argv[ 0 ] <<
    " <input image> <output image> <minimum scale> <maximum scale> <graph adjacency type>" << endl;
    cout << "\t<minimum scale> <maximum scale>: In 0.0 <= <minimum scale> <= <maximum scale> <= 1.0." << endl;
    cout << "\t<graph adjacency type>: 0-Knn, 1-LSH." << endl;
    return( 0 );
  }

  Feature< int > feature( 1, 1 );  
  float min_scale = atof( argv[ 3 ] );
  float max_scale = atof( argv[ 4 ] );
  int graph_type = atoi( argv[ 5 ] );
  
  string filename( argv[ 1 ] );
  string extension( File::ToLowerExtension
                    ( argv[ 1 ], static_cast< size_t >( std::max( 0, static_cast< int >( filename.size( ) ) - 8 ) ) ) );
  
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
    Adjacency adj = AdjacencyType::HyperSpheric( 1.9, img.Dims( ) );
    feature = MedianFeature( img, adj );
    label = Image< int >( img.Dim( ) );
  }

  size_t nlabels;
  if( graph_type == 1 ) {
    HierarchicalGraph< LSHGraphAdjacency > graph;
    nlabels = OPF::HierarchicalClustering( feature, min_scale, max_scale, graph );
  }
  else
    nlabels = OPF::HierarchicalClustering( feature, min_scale, max_scale );
  
  label.Set( 0 );
  for( size_t elm = 0; elm < feature.Elements( ); ++elm ) {
    label[ feature.Index( elm ) ] = feature.Label( elm );
  }
  cout << "Labels: " << nlabels << endl;

  Write( label, argv[ 2 ], argv[ 1 ] );

  return( 0 );
}
