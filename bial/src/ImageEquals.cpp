/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2015/Aug/21
 * @brief Algorithms to check if tow images have the same dimensions and containts.
 */

#ifndef BIALIMAGEEQUALS_C
#define BIALIMAGEEQUALS_C

#include "ImageEquals.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_ImageEquals )
#define BIAL_EXPLICIT_ImageEquals
#endif

#if defined ( BIAL_EXPLICIT_ImageEquals ) || ( BIAL_IMPLICIT_BIN )

#include "Image.hpp"
#include "FileImage.hpp"

namespace Bial {

  bool ImageOp::EqualImages( const std::string &file1, const std::string &file2 ) {
    try {
      Image< double > img1( Read< double >( file1 ) );
      Image< double > img2( Read< double >( file2 ) );
      return( ImageOp::EqualImages( img1, img2 ) );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error opening images." ) );
      throw( std::ios_base::failure( msg ) );
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

  template< class D, class D2 >
  inline bool ImageOp::EqualImages( const Image< D > &img1, const Image< D2 > &img2 ) {
    try {
      if( img1.Dim( ) != img2.Dim( ) ) {
        COMMENT( "Dimensions do not match.", 0 );
        return( false );
      }
      int errors = 0;
      for( size_t pxl = 0; pxl < img1.size( ); ++pxl ) {
        if( img1[ pxl ] != img2[ pxl ] ) {
          COMMENT( "Different pixel " << pxl << ". Values: " << img1[ pxl ] << ", " << img2[ pxl ], 4 );
          errors++;
        }
      }
      if( errors != 0 ) {
        COMMENT( errors << " pixels do not match. Errors: " << errors, 0 );
        return( false );
      }
      return( true );
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

#ifdef BIAL_EXPLICIT_ImageEquals
  
  template bool ImageOp::EqualImages( const Image< int > &img1, const Image< int > &img2 );
  template bool ImageOp::EqualImages( const Image< llint > &img1, const Image< int > &img2 );
  template bool ImageOp::EqualImages( const Image< float > &img1, const Image< int > &img2 );
  template bool ImageOp::EqualImages( const Image< double > &img1, const Image< int > &img2 );

  template bool ImageOp::EqualImages( const Image< int > &img1, const Image< llint > &img2 );
  template bool ImageOp::EqualImages( const Image< llint > &img1, const Image< llint > &img2 );
  template bool ImageOp::EqualImages( const Image< float > &img1, const Image< llint > &img2 );
  template bool ImageOp::EqualImages( const Image< double > &img1, const Image< llint > &img2 );

  template bool ImageOp::EqualImages( const Image< int > &img1, const Image< float > &img2 );
  template bool ImageOp::EqualImages( const Image< llint > &img1, const Image< float > &img2 );
  template bool ImageOp::EqualImages( const Image< float > &img1, const Image< float > &img2 );
  template bool ImageOp::EqualImages( const Image< double > &img1, const Image< float > &img2 );

  template bool ImageOp::EqualImages( const Image< int > &img1, const Image< double > &img2 );
  template bool ImageOp::EqualImages( const Image< llint > &img1, const Image< double > &img2 );
  template bool ImageOp::EqualImages( const Image< float > &img1, const Image< double > &img2 );
  template bool ImageOp::EqualImages( const Image< double > &img1, const Image< double > &img2 );
  
#endif

}

#endif

#endif
