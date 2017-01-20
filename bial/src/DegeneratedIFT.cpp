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
  DegeneratedIFT( C< D > &value, PathFunction< C, D > *function, BucketQueue *queue ) try :
    queue( queue ), value( value ), function( function ) {
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
  }

  template< template< class D > class C, class D >
  void DegeneratedIFT< C, D >::Run( ) {
    try {
      COMMENT( "Running.", 1 );
      while( !queue->Empty( ) ) {
        size_t index = queue->Remove( );
        COMMENT( "Initializing removed data.", 3 );
        bool capable = ( function->*function->RemoveData )( index, queue->State( index ) );
        queue->Finished( index );
        if( capable ) {
          for( size_t adj_index = 0; adj_index < value.size( ); ++adj_index ) {
            if( function->Capable( index, adj_index, queue->State( adj_index ) ) ) {
              D previous_value = value( adj_index );
              if( ( this->function->*( this->function->PropagateData ) )( index, adj_index ) )
                queue->Update( adj_index, previous_value, value( adj_index ) );
            }
          }
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

  template< template< class D > class C, class D >
  void DegeneratedIFT< C, D >::InsertSeeds( const Vector< bool > &seed ) {
    try {
      COMMENT( "Inserting seeds.", 1 );
      for( size_t it = 0; it < value.size( ); ++it ) {
        if( seed[ it ] )
          queue->Insert( it, value( it ) );
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

#endif

}

#endif

#endif
