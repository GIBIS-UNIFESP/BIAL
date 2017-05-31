#include "DescriptionCSD.hpp"
#include "AdjacencyRound.hpp"
#include "Signal.hpp"

namespace Bial {

  CSD::CSD( FeatureDetector< Color > *Fd ) try : CSD( Fd->Run( ) ) {
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


  CSD::CSD( const Vector< std::tuple< Image< Color >, Vector< size_t > > > &detected ) try : 
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

  void CSD::SetParameters( ParameterInterpreter *interpreter ) {
  }

  std::string CSD::GetParameters( ParameterInterpreter *interpreter ) {
    return( "" );
  }

  CSDfeature CSD::Run( ) {
    try {
      size_t size = 64;
      CSDfeature feat;
      for( size_t i = 0; i < this->detected.size( ); ++i ) {
        const Image< Color > &img( std::get< 0 >( detected[ i ] ) );
        const Vector< size_t > &mask( std::get< 1 >( detected[ i ] ) );
        size_t x_size = img.size( 0 );
        size_t y_size = img.size( 1 );
        size_t mask_size = mask.size( );
        size_t img_size = img.size( );
        if( mask_size == 0 ) {
          std::string msg( BIAL_ERROR( "Empty mask. " ) );
          throw( std::logic_error( msg ) );
        }
        COMMENT( "quantização------------------------------------------------", 3 );
        Image< int > quantized( x_size, y_size );
        Image< Color > hmmd( RgbToHmmd( img ) );
        for( size_t j = 0; j < img_size; ++j ) {
          size_t d = 5 * hmmd[ j ].channel[ 3 ] / 256;
          ulong h, s;
          switch( d ) {
          case 0:
            s = 8 * hmmd[ j ].channel[ 2 ] / 256;
            quantized[ j ] = s;
            break;
          case 1:
            h = 4 * hmmd[ j ].channel[ 1 ] / 256;
            s = 4 * hmmd[ j ].channel[ 2 ] / 256;
            quantized[ j ] = 8 + 4 * s + h;
            break;
          case 2:
            h = 6 * hmmd[ j ].channel[ 1 ] / 256;
            s = 4 * hmmd[ j ].channel[ 2 ] / 256;
            quantized[ j ] = 24 + 6 * s + h;
            break;
          case 3:
          case 4:
            h = 4 * hmmd[ j ].channel[ 1 ] / 256;
            s = 4 * hmmd[ j ].channel[ 2 ] / 256;
            quantized[ j ] = 48 + 4 * s + h;
            break;
          }
        }
        COMMENT( "Histograma------------------------------------------------", 3 );
        Features< int > histogram( size );
        Vector< bool > colors( size );
        int k = std::max( 0, ( ( int ) ( std::log( img_size ) / std::log( 2 ) ) ) / 2 - 8 );
        int w = 1 << k;
        int m = 8 * w;
        Adjacency adj( AdjacencyType::AdjacencyBox( m, m ) );
        Vector< bool > mask_vct( img_size );
        for( size_t pxl_idx = 0; pxl_idx < mask_size; ++pxl_idx )
          mask_vct[ mask[ pxl_idx ] ] = true;
        size_t maximum = 0;
        for( size_t y = 0; y <= y_size - m; y += w ) {
          for( size_t x = 0; x <= x_size - m; x += w ) {
            if( mask_vct( x + y * x_size ) == 1 ) {
              for( size_t j = 0; j < size; ++j )
                colors[ j ] = false;
              for( size_t pos = 0; pos < adj.size( ); ++pos ) {
                int x_dsp = x + adj( pos, 0 );
                int y_dsp = y + adj( pos, 1 );
                size_t pxl = x_dsp + y_dsp * x_size;
                if( ( quantized.ValidCoordinate( x_dsp, y_dsp ) ) && ( mask_vct( pxl ) == 1 ) )
                  colors[ quantized[ pxl ] ] = true;
              }
              for( size_t j = 0; j < size; ++j ) {
                if( colors[ j ] )
                  ++histogram[ j ];
              }
              ++maximum;
            }
          }
        }
        for( size_t j = 0; j < size; ++j )
          histogram[ j ] = Log( histogram[ j ], maximum );
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
