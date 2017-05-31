#include "DescriptionCCV.hpp"
#include "AdjacencyRound.hpp"
#include "Signal.hpp"

namespace Bial {


  CCV::CCV( FeatureDetector< Color > *Fd ) try : CCV( Fd->Run( ) ) {
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


  CCV::CCV( const Vector< std::tuple< Image< Color >, Vector< size_t > > > &detected ) try : 
    FeatureExtractor< Color, int >( detected ) {
      this->dim = 4;
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

  void CCV::SetParameters( ParameterInterpreter *interpreter ) {
    try {
      Vector< parameter > vet;
      vet.push_back( std::tie( "dim", dim ) );
      interpreter->SetExpectedParameters( vet );
      vet = interpreter->Interpret( );
      std::tie( std::ignore, dim ) = vet[ 0 ];
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

  std::string CCV::GetParameters( ParameterInterpreter *interpreter ) {
    try {
      Vector< parameter > vet;
      vet.push_back( std::tie( "dim", dim ) );
      interpreter->SetExpectedParameters( vet );
      return( interpreter->GetExpectedParameters( ) );
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

  CCVfeature CCV::Run( ) {
    try {
      size_t size = dim * dim * dim;
      CCVfeature feat;
      uchar fator_g = dim;
      uchar fator_b = fator_g * dim;
      std::queue< int > fila;
      int nlabels = 0;
      COMMENT( "quantização------------------------------------------------", 3 );
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
        Image< int > quantized( x_size, y_size );
        Vector< bool > frequency( img_size );
        int labels[ quantized.size( ) ];
        for( size_t i = 0; i < quantized.size( ); ++i )
          labels[ i ] = NIL;
        for( size_t j = 0; j < quantized.size( ); ++j ) {
          uchar r = dim * img[ j ].channel[ 1 ] / 256;
          uchar g = dim * img[ j ].channel[ 2 ] / 256;
          uchar b = dim * img[ j ].channel[ 3 ] / 256;
          quantized[ j ] = ( r + fator_g * g + fator_b * b );
        }
        COMMENT( "Frequencia dos pixels--------------------------------------", 3 );
        Adjacency adjpixels( AdjacencyType::Circular( 1.1f ) );
        for( size_t pxl_idx = 0; pxl_idx < mask_size; ++pxl_idx ) {
          size_t src_pxl = mask[ pxl_idx ];
          if( labels[ src_pxl ] == NIL ) {
            labels[ src_pxl ] = nlabels++;
            fila.push( src_pxl );
            while( !fila.empty( ) ) {
              src_pxl = fila.front( );
              fila.pop( );
              div_t pxl_coords = std::div( static_cast< int >( src_pxl ), static_cast< int >( x_size ) );
              size_t u_x = pxl_coords.rem;
              size_t u_y = pxl_coords.quot;
              for( size_t pos = 1; pos < adjpixels.size( ); ++pos ) {
                size_t v_x = u_x + adjpixels( pos, 0 );
                size_t v_y = u_y + adjpixels( pos, 1 );
                if( ( v_x < x_size ) && ( v_y < y_size ) ) {
                  size_t q = v_x + v_y * x_size;
                  if( ( labels[ q ] == NIL ) && ( quantized[ src_pxl ] == quantized[ q ] ) ) {
                    labels[ q ] = labels[ src_pxl ];
                    fila.push( q );
                  }
                }
              }
            }
          }
        }
        COMMENT( "Histograma------------------------------------------------", 3 );
        Vector< int > area( nlabels );
        for( int i = 0; i < nlabels; ++i )
          area[ i ] = 0;
        for( size_t p = 0; p < quantized.size( ); ++p )
          ++area[ labels[ p ] ];
        for( size_t p = 0; p < quantized.size( ); ++p ) {
          if( 100 * area[ labels[ p ] ] < static_cast< int >( MIN_AREA * quantized.size( ) ) )
            frequency[ p ] = LOW;
          else
            frequency[ p ] = HIGH;
        }
        Features< int > complete_histogram( 2 * size );
        Features< int > LOW_histogram( size );
        Features< int > HIGH_histogram( size );
        for( size_t j = 0; j < quantized.size( ); j++ ) {
          if( frequency[ j ] == LOW )
            ++LOW_histogram[ quantized[ j ] ];
          else
            ++HIGH_histogram[ quantized[ j ] ];
        }
        for( size_t j = 0; j < size; ++j )
          complete_histogram[ j ] = Log( LOW_histogram[ j ], quantized.size( ) );
        for( size_t j = 0; j < size; ++j )
          complete_histogram[ size + j ] = Log( HIGH_histogram[ j ], quantized.size( ) );
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
