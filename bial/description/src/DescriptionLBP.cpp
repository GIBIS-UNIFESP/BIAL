#include "DescriptionLBP.hpp"

#include "AdjacencyRound.hpp"
#include "Signal.hpp"

#include <fstream>
#include <stdlib.h>

namespace Bial {

  uchar LBP::Uniformity( uchar s[ 8 ] ) {
    uchar u;

    u = abs( s[ 7 ] - s[ 0 ] );
    for( size_t p = 1; p < 8; p++ ) {
      u += abs( s[ p ] - s[ p - 1 ] );
    }
    return( u );
  }

  uchar LBP::Signal( int x ) {
    if( x >= 0 ) {
      return( 1 );
    }
    else { return( 0 ); }
  }


  LBP::LBP( FeatureDetector< int > *Fd ) : LBP( Fd->Run( ) ) {
  }

  LBP::LBP( Vector < std::tuple < Image< int >, Image< int >> > detected ) : FeatureExtractor< int, int >( detected ) {
  }

  void LBP::SetParameters( ParameterInterpreter *interpreter ) {
  }

  std::string LBP::GetParameters( ParameterInterpreter *interpreter ) {
    return( "" );
  }

  LBPfeature LBP::Run( ) {
    size_t size = 10;

    LBPfeature feat;
    Image< int > img;
    Image< int > mask;

    Features< int > histogram;

    ulong p, q;
    uchar sum, s[ 8 ];
    int gC, gP;
    Adjacency adjpixels = AdjacencyType::Circular( 1.5f );
    FixAdj( adjpixels );
    for( size_t i = 0; i < this->detected.size( ); ++i ) {
      std::tie( img, mask ) = this->detected[ i ];

      /* Local Binary Pattern------------------------------------------------ */
      Vector< int > property( img.size( ) );
      for( size_t y = 0; y < img.size( 1 ); y++ ) {
        for( size_t x = 0; x < img.size( 0 ); x++ ) {
          if( mask( x, y ) == 1 ) {
            p = x + y * img.size( 0 );

            gC = img[ p ];
            for( size_t pos = 1; pos < adjpixels.size( ); pos++ ) {
              ulong pixel_x = x + adjpixels( pos, 0 );
              ulong pixel_y = y + adjpixels( pos, 1 );
              if( img.ValidCoordinate( pixel_x, pixel_y ) && ( mask( pixel_x, pixel_y ) == 1 ) ) {
                q = pixel_x + pixel_y * img.size( 0 );

                gP = img[ q ];
                s[ pos - 1 ] = Signal( gP - gC );
              }
            }
            if( Uniformity( s ) <= 2 ) {
              sum = 0;
              for( size_t j = 0; j < 8; j++ ) {
                sum += s[ j ];
              }
              property[ p ] = sum;
            }
            else {
              property[ p ] = 9;
            }
          }
        }
      }
      /* ---------------------------------------------------------- */

      /* Histograma------------------------------------------------ */
      histogram = Features< int >( size );
      for( size_t j = 0; j < property.size( ); j++ ) {
        histogram[ property[ j ] ]++;
      }
      for( size_t j = 0; j < size; j++ ) {
        histogram[ j ] = Log( histogram[ j ], img.size( ) );
      }
      feat.push_back( histogram );
      /* ---------------------------------------------------------- */
    }
    return( feat );
  }

}
