// REMOVED!!! CHECK CHANGES IN TWO COMMITS.

/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2013/Aug/27 */
/* Content: Brain and Tissue classes instantiation. */
/* Description: Classes that contains methods to be used with brain images. */
/* Future add-on's: many. */

#ifndef BIALBRAIN_C
#define BIALBRAIN_C

#include "Brain.hpp"


#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_Brain )
#define BIAL_EXPLICIT_Brain
#endif

#if defined ( BIAL_EXPLICIT_Brain ) || ( BIAL_IMPLICIT_BIN )

#include "AdjacencyRound.hpp"
#include "AdjacencyIterator.hpp"
#include "Binary.hpp"
#include "DataBasics.hpp"
#include "Geometrics.hpp"
#include "Histogram.hpp"
#include "Image.hpp"
#include "Integral.hpp"
#include "Intensity.hpp"
#include "MedianFeature.hpp"
#include "MorphologyErosion.hpp"
#include "NiftiHeader.hpp"
#include "OPFHierarchicalClustering.hpp"
#include "OPFSpectralClustering.hpp"
#include "Signal.hpp"
#include "SignalHysteresis.hpp"
#include "SortingSort.hpp"
#include "Transform.hpp"

namespace Bial {

  /* Brain class --------------------------------------------------------------------------------------------------- **/

  template< class D >
  void Brain::IntensityTissueLabeling( const Image< D > &image, const Image< D > &mask, Image< int > &label,
                                       D intensity ) {
    try {
      if( image.Dims( ) != mask.Dims( ) ) {
        std::string msg( BIAL_ERROR( "Image and mask dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      if( image.Dims( ) != label.Dims( ) ) {
        std::string msg( BIAL_ERROR( "Image and label dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      size_t nlabels = static_cast< size_t >( Intensity::Maximum( Image< D >( label ), mask ) ) + 1;
      Vector< double > mean( nlabels, 0.0 );
      Vector< size_t > size( nlabels, 0 );

      COMMENT( "Computing mean brightness and size of every label region.", 0 );
      for( size_t pxl = 0; pxl < mask.size( ); ++pxl ) {
        if( mask[ pxl ] != 0 ) {
          mean( label[ pxl ] ) += image[ pxl ];
          ++size( label[ pxl ] );
        }
      }
      for( size_t lbl = 0; lbl < nlabels; ++lbl ) {
        if( size( lbl ) != 0 ) {
          mean( lbl ) /= size( lbl );
        }
      }
      COMMENT( "Sorting mean vector.", 0 );
      Vector< size_t > order = Sorting::Sort( mean );

      COMMENT( "Computing position.", 0 );
      Vector< size_t > position( order );
      for( size_t idx = 0; idx < mean.size( ); ++idx ) {
        position( order( idx ) ) = idx;
      }
      COMMENT( "Locating first high intensity cluster.", 0 );
      size_t low_limit = 1;
      COMMENT( "Finding first non-zero size cluster.", 0 );
      while( ( low_limit < nlabels - 1 ) && ( mean( low_limit ) < intensity ) ) {
        ++low_limit;
      }
      COMMENT( "Labeling samples into low and high intensity classes.", 0 );
      for( size_t pxl = 0; pxl < image.size( ); ++pxl ) {
        if( mask[ pxl ] != 0 ) {
          if( position( label[ pxl ] ) < low_limit ) {
            label[ pxl ] = 0;
          }
          else {
            label[ pxl ] = 1;
          }
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

  template< class D >
  Image< int > Brain::TissueSegmentation( const Image< D > &img, const Image< D > &msk, D tissue_intensity,
                                          float scl_min, float scl_max, bool hierarchical ) {
    try {
      if( img.Dims( ) != msk.Dims( ) ) {
        std::string msg( BIAL_ERROR( "Image and mask dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      if( scl_min > scl_max ) {
        std::string msg( BIAL_ERROR( "scl_min greater than scl_max." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Segmenting tissues.", 0 );
      Feature< int > feat( MedianFeature( img, msk, AdjacencyType::Spheric( 1.5 ), 0.34 ) );
      if( hierarchical ) {
        COMMENT( "Hierarchical clustering.", 0 );
        OPF::HierarchicalClustering( feat, scl_min, scl_max );
      }
      else {
        COMMENT( "Spectral clustering.", 0 );
        OPF::SpectralClustering( feat, scl_min, scl_max );
      }
      COMMENT( "Labeling clusters into tissues.", 0 );
      Image< int > label( img );
      label.Set( 0 );
      for( size_t elm = 0; elm < feat.Elements( ); ++elm ) {
        label[ feat.Index( elm ) ] = feat.Label( elm );
      }
      Brain::IntensityTissueLabeling( img, msk, label, tissue_intensity );

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

  template< class D >
  Image< int > Brain::TissueSegmentation( const Image< D > &img, const Image< D > &msk, MRIModality modality,
                                          float csf_scl_min, float csf_scl_max, float gm_scl_min,
                                          float gm_scl_max, bool hierarchical ) {
    try {
      if( img.Dims( ) != msk.Dims( ) ) {
        std::string msg( BIAL_ERROR( "Image and mask dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      if( ( modality != MRIModality::T1 ) &&
          ( modality != MRIModality::T2 ) &&
          ( modality != MRIModality::PD ) ) {
        std::string msg( BIAL_ERROR( "Must select an MRI_T1, MRI_T2, or MRI_PD modalities." ) );
        throw( std::logic_error( msg ) );
      }
      if( csf_scl_min > csf_scl_max ) {
        std::string msg( BIAL_ERROR( "csf_scl_min greater than csf_scl_max." ) );
        throw( std::logic_error( msg ) );
      }
      if( gm_scl_min > gm_scl_max ) {
        std::string msg( BIAL_ERROR( "gm_scl_min greater than gm_scl_max." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Getting tissues intensities.", 0 );
      Image< D > brain = img * msk;
      BrainTissues tissues( BrainTissueIntensities( brain ) );

      COMMENT( "Segmenting CSF from GM+WM.", 0 );
      D intensity = tissues.first_valley;
      if( modality != MRIModality::T1 ) {
        intensity = tissues.second_valley;
      }
      Image< D > wmgm = Brain::TissueSegmentation( img, msk, intensity, csf_scl_min, csf_scl_max, hierarchical );
      if( modality != MRIModality::T1 ) {
        Binary::Complement( wmgm, msk );
      }
      COMMENT( "Segmenting GM from WM.", 0 );
      intensity = tissues.second_valley;
      if( modality != MRIModality::T1 ) {
        intensity = tissues.first_valley;
      }
      Image< int > tissue = Brain::TissueSegmentation( img, wmgm, intensity, gm_scl_min, gm_scl_max, hierarchical );
      if( modality == MRIModality::T1 ) {
        for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
          if( msk[ pxl ] != 0 ) {
            tissue[ pxl ] += wmgm[ pxl ] + 1;
          }
        }
      }
      else { /* if( ( modality == MRIModality::T2 ) || ( modality == MRIModality::PD ) ) { */
        for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
          if( msk[ pxl ] != 0 ) {
            tissue[ pxl ] = ( tissue[ pxl ] + 1 ) % 2 + wmgm[ pxl ] + 1;
          }
        }
      }
      return( tissue );
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
  Image< D > Brain::RemoveOutliers( const Image< D > &img, const Image< D > &msk, float compression,
                                    MRIModality modality ) {
    try {
      COMMENT( "Brain histogram.", 0 );
      Signal hst = SignalType::Histogram( img * msk );
      hst[ 0 ] = 0.0;
      Image< D > res( msk );

      COMMENT( "Creating mask free from outiliers.", 0 );
      if( modality == MRIModality::T1 ) {
        COMMENT( "Detecting outliers threshold by hysteresis.", 0 );
        size_t kernel_size = 5 + std::round( hst.size( ) / 1000.0 );
        D hgh_intensity = SignalOp::HighHysteresis( hst, kernel_size, 0.1, 0.2 );
        COMMENT( "hgh_intensity: " << hgh_intensity << ", max: " << Maximum( img * msk ), 2 );
        for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
          if( img[ pxl ] > hgh_intensity ) {
            res[ pxl ] = 0;
          }
        }
      }
      else {
        COMMENT( "Detecting outliers threshold by hysteresis.", 0 );
        size_t kernel_size = 5 + std::round( hst.size( ) / 1000.0 );
        D low_intensity = SignalOp::LowHysteresis( hst, kernel_size, 0.1, 0.2 );
        for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
          if( img[ pxl ] < low_intensity ) {
            res[ pxl ] = 0;
          }
        }
      }
      COMMENT( "Eroding to avoid outliers neighbors.", 0 );
      res = Morphology::ErodeBin( res, AdjacencyType::HyperSpheric( 5.0 / compression, img.Dims( ) ) );
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

#ifdef BIAL_EXPLICIT_Brain


  template Vector< size_t > Brain::Locate( const Image< int > &img, const std::string &orientation, 
                                           Vector< Box > &best_rectangles, Image< float > &similarity );
  template Image< int > Brain::Segmentation( const Image< int > &img, const std::string &orientation );
  template size_t Brain::UpperBrainSlice( const Image< int > &img, const std::string &orientation );
  template Vector< size_t > Brain::WeightedCOG( const Image< int > &img );
  template Vector< size_t > Brain::WeightedCOG2( const Image< int > &img );
  template Vector< size_t > Brain::WeightedCOG3( const Image< int > &img, const std::string &orientation );
  template Vector< size_t > Brain::WeightedCOG4( const Image< int > &img, const std::string &orientation, 
                                                 MRIModality modality );
  template void Brain::IntensityTissueLabeling( const Image< int > &image, const Image< int > &mask, 
                                                Image< int > &label, int intensity );
  template Image< int > Brain::TissueSegmentation( const Image< int > &img, const Image< int > &msk, 
                                                   int tissue_intensity, float scl_min, float scl_max, 
                                                   bool hierarchical );
  template Image< int > Brain::TissueSegmentation( const Image< int > &img, const Image< int > &msk, 
                                                   MRIModality modality, float csf_scl_min, 
                                                   float csf_scl_max, float gm_scl_min, float gm_scl_max, 
                                                   bool hierarchical );
  template Image< int > Brain::RemoveOutliers( const Image< int > &img, const Image< int > &msk, float compression,
                                               MRIModality modality );

  template Vector< size_t > Brain::Locate( const Image< llint > &img, const std::string &orientation, 
                                           Vector< Box > &best_rectangles, Image< float > &similarity );
  template Image< llint > Brain::Segmentation( const Image< llint > &img, const std::string &orientation );
  template size_t Brain::UpperBrainSlice( const Image< llint > &img, const std::string &orientation );
  template Vector< size_t > Brain::WeightedCOG( const Image< llint > &img );
  template Vector< size_t > Brain::WeightedCOG2( const Image< llint > &img );
  template Vector< size_t > Brain::WeightedCOG3( const Image< llint > &img, const std::string &orientation );
  template Vector< size_t > Brain::WeightedCOG4( const Image< llint > &img, const std::string &orientation, 
                                                 MRIModality modality );
  template void Brain::IntensityTissueLabeling( const Image< llint > &image, const Image< llint > &mask, 
                                                Image< int > &label, llint intensity );
  template Image< int > Brain::TissueSegmentation( const Image< llint > &img, const Image< llint > &msk, 
                                                   llint tissue_intensity, float scl_min, float scl_max, 
                                                   bool hierarchical );
  template Image< int > Brain::TissueSegmentation( const Image< llint > &img, const Image< llint > &msk, 
                                                   MRIModality modality, float csf_scl_min, 
                                                   float csf_scl_max, float gm_scl_min, float gm_scl_max, 
                                                   bool hierarchical );
  template Image< llint > Brain::RemoveOutliers( const Image< llint > &img, const Image< llint > &msk, 
                                                 float compression, MRIModality modality );

  template Vector< size_t > Brain::Locate( const Image< float > &img, const std::string &orientation, 
                                           Vector< Box > &best_rectangles, Image< float > &similarity );
  template Image< float > Brain::Segmentation( const Image< float > &img, const std::string &orientation );
  template size_t Brain::UpperBrainSlice( const Image< float > &img, const std::string &orientation );
  template Vector< size_t > Brain::WeightedCOG( const Image< float > &img );
  template Vector< size_t > Brain::WeightedCOG2( const Image< float > &img );
  template Vector< size_t > Brain::WeightedCOG3( const Image< float > &img, const std::string &orientation );
  template Vector< size_t > Brain::WeightedCOG4( const Image< float > &img, const std::string &orientation, 
                                                 MRIModality modality );
  template void Brain::IntensityTissueLabeling( const Image< float > &image, const Image< float > &mask, 
                                                Image< int > &label, float intensity );
  template Image< int > Brain::TissueSegmentation( const Image< float > &img, const Image< float > &msk, 
                                                   float tissue_intensity, float scl_min, float scl_max, 
                                                   bool hierarchical );
  template Image< int > Brain::TissueSegmentation( const Image< float > &img, const Image< float > &msk, 
                                                   MRIModality modality, float csf_scl_min, 
                                                   float csf_scl_max, float gm_scl_min, float gm_scl_max, 
                                                   bool hierarchical );
  template Image< float > Brain::RemoveOutliers( const Image< float > &img, const Image< float > &msk, 
                                                 float compression, MRIModality modality );
  
  template Vector< size_t > Brain::Locate( const Image< double > &img, const std::string &orientation, 
                                           Vector< Box > &best_rectangles, Image< float > &similarity );
  template Image< double > Brain::Segmentation( const Image< double > &img, const std::string &orientation );
  template size_t Brain::UpperBrainSlice( const Image< double > &img, const std::string &orientation );
  template Vector< size_t > Brain::WeightedCOG( const Image< double > &img );
  template Vector< size_t > Brain::WeightedCOG2( const Image< double > &img );
  template Vector< size_t > Brain::WeightedCOG3( const Image< double > &img, const std::string &orientation );
  template Vector< size_t > Brain::WeightedCOG4( const Image< double > &img, const std::string &orientation, 
                                                 MRIModality modality );
  template void Brain::IntensityTissueLabeling( const Image< double > &image, const Image< double > &mask, 
                                                Image< int > &label, double intensity );
  template Image< int > Brain::TissueSegmentation( const Image< double > &img, const Image< double > &msk, 
                                                   double tissue_intensity, float scl_min, float scl_max, 
                                                   bool hierarchical );
  template Image< int > Brain::TissueSegmentation( const Image< double > &img, const Image< double > &msk, 
                                                   MRIModality modality, float csf_scl_min, 
                                                   float csf_scl_max, float gm_scl_min, float gm_scl_max, 
                                                   bool hierarchical );
  template Image< double > Brain::RemoveOutliers( const Image< double > &img, const Image< double > &msk, 
                                                  float compression, MRIModality modality );

#endif

}

#endif

#endif
