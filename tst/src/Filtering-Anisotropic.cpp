/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Caio Augusto Palma */
/* Date: 2012/Sep/21 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test with filtering. */

#include "DiffusionFunction.hpp"
#include "FileImage.hpp"
#include "FilteringAnisotropicDiffusion.hpp"
#include "Image.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( ( argc < 3 ) || ( argc > 7 ) ) {
    cout << "Usage: " << argv[ 0 ] << " <Input image> <output image> [<diffusion function> [<iterations> [<kappa> " <<
    "[<adjacency radius>]]]] " << endl;
    cout << "\t\t<diffusion function>: 0: Power(1.0); 1: Power(2.0); 2: Gaussian; 3: Robust(0.5); 4: Robust(1.0)."
         << "Default: 4." << endl;
    cout << "\t\t<iterations>: 1 to 1000. Default: 3." << endl;
    cout << "\t\t<kappa>: 1.0 to 10000.0. Default: 10.0." << endl;
    cout << "\t\t<adjacency radius>: 1.0 to 3.0. Default: 1.01." << endl;
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
  float radius = 1.01;
  if( argc > 6 ) {
    radius = atof( argv[ 6 ] );
    if( ( radius < 1.0 ) || ( radius > 3.0 ) ) {
      cout << "Error: Invalid adjacency radius. Expected: 1.0 to 3.0. Found: " << radius << endl;
      return( 0 );
    }
  }
  DiffusionFunction *diff_func;
  if( argc < 4 ) {
    diff_func = new RobustDiffusionFunction( 1.0 );
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
      diff_func = new RobustDiffusionFunction( 0.5 );
    }
    else if( func == 4 ) {
      diff_func = new RobustDiffusionFunction( 1.0 );
    }
    else {
      cout << "Error: Invalid diffusion function option. Expected: 0 to 4. Found: " << func << endl;
      return( 0 );
    }
  }
  Image< int > res( Filtering::AnisotropicDiffusion( src, diff_func, kappa, iterations, radius ) );

  Write( res, argv[ 2 ] );
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
    else if( func == 3 ) {
      delete ( dynamic_cast< RobustDiffusionFunction* >( diff_func ) );
    }
    else { /* if( func == 4 ) */
      delete ( dynamic_cast< RobustDiffusionFunction* >( diff_func ) );
    }
  }
  return( 0 );

}
