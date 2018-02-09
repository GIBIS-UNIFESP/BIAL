/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2017/Mar/28
 * @brief Image class declaration. Handles 2D and 3D images.
 */

#ifndef BIALFOURIER_C
#define BIALFOURIER_C

#include "Fourier.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_Fourier )
#define BIAL_EXPLICIT_Fourier
#endif

#if defined ( BIAL_EXPLICIT_Fourier ) || ( BIAL_IMPLICIT_BIN )

#include "Image.hpp"

namespace Bial {

  Image< bial_complex > FFT( const Image< bial_complex > &img, bool forward_transform ) {
    try {
      size_t img_dims = img.Dims( );
      size_t x_size = img.size( 0 );
      size_t y_size = img.size( 1 );
      size_t z_size = img.size( 2 );
      IF_DEBUG( ( x_size < 1 ) || ( y_size < 1 ) || ( ( img_dims == 3 ) && ( z_size < 1 ) ) ) {
        std::string msg( BIAL_ERROR( "Image must have more than one pixel per dimension. " ) );
        throw( std::runtime_error( msg ) );
      }
      Image< bial_complex > padded_img;
      if( ( forward_transform ) && ( ( std::pow( static_cast< int >( std::sqrt( x_size ) ), 2 ) != x_size ) || 
                                     ( std::pow( static_cast< int >( std::sqrt( y_size ) ), 2 ) != y_size ) ||
                                     ( ( std::pow( static_cast< int >( std::sqrt( z_size ) ), 2 ) != z_size )
                                       && ( img_dims == 3 ) ) ) ) {
        COMMENT( "Zero padding image, since its dimensions are not powers of 2.", 0 );
        padded_img = Image< bial_complex >( ZeroPad( img ) );
      }
      else {
        COMMENT( "Keeping original image. No need to paddle zeros.", 0 );
        padded_img = img;
      }
      x_size = padded_img.size( 0 );
      y_size = padded_img.size( 1 );
      z_size = padded_img.size( 2 );
      COMMENT( "Applaying FFT to the image rows. Dimensions: " << x_size << ", " << y_size << ", " << z_size, 0 );
      Vector< bial_complex > aux( x_size );
      for( size_t z = 0; z < z_size; ++z ) {
        for( size_t y = 0; y < y_size; ++y ) {
          for( size_t x = 0; x < x_size; ++x )
            aux[ x ] = padded_img( x, y, z );
          FFT( aux, forward_transform );
          for( size_t x = 0; x < x_size; ++x )
            padded_img( x, y, z ) = aux[ x ];
        }
      }
      COMMENT( "Applaying FFT to the image columns.", 0 );
      aux = Vector< bial_complex >( y_size );
      for( size_t z = 0; z < z_size; ++z ) {
        for( size_t x = 0; x < x_size; ++x ) {
          for( size_t y = 0; y < y_size; ++y )
            aux[ y ] = padded_img( x, y, z );
          FFT( aux, forward_transform );
          for( size_t y = 0; y < y_size; ++y )
            padded_img( x, y, z ) = aux[ y ];
        }
      }
      if( img_dims == 3 ) {
        COMMENT( "Applaying FFT to the image depth.", 0 );
        aux = Vector< bial_complex >( z_size );
        for( size_t x = 0; x < x_size; ++x ) {
          for( size_t y = 0; y < y_size; ++y ) {
            for( size_t z = 0; z < z_size; ++z )
              aux[ z ] = padded_img( x, y, z );
            FFT( aux, forward_transform );
            for( size_t z = 0; z < z_size; ++z )
              padded_img( x, y, z ) = aux[ z ];
          }
        }
      }
      return( padded_img );
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
  
  void FFT( Vector< bial_complex > &aux, bool forward_transform ) {
    try {
      COMMENT( "Getting the number of points.", 0 );
      size_t vct_size = aux.size( );
      size_t vct_size_power = std::log2( vct_size );
      COMMENT( "Sort input vector elements such that they increase by counting their indexes considering " << 
               "the least significant bit as the most significant one, and vice-versa.", 0 );
      size_t vct_size_2 = vct_size >> 1;
      size_t chg_elm = 0;
      for( size_t elm = 0; elm < vct_size - 1; ++elm ) {
        //std::cout << "elm: " << elm << "\\" << vct_size - 1 << std::endl;
        if( elm < chg_elm )
          std::swap( aux[ elm ], aux[ chg_elm ] );
        size_t vct_factor = vct_size_2;
        while( vct_factor <= chg_elm ) {
          chg_elm -= vct_factor;
          vct_factor >>= 1;
          //std::cout << "chg_elm: " << chg_elm << ", vct_factor: " << vct_factor << std::endl;
        }
        chg_elm += vct_factor;
      }
      COMMENT( "Computing the FFT.", 0 );
      bial_complex fft_factor_chg( -1.0, 0.0 ); // Changes in FFT factor for every jump in a series.
      size_t adj_series_jump = 1; // Jump size to the next element to be considered in the series.
      size_t adj_series_size; // Number of parts that the series have.
      bial_complex diff_val; // Difference between compared src and tgt elements of a step in a series.
      for( size_t series = 0; series < vct_size_power; ++series ) {
        adj_series_size = adj_series_jump;
        adj_series_jump <<= 1;
        bial_complex fft_factor( 1.0, 0.0 ); // FFT factor used in a jump of a series.
        for( size_t step_elm = 0; step_elm < adj_series_size; ++step_elm ) {
          for( size_t src_elm = step_elm; src_elm < vct_size; src_elm += adj_series_jump ) {
            size_t tgt_elm = src_elm + adj_series_size;
            diff_val.real( fft_factor.real( ) * aux[ tgt_elm ].real( ) - fft_factor.imag( ) * aux[ tgt_elm ].imag( ) );
            diff_val.imag( fft_factor.imag( ) * aux[ tgt_elm ].real( ) + fft_factor.real( ) * aux[ tgt_elm ].imag( ) );
            aux[ tgt_elm ] = aux[ src_elm ] - diff_val;
            aux[ src_elm ] += diff_val;
          }
          double aux =  fft_factor.real( ) * fft_factor_chg.real( ) -
            fft_factor.imag( ) * fft_factor_chg.imag( );
          fft_factor.imag( fft_factor.real( ) * fft_factor_chg.imag( ) +
                           fft_factor.imag( ) * fft_factor_chg.real( ) );
          fft_factor.real( aux );
        }
        fft_factor_chg.imag( sqrt( ( 1.0 - fft_factor_chg.real( ) ) / 2.0 ) );
        if( !forward_transform )
          fft_factor_chg.imag( -fft_factor_chg.imag( ) );
        fft_factor_chg.real( sqrt( ( 1.0 + fft_factor_chg.real( ) ) / 2.0 ) );
      }
      COMMENT( "Applying scaling factor for the inverse transform.", 0 );
      if( !forward_transform ) {
        for( size_t elm = 0; elm < vct_size; ++elm )
          aux[ elm ] /= static_cast< double >( vct_size );
      }
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
  Image< D > ZeroPad( const Image< D > &img ) {
    try {
      size_t dims = img.Dims( );
      Vector< size_t > src_size( img.Dim( ) );
      Vector< size_t > tgt_size( 3, 1 );
      COMMENT( "Computing target dimensions with the closest power of 2 that is higher or equal to current size.", 0 );
      for( size_t dms = 0; dms < dims; ++dms )
        tgt_size[ dms ] = static_cast< size_t >( std::pow( 2.0, std::ceil( std::log2( src_size[ dms ] ) ) ) );
      COMMENT( "Creating padded image and copying source image contents. Dimensions: " << tgt_size, 0 );
      Image< D > res( tgt_size );
      Vector< size_t > size_diff_2( ( tgt_size - src_size ) / 2 );
      for( size_t src_z = 0; src_z < src_size[ 2 ]; ++src_z ) {
        size_t tgt_z = src_z + size_diff_2[ 2 ];
        for( size_t src_y = 0; src_y < src_size[ 1 ]; ++src_y ) {
          size_t tgt_y = src_y + size_diff_2[ 1 ];
          for( size_t src_x = 0; src_x < src_size[ 0 ]; ++src_x ) {
            size_t tgt_x = src_x + size_diff_2[ 0 ];
            res( tgt_x, tgt_y, tgt_z ) = img( src_x, src_y, src_z );
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

  template< class D >
  void ZeroUnPad( const Image< D > &src_img, Image< D > &tgt_img ) {
    try {
      size_t dims = src_img.Dims( );
      Vector< size_t > src_size( src_img.Dim( ) );
      Vector< size_t > tgt_size( tgt_img.Dim( ) );
      IF_DEBUG( dims != tgt_img.Dims( ) ) {
        std::string msg( BIAL_ERROR( "Images must have the same number of dimensions. Given src_img dims: "
                                     + std::to_string( dims ) + ", tgt_img dims: "
                                     + std::to_string( tgt_img.Dims( ) ) ) );
        throw( std::runtime_error( msg ) );
      }
      IF_DEBUG( ( src_size[ 0 ] < tgt_size[ 0 ] ) || ( src_size[ 1 ] < tgt_size[ 1 ] ) || 
                ( src_size[ 2 ] < tgt_size[ 2 ] ) ) {
        std::string msg( BIAL_ERROR( "Src_img must have larger dimensions than tgt_img." ) );
        throw( std::runtime_error( msg ) );
      }
      COMMENT( "Copying source image contents.", 0 );
      Vector< size_t > size_diff_2( ( src_size - tgt_size ) / 2 );
      for( size_t tgt_z = 0; tgt_z < tgt_size[ 2 ]; ++tgt_z ) {
        size_t src_z = tgt_z + size_diff_2[ 2 ];
        for( size_t tgt_y = 0; tgt_y < tgt_size[ 1 ]; ++tgt_y ) {
          size_t src_y = tgt_y + size_diff_2[ 1 ];
          for( size_t tgt_x = 0; tgt_x < tgt_size[ 0 ]; ++tgt_x ) {
            size_t src_x = tgt_x + size_diff_2[ 0 ];
            tgt_img( tgt_x, tgt_y, tgt_z ) = src_img( src_x, src_y, src_z );
          }
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

#ifdef BIAL_EXPLICIT_Fourier

  template Image< int > ZeroPad( const Image< int > &img );
  template void ZeroUnPad( const Image< int > &src_img, Image< int > &tgt_img );
  template Image< llint > ZeroPad( const Image< llint > &img );
  template void ZeroUnPad( const Image< llint > &src_img, Image< llint > &tgt_img );
  template Image< float > ZeroPad( const Image< float > &img );
  template void ZeroUnPad( const Image< float > &src_img, Image< float > &tgt_img );
  template Image< double > ZeroPad( const Image< double > &img );
  template void ZeroUnPad( const Image< double > &src_img, Image< double > &tgt_img );
  template Image< bial_complex > ZeroPad( const Image< bial_complex > &img );
  template void ZeroUnPad( const Image< bial_complex > &src_img, Image< bial_complex > &tgt_img );
  
#endif

}

#endif

#endif
