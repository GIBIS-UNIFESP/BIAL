/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2014/Jan/07
 * @brief Image splitting.
 */

#ifndef BIALIMAGESPLIT_C
#define BIALIMAGESPLIT_C

#include "ImageSplit.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_ImageSplit )
#define BIAL_EXPLICIT_ImageSplit
#endif

#if defined ( BIAL_EXPLICIT_ImageSplit ) || ( BIAL_IMPLICIT_BIN )

#include "Image.hpp"

namespace Bial {

  template< class D >
  Vector< Image< D > > ImageOp::Split( const Image< D > &img, size_t dimension ) {
    try {
      COMMENT( "Getting number of dimensions and verifying if image has 3 dimensions to be split.", 2 );
      size_t img_dims = img.Dims( );
      if( img_dims < 3 ) {
        std::string msg( BIAL_ERROR( " Source image must have three dimensions." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Verifying if split dimension exists and if it has more than 1 element.", 2 );
      if( ( img_dims <= dimension ) || ( img.size( dimension ) <= 1 ) ) {
        std::string msg( BIAL_ERROR(
                           " Split dimension does not exist or has less than two elements. Image dimensions: " +
                           std::to_string( img_dims ) + ", split dimension: " + std::to_string( dimension ) +
                           ". Split dimention size: " + std::to_string( img.size( dimension ) ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Computing number of elements in dimensions lower than the split dimension.", 2 );
      COMMENT( "If there is no lower dimension, set it to one element.", 2 );
      size_t lower_elements = 1;
      if( dimension > 0 )
        lower_elements = img.Displacement( dimension - 1 );
      COMMENT( "Compute displacement of data in dimensions higher than the split dimension and number of elements.", 2 );
      size_t higher_displacement = img.Displacement( dimension );
      size_t higher_elements = img.Displacement( img_dims - 1 ) / higher_displacement;
      COMMENT( "Compute the displacement of data in the split dimension and the number of elements.", 2 );
      size_t split_elements = img.size( dimension );
      COMMENT( "Computing target image properties.", 2 );
      Vector< size_t > spc_dim( img.Dim( ) );
      if( dimension < spc_dim.size( ) )
        spc_dim.erase( spc_dim.begin( ) + dimension );
      Vector< float > new_pixel_size( img.PixelSize( ) );
      if( dimension < spc_dim.size( ) ) {
        new_pixel_size.erase( new_pixel_size.begin( ) + dimension );
      }
      COMMENT( "Creating vector image.", 2 );
      Vector< Image< D > > res( split_elements, Image< D >( spc_dim, new_pixel_size ) );
      COMMENT( "Assigning split image data.", 2 );
      for( size_t hgh_coord = 0; hgh_coord < higher_elements; ++hgh_coord ) {
        for( size_t low_coord = 0; low_coord < lower_elements; ++low_coord ) {
          for( size_t spt_coord = 0; spt_coord < split_elements; ++spt_coord ) {
            size_t tgt_coord = hgh_coord * lower_elements + low_coord;
            size_t src_coord = hgh_coord * higher_displacement + low_coord + spt_coord * lower_elements;
            res( spt_coord )[ tgt_coord ] = img[ src_coord ];
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
    catch( const std::exception &e ) {
      std::cout << e.what( ) << std::string( "\n" ) << BIAL_ERROR( "Unexpected Error." ) << std::endl;
      throw( std::exception( ) );
    }
    catch( ... ) {
      std::cout << BIAL_ERROR( "Unexpected Error." ) << std::endl;
      throw( std::exception( ) );
    }
  }

#ifdef BIAL_EXPLICIT_ImageSplit

  template Vector< Image< int > > ImageOp::Split( const Image< int > &img, size_t dimension );
  template Vector< Image< llint > > ImageOp::Split( const Image< llint > &img, size_t dimension );
  template Vector< Image< float > > ImageOp::Split( const Image< float > &img, size_t dimension );
  template Vector< Image< double > > ImageOp::Split( const Image< double > &img, size_t dimension );

#endif

}

#endif

#endif
