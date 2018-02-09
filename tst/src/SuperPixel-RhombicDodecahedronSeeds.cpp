#include <queue>
#include "Image.hpp"
#include "FileImage.hpp"

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

  cout << setprecision( 8 ) << "image_volume: " << image_volume << ", supervoxel_volume: " << supervoxel_volume <<
    ", supervoxel_edge_length: " << supervoxel_edge_length <<
    ", supervoxel_dms_distance: " << supervoxel_dms_distance << endl <<
    "elements_per_x_line: " << elements_per_x_line <<
    ", elements_per_y_line: " << elements_per_y_line <<
    ", elements_per_z_line: " << elements_per_z_line << endl;

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

int main( int argc, char *argv[ ] ) {
  if( argc != 6 ) {
    cout << "Usage: " << argv[ 0 ] << " <x> <y> <z> <supervoxels> <outputfile>" << endl;
    return( 0 );
  }
  COMMENT( "Reading input parameters", 0 );
  size_t x = atoi( argv[ 1 ] );
  size_t y = atoi( argv[ 2 ] );
  size_t z = atoi( argv[ 3 ] );
  size_t supervoxels = atoi( argv[ 4 ] );

  Image< int > supervoxel_seed( x, y, z );
  SuperVoxelDodecahedronInit( supervoxel_seed, supervoxels );
  Write( supervoxel_seed, argv[ 5 ] );
    
  return( 0 );
}
