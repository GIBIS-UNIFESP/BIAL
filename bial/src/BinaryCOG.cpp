/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2012/Oct/18 */
/* Version: 1.0.00 */
/* Content: COG Function. */
/* Description: Function to compute COG of binary image. */
/* Future add-on's: many. */

#ifndef BIALBINARYCOG_C
#define BIALBINARYCOG_C

#include "BinaryCOG.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_BinaryCOG )
#define BIAL_EXPLICIT_BinaryCOG
#endif

#if defined ( BIAL_EXPLICIT_BinaryCOG ) || ( BIAL_IMPLICIT_BIN )

#include "Image.hpp"
#include "Vector.hpp"

namespace Bial {

  namespace Binary {

    template< class D >
    Vector< size_t > COG( const Image< D > &img ) {
      try {
        COMMENT( "Creating resultant coordinate vector.", 0 );
        Vector< size_t > result( img.Dims( ), 0 );

        COMMENT( "Counting the number of elements.", 0 );
        size_t elements = 0;
        for( size_t pxl = 0; pxl < img.Size( ); ++pxl ) {
          if( img[ pxl ] != 0.0 ) {
            Vector< size_t > pixel_coords = img.Coordinates( pxl );
            for( size_t dms = 0; dms < pixel_coords.size( ); dms++ ) {
              result( dms ) += pixel_coords( dms );
            }
            ++elements;
          }
        }
        if( elements == 0 ) {
          COMMENT( "If no element return the median coordinate.", 0 );
          for( size_t dms = 0; dms < img.Dims( ); ++dms ) {
            result( dms ) = img.size( dms ) / 2;
          }
        }
        else {
          COMMENT( "Else, return the mean among the element coordinates.", 0 );
          for( size_t dms = 0; dms < result.size( ); dms++ ) {
            result( dms ) /= elements;
          }
        }
        return( result );
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

#ifdef BIAL_EXPLICIT_BinaryCOG

    template Vector< size_t > COG( const Image< int > &img );
    template Vector< size_t > COG( const Image< llint > &img );
    template Vector< size_t > COG( const Image< float > &img );
    template Vector< size_t > COG( const Image< double > &img );

#endif

  }

}

#endif

#endif
