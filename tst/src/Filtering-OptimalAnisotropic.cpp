/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2013/Nov/01 */
/* Content: Test file. */
/* Description: Segmentation of background of images for the optimized anisotropic filter. */

#include "Common.hpp"
#include "DiffusionFunction.hpp"
#include "FileImage.hpp"
#include "FilteringAnisotropicDiffusion.hpp"
#include "FilteringOptimalAnisotropicDiffusion.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( ( argc < 4 ) || ( argc == 7 ) || ( argc > 8 ) ) {
    cout << "Usage: " << argv[ 0 ] << " <Input image> <output image> <conservativeness> [<diffusion function> "
         << "[<adjacency radius> [<edge_region> <flat_region>]]]" << endl;
    cout << "\t\t<conservativeness>: (conserve all edges) 0.0 to 1.0 (remove all noise)." << endl;
    cout << "\t\t<diffusion function>: 0: Power(1.0); 1: Power(2.0); 2: Gaussian; 3: Robust."
         << "Default: 3." << endl;
    cout << "\t\t<edge_region>: Canny edge detection." << endl;
    cout << "\t\t<flat_region>: Background segmentation." << endl;
    return( 0 );
  }

  /* Reading inputs: */
  Image< int > src( Read< int >( argv[ 1 ] ) );
  float conservativeness = atof( argv[ 3 ] );
  if( ( conservativeness < 0.0 ) || ( conservativeness > 1.0 ) ) {
    cout << "Error: Invalid conservativeness parameter. Expected: 0.0 to 1.0. Found: " << conservativeness << endl;
    return( 0 );
  }
  float radius = 1.01;
  if( argc > 5 ) {
    radius = atof( argv[ 5 ] );
    if( ( radius < 1.0 ) || ( radius > 3.0 ) ) {
      cout << "Error: Invalid adjacency radius. Expected: 1.0 to 3.0. Found: " << radius << endl;
      return( 0 );
    }
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
  if( argc > 6 ) {
    std::cout << "Using input images for flat and edge regions." << std::endl;
    Image< int > edge_region( Read< int >( argv[ 6 ] ) );
    Image< int > flat_region( Read< int >( argv[ 7 ] ) );
    Image< int > res( Filtering::OptimalAnisotropicDiffusion( src, diff_func, radius, conservativeness, 
                                                                edge_region, flat_region ) );
    Write( res, argv[ 2 ] );
  }
  else {
    std::cout << "Computing flat and edge region images." << std::endl;
    Image< int > res( Filtering::OptimalAnisotropicDiffusion( src, diff_func, radius, conservativeness ) );
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
