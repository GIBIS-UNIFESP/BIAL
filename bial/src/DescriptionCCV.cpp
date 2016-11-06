#include "DescriptionCCH.hpp"

#include "Adjacency.hpp"
#include "Signal.hpp"

#include <fstream>


namespace Bial {

  CCH::CCH( FeatureDetector< Color > *Fd ) : CCH( Fd->Run( ) ) {
  }

  CCH::CCH( Vector < std::tuple < Image< Color >, Image< int >> > detected ) : FeatureExtractor< Color, int >( detected ) {
    this->grid = 4;
    this->dim = 4;
  }

  void CCH::SetParameters( ParameterInterpreter *interpreter ) {

    Vector< parameter > vet;
    vet.push_back(  std::tie( "dim", dim ) );
    vet.push_back(  std::tie( "grid", grid ) );

    interpreter->SetExpectedParameters( vet );
    vet = interpreter->Interpret( );

     std::tie( std::ignore, dim ) = vet[ 0 ];
     std::tie( std::ignore, grid ) = vet[ 1 ];

    vet.clear( );
  }

   std::string CCH::GetParameters( ParameterInterpreter *interpreter ) {
    Vector< parameter > vet;
    vet.push_back(  std::tie( "dim", dim ) );
    vet.push_back(  std::tie( "grid", grid ) );

    interpreter->SetExpectedParameters( vet );

    return( interpreter->GetExpectedParameters( ) );
  }


  CCHfeature CCH::Run( ) {
    size_t size = dim * dim * dim;
    size_t cell = grid * grid;
    size_t bins = cell * size;

    CCHfeature feat;
    Image< Color > img;
    Image< int > mask;
    Image< int > quantized;

    unsigned char fator_g = dim;
    unsigned char fator_b = fator_g * dim;
    unsigned char r, g, b;

    Features< int > histogram, area, colors;
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

      /* Histograma------------------------------------------------ */
      colors = Features< int >( bins );
      area = Features< int >( size );


      size_t fator_x = grid;
      size_t fator_y = cell;
      for( size_t r = 0; r < quantized.size( 1 ); r++ ) {
        for( size_t c = 0; c < quantized.size( 0 ); c++ ) {
          if( mask( c, r ) == 1 ) {
            size_t x = grid * r / quantized.size( 1 );
            size_t y = grid * c / quantized.size( 0 );
            colors[ fator_y * quantized[ r * quantized.size( 0 ) + c ] + fator_x * y + x ]++;
          }
        }
      }
      for( size_t j = 0; j < quantized.size( ); j++ ) {
        if( mask[ j ] == 1 ) {
          area[ quantized[ j ] ]++;
        }
      }
      size_t n = 0;
      for( size_t j = 0; j < size; j++ ) {
        if( area[ j ] ) {
          for( size_t k = 0; k < cell; k++ ) {
            colors[ n++ ] = colors[ j * cell + k ];
          }
        }
      }
      colors.resize( n );

      histogram = Features< int >( 2 + n + size );
      n = 0;

      histogram[ n++ ] = dim;
      histogram[ n++ ] = grid;
      for( size_t j = 0; j < area.size( ); j++ ) {
        histogram[ n++ ] = Log( area[ j ], quantized.size( ) );
      }
      for( size_t j = 0; j < colors.size( ); j++ ) {
        histogram[ n++ ] = Log( colors[ j ], quantized.size( ) );
      }
      feat.push_back( histogram );
      /* ---------------------------------------------------------- */
    }
    return( feat );
  }

}
