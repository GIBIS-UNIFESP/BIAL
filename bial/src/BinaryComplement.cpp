/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Oct/18 
 * @brief Computes the complement of a binary image restricted or not to a mask.
 */

#ifndef BIALBINARYCOMPLEMENT_C
#define BIALBINARYCOMPLEMENT_C

#include "BinaryComplement.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_BinaryComplement )
#define BIAL_EXPLICIT_BinaryComplement
#endif

#if defined ( BIAL_EXPLICIT_BinaryComplement ) || ( BIAL_IMPLICIT_BIN )

#include "Image.hpp"

namespace Bial {

  template< class D >
  void Binary::Complement( Image< D > &img ) {
    try {
      for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
        img[ pxl ] = 1 - img[ pxl ];
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
  void Binary::Complement( Image< D > &img, const Image< D > &msk ) {
    try {
      if( img.Dims( ) != msk.Dims( ) ) {
        std::string msg( BIAL_ERROR( "Input image dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
        if( msk[ pxl ] != 0 ) {
          img[ pxl ] = 1 - img[ pxl ];
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

#ifdef BIAL_EXPLICIT_BinaryComplement

    template void Binary::Complement( Image< int > &img );
    template void Binary::Complement( Image< int > &img, const Image< int > &msk );
    template void Binary::Complement( Image< llint > &img );
    template void Binary::Complement( Image< llint > &img, const Image< llint > &msk );
    template void Binary::Complement( Image< float > &img );
    template void Binary::Complement( Image< float > &img, const Image< float > &msk );
    template void Binary::Complement( Image< double > &img );
    template void Binary::Complement( Image< double > &img, const Image< double > &msk );

#endif

}

#endif

#endif
