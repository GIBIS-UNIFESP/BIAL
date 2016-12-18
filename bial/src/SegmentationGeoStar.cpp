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
      Image< int > label( image );
      label.Set( -1 );
      Vector< bool > seeds( image.size( ), 0 );
      for( size_t idx = 0; idx < obj_seeds.size( ); ++idx ) {
        seeds( obj_seeds( idx ) ) = true;
        label( obj_seeds( idx ) ) = 1;
      }
      GeodesicRestrictionPathFunction< D > geo_path( grad, image, alpha, beta );
      COMMENT( "Running geodesic star restriction IFT.", 0 );
      size_t size = grad.size( );
      for( size_t elm = 0; elm < size; ++elm ) {
        if( seeds[ elm ] )
          grad[ elm ] = 0;
        else
          grad[ elm ] = std::numeric_limits< D >::max( );
      }
      ImageIFT< D > ift( grad, adj, &geo_path, &seeds, static_cast< Image< int >* >( nullptr ), &pred, false, 
                         static_cast< D >( 1.0 ), true );
      ift.Run( );
      COMMENT( "Setting background seeds.", 0 );
      for( size_t idx = 0; idx < bkg_seeds.size( ); ++idx ) {
        grad[ bkg_seeds( idx ) ] = 0;
        seeds( bkg_seeds( idx ) ) = true;
        pred[ bkg_seeds( idx ) ] = -1;
        label( bkg_seeds( idx ) ) = 0;
      }
      COMMENT( "Running oriented path function.", 0 );
      if( alpha >= 0.0 ) {
        OrientedExternPathFunction< D > oriented_path( value, image, &pred, alpha );
        for( size_t elm = 0; elm < size; ++elm ) {
          if( seeds[ elm ] )
            value[ elm ] = 0;
          else
            value[ elm ] = std::numeric_limits< D >::max( );
        }
        ImageIFT< D > ift( value, adj, &oriented_path, &seeds, &label, static_cast< Image< int >* >( nullptr ), false,
                           static_cast< D >( 1.0 ), true );
        ift.Run( );
      }
      else {
        OrientedInternPathFunction< D > oriented_path( value, image, &pred, -alpha );
        for( size_t elm = 0; elm < size; ++elm ) {
          if( seeds[ elm ] )
            value[ elm ] = 0;
          else
            value[ elm ] = std::numeric_limits< D >::max( );
        }
        ImageIFT< D > ift( value, adj, &oriented_path, &seeds, &label, static_cast< Image< int >* >( nullptr ), false,
                           static_cast< D >( 1.0 ), true );
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
