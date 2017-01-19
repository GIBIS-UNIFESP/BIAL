/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2015/Nov/12 */
/* Content: Test file. */
/* Description: Segmentation of disconnected objects. */

#include "AdjacencyRound.hpp"
#include "AdjacencyIterator.hpp"
#include "Common.hpp"
#include "FastIncreasingFifoBucketQueue.hpp"
#include "FileImage.hpp"
#include "Image.hpp"
#include "ImageIFT.hpp"
#include "EdgeMaxPathFunction.hpp"

using namespace Bial;
using namespace std;

int main( int argc, char *argv[ ] ) {
  if( argc < 4 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <seed image> <output image> [<connected output image>] [<energy threshold>]" << endl;
    cout << "\t<seed image>:\tSame dimensions as input image. " << endl;
    cout << "\t\t\tNon-seed pixels: Intensity = 0" << endl;
    cout << "\t\t\tbackground seed pixels: Intensity = 1" << endl;
    cout << "\t\t\tobject seed pixels: Intensity > 1" << endl;
    cout << "\t<energy threshold>:\tA value greater than or equal to zero." << endl;
    return( 0 );
  }
  COMMENT( "Reading input image.", 0 );
  Image< int > src_cst( Read< int >( argv[ 1 ] ) );
  Image< int > bkg_cst( src_cst );
  Image< int > init_value( src_cst );
  size_t size = src_cst.size( );
  COMMENT( "Creating path function.", 0 );
  Image< int > label( src_cst.Dim( ), src_cst.PixelSize( ) );
  EdgeMaxPathFunction< int > pf( src_cst, &label, nullptr, false, init_value, true );
  FastIncreasingFifoBucketQueue queue( size, 0, src_cst.Maximum( ) + 2 );
  COMMENT( "Reading seeds.", 0 );
  Image< int > seed_img( Read< int >( argv[ 2 ] ) );
  if( src_cst.size( ) != seed_img.size( ) ) {
    cout << "Error: seed image and image dimensions do not match." << endl;
    return( 0 );
  }
  COMMENT( "Defining seeds, seed labels, and initial IFT cost.", 0 );
  Vector< bool > seed( size, false );
  Vector< bool > bkg_seed( size, false );
  src_cst.Set( std::numeric_limits< int >::max( ) );
  for( size_t pxl = 0; pxl < size; ++pxl ) {
    label[ pxl ] = seed_img[ pxl ];
    if( label[ pxl ] != 255 ) {
      src_cst[ pxl ] = 0;
      queue.Insert( pxl, src_cst[ pxl ] );
      seed[ pxl ] = true;
    }
    if( label[ pxl ] == 0 ) {
      bkg_seed[ pxl ] = true;
      seed[ pxl ] = true;
    }
  }
  Adjacency adj( AdjacencyType::HyperSpheric( 1.1, src_cst.Dims( ) ) );
  AdjacencyIterator adj_itr( src_cst, adj );
  size_t adj_size = adj.size( );
  size_t adj_pxl;
  ImageIFT< int > ift( src_cst, adj, &pf, &queue );
  ift.Run( );
  if( argc >= 5 )
    Write( label, argv[ 4 ] );
  Write( src_cst, "connected_ift_cst.pgm" );
  COMMENT( "Checking if there are adjacent pixels from distinct object.", 0 );
  bool has_adjacent = false;
  for( size_t src_pxl = 0; src_pxl < size; ++src_pxl ) {
    queue.State( src_pxl, BucketState::NOT_VISITED );
    int src_lbl = label[ src_pxl ];
    if( src_lbl != 0 ) {
      for( size_t idx = 0; idx < adj_size; ++idx ) {
        if( ( adj_itr.*adj_itr.AdjIdx )( src_pxl, idx, adj_pxl ) ) {
          int adj_lbl = label[ adj_pxl ];
          if( ( adj_lbl != 0 ) && ( adj_lbl != src_lbl ) )
            has_adjacent = true;
        }
      }
    }
  }
  if( !has_adjacent ) {
    COMMENT( "Object seeds are disconnected with original IFT. Returning results.", 0 );
    Write( label, argv[ 3 ] );
    DEBUG_WRITE( src_cst, "disconnected_ift_cst", 0 );
    return( 0 );
  }
  pf = EdgeMaxPathFunction< int >( bkg_cst, nullptr, nullptr, false, init_value, true );
  bkg_cst.Set( std::numeric_limits< int >::max( ) );
  for( size_t pxl = 0; pxl < size; ++pxl ) {
    if( bkg_seed[ pxl ] ) {
      bkg_cst[ pxl ] = 0;
      queue.Insert( pxl, bkg_cst[ pxl ] );
    }
  }
  COMMENT( "Running the same IFT but this time only with background seeds.", 0 );  
  ImageIFT< int > ift2( bkg_cst, adj, &pf, &queue );
  ift2.Run( );
  Write( bkg_cst, "backgorund_ift_cst.pgm" );
  COMMENT( "Finding new backgournd seeds.", 0 );
  int thres = 0;
  if( argc == 6 )
    thres = atoi( argv[ 5 ] );
  cout << "thres: " << thres << endl;
  for( size_t src_pxl = 0; src_pxl < size; ++src_pxl ) {
    queue.State( src_pxl, BucketState::NOT_VISITED );
    int src_lbl = label[ src_pxl ];
    if( src_lbl != 0 ) {
      COMMENT( "Computing energy variation. dE(pxl)=E_bkg(pxl)-E_img(pxl)", 3 );
      bool lower_energy = true;
      has_adjacent = false;
      int src_energy = bkg_cst( src_pxl ) - src_cst( src_pxl );
      if( src_energy > thres ) {
        continue;
      }
      for( size_t idx = 0; idx < adj_size; ++idx ) {
        if( ( adj_itr.*adj_itr.AdjIdx )( src_pxl, idx, adj_pxl ) ) {
          int adj_lbl = label[ adj_pxl ];
          if( ( adj_lbl != 0 ) && ( adj_lbl != src_lbl ) ) {
            has_adjacent = true;
            COMMENT( "intersection point.", 4 );
            COMMENT( "If pixel have the same variation of energy of adjancents, both become bkg seeds.", 4 );
            int adj_energy = bkg_cst( adj_pxl ) - src_cst( adj_pxl );
            if( adj_energy < src_energy ) {
              lower_energy = false;
              break;
            }
          }
        }
      }
      if( ( has_adjacent ) && ( lower_energy ) ) {
        COMMENT( "Pixel " << src_pxl << 
                 " becomes seed because it has lower energy variation with respect to adjacents with other label.", 3 );
        seed( src_pxl ) = true;
        label[ src_pxl ] = 0;
      }
    }
  }
  // COMMENT( "Finding new backgournd seeds from the same object.", 0 );
  // for( size_t src_pxl = 0; src_pxl < size; ++src_pxl ) {
  //   bool lower_label = true;
  //   has_adjacent = false;
  //   int src_lbl = label[ src_pxl ];
  //   if( src_lbl != 0 ) {
  //     for( size_t idx = 0; idx < adj_size; ++idx ) {
  //       if( ( adj_itr.*adj_itr.AdjIdx )( src_pxl, idx, adj_pxl ) ) {
  //         int adj_lbl = label[ adj_pxl ];
  //         if( ( adj_lbl != 0 ) && ( adj_lbl != src_lbl ) ) {
  //           has_adjacent = true;
  //           if( adj_lbl < src_lbl )
  //             lower_label = false;
  //         }
  //       }
  //     }
  //     if( ( has_adjacent ) && ( !lower_label ) ) {
  //       COMMENT( "Pixel " << src_pxl << 
  //                " becomes seed because it has lower energy variation with respect to adjacents with other label.", 3 );
  //       seed( src_pxl ) = true;
  //       label[ src_pxl ] = 0;
  //     }
  //   }
  // }
  pf = EdgeMaxPathFunction< int >( src_cst, &label, nullptr, false, init_value, true );
  COMMENT( "final_seeds.", 0 );
  Image< int > print_seed( src_cst );
  print_seed.Set( 255 );
  src_cst.Set( std::numeric_limits< int >::max( ) );
  for( size_t elm = 0; elm < size; ++elm ) {
    if( seed( elm ) == true ) {
      COMMENT( "seed: " << src_cst.Coordinates( elm ), 3 );
      src_cst[ elm ] = 0;
      print_seed( elm ) = label( elm );
      queue.Insert( elm, src_cst[ elm ] );
    }
  }
  Write( print_seed, "final_seeds.pgm" );
  ImageIFT< int > ift3( src_cst, adj, &pf, &queue );
  ift3.Run( );
  Write( label, argv[ 3 ] );
  Write( src_cst, "disconnected_ift_cst.pgm" );
  return( 0 );
}
