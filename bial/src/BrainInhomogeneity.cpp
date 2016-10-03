/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Aug/27 
 * @brief Functions to estimate and correct inhomogeneity. 
 */

#ifndef BIALBRAININHOMOGENEITY_C
#define BIALBRAININHOMOGENEITY_C

#include "BrainInhomogeneity.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_BrainInhomogeneity )
#define BIAL_EXPLICIT_BrainInhomogeneity
#endif

#if defined ( BIAL_EXPLICIT_BrainInhomogeneity ) || ( BIAL_IMPLICIT_BIN )

#include "AdjacencyRound.hpp"
#include "AdjacencyIterator.hpp"
#include "FilteringMean.hpp"
#include "GeometricsScale.hpp"
#include "Histogram.hpp"
#include "Image.hpp"
#include "ImageFrame.hpp"
#include "ImageResize.hpp"
#include "ImageROI.hpp"
#include "MorphologyDilation.hpp"
#include "MorphologyErosion.hpp"
#include "SegmentationBorder.hpp"
#include "Signal.hpp"
#include "SignalHysteresis.hpp"
#include "TransformEuclDistInv.hpp"

namespace Bial {

  namespace Brain {

    template< class D >
    Image< D > RemoveIntensityOutliers( const Image< D > &img, const Image< D > &msk, float compression,
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
          COMMENT( "hgh_intensity: " << hgh_intensity << ", max: " << ( img * msk ).Maximum( ), 2 );
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

    template< class D >
    Image< D > BiasSurfaceEstimation( const Image< D > &img, const Image< D > &msk, const Image< D > &cln_msk,
                                      float radius, MRIModality modality ) {
      try {
        COMMENT( "Preparing images for inhomogeity surface computing.", 0 );
        Adjacency small_adj = AdjacencyType::HyperSpheric( 1.0, img.Dims( ) );
        Image< D > border = Segmentation::BorderPixels( msk, small_adj );
        Image< D > edt = Transform::InverseEDT( border, msk, radius );
        COMMENT( "Adjacency relation computing and result scene creating.", 0 );
        Vector< float > input_radius( img.Dims( ), radius );
        Vector< float > large_radius( img.Dims( ), 1.1 );
        for( size_t dms = 0; dms < img.Dims( ); ++dms ) {
          input_radius( dms ) /= img.PixelSize( dms );
          large_radius( dms ) /= img.PixelSize( dms );
        }
        Adjacency input_adj = AdjacencyType::HyperEllipsoid( input_radius );
        Adjacency large_adj = AdjacencyType::HyperEllipsoid( large_radius );
        Image< D > res( msk );
        res.Set( 0.0 );
        COMMENT( "Creating histogram of potential reference pixel region.", 0 );
        size_t histo_size = ( img * cln_msk ).Maximum( ) + 1;
        Signal histo( histo_size );
        COMMENT( "Computing the bias estimation scene.", 0 );
        for( size_t pxl = 0; pxl < msk.size( ); ++pxl ) {
          if( msk[ pxl ] != 0 ) {
            size_t total_voxels = 0;
            size_t root = edt[ pxl ];
            COMMENT( "Looking for the best value in each adjacency.", 4 );
            for( AdjacencyIterator itr = begin( input_adj,msk, root ); *itr != msk.size( ); ++itr ) {
              size_t adj_pxl = *itr;
              if( cln_msk[ adj_pxl ] != 0 ) {
                ++total_voxels;
                ++histo[ img[ adj_pxl ] ];
              }
            }
            COMMENT( "Getting histogram clipped value to avoid outliers.", 4 );
            if( total_voxels == 0 ) {
              res[ pxl ] = img[ pxl ];
            }
            else {
              size_t kernel_size = 5 + std::round( histo.size( ) / 1000.0 );
              if( modality == MRIModality::T1 ) {
                res[ pxl ] = SignalOp::HighHysteresis( histo, kernel_size, 0.2, 0.3 );
              }
              else {
                res[ pxl ] = SignalOp::LowHysteresis( histo, kernel_size, 0.2, 0.3 );
              }
            }
            for( size_t bin = 0; bin < histo_size; ++bin ) {
              histo[ bin ] = 0;
            }
          }
        }
        COMMENT( "Computing the mean to smooth the bias field.", 0 );
        res = Filtering::Mean( res, msk, 1.1 );
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

    template< class D >
    Image< D > BiasSurfaceRemoval( const Image< D > &img, const Image< D > &msk, const Image< D > &bias,
                                   MRIModality modality ) {
      try {
        Image< D > res( msk );
        if( modality == MRIModality::T1 ) {
          D max_img = ( img * msk ).Maximum( );
          D max_bias = ( bias * msk ).Maximum( );
          for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
            if( msk[ pxl ] != 0 ) {
              res[ pxl ] = std::min( max_img, static_cast< D >( img[ pxl ] * static_cast< float >
                                                                ( ( max_bias + 1.0 ) / ( bias[ pxl ] + 1.0 ) ) ) );
            }
          }
        }
        else {
          D min_img = ( img * msk ).Minimum( );
          D min_bias = ( bias * msk ).Minimum( );
          for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
            if( msk[ pxl ] != 0 ) {
              res[ pxl ] = std::max( min_img, static_cast< D >( img[ pxl ] * static_cast< float >
                                                                ( ( min_bias + 1.0 ) / ( bias[ pxl ] + 1.0 ) ) ) );
            }
          }
        }
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

    template< class D >
    Image< D > InhomogeneityCorrection( const Image< D > &img, const Image< D > &msk, float radius,
                                        size_t compression, MRIModality modality ) {
      try {
        if( ( radius < 7.0 ) || ( radius > 28.0 ) ) {
          std::string msg( BIAL_ERROR( "Radius length must be between 7.0 and 28.0. Given: " +
                                       std::to_string( radius ) ) );
          throw( std::logic_error( msg ) );
        }
        if( ( compression != 1 ) && ( compression != 2 ) && ( compression != 4 ) ) {
          std::string msg( BIAL_ERROR( "Compression must be 1, 2, or 4. Given: " + std::to_string( compression ) ) );
          throw( std::logic_error( msg ) );
        }
        if( img.Dims( ) != msk.Dims( ) ) {
          std::string msg( BIAL_ERROR( "Image and binary mask dimensions do not match." ) );
          throw( std::logic_error( msg ) );
        }
        COMMENT( "ROI of imagens with a security border for adjacency relation free access.", 0 );
        Vector< size_t > low_coord;
        Vector< size_t > hgh_coord;
        COMMENT( "Preparing images to compute bias surface.", 0 );
        std::tie( low_coord, hgh_coord ) = ImageOp::ROI( msk );
        Image< D > sub_img = ImageOp::Resize( img, low_coord, hgh_coord );
        Image< D > sub_msk = ImageOp::Resize( msk, low_coord, hgh_coord );
        sub_img = Geometrics::Scale( sub_img, 1.0 / compression, true );
        sub_msk = Geometrics::Scale( sub_msk, 1.0 / compression, true );
        sub_img = ImageOp::AddFrame( sub_img, 2 * compression );
        sub_msk = ImageOp::AddFrame( sub_msk, 2 * compression );
        COMMENT( "Computing mask without outliers.", 0 );
        Image< D > cln_msk = RemoveIntensityOutliers( sub_img, sub_msk, compression, modality );
        sub_msk = Morphology::DilateBin( sub_msk, AdjacencyType::HyperSpheric( 1.9, img.Dims( ) ) );
        COMMENT( "Computing bias surface.", 0 );
        Image< D > bias = BiasSurfaceEstimation( sub_img, sub_msk, cln_msk, radius / compression, modality );
        COMMENT( "Converting bias image to the input format.", 0 );
        Vector< size_t > unframed_size( hgh_coord );
        for( size_t dms = 0; dms < unframed_size.size( ); ++dms )
          unframed_size( dms ) -= low_coord( dms ) - 1;
        COMMENT( "Restoring bias here and then correct inhomogeneity. This will be more precise.", 0 );
        Vector< size_t > input_dim( img.Dim( ) );
        bias = ImageOp::RemoveFrame( bias, 2 * compression );
        bias = Geometrics::Scale( bias, compression, true );
        bias = ImageOp::AddFrame( bias, low_coord, input_dim );
        bias = bias * msk;
        COMMENT( "Computing corrected image.", 0 );
        Image< D > res = BiasSurfaceRemoval( img, msk, bias, modality );
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

#ifdef BIAL_EXPLICIT_BrainInhomogeneity


    template Image< int > RemoveIntensityOutliers( const Image< int > &img, const Image< int > &msk, float compression,
                                                   MRIModality modality );
    template Image< int > BiasSurfaceEstimation( const Image< int > &img, const Image< int > &msk, 
                                                 const Image< int > &cln_msk, float radius,  MRIModality modality );
    template Image< int > BiasSurfaceRemoval( const Image< int > &img, const Image< int > &msk, 
                                              const Image< int > &bias, MRIModality modality );
    template Image< int > InhomogeneityCorrection( const Image< int > &img, const Image< int > &msk, 
                                                   float radius, size_t compression, MRIModality modality );

    template Image< llint > RemoveIntensityOutliers( const Image< llint > &img, const Image< llint > &msk,
                                                     float compression, MRIModality modality );
    template Image< llint > BiasSurfaceEstimation( const Image< llint > &img, const Image< llint > &msk, 
                                                   const Image< llint > &cln_msk, float radius, MRIModality modality );
    template Image< llint > BiasSurfaceRemoval( const Image< llint > &img, const Image< llint > &msk, 
                                                const Image< llint > &bias, MRIModality modality );
    template Image< llint > InhomogeneityCorrection( const Image< llint > &img, const Image< llint > &msk, 
                                                     float radius, size_t compression, MRIModality modality );

    template Image< float > RemoveIntensityOutliers( const Image< float > &img, const Image< float > &msk, 
                                                     float compression, MRIModality modality );
    template Image< float > BiasSurfaceEstimation( const Image< float > &img, const Image< float > &msk, 
                                                   const Image< float > &cln_msk, float radius, MRIModality modality );
    template Image< float > BiasSurfaceRemoval( const Image< float > &img, const Image< float > &msk, 
                                                const Image< float > &bias, MRIModality modality );
    template Image< float > InhomogeneityCorrection( const Image< float > &img, const Image< float > &msk, 
                                                     float radius, size_t compression, MRIModality modality );

    template Image< double > RemoveIntensityOutliers( const Image< double > &img, const Image< double > &msk,
                                                      float compression, MRIModality modality );  
    template Image< double > BiasSurfaceEstimation( const Image< double > &img, const Image< double > &msk, 
                                                    const Image< double > &cln_msk, float radius, 
                                                    MRIModality modality );
    template Image< double > BiasSurfaceRemoval( const Image< double > &img, const Image< double > &msk, 
                                                 const Image< double > &bias, MRIModality modality );
    template Image< double > InhomogeneityCorrection( const Image< double > &img, const Image< double > &msk, 
                                                      float radius, size_t compression, MRIModality modality );

#endif

  }

}

#endif

#endif
