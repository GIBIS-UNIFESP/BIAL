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

#ifndef BIALBUCKETQUEUE_C
#define BIALBUCKETQUEUE_C

#include "BucketQueue.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_BucketQueue )
#define BIAL_EXPLICIT_BucketQueue
#endif

#if defined ( BIAL_EXPLICIT_BucketQueue ) || ( BIAL_IMPLICIT_BIN )

namespace Bial {

  BucketQueue::BucketQueue( size_t size, bool increasing_order, bool fifo_tie ) try :
    identity( Vector< IdentityNode >( size, IdentityNode( ) ) ), weight( Vector< WeightNode >( 1 ) ), elements( 0 ),
      increasing( increasing_order ), fifo( fifo_tie ) {
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

  bool BucketQueue::Empty( ) const {
    if( elements == 0 ) {
      return( true );
    }
    return( false );
  }

  size_t BucketQueue::Elements( ) const {
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
