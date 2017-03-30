#include "DescriptionSCD.hpp"

#include "Signal.hpp"

#include <fstream>
#include <stdlib.h>

namespace Bial {

  SCD::SCD( FeatureDetector< Color > *Fd ) : SCD( Fd->Run( ) ) {
  }

  SCD::SCD( Vector < std::tuple < Image< Color >, Image< int >> > detected ) : FeatureExtractor< Color, int >( detected ) {
    this->dim = 4;
  }

  void SCD::SetParameters( ParameterInterpreter *interpreter ) {
    Vector< parameter > vet;
    vet.push_back( std::tie( "dim", dim ) );

    interpreter->SetExpectedParameters( vet );
    vet = interpreter->Interpret( );

    std::tie( std::ignore, dim ) = vet[ 0 ];
  }

  std::string SCD::GetParameters( ParameterInterpreter *interpreter ) {
    Vector< parameter > vet;
    vet.push_back( std::tie( "dim", dim ) );
    interpreter->SetExpectedParameters( vet );

    return( interpreter->GetExpectedParameters( ) );
  }


  SCDfeature SCD::Run( ) {
    size_t size = dim * dim * dim * dim;

    SCDfeature feat;
    Image< Color > img, cimg;
    Image< int > mask;
    Image< int > quantized;

    ulong fator_s = dim * dim;
    ulong fator_v = fator_s * dim;
    ulong h, s, v;


    Features< int > histogram;
    for( size_t i = 0; i < this->detected.size( ); ++i ) {
      std::tie( img, mask ) = this->detected[ i ];

      /* quantização------------------------------------------------ */
      quantized = Image< int >( img.size( 0 ), img.size( 1 ) );
      cimg = RgbToHsv( img );
      for( size_t j = 0; j < quantized.size( ); ++j ) {
        h = dim * dim * cimg[ j ].channel[ 1 ] / 256;
        s = dim * cimg[ j ].channel[ 2 ] / 256;
        v = dim * cimg[ j ].channel[ 3 ] / 256;

        quantized[ j ] = ( h + fator_s * s + fator_v * v );
      }
      /* ----------------------------------------------------------- */

      /* Histograma------------------------------------------------ */
      histogram = Features< int >( size );
      size_t mask_size = 0;

      for( size_t j = 0; j < quantized.size( ); j++ ) {
        if( mask[ j ] == 1 ) {
          histogram[ quantized[ j ] ]++;
          mask_size++;
        }
      }
      for( size_t j = 0; j < size; j++ ) {
        histogram[ j ] = Log( histogram[ j ], mask_size );
      }
      feat.push_back( histogram );
      /* ---------------------------------------------------------- */
    }
    return( feat );
  }

}
