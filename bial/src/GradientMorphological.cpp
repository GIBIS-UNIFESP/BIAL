/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/21 
 * @brief Computes morphological gradient, that is, open - close.
 */

#ifndef BIALGRADIENTMORPHOLOGICAL_C
#define BIALGRADIENTMORPHOLOGICAL_C

#include "GradientMorphological.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_GradientMorphological )
#define BIAL_EXPLICIT_GradientMorphological
#endif
#if defined ( BIAL_EXPLICIT_GradientMorphological ) || ( BIAL_IMPLICIT_BIN )

#include "Adjacency.hpp"
#ifdef BIAL_DEBUG
#include "FileImage.hpp"
#endif
#include "Image.hpp"
#include "MorphologyDilation.hpp"
#include "MorphologyErosion.hpp"

namespace Bial {

  template< class D >
  Image< D > Gradient::Morphological( const Image< D > &image ) {
    try {
      return( Morphology::Dilate( image ) - Morphology::Erode( image ) );
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
  Image< D > Gradient::Morphological( const Image< D > &image, const Adjacency &adjacency ) {
    try {
      if( image.Dims( ) != adjacency.Dims( ) ) {
        std::string msg( BIAL_ERROR( "Image and adjacency relation dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      return( Morphology::Dilate( image, adjacency ) - Morphology::Erode( image, adjacency ) );
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

#ifdef BIAL_EXPLICIT_GradientMorphological

  template Image< int > Gradient::Morphological( const Image< int > &image );
  template Image< int > Gradient::Morphological( const Image< int > &image, const Adjacency &adjacency );

  template Image< llint > Gradient::Morphological( const Image< llint > &image );
  template Image< llint > Gradient::Morphological( const Image< llint > &image, const Adjacency &adjacency );

  template Image< float > Gradient::Morphological( const Image< float > &image );
  template Image< float > Gradient::Morphological( const Image< float > &image, const Adjacency &adjacency );

  template Image< double > Gradient::Morphological( const Image< double > &image );
  template Image< double > Gradient::Morphological( const Image< double > &image, const Adjacency &adjacency );

#endif

}

#endif

#endif
