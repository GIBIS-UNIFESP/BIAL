/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Jul/05
 * @brief Kernel relation with square format.
 * <br> Future add-on's: none.
 */

#ifndef BIALKERNELSQUARE_C
#define BIALKERNELSQUARE_C

#include "KernelSquare.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_KernelSquare )
#define BIAL_EXPLICIT_KernelSquare
#endif

#if defined ( BIAL_EXPLICIT_KernelSquare ) || ( BIAL_IMPLICIT_BIN )

#include "AdjacencySquare.hpp"
#include "Color.hpp"
#include "ColorRGB.hpp"
#include "Image.hpp"
#include "Matrix.hpp"

namespace Bial {

  template< class D > 
  Kernel KernelType::Square( const Matrix< D > &mat ) {
    try {
      Kernel krn( AdjacencyType::Square( mat ) );
      COMMENT( "Setting coefficients.", 2 );
      size_t adj = 0;
      for( size_t elm = 0; elm < mat.size( ); ++elm ) {
        if( mat[ elm ] != 0 ) {
          krn.Value( adj ) = mat[ elm ];
          ++adj;
        }
      }
      return( krn );
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
  Kernel KernelType::Square( const Image< D > &img ) {
    try {
      Kernel krn( AdjacencyType::Square( img ) );
      COMMENT( "Setting coefficients.", 2 );
      size_t adj = 0;
      for( size_t elm = 0; elm < img.size( ); ++elm ) {
        if( img[ elm ] != 0 ) {
          krn.Value( adj ) = img[ elm ];
          ++adj;
        }
      }
      return( krn );
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

  Kernel KernelType::Square( const Image< Color > &img ) {
    try {
      IF_DEBUG( ( img.Dims( ) < 2 ) || ( img.Dims( ) > 3 ) ) {
        std::string msg( BIAL_ERROR( "Kernel must have 2 or 3 dimensions." ) );
        throw( std::logic_error( msg ) );
      }
      IF_DEBUG( img.size( ) == 0 ) {
        std::string msg( BIAL_ERROR( "Empty adjacency relation." ) );
        throw( std::logic_error( msg ) );
      }
      Image< int > gray_img( ColorSpace::ARGBtoGraybyBrightness< int >( img ) );
      return( KernelType::Square( gray_img ) );
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
  
#ifdef BIAL_EXPLICIT_KernelSquare

  /* Explicit instantiations. --------------------------------------------------------------------------------------- */

#endif

}

#endif

#endif
