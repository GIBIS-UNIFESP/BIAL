#include "DescriptionLBP.hpp"
#include "AdjacencyRound.hpp"
#include "Signal.hpp"

namespace Bial {

  uchar LBP::Uniformity( uchar s[ 8 ] ) {
    try {
      uchar u;
      u = std::abs( s[ 7 ] - s[ 0 ] );
      for( size_t p = 1; p < 8; ++p )
        u += std::abs( s[ p ] - s[ p - 1 ] );
      return( u );
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

  uchar LBP::Signal( int x ) {
    if( x >= 0 )
      return( 1 );
    else
      return( 0 );
  }

  LBP::LBP( FeatureDetector< int > *Fd ) try : LBP( Fd->Run( ) ) {
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

  LBP::LBP( const Vector< std::tuple< Image< int >, Vector< size_t > > > &detected ) try :
    FeatureExtractor< int, int >( detected ) {
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

  void LBP::SetParameters( ParameterInterpreter *interpreter ) {
  }

  std::string LBP::GetParameters( ParameterInterpreter *interpreter ) {
    return( "" );
  }

  LBPfeature LBP::Run( ) {
    try {
      size_t size = 10;
      LBPfeature feat;
      uchar s[ 8 ];
      Adjacency adjpixels( AdjacencyType::Circular( 1.5f ) );
      size_t adj_size = adjpixels.size( );
      FixAdj( adjpixels );
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

        COMMENT( "Local Binary Pattern------------------------------------------------", 3 );
        Vector< int > property( img_size );
        for( size_t pxl_idx = 0; pxl_idx < mask_size; ++pxl_idx ) {
          size_t src_pxl = mask[ pxl_idx ];
          div_t pxl_coords = std::div( static_cast< int >( src_pxl ), static_cast< int >( x_size ) );
          size_t src_x = pxl_coords.rem;
          size_t src_y = pxl_coords.quot;
          int gC = img[ src_pxl ];
          for( size_t pos = 1; pos < adj_size; ++pos ) {
            size_t adj_x = src_x + adjpixels( pos, 0 );
            size_t adj_y = src_y + adjpixels( pos, 1 );
            size_t adj_pxl = adj_x + adj_y * x_size;
            if( img.ValidCoordinate( adj_x, adj_y ) && ( mask_vct[ adj_pxl ] ) ) {
              int gP = img[ adj_pxl ];
              s[ pos - 1 ] = Signal( gP - gC );
            }
          }
          if( Uniformity( s ) <= 2 ) {
            uchar sum = 0;
            for( size_t j = 0; j < 8; j++ )
              sum += s[ j ];
            property[ src_pxl ] = sum;
          }
          else
            property[ src_pxl ] = 9;
        }
        COMMENT( "Histograma------------------------------------------------", 3 );
        Features< int > histogram( size );
        for( size_t j = 0; j < img_size; ++j )
          ++histogram[ property[ j ] ];
        for( size_t j = 0; j < size; j++ )
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
