/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2018/Oct/10 */
/* Content: Test file. */
/* Description: Testing otimized non-local anisotropic filter. */

#include "Common.hpp"
#include "FileImage.hpp"
#include "Filtering-NonLocalAnisotropicBasics.cpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( ( argc < 4 ) || ( argc == 6 ) || ( argc > 11 ) ) {
    cout << "Usage: " << argv[ 0 ] << " <Input image> <output image> <conservativeness> [<diffusion function> "
         << "[<edge_region> <flat_region> [<non-local search adjacency radius> [<patch adjacency radius> "
	 << "[<patch distance> [<non-local patch adjacents>]]]]]]" << endl;
    cout << "\t\t<conservativeness>: (conserve all edges) 0.0 to 1.0 (remove all noise)." << endl;
    cout << "\t\t<diffusion function>: 0: Power(1.0); 1: Power(2.0); 2: Gaussian; 3: Robust."
         << "Default: 3." << endl;
    cout << "\t\t<edge_region>: Canny edge detection." << endl;
    cout << "\t\t<flat_region>: Background segmentation." << endl;
    cout << "\t\t<non-local search adjacency radius>: 0.0 to 5.0. Default: 3.0." << endl;
    cout << "\t\t<patch adjacency radius>: 1.0 to 1.9. Default: 1.5." << endl;
    cout << "\t\t<patch distance>: 0.01 to 2.0. Default: 0.01." << endl;
    cout << "\t\t<non-local patch adjacents>: 0 to 4. Default: 1." << endl;
    return( 0 );
  }

  /* Reading inputs: */
  Image< int > src( Read< int >( argv[ 1 ] ) );
  float conservativeness = atof( argv[ 3 ] );
  if( ( conservativeness < 0.0 ) || ( conservativeness > 1.0 ) ) {
    cout << "Error: Invalid conservativeness parameter. Expected: 0.0 to 1.0. Found: " << conservativeness << endl;
    return( 0 );
  }
  DiffusionFunction *diff_func;
  if( argc < 5 ) {
    diff_func = new RobustDiffusionFunction;
  }
  else {
    int func = atof( argv[ 4 ] );
    if( func == 0 ) {
      diff_func = new PowerDiffusionFunction( 1.0 );
    }
    else if( func == 1 ) {
      diff_func = new PowerDiffusionFunction( 2.0 );
    }
    else if( func == 2 ) {
      diff_func = new GaussianDiffusionFunction;
    }
    else if( func == 3 ) {
      diff_func = new RobustDiffusionFunction;
    }
    else {
      cout << "Error: Invalid diffusion function option. Expected: 0 to 4. Found: " << func << endl;
      return( 0 );
    }
  }
  float search_radius = 3.0f;
  if( argc > 7 ) {
    search_radius = atof( argv[ 7 ] );
    if( ( search_radius < 0.0f ) || ( search_radius > 5.0f ) ) {
      cout << "Error: Invalid search adjacency radius. Expected: 0.0 to 5.0. Found: " << search_radius << endl;
      return( 0 );
    }
  }
  float patch_radius = 1.5f;
  if( argc > 8 ) {
    patch_radius = atof( argv[ 8 ] );
    if( ( patch_radius < 1.0f ) || ( patch_radius > 1.9f ) ) {
      cout << "Error: Invalid patch adjacency radius. Expected: 1.0 to 1.9. Found: " << patch_radius << endl;
      return( 0 );
    }
  }
  float patch_distance = 0.01f;
  if( argc > 9 ) {
    patch_distance = atof( argv[ 9 ] );
    if( ( patch_distance < 0.009f ) || ( patch_distance > 2.0f ) ) {
      cout << "Error: Invalid patch distance. Expected: 0.01 to 2.0. Found: " << patch_distance << endl;
      return( 0 );
    }
  }
  size_t patch_adjacents = 1;
  if( argc > 10 ) {
    patch_adjacents = atoi( argv[ 10 ] );
    if( ( patch_adjacents < 0 ) || ( patch_adjacents > 4 ) ) {
      cout << "Error: Invalid patch adjacents. Expected: 0 to 4. Found: " << patch_adjacents << endl;
      return( 0 );
    }
  }
  if( argc > 5 ) {
    std::cout << "Using input images for flat and edge regions." << std::endl;
    Image< int > edge_region( Read< int >( argv[ 5 ] ) );
    Image< int > flat_region( Read< int >( argv[ 6 ] ) );
    Image< int > res( OptimalNonLocalAnisotropicDiffusion( src, diff_func, conservativeness, edge_region, flat_region,
							   search_radius, patch_radius, patch_distance, patch_adjacents ) );
    Write( res, argv[ 2 ] );
  }
  else {
    std::cout << "Computing flat and edge region images." << std::endl;
    Image< int > res( OptimalNonLocalAnisotropicDiffusion( src, diff_func, conservativeness, search_radius,
							   patch_radius, patch_distance, patch_adjacents ) );
    Write( res, argv[ 2 ] );
  }
  if( argc < 5 ) {
    delete ( dynamic_cast< RobustDiffusionFunction* >( diff_func ) );
  }
  else {
    int func = atof( argv[ 4 ] );
    if( func == 0 ) {
      delete ( dynamic_cast< PowerDiffusionFunction* >( diff_func ) );
    }
    else if( func == 1 ) {
      delete ( dynamic_cast< PowerDiffusionFunction* >( diff_func ) );
    }
    else if( func == 2 ) {
      delete ( dynamic_cast< GaussianDiffusionFunction* >( diff_func ) );
    }
    else if( func == 3 ) {
      delete ( dynamic_cast< RobustDiffusionFunction* >( diff_func ) );
    }
    else { /* if( func == 4 ) */
      delete ( dynamic_cast< RobustDiffusionFunction* >( diff_func ) );
    }
  }
  return( 0 );
}

