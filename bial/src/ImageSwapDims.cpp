/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2014/Jan/07
 * @brief Image swap dimensions.
 */

#ifndef BIALIMAGESWAPDIMS_C
#define BIALIMAGESWAPDIMS_C

#include "Image.hpp"
#include "ImageSwapDims.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_ImageSwapDims )
#define BIAL_EXPLICIT_ImageSwapDims
#endif

#if defined ( BIAL_EXPLICIT_ImageSwapDims ) || ( BIAL_IMPLICIT_BIN )

namespace Bial {

  template< class D >
  Image< D > ImageOp::SwapDims( const Image< D > &img, size_t dim_0, size_t dim_1, bool invert_dir_0, 
                                bool invert_dir_1 ) {
    try {
      COMMENT( "Verifying dimensions.", 1 );
      if( ( dim_0 >= img.Dims( ) ) || ( dim_1 >= img.Dims( ) ) ) {
        std::string msg( BIAL_ERROR( "Only spatial dimensions can be swapped." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Creating new image.", 0 );
      Vector< size_t > dims( img.Dim( ) );
      std::swap( dims( dim_0 ), dims( dim_1 ) );
      Vector< size_t > pix_dims = img.PixelSize( );
      std::swap( pix_dims( dim_0 ), pix_dims( dim_1 ) );
      Image< D > res( dims, pix_dims );
      COMMENT( "Computing auxliary variables containing the correct dimension, their sizes and displacements.", 0 );
      size_t src_size_0 = img.size( dim_0 );
      size_t src_size_1 = img.size( dim_1 );
      size_t src_disp_0 = img.Displacement( dim_0 ) / src_size_0;
      size_t src_disp_1 = img.Displacement( dim_1 ) / src_size_1;
      size_t tgt_disp_0 = res.Displacement( dim_0 ) / src_size_1;
      size_t tgt_disp_1 = res.Displacement( dim_1 ) / src_size_0;
      COMMENT( "Converting image.", 0 );
      COMMENT( "Code duplicated for performance reasons.", 3 );
      if( ( !invert_dir_0 ) && ( !invert_dir_1 ) ) {
        for( size_t dir_1 = 0; dir_1 < src_size_1; ++dir_1 ) {
          for( size_t dir_0 = 0; dir_0 < src_size_0; ++dir_0 ) {
            res( dir_0 * tgt_disp_0 + dir_1 * tgt_disp_1 ) = img( dir_0 * src_disp_0 + dir_1 * src_disp_1 );
          }
        }
      }
      if( ( invert_dir_0 ) && ( !invert_dir_1 ) ) {
        for( size_t dir_1 = 0; dir_1 < src_size_1; ++dir_1 ) {
          for( size_t dir_0 = 0; dir_0 < src_size_0; ++dir_0 ) {
            res( dir_0 * tgt_disp_0 + ( src_size_1 - dir_1 - 1 ) * tgt_disp_1 ) =
              img( dir_0 * src_disp_0 + dir_1 * src_disp_1 );
          }
        }
      }
      if( ( !invert_dir_0 ) && ( invert_dir_1 ) ) {
        for( size_t dir_1 = 0; dir_1 < src_size_1; ++dir_1 ) {
          for( size_t dir_0 = 0; dir_0 < src_size_0; ++dir_0 ) {
            res( ( src_size_0 - dir_0 - 1 ) * tgt_disp_0 + dir_1 * tgt_disp_1 ) =
              img( dir_0 * src_disp_0 + dir_1 * src_disp_1 );
          }
        }
      }
      if( ( invert_dir_0 ) && ( invert_dir_1 ) ) {
        for( size_t dir_1 = 0; dir_1 < src_size_1; ++dir_1 ) {
          for( size_t dir_0 = 0; dir_0 < src_size_0; ++dir_0 ) {
            res( ( src_size_0 - dir_0 - 1 ) * tgt_disp_0 + ( src_size_1 - dir_1 - 1 ) * tgt_disp_1 ) =
              img( dir_0 * src_disp_0 + dir_1 * src_disp_1 );
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

#ifdef BIAL_EXPLICIT_ImageSwapDims

  template Image< int > ImageOp::SwapDims( const Image< int > &img, size_t dim_0, size_t dim_1, bool invert_dir_0,
                                           bool invert_dir_1 );
  template Image< llint > ImageOp::SwapDims( const Image< llint > &img, size_t dim_0, size_t dim_1, bool invert_dir_0,
                                             bool invert_dir_1 );
  template Image< float > ImageOp::SwapDims( const Image< float > &img, size_t dim_0, size_t dim_1, bool invert_dir_0,
                                             bool invert_dir_1 );
  template Image< double > ImageOp::SwapDims( const Image< double > &img, size_t dim_0, size_t dim_1, bool invert_dir_0,
                                              bool invert_dir_1 );

#endif

}

#endif

#endif
