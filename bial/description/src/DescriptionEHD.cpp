#include "DescriptionEHD.hpp"
#include "Signal.hpp"

namespace Bial {

  size_t EHD::FindMaxPosition( const Vector< int > &vet ) {
    try {
      int val = vet[ 0 ];
      size_t max = 0;
      size_t vet_size = vet.size( );
      for( size_t i = 1; i < vet_size; ++i ) {
        if( val < vet[ i ] ) {
          val = vet[ i ];
          max = i;
        }
      }
      return( max );
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

  Vector< int > EHD::ConvolveFilter( int mask[ 2 ][ 2 ] ) {
    try {
      Vector< int > ori( BINS );
      ori[ 0 ] = std::abs( mask[ 0 ][ 0 ] + mask[ 0 ][ 1 ] - mask[ 1 ][ 0 ] - mask[ 1 ][ 1 ] );
      ori[ 1 ] = std::abs( mask[ 0 ][ 0 ] - mask[ 1 ][ 1 ] );
      ori[ 1 ] = RIGHT_SHIFT( DESCALE( MULTIPLY( FIX( ori[ 1 ] ), FIX_1_4142135 ), CONST_BITS - PASS1_BITS ),
                              CONST_BITS + PASS1_BITS );
      ori[ 2 ] = std::abs( mask[ 0 ][ 0 ] - mask[ 0 ][ 1 ] + mask[ 1 ][ 0 ] - mask[ 1 ][ 1 ] );
      ori[ 3 ] = std::abs( mask[ 0 ][ 1 ] - mask[ 1 ][ 0 ] );
      ori[ 3 ] = RIGHT_SHIFT( DESCALE( MULTIPLY( FIX( ori[ 3 ] ), FIX_1_4142135 ), CONST_BITS - PASS1_BITS ),
                              CONST_BITS + PASS1_BITS );
      ori[ 4 ] = std::abs( mask[ 0 ][ 0 ] - mask[ 0 ][ 1 ] - mask[ 1 ][ 0 ] + mask[ 1 ][ 1 ] );
      ori[ 4 ] = MULTIPLY( ori[ 4 ], 2 );
      return( ori );
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

  EHD::EHD( FeatureDetector< int > *Fd ) try : EHD( Fd->Run( ) ) {
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

  EHD::EHD( const Vector< std::tuple< Image< int >, Vector< size_t > > > &detected ) try :
    FeatureExtractor< int, int >( detected ) {
      GRID = 4;
      BINS = 5;
      BLCK = 16;
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

  void EHD::SetParameters( ParameterInterpreter *interpreter ) {
    try {
      Vector< parameter > vet;
      vet.push_back( std::tie( "grid", GRID ) );
      vet.push_back( std::tie( "bins", BINS ) );
      vet.push_back( std::tie( "blck", BLCK ) );
      interpreter->SetExpectedParameters( vet );
      vet = interpreter->Interpret( );
      std::tie( std::ignore, GRID ) = vet[ 0 ];
      std::tie( std::ignore, BINS ) = vet[ 1 ];
      std::tie( std::ignore, BLCK ) = vet[ 2 ];
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

  std::string EHD::GetParameters( ParameterInterpreter *interpreter ) {
    try {
      Vector< parameter > vet;
      vet.push_back( std::tie( "grid", GRID ) );
      vet.push_back( std::tie( "bins", BINS ) );
      vet.push_back( std::tie( "blck", BLCK ) );
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

  EHDfeature EHD::Run( ) {
    try {
      const size_t CELL = ( GRID * GRID * BINS );
      const size_t SIZE = ( CELL + 14 * BINS );
      EHDfeature feat;
      int level[ GRID ][ GRID ][ BLCK ][ BLCK ][ 2 ][ 2 ];
      ulong hist[ GRID ][ GRID ][ BINS ], ext[ 14 * BINS ];
      for( size_t i = 0; i < this->detected.size( ); ++i ) {
        const Image< int > &img( std::get< 0 >( detected[ i ] ) );
        const Vector< size_t > &mask( std::get< 1 >( detected[ i ] ) );
        size_t x_size = img.size( 0 );
        size_t y_size = img.size( 1 );
        size_t mask_size = mask.size( );
        if( mask_size == 0 ) {
          std::string msg( BIAL_ERROR( "Empty mask. " ) );
          throw( std::logic_error( msg ) );
        }
        COMMENT( "initialize levels---------------------------------------------------------", 3 );
        for( size_t ygrid = 0; ygrid < GRID; ++ygrid ) {
          for( size_t xgrid = 0; xgrid < GRID; ++xgrid ) {
            for( size_t yblck = 0; yblck < BLCK; ++yblck ) {
              for( size_t xblck = 0; xblck < BLCK; ++xblck ) {
                for( size_t ymask = 0; ymask < 2; ++ymask ) {
                  for( size_t xmask = 0; xmask < 2; ++xmask )
                    level[ ygrid ][ xgrid ][ yblck ][ xblck ][ ymask ][ xmask ] = 0;
                }
              }
            }
          }
        }
        COMMENT( "initialize hist-----------------------------------------------------------", 3 );
        for( size_t ygrid = 0; ygrid < GRID; ++ygrid ) {
          for( size_t xgrid = 0; xgrid < GRID; ++xgrid ) {
            for( size_t bin = 0; bin < BINS; ++bin )
              hist[ ygrid ][ xgrid ][ bin ] = 0;
          }
        }
        COMMENT( "quantize levels-----------------------------------------------------------", 3 );
        size_t grid_rows = y_size / GRID;
        size_t grid_cols = x_size / GRID;
        size_t blck_rows = grid_rows / BLCK;
        size_t blck_cols = grid_cols / BLCK;
        size_t mask_rows = blck_rows / 2;
        size_t mask_cols = blck_cols / 2;
        for( size_t pxl_idx = 0; pxl_idx < mask_size; ++pxl_idx ) {
          size_t pxl = mask[ pxl_idx ];
          div_t pxl_coords = std::div( static_cast< int >( pxl ), static_cast< int >( x_size ) );
          size_t col = pxl_coords.rem;
          size_t row = pxl_coords.quot;
          size_t ygrid = GRID * row / y_size;
          size_t xgrid = GRID * col / x_size;
          size_t yblck = BLCK * ( row % grid_rows ) / grid_rows;
          size_t xblck = BLCK * ( col % grid_cols ) / grid_cols;
          size_t ymask = 2 * ( ( row % grid_rows ) % blck_rows ) / blck_rows;
          size_t xmask = 2 * ( ( col % grid_cols ) % blck_cols ) / blck_cols;
          level[ ygrid ][ xgrid ] [ yblck ][ xblck ] [ ymask ][ xmask ] += pxl;
        }
        COMMENT( "Normalize level-----------------------------------------------------------", 3 );
        for( size_t ygrid = 0; ygrid < GRID; ++ygrid ) {
          for( size_t xgrid = 0; xgrid < GRID; ++xgrid ) {
            for( size_t yblck = 0; yblck < BLCK; ++yblck ) {
              for( size_t xblck = 0; xblck < BLCK; ++xblck ) {
                for( size_t ymask = 0; ymask < 2; ++ymask ) {
                  for( size_t xmask = 0; xmask < 2; ++xmask )
                    level[ ygrid ][ xgrid ] [ yblck ][ xblck ] [ ymask ][ xmask ] /= ( mask_rows * mask_cols );
                }
              }
            }
          }
        }
        COMMENT( "Compute hist--------------------------------------------------------------", 3 );
        for( size_t ygrid = 0; ygrid < GRID; ++ygrid ) {
          for( size_t xgrid = 0; xgrid < GRID; ++xgrid ) {
            for( size_t yblck = 0; yblck < BLCK; ++yblck ) {
              for( size_t xblck = 0; xblck < BLCK; ++xblck ) {
                Vector< int > ori( ConvolveFilter( level[ ygrid ][ xgrid ][ yblck ][ xblck ] ) );
                size_t max = FindMaxPosition( ori );
                if( ori[ max ] > 0 )
                  hist[ ygrid ][ xgrid ][ max ]++;
              }
            }
          }
        }
        COMMENT( "Extend histogram----------------------------------------------------------", 3 );
        size_t k = 0;
        for( size_t ygrid = 0; ygrid < GRID; ++ygrid ) {
          for( size_t bin = 0; bin < BINS; ++bin ) {
            size_t sum = 0;
            for( size_t xgrid = 0; xgrid < GRID; ++xgrid )
              sum += hist[ ygrid ][ xgrid ][ bin ];
            ext[ k++ ] = sum;
          }
        }
        for( size_t xgrid = 0; xgrid < GRID; ++xgrid ) {
          for( size_t bin = 0; bin < BINS; ++bin ) {
            size_t sum = 0;
            for( size_t ygrid = 0; ygrid < GRID; ++ygrid )
              sum += hist[ ygrid ][ xgrid ][ bin ];
            ext[ k++ ] = sum;
          }
        }
        for( size_t j = 8 * BINS; j < 14 * BINS; j++ )
          ext[ j ] = 0;
        for( size_t ygrid = 0; ygrid < GRID; ++ygrid ) {
          for( size_t xgrid = 0; xgrid < GRID; ++xgrid ) {
            k = ( 8 + ( ygrid / 2 * 2 + xgrid / 2 ) ) * BINS;
            for( size_t bin = 0; bin < BINS; ++bin )
              ext[ k + bin ] += hist[ ygrid ][ xgrid ][ bin ];
          }
        }
        for( size_t bin = 0; bin < BINS; ++bin ) {
          ext[ 12 * BINS + bin ] = hist[ GRID / 2 ][ GRID / 2 ][ bin ] +
            hist[ GRID / 2 ][ GRID / 2 + 1 ][ bin ] +
            hist[ GRID / 2 + 1 ][ GRID / 2 ][ bin ] +
            hist[ GRID / 2 + 1 ][ GRID / 2 + 1 ][ bin ];
        }
        for( size_t ygrid = 0; ygrid < GRID; ++ygrid ) {
          for( size_t xgrid = 0; xgrid < GRID; ++xgrid ) {
            for( size_t bin = 0; bin < BINS; ++bin )
              ext[ 13 * BINS + bin ] += hist[ ygrid ][ xgrid ][ bin ];
          }
        }
        COMMENT( "Histograma----------------------------------------------------------------", 3 );
        Features< int > histogram( SIZE );
        k = 0;
        for( size_t ygrid = 0; ygrid < GRID; ++ygrid ) {
          for( size_t xgrid = 0; xgrid < GRID; ++xgrid ) {
            for( size_t bin = 0; bin < BINS; ++bin )
              histogram[ k++ ] = hist[ ygrid ][ xgrid ][ bin ];
          }
        }
        for( ; k < SIZE; ++k )
          histogram[ k ] = ext[ k - CELL ];
        COMMENT( "Normalize histogram-------------------------------------------------------", 3 );
        for( size_t ygrid = 0; ygrid < GRID; ++ygrid ) {
          for( size_t xgrid = 0; xgrid < GRID; ++xgrid ) {
            size_t cell = ( ygrid * GRID + xgrid ) * BINS;
            size_t sum = 0;
            for( size_t bin = 0; bin < BINS; ++bin )
              sum += histogram[ bin + cell ];
            for( size_t bin = 0; bin < BINS; ++bin )
              histogram[ bin + cell ] = Log( histogram[ bin + cell ], sum );
          }
        }
        for( size_t j = 0; j < 14; ++j ) {
          size_t cell = CELL + j * BINS;
          size_t sum = 0;
          for( size_t bin = 0; bin < BINS; ++bin )
            sum += histogram[ bin + cell ];
          for( size_t bin = 0; bin < BINS; ++bin )
            histogram[ bin + cell ] = Log( histogram[ bin + cell ], sum );
        }
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
