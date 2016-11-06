#include "DescriptionLCH.hpp"

#include "Adjacency.hpp"
#include "Signal.hpp"

#include <fstream>

namespace Bial {

  LCH::LCH( FeatureDetector< Color > *Fd ) : LCH( Fd->Run( ) ) {
  }

  LCH::LCH( Vector < std::tuple < Image< Color >, Image< int >> > detected ) : FeatureExtractor< Color, int >( detected ) {
    this->dim = 4;
    this->BINS = 4;
  }

  void LCH::SetParameters( ParameterInterpreter *interpreter ) {
    Vector< parameter > vet;
    vet.push_back( std::tie( "dim", dim ) );
    vet.push_back( std::tie( "BINS", BINS ) );

    interpreter->SetExpectedParameters( vet );
    vet = interpreter->Interpret( );

    std::tie( std::ignore, dim ) = vet[ 0 ];
    std::tie( std::ignore, BINS ) = vet[ 1 ];
  }

  std::string LCH::GetParameters( ParameterInterpreter *interpreter ) {
    Vector< parameter > vet;
    vet.push_back( std::tie( "dim", dim ) );
    vet.push_back( std::tie( "BINS", BINS ) );

    interpreter->SetExpectedParameters( vet );

    return( interpreter->GetExpectedParameters( ) );
  }

  LCHfeature LCH::Run( ) {
    size_t size = dim * dim * dim;
    size_t NBINS = BINS * BINS * size;

    LCHfeature feat;
    Image< Color > img;
    Image< int > mask;
    Image< int > quantized;

    unsigned char fator_g = dim;
    unsigned char fator_b = fator_g * dim;
    unsigned char r, g, b;


    Features< int > histogram;
    for( size_t i = 0; i < this->detected.size( ); ++i ) {
      /* quantização------------------------------------------------ */
      std::tie( img, mask ) = this->detected[ i ];

      quantized = Image< int >( img.size( 0 ), img.size( 1 ) );
      for( size_t j = 0; j < quantized.size( ); ++j ) {
        r = dim * img[ j ].channel[ 1 ] / 256;
        g = dim * img[ j ].channel[ 2 ] / 256;
        b = dim * img[ j ].channel[ 3 ] / 256;

        quantized[ j ] = ( r + fator_g * g + fator_b * b );
      }
      /* ----------------------------------------------------------- */

      /* Criação do Histograma-------------------------------------- */
      ulong fator_x, fator_y;
      ulong x, y;
      ulong r, c;

      histogram = Features< int >( NBINS );

      fator_x = size;
      fator_y = BINS * size;

      size_t mask_pixels = 0;
      for( r = 0; r < quantized.size( 1 ); r++ ) {
        for( c = 0; c < quantized.size( 0 ); c++ ) {
          if( mask( c, r ) == 1 ) {
            mask_pixels++;

            x = BINS * r / quantized.size( 1 );
            y = BINS * c / quantized.size( 0 );

            histogram[ quantized[ r * quantized.size( 0 ) + c ] + fator_x * x + fator_y * y ]++;
          }
        }
      }
      /*
       * ----------------------------------------------------------
       * Compressão do Histograma------------------------------------------------
       */
      for( size_t j = 0; j < histogram.size( ); j++ ) {
        histogram[ j ] = ( uchar ) ( 255. * ( ( float ) histogram[ j ] / ( float ) mask_pixels ) );
      }
      /* ---------------------------------------------------------- */


      feat.push_back( histogram );
    }
    return( feat );
  }

}
