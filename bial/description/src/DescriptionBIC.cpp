#include "AdjacencyRound.hpp"
#include "Color.hpp"
#include "DescriptionBIC.hpp"

namespace Bial {
  BIC::BIC( FeatureDetector< Color > *Fd ) try : BIC( Fd->Run( ) ) {
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

  BIC::BIC( const Vector< std::tuple< Image< Color >, Vector< size_t > > > &detected ) try :
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

  void BIC::SetParameters( ParameterInterpreter *interpreter ) {
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

  std::string BIC::GetParameters( ParameterInterpreter *interpreter ) {
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

  BICfeature BIC::Run( ) {
    try {
      int size = dim * dim * dim;
      Vector< int > border, interior;
      BICfeature feat;
      int fator_g = dim;
      int fator_b = fator_g * dim;
      Adjacency adjpixels = AdjacencyType::Circular( 1.0 );
      size_t adj_size = adjpixels.size( );
      for( size_t i = 0; i < this->detected.size( ); ++i ) {
        COMMENT( "quantização------------------------------------------------", 3 );
        const Image< Color > &img( std::get< 0 >( this->detected[ i ] ) );
        const Vector< size_t > &mask( std::get< 1 >( this->detected[ i ] ) );
        size_t x_size = img.size( 0 );
        size_t y_size = img.size( 1 );
        size_t mask_size = mask.size( );
        size_t img_size = img.size( );
        if( mask_size == 0 ) {
          std::string msg( BIAL_ERROR( "Empty mask. " ) );
          throw( std::logic_error( msg ) );
        }
        Vector< bool > mask_vct( img_size, false );
        for( size_t pxl_idx = 0; pxl_idx < mask_size; ++pxl_idx )
          mask_vct[ mask[ pxl_idx ] ] = true;
        Image< int > quantized( x_size, y_size );
        for( size_t j = 0; j < img_size; ++j ) {
          int r = dim * img[ j ].channel[ 1 ] / 256;
          int g = dim * img[ j ].channel[ 2 ] / 256;
          int b = dim * img[ j ].channel[ 3 ] / 256;
          quantized[ j ] = ( r + fator_g * g + fator_b * b );
        }
        COMMENT( "Classificação dos pixels----------------------------------", 3 );
        for( size_t pxl_idx = 0; pxl_idx < mask_size; ++pxl_idx ) {
          size_t src_pxl = mask[ pxl_idx ];
          int same_color = 0;
          div_t pxl_coords = std::div( static_cast< int >( src_pxl ), static_cast< int >( x_size ) );
          size_t x = pxl_coords.rem;
          size_t y = pxl_coords.quot;
          for( size_t pos = 1; pos < adj_size; ++pos ) {
            int x_dsp = x + adjpixels( pos, 0 );
            int y_dsp = y + adjpixels( pos, 1 );
            size_t p_dsp = x_dsp + y_dsp * x_size;
            if( quantized.ValidCoordinate( x_dsp, y_dsp ) && ( mask_vct[ p_dsp ] != 0 ) &&
                ( quantized[ src_pxl ] == quantized[ p_dsp ] ) )
              ++same_color;
          }
          if( same_color == 4 )
            interior.push_back( quantized[ src_pxl ] );
          else
            border.push_back( quantized[ src_pxl ] );
        }
        COMMENT( "Histograma------------------------------------------------", 3 );
        Features< int >complete_histogram( 2 * size );
        for( size_t j = 0; j < border.size( ); ++j )
          ++complete_histogram[ border[ j ] + size ];
        for( size_t j = 0; j < interior.size( ); ++j )
          ++complete_histogram[ interior[ j ] ];
        for( size_t j = 0; j < complete_histogram.size( ); ++j )
          complete_histogram[ j ] = Log( complete_histogram[ j ], mask_size );
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
