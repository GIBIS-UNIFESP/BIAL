/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/11 
 * @brief Functions to locate key points in signal such as peaks, acclivities, plateaus, etc.
 */

#ifndef BIALSIGNALKEYPOINTS_C
#define BIALSIGNALKEYPOINTS_C

#include "SignalKeyPoints.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_SignalKeyPoints )
#define BIAL_EXPLICIT_SignalKeyPoints
#endif

#if defined ( BIAL_EXPLICIT_SignalKeyPoints ) || ( BIAL_IMPLICIT_BIN )

#include "Signal.hpp"

namespace Bial {

  size_t SignalOp::Peak( const Signal &sgn, size_t min_bin, size_t max_bin ) {
    try {
      size_t peak = min_bin;
      for( size_t it = min_bin + 1; it < max_bin; ++it ) {
        if( sgn[ peak ] < sgn[ it ] )
          peak = it;
      }
      return( peak );
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

  Vector< size_t > SignalOp::Peaks( const Signal &sgn, size_t min_bin, size_t max_bin, size_t kernel_size ) {
    try {
      if( sgn.size( ) <= kernel_size ) {
        std::string msg( BIAL_ERROR( "Kernel size must be greater than the signal size. Given: signal size: " +
                                     std::to_string( sgn.size( ) ) + ", kernel size: " + std::to_string(
                                       kernel_size ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      Vector< size_t > res;
      /* Searching the adjacents of each bin to find if they are local maxima. */
      for( size_t bin = min_bin; bin <= max_bin; ++bin ) {
        bool is_peak = true;
        for( size_t adj = 0; adj < kernel_size; ++adj ) {
          if( ( bin + adj >= kernel_size / 2 ) && ( bin + adj <= max_bin + kernel_size / 2 ) ) {
            if( sgn[ bin ] < sgn[ bin - kernel_size / 2 + adj ] ) {
              is_peak = false;
              break;
            }
          }
          else {
            is_peak = false;
            break;
          }
        }
        if( is_peak ) {
          res.push_back( bin );
        }
      }
      return( res );
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

  size_t SignalOp::FirstRightPeak( const Signal &sgn, size_t start_bin, size_t kernel_size ) {
    try {
      if( sgn.size( ) > 0 )
        return( SignalOp::FirstRightPeak( sgn, start_bin, sgn.size( ) - 1, kernel_size ) );
      return( 0 );
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

  size_t SignalOp::FirstRightPeak( const Signal &sgn, size_t start_bin, size_t end_bin, size_t kernel_size ) {
    try {
      size_t peak = start_bin;
      for( size_t it = start_bin; it <= end_bin; ++it ) {
        if( sgn[ it ] > sgn[ peak ] )
          peak = it;
        else if( static_cast< int >( peak ) < static_cast< int >( it ) - static_cast< int >( kernel_size ) )
          break;
      }
      return( peak );
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

  size_t SignalOp::FirstLeftPeak( const Signal &sgn, size_t start_bin, size_t kernel_size ) {
    try {
      return( SignalOp::FirstLeftPeak( sgn, start_bin, 0, kernel_size ) );
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

  size_t SignalOp::FirstLeftPeak( const Signal &sgn, size_t start_bin, size_t end_bin, size_t kernel_size ) {
    try {
      size_t peak = start_bin;
      for( int it = static_cast< int >( start_bin ); it >= static_cast< int >( end_bin ); --it ) {
        if( sgn[ it ] > sgn[ peak ] )
          peak = static_cast< size_t >( it );
        else if( peak > it + kernel_size )
          break;
      }
      return( peak );
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

  size_t SignalOp::Valley( const Signal &sgn, size_t min_bin, size_t max_bin ) {
    try {
      size_t valley = min_bin;
      for( size_t it = min_bin + 1; it < max_bin; ++it ) {
        if( sgn[ valley ] > sgn[ it ] ) {
          valley = it;
        }
      }
      return( valley );
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

  Vector< size_t > SignalOp::Valleys( const Signal &sgn, size_t, size_t, size_t kernel_size ) {
    try {
      if( sgn.size( ) <= kernel_size ) {
        std::string msg( BIAL_ERROR( "Kernel size must be greater than the signal size. Given: signal size: " +
                                     std::to_string( sgn.size( ) ) + ", kernel size: " + std::to_string(
                                       kernel_size ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      size_t tot_bins = sgn.size( );
      Vector< size_t > res;
      /* Searching the adjacents of each bin to find if they are local minima. */
      for( size_t bin = 0; bin < tot_bins; ++bin ) {
        bool is_valley = true;
        for( size_t adj = 0; adj < kernel_size; ++adj ) {
          if( ( bin + adj >= kernel_size / 2 ) && ( bin + adj < tot_bins + kernel_size / 2 ) ) {
            if( sgn[ bin ] > sgn[ bin - kernel_size / 2 + adj ] ) {
              is_valley = false;
              break;
            }
          }
          else {
            is_valley = false;
            break;
          }
        }
        if( is_valley ) {
          res.push_back( bin );
        }
      }
      return( res );
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

  size_t SignalOp::FirstRightValley( const Signal &sgn, size_t start_bin, size_t kernel_size ) {
    try {
      COMMENT( "FirstRightValley. Size: " << sgn.size( ), 0 );
      if( sgn.size( ) > 0 )
        return( SignalOp::FirstRightValley( sgn, start_bin, sgn.size( ) - 1, kernel_size ) );
      return( 0 );
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

  size_t SignalOp::FirstRightValley( const Signal &sgn, size_t start_bin, size_t end_bin, size_t kernel_size ) {
    try {
      COMMENT( "Valley start.", 0 );
      size_t valley = start_bin;
      for( size_t it = start_bin; it <= end_bin; ++it ) {
        if( sgn[ it ] < sgn[ valley ] )
          valley = it;
        else if( static_cast< int >( valley ) < static_cast< int >( it ) - static_cast< int >( kernel_size ) )
          break;
      }
      return( valley );
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

  size_t SignalOp::FirstLeftValley( const Signal &sgn, size_t start_bin, size_t kernel_size ) {
    try {
      return( SignalOp::FirstLeftValley( sgn, start_bin, 0, kernel_size ) );
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

  size_t SignalOp::FirstLeftValley( const Signal &sgn, size_t start_bin, size_t end_bin, size_t kernel_size ) {
    try {
      size_t valley = start_bin;
      for( int it = static_cast< int >( start_bin ); it >= static_cast< int >( end_bin ); --it ) {
        if( sgn[ it ] < sgn[ valley ] ) {
          valley = static_cast< size_t >( it );
        }
        else if( valley > it + kernel_size ) {
          break;
        }
      }
      return( valley );
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

  size_t SignalOp::FirstRightAcclivity( const Signal &sgn, size_t start_bin, size_t kernel_size, 
                                        float signal_delta_proportion ) {
    try {
      if( sgn.size( ) > 0 )
        return( SignalOp::FirstRightAcclivity( sgn, start_bin, sgn.size( ) - 1, kernel_size, 
                                               signal_delta_proportion ) );
      return( 0 );
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

  size_t SignalOp::FirstRightAcclivity( const Signal &sgn, size_t start_bin, size_t end_bin, size_t kernel_size, 
                                        float signal_delta_proportion ) {
    try {
      size_t acclivity = start_bin;
      for( size_t it = start_bin; it <= end_bin; ++it ) {
        if( sgn[ it ] < sgn[ start_bin ] * ( 1.0 + signal_delta_proportion ) ) {
          acclivity = it;
        }
        else if( static_cast< int >( acclivity ) < static_cast< int >( it ) - static_cast< int >( kernel_size ) ) {
          break;
        }
      }
      return( acclivity + 1 );
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

  size_t SignalOp::FirstLeftAcclivity( const Signal &sgn, size_t start_bin, size_t kernel_size, 
                                       float signal_delta_proportion ) {
    try {
      return( SignalOp::FirstLeftAcclivity( sgn, start_bin, 0, kernel_size, signal_delta_proportion ) );
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
  
  size_t SignalOp::FirstLeftAcclivity( const Signal &sgn, size_t start_bin, size_t end_bin, size_t kernel_size,
                                       float signal_delta_proportion ) {
    try {
      size_t acclivity = start_bin;
      for( int it = static_cast< int >( start_bin ); it >= static_cast< int >( end_bin ); --it ) {
        if( sgn[ it ] < sgn[ start_bin ] * ( 1.0 + signal_delta_proportion ) ) {
          acclivity = static_cast< size_t >( it );
        }
        else if( acclivity > it + kernel_size ) {
          break;
        }
      }
      return( acclivity - 1 );
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

  size_t SignalOp::FirstRightDeclivity( const Signal &sgn, size_t start_bin, size_t kernel_size, 
                                        float signal_delta_proportion ) {
    try {
      if( sgn.size( ) > 0 )
        return( SignalOp::FirstRightDeclivity( sgn, start_bin, sgn.size( ) - 1, kernel_size, 
                                               signal_delta_proportion ) );
      return( 0 );
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

  size_t SignalOp::FirstRightDeclivity( const Signal &sgn, size_t start_bin, size_t end_bin, size_t kernel_size,
                                        float signal_delta_proportion ) {
    try {
      size_t declivity = start_bin;
      for( size_t it = start_bin; it <= end_bin; ++it ) {
        if( sgn[ it ] > sgn[ start_bin ] * ( 1.0 - signal_delta_proportion ) ) {
          declivity = it;
        }
        else if( static_cast< int >( declivity ) < static_cast< int >( it ) - static_cast< int >( kernel_size ) ) {
          break;
        }
      }
      return( declivity + 1 );
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

  size_t SignalOp::FirstLeftDeclivity( const Signal &sgn, size_t start_bin, size_t kernel_size, 
                                       float signal_delta_proportion ) {
    try {
      return( SignalOp::FirstLeftDeclivity( sgn, start_bin, 0, kernel_size, signal_delta_proportion ) );
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

  size_t SignalOp::FirstLeftDeclivity( const Signal &sgn, size_t start_bin, size_t end_bin, size_t kernel_size,
                                       float signal_delta_proportion ) {
    try {
      size_t declivity = start_bin;
      for( int it = static_cast< int >( start_bin ); it >= static_cast< int >( end_bin ); --it ) {
        if( sgn[ it ] > sgn[ start_bin ] * ( 1.0 - signal_delta_proportion ) ) {
          declivity = static_cast< size_t >( it );
        }
        else if( declivity > it + kernel_size ) {
          break;
        }
      }
      return( declivity - 1 );
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

  size_t SignalOp::PlateauRightLimit( const Signal &sgn, size_t start_bin, size_t kernel_size, 
                                      float signal_delta_proportion )
  {
    try {
      if( sgn.size( ) > 0 )
        return( SignalOp::PlateauRightLimit( sgn, start_bin, sgn.size( ) - 1, kernel_size, signal_delta_proportion ) );
      return( 0 );
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

  size_t SignalOp::PlateauRightLimit( const Signal &sgn, size_t start_bin, size_t end_bin, size_t kernel_size,
                                      float signal_delta_proportion ) {
    try {
      size_t plateau_end = start_bin;
      for( size_t it = start_bin; it <= end_bin; ++it ) {
        if( ( sgn[ it ] > sgn[ start_bin ] * ( 1.0 - signal_delta_proportion ) ) &&
            ( ( sgn[ it ] ) < sgn[ start_bin ] * ( 1.0 + signal_delta_proportion ) ) ) {
          plateau_end = it;
        }
        else if( ( it >= kernel_size ) &&
                 ( static_cast< int >( plateau_end ) < static_cast< int >( it ) -
                   static_cast< int >( kernel_size ) ) ) {
          break;
        }
      }
      return( plateau_end );
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

  size_t SignalOp::PlateauLeftLimit( const Signal &sgn, size_t start_bin, size_t kernel_size, 
                                     float signal_delta_proportion ) {
    try {
      return( SignalOp::PlateauLeftLimit( sgn, start_bin, 0, kernel_size, signal_delta_proportion ) );
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

  size_t SignalOp::PlateauLeftLimit( const Signal &sgn, size_t start_bin, size_t end_bin, size_t kernel_size,
                                     float signal_delta_proportion ) {
    try {
      size_t plateau_end = start_bin;
      for( int it = static_cast< int >( start_bin ); it >= static_cast< int >( end_bin ); --it ) {
        if( ( sgn[ it ] > sgn[ start_bin ] * ( 1.0 - signal_delta_proportion ) ) &&
            ( sgn[ it ] < sgn[ start_bin ] * ( 1.0 + signal_delta_proportion ) ) ) {
          plateau_end = static_cast< size_t >( it );
        }
        else if( ( it < static_cast< int >( sgn.size( ) ) - static_cast< int >( kernel_size ) ) &&
                 ( plateau_end > it + kernel_size ) ) {
          break;
        }
      }
      return( plateau_end );
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

#ifdef BIAL_EXPLICIT_SignalKeyPoints

#endif

}

#endif

#endif
