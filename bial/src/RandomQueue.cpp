/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @file RandomQueue.hpp
 * @author Fábio Augusto Menocci Cappabianco 
 * @date 2012/Sep/12 
 * @version 1.0.00
 * @brief
 * Content: Random-access queue class declaration. 
 * <br> Description: A random-acess queue. Its size is fixed. As soon as it gets full, the first inserted element is
 * automatically replaced by the next inserted element.<br> 
 * It is useful to find the median element as walking through signal or image data, e.g. MedianFilter method of 
 * Signal class. 
 * Insertion and removal are made in limiar time with respect to the queue size. <br>
 *  Access to ordered keys by position is made in constant time. 
 * <br> Future add-on's: 
 * - Optimize Update method by usign a binary search for the key.
 * - A constructor with maximum size, sorting order, and an initial vector, sorted or not sorted. 
 */

#ifndef BIALRANDOMQUEUE_C
#define BIALRANDOMQUEUE_C

#include "RandomQueue.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_RandomQueue )
#define BIAL_EXPLICIT_RandomQueue
#endif

#if defined ( BIAL_EXPLICIT_RandomQueue ) || ( BIAL_IMPLICIT_BIN )

namespace Bial {

  RandomQueue::RandomQueue( size_t size, bool increasing_order ) try : _data( size,
                                                                              0.0 ),
    order( size, 0 ), qk_data( nullptr ), occupancy( 0 ), last( 0 ), increasing( increasing_order ) {
    qk_data = _data.data( );
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

  double RandomQueue::operator[]( size_t elm ) const {
    try {
      return( QK_DATA( elm ) );
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

  size_t RandomQueue::Size( ) const {
    return( _data.size( ) );
  }

  size_t RandomQueue::Capacity( ) const {
    return( occupancy );
  }

  void RandomQueue::Insert( double key ) {
    try {
      size_t pos = last + occupancy;
      if( occupancy < _data.size( ) ) {
        COMMENT( "Queue is not full. Just insert.", 2 );
        COMMENT( "Inserting key.", 2 );
        QK_DATA( occupancy ) = key;
        COMMENT( "Inserting position.", 2 );
        order( occupancy ) = pos;
        COMMENT( "Sorting.", 2 );
        for( size_t it = occupancy;
             ( it > 0 ) && ( ( ( QK_DATA( it - 1 ) > QK_DATA( it ) ) && ( increasing ) ) ||
                             ( ( QK_DATA( it - 1 ) < QK_DATA( it ) ) && ( !increasing ) ) ); --it ) {
          std::swap( QK_DATA( it - 1 ), QK_DATA( it ) );
          std::swap( order( it - 1 ), order( it ) );
        }
        COMMENT( "Incriase occupancy.", 2 );
        ++occupancy;
        COMMENT( "Finished.", 2 );
      }
      else {
        COMMENT( "Queue is full. Need to replace.", 2 );
        size_t it = occupancy - 1;
        while( ( order( it ) != last ) && ( ( ( key <= QK_DATA( it ) ) && ( increasing ) ) ||
                                            ( ( key >= QK_DATA( it ) ) && ( !increasing ) ) ) ) {
          --it;
        }
        if( order( it ) == last ) {
          std::swap( QK_DATA( it ), key );
          std::swap( order( it ), pos );
          while( ( it > 0 ) && ( ( ( QK_DATA( it - 1 ) > QK_DATA( it ) ) && ( increasing ) ) ||
                                 ( ( QK_DATA( it - 1 ) < QK_DATA( it ) ) && ( !increasing ) ) ) ) {
            std::swap( QK_DATA( it - 1 ), QK_DATA( it ) );
            std::swap( order( it - 1 ), order( it ) );
            --it;
          }
        }
        else {
          while( order( it ) != last ) {
            std::swap( QK_DATA( it ), key );
            std::swap( order( it ), pos );
            --it;
          }
          std::swap( QK_DATA( it ), key );
          std::swap( order( it ), pos );
        }
        ++last;
      }
      COMMENT( "Checking for size overflow.", 2 );
      COMMENT( "last:" << last, 2 );
      COMMENT( "data size: " << _data.size( ), 2 );
      if( last + _data.size( ) == std::numeric_limits< size_t >::max( ) - 1 ) {
        COMMENT( "Avoiding 'last' variable overflow.", 2 );
        for( size_t it = 0; it < occupancy; ++it ) {
          order( it ) -= last;
        }
        last = 0;
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

  double RandomQueue::Remove( ) {
    try {
      if( occupancy == 0 ) {
        return( 0 );
      }
      double key = QK_DATA( occupancy - 1 );
      size_t pos = order( occupancy - 1 );
      size_t it = occupancy - 1;
      for( ; order( it ) != last; --it ) {
        std::swap( QK_DATA( it ), key );
        std::swap( order( it ), pos );
      }
      std::swap( QK_DATA( it ), key );
      std::swap( order( it ), pos );
      last++;
      occupancy--;
      if( last + _data.size( ) == std::numeric_limits< size_t >::max( ) - 1 ) {
        COMMENT( "Avoiding 'last' variable overflow.", 2 );
        for( it = 0; it < occupancy; ++it ) {
          order( it ) -= last;
        }
        last = 0;
      }
      return( key );
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

  void RandomQueue::Update( double key, double new_key ) {
    try {
      size_t it = occupancy - 1;
      COMMENT( "Searching for key.", 2 );
      for( ; ( it > 0 ) && ( QK_DATA( it ) != key ); --it ) {
      }
      if( QK_DATA( it ) == key ) {
        COMMENT( "Found key. Update it and, if necessary, reorder queue.", 2 );
        std::swap( QK_DATA( it ), new_key );
        for( ++it; ( it < occupancy ) && ( ( ( QK_DATA( it ) < QK_DATA( it - 1 ) ) && ( increasing ) )
                                           || ( ( QK_DATA( it ) > QK_DATA( it - 1 ) ) && ( !increasing ) ) ); ++it ) {
          std::swap( QK_DATA( it ), QK_DATA( it - 1 ) );
          std::swap( order( it ), order( it - 1 ) );
        }
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

  double RandomQueue::Minimum( ) const {
    try {
      return( QK_DATA( 0 ) );
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

  double RandomQueue::Median( ) const {
    try {
      return( ( QK_DATA( ( occupancy - 1 ) / 2 ) + QK_DATA( occupancy / 2 ) ) / 2.0 );
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

  double RandomQueue::Maximum( ) const {
    try {
      return( QK_DATA( occupancy - 1 ) );
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

  bool RandomQueue::Full( ) {
    return( occupancy == _data.size( ) );
  }

  bool RandomQueue::Empty( ) {
    return( occupancy == 0 );
  }

  /*
   * template< class O >
   * void RandomQueue::Print( O& os ) {
   *   try {
   *     for( size_t it = 0; it < occupancy; ++it )
   *       os << QK_DATA( it ) << " ";
   *     os << std::endl;
   *   }
   *   catch( const std::out_of_range& e ) {
   *     std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
   *     throw( std::out_of_range( msg ) );
   *   }
   *   catch( const std::logic_error& e ) {
   *     std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
   *     throw( std::logic_error( msg ) );
   *   }
   * }
   */

}

#endif

#endif
