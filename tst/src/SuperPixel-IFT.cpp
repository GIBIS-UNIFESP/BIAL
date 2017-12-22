#include <queue>
#include "Adjacency.hpp"
#include "AdjacencyIterator.hpp"
#include "AdjacencyRound.hpp"
#include "Image.hpp"
#include "FileImage.hpp"
#include "RotatingBucketQueue.hpp"


using namespace std;
using namespace Bial;

void SuperVoxelDodecahedronInit( Image< int > &supervoxel_seed, double supervoxels ) {
  // Volume = 16*sqrt(3)*a^3/9 -> a = edge length. For cube with edge length = 1, a = sqrt(3).
  // Distance between dodecahedrons: sqrt(2) to each diagonal in 3D space.
  const double dodecahedron_volume_const = 9.0 / ( 16.0 * std::sqrt( 3.0 ) );
  COMMENT( "Computing scaling parameters for supervoxel position.", 0 );
  size_t image_volume = supervoxel_seed.size( );
  size_t x_size = supervoxel_seed.size( 0 );
  size_t y_size = supervoxel_seed.size( 1 );
  size_t z_size = supervoxel_seed.size( 2 );
  size_t x_size__2 = x_size / 2;
  size_t y_size__2 = y_size / 2;
  size_t z_size__2 = z_size / 2;
  double supervoxel_volume = image_volume / supervoxels;
  double supervoxel_edge_length = std::pow( supervoxel_volume * dodecahedron_volume_const, 1.0/3.0 );
  double supervoxel_dms_distance = supervoxel_edge_length * 2.0 / std::sqrt( 3.0 );
  double supervoxel_dms_distance__2 = std::max( 1.0, std::round( supervoxel_dms_distance / 2.0 ) );
  double elements_per_x_line = x_size / supervoxel_dms_distance;
  double elements_per_y_line = y_size / supervoxel_dms_distance;
  double elements_per_z_line = z_size / supervoxel_dms_distance;
  COMMENT( "image_volume: " << image_volume << ", supervoxel_volume: " << supervoxel_volume <<
	   ", supervoxel_edge_length: " << supervoxel_edge_length <<
	   ", supervoxel_dms_distance: " << supervoxel_dms_distance << endl <<
	   "elements_per_x_line: " << elements_per_x_line <<
	   ", elements_per_y_line: " << elements_per_y_line <<
	   ", elements_per_z_line: " << elements_per_z_line, 0 );
  COMMENT( "Preparing auxiliary vectors to compute dodecahedron coordinates.", 0 );
  queue< Vector< double > > queue; // Queue for BFS.
  Vector< double > src_vxl( 3, 0 );
  size_t idx_x = 0;
  size_t idx_y = 0;
  size_t idx_z = 0;
  for( double dist_z = z_size__2 - elements_per_z_line * supervoxel_dms_distance__2;
       dist_z < z_size; dist_z += supervoxel_dms_distance__2, ++idx_z ) {
    int z = dist_z;
    if( ( z < 0 ) || ( z >= static_cast< int >( z_size ) ) )
      continue;
    idx_y = 0;
    for( double dist_y = y_size__2 - elements_per_y_line * supervoxel_dms_distance__2;
	 dist_y < y_size; dist_y += supervoxel_dms_distance__2, ++idx_y ) {
      int y = dist_y;
      if( ( y < 0 ) || ( y >= static_cast< int >( y_size ) ) )
	continue;
      idx_x = 0;
      for( double dist_x = x_size__2 - elements_per_x_line * supervoxel_dms_distance__2;
	   dist_x < x_size; dist_x += supervoxel_dms_distance__2, ++idx_x ) {
	int x = dist_x;
	if( ( x < 0 ) || ( x >= static_cast< int >( x_size ) ) )
	  continue;
	if( ( idx_x + idx_y + idx_z ) % 2 == 0 )
	  supervoxel_seed( x, y, z ) = 1;
      }
    }
  }
}

template< class D >
Image< D > IFTSlic( const Image< D > &img, const Image< int > &seeds, float alpha, size_t iterations ) {
  try {
    COMMENT( "Computing initial values from input image.", 0 );
    size_t size = img.size( );
    size_t x_size = img.size( 0 );
    size_t y_size = img.size( 1 );
    size_t z_size = img.size( 2 );
    size_t x_size_1 = x_size - 1;
    size_t y_size_1 = y_size - 1;
    size_t z_size_1 = z_size - 1;
    size_t xy_size = img.Displacement( 1 );
    IF_DEBUG( seeds.size( ) != size ) {
      std::string msg( BIAL_ERROR( "image and seed vector must have the same number of elements." ) );
      throw( std::logic_error( msg ) );
    }
    COMMENT( "Creating images.", 0 );
    Image< int > label( img.Dim( ), img.PixelSize( ) );
    Image< D > value( img );
    Image< D > root( img );
    COMMENT( "Setting seeds. Image size: " << size, 0 );
    RotatingBucketQueue queue( size, img.Maximum( ) + 10 );
    size_t superpixels = 0;
    for( size_t pxl = 0; pxl < size; ++pxl ) {
      root[ pxl ] = pxl;
      if( seeds[ pxl ] != 0 ) {
	value[ pxl ] = 0;
	queue.Insert( pxl, value[ pxl ] );
	++superpixels;
      }
      else
	value[ pxl ] = std::numeric_limits< D >::max( );
    }
    COMMENT( "Setting adjacency related variables.", 0 );
    Adjacency spheric( AdjacencyType::HyperSpheric( 1.1, img.Dims( ) ) );
    AdjacencyIterator adj_itr( img, spheric );
    size_t adj_size = spheric.size( );
    size_t adj_index;
    Vector< size_t > src_vector( 3 );
    Vector< size_t > adj_vector( 3 );
    for( size_t itr = 0; itr < iterations; ++itr ) {
      size_t next_label = 0;
      COMMENT( "Running Image IFT. Queue: " << ( queue.Empty( ) ? "empty" : "not empty" ), 0 );
      while( !queue.Empty( ) ) {
	COMMENT( "Initializing removed data.", 4 );
	int src_index = queue.Remove( );
	if( queue.State( src_index ) == BucketState::INSERTED ) {
	  label[ src_index ] = next_label;
	  ++next_label;
	}
	COMMENT( "Src_Index: " << src_index << ", value: " << value[ src_index ], 4 );
	queue.Finished( src_index );
	div_t div_index_by_xy = std::div( static_cast< int >( src_index ), static_cast< int >( xy_size ) );
	div_t div_rem_by_x =  std::div( div_index_by_xy.rem, static_cast< int >( x_size ) );
	src_vector[ 0 ] = static_cast< size_t >( div_rem_by_x.rem );
	src_vector[ 1 ] = static_cast< size_t >( div_rem_by_x.quot );
	src_vector[ 2 ] = static_cast< size_t >( div_index_by_xy.quot );
	if( ( src_vector[ 0 ] > 0 ) && ( src_vector[ 0 ] < x_size_1 ) && ( src_vector[ 1 ] > 0 ) && 
	    ( src_vector[ 1 ] < y_size_1 ) && ( src_vector[ 2 ] > 0 ) && ( src_vector[ 2 ] < z_size_1 ) ) {
	  COMMENT( "For pixels that are not in the image border, there is no need for adjacency validation.", 4 );
	  for( size_t adj = 0; adj < adj_size; ++adj ) {
	    adj_index = adj_itr( src_index, adj );
	    if( queue.State( adj_index ) != BucketState::REMOVED ) {
	      COMMENT( "Conquering: " << adj_index, 4 );
	      D previous_value = value[ adj_index ];
	      COMMENT( "previous_value: " << previous_value, 4 );
	      adj_itr.AdjVct( src_vector, adj, adj_vector );
	      D dist = ( src_vector[ 0 ] - adj_vector[ 0 ] ) * ( src_vector[ 0 ] - adj_vector[ 0 ] ) +
		( src_vector[ 1 ] - adj_vector[ 1 ] ) * ( src_vector[ 1 ] - adj_vector[ 1 ] ) +
		( src_vector[ 2 ] - adj_vector[ 2 ] ) * ( src_vector[ 2 ] - adj_vector[ 2 ] );
	      D arc_weight = std::abs( img( root( src_index ) ) - img( adj_index ) ) * alpha + dist;
	      D prp_value = value( src_index ) + arc_weight;
	      if( previous_value > prp_value ) {
		COMMENT( "propagated!", 4 );
		value( adj_index ) = prp_value;
		label( adj_index ) = label( src_index );
		root( adj_index ) = root( src_index );
		queue.Update( adj_index, previous_value, prp_value );
		COMMENT( "queue updated!", 4 );
	      }
	    }
	  }
	}
	else {
	  COMMENT( "For pixels in the image border, adjacency must be validated.", 4 );
	  for( size_t adj = 0; adj < adj_size; ++adj ) {
	    adj_index = adj_itr( src_index, adj );
	    if( ( adj_itr.AdjVct( src_vector, adj, adj_vector ) ) &&
		( queue.State( adj_index ) != BucketState::REMOVED ) ) {
	      COMMENT( "Conquering: " << adj_index, 3 );
	      D previous_value = value[ adj_index ];
	      COMMENT( "previous_value: " << previous_value, 3 );
	      D dist = ( src_vector[ 0 ] - adj_vector[ 0 ] ) * ( src_vector[ 0 ] - adj_vector[ 0 ] ) +
		( src_vector[ 1 ] - adj_vector[ 1 ] ) * ( src_vector[ 1 ] - adj_vector[ 1 ] ) +
		( src_vector[ 2 ] - adj_vector[ 2 ] ) * ( src_vector[ 2 ] - adj_vector[ 2 ] );
	      D arc_weight = std::abs( img( root( src_index ) ) - img( adj_index ) ) * alpha + dist;
	      D prp_value = value( src_index ) + arc_weight;
	      if( previous_value > prp_value ) {
		COMMENT( "propagated! From " << previous_value << ", to: " << prp_value, 3 );
		value( adj_index ) = prp_value;
		label( adj_index ) = label( src_index );
		root( adj_index ) = root( src_index );
		queue.Update( adj_index, previous_value, prp_value );
		COMMENT( "queue updated!", 4 );
	      }
	    }
	  }
	}
      }
      if( itr == iterations - 1 )
	break;
      COMMENT( "Updating the superpixel centers.", 3 );
      Vector< Vector< double > > center_coords( superpixels + 1, Vector< double >( 3, 0.0 ) );
      Vector< size_t > superpixel_size( superpixels + 1, 0 );
      for( size_t z = 0; z < z_size; ++z ) {
	for( size_t y = 0; y < y_size; ++y ) {
	  for( size_t x = 0; x < x_size; ++x ) {
	    int lbl = label( x, y, z );
	    if( lbl > superpixels )
	      cout << "Erro" << ": lbl: " << lbl << ", superpixels: " << superpixels << endl;
	    center_coords[ lbl ][ 0 ] += x;
	    center_coords[ lbl ][ 1 ] += y;
	    center_coords[ lbl ][ 2 ] += z;
	    ++superpixel_size[ lbl ];
	  }
	}
      }
      for( size_t lbl = 0; lbl <= superpixels; ++lbl ) {
	if( superpixel_size[ lbl ] > 0 ) {
	  double inv_size = 1.0 / superpixel_size[ lbl ];
	  center_coords[ lbl ][ 0 ] *= inv_size;
	  center_coords[ lbl ][ 1 ] *= inv_size;
	  center_coords[ lbl ][ 2 ] *= inv_size;
	}
      }
      COMMENT( "Updating priority queue to run next round of the IFT.", 0 );
      queue.Reset( );
      for( size_t pxl = 0; pxl < size; ++pxl ) {
	value[ pxl ] = std::numeric_limits< D >::max( );
	root[ pxl ] = pxl;
      }
      for( size_t elm = 0; elm <= superpixels; ++elm ) {
	if( superpixel_size[ elm ] > 0 ) {
	  size_t pxl = std::round( center_coords[ elm ][ 0 ] ) +
	    std::round( center_coords[ elm ][ 1 ] ) * x_size +
	    std::round( center_coords[ elm ][ 2 ] ) * xy_size;
	  if( queue.State( pxl ) != BucketState::INSERTED ) {
	    value[ pxl ] = 0;
	    queue.Insert( pxl, value[ pxl ] );
	  }
	}
      }
    }
    return( label );
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


int main( int argc, char *argv[ ] ) {
  if( argc != 6 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <supervoxels> <alpha> <iterations> <outputfile>" << endl;
    return( 0 );
  }
  COMMENT( "Reading input parameters", 0 );
  Image< int > img( Read< int >( argv[ 1 ] ) );
  Image< int > supervoxel_seed( img.Dim( ), img.PixelSize( ) );
  size_t supervoxels = atoi( argv[ 2 ] );
  float alpha = atof( argv[ 3 ] );
  size_t iterations = atoi( argv[ 4 ] );

  cout << "Computing seeds." << endl;
  SuperVoxelDodecahedronInit( supervoxel_seed, supervoxels );
  Write( supervoxel_seed, "dat/seeds.nii.gz" );
  cout << "Computing IFTSlic." << endl;
  Image< int > res( IFTSlic( img, supervoxel_seed, alpha, iterations ) );
  cout << "Writing result." << endl;
  Write( res, argv[ 5 ] );
    
  return( 0 );
}
