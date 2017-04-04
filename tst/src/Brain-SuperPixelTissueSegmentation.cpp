/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2015/Sep/15 */
/* Content: Test file. */
/* Description: Test with superpixels. */

#include "FileImage.hpp"
#include "Image.hpp"
#include "Matrix.hpp"
#include "SegmentationIntensityThreshold.hpp"
#include "StatisticsDice.hpp"
#include "Superpixel.hpp"

using namespace Bial;
using namespace std;

template< class D >
Feature< D > SuperPixelMedian( const Image< D > &img, const Image< int > &superpixel, const Image< D > &msk, 
                               float threshold ) {
  COMMENT( "Computing mean intensity of superpixels.", 0 );
  COMMENT( "Computing superpixels adjacency.", 0 );
  COMMENT( "Computing superpixel feature.", 0 );
  COMMENT( "Setting pixel features... or not... maybe work with superpixels from here.", 0 );
  COMMENT( "", 0 );
  COMMENT( "", 0 );
}

template< class D >
Image< D > BrainSuperPixelTissueSegmentation( const Image< D > &img, const Image< D > &msk, size_t sup_pixel_size, 
                                              MRIModality modality, double compactness, float csf_min_scl, 
                                              float csf_max_scl, float gm_min_scl, float gm_max_scl ) {
  COMMENT( "Running slic.", 0 );
  Image< int > superpixel( Superpixel::SuperVoxel( img, sup_pixel_size, compactness ) );
  COMMENT( "Computing tissue thresholds.", 0 );
  Image< D > brain( img * msk );
  BrainIntensities tissues( brain );
  COMMENT( "Computing features.", 0 );

  COMMENT( "Clustering CSF and GM+WM.", 0 );
  Feature< D > csf_feat( SuperPixelMedian( img, superpixel, msk, 0.5 ) );
  OPF::SpectralClustering( csf_feat, csf_min_scl, csf_max_scl );
  COMMENT( "Labeling clusters into tissues.", 0 );
  Image< D > wmgm( img.Dim( ) );
  for( size_t elm = 0; elm < csf_feat.Elements( ); ++elm )
    wmgm[ csf_feat.Index( elm ) ] = csf_feat.Label( elm );
  if( modality == MRIModality::T1 )
    TissueLabeling( img, msk, wmgm, tissues.first_valley );
  else {
    TissueLabeling( img, msk, wmgm, tissues.second_valley );
    Binary::Complement( wmgm, msk );
  }
  
  COMMENT( "Clustering GM and WM.", 0 );
  Feature< D > gm_feat( SuperPixelMedian( img, superpixel, msk, 0.5 ) );
  OPF::SpectralClustering( gm_feat, gm_min_scl, gm_max_scl );
  COMMENT( "Labeling clusters into tissues.", 0 );
  Image< D > wm( img.Dim( ) );
  for( size_t elm = 0; elm < gm_feat.Elements( ); ++elm )
    wm[ gm_feat.Index( elm ) ] = gm_feat.Label( elm );
  if( modality == MRIModality::T1 )
    TissueLabeling( img, wmgm, wm, tissues.second_valley );
  else {
    TissueLabeling( img, wmgm, wm, tissues.first_valley );
    Binary::Complement( wm, wmgm );
  }
  
  COMMENT( "Computing final tissue label.", 0 );
  return( tissue( wm + wmgm + brain ) );
}
  



int main( int argc, char *argv[ ] ) {
  
  if( argc < 7 ){
    cout << "usage: " << argv[ 0 ] << " <brain image> <brain mask> <resulting image> <modality> <supervoxel_size> "
         << "<compactness> [<csf_min_scl>, <csf_max_scl>] [<gm_min_scl>, <gm_max_scl>]" << endl;
    cout << "\tmodality: T1, T2, or PD." << endl;
    cout << "\t <supervoxel_size>... The desired size of supervoxel." << endl;
    cout << "\t <compactness>....... The relative importance between color similarity and spatial proximity." << endl;
    cout << "\t                      It can be in the range [1, 100]." << endl;
    cout << "\tcsf_min_scl, csf_max_cls: Default values = 0.4, 0.5. " << 
      "Suggested values: 0.1 <= csf_min_scl <= csf_max_scl <= 0.9." << endl;
    cout << "\tgm_min_scl, gm_max_scl: Default values = 0.4, 0.5. " <<
      "Suggested values: 0.1 <= gm_min_scl <= gm_max_scl <= 0.9." << endl;
    exit( 0 );
  }

  COMMENT( "Reading parameters.", 0 );
  Image< int > img( Read< int >( argv[ 1 ] ) );
  Image< int > msk( Read< int >( argv[ 2 ] ) );


  COMMENT( "Validating modality.", 0 );
  string modality_str( argv[ 4 ] );
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

  COMMENT( "Getting superpixel parameters.", 0 );
  size_t sup_pixel_size = atoi( argv[ 5 ] );
  double compactness = atof( argv[ 6 ] );

  COMMENT( "Getting threshold parameters.", 0 );
  float csf_min_scl = 0.2f;
  float csf_max_scl = 0.3f;
  if( argc > 7 ) {
    csf_min_scl = atof( argv[ 7 ] );
    csf_max_scl = atof( argv[ 8 ] );
    if( csf_min_scl > csf_max_scl ) {
      cout << "Error: csf_min_scl can not be greater than csf_max_scl. Given: csf_min_scl = " << csf_min_scl <<
      ", csf_max_scl = " << csf_max_scl << "." << endl;
      exit( 1 );
    }
  }
  float gm_min_scl = 0.1f;
  float gm_max_scl = 0.2f;
  if( argc > 9 ) {
    gm_min_scl = atof( argv[ 9 ] );
    gm_max_scl = atof( argv[ 10 ] );
    if( gm_min_scl > gm_max_scl ) {
      cout << "Error: gm_min_scl can not be greater than gm_max_scl. Given: gm_min_scl = " << gm_min_scl <<
      ", gm_max_scl = " << gm_max_scl << "." << endl;
      exit( 1 );
    }
  }

  Image< int > tissue( BrainSuperPixelTissueSegmentation( img, msk, sup_pixel_size, modality, compactness, csf_min_scl,
                                                          csf_max_scl, gm_min_scl, gm_max_scl ) );
  
  COMMENT( "Writing result.", 0 );
  Write( tissue, argv[ 3 ], argv[ 1 ] );

  return( 0 );
}
