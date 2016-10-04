/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2013/Aug/27 
 * @brief Functions to segment brain tissues into GM/WM/CSF.
 */

#ifndef BIALBRAINTISSUESEGMENTATION_C
#define BIALBRAINTISSUESEGMENTATION_C

#include "BrainTissueSegmentation.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_BrainTissueSegmentation )
#define BIAL_EXPLICIT_BrainTissueSegmentation
#endif

#if defined ( BIAL_EXPLICIT_BrainTissueSegmentation ) || ( BIAL_IMPLICIT_BIN )

#include "AdjacencyRound.hpp"
#include "BrainIntensities.hpp"
#include "BinaryComplement.hpp"
#include "Feature.hpp"
#include "Image.hpp"
#include "IntensityGlobals.hpp"
#include "MedianFeature.hpp"
#include "OPFHierarchicalClustering.hpp"
#include "OPFSpectralClustering.hpp"
#include "SortingSort.hpp"

namespace Bial {

  namespace Brain {

    /* Brain class --------------------------------------------------------------------------------------------------- **/

    template< class D >
    void TissueLabeling( const Image< D > &image, const Image< D > &mask, Image< int > &label, D intensity ) {
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
    Image< int > TissueSegmentation( const Image< D > &img, const Image< D > &msk, D tissue_intensity,
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
        Feature< D > feat( MedianFeature( img, msk, AdjacencyType::Spheric( 1.5 ), 0.34 ) );
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
        TissueLabeling( img, msk, label, tissue_intensity );
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
    Image< int > TissueSegmentation( const Image< D > &img, const Image< D > &msk, MRIModality modality,
                                          float csf_scl_min, float csf_scl_max, float gm_scl_min, float gm_scl_max,
                                          bool hierarchical ) {
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
        Image< D > brain( img );
        for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
          if( msk[ pxl ] == 0 )
            brain[ pxl ] = 0;
        }
        BrainIntensities tissues( brain );
        COMMENT( "Segmenting CSF from GM+WM.", 0 );
        D intensity = tissues.first_valley;
        if( modality != MRIModality::T1 ) {
          intensity = tissues.second_valley;
        }
        Image< D > wmgm = TissueSegmentation( img, msk, intensity, csf_scl_min, csf_scl_max, hierarchical );
        if( modality != MRIModality::T1 ) {
          Binary::Complement( wmgm, msk );
        }
        COMMENT( "Segmenting GM from WM.", 0 );
        intensity = tissues.second_valley;
        if( modality != MRIModality::T1 ) {
          intensity = tissues.first_valley;
        }
        Image< int > tissue = TissueSegmentation( img, wmgm, intensity, gm_scl_min, gm_scl_max, hierarchical );
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

#ifdef BIAL_EXPLICIT_BrainTissueSegmentation

    template void TissueLabeling( const Image< int > &image, const Image< int > &mask, Image< int > &label, 
                                  int intensity );
    template Image< int > TissueSegmentation( const Image< int > &img, const Image< int > &msk, int tissue_intensity,
                                              float scl_min, float scl_max,  bool hierarchical );
    template Image< int > TissueSegmentation( const Image< int > &img, const Image< int > &msk, MRIModality modality,
                                              float csf_scl_min,  float csf_scl_max, float gm_scl_min,
                                              float gm_scl_max, bool hierarchical );

    template void TissueLabeling( const Image< llint > &image, const Image< llint > &mask, Image< int > &label, 
                                  llint intensity );
    template Image< int > TissueSegmentation( const Image< llint > &img, const Image< llint > &msk,
                                              llint tissue_intensity, float scl_min, float scl_max, 
                                              bool hierarchical );
    template Image< int > TissueSegmentation( const Image< llint > &img, const Image< llint > &msk, 
                                              MRIModality modality, float csf_scl_min,  float csf_scl_max,
                                              float gm_scl_min, float gm_scl_max, bool hierarchical );

    template void TissueLabeling( const Image< float > &image, const Image< float > &mask, Image< int > &label,
                                  float intensity );
    template Image< int > TissueSegmentation( const Image< float > &img, const Image< float > &msk,
                                              float tissue_intensity, float scl_min, float scl_max,
                                              bool hierarchical );
    template Image< int > TissueSegmentation( const Image< float > &img, const Image< float > &msk,
                                              MRIModality modality, float csf_scl_min, float csf_scl_max,
                                              float gm_scl_min, float gm_scl_max,  bool hierarchical );
  
    template void TissueLabeling( const Image< double > &image, const Image< double > &mask, Image< int > &label,
                                  double intensity );
    template Image< int > TissueSegmentation( const Image< double > &img, const Image< double > &msk, 
                                              double tissue_intensity, float scl_min, float scl_max, 
                                              bool hierarchical );
    template Image< int > TissueSegmentation( const Image< double > &img, const Image< double > &msk, 
                                              MRIModality modality, float csf_scl_min, float csf_scl_max,
                                              float gm_scl_min, float gm_scl_max, bool hierarchical );

#endif

  }

}

#endif

#endif
