 /* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Jul/12 
 * @brief: Segmentation of a 2D image in quadrants.
 */

#ifndef BIALSEGMENTATIONQUADRANTS_C
#define BIALSEGMENTATIONQUADRANTS_C

#include "SegmentationQuadrants.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_SegmentationQuadrants )
#define BIAL_EXPLICIT_SegmentationQuadrants
#endif
#if defined ( BIAL_EXPLICIT_SegmentationQuadrants ) || ( BIAL_IMPLICIT_BIN )

#include "Image.hpp"
#include "ImageResize.hpp"

namespace Bial {

  template< class D >
  Matrix< Image< D > > Segmentation::Quadrants( const Image< D > &data ) {
    try {
      COMMENT( "Getting the number of dimensions to compute the size of the resultant matrix.", 1 );
      size_t dims = data.Dims( );
      if( dims != 2 ) {
        std::string msg( BIAL_ERROR( "Invalid image or matrix input dimensions. Given: " + std::to_string( dims ) +
                                     ", expected: 2." ) );
        throw( std::runtime_error( msg ) );
      }
      COMMENT( "Creating resultant image.", 1 );
      Matrix< Image< D > > res( 2, 2 );
      COMMENT( "Computing quadrants for 2D image or matrix.", 1 );
      res( 0, 0 ) = ImageOp::Resize( data, { 0, 0 }, { data.size( 0 ) / 2, data.size( 1 ) / 2 } );
      res( 1, 0 ) = ImageOp::Resize( data, { data.size( 0 ) / 2 + 1, 0 }, { data.size( 0 ) - 1, data.size( 1 ) / 2 } );
      res( 0, 1 ) = ImageOp::Resize( data, { 0, data.size( 1 ) / 2 + 1 }, { data.size( 0 ) / 2, data.size( 1 ) - 1 } );
      res( 1, 1 ) = ImageOp::Resize( data, { data.size( 0 ) / 2 + 1, data.size( 1 ) / 2 + 1 }, 
                                     { data.size( 0 ) - 1, data.size( 1 ) - 1 } );
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
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Empty kernel." ) );
      throw( std::logic_error( msg ) );
    }
  }

#ifdef BIAL_EXPLICIT_SegmentationQuadrants

  template Matrix< Image< int > > Segmentation::Quadrants( const Image< int > &data );
  template Matrix< Image< llint > > Segmentation::Quadrants( const Image< llint > &data );
  template Matrix< Image< float > > Segmentation::Quadrants( const Image< float > &data );
  template Matrix< Image< double > > Segmentation::Quadrants( const Image< double > &data );

#endif

}

#endif

#endif
