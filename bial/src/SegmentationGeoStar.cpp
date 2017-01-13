 /* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Jul/12 
 * @brief: Segmentation with oriented geodesic star algorithm. 
 *         Combines geodesic distance and sum of arcs with orientation.
 */

#ifndef BIALSEGMENTATIONGEOSTAR_C
#define BIALSEGMENTATIONGEOSTAR_C

#include "SegmentationGeoStar.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_SegmentationGeoStar )
#define BIAL_EXPLICIT_SegmentationGeoStar
#endif
#if defined ( BIAL_EXPLICIT_SegmentationGeoStar ) || ( BIAL_IMPLICIT_BIN )

#include "AdjacencyRound.hpp"
#include "GeoSumPathFunction.hpp"
#include "GradientDirectional.hpp"
#include "GrowingBucketQueue.hpp"
#include "Image.hpp"
#include "ImageIFT.hpp"
#include "OrientedExternPathFunction.hpp"
#include "OrientedInternPathFunction.hpp"

namespace Bial {

  template< class D >
  Image< D > Segmentation::OrientedGeodesicStar( const Image< D > &image, const Vector< size_t > &obj_seeds,
                                                 const Vector< size_t > &bkg_seeds, double alpha, double beta ) {
    try {
      if( ( alpha < -1.0 ) || ( alpha > 1.0 ) ) {
        std::string msg( BIAL_ERROR( "Alpha must be set between -1.0 and 1.0. Given value: " + std::to_string( alpha )
                                     + "." ) );
        throw( std::logic_error( msg ) );
      }
      if( ( beta < 0.0 ) || ( beta > 4.0 ) ) {
        std::string msg( BIAL_ERROR( "Beta must be set between 0.0 and 4.0. Given value: " +
                                     std::to_string( beta ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      if( ( obj_seeds.size( ) == 0 ) || ( bkg_seeds.size( ) == 0 ) ) {
        std::string msg( BIAL_ERROR( "Must have at last one object and one background seed. Given object seeds: " +
                                     std::to_string( obj_seeds.size( ) ) + ", background seeds: " +
                                     std::to_string( bkg_seeds.size( ) ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Initializing maps.", 0 );
      Adjacency adj( AdjacencyType::HyperSpheric( 1.5, image.Dims( ) ) );
      Image< D > grad( Gradient::Directional( image, adj ) );
      Image< int > pred( image );
      Image< D > value( grad );
      Image< int > label( image.Dim( ), image.PixelSize( ) );
      label.Set( -1 );
      size_t size = image.size( );
      Vector< bool > seeds( image.size( ), 0 );
      COMMENT( "FIRST STEP. Running with foreground seeds.", 0 );
      GeodesicRestrictionPathFunction< D > geo_path( grad, nullptr, &pred, false, grad, image, alpha, beta );
      GrowingBucketQueue queue( size, 1, geo_path.Increasing( ), true );
      for( size_t elm = 0; elm < size; ++elm )
        grad[ elm ] = std::numeric_limits< D >::max( );
      for( size_t idx = 0; idx < obj_seeds.size( ); ++idx ) {
        size_t elm = obj_seeds( idx );
        label( elm ) = 1;
        grad[ elm ] = 0;
        queue.Insert( elm, grad[ elm ] );
        seeds[ elm ] = true;
      }
      COMMENT( "Running geodesic star restriction IFT.", 0 );
      ImageIFT< D > ift( grad, adj, &geo_path, &queue );
      ift.Run( );
      COMMENT( "SECOND STEP. Running with background and foreground seeds.", 0 );
      COMMENT( "Setting seeds with background.", 0 );
      for( size_t elm = 0; elm < size; ++elm )
        grad[ elm ] = std::numeric_limits< D >::max( );
      COMMENT( "Inserting obj seeds:", 0 );
      for( size_t idx = 0; idx < obj_seeds.size( ); ++idx ) {
        size_t elm = obj_seeds( idx );
        grad[ elm ] = 0;
        queue.Insert( elm, grad[ elm ] );
      }
      COMMENT( "Inserting bkg seeds:", 0 );
      for( size_t idx = 0; idx < bkg_seeds.size( ); ++idx ) {
        size_t elm = bkg_seeds( idx );
        label( elm ) = 0;
        pred( elm ) = -1;
        grad[ elm ] = 0;
        queue.Insert( elm, grad[ elm ] );
        seeds[ elm ] = true;
      }
      COMMENT( "Running oriented path function.", 0 );
      if( alpha >= 0.0 ) {
        COMMENT( "OrientedExternPathFunction.", 0 );
        OrientedExternPathFunction< D > oriented_path( grad, label, nullptr, false, value, image, &pred, alpha );
        ImageIFT< D > ift( grad, adj, &oriented_path, &queue );
        ift.Run( );
      }
      else {
        COMMENT( "OrientedInternPathFunction.", 0 );
        OrientedInternPathFunction< D > oriented_path( grad, label, nullptr, false, value, image, &pred, -alpha );
        ImageIFT< D > ift( grad, adj, &oriented_path, &queue );
        ift.Run( );
      }
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

#ifdef BIAL_EXPLICIT_SegmentationGeoStar

  template 
  Image< int > Segmentation::OrientedGeodesicStar( const Image< int > &image, const Vector< size_t > &obj_seeds,
                                                   const Vector< size_t > &bkg_seeds, double alpha, double beta );
  template 
  Image< llint > Segmentation::OrientedGeodesicStar( const Image< llint > &image, const Vector< size_t > &obj_seeds,
                                                     const Vector< size_t > &bkg_seeds, double alpha, double beta );
  template
  Image< float > Segmentation::OrientedGeodesicStar( const Image< float > &image, const Vector< size_t > &obj_seeds,
                                                     const Vector< size_t > &bkg_seeds, double alpha, double beta );
  template 
  Image< double > Segmentation::OrientedGeodesicStar( const Image< double > &image, const Vector< size_t > &obj_seeds,
                                                      const Vector< size_t > &bkg_seeds, double alpha, double beta );

#endif

}

#endif

#endif
