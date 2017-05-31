#include "DescriptionGCH.hpp"
#include "Signal.hpp"

namespace Bial {

  GCH::GCH( FeatureDetector< Color > *Fd ) try : GCH( Fd->Run( ) ) {
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

  GCH::GCH( const Vector< std::tuple< Image< Color >, Vector< size_t > > > &detected ) try : 
    FeatureExtractor< Color, int >( detected ) {
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

  void GCH::SetParameters( ParameterInterpreter *interpreter ) {
    try {
      Vector< parameter > vet;
      vet.push_back( std::tie( "dim", dim ) );
      interpreter->SetExpectedParameters( vet );
      vet = interpreter->Interpret( );
      std::tie( std::ignore, dim ) = vet[ 0 ];
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

  std::string GCH::GetParameters( ParameterInterpreter *interpreter ) {
    try {
      Vector< parameter > vet;
      vet.push_back( std::tie( "dim", dim ) );
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

  GCHfeature GCH::Run( ) {
    try {
      size_t size = dim * dim * dim;
      GCHfeature feat;
      int fator_g = dim;
      int fator_b = fator_g * dim;
      for( size_t i = 0; i < detected.size( ); ++i ) {
        COMMENT( "quantização------------------------------------------------", 3 )
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
          int r = dim * img[ j ][ 1 ] / 256;
          int g = dim * img[ j ][ 2 ] / 256;
          int b = dim * img[ j ][ 3 ] / 256;
          quantized[ j ] = ( r + fator_g * g + fator_b * b );
        }
        COMMENT( "Histograma------------------------------------------------", 3 );
        Features< int > complete_histogram( size );
        for( size_t pxl_idx = 0; pxl_idx < mask_size; ++pxl_idx ) {
          size_t pxl = mask[ pxl_idx ];
          int value = quantized[ pxl ];
          ++complete_histogram[ value ];
        }
        size_t hist_size = complete_histogram.size( );
          for( size_t i = 0; i < hist_size; ++i )
            complete_histogram[ i ] = Log( complete_histogram[ i ], img_size );
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
