#include "DescriptionUNSER.hpp"
#include "AdjacencyRound.hpp"
#include "Signal.hpp"

namespace Bial {
  uchar UNSER::LinearNormalize( double value, double n ) {
    try {
      value = value / n;
      return( 255.0 * value );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  float UNSER::Mean( const Vector< float > &s ) {
    try {
      float mean = 0.0;
      size_t size = s.size( );
      for( size_t i = 0; i < size; ++i )
        mean += i * s[ i ];
      return( mean * 0.5 );
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

  float UNSER::Contrast( const Vector< float > &d ) {
    try {
      float contrast = 0.0;
      for( int i = -255; i <= 255; ++i )
        contrast += i * i * d[ i + 255 ];
      return( contrast );
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

  float UNSER::Correlation( const Vector< float > &s, float mean, float contrast ) {
    try {
      float correlation = 0.0;
      size_t size = s.size( );
      for( size_t i = 0; i < size; ++i ) {
        float aux = i - 2.0 * mean;
        correlation += aux * aux * s[ i ];
      }
      correlation -= contrast;
      correlation *= 0.5;
      return( correlation );
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

  float UNSER::Energy( const Vector< float > &s, const Vector< float > &d ) {
    try {
      float energy_s = 0.0;
      float energy_d = 0.0;
      size_t size = s.size( );
      for( size_t i = 0; i < size; i++ ) {
        energy_s += s[ i ] * s[ i ];
        energy_d += d[ i ] * d[ i ];
      }
      return( energy_s * energy_d );
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

  float UNSER::Entropy( const Vector< float > &s, const Vector< float > &d ) {
    try {
      float entropy_s = 0.0;
      float entropy_d = 0.0;
      size_t size = s.size( );
      for( size_t i = 0; i < size; i++ ) {
        if( s[ i ] > 0.0 )
          entropy_s += s[ i ] * log10( s[ i ] );
        if( d[ i ] > 0.0 )
          entropy_d += d[ i ] * log10( d[ i ] );
      }
      return( -entropy_s - entropy_d );
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

  float UNSER::Homogeneity( const Vector< float > &d ) {
    try {
      float homogeneity = 0.0;
      for( int j = -255; j <= 255; j++ )
        homogeneity += ( 1.0 / ( 1.0 + ( float ) ( j * j ) ) ) * d[ j + 255 ];
      return( homogeneity );
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

  float UNSER::MaximalProbability( const Vector< float > &s ) {
    try {
      float max = 0.0;
      size_t size = s.size( );
      for( size_t i = 0; i < size; ++i ) {
        if( max < s[ i ] )
          max = s[ i ];
      }
      return( max );
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

  float UNSER::StandardDeviation( float contrast, float correlation ) {
    return( std::sqrt( correlation + contrast ) );
  }


  UNSER::UNSER( FeatureDetector< int > *Fd ) try : UNSER( Fd->Run( ) ) {
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

  UNSER::UNSER( const Vector< std::tuple< Image< int >, Vector< size_t > > > &detected ) try : 
    FeatureExtractor< int, int >( detected ) {
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


  void UNSER::SetParameters( ParameterInterpreter *interpreter ) {
  }

  std::string UNSER::GetParameters( ParameterInterpreter *interpreter ) {
    return( "" );
  }

  UNSERfeature UNSER::Run( ) {
    try {
      const size_t size = 32;
      UNSERfeature feat;
      for( size_t i = 0; i < this->detected.size( ); ++i ) {
        const Image< int > &img( std::get< 0 >( detected[ i ] ) );
        const Vector< size_t > &mask( std::get< 1 >( detected[ i ] ) );
        size_t x_size = img.size( 0 );
        size_t mask_size = mask.size( );
        Vector< bool > mask_vct( img.size( ), false );
        for( size_t pxl_idx = 0; pxl_idx < mask_size; ++pxl_idx )      
          mask_vct[ mask[ pxl_idx ] ] = true;
        IF_DEBUG( mask_size == 0 ) {
          std::string msg( BIAL_ERROR( "Empty mask. " ) );
          throw( std::logic_error( msg ) );
        }
        COMMENT( "Computation of dif and sum------------------------------------------------", 3 );
        Adjacency adjpixels( AdjacencyType::Circular( 1.5f ) );
        size_t adj_size = adjpixels.size( );
        Vector< Vector< float > > sum( 4 );
        for( size_t j = 0; j < sum.size( ); ++j )
          sum[ j ] = Vector< float >( 511 );
        Vector< Vector< float > > dif( 4 );
        for( size_t j = 0; j < dif.size( ); ++j )
          dif[ j ] = Vector< float >( 511 );
        for( size_t pxl_idx = 0; pxl_idx < mask_size; ++pxl_idx ) {
          size_t src_pxl = mask[ pxl_idx ];
          div_t pxl_coords = std::div( static_cast< int >( src_pxl ), static_cast< int >( x_size ) );
          size_t x = pxl_coords.rem;
          size_t y = pxl_coords.quot;
          for( size_t pos = 1; pos <= adj_size >> 1; ++pos ) {
            size_t adj_elm = ( pos + 3 ) % 5 + 1;
            size_t x_dsp;
            if( pos == 1 )
              x_dsp = x - adjpixels( adj_elm, 0 );
            else
              x_dsp = x + adjpixels( adj_elm, 0 );
            size_t y_dsp = y + adjpixels( adj_elm, 1 );
            size_t dsp_pxl = x_dsp + y_dsp * x_size;
            if( img.ValidCoordinate( x_dsp, y_dsp ) && ( mask_vct[ dsp_pxl ] ) ) {
              sum[ pos - 1 ] [ img[ src_pxl ] + img[ dsp_pxl ] ] += 1.0;
              dif[ pos - 1 ] [ img[ src_pxl ] - img[ dsp_pxl ] + 255 ] += 1.0;
            }
          }
        }
        for( size_t j = 0; j < 4; j++ ) {
          for( size_t k = 0; k < 511; k++ ) {
            sum[ j ][ k ] /= ( float ) mask_size;
            dif[ j ][ k ] /= ( float ) mask_size;
          }
        }
        COMMENT( "Histogram------------------------------------------------", 3 );
        Features< int > histogram( size );
        for( size_t j = 0; j < sum.size( ); j++ ) {
          float mean = Mean( sum[ j ] );
          histogram[ j * 8 + 0 ] = mean;
          float contrast = Contrast( dif[ j ] );
          histogram[ j * 8 + 1 ] = contrast / 255.0;
          float correlation = Correlation( sum[ j ], mean, contrast );
          histogram[ j * 8 + 2 ] = ( correlation + 32512.5 ) / 255.0;
          histogram[ j * 8 + 3 ] = 255.0 * Energy( sum[ j ], dif[ j ] );
          histogram[ j * 8 + 4 ] = 255.0 * Entropy( sum[ j ], dif[ j ] ) / 5.4168418;
          histogram[ j * 8 + 5 ] = 255.0 * Homogeneity( dif[ j ] );
          histogram[ j * 8 + 6 ] = 255.0 * MaximalProbability( sum[ j ] );
          histogram[ j * 8 + 7 ] = std::sqrt( 2 ) * StandardDeviation( contrast, correlation );
        }
        for( size_t j = 0; j < size; j++ )
          histogram[ j ] = LinearNormalize( histogram[ j ], 255 );
        feat.push_back( histogram );
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
