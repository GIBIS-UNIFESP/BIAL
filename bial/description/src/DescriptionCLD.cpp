#include "DescriptionCLD.hpp"
#include "Signal.hpp"

namespace Bial {
  const unsigned short int CLD::quantbits[ DCTSIZE2 ] = {
    6, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5
  };

/* This is the zigzag scan pattern. */
  const unsigned short int CLD::zigzag[ DCTSIZE2 ] = {
    0, 1, 8, 16, 9, 2, 3, 10,
    17, 24, 32, 25, 18, 11, 4, 5,
    12, 19, 26, 33, 40, 48, 41, 34,
    27, 20, 13, 6, 7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36,
    29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46,
    53, 60, 61, 54, 47, 55, 62, 63
  };

  uchar CLD::LinearNormalize( double value, double n ) {
    try {
      value = value / n;
      return( static_cast< uchar >( 255.0 * value ) );
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

  void CLD::DiscreteCosineTransform( Vector< int > &data ) {
    try {
      COMMENT( "Step 1: process rows.", 3 );
      /* Note results are scaled up by sqrt(8) compared to a true DCT; */
      /* furthermore, we scale the results by 2**PASS1_BITS. */
      size_t row = 0;
      for( int ctr = DCTSIZE - 1; ctr >= 0; --ctr ) {
        int tmp0 = data[ row + 0 ] + data[ row + 7 ];
        int tmp7 = data[ row + 0 ] - data[ row + 7 ];
        int tmp1 = data[ row + 1 ] + data[ row + 6 ];
        int tmp6 = data[ row + 1 ] - data[ row + 6 ];
        int tmp2 = data[ row + 2 ] + data[ row + 5 ];
        int tmp5 = data[ row + 2 ] - data[ row + 5 ];
        int tmp3 = data[ row + 3 ] + data[ row + 4 ];
        int tmp4 = data[ row + 3 ] - data[ row + 4 ];
        /* Even part per LL&M figure 1 --- note that published figure is faulty;
         * rotator "sqrt(2)*c1" should be "sqrt(2)*c6".
         */
        int tmp10 = tmp0 + tmp3;
        int tmp13 = tmp0 - tmp3;
        int tmp11 = tmp1 + tmp2;
        int tmp12 = tmp1 - tmp2;
        data[ row + 0 ] = ( tmp10 + tmp11 ) << PASS1_BITS;
        data[ row + 4 ] = ( tmp10 - tmp11 ) << PASS1_BITS;
        int z1 = MULTIPLY( tmp12 + tmp13, FIX_0_541196100 );
        data[ row + 2 ] = DESCALE( z1 + MULTIPLY( tmp13, FIX_0_765366865 ), CONST_BITS - PASS1_BITS );
        data[ row + 6 ] = DESCALE( z1 + MULTIPLY( tmp12, -FIX_1_847759065 ), CONST_BITS - PASS1_BITS );
        /* Odd part per figure 8 --- note paper omits factor of sqrt(2).
         * cK represents cos(K*pi/16).
         * i0..i3 in the paper are tmp4..tmp7 here.
         */
        z1 = tmp4 + tmp7;
        int z2 = tmp5 + tmp6;
        int z3 = tmp4 + tmp6;
        int z4 = tmp5 + tmp7;
        int z5 = MULTIPLY( z3 + z4, FIX_1_175875602 );                /* sqrt(2) * c3 */
        tmp4 = MULTIPLY( tmp4, FIX_0_298631336 );                /* sqrt(2) * (-c1+c3+c5-c7) */
        tmp5 = MULTIPLY( tmp5, FIX_2_053119869 );                /* sqrt(2) * ( c1+c3-c5+c7) */
        tmp6 = MULTIPLY( tmp6, FIX_3_072711026 );                /* sqrt(2) * ( c1+c3+c5-c7) */
        tmp7 = MULTIPLY( tmp7, FIX_1_501321110 );                /* sqrt(2) * ( c1+c3-c5-c7) */
        z1 = MULTIPLY( z1, -FIX_0_899976223 );                /* sqrt(2) * (c7-c3) */
        z2 = MULTIPLY( z2, -FIX_2_562915447 );                /* sqrt(2) * (-c1-c3) */
        z3 = MULTIPLY( z3, -FIX_1_961570560 );                /* sqrt(2) * (-c3-c5) */
        z4 = MULTIPLY( z4, -FIX_0_390180644 );                /* sqrt(2) * (c5-c3) */
        z3 += z5;
        z4 += z5;
        data[ row + 7 ] = DESCALE( tmp4 + z1 + z3, CONST_BITS - PASS1_BITS );
        data[ row + 5 ] = DESCALE( tmp5 + z2 + z4, CONST_BITS - PASS1_BITS );
        data[ row + 3 ] = DESCALE( tmp6 + z2 + z3, CONST_BITS - PASS1_BITS );
        data[ row + 1 ] = DESCALE( tmp7 + z1 + z4, CONST_BITS - PASS1_BITS );
        row += DCTSIZE;                /* advance to next row */
      }
      COMMENT( "Step 2: process columns.", 3 );
      /* We remove the PASS1_BITS scaling, but leave the results scaled up
       * by an overall factor of 8.
       */
      int col = 0;
      for( int ctr = DCTSIZE - 1; ctr >= 0; --ctr ) {
        int tmp0 = data[ col + DCTSIZE * 0 ] + data[ col + DCTSIZE * 7 ];
        int tmp7 = data[ col + DCTSIZE * 0 ] - data[ col + DCTSIZE * 7 ];
        int tmp1 = data[ col + DCTSIZE * 1 ] + data[ col + DCTSIZE * 6 ];
        int tmp6 = data[ col + DCTSIZE * 1 ] - data[ col + DCTSIZE * 6 ];
        int tmp2 = data[ col + DCTSIZE * 2 ] + data[ col + DCTSIZE * 5 ];
        int tmp5 = data[ col + DCTSIZE * 2 ] - data[ col + DCTSIZE * 5 ];
        int tmp3 = data[ col + DCTSIZE * 3 ] + data[ col + DCTSIZE * 4 ];
        int tmp4 = data[ col + DCTSIZE * 3 ] - data[ col + DCTSIZE * 4 ];
        /* Even part per LL&M figure 1 --- note that published figure is faulty;
         * rotator "sqrt(2)*c1" should be "sqrt(2)*c6".
         */
        int tmp10 = tmp0 + tmp3;
        int tmp13 = tmp0 - tmp3;
        int tmp11 = tmp1 + tmp2;
        int tmp12 = tmp1 - tmp2;
        data[ col + DCTSIZE * 0 ] = DESCALE( tmp10 + tmp11, PASS1_BITS );
        data[ col + DCTSIZE * 4 ] = DESCALE( tmp10 - tmp11, PASS1_BITS );
        int z1 = MULTIPLY( tmp12 + tmp13, FIX_0_541196100 );
        data[ col + DCTSIZE * 2 ] = DESCALE( z1 + MULTIPLY( tmp13, FIX_0_765366865 ), CONST_BITS + PASS1_BITS );
        data[ col + DCTSIZE * 6 ] = DESCALE( z1 + MULTIPLY( tmp12, -FIX_1_847759065 ), CONST_BITS + PASS1_BITS );
        /* Odd part per figure 8 --- note paper omits factor of sqrt(2).
         * cK represents cos(K*pi/16).
         * i0..i3 in the paper are tmp4..tmp7 here.
         */
        z1 = tmp4 + tmp7;
        int z2 = tmp5 + tmp6;
        int z3 = tmp4 + tmp6;
        int z4 = tmp5 + tmp7;
        int z5 = MULTIPLY( z3 + z4, FIX_1_175875602 );                /* sqrt(2) * c3 */
        tmp4 = MULTIPLY( tmp4, FIX_0_298631336 );                /* sqrt(2) * (-c1+c3+c5-c7) */
        tmp5 = MULTIPLY( tmp5, FIX_2_053119869 );                /* sqrt(2) * ( c1+c3-c5+c7) */
        tmp6 = MULTIPLY( tmp6, FIX_3_072711026 );                /* sqrt(2) * ( c1+c3+c5-c7) */
        tmp7 = MULTIPLY( tmp7, FIX_1_501321110 );                /* sqrt(2) * ( c1+c3-c5-c7) */
        z1 = MULTIPLY( z1, -FIX_0_899976223 );                /* sqrt(2) * (c7-c3) */
        z2 = MULTIPLY( z2, -FIX_2_562915447 );                /* sqrt(2) * (-c1-c3) */
        z3 = MULTIPLY( z3, -FIX_1_961570560 );                /* sqrt(2) * (-c3-c5) */
        z4 = MULTIPLY( z4, -FIX_0_390180644 );                /* sqrt(2) * (c5-c3) */
        z3 += z5;
        z4 += z5;
        data[ col + DCTSIZE * 7 ] = DESCALE( tmp4 + z1 + z3, CONST_BITS + PASS1_BITS );
        data[ col + DCTSIZE * 5 ] = DESCALE( tmp5 + z2 + z4, CONST_BITS + PASS1_BITS );
        data[ col + DCTSIZE * 3 ] = DESCALE( tmp6 + z2 + z3, CONST_BITS + PASS1_BITS );
        data[ col + DCTSIZE * 1 ] = DESCALE( tmp7 + z1 + z4, CONST_BITS + PASS1_BITS );
        ++col;                /* advance pointer to next column */
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

  void CLD::QuantizeCoefficients( Vector< int > &data ) {
    try {
      for( size_t i = 0; i < DCTSIZE2; ++i ) {
        int qval = ( LEFT_SHIFT( ONE, 8 - quantbits[ i ] + 1 ) ) << 3;
        int temp = data[ i ];
        COMMENT( "Divide the coefficient value by qval, ensuring proper rounding.", 3 );
        /* Since C does not specify the direction of rounding for negative
         * quotients, we have to force the dividend positive for portability.
         *
         * In most files, at least half of the output values will be zero
         * (at default quantization settings, more like three-quarters...)
         * so we should ensure that this case is fast.  On many machines,
         * a comparison is enough cheaper than a divide to make a special test
         * a win.  Since both inputs will be nonnegative, we need only test
         * for a < b to discover whether a/b is 0.
         */
#define DIVIDE_BY( a, b ) if( a >= b ) { a /= b;        \
        }                                               \
        else a = 0
        if( temp < 0 ) {
          temp = -temp;
          temp += qval >> 1;                /* for rounding */
          DIVIDE_BY( temp, qval );
          temp = -temp;
        }
        else {
          temp += qval >> 1;                /* for rounding */
          DIVIDE_BY( temp, qval );
        }
        temp += LEFT_SHIFT( ONE, quantbits[ i ] - 1 );
        data[ i ] = temp;
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

  CLD::CLD( FeatureDetector< Color > *Fd ) try : CLD( Fd->Run( ) ) {
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

  CLD::CLD( const Vector< std::tuple< Image< Color >, Vector< size_t > > > &detected ) try : 
    FeatureExtractor< Color, int >( detected ) {
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

  void CLD::SetParameters( ParameterInterpreter *interpreter ) {
  }

  std::string CLD::GetParameters( ParameterInterpreter *interpreter ) {
    return( "" );
  }

  CLDfeature CLD::Run( ) {
    try {
      size_t size = 16;
      CLDfeature feat;
      for( size_t i = 0; i < this->detected.size( ); ++i ) {
        const Image< Color > &img( std::get< 0 >( detected[ i ] ) );
        const Vector< size_t > &mask( std::get< 1 >( detected[ i ] ) );
        size_t x_size = img.size( 0 );
        size_t y_size = img.size( 1 );
        size_t mask_size = mask.size( );
        if( mask_size == 0 ) {
          std::string msg( BIAL_ERROR( "Empty mask. " ) );
          throw( std::logic_error( msg ) );
        }
        COMMENT( "property -----------------------------------------------------------------", 3 );
        Vector< int > property_y( DCTSIZE2 );
        Vector< int > property_cb( DCTSIZE2 );
        Vector< int > property_cr( DCTSIZE2 );
        Vector< int > property_n( DCTSIZE2 );
        for( size_t pxl_idx = 0; pxl_idx < mask_size; ++pxl_idx ) {
          size_t src_pxl = mask[ pxl_idx ];
          div_t pxl_coords = std::div( static_cast< int >( src_pxl ), static_cast< int >( x_size ) );
          size_t n = pxl_coords.rem;
          size_t m = pxl_coords.quot;
          int x = DCTSIZE * m / y_size;
          int y = DCTSIZE * n / x_size;
          int k = x + DCTSIZE * y;
          int r = img[ src_pxl ].channel[ 1 ];
          int g = img[ src_pxl ].channel[ 2 ];
          int b = img[ src_pxl ].channel[ 3 ];
          property_y[ k ] += ( ulong ) ( 0.29900 * r + 0.58700 * g + 0.11400 * b );
          property_cb[ k ] += ( ulong ) ( -0.16900 * r - 0.33100 * g + 0.50000 * b + 128 );
          property_cr[ k ] += ( ulong ) ( 0.50000 * r - 0.41900 * g - 0.08100 * b + 128 );
          ++property_n[ k ];
        }
        COMMENT( "coefficient---------------------------------------------------------------", 3 );
        for( size_t j = 0; j < DCTSIZE2; ++j ) {
          property_y [ j ] = property_y  [ j ] / property_n[ j ] - 128;
          property_cb[ j ] = property_cb [ j ] / property_n[ j ] - 128;
          property_cr[ j ] = property_cr [ j ] / property_n[ j ] - 128;
        }
        DiscreteCosineTransform( property_y );
        QuantizeCoefficients( property_y );
        DiscreteCosineTransform( property_cb );
        QuantizeCoefficients( property_cb );
        DiscreteCosineTransform( property_cr );
        QuantizeCoefficients( property_cr );
        Features< int > histogram_y( size / 2 );
        Features< int > histogram_cb( size / 4 );
        Features< int > histogram_cr( size / 4 );
        for( size_t j = 0; j < histogram_y.size( ); ++j )
          histogram_y[ j ] = property_y[ zigzag[ j ] ];
        for( size_t j = 0; j < histogram_cb.size( ); ++j )
          histogram_cb[ j ] = property_cb[ zigzag[ j ] ];
        for( size_t j = 0; j < histogram_cr.size( ); ++j )
          histogram_cr[ j ] = property_cr[ zigzag[ j ] ];
        COMMENT( "Normalize coefficients----------------------------------------------------", 3 );
        for( size_t j = 0; j < histogram_y.size( ); ++j )
          histogram_y[ j ] = LinearNormalize( histogram_y[ j ], 255.0 );
        for( size_t j = 0; j < histogram_cb.size( ); ++j )
          histogram_cb[ j ] = LinearNormalize( histogram_cb[ j ], 255.0 );
        for( size_t j = 0; j < histogram_cr.size( ); ++j )
          histogram_cr[ j ] = LinearNormalize( histogram_cr[ j ], 255.0 );
        COMMENT( "histogram-----------------------------------------------------------------", 3 );
        Features< int > complete_histogram( size );
        size_t counter = 0;
        for( size_t j = 0; j < histogram_y.size( ); ++j )
          complete_histogram[ counter++ ] = histogram_y[ j ];
        for( size_t j = 0; j < histogram_cb.size( ); ++j )
          complete_histogram[ counter++ ] = histogram_cb[ j ];
        for( size_t j = 0; j < histogram_cr.size( ); ++j )
          complete_histogram[ counter++ ] = histogram_cr[ j ];
        feat.push_back( complete_histogram );
      }
      return( feat );
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

}
