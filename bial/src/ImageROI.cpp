/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2014/Jan/07
 * @brief Image region of interest. Minimal sub image coordinates without zero frame.
 */

#ifndef BIALIMAGEROI_C
#define BIALIMAGEROI_C

#include "Image.hpp"
#include "ImageROI.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_ImageROI )
#define BIAL_EXPLICIT_ImageROI
#endif

#if defined ( BIAL_EXPLICIT_ImageROI ) || ( BIAL_IMPLICIT_BIN )

namespace Bial {

  template< class D >
  std::tuple< Vector< size_t >, Vector< size_t > > ImageOp::ROI( const Image< D > &img ) {
    try {
      COMMENT( "Computing low and high coordinates.", 0 );
      Vector< size_t > dim( img.Dim( ) );
      Vector< size_t > low_coord( dim );
      Vector< size_t > hgh_coord( 3, 0 );
      for( size_t z = 0; z < dim[ 2 ]; ++z ) {
        for( size_t y = 0; y < dim[ 1 ]; ++y ) {
          for( size_t x = 0; x < dim[ 0 ]; ++x ) {
            COMMENT( "Checking and updating new image dimensions.", 3 );
            if( img( x, y, z ) != 0 ) {
              if( low_coord( 0 ) > x )
                low_coord( 0 ) = x;
              if( low_coord( 1 ) > y )
                low_coord( 1 ) = y;
              if( low_coord( 2 ) > z )
                low_coord( 2 ) = z;
              if( hgh_coord( 0 ) < x )
                hgh_coord( 0 ) = x;
              if( hgh_coord( 1 ) < y )
                hgh_coord( 1 ) = y;
              if( hgh_coord( 2 ) < z )
                hgh_coord( 2 ) = z;
            }
          }
        }
      }
      return( std::make_tuple( low_coord, hgh_coord ) );
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

#ifdef BIAL_EXPLICIT_ImageROI

  template std::tuple< Vector< size_t >, Vector< size_t > > ImageOp::ROI( const Image< int > &img );
  template std::tuple< Vector< size_t >, Vector< size_t > > ImageOp::ROI( const Image< llint > &img );
  template std::tuple< Vector< size_t >, Vector< size_t > > ImageOp::ROI( const Image< float > &img );
  template std::tuple< Vector< size_t >, Vector< size_t > > ImageOp::ROI( const Image< double > &img );

#endif

}

#endif

#endif
