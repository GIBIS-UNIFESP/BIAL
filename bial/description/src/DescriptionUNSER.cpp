#include "DescriptionUNSER.hpp"

#include "AdjacencyRound.hpp"
#include "Signal.hpp"

#include <fstream>
#include <stdlib.h>

namespace Bial {
  uchar UNSER::LinearNormalize( double value, double n ) {
    value = value / n;
    return( uchar( 255.0 * value ) );
  }

  float UNSER::Mean( Vector< float > s ) {
    float mean = 0.0;
    for( size_t i = 0; i < s.size( ); i++ ) {
      mean += i * s[ i ];
    }
    return( mean * 0.5 );
  }

  float UNSER::Contrast( Vector< float > d ) {
    float contrast = 0.0;
    for( int i = -255; i <= 255; i++ ) {
      contrast += i * i * d[ i + 255 ];
    }
    return( contrast );
  }

  float UNSER::Correlation( Vector< float > s, float mean, float contrast ) {
    float aux;

    float correlation = 0.0;
    for( size_t i = 0; i < s.size( ); i++ ) {
      aux = i - 2.0 * mean;
      correlation += aux * aux * s[ i ];
    }
    correlation -= contrast;
    correlation *= 0.5;

    return( correlation );
  }

  float UNSER::Energy( Vector< float > s, Vector< float > d ) {
    float energy_s = 0.0;
    float energy_d = 0.0;
    for( size_t i = 0; i < s.size( ); i++ ) {
      energy_s += s[ i ] * s[ i ];
      energy_d += d[ i ] * d[ i ];
    }
    return( energy_s * energy_d );
  }

  float UNSER::Entropy( Vector< float > s, Vector< float > d ) {
    float entropy_s = 0.0;
    float entropy_d = 0.0;
    for( size_t i = 0; i < s.size( ); i++ ) {
      if( s[ i ] > 0.0 ) {
        entropy_s += s[ i ] * log10( s[ i ] );
      }
      if( d[ i ] > 0.0 ) {
        entropy_d += d[ i ] * log10( d[ i ] );
      }
    }
    return( -entropy_s - entropy_d );
  }

  float UNSER::Homogeneity( Vector< float > d ) {
    float homogeneity = 0.0;
    for( int j = -255; j <= 255; j++ ) {
      homogeneity += ( 1.0 / ( 1.0 + ( float ) ( j * j ) ) ) * d[ j + 255 ];
    }
    return( homogeneity );
  }

  float UNSER::MaximalProbability( Vector< float > s ) {
    float max = 0.0;
    for( size_t i = 0; i < s.size( ); i++ ) {
      if( max < s[ i ] ) {
        max = s[ i ];
      }
    }
    return( max );
  }

  float UNSER::StandardDeviation( float contrast, float correlation ) {
    return( sqrt( correlation + contrast ) );
  }


  UNSER::UNSER( FeatureDetector< int > *Fd ) : UNSER( Fd->Run( ) ) {
  }

  UNSER::UNSER( Vector <  std::tuple < Image< int >, Image< int >> > detected ) : FeatureExtractor< int, int >( detected ) {
  }

  void UNSER::SetParameters( ParameterInterpreter *interpreter ) {
  }

  std::string UNSER::GetParameters( ParameterInterpreter *interpreter ) {
    return( "" );
  }

  UNSERfeature UNSER::Run( ) {
    size_t size = 32;

    size_t p, q;
    int x_dsp, y_dsp;

    UNSERfeature feat;
    Image< int > img;
    Image< int > mask;

    size_t mask_size = 0;
    Features< int > histogram;
    for( size_t i = 0; i < this->detected.size( ); ++i ) {
      std::tie( img, mask ) = this->detected[ i ];

      /* Calculo do dif e sum------------------------------------------------ */
      Adjacency adjpixels = AdjacencyType::Circular( 1.5f );

      Vector< Vector< float > > sum( 4 );
      for( size_t j = 0; j < sum.size( ); j++ ) {
        sum[ j ] = Vector< float >( 511 );
      }
      Vector< Vector< float > > dif( 4 );
      for( size_t j = 0; j < dif.size( ); j++ ) {
        dif[ j ] = Vector< float >( 511 );
      }
      for( size_t y = 1; y < img.size( 1 ) - 1; y++ ) {
        for( size_t x = 1; x < img.size( 0 ) - 1; x++ ) {
          if( mask( x, y ) == 1 ) {
            mask_size++;
            p = x + y * img.size( 0 );
            for( size_t pos = 1; pos <= adjpixels.size( ) >> 1; ++pos ) {
              if( pos == 1 ) {
                x_dsp = x - adjpixels( ( pos + 3 ) % 5 + 1, 0 );
              }
              else {
                x_dsp = x + adjpixels( ( pos + 3 ) % 5 + 1, 0 );
              }
              y_dsp = y + adjpixels( ( pos + 3 ) % 5 + 1, 1 );

              q = x_dsp + y_dsp * img.size( 0 );
              if( img.ValidCoordinate( x_dsp, y_dsp ) && ( mask( x_dsp, y_dsp ) == 1 ) ) {
                sum[ pos - 1 ] [ img[ p ] + img[ q ] ] += 1.0;
                dif[ pos - 1 ] [ img[ p ] - img[ q ] + 255 ] += 1.0;
              }
            }
          }
        }
      }
      for( size_t j = 0; j < 4; j++ ) {
        for( size_t k = 0; k < 511; k++ ) {
          sum[ j ][ k ] /= ( float ) mask_size;
          dif[ j ][ k ] /= ( float ) mask_size;
        }
      }
      /* ---------------------------------------------------------- */

      /* Histograma------------------------------------------------ */
      histogram = Features< int >( size );
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
        histogram[ j * 8 + 7 ] = sqrt( 2 ) * StandardDeviation( contrast, correlation );
      }
      for( size_t j = 0; j < size; j++ ) {
        histogram[ j ] = ( int ) LinearNormalize( histogram[ j ], 255 );
      }
      feat.push_back( histogram );
      /* ---------------------------------------------------------- */
    }
    return( feat );
  }
}
