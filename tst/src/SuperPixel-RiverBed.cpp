/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2015/Jun/09 */
/* Content: Test file. */
/* Description: Test with RiverBed and SuperPixel function. */

#include "AdjacencyRound.hpp"
#include "AdjacencyIterator.hpp"
#include "DrawLine.hpp"
#include "EdgeRiverBed.hpp"
#include "FileImage.hpp"
#include "Graph.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  /*
   * if( argc < 5 ) {
   *   cout << "Usage: " << argv[ 0 ] << " <input image> <output basename> <seed file> <superpixel mask image>" << endl;
   *   cout << "\t\t<output basename>: Basename completed with extensions for cost and predecessor images." << endl;
   *   cout << "\t\t<seed file>: Text file containing the coordinates of the pixels, each dimension separated by space "
   * <<
   *     "and one pixel per line. The file begins with a number that gives the number of dimensions." << endl;
   *   cout << "\t\t<mask image>: Binary image. It is different of zero where the algorithm should run." << endl;
   *   return( 0 );
   * }
   */
  COMMENT( "Reading input images.", 0 );
  Image< int > img( Read< int >( argv[ 1 ] ) );
  Image< int > lbl( Read< int >( argv[ 2 ] ) );
  COMMENT( "Locating the intersections between regions.", 0 );
  Vector< Vector< size_t > > intersection;
  Adjacency adj( AdjacencyType::HyperSpheric( 1.9, lbl.Dims( ) ) );
  AdjacencyIterator adj_itr( img, adj );
  size_t img_size = img.size( );
  size_t adj_size = adj.size( );
  size_t adj_pxl;
  for( size_t pxl = 0; pxl < img_size; ++pxl ) {
    int src_lbl = lbl( pxl );
    int adj_lbl = -1;
    for( size_t idx = 0; idx < adj_size; ++idx ) {
      if( ( adj_itr.*adj_itr.AdjIdx )( pxl, idx, adj_pxl ) ) {
        COMMENT( "Checking for adjacent pixels with distinct label.", 4 );
        if( lbl( adj_pxl ) != src_lbl ) {
          COMMENT( "Requires two distinct labels from the source to set as intersection point.", 4 );
          if( ( adj_lbl != -1 ) && ( adj_lbl != lbl( adj_pxl ) ) ) {
            intersection.push_back( lbl.Coordinates( pxl ) );
            break;
          }
          adj_lbl = lbl( adj_pxl );
        }
      }
    }
  }
  COMMENT( "Number of intersections found: " << intersection.size( ), 0 );
  COMMENT( "Removing redundant pixels.", 0 );
  Vector< Vector< size_t > > vertex;
  for( size_t src_vtx = intersection.size( ) - 1; src_vtx < intersection.size( ); --src_vtx ) {
    COMMENT( "Getting labels of the source element.", 3 );
    Vector< int > src_lbl;
    bool valid = true;
    size_t pxl = lbl.Position( intersection[ src_vtx ] );
    for( size_t idx = 0; idx < adj_size; ++idx ) {
      if( ( adj_itr.*adj_itr.AdjIdx )( pxl, idx, adj_pxl ) ) {
        bool insert = true;
        int new_lbl = lbl[ adj_pxl ];
        for( size_t sl = 0; sl < src_lbl.size( ); ++sl ) {
          if( src_lbl[ sl ] == new_lbl ) {
            insert = false;
            break;
          }
        }
        if( insert )
          src_lbl.push_back( new_lbl );
      }
    }
    sort( src_lbl.begin( ), src_lbl.end( ) );
    /* cout << "src lbls: " << src_lbl.size( ) << endl; */
    for( size_t tgt_vtx = src_vtx + 1; tgt_vtx < intersection.size( ); ++tgt_vtx ) {
      COMMENT( "Getting labels of the target element.", 3 );
      Vector< int > tgt_lbl;
      size_t pxl = lbl.Position( intersection[ tgt_vtx ] );
      for( size_t idx = 0; idx < adj_size; ++idx ) {
        if( ( adj_itr.*adj_itr.AdjIdx )( pxl, idx, adj_pxl ) ) {
          bool insert = true;
          int new_lbl = lbl[ adj_pxl ];
          for( size_t tl = 0; tl < tgt_lbl.size( ); ++tl ) {
            if( tgt_lbl[ tl ] == new_lbl ) {
              insert = false;
              break;
            }
          }
          if( insert )
            tgt_lbl.push_back( new_lbl );
        }
      }
      sort( tgt_lbl.begin( ), tgt_lbl.end( ) );
      /* cout << "tgt lbls: " << src_lbl.size( ) << endl; */
      COMMENT( "Looking for adjacents if they have all superpixels in common.", 4 );
      if( src_lbl == tgt_lbl ) {
        valid = false;
        break;
      }
    }
    if( valid )
      vertex.push_back( intersection[ src_vtx ] );
  }
  COMMENT( "Number of vertices found: " << vertex.size( ), 0 );
  COMMENT( "Creating edges.", 0 );
  Vector< Vector< size_t > > edge_idx;
  Vector< Vector< int > > edge_lbl;
  for( size_t src_vtx = vertex.size( ) - 2; src_vtx < vertex.size( ); --src_vtx ) {
    COMMENT( "Getting labels of the source element.", 3 );
    Vector< int > src_lbl;
    size_t pxl = lbl.Position( vertex[ src_vtx ] );
    for( size_t idx = 0; idx < adj_size; ++idx ) {
      if( ( adj_itr.*adj_itr.AdjIdx )( pxl, idx, adj_pxl ) ) {
        bool insert = true;
        int new_lbl = lbl[ adj_pxl ];
        for( size_t sl = 0; sl < src_lbl.size( ); ++sl ) {
          if( src_lbl[ sl ] == new_lbl ) {
            insert = false;
            break;
          }
        }
        if( insert )
          src_lbl.push_back( new_lbl );
      }
    }
    sort( src_lbl.begin( ), src_lbl.end( ) );
    /* cout << "src lbls: " << src_lbl.size( ) << endl; */
    for( size_t tgt_vtx = src_vtx + 1; tgt_vtx < vertex.size( ); ++tgt_vtx ) {
      COMMENT( "Getting labels of the target element.", 4 );
      Vector< int > tgt_lbl;
      size_t pxl = lbl.Position( vertex[ tgt_vtx ] );
      for( size_t idx = 0; idx < adj_size; ++idx ) {
        if( ( adj_itr.*adj_itr.AdjIdx )( pxl, idx, adj_pxl ) ) {
          bool insert = true;
          int new_lbl = lbl[ adj_pxl ];
          for( size_t tl = 0; tl < tgt_lbl.size( ); ++tl ) {
            if( tgt_lbl[ tl ] == new_lbl ) {
              insert = false;
              break;
            }
          }
          if( insert )
            tgt_lbl.push_back( new_lbl );
        }
      }
      sort( tgt_lbl.begin( ), tgt_lbl.end( ) );
      /* cout << "tgt lbls: " << src_lbl.size( ) << endl; */
      COMMENT( "Looking for adjacents if they have two superpixels in common.", 4 );
      size_t sl = 0, tl = 0;
      Vector< int > same_lbl;
      while( ( sl < src_lbl.size( ) ) && ( tl < tgt_lbl.size( ) ) ) {
        if( src_lbl[ sl ] == tgt_lbl[ tl ] ) {
          same_lbl.push_back( src_lbl[ sl ] );
          ++sl;
          ++tl;
        }
        else if( src_lbl[ sl ] < tgt_lbl[ tl ] )
          ++sl;
        else
          ++tl;
      }
      COMMENT( "Inserting edges linking vertices with two common labels.", 4 );
      if( same_lbl.size( ) == 2 ) {
        sort( same_lbl.begin( ), same_lbl.end( ) );
        edge_lbl.push_back( same_lbl );
        edge_idx.push_back( { tgt_vtx, src_vtx } );
      }
      else if( same_lbl.size( ) > 2 )
        BIAL_WARNING( "Multiple edges between superpixels." );
    }
  }
  COMMENT( "edges: " << edge_lbl.size( ), 0 );
  Image< int > graph( img );
  for( size_t vtx = 0; vtx < vertex.size( ); ++vtx ) {
    for( int x = -1; x < 1; ++x ) {
      for( int y = -1; y < 1; ++y )
        graph( vertex[ vtx ][ 0 ] + x, vertex[ vtx ][ 1 ] + y ) = 255;
    }
  }
  for( size_t edg = 0; edg < edge_idx.size( ); ++edg ) {
    Line ln( vertex[ edge_idx[ edg ][ 0 ] ], vertex[ edge_idx[ edg ][ 1 ] ], { 0, 1, 0 } );
    ln.Draw( graph );
  }
  Write( graph, "dat/graph.ppm" );
  COMMENT( "Computing the feature for each superpixel. Now using the mean intensity of edges.", 0 );
  Vector< int > mean( edge_lbl.size( ), 0 );
  Vector< size_t > size( edge_lbl.size( ), 0 );
  for( size_t pxl = 0; pxl < lbl.size( ); ++pxl ) {
    Vector< int > adj_lbl( 1, lbl( pxl ) );
    for( size_t idx = 0; idx < adj_size; ++idx ) {
      if( ( adj_itr.*adj_itr.AdjIdx )( pxl, idx, adj_pxl ) ) {
        COMMENT( "Checking for adjacent pixels with distinct label.", 4 );
        int new_lbl = lbl( adj_pxl );
        if( new_lbl != adj_lbl( 0 ) ) {
          adj_lbl.push_back( new_lbl );
          sort( adj_lbl.begin( ), adj_lbl.end( ) );
          for( size_t edg = 0; edg < edge_lbl.size( ); ++edg ) {
            if( edge_lbl( edg ) == adj_lbl ) {
              mean( edg ) += std::abs( img( pxl ) - img( adj_pxl ) ) +
                std::abs( img( pxl + img.size( ) ) - img( adj_pxl + img.size( ) ) ) +
                std::abs( img( pxl + 2 * img.size( ) ) - img( adj_pxl + 2 * img.size( ) ) );
              ++size( edg );
            }
          }
          break;
        }
      }
    }
  }
  for( size_t edg = 0; edg < mean.size( ); ++edg ) {
    if( size( edg ) != 0 )
      mean( edg ) /= size( edg );
  }
  Image< int > feat_img( lbl );
  feat_img.Set( 0 );
  for( size_t vtx = 0; vtx < vertex.size( ); ++vtx ) {
    for( int x = -1; x < 1; ++x ) {
      for( int y = -1; y < 1; ++y )
        feat_img( vertex[ vtx ][ 0 ] + x, vertex[ vtx ][ 1 ] + y ) = 255;
    }
  }
  float max = mean.Maximum( );
  for( size_t edg = 0; edg < edge_idx.size( ); ++edg ) {
    /*
     * Line ln( vertex[ edge_idx[ edg ][ 0 ] ], vertex[ edge_idx[ edg ][ 1 ] ],
     *          { mean( edg ) / max, mean( edg ) / max, mean( edg ) / max } );
     */
    Line ln( vertex[ edge_idx[ edg ][ 0 ] ], vertex[ edge_idx[ edg ][ 1 ] ], { mean( edg ) / max } );
    ln.Draw( feat_img );
  }
  Write( feat_img, "dat/superpixel_feature.pgm" );
  /*
   * COMMENT( "Running RiverBed.", 0 );
   * Image< float > cost;
   * Image< int > pred;
   * std::tie( cost, pred ) = Edge::RiverBed( img, msk, seed );
   */
  /*
   * COMMENT( "Seting pixels for frendly displaying.", 0 );
   * for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
   *   if( msk[ pxl ] == 0 ) {
   *     cost[ pxl ] = 0;
   *     pred[ pxl ] = 0;
   *   }
   *   if( pred[ pxl ] < 0 )
   *     pred[ pxl ] = pxl;
   * }
   */
  /*
   * Write( cost, string( argv[ 2 ] ) + "-cst" + ( img.Dims( ) == 2 ? ".pgm" : ".nii.gz" ), argv[ 1 ] );
   * Write( pred, string( argv[ 2 ] ) + "-prd" + ( img.Dims( ) == 2 ? ".pgm" : ".nii.gz" ), argv[ 1 ] );
   */
  return( 0 );
}
