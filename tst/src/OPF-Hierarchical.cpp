/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2013/Jul/26 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Basic test with MRI image class. */

#include "HierarchicalGraph.hpp"
#include "Image.hpp"
#include "Feature.hpp"
#include "FileFeature.hpp"
#include "LSHGraphAdjacency.hpp"
#include "OPFHierarchicalClustering.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char *argv[] ) {
  if( argc != 5 ) {
    cout << "Usage: " << argv[ 0 ] << " <input features> <minimum scale> <maximum scale> <graph adjacency type>" <<
    endl;
    cout << "\t<minimum scale> <maximum scale>: In 0.0 <= <minimum scale> <= <maximum scale> <= 1.0." << endl;
    cout << "\t<graph adjacency type>: 0-Knn, 1-LSH." << endl;
    return( 0 );
  }
  COMMENT( "Reading features.", 0 );
  Feature< int > feature = FileFeature::Read< int >( argv[ 1 ] );
  COMMENT( "Reading other parameters.", 0 );
  float min_scale = atof( argv[ 2 ] );
  float max_scale = atof( argv[ 3 ] );
  int graph_type = atoi( argv[ 4 ] );

  COMMENT( "Running Hierarchical OPF.", 0 );
  size_t nlabels;
  if( graph_type == 1 ) {
    HierarchicalGraph< LSHGraphAdjacency > graph;
    nlabels = OPF::HierarchicalClustering( feature, min_scale, max_scale, graph );
  }
  else
    nlabels = OPF::HierarchicalClustering( feature, min_scale, max_scale );    

  cout << "Labels: " << nlabels << endl;
  cout << feature.Label( ) << endl;

  return( 0 );
}
