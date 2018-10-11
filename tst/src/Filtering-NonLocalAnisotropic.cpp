/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2018/Oct/10 */
/* Content: Test file. */
/* Description: Test with filtering. */

#include "Filtering-NonLocalAnisotropicBasics.cpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( ( argc < 3 ) || ( argc > 9 ) ) {
    cout << "Usage: " << argv[ 0 ] << " <Input image> <output image> [<diffusion function> [<iterations> [<kappa> " <<
    "[<search radius> [<patch radius> [<patch adjacents>]]]]]] " << endl;
    cout << "\t\t<diffusion function>: 0: Power(1.0); 1: Power(2.0); 2: Gaussian; 3: Robust."
         << "Default: 3." << endl;
    cout << "\t\t<iterations>: 1 to 1000. Default: 3." << endl;
    cout << "\t\t<kappa>: 1.0 to 10000.0. Default: 10.0." << endl;
    cout << "\t\t<non-local search adjacency radius>: 0.0 to 10.0. Default: 5.00." << endl;
    cout << "\t\t<non-local patch adjacency radius>: 1.0 to 5.0. Default: 3.00." << endl;
    cout << "\t\t<non-local patch adjacents>: 0 to 4. Default: 1." << endl;
    return( 0 );
  }
  /* Reading inputs: */
  Image< int > src( Read< int >( argv[ 1 ] ) );

  size_t iterations = 3;
  if( argc > 4 ) {
    iterations = static_cast< size_t >( atoi( argv[ 4 ] ) );
    if( ( iterations < 1 ) || ( iterations > 1000 ) ) {
      cout << "Error: Invalid number of iterations. Expected: 1 to 1000. Found: " << iterations << endl;
      return( 0 );
    }
  }
  float kappa = 10.0;
  if( argc > 5 ) {
    kappa = atof( argv[ 5 ] );
    if( ( kappa < 1.0 ) || ( kappa > 10000.0 ) ) {
      cout << "Error: Invalid kappa value. Expected: 1.0 to 10000.0. Found: " << kappa << endl;
      return( 0 );
    }
  }
  float search_radius = 5.00;
  if( argc > 6 ) {
    search_radius = atof( argv[ 6 ] );
    if( ( search_radius < 0.0 ) || ( search_radius > 10.0 ) ) {
      cout << "Error: Invalid search adjacency radius. Expected: 0.0 to 10.0. Found: " << search_radius << endl;
      return( 0 );
    }
  }
  float patch_radius = 3.00;
  if( argc > 7 ) {
    patch_radius = atof( argv[ 7 ] );
    if( ( patch_radius < 1.0 ) || ( search_radius > 5.0 ) ) {
      cout << "Error: Invalid patch adjacency radius. Expected: 1.0 to 5.0. Found: " << patch_radius << endl;
      return( 0 );
    }
  }
  size_t patch_adjacents = 1;
  if( argc > 8 ) {
    patch_adjacents = atoi( argv[ 8 ] );
    if( ( patch_adjacents < 0 ) || ( patch_adjacents > 4 ) ) {
      cout << "Error: Invalid patch adjacents. Expected: 0 to 4. Found: " << patch_adjacents << endl;
      return( 0 );
    }
  }
  DiffusionFunction *diff_func;
  if( argc < 4 ) {
    diff_func = new RobustDiffusionFunction;
  }
  else {
    int func = atof( argv[ 3 ] );
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
      cout << "Error: Invalid diffusion function option. Expected: 0 to 3. Found: " << func << endl;
      return( 0 );
    }
  }
  Image< int > res( NonLocalAnisotropicDiffusion( src, diff_func, kappa, iterations, search_radius, patch_radius, patch_adjacents ) );
  NiftiHeader nii( res );
  
  Write( res, argv[ 2 ], argv[ 1 ] );
  if( argc < 4 ) {
    delete ( dynamic_cast< RobustDiffusionFunction* >( diff_func ) );
  }
  else {
    int func = atof( argv[ 3 ] );
    if( func == 0 ) {
      delete ( dynamic_cast< PowerDiffusionFunction* >( diff_func ) );
    }
    else if( func == 1 ) {
      delete ( dynamic_cast< PowerDiffusionFunction* >( diff_func ) );
    }
    else if( func == 2 ) {
      delete ( dynamic_cast< GaussianDiffusionFunction* >( diff_func ) );
    }
    else { /* if( func == 3 ) */
      delete ( dynamic_cast< RobustDiffusionFunction* >( diff_func ) );
    }
  }
  return( 0 );

}
