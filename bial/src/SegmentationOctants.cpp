 /* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Jul/12 
 * @brief: Segmentation of 3D image into octants.
 */

#ifndef BIALSEGMENTATIONOCTANTS_C
#define BIALSEGMENTATIONOCTANTS_C

#include "SegmentationOctants.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_SegmentationOctants )
#define BIAL_EXPLICIT_SegmentationOctants
#endif
#if defined ( BIAL_EXPLICIT_SegmentationOctants ) || ( BIAL_IMPLICIT_BIN )

#include "Image.hpp"
#include "ImageResize.hpp"

namespace Bial {

  template< class D >
  Matrix< Image< D > > Segmentation::Octants( const Image< D > &data ) {
    try {
      COMMENT( "Getting the number of dimensions to compute the size of the resultant matrix.", 1 );
      size_t dims = data.Dims( );
      if( dims != 3 ) {
        std::string msg( BIAL_ERROR( "Invalid image or matrix input dimensions. Given: " + std::to_string( dims ) +
                                     ", expected: 3." ) );
        throw( std::runtime_error( msg ) );
      }
      COMMENT( "Creating resultant image.", 1 );
      Matrix< Image< D > > res( 2, 2, 2 );
      COMMENT( "Computing octants for 3D image or matrix.", 1 );
      res( 0, 0, 0 ) = ImageOp::Resize( data, { 0, 0, 0 },
                                        { data.size( 0 ) / 2, data.size( 1 ) / 2, data.size( 2 ) / 2 } );
      res( 1, 0, 0 ) = ImageOp::Resize( data, { data.size( 0 ) / 2 + 1, 0, 0 },
                                        { data.size( 0 ) - 1, data.size( 1 ) / 2, data.size( 2 ) / 2 } );
      res( 0, 1, 0 ) = ImageOp::Resize( data, { 0, data.size( 1 ) / 2 + 1, 0 },
                                        { data.size( 0 ) / 2, data.size( 1 ) - 1, data.size( 2 ) / 2 } );
      res( 1, 1, 0 ) = ImageOp::Resize( data, { data.size( 0 ) / 2 + 1, data.size( 1 ) / 2 + 1, 0 },
                                        { data.size( 0 ) - 1, data.size( 1 ) - 1, data.size( 2 ) / 2 } );
      res( 0, 0, 1 ) = ImageOp::Resize( data, { 0, 0, data.size( 2 ) / 2 + 1 },
                                        { data.size( 0 ) / 2, data.size( 1 ) / 2, data.size( 2 ) - 1 } );
      res( 1, 0, 1 ) = ImageOp::Resize( data, { data.size( 0 ) / 2 + 1, 0, data.size( 2 ) / 2 + 1 },
                                        { data.size( 0 ) - 1, data.size( 1 ) / 2, data.size( 2 ) - 1 } );
      res( 0, 1, 1 ) = ImageOp::Resize( data, { 0, data.size( 1 ) / 2 + 1, data.size( 2 ) / 2 + 1 },
                                        { data.size( 0 ) / 2, data.size( 1 ) - 1, data.size( 2 ) - 1 } );
      res( 1, 1, 1 ) = ImageOp::Resize( data, { data.size( 0 ) / 2 + 1, data.size( 1 ) / 2 + 1, 
            data.size( 2 ) / 2 + 1 }, { data.size( 0 ) - 1, data.size( 1 ) - 1, data.size( 2 ) - 1 } );
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

  template< class D >
  Image< D > Segmentation::Octants( const Matrix< Image< D > > &data ) {
    COMMENT( "Creating resultant image or matrix.", 1 );
    Image< D > res( data( 0, 0, 0 ).size( 0 ) + data( 1, 0, 0 ).size( 0 ),
                    data( 0, 0, 0 ).size( 1 ) + data( 0, 1, 0 ).size( 1 ),
                    data( 0, 0, 0 ).size( 2 ) + data( 0, 0, 1 ).size( 2 ) );
    COMMENT( "Copying data into the resultant image.", 1 );
    for( size_t oct_z = 0; oct_z < 2; ++oct_z ) {
      for( size_t oct_y = 0; oct_y < 2; ++oct_y ) {
        for( size_t oct_x = 0; oct_x < 2; ++oct_x ) {
          for( size_t pxl_z = 0; pxl_z < data( oct_x, oct_y, oct_z ).size( 2 ); ++pxl_z ) {
            for( size_t pxl_y = 0; pxl_y < data( oct_x, oct_y, oct_z ).size( 1 ); ++pxl_y ) {
              for( size_t pxl_x = 0; pxl_x < data( oct_x, oct_y, oct_z ).size( 0 ); ++pxl_x ) {
                res( pxl_x + oct_x * data( 0, 0, 0 ).size( 0 ),
                     pxl_y + oct_y * data( 0, 0, 0 ).size( 1 ),
                     pxl_z + oct_z * data( 0, 0, 0 ).size( 2 ) ) =
                  data( oct_x, oct_y, oct_z )( pxl_x, pxl_y, pxl_z );
              }
            }
          }
        }
      }
    }
    return( res );
  }


#ifdef BIAL_EXPLICIT_SegmentationOctants

  template Matrix< Image< int > > Segmentation::Octants( const Image< int > &data );
  template Image< int > Segmentation::Octants( const Matrix< Image< int > > &data );
  template Matrix< Image< llint > > Segmentation::Octants( const Image< llint > &data );
  template Image< llint > Segmentation::Octants( const Matrix< Image< llint > > &data );
  template Matrix< Image< float > > Segmentation::Octants( const Image< float > &data );
  template Image< float > Segmentation::Octants( const Matrix< Image< float > > &data );
  template Matrix< Image< double > > Segmentation::Octants( const Image< double > &data );
  template Image< double > Segmentation::Octants( const Matrix< Image< double > > &data );

#endif

}

#endif

#endif
