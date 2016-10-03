/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2012/Sep/06 */
/* Version: 1.0.00 */
/* Content: Bucket-sort queue class */
/* Description: A bucket-sort queue. It is useful to sort a large set
 * of elements whose weight domain is not very large,
 * e.g. IFT and OPF algorithms over MR images. */
/* Future add-on's: Deal with negative values. */
/*                  Add fast operation functions in which queue does not grow. */
/* Warning: In Boost library, there is a class named circular_buffer
 * that may do exactly the same as Vector< WeightNode >. */

#ifndef BIALBUCKETQUEUE_C
#define BIALBUCKETQUEUE_C

#include "BucketQueue.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_BucketQueue )
#define BIAL_EXPLICIT_BucketQueue
#endif

#if defined ( BIAL_EXPLICIT_BucketQueue ) || ( BIAL_IMPLICIT_BIN )

namespace Bial {

  BucketQueue::BucketQueue( size_t size, ldbl bucket_size, bool increasing_order, bool fifo_tie ) try
    : identity( Vector< IdentityNode >( size, IdentityNode( ) ) ), 
        weight( Vector< WeightNode >( std::min( static_cast< size_t >( 256 ), sizeof( ldbl ) * 255 + 1 ),
                                      WeightNode( ) ) ), minimum( 0.0L ), maximum( 0.0L ), delta( bucket_size ), 
        elements( 0 ), increasing( increasing_order ), fifo( fifo_tie ) {
      COMMENT( "Created with size : " << weight.size( ), 3 );
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

  size_t BucketQueue::Index( ldbl value ) {
    return( static_cast< size_t >( value / delta ) % weight.size( ) );
  }

  void BucketQueue::Grow( size_t new_size ) {
    try {
      COMMENT( "Growing queue. Current size: " << weight.size( ) << ", new size: " << new_size << ".", 3 );
      ldbl wgt = minimum;
      size_t last_idx = Index( wgt );
      size_t cur_idx = Index( wgt );
      COMMENT( "last_idx: " << last_idx, 4 );
      COMMENT( "Converting current data to new weight vector.", 4 );
      Vector< WeightNode > new_weight = Vector< WeightNode >( new_size, WeightNode( ) );
      do {
        size_t new_idx = static_cast< size_t >( wgt / delta ) % new_size;
        COMMENT( "Updating index from: " << cur_idx << " to " << new_idx << ".", 3 );
        new_weight( new_idx ).first = weight( cur_idx ).first;
        new_weight( new_idx ).last = weight( cur_idx ).last;
        COMMENT( "first: " << weight( cur_idx ).first << ", last: " << weight( cur_idx ).last << ".", 4 );
        wgt += delta;
        cur_idx = Index( wgt );
      } while( last_idx != cur_idx );
      weight = new_weight;
      COMMENT( "weight.size( ): " << weight.size( ), 4 );
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

  void BucketQueue::Insert( size_t idt, ldbl wgt ) {
    try {
      COMMENT( "Current amount of elements prior to insertion: " << elements, 3 );
      COMMENT( "Inserting element: " << idt << ", with weight: " << wgt << ".", 4 );
      if( ( identity( idt ).state == BucketState::INSERTED ) || ( identity( idt ).state == BucketState::UPDATED ) ) {
        std::string msg( BIAL_ERROR( "Inserting element that is already in queue." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Setting minimum and maximum values in queue.", 4 );
      if( elements == 0 ) {
        COMMENT( "First element.", 4 );
        minimum = wgt;
        maximum = minimum;
      }
      else {
        COMMENT( "Verify weight bounds and eventual growth.", 4 );
        ldbl new_min = std::min( wgt, minimum );
        ldbl new_max = std::max( wgt, maximum );
        if( ( ( new_min != minimum ) && ( new_min == std::numeric_limits< ldbl >::lowest( ) ) ) ||
            ( ( new_max != maximum ) && ( new_max == std::numeric_limits< ldbl >::max( ) ) ) ) {
          std::cerr <<
          "Warning: Bucket queue limits may have being exceeded. Queue ordering may be incorrect or cause "
                    << "unexpected errors. Try to fix your programa or to use a data type with greater capacity."
                    << std::endl;
        }
        ldbl new_size = 1.0L + ( new_max - new_min ) / delta;
        if( new_size > 1000000.0L ) {
          if( new_size > 10000000.0L ) {
            std::string msg( BIAL_ERROR( std::string( "BucketQueue is too big. This will take forever to run. " ) +
                                         "Fix your program. Minimum: " + std::to_string( new_min ) + " Maximum: " + 
                                         std::to_string( new_max ) + " wgt: " + std::to_string( wgt ) ) );
            throw( std::runtime_error( msg ) );
          }
          BIAL_WARNING( std::string( "BucketQueue is becaming huge. This may take forever to run. " ) +
                        "Fix your program or continue at your own risk. Minimum: " + std::to_string( new_min ) +
                        " Maximum: " + std::to_string( new_max ) + " wgt: " + std::to_string( wgt ) );
        }
        if( weight.size( ) <= new_size ) {
          COMMENT( "Growing queue because " << weight.size( ) << " <= " << new_size << ".", 3 );
          Grow( new_size + 10000 );
        }
        minimum = new_min;
        maximum = new_max;
      }
      COMMENT( "Inserting element.", 4 );
      size_t weight_idx = Index( wgt );
      COMMENT( "Inserting! elements: " << elements << ", idt: " << idt << ", minimum: " << minimum << ", maximum: " <<
               maximum << ", weight_idx: " << weight_idx << ", wgt: " << wgt << ", first: " << weight(
                 weight_idx ).first
               << ", last: " << weight( weight_idx ).last, 3 );
      ++elements;
      if( fifo ) {
        if( weight( weight_idx ).first == -1 ) {
          COMMENT( "First element in bucket.", 4 );
          weight( weight_idx ).first = idt;
          identity( idt ).prev = -1;
        }
        else {
          COMMENT( "Middle element in bucket.", 4 );
          identity( weight( weight_idx ).last ).next = idt;
          identity( idt ).prev = weight( weight_idx ).last;
        }
        weight( weight_idx ).last = idt;
        identity( idt ).next = -1;
      }
      else { /* LIFO */
        if( weight( weight_idx ).first == -1 ) {
          COMMENT( "First element in bucket.", 4 );
          weight( weight_idx ).last = idt;
          identity( idt ).next = -1;
        }
        else {
          COMMENT( "Middle element in bucket.", 4 );
          identity( weight( weight_idx ).first ).prev = idt;
          identity( idt ).next = weight( weight_idx ).first;
        }
        weight( weight_idx ).first = idt;
        identity( idt ).prev = -1;
      }
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

  int BucketQueue::Remove( ) {
    try {
      COMMENT( "Current amount of elements prior to removal: " << elements, 3 );
      COMMENT( "Check if queue is initialized.", 4 );
      if( elements == 0 ) {
        std::string msg( BIAL_ERROR( "Removing element from empty queue." ) );
        throw( std::underflow_error( msg ) );
      }
      COMMENT( "Removing! elements: " << elements << ", minimum: " << minimum << ", maximum: " << maximum, 3 );
      COMMENT( "Finding next element or returning EMPTY bucket queue.", 4 );
      size_t current_weight;
      if( increasing ) {
        current_weight = Index( minimum );
        COMMENT( "Testing weight index: " << current_weight, 3 );
        while( weight( current_weight ).first == -1 ) {
          minimum += delta;
          current_weight = Index( minimum );
        }
      }
      else {
        current_weight = Index( maximum );
        COMMENT( "Testing weight index: " << current_weight, 3 );
        while( weight( current_weight ).first == -1 ) {
          maximum -= delta;
          current_weight = Index( maximum );
        }
      }
      COMMENT( "Found index: " << current_weight, 3 );
      --elements;
      int idt = weight( current_weight ).first;
      int next = identity( idt ).next;
      COMMENT( "Removing idt: " << idt << ", weight_idx: " << current_weight << ", prev: " << identity( idt ).prev <<
               ", next:" << next, 3 );
      weight( current_weight ).first = next;
      if( next == -1 ) {
        weight( current_weight ).last = -1;
      }
      else {
        identity( next ).prev = -1;
      }
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

  void BucketQueue::Remove( size_t idt, ldbl wgt ) {
    try {
      COMMENT( "Current amount of elements prior to index removal: " << elements, 3 );
      COMMENT( "Check if queue is initialized.", 4 );
      if( elements == 0 ) {
        return;
      }
      COMMENT( "Removing element " << idt << " with weight " << wgt << ".", 3 );
      size_t weight_idx = Index( wgt );
      int prev = identity( idt ).prev;
      int next = identity( idt ).next;
      COMMENT( "Removing for update! elements: " << elements << ", minimum: " << minimum << ", maximum: " <<
               maximum << ", idt: " << idt << ", wgt: " << wgt << ", weight_idx: " << weight_idx << ", previous: " <<
               prev << ", next:" << next << ", first: " << weight( weight_idx ).first, 3 );
      --elements;
      if( weight( weight_idx ).first == static_cast< int >( idt ) ) {
        COMMENT( "idt is the first element.", 4 );
        weight( weight_idx ).first = next;
        if( next == -1 ) {
          COMMENT( "Idt is also the last one.", 4 );
          weight( weight_idx ).last = -1;
        }
        else {
          identity( next ).prev = -1;
        }
      }
      else {
        COMMENT( "idt is in the middle or it is the last.", 4 );
        identity( prev ).next = next;
        if( next == -1 ) {
          COMMENT( "idt is the last element.", 4 );
          weight( weight_idx ).last = prev;
        }
        else {
          COMMENT( "idt in the middle.", 4 );
          identity( next ).prev = prev;
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

  void BucketQueue::Update( size_t idt, ldbl cur_wgt, ldbl new_wgt ) {
    try {
      if( ( identity( idt ).state == BucketState::INSERTED ) || ( identity( idt ).state == BucketState::UPDATED ) ) {
        COMMENT( "Updating element: " << idt << " from " << cur_wgt << " to " << new_wgt << ".", 3 );
        Remove( idt, cur_wgt );
      }
      else {
        COMMENT( "Inserting element: " << idt << " from " << cur_wgt << " to " << new_wgt << ".", 3 );
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

  bool BucketQueue::Empty( ) {
    if( elements == 0 ) {
      return( true );
    }
    return( false );
  }

  size_t BucketQueue::Elements( ) {
    return( elements );
  }

  bool BucketQueue::Increasing( ) const {
    return( increasing );
  }

  BucketState BucketQueue::State( size_t idt ) const {
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

  void BucketQueue::State( size_t idt, BucketState new_state ) {
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

  void BucketQueue::Finished( size_t idt ) {
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

  size_t BucketQueue::Buckets( ) const {
    return( weight.size( ) );
  }

  void BucketQueue::ResetState( ) {
    size_t size = identity.size( );
    for( size_t elm = 0; elm < size; ++ elm )
      identity[ elm ].state = BucketState::NOT_VISITED;
  }

}

#endif

#endif
