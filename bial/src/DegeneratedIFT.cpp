/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Sep/08
 * @brief Degenerated IFT algorithm. Used for segmentation by MST method.
 */

#ifndef BIALDEGENERATEDIFT_C
#define BIALDEGENERATEDIFT_C

#include "DegeneratedIFT.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_DegeneratedIFT )
#define BIAL_EXPLICIT_DegeneratedIFT
#endif

#if defined ( BIAL_EXPLICIT_DegeneratedIFT ) || ( BIAL_IMPLICIT_BIN )

#include "FastBucketQueue.hpp"
#include "FastDecreasingFifoBucketQueue.hpp"
#include "FastDecreasingLifoBucketQueue.hpp"
#include "FastIncreasingFifoBucketQueue.hpp"
#include "FastIncreasingLifoBucketQueue.hpp"
#include "GrowingBucketQueue.hpp"

namespace Bial {

  template< template< class D > class C, class D >
  DegeneratedIFT< C, D >::
  DegeneratedIFT( C< D > &value, PathFunction< C, D > *function, const Vector< bool > *seed, C< int > *label, 
                  C< int > *predecessor, bool sequential_label, ldbl bucket_size, bool fifo_tie ) try :
    value( value ), function( function ), label( label ), predecessor( predecessor ) {
      COMMENT( "Initializing.", 1 );
      Initialize( seed, sequential_label, bucket_size, fifo_tie );
      RemoveData = function->RemoveFunction( );
      UpdateData = function->UpdateFunction( );
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
    std::string msg( e.what( ) + std::string( "\n" ) + 
                     BIAL_ERROR( "Image, window end, and/or window size dimensions do not match." ) );
    throw( std::logic_error( msg ) );
  }

  template< template< class D > class C, class D >
  DegeneratedIFT< C, D >::
  DegeneratedIFT( C< D > &value, PathFunction< C, D > *function, D minimum_value, size_t value_range, 
                    const Vector< bool > *seed, C< int > *label, C< int > *predecessor, bool sequential_label,
                    bool fifo_tie ) try :
    value( value ), function( function ), label( label ), predecessor( predecessor ) {
      COMMENT( "Initializing.", 1 );
      Initialize( seed, sequential_label, minimum_value, value_range, fifo_tie );
      RemoveData = function->RemoveFunction( );
      UpdateData = function->UpdateFunction( );
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
    std::string msg( e.what( ) + std::string( "\n" ) + 
                     BIAL_ERROR( "Image, window end, and/or window size dimensions do not match." ) );
    throw( std::logic_error( msg ) );
  }
  
  template< template< class D > class C, class D >
  DegeneratedIFT< C, D >::~DegeneratedIFT( ) {
    delete ( queue );
  }

  template< template< class D > class C, class D >
  void DegeneratedIFT< C, D >::Run( ) {
    try {
      COMMENT( "Running.", 1 );
      while( !queue->Empty( ) ) {
        int index = queue->Remove( );
        COMMENT( "Initializing removed data.", 3 );
        bool capable = ( function->*RemoveData )( index, queue->State( index ) );
        queue->Finished( index );
        if( capable ) {
          for( size_t adj_index = 0; adj_index < value.size( ); ++adj_index ) {
            if( function->Capable( index, adj_index, queue->State( adj_index ) ) ) {
              D previous_value = value( adj_index );
              if( function->Propagate( index, adj_index ) ) {
                queue->Update( adj_index, previous_value, value( adj_index ) );
                ( function->*UpdateData )( index, adj_index );
              }
            }
          }
        }
      }
      queue->ResetState( );
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
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR(
                         "Image, window end, and/or window size dimensions do not match." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< template< class D > class C, class D >
  void DegeneratedIFT< C, D >::Initialize( const Vector< bool > *seed, bool sequential_label, ldbl bucket_size, 
                                           bool fifo_tie ) {
    try {
      queue = new GrowingBucketQueue( value.size( ), bucket_size, function->Increasing( ), fifo_tie );
      COMMENT( "Initializing data in path function.", 1 );
      function->Initialize( value, label, predecessor, sequential_label );
      COMMENT( "Initializing the maps.", 1 );
      if( predecessor != nullptr ) {
        for( size_t it = 0; it < predecessor->size( ); ++it ) {
          predecessor->operator()( it ) = -1;
        }
      }
      if( seed != nullptr ) {
        COMMENT( "Initializing data with seeds.", 1 );
        InsertSeeds( *seed );
      }
      else {
        COMMENT( "Initializing data without seeds.", 1 );
        for( size_t it = 0; it < value.size( ); ++it ) {
          queue->Insert( it, value( it ) );
        }
      }
      COMMENT( "value: " << value, 5 );
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
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR(
                         "Image, window end, and/or window size dimensions do not match." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< template< class D > class C, class D >
  void DegeneratedIFT< C, D >::Initialize( const Vector< bool > *seed, bool sequential_label, ldbl minimum_value, 
                                           size_t value_range, bool fifo_tie ) {
    try {
      //queue = new FastBucketQueue( value.size( ), minimum_value, value_range, function->Increasing( ), fifo_tie );
      if( fifo_tie ) {
        if( function->Increasing( ) ) 
          queue = new FastIncreasingFifoBucketQueue( value.size( ), minimum_value, value_range );
        else
          queue = new FastDecreasingFifoBucketQueue( value.size( ), minimum_value, value_range );
      }
      else {
        if( function->Increasing( ) ) 
          queue = new FastIncreasingLifoBucketQueue( value.size( ), minimum_value, value_range );
        else
          queue = new FastDecreasingLifoBucketQueue( value.size( ), minimum_value, value_range );
      }
      COMMENT( "Initializing data in path function.", 1 );
      function->Initialize( value, label, predecessor, sequential_label );
      COMMENT( "Initializing the maps.", 1 );
      if( predecessor != nullptr ) {
        for( size_t it = 0; it < predecessor->size( ); ++it ) {
          predecessor->operator()( it ) = -1;
        }
      }
      if( seed != nullptr ) {
        COMMENT( "Initializing data with seeds.", 1 );
        InsertSeeds( *seed );
      }
      else {
        COMMENT( "Initializing data without seeds.", 1 );
        for( size_t it = 0; it < value.size( ); ++it ) {
          queue->Insert( it, value( it ) );
        }
      }
      COMMENT( "value: " << value, 5 );
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
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR(
                                                                    "Image, window end, and/or window size dimensions do not match." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< template< class D > class C, class D >
  void DegeneratedIFT< C, D >::InsertSeeds( const Vector< bool > &seed ) {
    try {
      COMMENT( "Inserting seeds.", 1 );
      for( size_t it = 0; it < value.size( ); ++it ) {
        if( seed[ it ] )
          queue->Insert( it, value( it ) );
      }
      if( predecessor != nullptr ) {
        COMMENT( "Setting seed predecessor", 1 );
        for( size_t it = 0; it < predecessor->size( ); ++it ) {
          if( seed[ it ] )
            predecessor->operator()( it ) = -1;
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
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR(
                         "Image, window end, and/or window size dimensions do not match." ) );
      throw( std::logic_error( msg ) );
    }
  }

#ifdef BIAL_EXPLICIT_DegeneratedIFT

  template class DegeneratedIFT< Vector, int >;
  template class DegeneratedIFT< Vector, llint >;
  template class DegeneratedIFT< Vector, float >;
  template class DegeneratedIFT< Vector, double >;

  template class DegeneratedIFT< Matrix, int >;
  template class DegeneratedIFT< Matrix, llint >;
  template class DegeneratedIFT< Matrix, float >;
  template class DegeneratedIFT< Matrix, double >;

  template class DegeneratedIFT< Image, int >;
  template class DegeneratedIFT< Image, llint >;
  template class DegeneratedIFT< Image, float >;
  template class DegeneratedIFT< Image, double >;

  // template void DegeneratedIFT::DegeneratedIFT( Vector< int > &value, PathFunction< Vector, int > *function,
  //                                               const Vector< bool > *seed, Vector< int > *label, 
  //                                               Vector< int > *predecessor, bool sequential_label, 
  //                                               long double bucket_size, bool fifo_tie );
  // template BucketQueue* DegeneratedIFT::Initialize( Vector< int > &value, PathFunction< Vector, int > *function,
  //                                                   const Vector< bool > *seed, Vector< int > *label, 
  //                                                   Vector< int > *predecessor, bool sequential_label, 
  //                                                   long double bucket_size, bool fifo_tie );
  // template void DegeneratedIFT::DegeneratedIFT( Vector< llint > &value, PathFunction< Vector, llint > *function,
  //                                               const Vector< bool > *seed, Vector< int > *label, 
  //                                               Vector< int > *predecessor, bool sequential_label, 
  //                                               long double bucket_size, bool fifo_tie );
  // template BucketQueue* DegeneratedIFT::Initialize( Vector< llint > &value, PathFunction< Vector, llint > *function,
  //                                                   const Vector< bool > *seed, Vector< int > *label, 
  //                                                   Vector< int > *predecessor, bool sequential_label, 
  //                                                   long double bucket_size, bool fifo_tie );
  // template void DegeneratedIFT::DegeneratedIFT( Vector< float > &value, PathFunction< Vector, float > *function,
  //                                               const Vector< bool > *seed, Vector< int > *label, 
  //                                               Vector< int > *predecessor, bool sequential_label, 
  //                                               long double bucket_size, bool fifo_tie );
  // template BucketQueue* DegeneratedIFT::Initialize( Vector< float > &value, PathFunction< Vector, float > *function,
  //                                                   const Vector< bool > *seed, Vector< int > *label, 
  //                                                   Vector< int > *predecessor, bool sequential_label, 
  //                                                   long double bucket_size, bool fifo_tie );
  // template void DegeneratedIFT::DegeneratedIFT( Vector< double > &value, PathFunction< Vector, double > *function,
  //                                               const Vector< bool > *seed, Vector< int > *label, 
  //                                               Vector< int > *predecessor, bool sequential_label, 
  //                                               long double bucket_size, bool fifo_tie );
  // template BucketQueue* DegeneratedIFT::Initialize( Vector< double > &value, PathFunction< Vector, double > *function,
  //                                                   const Vector< bool > *seed, Vector< int > *label, 
  //                                                   Vector< int > *predecessor, bool sequential_label, 
  //                                                   long double bucket_size, bool fifo_tie );

  // template void DegeneratedIFT::DegeneratedIFT( Matrix< int > &value_image, PathFunction< Matrix, int > *function,
  //                                               const Vector< bool > *seed, Matrix< int > *label, 
  //                                               Matrix< int > *predecessor, bool sequential_label, 
  //                                               long double bucket_size, bool fifo_tie );
  // template BucketQueue* DegeneratedIFT::Initialize( Matrix< int > &value, PathFunction< Matrix, int > *function,
  //                                                   const Vector< bool > *seed, Matrix< int > *label, 
  //                                                   Matrix< int > *predecessor, bool sequential_label, 
  //                                                   long double bucket_size, bool fifo_tie );
  // template void DegeneratedIFT::DegeneratedIFT( Matrix< llint > &value_image, PathFunction< Matrix, llint > *function,
  //                                               const Vector< bool > *seed, Matrix< int > *label, 
  //                                               Matrix< int > *predecessor, bool sequential_label, 
  //                                               long double bucket_size, bool fifo_tie );
  // template BucketQueue* DegeneratedIFT::Initialize( Matrix< llint > &value, PathFunction< Matrix, llint > *function,
  //                                                   const Vector< bool > *seed, Matrix< int > *label, 
  //                                                   Matrix< int > *predecessor, bool sequential_label, 
  //                                                   long double bucket_size, bool fifo_tie );
  // template void DegeneratedIFT::DegeneratedIFT( Matrix< float > &value_image, PathFunction< Matrix, float > *function,
  //                                               const Vector< bool > *seed, Matrix< int > *label, 
  //                                               Matrix< int > *predecessor, bool sequential_label, 
  //                                               long double bucket_size, bool fifo_tie );
  // template BucketQueue* DegeneratedIFT::Initialize( Matrix< float > &value, PathFunction< Matrix, float > *function,
  //                                                   const Vector< bool > *seed, Matrix< int > *label, 
  //                                                   Matrix< int > *predecessor, bool sequential_label, 
  //                                                   long double bucket_size, bool fifo_tie );
  // template void DegeneratedIFT::DegeneratedIFT( Matrix< double > &value_image, PathFunction< Matrix, double > *function,
  //                                               const Vector< bool > *seed, Matrix< int > *label, 
  //                                               Matrix< int > *predecessor, bool sequential_label, 
  //                                               long double bucket_size, bool fifo_tie );
  // template BucketQueue* DegeneratedIFT::Initialize( Matrix< double > &value, PathFunction< Matrix, double > *function,
  //                                                   const Vector< bool > *seed, Matrix< int > *label, 
  //                                                   Matrix< int > *predecessor, bool sequential_label, 
  //                                                   long double bucket_size, bool fifo_tie );

  // template void DegeneratedIFT::DegeneratedIFT( Image< int > &value, PathFunction< Image, int > *function,
  //                                               const Vector< bool > *seed, Image< int > *label, 
  //                                               Image< int > *predecessor, bool sequential_label, 
  //                                               long double bucket_size, bool fifo_tie );
  // template BucketQueue* DegeneratedIFT::Initialize( Image< int > &value, PathFunction< Image, int > *function,
  //                                                   const Vector< bool > *seed, Image< int > *label, 
  //                                                   Image< int > *predecessor, bool sequential_label,
  //                                                   long double bucket_size, bool fifo_tie );
  // template void DegeneratedIFT::DegeneratedIFT( Image< llint > &value, PathFunction< Image, llint > *function,
  //                                               const Vector< bool > *seed, Image< int > *label,
  //                                               Image< int > *predecessor, bool sequential_label,
  //                                               long double bucket_size, bool fifo_tie );
  // template BucketQueue* DegeneratedIFT::Initialize( Image< llint > &value, PathFunction< Image, llint > *function,
  //                                                   const Vector< bool > *seed, Image< int > *label,
  //                                                   Image< int > *predecessor, bool sequential_label,
  //                                                   long double bucket_size, bool fifo_tie );
  // template void DegeneratedIFT::DegeneratedIFT( Image< float > &value, PathFunction< Image, float > *function,
  //                                               const Vector< bool > *seed, Image< int > *label,
  //                                               Image< int > *predecessor, bool sequential_label, 
  //                                               long double bucket_size, bool fifo_tie );
  // template BucketQueue* DegeneratedIFT::Initialize( Image< float > &value, PathFunction< Image, float > *function,
  //                                                   const Vector< bool > *seed, Image< int > *label, 
  //                                                   Image< int > *predecessor, bool sequential_label, 
  //                                                   long double bucket_size, bool fifo_tie );
  // template void DegeneratedIFT::DegeneratedIFT( Image< double > &value, PathFunction< Image, double > *function,
  //                                               const Vector< bool > *seed, Image< int > *label, 
  //                                               Image< int > *predecessor, bool sequential_label, 
  //                                               long double bucket_size, bool fifo_tie );
  // template BucketQueue* DegeneratedIFT::Initialize( Image< double > &value, PathFunction< Image, double > *function,
  //                                                   const Vector< bool > *seed, Image< int > *label, 
  //                                                   Image< int > *predecessor, bool sequential_label, 
  //                                                   long double bucket_size, bool fifo_tie );

#endif

}

#endif

#endif
