 /* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2016/Nov/03 
 * @brief: Segmentation with Sum Path Funcion.
 */

#ifndef BIALSEGMENTATIONFSUM_C
#define BIALSEGMENTATIONFSUM_C

#include "SegmentationFSum.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_SegmentationFSum )
#define BIAL_EXPLICIT_SegmentationFSum
#endif
#if defined ( BIAL_EXPLICIT_SegmentationFSum ) || ( BIAL_IMPLICIT_BIN )

#include "AdjacencyRound.hpp"
#include "Image.hpp"
#include "ImageIFT.hpp"
#include "IntensityLocals.hpp"
#include "SumPathFunction.hpp"

namespace Bial {

  template< class D >
  Image< int > Segmentation::FSum( Image< D > &gradient, float radius ) {
    try {
      Adjacency spheric = AdjacencyType::HyperSpheric( radius, gradient.Dims( ) );
      Vector< bool > local_minima = Intensity::LocalMinima( gradient, spheric );
      return( Segmentation::FSum( gradient, local_minima ) );
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
  Image< int > Segmentation::FSum( Image< D > &gradient, const Vector< bool > &seeds ) {
    try {
      if( seeds.size( ) != gradient.size( ) ) {
        std::string msg( BIAL_ERROR( "Gradient image and seed vector must have the same number of elements." ) );
        throw( std::logic_error( msg ) );
      }
      Adjacency spheric = AdjacencyType::HyperSpheric( 1.0, gradient.Dims( ) );
      Image< int > label( gradient );
      label.Set( 0.0 );
      SumPathFunction< Image, D > max_function( gradient );
      size_t size = gradient.size( );
      COMMENT( "Setting seeds. Image size: " << size, 0 );
      for( size_t pxl = 0; pxl < size; ++pxl ) {
        if( !seeds[ pxl ] )
          gradient[ pxl ] += 1.0;
        else
          gradient[ pxl ] = std::numeric_limits< D >::max( );
      }
      COMMENT( "Running IFT.", 0 );
      ImageIFT< D > ift( gradient, spheric, &max_function, &seeds, &label, static_cast< Image< int >* >( nullptr ), 
                         true, static_cast< D >( 1.0 ), true );
      ift.Run( );
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
  Image< int > Segmentation::FSum( Image< D > &gradient, const Vector< size_t > &obj_seeds,
                                   const Vector< size_t > &bkg_seeds ) {
    try {
      if( ( obj_seeds.size( ) == 0 ) || ( bkg_seeds.size( ) == 0 ) ) {
        std::string msg( BIAL_ERROR( "Object seeds and background seeds should not be empty vectors. Given: " +
                                     std::to_string( obj_seeds.size( ) ) + ", " +
                                     std::to_string( bkg_seeds.size( ) ) ) );
        throw( std::logic_error( msg ) );
      }
      Adjacency spheric = AdjacencyType::HyperSpheric( 1.0, gradient.Dims( ) );
      Image< int > label( gradient );
      size_t size = gradient.size( );
      Vector< bool > seeds( size, false );
      label.Set( 0.0 );
      SumPathFunction< Image, D > min_function( gradient );
      for( size_t elm = 0; elm < obj_seeds.size( ); ++elm ) {
        label[ obj_seeds[ elm ] ] = 1;
        seeds[ obj_seeds[ elm ] ] = true;
        gradient[ obj_seeds[ elm ] ] += 1.0;
      }
      for( size_t elm = 0; elm < bkg_seeds.size( ); ++elm ) {
        label[ bkg_seeds[ elm ] ] = 0;
        seeds[ bkg_seeds[ elm ] ] = true;
        gradient[ bkg_seeds[ elm ] ] += 1.0;
      }
      for( size_t elm = 0; elm < size; ++elm ) {
        if( !seeds[ elm ] )
          gradient[ elm ] = std::numeric_limits< D >::max( );
      }
      ImageIFT< D > ift( gradient, spheric, &min_function, &seeds, &label, static_cast< Image< int >* >( nullptr ), 
                         false, static_cast< D >( 1.0 ), true );
      ift.Run( );
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


#ifdef BIAL_EXPLICIT_SegmentationFSum

  template Image< int > Segmentation::FSum( Image< int > &gradient, float radius );
  template Image< int > Segmentation::FSum( Image< int > &gradient, const Vector< bool > &seeds );
  template Image< int > Segmentation::FSum( Image< int > &gradient, const Vector< size_t > &obj_seeds,
                                                 const Vector< size_t > &bkg_seeds );
  template Image< int > Segmentation::FSum( Image< llint > &gradient, float radius );
  template Image< int > Segmentation::FSum( Image< llint > &gradient, const Vector< bool > &seeds );
  template Image< int > Segmentation::FSum( Image< llint > &gradient, const Vector< size_t > &obj_seeds,
                                                 const Vector< size_t > &bkg_seeds );
  template Image< int > Segmentation::FSum( Image< float > &gradient, float radius );
  template Image< int > Segmentation::FSum( Image< float > &gradient, const Vector< bool > &seeds );
  template Image< int > Segmentation::FSum( Image< float > &gradient, const Vector< size_t > &obj_seeds,
                                                 const Vector< size_t > &bkg_seeds );
  template Image< int > Segmentation::FSum( Image< double > &gradient, float radius );
  template Image< int > Segmentation::FSum( Image< double > &gradient, const Vector< bool > &seeds );
  template Image< int > Segmentation::FSum( Image< double > &gradient, const Vector< size_t > &obj_seeds,
                                                 const Vector< size_t > &bkg_seeds );

#endif

}

#endif

#endif
