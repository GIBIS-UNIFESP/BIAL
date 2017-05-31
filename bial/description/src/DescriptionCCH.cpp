#include "DescriptionCCH.hpp"
#include "Signal.hpp"

namespace Bial {

  CCH::CCH( FeatureDetector< Color > *Fd ) try : CCH( Fd->Run( ) ) {
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


  CCH::CCH( const Vector < std::tuple< Image< Color >, Vector< size_t > > > &detected ) try : 
    FeatureExtractor< Color, int >( detected ) {
      this->grid = 4;
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

  void CCH::SetParameters( ParameterInterpreter *interpreter ) {
    try {
      Vector< parameter > vet;
      vet.push_back( std::tie( "dim", dim ) );
      vet.push_back( std::tie( "grid", grid ) );
      interpreter->SetExpectedParameters( vet );
      vet = interpreter->Interpret( );
      std::tie( std::ignore, dim ) = vet[ 0 ];
      std::tie( std::ignore, grid ) = vet[ 1 ];
      vet.clear( );
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

  std::string CCH::GetParameters( ParameterInterpreter *interpreter ) {
    try {
      Vector< parameter > vet;
      vet.push_back( std::tie( "dim", dim ) );
      vet.push_back( std::tie( "grid", grid ) );
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

  CCHfeature CCH::Run( ) {
    try {
      size_t size = dim * dim * dim;
      size_t cell = grid * grid;
      size_t bins = cell * size;
      CCHfeature feat;
      uchar fator_g = dim;
      uchar fator_b = fator_g * dim;
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
          uchar r = dim * img[ j ][ 1 ] / 256;
          uchar g = dim * img[ j ][ 2 ] / 256;
          uchar b = dim * img[ j ][ 3 ] / 256;
          quantized[ j ] = ( r + fator_g * g + fator_b * b );
        }
        COMMENT( "Histograma------------------------------------------------", 3 );
        Features< int >colors( bins );
        Features< int > area( size );
        size_t fator_x = grid;
        size_t fator_y = cell;
        for( size_t pxl_idx = 0; pxl_idx < mask_size; ++pxl_idx ) {
          div_t pxl_coords = std::div( static_cast< int >( mask[ pxl_idx ] ), static_cast< int >( x_size ) );
          size_t c = pxl_coords.rem;
          size_t r = pxl_coords.quot;
          size_t x = grid * r / y_size;
          size_t y = grid * c / x_size;
          ++colors[ fator_y * quantized[ r * quantized.size( 0 ) + c ] + fator_x * y + x ];
        }
        for( size_t pxl_idx = 0; pxl_idx < mask_size; ++pxl_idx ) {
          size_t pxl = mask[ pxl_idx ];
          ++area[ quantized[ pxl ] ];
        }
        size_t n = 0;
        for( size_t j = 0; j < size; j++ ) {
          if( area[ j ] ) {
            for( size_t k = 0; k < cell; k++ )
              colors[ n++ ] = colors[ j * cell + k ];
          }
        }
        colors.resize( n );
        Features< int > histogram( 2 + n + size );
        n = 0;
        histogram[ n++ ] = dim;
        histogram[ n++ ] = grid;
        for( size_t j = 0; j < size; ++j )
          histogram[ n++ ] = Log( area[ j ], mask_size );
        for( size_t j = 0; j < bins; ++j )
          histogram[ n++ ] = Log( colors[ j ], mask_size );
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
