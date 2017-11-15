/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/11 
 * @brief Signal handeling. 
 */

#ifndef BIALSIGNAL_C
#define BIALSIGNAL_C

#include "Signal.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_Signal )
#define BIAL_EXPLICIT_Signal
#endif

#if defined ( BIAL_EXPLICIT_Signal ) || ( BIAL_IMPLICIT_BIN )

#include "File.hpp"
#include "RandomQueue.hpp"
#include "SortingBinarySearch.hpp"

namespace Bial {

  Signal::Signal( ) : frequency( ), _data( ), qk_data( ) {
  }

  Signal::Signal( size_t data_size, double data_minimum, double data_step ) try : 
    frequency( data_size, 0.0 ), _data( data_size, data_minimum ), qk_data( ) {
      COMMENT( "Assigning quick pointers.", 2 );
      qk_data = &_data[ 0 ];

      COMMENT( "Assigning data.", 2 );
      for( size_t dt = 1; dt < _data.size( ); ++dt ) {
        QK_DATA( dt ) = QK_DATA( dt - 1 ) + data_step;
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
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Empty kernel." ) );
    throw( std::logic_error( msg ) );
  }

  template< class D > Signal::Signal( Vector< D > &new_frequency, double data_minimum, double data_step ) try :
    frequency( new_frequency ), _data( new_frequency.size( ), data_minimum ), qk_data( ) {
    if( new_frequency.size( ) == 0 ) {
      std::string msg( BIAL_ERROR( "Empty data vector to build the histogram." ) );
      throw( std::logic_error( msg ) );
    }
    COMMENT( "Assigning quick pointers.", 2 );
    qk_data = &_data[ 0 ];

    COMMENT( "Assigning data.", 2 );
    for( size_t dt = 1; dt < _data.size( ); ++dt ) {
      QK_DATA( dt ) = QK_DATA( dt - 1 ) + data_step;
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
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Empty kernel." ) );
    throw( std::logic_error( msg ) );
  }

  template< class D > Signal::Signal( Vector< D > &new_frequency, Vector< D > &new_data ) try :
    frequency( new_frequency ), _data( new_data ), qk_data( ) {
      if( new_frequency.size( ) == 0 ) {
        std::string msg( BIAL_ERROR( "Empty data vector to build the histogram." ) );
        throw( std::logic_error( msg ) );
      }
      if( new_frequency.size( ) != new_data.size( ) ) {
        std::string msg( BIAL_ERROR( "Frequency and data vector dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Assigning quick pointers.", 2 );
      qk_data = &_data[ 0 ];
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
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Empty kernel." ) );
    throw( std::logic_error( msg ) );
  }

  Signal::Signal( const Signal &sgn ) try : frequency( sgn.frequency ), _data( sgn._data ), qk_data( ) {
    COMMENT( "Assigning quick access pointers.", 2 );
    qk_data = &_data[ 0 ];
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

  Signal &Signal::operator=( const Signal &other ) {
    COMMENT( "Assigning signal", 2 );
    frequency = other.frequency;
    _data = other._data;
    qk_data = &_data[ 0 ];
    return( *this );
  }

  const double &Signal::operator[]( size_t p ) const {
    try {
#ifdef BIAL_DEBUG
      return( frequency.at( p ) );
#else
      return( frequency( p ) );
#endif
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
  }

  double &Signal::operator[]( size_t p ) {
    try {
#ifdef BIAL_DEBUG
      return( frequency.at( p ) );
#else
      return( frequency( p ) );
#endif
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
  }

  size_t Signal::Size( ) const {
    return( _data.size( ) );
  }

  size_t Signal::size( ) const {
    return( _data.size( ) );
  }

  size_t Signal::Bins( ) const {
    return( _data.size( ) );
  }

  const double* Signal::Data( ) const {
    return( qk_data );
  }

  double* Signal::Data( ) {
    return( qk_data );
  }

  const double* Signal::data( ) const {
    return( qk_data );
  }

  double* Signal::data( ) {
    return( qk_data );
  }

  template< class D >
  void Signal::Data( Vector< D > val ) {
    try {
      if( val.size( ) != frequency.size( ) ) {
        std::string msg( BIAL_ERROR( "Frequency and input vector dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      _data = val;
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    ;
  }

  const double* Signal::Frequency( ) const {
    return( frequency.data( ) );
  }

  double* Signal::Frequency( ) {
    return( frequency.data( ) );
  }

  void Signal::Frequency( Vector< double > val ) {
    try {
      if( val.size( ) != _data.size( ) ) {
        std::string msg( BIAL_ERROR( "Data and input vector dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      frequency = val;
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    ;
  }

  double Signal::MinimumFrequency( size_t min_bin ) const {
    try {
      if( _data.size( ) > 0 ) {
        return( MinimumFrequency( min_bin, _data.size( ) - 1 ) );
      }
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

  double Signal::MinimumFrequency( size_t min_bin, size_t max_bin ) const {
    try {
      double min = 0.0;
      for( size_t it = min_bin; it <= max_bin; ++it ) {
        if( min > frequency( it ) ) {
          min = frequency( it );
        }
      }
      return( min );
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

  double Signal::MaximumFrequency( size_t min_bin ) const {
    try {
      if( _data.size( ) > 0 ) {
        return( MaximumFrequency( min_bin, _data.size( ) - 1 ) );
      }
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

  double Signal::MaximumFrequency( size_t min_bin, size_t max_bin ) const {
    try {
      double max = 0.0;
      for( size_t it = min_bin; it <= max_bin; ++it ) {
        if( max < frequency( it ) ) {
          max = frequency( it );
        }
      }
      return( max );
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

  size_t Signal::MinimumFrequencyBin( size_t min_bin ) const {
    try {
      if( _data.size( ) > 0 ) {
        return( MinimumFrequencyBin( min_bin, _data.size( ) - 1 ) );
      }
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

  size_t Signal::MinimumFrequencyBin( size_t min_bin, size_t max_bin ) const {
    try {
      size_t bin = min_bin;
      double min = frequency( min_bin );
      for( size_t it = min_bin + 1; it <= max_bin; ++it ) {
        if( min > frequency( it ) ) {
          min = frequency( it );
          bin = it;
        }
      }
      return( bin );
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

  size_t Signal::MaximumFrequencyBin( size_t min_bin ) const {
    try {
      if( _data.size( ) > 0 ) {
        return( MaximumFrequencyBin( min_bin, _data.size( ) - 1 ) );
      }
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

  size_t Signal::MaximumFrequencyBin( size_t min_bin, size_t max_bin ) const {
    try {
      size_t bin = min_bin;
      double max = frequency( min_bin );
      for( size_t it = min_bin + 1; it <= max_bin; ++it ) {
        if( max < frequency( it ) ) {
          max = frequency( it );
          bin = it;
        }
      }
      return( bin );
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

  size_t Signal::Bin( double val ) const {
    try {
      return( Sorting::BinarySearch( _data, val ) );
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

  inline void Signal::Bin( size_t bin, double val ) {
    QK_DATA( bin ) = val;
  }

  const double &Signal::Data( size_t bin ) const {
    return( QK_DATA( bin ) );
  }

  double &Signal::Data( size_t bin ) {
    return( QK_DATA( bin ) );
  }

  template< class O >
  void Signal::Print( O &os ) const {
    try {
      for( size_t it = 0; it < _data.size( ); ++it ) {
        os << QK_DATA( it ) << " " << frequency( it ) << std::endl;
      }
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


#ifdef BIAL_EXPLICIT_Signal

  template Signal::Signal( Vector< int > &new_frequency, double data_minimum, double data_step );
  template Signal::Signal( Vector< int > &new_frequency, Vector< int > &new_data );
  template void Signal::Data( Vector< int > val );

  template Signal::Signal( Vector< llint > &new_frequency, double data_minimum, double data_step );
  template Signal::Signal( Vector< llint > &new_frequency, Vector< llint > &new_data );
  template void Signal::Data( Vector< llint > val );

  template Signal::Signal( Vector< float > &new_frequency, double data_minimum, double data_step );
  template Signal::Signal( Vector< float > &new_frequency, Vector< float > &new_data );
  template void Signal::Data( Vector< float > val );

  template Signal::Signal( Vector< double > &new_frequency, double data_minimum, double data_step );
  template Signal::Signal( Vector< double > &new_frequency, Vector< double > &new_data );
  template void Signal::Data( Vector< double > val );

  template void Signal::Print( std::ostream &os ) const;
  template void Signal::Print( OFile &os ) const;

#endif

}

#endif

#endif
