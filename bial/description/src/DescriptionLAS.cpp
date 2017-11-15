#include "DescriptionLAS.hpp"
#include "AdjacencyRound.hpp"
#include "Signal.hpp"

namespace Bial {

  size_t LAS::QuantizeValue( int value ) {
    if( value < 4 )
      return( 0 );
    else if( value < 16 )
      return( 1 );
    else if( value < 64 )
      return( 2 );
    else 
      return( 3 );
  }

  LAS::LAS( FeatureDetector< int > *Fd ) try : LAS( Fd->Run( ) ) {
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

  LAS::LAS( const Vector< std::tuple< Image< int >, Vector< size_t > > > &detected ) try : 
    FeatureExtractor< int, int >( detected ) {
      this->bins = 4;
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

  void LAS::SetParameters( ParameterInterpreter *interpreter ) {
    try {
      Vector< parameter > vet;
      vet.push_back( std::tie( "bins", bins ) );
      interpreter->SetExpectedParameters( vet );
      vet = interpreter->Interpret( );
      std::tie( std::ignore, bins ) = vet[ 0 ];
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

  std::string LAS::GetParameters( ParameterInterpreter *interpreter ) {
    try {
      Vector< parameter > vet;
      vet.push_back( std::tie( "bins", bins ) );
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

  LASfeature LAS::Run( ) {
    try {
      size_t size = 3 * bins * bins * bins + 3 * bins * bins + 3 * bins + 3 + 1;
      LASfeature feat;
      int g[ 4 ];
      Adjacency adjpixels = AdjacencyType::Circular( 1.5f );
      size_t adj_size = adjpixels.size( );
      for( size_t i = 0; i < this->detected.size( ); ++i ) {
        const Image< int > &img( std::get< 0 >( detected[ i ] ) );
        const Vector< size_t > &mask( std::get< 1 >( detected[ i ] ) );
        size_t x_size = img.size( 0 );
        size_t mask_size = mask.size( );
        size_t img_size = img.size( );
        if( mask_size == 0 ) {
          std::string msg( BIAL_ERROR( "Empty mask. " ) );
          throw( std::logic_error( msg ) );
        }
        Vector< bool > mask_vct( img_size, false );
        for( size_t pxl_idx = 0; pxl_idx < mask_size; ++pxl_idx )      
          mask_vct[ mask[ pxl_idx ] ] = true;
        Vector< int > property( img_size );
        COMMENT( "Local Activity Spectrum------------------------------------------------", 3 );
        for( size_t pxl_idx = 0; pxl_idx < mask_size; ++pxl_idx ) {
          size_t src_pxl = mask[ pxl_idx ];
          div_t pxl_coords = std::div( static_cast< int >( src_pxl ), static_cast< int >( x_size ) );
          size_t src_x = pxl_coords.rem;
          size_t src_y = pxl_coords.quot;
          for( size_t pos = 1; pos <= adj_size >> 1; pos++ ) {
            ulong adj_x = src_x + adjpixels( pos, 0 );
            ulong adj_y = src_y + adjpixels( pos, 1 );
            size_t adj_pxl = adj_x + adj_y * x_size;
            g[ pos - 1 ] = 0;
            if( img.ValidCoordinate( adj_x, adj_y ) && ( mask_vct[ adj_pxl ] ) )
              g[ pos - 1 ] += std::abs( img[ src_pxl ] - img[ adj_pxl ] );
            adj_x = src_x - adjpixels( pos, 0 );
            adj_y = src_y - adjpixels( pos, 1 );
            adj_pxl = adj_x + adj_y * x_size;
            if( img.ValidCoordinate( adj_x, adj_y ) && ( mask_vct[ adj_pxl ] ) )
              g[ pos - 1 ] += std::abs( img[ src_pxl ] - img[ adj_pxl ] );
            g[ pos - 1 ] = QuantizeValue( g[ pos - 1 ] );
          }
          property[ src_pxl ] = g[ 0 ] * bins * bins * bins + g[ 3 ] * bins * bins + g[ 2 ] * bins + g[ 1 ];
        }
        COMMENT( "Histograma------------------------------------------------", 3 );
        Features< int > histogram( size );
        for( size_t j = 0; j < img_size; ++j )
          ++histogram[ property[ j ] ];
        for( size_t j = 0; j < size; ++j )
          histogram[ j ] = Log( histogram[ j ], mask_size );
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
