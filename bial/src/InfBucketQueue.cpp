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

#ifndef BIALINFBUCKETQUEUE_C
#define BIALINFBUCKETQUEUE_C

#include "InfBucketQueue.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_InfBucketQueue )
#define BIAL_EXPLICIT_InfBucketQueue
#endif

#if defined ( BIAL_EXPLICIT_InfBucketQueue ) || ( BIAL_IMPLICIT_BIN )

namespace Bial {

  InfBucketQueue::InfBucketQueue( size_t size, llint bucket_size, llint inf_val, bool increasing_order, bool fifo_tie )
  try : identity( Vector< IdentityNode >( size, IdentityNode( ) ) ), 
          weight( Vector< WeightNode >( std::min( static_cast< size_t >( 256 ), sizeof( ldbl ) * 255 + 1 ),
                                        WeightNode( ) ) ), minimum( 0 ), maximum( 0 ), delta( bucket_size ), 
          elements( 0 ), increasing( increasing_order ), fifo( fifo_tie ), inf_elements( 0 ), infinite_val( inf_val ) {
    COMMENT( "Created with size : " << weight.size( ), 1 );
    COMMENT( "Infinite value : " << infinite_val, 1 );
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

  size_t InfBucketQueue::Index( llint value ) {
    if( value == infinite_val ) {
      return( weight.size( ) - 1 );
    }
    return( static_cast< size_t >( value / delta ) % ( weight.size( ) - 1 ) );
  }

  void InfBucketQueue::Grow( size_t new_size ) {
    try {
      COMMENT( "Growing queue. Current size: " << weight.size( ) << ", new size: " << new_size << ".", 3 );
      COMMENT( "Getting minimum weight, maximum weight, and elements for growing queue.", 4 );

      COMMENT( "Converting current data to new weight vector.", 4 );
      Vector< WeightNode > new_weight = Vector< WeightNode >( new_size, WeightNode( ) );
      llint wgt = minimum;
      size_t last_idx = Index( wgt );
      size_t cur_idx = Index( wgt );
      COMMENT( "last_idx: " << last_idx, 4 );
      do {
        COMMENT( "Getting new index position.", 4 );
        size_t new_idx = static_cast< size_t >( wgt / delta ) % ( new_size - 1 );
        COMMENT( "Updating index from: " << cur_idx << " to " << new_idx << ".", 4 );
        new_weight( new_idx ).first = weight( cur_idx ).first;
        new_weight( new_idx ).last = weight( cur_idx ).last;
        COMMENT( "first: " << weight( cur_idx ).first << ", last: " << weight( cur_idx ).last << ".", 4 );
        wgt += delta;
        cur_idx = Index( wgt );
      } while( last_idx != cur_idx );
      COMMENT( "Updating infinite elements.", 4 );
      cur_idx = weight.size( ) - 1;
      size_t new_idx = new_size - 1;
      COMMENT( "Updating index from: " << cur_idx << " to " << new_idx << ".", 4 );
      new_weight( new_idx ).first = weight( cur_idx ).first;
      new_weight( new_idx ).last = weight( cur_idx ).last;
      COMMENT( "first: " << weight( cur_idx ).first << ", last: " << weight( cur_idx ).last << ".", 4 );

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

  void InfBucketQueue::Insert( size_t idt, llint wgt ) {
    try {
      COMMENT( "Current amount of elements prior to insertion: " << elements, 3 );
      COMMENT( "Inserting element: " << idt << ", with weight: " << wgt << ".", 4 );
      if( ( identity( idt ).state == BucketState::INSERTED ) ||
          ( identity( idt ).state == BucketState::UPDATED ) ) {
        std::string msg( BIAL_ERROR( "Inserting element that is already in queue." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Setting minimum and maximum values in queue.", 4 );
      if( wgt != infinite_val ) {
        if( elements - inf_elements == 0 ) {
          COMMENT( "First finite element.", 4 );
          minimum = wgt;
          maximum = minimum;
        }
        else {
          COMMENT( "Verify weight bounds and eventual growth.", 4 );
          llint new_min = std::min( wgt, minimum );
          llint new_max = std::max( wgt, maximum );
          size_t new_size = 2.0 + ( new_max - new_min ) / delta;
          if( new_size > 1000000 ) {
            if( new_size > 10000000 ) {
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
      }
      COMMENT( "Inserting element.", 4 );
      size_t weight_idx = Index( wgt );
      COMMENT( "Inserting! elements: " << elements << ", infinite elements: " << inf_elements << ", idt: " << idt
               << ", minimum: " << minimum << ", maximum: " << maximum << ", weight_idx: " << weight_idx <<
               ", wgt: " << wgt << ", first: " << weight( weight_idx ).first << ", last: " << 
               weight( weight_idx ).last, 3 );
      COMMENT( "Updating the number of elements.", 4 );
      if( wgt == infinite_val ) {
        COMMENT( "Inserted infinite element.", 4 );
        ++inf_elements;
      }
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

  int InfBucketQueue::Remove( ) {
    try {
      COMMENT( "Current amount of elements prior to removal: " << elements, 3 );
      COMMENT( "Check if queue is initialized.", 4 );
      if( elements == 0 ) {
        std::string msg( BIAL_ERROR( "Removing element from empty queue." ) );
        throw( std::underflow_error( msg ) );
      }
      COMMENT( "Removing! elements: " << elements << ", infinite elements: " << inf_elements << ", minimum: " <<
               minimum << ", maximum: " << maximum, 3 );
      COMMENT( "Finding next element.", 4 );
      size_t current_weight;
      if( elements == inf_elements ) {
        COMMENT( "Removed infinite element. Elements: " << elements << ", infs: " << inf_elements, 1 );
        --inf_elements;
        current_weight = weight.size( ) - 1;
      }
      else if( increasing ) {
        current_weight = Index( minimum );
        while( weight( current_weight ).first == -1 ) {
          minimum += delta;
          current_weight = Index( minimum );
        }
      }
      else {
        current_weight = Index( maximum );
        while( weight( current_weight ).first == -1 ) {
          maximum -= delta;
          current_weight = Index( maximum );
        }
      }
      COMMENT( "Found index: " << current_weight, 3 );
      --elements;
      int idt = weight( current_weight ).first;
      COMMENT( "idt: " << idt, 3 );
      int next = identity( idt ).next;
      COMMENT( "Removing idt: " << idt << ", weight_idx: " << current_weight << ", prev: " << 
               identity( idt ).prev << ", next:" << next, 3 );
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

  void InfBucketQueue::Remove( size_t idt, llint wgt ) {
    try {
      COMMENT( "Current amount of elements prior to index removal: " << elements, 3 );
      COMMENT( "Check if queue is initialized.", 4 );
      if( elements == 0 ) {
        std::string msg( BIAL_ERROR( "Removing element from empty queue." ) );
        throw( std::underflow_error( msg ) );
      }
      COMMENT( "Removing element " << idt << " with weight " << wgt << ".", 3 );
      size_t weight_idx = Index( wgt );
      int prev = identity( idt ).prev;
      int next = identity( idt ).next;
      COMMENT( "Removing for update! elements: " << elements << ", infinite elements: " << inf_elements <<
               ", minimum: " << minimum << ", maximum: " << maximum << ", idt: " << idt << ", wgt: " << 
               wgt << ", weight_idx: " << weight_idx << ", previous: " << prev << ", next:" << next << ", first: " << 
               weight( weight_idx ).first, 3 );
      --elements;
      if( wgt == infinite_val ) {
        COMMENT( "Removed infinite element. Elements: " << elements << ", infs: " << inf_elements, 3 );
        --inf_elements;
      }
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

  void InfBucketQueue::Update( size_t idt, llint cur_wgt, llint new_wgt ) {
    try {
      if( ( identity( idt ).state == BucketState::INSERTED ) ||
          ( identity( idt ).state == BucketState::UPDATED ) ) {
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

  bool InfBucketQueue::Empty( ) {
    if( elements == 0 ) {
      return( true );
    }
    return( false );
  }

  size_t InfBucketQueue::Elements( ) {
    return( elements );
  }

  size_t InfBucketQueue::InfiniteElements( ) {
    return( inf_elements );
  }

  bool InfBucketQueue::Increasing( ) const {
    return( increasing );
  }

  BucketState InfBucketQueue::State( size_t idt ) const {
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

  void InfBucketQueue::State( size_t idt, BucketState new_state ) {
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

  void InfBucketQueue::Finished( size_t idt ) {
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

  size_t InfBucketQueue::Buckets( ) const {
    return( weight.size( ) );
  }

  void InfBucketQueue::ResetState( ) {
    size_t size = identity.size( );
    for( size_t elm = 0; elm < size; ++ elm )
      identity[ elm ].state = BucketState::NOT_VISITED;
  }

}

#endif

#endif
