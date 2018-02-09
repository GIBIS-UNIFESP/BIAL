/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2013/Oct/29
 * @brief Operations over color channels.
 */

#ifndef BIALCOLORCHANNEL_C
#define BIALCOLORCHANNEL_C

#include "ColorChannel.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_ColorChannel )
#define BIAL_EXPLICIT_ColorChannel
#endif

#if defined ( BIAL_EXPLICIT_ColorChannel ) || ( BIAL_IMPLICIT_BIN )

#include "Color.hpp"
#include "Image.hpp"
#include "RealColor.hpp"

namespace Bial {

  namespace ColorSpace {

    template< class D >
    Image< Color > Merge( Vector< Image< D > > &img_vector ) {
      try {
        COMMENT( "Checking if vector is not empty.", 2 );
        if( ( img_vector.size( ) < 3 ) || ( img_vector.size( ) > 4 ) ) {
          std::string msg( BIAL_ERROR( "img_vector must have 3 or 3 dimensions." ) );
          throw( std::logic_error( msg ) );
        }
        COMMENT( "Checking if images have the same dimensions.", 2 );
        for( size_t i = 1; i < img_vector.size( ); ++i ) {
          if( img_vector[ 0 ].Dims( ) != img_vector[ i ].Dims( ) ) {
            std::string msg( BIAL_ERROR( "All images must have the same dimensions." ) );
            throw( std::logic_error( msg ) );
          }
        }
        COMMENT( "Setting the parameters for the constructor.", 2 );
        Vector< size_t > spc_dims( img_vector[ 0 ].Dim( ) );
        Vector< float > pixel_size( img_vector[ 0 ].PixelSize( ) );
        size_t channels = img_vector.size( );
        Image< Color > res( spc_dims, pixel_size );
        COMMENT( "Computing spatial size.", 2 );
        size_t spc_size = spc_dims[ 0 ];
        for( size_t dms = 1; dms < spc_dims.size( ); ++dms )
          spc_size *= spc_dims[ dms ];
        COMMENT( "Copying image contents.", 2 );
        for( size_t chn = 0; chn < channels; ++chn ) {
          for( size_t pxl = 0; pxl < spc_size; ++pxl )
            res( pxl )( chn ) = img_vector( chn )( pxl );
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

    template< class D >
    Vector< Image< D > > Split( const Image< Color > &img ) {
      try {
        COMMENT( "Creating vector image.", 2 );
        Vector< Image< D > > res( 4, Image< D >( img.Dim( ), img.PixelSize( ) ) );
        COMMENT( "Assigning split image data.", 2 );
        for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
          for( size_t chl = 0; chl < 4; ++chl )
            res( chl )[ pxl ] = img( pxl )( chl );
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


    template< class D >
    Image< D > Channel( const Image< Color > &img, size_t channel ) {
      try {
        if( channel >= 4 ) {
          std::string msg( BIAL_ERROR( "Invalid channel. Expected: <= 3. Given: " + std::to_string( channel ) ) );
          throw( std::runtime_error( msg ) );
        }
        Image< D > res( img.Dim( ), img.PixelSize( ) );
        size_t size = img.size( );
        for( size_t pxl = 0; pxl < size; ++pxl )
          res( pxl ) = img( pxl )( channel );
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


    template< class D >
    Image< D > Channel( const Image< RealColor > &img, size_t channel ) {
      try {
        if( channel >= 4 ) {
          std::string msg( BIAL_ERROR( "Invalid channel. Expected: <= 3. Given: " + std::to_string( channel ) ) );
          throw( std::runtime_error( msg ) );
        }
        Image< D > res( img.Dim( ), img.PixelSize( ) );
        size_t size = img.size( );
        for( size_t pxl = 0; pxl < size; ++pxl )
          res( pxl ) = img( pxl )( channel );
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

#ifdef BIAL_EXPLICIT_ColorChannel

    template Image< Color > Merge( Vector< Image< int > > &img_vector );
    template Vector< Image< int > > Split( const Image< Color > &img );
    template Image< Color > Merge( Vector< Image< llint > > &img_vector );
    template Vector< Image< llint > > Split( const Image< Color > &img );
    template Image< Color > Merge( Vector< Image< float > > &img_vector );
    template Vector< Image< float > > Split( const Image< Color > &img );
    template Image< Color > Merge( Vector< Image< double > > &img_vector );
    template Vector< Image< double > > Split( const Image< Color > &img );

    template Image< int > Channel( const Image< Color > &img, size_t channel );
    template Image< int > Channel( const Image< RealColor > &img, size_t channel );
    template Image< llint > Channel( const Image< Color > &img, size_t channel );
    template Image< llint > Channel( const Image< RealColor > &img, size_t channel );
    template Image< float > Channel( const Image< Color > &img, size_t channel );
    template Image< float > Channel( const Image< RealColor > &img, size_t channel );
    template Image< double > Channel( const Image< Color > &img, size_t channel );
    template Image< double > Channel( const Image< RealColor > &img, size_t channel );

#endif

  }

}

#endif

#endif
