/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2018/Dec/14 */
/* Content: Test file. */
/* Description: Counts the number of crossings between the border of two binary objects. */


#include "AdjacencyIterator.hpp"
#include "AdjacencySequential.hpp"
#include "AdjacencyRound.hpp"
#include "FileImage.hpp"
#include "Image.hpp"
#include "SegmentationBorder.hpp"
#include "SegmentationConnectedComponents.hpp"
#include "TransformEuclDist.hpp"

using namespace std;
using namespace Bial;

template< class D >
int RunContour( const Image< D > &src, const Image< D > &tgt_lbl, const Image< float > &src_dst, float dist ) {
  try {
    COMMENT( "Computing the size of each component.", 0 );
    size_t img_size = src.size( );
    size_t labels = tgt_lbl.Maximum( );
    Vector< size_t > label_freq( labels + 1, 0 );
    for( size_t pxl = 0; pxl < img_size; ++pxl )
      ++label_freq[ tgt_lbl[ pxl ] ];
    size_t crossings = 0;
    for( size_t lbl = 1; lbl <= labels; ++lbl ) {
      //cout << "label_freq[ " << lbl << " ]: " << label_freq[ lbl ] << endl;
      COMMENT( "Running algorithm for each valid label.", 0 );
      if( label_freq[ lbl ] < 8 ) {
	COMMENT( "Ignoring very small contours.", 0 );
	continue;
      }
      COMMENT( "Finding first pixel belonging to the target border.", 0 );
      size_t init_pxl = 0;
      while( ( tgt_lbl[ init_pxl ] != static_cast< int >( lbl ) ) && ( init_pxl < img_size ) )
	++init_pxl;
      if( init_pxl == img_size ) {
	std::string msg( BIAL_ERROR( "Could not find contour of valid label." ) );
	throw( std::logic_error( msg ) );
      }
      Adjacency adj_rel( AdjacencyType::Sequential( 1.5 ) );
      AdjacencyIterator adj_itr( tgt_lbl, adj_rel );
      size_t adj_rel_size = adj_rel.size( );
      size_t adj_idx = 3;
      size_t adj_pxl = 0;
      size_t curr_pxl = init_pxl;
      bool inside = ( src[ init_pxl ] != 0 );
      bool counted = false;
      if( src_dst[ curr_pxl ] >= dist ) {
	++crossings;
	counted = true;
      }
      size_t contour_size = 1;
      do {
	COMMENT( "Raster the adjacent pixels in clockwise direction, starting from the first adjacent to opposite direction of the last movement.", 3 );
	adj_idx = ( adj_idx + 5 ) % adj_rel_size;
	do {
	  if( ( adj_itr.AdjIdx2( curr_pxl, adj_idx, adj_pxl ) ) && ( tgt_lbl[ adj_pxl ] == static_cast< int >( lbl ) ) ) {
	    curr_pxl = adj_pxl;
	    if( inside != ( src[ curr_pxl ] != 0 ) ) {
	      counted = false;
	      inside = ( src[ curr_pxl ] != 0 );
	    }
	    if( ( !counted ) && ( src_dst[ curr_pxl ] >= dist ) ) {
	      counted = true;
	      ++crossings;
	      inside = ( src[ curr_pxl ] != 0 );
	    }
	    ++contour_size;
	    break;
	  }
	  adj_idx = ( adj_idx + 1 ) % adj_rel_size;
	} while( true );
      } while( curr_pxl != init_pxl );
    }
    return( crossings );
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

template< class D >
Image< D > GetZSlice( const Image< D > &img, size_t slice ) {
  try {
    size_t x_size = img.size( 0 );
    size_t y_size = img.size( 1 );
    Image< int > res( x_size, y_size );
    for( size_t y = 0; y < y_size; ++y ) {
      for( size_t x = 0; x < x_size; ++x )
	res( x, y ) = img( x, y, slice );
    }
    return( res );
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

template< class D >
void Crossings( const Image< D > &src, const Image< D > &tgt, float dist ) {
  try {
    size_t img_dims = src.Dims( );
    if( img_dims == 2 ) {
      COMMENT( "2D image.", 0 );
      Adjacency adj( AdjacencyType::Circular( 1.1 ) );
      Image< int > src_bdr( Segmentation::BorderPixels( src, adj ) );
      Image< float > src_dst( Transform::EDT( Image< float >( src_bdr ) ) );
      //Image< int > tgt_bdr( Segmentation::BorderPixels( src, adj ) );
      Image< D > tgt_lbl( Segmentation::ConnectedComponents( tgt, AdjacencyType::Circular( 1.8 ) ) );
      std::cout << RunContour( src, tgt_lbl, src_dst, dist ) << std::endl;
    }
    else {
      COMMENT( "3D image.", 0 );
      Adjacency adj( AdjacencyType::Spheric( 1.1 ) );
      Image< int > src_bdr( Segmentation::BorderPixels( src, adj ) );
      Image< float > src_dst( Transform::EDT( Image< float >( src_bdr ) ) );
      //Image< int > tgt_bdr( Segmentation::BorderPixels( src, adj ) );
      size_t z_size = src.size( 2 );
      cout << "dims: " << src.Dim( ) << endl;
      for( size_t z = 0; z < z_size; ++z ) {
	Image< D > src_slice( GetZSlice( src, z ) );
	Image< D > tgt_slice( GetZSlice( tgt, z ) );
	Image< float > src_dst_slice( GetZSlice( src_dst, z ) );
	Image< D > tgt_lbl_slice( Segmentation::ConnectedComponents( tgt_slice, AdjacencyType::Circular( 1.8 ) ) );
	std::cout << z << ": " << RunContour( src_slice, tgt_lbl_slice, src_dst_slice, dist ) << std::endl;
      }
    }
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

int main( int argc, char **argv ) {
  try {
    if( argc < 3 ) {
      cout << "Usage: " << argv[ 0 ] << " <binary image 1> <binary image 2> <minimum distance>" << endl;
      cout << "\t\t<minimum distance>: Minimum distance after effectively crossing the border to count as a crossing. [Minimum: 0]." << endl;
      return( 0 );
    }
    Image< int > src( Read< int >( argv[ 1 ] ) );
    Image< int > tgt( Read< int >( argv[ 2 ] ) );
    float dist = atof( argv[ 3 ] );
    if( src.Dim( ) != tgt.Dim( ) ) {
      cout << "Error. Source and target images have different sizes." << endl;
      return( 0 );
    }
    Crossings( src, tgt, dist );
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
