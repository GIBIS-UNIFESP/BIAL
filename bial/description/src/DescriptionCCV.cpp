#include "DescriptionCCV.hpp"

#include "AdjacencyRound.hpp"
#include "Signal.hpp"

#include <fstream>
#include <queue>

namespace Bial {


  CCV::CCV( FeatureDetector< Color > *Fd ) : CCV( Fd->Run( ) ) {
  }

  CCV::CCV( Vector < std::tuple < Image< Color >, Image< int >> > detected ) : FeatureExtractor< Color, int >( detected ) {
    this->dim = 4;
  }

  void CCV::SetParameters( ParameterInterpreter *interpreter ) {
    Vector< parameter > vet;
    vet.push_back( std::tie( "dim", dim ) );

    interpreter->SetExpectedParameters( vet );
    vet = interpreter->Interpret( );

    std::tie( std::ignore, dim ) = vet[ 0 ];
  }

  std::string CCV::GetParameters( ParameterInterpreter *interpreter ) {
    Vector< parameter > vet;
    vet.push_back( std::tie( "dim", dim ) );

    interpreter->SetExpectedParameters( vet );

    return( interpreter->GetExpectedParameters( ) );
  }


  CCVfeature CCV::Run( ) {
    size_t size = dim * dim * dim;

    size_t p;

    CCVfeature feat;
    Image< Color > img;
    Image< int > mask;
    Image< int > quantized;

    unsigned char fator_g = dim;
    unsigned char fator_b = fator_g * dim;
    unsigned char r, g, b;

    Features< int > complete_histogram;
    Features< int > HIGH_histogram;
    Features< int > LOW_histogram;

    Vector< bool > frequency;
    std::queue< int > fila;
    int nlabels = 0;
    for( size_t i = 0; i < this->detected.size( ); ++i ) {
      /* quantização------------------------------------------------ */
      std::tie( img, mask ) = this->detected[ i ];

      quantized = Image< int >( img.size( 0 ), img.size( 1 ) );

      frequency = Vector< bool >( quantized.size( ) );
      int labels[ quantized.size( ) ];
      for( size_t i = 0; i < quantized.size( ); i++ ) {
        labels[ i ] = NIL;
      }
      for( size_t j = 0; j < quantized.size( ); ++j ) {
        r = dim * img[ j ].channel[ 1 ] / 256;
        g = dim * img[ j ].channel[ 2 ] / 256;
        b = dim * img[ j ].channel[ 3 ] / 256;

        quantized[ j ] = ( r + fator_g * g + fator_b * b );
      }
      /* ----------------------------------------------------------- */

      /* Frequencia dos pixels-------------------------------------- */

      Adjacency adjpixels = AdjacencyType::Circular( 1.1f );
      for( size_t y = 0; y < quantized.size( 1 ); y++ ) {
        for( size_t x = 0; x < quantized.size( 0 ); x++ ) {
          if( mask( x, y ) == 1 ) {
            size_t p = x + y * quantized.size( 0 );
            if( labels[ p ] == NIL ) {
              labels[ p ] = nlabels++;
              fila.push( p );
              while( !fila.empty( ) ) {
                p = fila.front( );
                fila.pop( );

                int u_x = p % quantized.size( 0 );
                int u_y = p / quantized.size( 0 );
                for( size_t pos = 1; pos < adjpixels.size( ); ++pos ) {
                  int v_x = u_x + adjpixels( pos, 0 );
                  int v_y = u_y + adjpixels( pos, 1 );
                  if( ( v_x >= 0 ) && ( v_x < static_cast< int >( quantized.size( 0 ) ) ) &&
                          ( v_y >= 0 ) && ( v_y < static_cast< int >( quantized.size( 1 ) ) ) ) {
                    int q = v_x + v_y * quantized.size( 0 );
                    if( ( labels[ q ] == NIL ) && ( quantized[ p ] == quantized[ q ] ) ) {
                      labels[ q ] = labels[ p ];
                      fila.push( q );
                    }
                  }
                }
              }
            }
          }
        }
      }
      /* ---------------------------------------------------------- */

      /* Histograma------------------------------------------------ */
      Vector< int > area( nlabels );
      for( int i = 0; i < nlabels; i++ ) {
        area[ i ] = 0;
      }
      for( p = 0; p < quantized.size( ); p++ ) {
        area[ labels[ p ] ]++;
      }
      for( p = 0; p < quantized.size( ); p++ ) {
        if( 100 * area[ labels[ p ] ] < static_cast< int >( MIN_AREA * quantized.size( ) ) ) {
          frequency[ p ] = LOW;
        }
        else {
          frequency[ p ] = HIGH;
        }
      }
      complete_histogram = Features< int >( 2 * size );
      LOW_histogram = Features< int >( size );
      HIGH_histogram = Features< int >( size );
      for( size_t j = 0; j < quantized.size( ); j++ ) {
        if( frequency[ j ] == LOW ) {
          LOW_histogram[ quantized[ j ] ]++;
        }
        else {
          HIGH_histogram[ quantized[ j ] ]++;
        }
      }
      for( size_t j = 0; j < size; j++ ) {
        complete_histogram[ j ] = ( int ) Log( LOW_histogram[ j ], quantized.size( ) );
      }
      for( size_t j = 0; j < size; j++ ) {
        complete_histogram[ size + j ] = ( int ) Log( HIGH_histogram[ j ], quantized.size( ) );
      }
      feat.push_back( complete_histogram );
      /* ---------------------------------------------------------- */
    }
    return( feat );
  }

}
