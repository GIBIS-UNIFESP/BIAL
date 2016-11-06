#include "DescriptionGCH.hpp"

#include "Adjacency.hpp"
#include "Signal.hpp"

namespace Bial {

  GCH::GCH( FeatureDetector< Color > *Fd ) : GCH( Fd->Run( ) ) {
  }

  GCH::GCH( Vector < std::tuple < Image< Color >, Image< int >> > detected ) : FeatureExtractor< Color, int >( detected ) {
    this->dim = 4;
  }


  void GCH::SetParameters( ParameterInterpreter *interpreter ) {
    Vector< parameter > vet;
    vet.push_back( std::tie( "dim", dim ) );

    interpreter->SetExpectedParameters( vet );
    vet = interpreter->Interpret( );

    std::tie( std::ignore, dim ) = vet[ 0 ];
  }

  std::string GCH::GetParameters( ParameterInterpreter *interpreter ) {
    Vector< parameter > vet;
    vet.push_back( std::tie( "dim", dim ) );

    interpreter->SetExpectedParameters( vet );

    return( interpreter->GetExpectedParameters( ) );
  }

  GCHfeature GCH::Run( ) {
    size_t size = dim * dim * dim;

    GCHfeature feat;
    Image< Color > img;
    Image< int > mask;
    Image< int > quantized;
    int fator_g = dim;
    int fator_b = fator_g * dim;
    int r, g, b;
    Features< int > complete_histogram;
    for( size_t i = 0; i < detected.size( ); ++i ) {
      /* quantização------------------------------------------------ */
      std::tie( img, mask ) = detected[ i ];
      quantized = Image< int >( img.size( 0 ), img.size( 1 ) );
      for( size_t j = 0; j < quantized.size( ); ++j ) {
        r = dim * img[ j ].channel[ 1 ] / 256;
        g = dim * img[ j ].channel[ 2 ] / 256;
        b = dim * img[ j ].channel[ 3 ] / 256;

        quantized[ j ] = ( r + fator_g * g + fator_b * b );
      }
      /*
       * -----------------------------------------------------------
       * Histograma------------------------------------------------
       */
      complete_histogram = Features< int >( size );
      for( size_t i = 0; i < quantized.size( ); i++ ) {
        if( mask[ i ] == 1 ) {
          ( complete_histogram[ quantized[ i ] ] )++;
        }
      }
      for( size_t i = 0; i < complete_histogram.size( ); i++ ) {
        complete_histogram[ i ] = Log( complete_histogram[ i ], quantized.size( ) );
      }
      /* ----------------------------------------------------------Z */
      feat.push_back( complete_histogram );
    }
    return( feat );
  }

}
