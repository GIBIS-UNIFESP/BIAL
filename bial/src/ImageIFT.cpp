/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Sep/08
 * @brief IFT algorithm running on Images.
 */

#ifndef BIALIMAGEIFT_C
#define BIALIMAGEIFT_C

#include "ImageIFT.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_ImageIFT )
#define BIAL_EXPLICIT_ImageIFT
#endif

#if defined ( BIAL_EXPLICIT_ImageIFT ) || ( BIAL_IMPLICIT_BIN )

#include "AdjacencyIterator.hpp"
#include "BucketQueue.hpp"

namespace Bial {

  template< class D >
  ImageIFT< D >::ImageIFT( Image< D > &value, const Adjacency &adjacency, PathFunction< Image, D > *function,
                           BucketQueue *queue ) try :
    DegeneratedIFT< Image, D >( value, function, queue ), adjacency( adjacency ), stop_elm( value.size( ) ) {
      if( value.Dims( ) != adjacency.Dims( ) ) {
        std::string msg( BIAL_ERROR( "Image and adjacency relation dimensions do not match. Image dimensions: " +
                                     std::to_string( value.Dims( ) ) + ", adjacency dimensions: " +
                                     std::to_string( adjacency.Dims( ) ) ) );
        throw( std::logic_error( msg ) );
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
    std::string msg( e.what( ) + std::string( "\n" ) + 
                     BIAL_ERROR( "Image, window end, and/or window size dimensions do not match." ) );
    throw( std::logic_error( msg ) );
  }

  template< class D >
  void ImageIFT< D >::Run( ) {
    try {
      COMMENT( "Running Image IFT. Queue: " << ( this->queue->Empty( ) ? "empty" : "not empty" ), 0 );
      AdjacencyIterator adj_itr( this->value, adjacency );
      size_t adj_size = adjacency.size( );
      size_t adj_index;
      size_t index = std::numeric_limits< size_t >::max( );
      while( ( !this->queue->Empty( ) ) && ( index != stop_elm ) ) {
        COMMENT( "Initializing removed data.", 4 );
        index = this->queue->Remove( );
        bool capable = ( this->function->*( this->function->RemoveData ) )( index, this->queue->State( index ) );
        COMMENT( "Index: " << index << ", value: " << this->value[ index ] << ", is capable: " <<
                 ( capable ? "true" : "false" ), 4 );
        this->queue->Finished( index );
        if( capable ) {
          for( size_t adj = 0; adj < adj_size; ++adj ) {
            if( ( adj_itr.AdjIdx( index, adj, adj_index ) ) &&
                ( this->queue->State( adj_index ) != BucketState::REMOVED ) &&
                ( this->function->Capable( index, adj_index, this->queue->State( adj_index ) ) ) ) {
              COMMENT( "Conquering: " << adj_index, 4 );
              D previous_value = this->value[ adj_index ];
              COMMENT( "previous_value: " << previous_value, 4 );
              if( ( this->function->*( this->function->PropagateData ) )( index, adj_index, adj ) )
                this->queue->Update( adj_index, previous_value, this->value[ adj_index ] );
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

  template< class D >
  void ImageIFT< D >::EnableStopElement( size_t elm ) {
    stop_elm = elm;
  }

  template< class D >
  void ImageIFT< D >::DisableStopElement( ) {
    stop_elm = this->value.size( );
  }

#ifdef BIAL_EXPLICIT_ImageIFT

  template class ImageIFT< int >;
  template class ImageIFT< llint >;
  template class ImageIFT< float >;
  template class ImageIFT< double >;

#endif

}

#endif

#endif
