/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/06 
 * @brief
 * Content: Bucket-sort queue class 
 * <br> Description: A bucket-sort queue. It is useful to sort a large set
 * of elements whose weight domain is not very large,
 * e.g. IFT and OPF algorithms over MR images. 
 * <br> Future add-on's: Deal with negative values. 
 *                  Add fast operation functions in which queue does not grow. 
 * Warning: In Boost library, there is a class named circular_buffer
 * that may do exactly the same as Vector< WeightNode >. 
 */

#ifndef BIALDISTANCEBUCKETQUEUE_C
#define BIALDISTANCEBUCKETQUEUE_C

#include "DistanceBucketQueue.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_DistanceBucketQueue )
#define BIAL_EXPLICIT_DistanceBucketQueue
#endif

#if defined ( BIAL_EXPLICIT_DistanceBucketQueue ) || ( BIAL_IMPLICIT_BIN )

namespace Bial {

  DistanceBucketQueue::DistanceBucketQueue( size_t size, size_t max_dist ) try :
    identity( size, IdentityNode( ) ), weight( max_dist + 1, WeightNode( ) ), elements( 0 ), minimum( 0 ) {
      COMMENT( "Creating queue with " << weight.size( ) << " elements.", 3 );
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

  bool DistanceBucketQueue::Empty( ) const {
    if( elements == 0 ) {
      return( true );
    }
    return( false );
  }

  size_t DistanceBucketQueue::Elements( ) const {
    return( elements );
  }

  BucketState DistanceBucketQueue::State( size_t idt ) const {
    try {
      return( identity( idt ).state );
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

  void DistanceBucketQueue::State( size_t idt, BucketState new_state ) {
    try {
      identity( idt ).state = new_state;
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

  void DistanceBucketQueue::Finished( size_t idt ) {
    try {
      identity( idt ).state = BucketState::REMOVED;
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

  size_t DistanceBucketQueue::Buckets( ) const {
    return( weight.size( ) );
  }

  void DistanceBucketQueue::ResetMinimum( ) {
    minimum = 0;
  }

  void DistanceBucketQueue::Reset( ) {
    size_t size = identity.size( );
    for( size_t elm = 0; elm < size; ++ elm )
      identity[ elm ].state = BucketState::NOT_VISITED;
    minimum = 0;
  }

  void DistanceBucketQueue::Insert( size_t idt, int weight_idx ) {
    try {
      COMMENT( "Current amount of elements prior to insertion: " << elements, 3 );
      COMMENT( "Inserting element: " << idt << ", with weight index: " << weight_idx << ".", 4 );
      IF_DEBUG( ( identity( idt ).state == BucketState::INSERTED ) ||
                ( identity( idt ).state == BucketState::UPDATED ) ) {
        std::string msg( BIAL_ERROR( "Inserting element " + std::to_string( idt ) + " that is already in queue." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Inserting! elements: " << elements << ", idt: " << idt << ", minimum: " << minimum <<
               ", weight_idx: " << weight_idx << ", first: " << weight( weight_idx ).first  <<
               ", last: " << weight( weight_idx ).last, 3 );
      ++elements;
      if( weight( weight_idx ).first == -1 ) {
        COMMENT( "First element in bucket.", 4 );
        weight( weight_idx ).first = idt;
        identity( idt ).prev = -1;
      }
      else {
        COMMENT( "Last element in bucket.", 4 );
        identity( weight( weight_idx ).last ).next = idt;
        identity( idt ).prev = weight( weight_idx ).last;
      }
      weight( weight_idx ).last = idt;
      identity( idt ).next = -1;
      identity( idt ).state = BucketState::INSERTED;
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

  size_t DistanceBucketQueue::Remove( ) {
    try {
      COMMENT( "Current amount of elements prior to removal: " << elements, 3 );
      IF_DEBUG( elements == 0 ) {
        std::string msg( BIAL_ERROR( "Removing element from empty queue." ) );
        throw( std::underflow_error( msg ) );
      }
      COMMENT( "Removing! elements: " << elements << ", minimum: " << minimum, 3 );
      COMMENT( "Finding next element or returning EMPTY bucket queue.", 4 );
      while( weight( minimum ).first == -1 )
        ++minimum;
      COMMENT( "Found index: " << minimum, 3 );
      --elements;
      int idt = weight( minimum ).first;
      int next = identity( idt ).next;
      COMMENT( "Removing idt: " << idt << ", weight_idx: " << minimum << ", prev: " << 
               identity( idt ).prev << ", next:" << next, 3 );
      weight( minimum ).first = next;
      if( next == -1 )
        weight( minimum ).last = -1;
      else
        identity( next ).prev = -1;
      COMMENT( "Remove done.", 3 );
      return( idt );
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

  void DistanceBucketQueue::Remove( size_t idt, int weight_idx ) {
    try {
      COMMENT( "Current amount of elements prior to index removal: " << elements, 3 );
      COMMENT( "Check if queue is initialized.", 4 );
      IF_DEBUG( elements == 0 ) {
        std::string msg( BIAL_ERROR( "Removing element from empty queue." ) );
        throw( std::underflow_error( msg ) );
      }
      COMMENT( "Removing element " << idt << " with weight " << weight_idx << ".", 3 );
      int prev = identity( idt ).prev;
      int next = identity( idt ).next;
      COMMENT( "Removing for update! elements: " << elements << ", minimum: " << minimum << ", idt: " << idt
               << ", weight_idx: " << weight_idx << ", previous: " << prev << ", next:" << next 
               << ", first: " << weight( weight_idx ).first, 3 );
      --elements;
      if( prev == -1 ) {
        COMMENT( idt << " << is the first element.", 4 );
        weight( weight_idx ).first = next;
      }
      else {
        COMMENT( idt << " is not the first element.", 4 );
        identity( prev ).next = next;
      }
      if( next == -1 ) {
        COMMENT( "idt is the last element.", 4 );
        weight( weight_idx ).last = prev;
      }
      else {
        COMMENT( "idt is not the last element.", 4 );
        identity( next ).prev = prev;
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

  void DistanceBucketQueue::Update( size_t idt, int cur_wgt, int new_wgt ) {
    try {
      if( ( identity( idt ).state == BucketState::INSERTED ) || 
          ( identity( idt ).state == BucketState::UPDATED ) ) {
        COMMENT( "Updating element: " << idt << " from " << cur_wgt << " to " << new_wgt << ".", 3 );
        Remove( idt, cur_wgt );
      }
      else {
        COMMENT( "Inserting element: " << idt << " with weight: " << new_wgt << ".", 3 );
      }
      identity( idt ).state = BucketState::REMOVED;
      Insert( idt, new_wgt );
      identity( idt ).state = BucketState::UPDATED;
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

#endif

#endif
