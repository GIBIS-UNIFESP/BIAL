#include "DescriptionACC.hpp"

#include "Adjacency.hpp"
#include "AdjacencyRound.hpp"
#include "Signal.hpp"

#include <fstream>

namespace Bial {

  ACC::ACC( FeatureDetector< Color > *Fd ) : ACC( Fd->Run( ) ) {
  }

  ACC::ACC( Vector < std::tuple < Image< Color >, Image< int >> > detected ) : FeatureExtractor< Color, int >( detected ) {
    this->n_distances = 4;
    for( size_t r = 1; r <= 7; r += 2 ) {
      this->distances.push_back( r );
    }
    this->dim = 4;
  }

  void ACC::SetParameters( ParameterInterpreter *interpreter ) {
    size_t d;
    std::string temp;
    Vector< parameter > vet;
    vet.push_back(  std::tie( "dim", dim ) );
    vet.push_back(  std::tie( "n_distances", n_distances ) );

    interpreter->SetExpectedParameters( vet );
    vet = interpreter->Interpret( );

    std::tie( std::ignore, dim ) = vet[ 0 ];
    std::tie( std::ignore, n_distances ) = vet[ 1 ];

    vet.clear( );
    for( size_t i = 0; i < n_distances; i++ ) {
      temp = std::string( "d" ) + std::to_string( i );
      if( i < distances.size( ) ) {
        vet.push_back( std::tie( temp, distances[ i ] ) );
      }
      else {
        double nd = 0.0;
        vet.push_back( std::tie( temp, nd ) );
      }
    }
    interpreter->SetExpectedParameters( vet );
    vet = interpreter->Interpret( );

    distances.clear( );
    for( size_t i = 0; i < n_distances; i++ ) {
      std::tie( std::ignore, d ) = vet[ i ];

      distances.push_back( d );
    }
  }

  std::string ACC::GetParameters( ParameterInterpreter *interpreter ) {
    Vector< parameter > vet;
    vet.push_back( std::tie( "dim", dim ) );
    vet.push_back( std::tie( "n_distances", n_distances ) );
    for( size_t i = 0; i < n_distances; i++ ) {
      std::string temp = std::string( "d" ) + std::to_string( i );
      vet.push_back( std::tie( temp, distances[ i ] ) );
    }
    interpreter->SetExpectedParameters( vet );

    return( interpreter->GetExpectedParameters( ) );
  }


  ACCfeature ACC::Run( ) {
    size_t size = dim * dim * dim * n_distances;

    size_t p;

    ACCfeature feat;
    Image< Color > img;
    Image< int > mask;
    Image< int > quantized;

    unsigned char fator_g = dim;
    unsigned char fator_b = fator_g * dim;
    unsigned char r, g, b;


    Features< int > complete_histogram;
    Vector< int > frequency[ n_distances ];
    for( size_t i = 0; i < this->detected.size( ); ++i ) {
      /* quantização------------------------------------------------ */
      std::tie( img, mask ) = this->detected[ i ];

      quantized = Image< int >( img.size( 0 ), img.size( 1 ) );
      for( size_t j = 0; j < quantized.size( ); ++j ) {
        r = dim * img[ j ][ 1 ] / 256;
        g = dim * img[ j ][ 2 ] / 256;
        b = dim * img[ j ][ 3 ] / 256;

        quantized[ j ] = ( r + fator_g * g + fator_b * b );
      }
      /*
       * -----------------------------------------------------------
       * Frequencia dos pixels--------------------------------------
       */
      size_t mask_size = 0;
      for( size_t j = 0; j < n_distances; j++ ) {
        frequency[ j ] = Vector< int >( quantized.size( ) );
      }
      Adjacency adjpixels = AdjacencyType::Circular( 1.1f );
      for( size_t y = 0; y < quantized.size( 1 ); y++ ) {
        for( size_t x = 0; x < quantized.size( 0 ); x++ ) {
          if( mask( x, y ) == 1 ) {
            mask_size++;
            for( size_t d = 0; d < n_distances; d++ ) {
              size_t raio = distances[ d ];
              p = x + y * img.size( 0 );
              for( size_t pos = 1; pos < adjpixels.size( ); ++pos ) {
                int x_dsp = x + raio * adjpixels( pos, 0 );
                int y_dsp = y + raio * adjpixels( pos, 1 );
                if( quantized.ValidCoordinate( x_dsp, y_dsp ) && ( mask( x_dsp, y_dsp ) == 1 ) &&
                  ( quantized( x, y ) == quantized( x_dsp, y_dsp ) ) && ( raio != 0 ) ) {
                  frequency[ d ][ p ]++;
                }
              }
            }
          }
        }
      }
      /* ---------------------------------------------------------- */

      /* Histograma------------------------------------------------ */
      complete_histogram = Features< int >( size );
      for( size_t j = 0; j < quantized.size( ); j++ ) {
        for( size_t d = 0; d < n_distances; d++ ) {
          complete_histogram[ n_distances * quantized[ j ] + d ] += frequency[ d ][ j ];
        }
      }
      for( size_t j = 0; j < size; j++ ) {
        complete_histogram[ j ] = Log( complete_histogram[ j ], n_distances * mask_size );
      }
      feat.push_back( complete_histogram );
      /* ---------------------------------------------------------- */
    }
    return( feat );
  }
}
