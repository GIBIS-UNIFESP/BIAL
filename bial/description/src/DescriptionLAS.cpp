#include "DescriptionLAS.hpp"

#include "AdjacencyRound.hpp"
#include "Signal.hpp"

#include <fstream>
#include <stdlib.h>

namespace Bial {

  size_t LAS::QuantizeValue( int value ) {
    if( value < 4 ) {
      return( 0 );
    }
    else if( value < 16 ) {
      return( 1 );
    }
    else if( value < 64 ) {
      return( 2 );
    }
    else { return( 3 ); }
  }

  LAS::LAS( FeatureDetector< int > *Fd ) : LAS( Fd->Run( ) ) {
  }

  LAS::LAS( Vector < std::tuple < Image< int >, Image< int >> > detected ) : FeatureExtractor< int, int >( detected ) {
    this->bins = 4;
  }

  void LAS::SetParameters( ParameterInterpreter *interpreter ) {
    Vector< parameter > vet;
    vet.push_back( std::tie( "bins", bins ) );

    interpreter->SetExpectedParameters( vet );
    vet = interpreter->Interpret( );

    std::tie( std::ignore, bins ) = vet[ 0 ];
  }

  std::string LAS::GetParameters( ParameterInterpreter *interpreter ) {
    Vector< parameter > vet;
    vet.push_back( std::tie( "bins", bins ) );

    interpreter->SetExpectedParameters( vet );

    return( interpreter->GetExpectedParameters( ) );
  }


  LASfeature LAS::Run( ) {
    size_t size = 3 * bins * bins * bins
                  + 3 * bins * bins
                  + 3 * bins
                  + 3 + 1;

    LASfeature feat;
    Image< int > img;
    Image< int > mask;


    Features< int > histogram;
    Vector< int > property;

    ulong p, q;
    int g[ 4 ];
    for( size_t i = 0; i < this->detected.size( ); ++i ) {
      std::tie( img, mask ) = this->detected[ i ];
      property = Vector< int >( img.size( ) );

      size_t mask_size = 0;
      /* Local Activity Spectrum------------------------------------------------ */
      Adjacency adjpixels = AdjacencyType::Circular( 1.5f );
      for( size_t y = 0; y < img.size( 1 ); y++ ) {
        for( size_t x = 0; x < img.size( 0 ); x++ ) {
          if( mask( x, y ) == 1 ) {
            mask_size++;
            p = x + y * img.size( 0 );
            for( size_t pos = 1; pos <= adjpixels.size( ) / 2; pos++ ) {
              ulong pixel_x = x + adjpixels( pos, 0 );
              ulong pixel_y = y + adjpixels( pos, 1 );

              g[ pos - 1 ] = 0;
              if( img.ValidCoordinate( pixel_x, pixel_y ) && ( mask( pixel_x, pixel_y ) == 1 ) ) {
                q = pixel_x + pixel_y * img.size( 0 );
                g[ pos - 1 ] += abs( img[ p ] - img[ q ] );
              }
              pixel_x = x - adjpixels( pos, 0 );
              pixel_y = y - adjpixels( pos, 1 );
              if( img.ValidCoordinate( pixel_x, pixel_y ) && ( mask( pixel_x, pixel_y ) == 1 ) ) {
                q = pixel_x + pixel_y * img.size( 0 );
                g[ pos - 1 ] += abs( img[ p ] - img[ q ] );
              }
              g[ pos - 1 ] = QuantizeValue( g[ pos - 1 ] );

            }
            property[ p ] = g[ 0 ] * bins * bins * bins +
                            g[ 3 ] * bins * bins +
                            g[ 2 ] * bins +
                            g[ 1 ];

          }
        }
      }
      /* ---------------------------------------------------------- */

      /* Histograma------------------------------------------------ */
      histogram = Features< int >( size );
      for( size_t j = 0; j < img.size( ); j++ ) {
        histogram[ property[ j ] ]++;
      }
      for( size_t j = 0; j < size; j++ ) {
        histogram[ j ] = ( int ) Log( histogram[ j ], mask_size );
      }
      feat.push_back( histogram );
      /* ---------------------------------------------------------- */
    }
    return( feat );
  }

}
