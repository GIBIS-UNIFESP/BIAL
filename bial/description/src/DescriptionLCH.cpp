#include "DescriptionLCH.hpp"
#include "Signal.hpp"

namespace Bial {

  LCH::LCH( FeatureDetector< Color > *Fd ) try : LCH( Fd->Run( ) ) {
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

  LCH::LCH( const Vector< std::tuple< Image< Color >, Vector< size_t > > > &detected ) try : 
    FeatureExtractor< Color, int >( detected ) {
      this->dim = 4;
      this->BINS = 4;
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


  void LCH::SetParameters( ParameterInterpreter *interpreter ) {
    try {
      Vector< parameter > vet;
      vet.push_back( std::tie( "dim", dim ) );
      vet.push_back( std::tie( "BINS", BINS ) );
      interpreter->SetExpectedParameters( vet );
      vet = interpreter->Interpret( );
      std::tie( std::ignore, dim ) = vet[ 0 ];
      std::tie( std::ignore, BINS ) = vet[ 1 ];
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

  std::string LCH::GetParameters( ParameterInterpreter *interpreter ) {
    try {
      Vector< parameter > vet;
      vet.push_back( std::tie( "dim", dim ) );
      vet.push_back( std::tie( "BINS", BINS ) );
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

  LCHfeature LCH::Run( ) {
    try {
      size_t size = dim * dim * dim;
      size_t NBINS = BINS * BINS * size;
      LCHfeature feat;
      unsigned char fator_g = dim;
      unsigned char fator_b = fator_g * dim;
    
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
      
        Image< int > quantized( x_size, y_size );
        for( size_t j = 0; j < img_size; ++j ) {
          uchar r = dim * img[ j ].channel[ 1 ] / 256;
          uchar g = dim * img[ j ].channel[ 2 ] / 256;
          uchar b = dim * img[ j ].channel[ 3 ] / 256;

          quantized[ j ] = ( r + fator_g * g + fator_b * b );
        }
        COMMENT( "Criação do Histograma--------------------------------------", 3 );
        Features< int > histogram( NBINS );
        size_t fator_x = size;
        size_t fator_y = BINS * size;
        for( size_t pxl_idx = 0; pxl_idx < mask_size; ++pxl_idx ) {
          div_t pxl_coords = std::div( static_cast< int >( mask[ pxl_idx ] ), static_cast< int >( x_size ) );
          size_t c = pxl_coords.rem;
          size_t r = pxl_coords.quot;
          size_t x = BINS * r / y_size; // Is this correct? Division of x by y?
          size_t y = BINS * c / x_size;
          ++histogram[ quantized[ r * x_size + c ] + fator_x * x + fator_y * y ];
        }
        COMMENT( "Compressão do Histograma------------------------------------------------", 3 );
        size_t hist_size = histogram.size( );
        for( size_t j = 0; j < hist_size; ++j )
          histogram[ j ] = static_cast< uchar >( 255.0f * ( static_cast< float >( histogram[ j ] ) / mask_size ) );
        feat.push_back( histogram );
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
