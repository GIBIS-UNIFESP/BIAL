/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2012/Jun/27 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Testing Nifti header orientation operations. */

#include "FileImage.hpp"
#include "Image.hpp"
#include "NiftiHeader.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc < 6 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image> <x> <y> <z> <output image>" << endl;
    cerr <<
    "<x>, <y>, and <z> can take values RL, LR, AP, PA, SI, IS, and must represent different orientations among them."
         <<
    endl;
    exit( -1 );
  }
  Image< int > mri( Read< int >( argv[ 1 ] ) );

  string x( argv[ 2 ] );
  string y( argv[ 3 ] );
  string z( argv[ 4 ] );
  Matrix< float > mat( 4, 4 );
  mat( 3, 3 ) = 1.0;
  mat( 0, 3 ) = 100.0;
  mat( 1, 3 ) = 100.0;
  mat( 2, 3 ) = 100.0;
  /* cout << "data readed: " << x << " " << y << " " << z << endl; */
  if( x.compare( "RL" ) == 0 ) {
    if( y.compare( "AP" ) == 0 ) {
      if( z.compare( "SI" ) == 0 ) {
        mat( 0, 0 ) = -1.0;
        mat( 1, 1 ) = -1.0;
        mat( 2, 2 ) = -1.0;
      }
      else if( z.compare( "IS" ) == 0 ) {
        mat( 0, 0 ) = -1.0;
        mat( 1, 1 ) = -1.0;
        mat( 2, 2 ) = +1.0;
        mat( 2, 3 ) = -100.0;
      }
      else {
        cerr <<
        "Error: <x>, <y>, and <z> can take values RL, LR, AP, PA, SI, IS, and must represent different orientations among them."
             << endl;
        exit( -1 );
      }
    }
    else if( y.compare( "PA" ) == 0 ) {
      if( z.compare( "SI" ) == 0 ) {
        mat( 0, 0 ) = -1.0;
        mat( 1, 1 ) = +1.0;
        mat( 2, 2 ) = -1.0;
        mat( 1, 3 ) = -100.0;
      }
      else if( z.compare( "IS" ) == 0 ) {
        mat( 0, 0 ) = -1.0;
        mat( 1, 1 ) = +1.0;
        mat( 2, 2 ) = +1.0;
        mat( 1, 3 ) = -100.0;
        mat( 2, 3 ) = -100.0;
      }
      else {
        cerr <<
        "Error: <x>, <y>, and <z> can take values RL, LR, AP, PA, SI, IS, and must represent different orientations among them."
             << endl;
        exit( -1 );
      }
    }
    else if( y.compare( "SI" ) == 0 ) {
      if( z.compare( "AP" ) == 0 ) {
        mat( 0, 0 ) = -1.0;
        mat( 2, 1 ) = -1.0;
        mat( 1, 2 ) = -1.0;
      }
      else if( z.compare( "PA" ) == 0 ) {
        mat( 0, 0 ) = -1.0;
        mat( 2, 1 ) = -1.0;
        mat( 1, 2 ) = +1.0;
        mat( 1, 3 ) = -100.0;
      }
      else {
        cerr <<
        "Error: <x>, <y>, and <z> can take values RL, LR, AP, PA, SI, IS, and must represent different orientations among them."
             << endl;
        exit( -1 );
      }
    }
    else if( y.compare( "IS" ) == 0 ) {
      if( z.compare( "AP" ) == 0 ) {
        mat( 0, 0 ) = -1.0;
        mat( 2, 1 ) = +1.0;
        mat( 1, 2 ) = -1.0;
        mat( 2, 3 ) = -100.0;
      }
      else if( z.compare( "PA" ) == 0 ) {
        mat( 0, 0 ) = -1.0;
        mat( 2, 1 ) = +1.0;
        mat( 1, 2 ) = +1.0;
        mat( 1, 3 ) = -100.0;
        mat( 2, 3 ) = -100.0;
      }
      else {
        cerr <<
        "Error: <x>, <y>, and <z> can take values RL, LR, AP, PA, SI, IS, and must represent different orientations among them."
             << endl;
        exit( -1 );
      }
    }
    else {
      cerr <<
      "Error: <x>, <y>, and <z> can take values RL, LR, AP, PA, SI, IS, and must represent different orientations among them."
           << endl;
      exit( -1 );
    }
  }
  else if( x.compare( "LR" ) == 0 ) {
    if( y.compare( "AP" ) == 0 ) {
      if( z.compare( "SI" ) == 0 ) {
        mat( 0, 0 ) = +1.0;
        mat( 1, 1 ) = -1.0;
        mat( 2, 2 ) = -1.0;
        mat( 0, 3 ) = -100.0;
      }
      else if( z.compare( "IS" ) == 0 ) {
        mat( 0, 0 ) = +1.0;
        mat( 1, 1 ) = -1.0;
        mat( 2, 2 ) = +1.0;
        mat( 0, 3 ) = -100.0;
        mat( 2, 3 ) = -100.0;
      }
      else {
        cerr <<
        "Error: <x>, <y>, and <z> can take values RL, LR, AP, PA, SI, IS, and must represent different orientations among them."
             << endl;
        exit( -1 );
      }
    }
    else if( y.compare( "PA" ) == 0 ) {
      if( z.compare( "SI" ) == 0 ) {
        mat( 0, 0 ) = +1.0;
        mat( 1, 1 ) = +1.0;
        mat( 2, 2 ) = -1.0;
        mat( 0, 3 ) = -100.0;
        mat( 1, 3 ) = -100.0;
      }
      else if( z.compare( "IS" ) == 0 ) {
        mat( 0, 0 ) = +1.0;
        mat( 1, 1 ) = +1.0;
        mat( 2, 2 ) = +1.0;
        mat( 0, 3 ) = -100.0;
        mat( 1, 3 ) = -100.0;
        mat( 2, 3 ) = -100.0;
      }
      else {
        cerr <<
        "Error: <x>, <y>, and <z> can take values RL, LR, AP, PA, SI, IS, and must represent different orientations among them."
             << endl;
        exit( -1 );
      }
    }
    else if( y.compare( "SI" ) == 0 ) {
      if( z.compare( "AP" ) == 0 ) {
        mat( 0, 0 ) = +1.0;
        mat( 2, 1 ) = -1.0;
        mat( 1, 2 ) = -1.0;
        mat( 0, 3 ) = -100.0;
      }
      else if( z.compare( "PA" ) == 0 ) {
        mat( 0, 0 ) = +1.0;
        mat( 2, 1 ) = -1.0;
        mat( 1, 2 ) = +1.0;
        mat( 0, 3 ) = -100.0;
        mat( 1, 3 ) = -100.0;
      }
      else {
        cerr <<
        "Error: <x>, <y>, and <z> can take values RL, LR, AP, PA, SI, IS, and must represent different orientations among them."
             << endl;
        exit( -1 );
      }
    }
    else if( y.compare( "IS" ) == 0 ) {
      if( z.compare( "AP" ) == 0 ) {
        mat( 0, 0 ) = +1.0;
        mat( 2, 1 ) = +1.0;
        mat( 1, 2 ) = -1.0;
        mat( 0, 3 ) = -100.0;
        mat( 2, 3 ) = -100.0;
      }
      else if( z.compare( "PA" ) == 0 ) {
        mat( 0, 0 ) = +1.0;
        mat( 2, 1 ) = +1.0;
        mat( 1, 2 ) = +1.0;
        mat( 0, 3 ) = -100.0;
        mat( 1, 3 ) = -100.0;
        mat( 2, 3 ) = -100.0;
      }
      else {
        cerr <<
        "Error: <x>, <y>, and <z> can take values RL, LR, AP, PA, SI, IS, and must represent different orientations among them."
             << endl;
        exit( -1 );
      }
    }
    else {
      cerr <<
      "Error: <x>, <y>, and <z> can take values RL, LR, AP, PA, SI, IS, and must represent different orientations among them."
           << endl;
      exit( -1 );
    }
  }
  else if( x.compare( "AP" ) == 0 ) {
    if( y.compare( "LR" ) == 0 ) {
      if( z.compare( "SI" ) == 0 ) {
        mat( 1, 0 ) = -1.0;
        mat( 0, 1 ) = +1.0;
        mat( 2, 2 ) = -1.0;
        mat( 0, 3 ) = -100.0;
      }
      else if( z.compare( "IS" ) == 0 ) {
        mat( 1, 0 ) = -1.0;
        mat( 0, 1 ) = +1.0;
        mat( 2, 2 ) = +1.0;
        mat( 0, 3 ) = -100.0;
        mat( 2, 3 ) = -100.0;
      }
      else {
        cerr <<
        "Error: <x>, <y>, and <z> can take values RL, LR, AP, PA, SI, IS, and must represent different orientations among them."
             << endl;
        exit( -1 );
      }
    }
    else if( y.compare( "RL" ) == 0 ) {
      if( z.compare( "SI" ) == 0 ) {
        mat( 1, 0 ) = -1.0;
        mat( 0, 1 ) = -1.0;
        mat( 2, 2 ) = -1.0;
      }
      else if( z.compare( "IS" ) == 0 ) {
        mat( 1, 0 ) = -1.0;
        mat( 0, 1 ) = -1.0;
        mat( 2, 2 ) = +1.0;
        mat( 2, 3 ) = -100.0;
      }
      else {
        cerr <<
        "Error: <x>, <y>, and <z> can take values RL, LR, AP, PA, SI, IS, and must represent different orientations among them."
             << endl;
        exit( -1 );
      }
    }
    else if( y.compare( "SI" ) == 0 ) {
      if( z.compare( "LR" ) == 0 ) {
        mat( 1, 0 ) = -1.0;
        mat( 2, 1 ) = -1.0;
        mat( 0, 2 ) = +1.0;
        mat( 0, 3 ) = -100.0;
      }
      else if( z.compare( "RL" ) == 0 ) {
        mat( 1, 0 ) = -1.0;
        mat( 2, 1 ) = -1.0;
        mat( 0, 2 ) = -1.0;
      }
      else {
        cerr <<
        "Error: <x>, <y>, and <z> can take values RL, LR, AP, PA, SI, IS, and must represent different orientations among them."
             << endl;
        exit( -1 );
      }
    }
    else if( y.compare( "IS" ) == 0 ) {
      if( z.compare( "LR" ) == 0 ) {
        mat( 1, 0 ) = -1.0;
        mat( 2, 1 ) = +1.0;
        mat( 0, 2 ) = +1.0;
        mat( 0, 3 ) = -100.0;
        mat( 2, 3 ) = -100.0;
      }
      else if( z.compare( "RL" ) == 0 ) {
        mat( 1, 0 ) = -1.0;
        mat( 2, 1 ) = +1.0;
        mat( 0, 2 ) = -1.0;
        mat( 2, 3 ) = -100.0;
      }
      else {
        cerr <<
        "Error: <x>, <y>, and <z> can take values RL, LR, AP, PA, SI, IS, and must represent different orientations among them."
             << endl;
        exit( -1 );
      }
    }
    else {
      cerr <<
      "Error: <x>, <y>, and <z> can take values RL, LR, AP, PA, SI, IS, and must represent different orientations among them."
           << endl;
      exit( -1 );
    }
  }
  else if( x.compare( "PA" ) == 0 ) {
    if( y.compare( "LR" ) == 0 ) {
      if( z.compare( "SI" ) == 0 ) {
        mat( 1, 0 ) = +1.0;
        mat( 0, 1 ) = +1.0;
        mat( 2, 2 ) = -1.0;
        mat( 0, 3 ) = -100.0;
        mat( 1, 3 ) = -100.0;
      }
      else if( z.compare( "IS" ) == 0 ) {
        mat( 1, 0 ) = +1.0;
        mat( 0, 1 ) = +1.0;
        mat( 2, 2 ) = +1.0;
        mat( 0, 3 ) = -100.0;
        mat( 1, 3 ) = -100.0;
        mat( 2, 3 ) = -100.0;
      }
      else {
        cerr <<
        "Error: <x>, <y>, and <z> can take values RL, LR, AP, PA, SI, IS, and must represent different orientations among them."
             << endl;
        exit( -1 );
      }
    }
    else if( y.compare( "RL" ) == 0 ) {
      if( z.compare( "SI" ) == 0 ) {
        mat( 1, 0 ) = +1.0;
        mat( 0, 1 ) = -1.0;
        mat( 2, 2 ) = -1.0;
        mat( 1, 3 ) = -100.0;
      }
      else if( z.compare( "IS" ) == 0 ) {
        mat( 1, 0 ) = +1.0;
        mat( 0, 1 ) = +1.0;
        mat( 2, 2 ) = +1.0;
        mat( 0, 3 ) = -100.0;
        mat( 1, 3 ) = -100.0;
        mat( 2, 3 ) = -100.0;
      }
      else {
        cerr <<
        "Error: <x>, <y>, and <z> can take values RL, LR, AP, PA, SI, IS, and must represent different orientations among them."
             << endl;
        exit( -1 );
      }
    }
    else if( y.compare( "SI" ) == 0 ) {
      if( z.compare( "LR" ) == 0 ) {
        mat( 1, 0 ) = +1.0;
        mat( 2, 1 ) = -1.0;
        mat( 0, 2 ) = +1.0;
        mat( 0, 3 ) = -100.0;
        mat( 1, 3 ) = -100.0;
      }
      else if( z.compare( "RL" ) == 0 ) {
        mat( 1, 0 ) = +1.0;
        mat( 2, 1 ) = -1.0;
        mat( 0, 2 ) = -1.0;
        mat( 1, 3 ) = -100.0;
      }
      else {
        cerr <<
        "Error: <x>, <y>, and <z> can take values RL, LR, AP, PA, SI, IS, and must represent different orientations among them."
             << endl;
        exit( -1 );
      }
    }
    else if( y.compare( "IS" ) == 0 ) {
      if( z.compare( "LR" ) == 0 ) {
        mat( 1, 0 ) = +1.0;
        mat( 2, 1 ) = +1.0;
        mat( 0, 2 ) = +1.0;
        mat( 0, 3 ) = -100.0;
        mat( 1, 3 ) = -100.0;
        mat( 2, 3 ) = -100.0;
      }
      else if( z.compare( "RL" ) == 0 ) {
        mat( 1, 0 ) = +1.0;
        mat( 2, 1 ) = +1.0;
        mat( 0, 2 ) = -1.0;
        mat( 1, 3 ) = -100.0;
        mat( 2, 3 ) = -100.0;
      }
      else {
        cerr <<
        "Error: <x>, <y>, and <z> can take values RL, LR, AP, PA, SI, IS, and must represent different orientations among them."
             << endl;
        exit( -1 );
      }
    }
    else {
      cerr <<
      "Error: <x>, <y>, and <z> can take values RL, LR, AP, PA, SI, IS, and must represent different orientations among them."
           << endl;
      exit( -1 );
    }
  }
  else if( x.compare( "SI" ) == 0 ) {
    if( y.compare( "LR" ) == 0 ) {
      if( z.compare( "AP" ) == 0 ) {
        mat( 2, 0 ) = -1.0;
        mat( 0, 1 ) = +1.0;
        mat( 1, 2 ) = -1.0;
        mat( 0, 3 ) = -100.0;
      }
      else if( z.compare( "PA" ) == 0 ) {
        mat( 2, 0 ) = -1.0;
        mat( 0, 1 ) = +1.0;
        mat( 1, 2 ) = +1.0;
        mat( 0, 3 ) = -100.0;
        mat( 1, 3 ) = -100.0;
      }
      else {
        cerr <<
        "Error: <x>, <y>, and <z> can take values RL, LR, AP, PA, SI, IS, and must represent different orientations among them."
             << endl;
        exit( -1 );
      }
    }
    else if( y.compare( "RL" ) == 0 ) {
      if( z.compare( "AP" ) == 0 ) {
        mat( 2, 0 ) = -1.0;
        mat( 0, 1 ) = -1.0;
        mat( 1, 2 ) = -1.0;
      }
      else if( z.compare( "PA" ) == 0 ) {
        mat( 2, 0 ) = -1.0;
        mat( 0, 1 ) = -1.0;
        mat( 1, 2 ) = +1.0;
        mat( 1, 3 ) = -100.0;
      }
      else {
        cerr <<
        "Error: <x>, <y>, and <z> can take values RL, LR, AP, PA, SI, IS, and must represent different orientations among them."
             << endl;
        exit( -1 );
      }
    }
    else if( y.compare( "AP" ) == 0 ) {
      if( z.compare( "LR" ) == 0 ) {
        mat( 2, 0 ) = -1.0;
        mat( 1, 1 ) = -1.0;
        mat( 0, 2 ) = +1.0;
        mat( 0, 3 ) = -100.0;
      }
      else if( z.compare( "RL" ) == 0 ) {
        mat( 2, 0 ) = -1.0;
        mat( 1, 1 ) = -1.0;
        mat( 0, 2 ) = -1.0;
      }
      else {
        cerr <<
        "Error: <x>, <y>, and <z> can take values RL, LR, AP, PA, SI, IS, and must represent different orientations among them."
             << endl;
        exit( -1 );
      }
    }
    else if( y.compare( "PA" ) == 0 ) {
      if( z.compare( "LR" ) == 0 ) {
        mat( 2, 0 ) = -1.0;
        mat( 1, 1 ) = +1.0;
        mat( 0, 2 ) = +1.0;
        mat( 0, 3 ) = -100.0;
        mat( 1, 3 ) = -100.0;
      }
      else if( z.compare( "RL" ) == 0 ) {
        mat( 2, 0 ) = -1.0;
        mat( 1, 1 ) = +1.0;
        mat( 0, 2 ) = -1.0;
        mat( 1, 3 ) = -100.0;
      }
      else {
        cerr <<
        "Error: <x>, <y>, and <z> can take values RL, LR, AP, PA, SI, IS, and must represent different orientations among them."
             << endl;
        exit( -1 );
      }
    }
    else {
      cerr <<
      "Error: <x>, <y>, and <z> can take values RL, LR, AP, PA, SI, IS, and must represent different orientations among them."
           << endl;
      exit( -1 );
    }
  }
  else if( x.compare( "IS" ) == 0 ) {
    if( y.compare( "LR" ) == 0 ) {
      if( z.compare( "AP" ) == 0 ) {
        mat( 2, 0 ) = +1.0;
        mat( 0, 1 ) = +1.0;
        mat( 1, 2 ) = -1.0;
        mat( 0, 3 ) = -100.0;
        mat( 2, 3 ) = -100.0;
      }
      else if( z.compare( "PA" ) == 0 ) {
        mat( 2, 0 ) = +1.0;
        mat( 0, 1 ) = +1.0;
        mat( 1, 2 ) = +1.0;
        mat( 0, 3 ) = -100.0;
        mat( 1, 3 ) = -100.0;
        mat( 2, 3 ) = -100.0;
      }
      else {
        cerr <<
        "Error: <x>, <y>, and <z> can take values RL, LR, AP, PA, SI, IS, and must represent different orientations among them."
             << endl;
        exit( -1 );
      }
    }
    else if( y.compare( "RL" ) == 0 ) {
      if( z.compare( "AP" ) == 0 ) {
        mat( 2, 0 ) = +1.0;
        mat( 0, 1 ) = -1.0;
        mat( 1, 2 ) = -1.0;
        mat( 2, 3 ) = -100.0;
      }
      else if( z.compare( "PA" ) == 0 ) {
        mat( 2, 0 ) = +1.0;
        mat( 0, 1 ) = -1.0;
        mat( 1, 2 ) = +1.0;
        mat( 1, 3 ) = -100.0;
        mat( 2, 3 ) = -100.0;
      }
      else {
        cerr <<
        "Error: <x>, <y>, and <z> can take values RL, LR, AP, PA, SI, IS, and must represent different orientations among them."
             << endl;
        exit( -1 );
      }
    }
    else if( y.compare( "AP" ) == 0 ) {
      if( z.compare( "LR" ) == 0 ) {
        mat( 2, 0 ) = +1.0;
        mat( 1, 1 ) = -1.0;
        mat( 0, 2 ) = +1.0;
        mat( 0, 3 ) = -100.0;
        mat( 2, 3 ) = -100.0;
      }
      else if( z.compare( "RL" ) == 0 ) {
        mat( 2, 0 ) = +1.0;
        mat( 1, 1 ) = -1.0;
        mat( 0, 2 ) = -1.0;
        mat( 2, 3 ) = -100.0;
      }
      else {
        cerr <<
        "Error: <x>, <y>, and <z> can take values RL, LR, AP, PA, SI, IS, and must represent different orientations among them."
             << endl;
        exit( -1 );
      }
    }
    else if( y.compare( "PA" ) == 0 ) {
      if( z.compare( "LR" ) == 0 ) {
        mat( 2, 0 ) = +1.0;
        mat( 1, 1 ) = +1.0;
        mat( 0, 2 ) = +1.0;
        mat( 0, 3 ) = -100.0;
        mat( 1, 3 ) = -100.0;
        mat( 2, 3 ) = -100.0;
      }
      else if( z.compare( "RL" ) == 0 ) {
        mat( 2, 0 ) = +1.0;
        mat( 1, 1 ) = +1.0;
        mat( 0, 2 ) = -1.0;
        mat( 1, 3 ) = -100.0;
        mat( 2, 3 ) = -100.0;
      }
      else {
        cerr <<
        "Error: <x>, <y>, and <z> can take values RL, LR, AP, PA, SI, IS, and must represent different orientations among them."
             << endl;
        exit( -1 );
      }
    }
    else {
      cerr <<
      "Error: <x>, <y>, and <z> can take values RL, LR, AP, PA, SI, IS, and must represent different orientations among them."
           << endl;
      exit( -1 );
    }
  }
  else {
    cerr <<
    "Error: <x>, <y>, and <z> can take values RL, LR, AP, PA, SI, IS, and must represent different orientations among them."
         << endl;
    exit( -1 );
  }
  NiftiHeader hdr( mri );
  hdr.QuaternMatrix( mat );
  hdr.AffineMatrix( mat );

  WriteNifti( mri, argv[ 5 ], hdr );

  return( 0 );
}
