/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Sep/21 */
/* Content: Test file. */
/* Description: Test with Brain and OPFClustering classes. */

#include "BrainTissueSegmentation.hpp"
#include "FileImage.hpp"
#include "Image.hpp"
#include "MRIModality.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( ( argc < 5 ) || ( argc == 6 ) || ( argc == 8 ) ) {
    cout << "Usage: " << argv[ 0 ] <<
      " <input image> <brain mask> <modality> <output image> [<csf_min_scl>, <csf_max_scl>] " <<
      "[<gm_min_scl>, <gm_max_scl>]" << endl;
    cout << "\tmodality: T1, T2, or PD." << endl;
    cout << "\tcsf_min_scl, csf_max_cls: Default values = 0.4, 0.5. " << 
      "Suggested values: 0.1 <= csf_min_scl <= csf_max_scl <= 0.9." << endl;
    cout << "\tgm_min_scl, gm_max_scl: Default values = 0.4, 0.5. " <<
      "Suggested values: 0.1 <= gm_min_scl <= gm_max_scl <= 0.9." << endl;
    exit( 1 );
  }
  /* Fixed number sequence */
  Common::Randomize( false );

  /* Validating parameters. */
  string modality_str( argv[ 3 ] );
  MRIModality modality = MRIModality::T1;
  if( modality_str.compare( "T2" ) == 0 ) {
    modality = MRIModality::T2;
  }
  else if( modality_str.compare( "PD" ) == 0 ) {
    modality = MRIModality::PD;
  }
  else if( modality_str.compare( "T1" ) != 0 ) {
    cout << "Error: Modality must be T1, T2, or PD. Given \"" << modality_str << "\"." << endl;
    exit( 1 );
  }
  float csf_min_scl = 0.2f;
  float csf_max_scl = 0.3f;
  if( argc > 5 ) {
    csf_min_scl = atof( argv[ 5 ] );
    csf_max_scl = atof( argv[ 6 ] );
    if( csf_min_scl > csf_max_scl ) {
      cout << "Error: csf_min_scl can not be greater than csf_max_scl. Given: csf_min_scl = " << csf_min_scl <<
      ", csf_max_scl = " << csf_max_scl << "." << endl;
      exit( 1 );
    }
  }
  float gm_min_scl = 0.1f;
  float gm_max_scl = 0.2f;
  if( argc > 7 ) {
    gm_min_scl = atof( argv[ 7 ] );
    gm_max_scl = atof( argv[ 8 ] );
    if( gm_min_scl > gm_max_scl ) {
      cout << "Error: gm_min_scl can not be greater than gm_max_scl. Given: gm_min_scl = " << gm_min_scl <<
      ", gm_max_scl = " << gm_max_scl << "." << endl;
      exit( 1 );
    }
  }
  /* Reading input data */
  Image< int > scn( Read< int >( argv[ 1 ] ) );
  Image< int > mask( Read< int >( argv[ 2 ] ) );

  /* Running tissue segmentation */
  Image< int > tissues( Brain::TissueSegmentation( scn, mask, modality, csf_min_scl, csf_max_scl, gm_min_scl, 
                                                   gm_max_scl ) );

  /* Writing result */
  Write( tissues, argv[ 4 ], argv[ 1 ] );
  return( 0 );
}
