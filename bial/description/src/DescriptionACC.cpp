#include "DescriptionACC.hpp"
#include "Adjacency.hpp"
#include "AdjacencyRound.hpp"
#include "Signal.hpp"

namespace Bial {

  ACC::ACC( FeatureDetector< Color > *Fd ) try : ACC( Fd->Run( ) ) {
  }
  catch( std::bad_alloc &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
    throw( std::runtime_error( msg ) );
  }
  catch( std::runtime_error &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
    throw( std::runtime_error( msg ) );
  }
  catch( const std::out_of_range &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
    throw( std::out_of_range( msg ) );
  }
  catch( const std::logic_error &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
    throw( std::logic_error( msg ) );
  }

  ACC::ACC( const Vector< std::tuple< Image< Color >, Vector< size_t > > > &detected ) try : 
    FeatureExtractor< Color, int >( detected ) {
      this->n_distances = 4;
      for( size_t r = 1; r <= 7; r += 2 )
        this->distances.push_back( r );
      this->dim = 4;
    }
  catch( std::bad_alloc &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
    throw( std::runtime_error( msg ) );
  }
  catch( std::runtime_error &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
    throw( std::runtime_error( msg ) );
  }
  catch( const std::out_of_range &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
    throw( std::out_of_range( msg ) );
  }
  catch( const std::logic_error &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
    throw( std::logic_error( msg ) );
  }

  void ACC::SetParameters( ParameterInterpreter *interpreter ) {
    try {
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
      for( size_t i = 0; i < n_distances; ++i ) {
        temp = std::string( "d" ) + std::to_string( i );
        if( i < distances.size( ) )
          vet.push_back( std::tie( temp, distances[ i ] ) );
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
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  std::string ACC::GetParameters( ParameterInterpreter *interpreter ) {
    try {
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
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  ACCfeature ACC::Run( ) {
    try {
      size_t size = dim * dim * dim * n_distances;
      ACCfeature feat;
      unsigned char fator_g = dim;
      unsigned char fator_b = fator_g * dim;
      Vector< Vector< int > > frequency( n_distances );
      Adjacency adjpixels = AdjacencyType::Circular( 1.1f );
      size_t adj_size = adjpixels.size( );
      for( size_t i = 0; i < this->detected.size( ); ++i ) {
        COMMENT( "quantização------------------------------------------------", 3 );
        const Image< Color > &img( std::get< 0 >( detected[ i ] ) );
        const Vector< size_t > &mask( std::get< 1 >( detected[ i ] ) );
        size_t x_size = img.size( 0 );
        size_t y_size = img.size( 1 );
        size_t mask_size = mask.size( );
        size_t img_size = img.size( );
        if( mask_size == 0 ) {
          std::string msg( BIAL_ERROR( "Empty mask. " ) );
          throw( std::logic_error( msg ) );
        }
        Vector< bool > mask_vct( img_size );
        for( size_t pxl_idx = 0; pxl_idx < mask_size; ++pxl_idx )
          mask_vct[ mask[ pxl_idx ] ] = true;
        Image< int > quantized( x_size, y_size );
        for( size_t j = 0; j < img_size; ++j ) {
          int r = dim * img[ j ][ 1 ] / 256;
          int g = dim * img[ j ][ 2 ] / 256;
          int b = dim * img[ j ][ 3 ] / 256;
          quantized[ j ] = ( r + fator_g * g + fator_b * b );
        }
        COMMENT( "Frequencia dos pixels--------------------------------------", 3 );
        for( size_t j = 0; j < n_distances; ++j )
          frequency[ j ] = Vector< int >( img_size );
        for( size_t pxl_idx = 0; pxl_idx < mask_size; ++pxl_idx ) {
          size_t src_pxl = mask[ pxl_idx ];
          div_t pxl_coords = std::div( static_cast< int >( src_pxl  ), static_cast< int >( x_size ) );
          size_t x = pxl_coords.rem;
          size_t y = pxl_coords.quot;
          for( size_t d = 0; d < n_distances; ++d ) {
            size_t radius = distances[ d ];
            for( size_t pos = 1; pos < adj_size; ++pos ) {
              size_t x_dsp = x + radius * adjpixels( pos, 0 );
              size_t y_dsp = y + radius * adjpixels( pos, 1 );
              if( quantized.ValidCoordinate( x_dsp, y_dsp ) && ( mask_vct( x_dsp + y_dsp * x_size ) == 1 ) &&
                  ( quantized( x, y ) == quantized( x_dsp, y_dsp ) ) )
                frequency[ d ][ src_pxl ] += 1;
            }
          }
        }
        COMMENT( "Histograma------------------------------------------------", 3 );
        Features< int > complete_histogram( size );
        for( size_t j = 0; j < img_size; ++j ) {
          for( size_t d = 0; d < n_distances; d++ )
            complete_histogram[ n_distances * quantized[ j ] +d ] += frequency[ d ][ j ];
        }
        for( size_t j = 0; j < size; ++j )
          complete_histogram[ j ] = Log( complete_histogram[ j ], n_distances * mask_size );
        feat.push_back( complete_histogram );
      }
      return( feat );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

}
