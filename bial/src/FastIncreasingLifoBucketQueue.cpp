/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2016/Dec/05
 * @brief
 * Content: Fast implementation of Bucket-sort queue class 
 * <br> Description: A bucket-sort queue. It is useful to sort a large set of elements whose weight domain is not very 
 * large, * e.g. IFT and OPF algorithms over MR images. 
 * Warning: In Boost library, there is a class named circular_buffer
 * that may do exactly the same as Vector< WeightNode >. 
 */

#ifndef BIALFASTINCREASINGLIFOBUCKETQUEUE_C
#define BIALFASTINCREASINGLIFOBUCKETQUEUE_C

#include "FastIncreasingLifoBucketQueue.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_FastIncreasingLifoBucketQueue )
#define BIAL_EXPLICIT_FastIncreasingLifoBucketQueue
#endif

#if defined ( BIAL_EXPLICIT_FastIncreasingLifoBucketQueue ) || ( BIAL_IMPLICIT_BIN )

namespace Bial {

  FastIncreasingLifoBucketQueue::FastIncreasingLifoBucketQueue( size_t size, ldbl minimum_value, size_t n_buckets ) try
    : BucketQueue( size, true, false ), minimum( minimum_value ), maximum( minimum_value + n_buckets - 1 ),
        current( minimum_value ) {
      this->weight = Vector< WeightNode >( n_buckets );
      COMMENT( "Created with size : " << this->weight.size( ), 3 );
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

  size_t FastIncreasingLifoBucketQueue::Index( ldbl value ) {
    return( static_cast< size_t >( value - minimum ) );
  }

  void FastIncreasingLifoBucketQueue::Insert( size_t idt, ldbl wgt ) {
    try {
      COMMENT( "Current amount of elements prior to insertion: " << this->elements, 3 );
      COMMENT( "Inserting element: " << idt << ", with weight: " << wgt << ".", 4 );
      // Trocar esse if por um if condicional de debug.
      // if( ( this->identity( idt ).state == BucketState::INSERTED ) || 
      //     ( this->identity( idt ).state == BucketState::UPDATED ) ) {
      //   std::string msg( BIAL_ERROR( "Inserting element that is already in queue." ) );
      //   throw( std::logic_error( msg ) );
      // }
      size_t weight_idx = static_cast< size_t >( wgt - minimum );
      COMMENT( "Inserting! elements: " << this->elements << ", idt: " << idt << ", minimum: " << minimum << 
               ", maximum: " << maximum << ", weight_idx: " << weight_idx << ", wgt: " << wgt << ", first: " <<
               this->weight( weight_idx ).first  << ", last: " << this->weight( weight_idx ).last, 3 );
      ++( this->elements );
      if( this->weight( weight_idx ).first == -1 ) {
        COMMENT( "First element in bucket.", 4 );
        this->weight( weight_idx ).last = idt;
        this->identity( idt ).next = -1;
      }
      else {
        COMMENT( "Middle element in bucket.", 4 );
        this->identity( this->weight( weight_idx ).first ).prev = idt;
        this->identity( idt ).next = this->weight( weight_idx ).first;
      }
      this->weight( weight_idx ).first = idt;
      this->identity( idt ).prev = -1;
      if( current > weight_idx )
        current = weight_idx;
      this->identity( idt ).state = BucketState::INSERTED;
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

  int FastIncreasingLifoBucketQueue::Remove( ) {
    try {
      COMMENT( "Current amount of elements prior to removal: " << this->elements, 3 );
      COMMENT( "Check if queue is initialized.", 4 );
      // Trocar if por if debug.
      // if( this->elements == 0 ) {
      //   std::string msg( BIAL_ERROR( "Removing element from empty queue." ) );
      //   throw( std::underflow_error( msg ) );
      // }
      COMMENT( "Removing! elements: " << this->elements << ", minimum: " << minimum << ", maximum: " << maximum, 3 );
      COMMENT( "Finding next element or returning EMPTY bucket queue.", 4 );
      COMMENT( "Testing weight index: " << current, 3 );
      while( this->weight( current ).first == -1 )
        ++current;
      COMMENT( "Found index: " << current, 3 );
      --this->elements;
      int idt = this->weight( current ).first;
      int next = this->identity( idt ).next;
      COMMENT( "Removing idt: " << idt << ", weight_idx: " << current << ", prev: " << 
               this->identity( idt ).prev << ", next:" << next, 3 );
      this->weight( current ).first = next;
      if( next == -1 )
        this->weight( current ).last = -1;
      else
        this->identity( next ).prev = -1;
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

  void FastIncreasingLifoBucketQueue::Remove( size_t idt, ldbl wgt ) {
    try {
      COMMENT( "Current amount of elements prior to index removal: " << this->elements, 3 );
      COMMENT( "Check if queue is initialized.", 4 );
      if( this->elements == 0 ) {
        return;
      }
      COMMENT( "Removing element " << idt << " with weight " << wgt << ".", 3 );
      size_t weight_idx = static_cast< size_t >( wgt - minimum );
      int prev = this->identity( idt ).prev;
      int next = this->identity( idt ).next;
      COMMENT( "Removing for update! elements: " << this->elements << ", minimum: " << minimum << ", maximum: " <<
               maximum << ", idt: " << idt << ", wgt: " << wgt << ", weight_idx: " << weight_idx << ", previous: " <<
               prev << ", next:" << next << ", first: " << this->weight( weight_idx ).first, 3 );
      --( this->elements );
      if( this->weight( weight_idx ).first == static_cast< int >( idt ) ) {
        COMMENT( "idt is the first element.", 4 );
        this->weight( weight_idx ).first = next;
        if( next == -1 ) {
          COMMENT( "Idt is also the last one.", 4 );
          this->weight( weight_idx ).last = -1;
        }
        else
          this->identity( next ).prev = -1;
      }
      else {
        COMMENT( "idt is in the middle or it is the last.", 4 );
        this->identity( prev ).next = next;
        if( next == -1 ) {
          COMMENT( "idt is the last element.", 4 );
          this->weight( weight_idx ).last = prev;
        }
        else {
          COMMENT( "idt in the middle.", 4 );
          this->identity( next ).prev = prev;
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

  void FastIncreasingLifoBucketQueue::Update( size_t idt, ldbl cur_wgt, ldbl new_wgt ) {
    try {
      if( ( this->identity( idt ).state == BucketState::INSERTED ) || 
          ( this->identity( idt ).state == BucketState::UPDATED ) ) {
        COMMENT( "Updating element: " << idt << " from " << cur_wgt << " to " << new_wgt << ".", 3 );
        Remove( idt, cur_wgt );
      }
      else {
        COMMENT( "Inserting element: " << idt << " with weight: " << new_wgt << ".", 3 );
      }
      this->identity( idt ).state = BucketState::REMOVED;
      Insert( idt, new_wgt );
      this->identity( idt ).state = BucketState::UPDATED;
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
