#include "DescriptionCSD.hpp"

#include "AdjacencyRound.hpp"
#include "Signal.hpp"

#include <fstream>
#include <stdlib.h>

namespace Bial {

  CSD::CSD( FeatureDetector< Color > *Fd ) : CSD( Fd->Run( ) ) {
  }

  CSD::CSD( Vector < std::tuple < Image< Color >, Image< int >> > detected ) : FeatureExtractor< Color, int >( detected ) {
  }

  void CSD::SetParameters( ParameterInterpreter *interpreter ) {
  }

  std::string CSD::GetParameters( ParameterInterpreter *interpreter ) {
    return( "" );
  }

  CSDfeature CSD::Run( ) {
    size_t size = 64;

    CSDfeature feat;
    Image< Color > img, hmmd;
    Image< int > mask;
    Image< int > quantized;

    ulong h, s, d;

    int k, m, w;
    size_t maximum;

    Vector< int > dx;
    Vector< int > dy;
    std::vector< bool > colors;

    Features< int > histogram;
    for( size_t i = 0; i < this->detected.size( ); ++i ) {
      std::tie( img, mask ) = this->detected[ i ];

      /* quantização------------------------------------------------ */
      quantized = Image< int >( img.size( 0 ), img.size( 1 ) );
      hmmd = RgbToHmmd( img );
      for( size_t j = 0; j < quantized.size( ); ++j ) {
        d = 5 * hmmd[ j ].channel[ 3 ] / 256;
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
      /* ----------------------------------------------------------- */

      /* Histograma------------------------------------------------ */
      histogram = Features< int >( size );
      colors = std::vector< bool >( size );


      k = std::max( 0, ( ( int ) ( std::log( quantized.size( ) ) / std::log( 2 ) ) ) / 2 - 8 );
      w = 1 << k;
      m = 8 * w;

      Adjacency adj = AdjacencyType::AdjacencyBox( m, m );

      maximum = 0;
      for( size_t y = 0; y <= quantized.size( 1 ) - m; y += w ) {
        for( size_t x = 0; x <= quantized.size( 0 ) - m; x += w ) {
          if( mask( x, y ) == 1 ) {
            for( size_t j = 0; j < size; j++ ) {
              colors[ j ] = false;
            }
            for( size_t pos = 0; pos < adj.size( ); ++pos ) {
              int x_dsp = x + adj( pos, 0 );
              int y_dsp = y + adj( pos, 1 );
              if( quantized.ValidCoordinate( x_dsp, y_dsp ) && ( mask( x_dsp, y_dsp ) == 1 ) ) {
                size_t p = x_dsp + y_dsp * quantized.size( 0 );
                colors[ quantized[ p ] ] = true;
              }
            }
            for( size_t j = 0; j < size; j++ ) {
              if( colors[ j ] ) {
                histogram[ j ]++;
              }
            }
            maximum++;
          }
        }
      }
      for( size_t j = 0; j < size; j++ ) {
        histogram[ j ] = Log( histogram[ j ], maximum );
      }
      /* ---------------------------------------------------------- */


      feat.push_back( histogram );
    }
    return( feat );
  }

}
