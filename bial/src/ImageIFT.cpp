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
                           const Vector< bool > *seed, Image< int > *label, Image< int > *predecessor,
                           bool sequential_label, long double bucket_size, bool fifo_tie ) try :
    DegeneratedIFT< Image, D >( value, function, seed, label, predecessor, sequential_label, bucket_size, fifo_tie ),
      adjacency( adjacency ), dift_enb( false ), dift_elm( 0 ) {
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
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR(
                                                                  "Image, window end, and/or window size dimensions do not match." ) );
    throw( std::logic_error( msg ) );
  }

  template< class D >
  void ImageIFT< D >::Run( ) {
    try {
      COMMENT( "Running Image IFT. Queue: " << ( this->queue->Empty( ) ? "empty" : "not empty" ), 0 );
      size_t size = this->value.size( );
      while( ( !this->queue->Empty( ) ) && 
             ( ( !dift_enb ) || ( this->queue->State( dift_elm ) != BucketState::REMOVED ) ) ) {
        COMMENT( "Initializing removed data.", 4 );
        int index = this->queue->Remove( );
        bool capable = ( this->function->*( this->RemoveData ) )( index, this->queue->State( index ) );
        COMMENT( "Index: " << index << ", value: " << this->value[ index ] << ", is capable: " <<
                 ( capable ? "true" : "false" ), 4 );
        this->queue->Finished( index );
        if( capable ) {
          for( AdjacencyIterator adj = begin( adjacency, this->value, index ); *adj < size; ++adj ) {
            size_t adj_index = *( adj );
            if( this->function->Capable( index, adj_index, this->queue->State( adj_index ) ) ) {
              COMMENT( "Conquering: " << adj_index, 4 );
              D previous_value = this->value[ adj_index ];
              COMMENT( "previous_value: " << previous_value, 4 );
              if( this->function->Propagate( index, adj_index ) ) {
                COMMENT( "propagated!", 4 );
                this->queue->Update( adj_index, previous_value, this->value[ adj_index ] );
                COMMENT( "queue updated!", 4 );
                ( this->function->*( this->UpdateData ) )( index, adj_index );
                COMMENT( "function updated!", 4 );
              }
            }
          }
        }
      }
      COMMENT( "Reseting queue.", 0 );
      this->queue->ResetState( );
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
  void ImageIFT< D >::EnableDifferentialIFT( size_t elm ) {
    dift_enb = true;
    dift_elm = elm;
  }

  template< class D >
  void ImageIFT< D >::DisableDifferentialIFT( ) {
    dift_enb = false;
    dift_elm = 0;
  }

  // template< class D, class D2 >
  // void ImageIFT::ImageIFT( Image< D > &value, const Image< D2 > &mask, const Adjacency &adjacency, 
  //                     PathFunction< Image, D > *function, const Vector< bool > *seed, Image< int > *label,
  //                     Image< int > *predecessor, bool sequential_label, long double bucket_size, bool fifo_tie ) {
  //   if( value.Dims( ) != adjacency.Dims( ) ) {
  //     std::string msg( BIAL_ERROR( "Image and adjacency relation dimensions do not match. Image dimensions: " +
  //                                  std::to_string( value.Dims( ) ) + ", adjacency dimensions: " +
  //                                  std::to_string( adjacency.Dims( ) ) ) );
  //     throw( std::logic_error( msg ) );
  //   }
  //   if( value.Dims( ) != mask.Dims( ) ) {
  //     std::string msg( BIAL_ERROR( "Image and mask dimensions do not match." ) );
  //     throw( std::logic_error( msg ) );
  //   }
  //   try {
  //     COMMENT( "Initializing.", 1 );
  //     size_t size = value.size( );
  //     BucketQueue *queue = ImageIFT::Initialize( value, function, seed, label, predecessor, sequential_label, bucket_size,
  //                                           fifo_tie );
  //     typename PathFunction< Image, D >::RemoveFn RemoveData = function->RemoveFunction( );
  //     typename PathFunction< Image, D >::UpdateFn UpdateData = function->UpdateFunction( );
  //     COMMENT( "Running.", 1 );
  //     while( ( !queue->Empty( ) ) && ( ( !ImageIFT::dift ) || ( queue->State( ImageIFT::elm ) != BucketState::REMOVED ) ) ) {
  //       COMMENT( "Initializing removed data.", 3 );
  //       int index = queue->Remove( );
  //       if( mask[ index ] != 0 ) {
  //         bool capable = ( function->*RemoveData )( index, queue->State( index ) );
  //         queue->Finished( index );
  //         if( capable ) {
  //           for( AdjacencyIterator adj = begin( adjacency, value, index ); *adj < size; ++adj ) {
  //             size_t adj_index = *adj;
  //             if( ( mask[ adj_index ] != 0 ) &&
  //                 ( function->Capable( index, adj_index, queue->State( adj_index ) ) ) ) {
  //               D previous_value = value[ adj_index ];
  //               if( function->Propagate( index, adj_index ) ) {
  //                 queue->Update( adj_index, previous_value, value[ adj_index ] );
  //                 ( function->*UpdateData )( index, adj_index );
  //               }
  //             }
  //           }
  //         }
  //       }
  //     }
  //     if( !ImageIFT::dift ) {
  //       delete ( queue );
  //     }
  //   }
  //   catch( std::bad_alloc &e ) {
  //     std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
  //     throw( std::runtime_error( msg ) );
  //   }
  //   catch( std::runtime_error &e ) {
  //     std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
  //     throw( std::runtime_error( msg ) );
  //   }
  //   catch( const std::out_of_range &e ) {
  //     std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
  //     throw( std::out_of_range( msg ) );
  //   }
  //   catch( const std::logic_error &e ) {
  //     std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR(
  //                        "Image, window end, and/or window size dimensions do not match." ) );
  //     throw( std::logic_error( msg ) );
  //   }
  // }


#ifdef BIAL_EXPLICIT_ImageIFT

  template class ImageIFT< int >;
  template class ImageIFT< llint >;
  template class ImageIFT< float >;
  template class ImageIFT< double >;

  // template void ImageIFT::ImageIFT( Image< int > &value, const Adjacency &adjacency,
  //                              PathFunction< Image, int > *function, const Vector< bool > *seed, Image< int > *label,
  //                              Image< int > *predecessor, bool sequential_label, long double bucket_size,
  //                              bool fifo_tie );
  // template void ImageIFT::ImageIFT( Image< llint > &value, const Adjacency &adjacency,
  //                              PathFunction< Image, llint > *function, const Vector< bool > *seed, Image< int > *label,
  //                              Image< int > *predecessor, bool sequential_label, long double bucket_size,
  //                              bool fifo_tie );
  // template void ImageIFT::ImageIFT( Image< float > &value, const Adjacency &adjacency,
  //                              PathFunction< Image, float > *function, const Vector< bool > *seed, Image< int > *label,
  //                              Image< int > *predecessor, bool sequential_label, long double bucket_size,
  //                              bool fifo_tie );
  // template void ImageIFT::ImageIFT( Image< double > &value, const Adjacency &adjacency,
  //                              PathFunction< Image, double > *function, const Vector< bool > *seed, Image< int > *label,
  //                              Image< int > *predecessor, bool sequential_label, long double bucket_size,
  //                              bool fifo_tie );

  // template void ImageIFT::ImageIFT( Image< int > &value, const Image< int > &mask, const Adjacency &adjacency,
  //                              PathFunction< Image, int > *function, const Vector< bool > *seed, Image< int > *label,
  //                              Image< int > *predecessor, bool sequential_label, long double bucket_size,
  //                              bool fifo_tie );
  // template void ImageIFT::ImageIFT( Image< llint > &value, const Image< int > &mask, const Adjacency &adjacency,
  //                              PathFunction< Image, llint > *function, const Vector< bool > *seed, Image< int > *label,
  //                              Image< int > *predecessor, bool sequential_label, long double bucket_size,
  //                              bool fifo_tie );
  // template void ImageIFT::ImageIFT( Image< float > &value, const Image< int > &mask, const Adjacency &adjacency,
  //                              PathFunction< Image, float > *function, const Vector< bool > *seed, Image< int > *label,
  //                              Image< int > *predecessor, bool sequential_label, long double bucket_size,
  //                              bool fifo_tie );
  // template void ImageIFT::ImageIFT( Image< double > &value, const Image< int > &mask, const Adjacency &adjacency,
  //                              PathFunction< Image, double > *function, const Vector< bool > *seed, Image< int > *label,
  //                              Image< int > *predecessor, bool sequential_label, long double bucket_size,
  //                              bool fifo_tie );

  // template void ImageIFT::ImageIFT( Image< int > &value, const Image< llint > &mask, const Adjacency &adjacency,
  //                              PathFunction< Image, int > *function, const Vector< bool > *seed, Image< int > *label,
  //                              Image< int > *predecessor, bool sequential_label, long double bucket_size,
  //                              bool fifo_tie );
  // template void ImageIFT::ImageIFT( Image< llint > &value, const Image< llint > &mask, const Adjacency &adjacency,
  //                              PathFunction< Image, llint > *function, const Vector< bool > *seed, Image< int > *label,
  //                              Image< int > *predecessor, bool sequential_label, long double bucket_size,
  //                              bool fifo_tie );
  // template void ImageIFT::ImageIFT( Image< float > &value, const Image< llint > &mask, const Adjacency &adjacency,
  //                              PathFunction< Image, float > *function, const Vector< bool > *seed, Image< int > *label,
  //                              Image< int > *predecessor, bool sequential_label, long double bucket_size,
  //                              bool fifo_tie );
  // template void ImageIFT::ImageIFT( Image< double > &value, const Image< llint > &mask, const Adjacency &adjacency,
  //                              PathFunction< Image, double > *function, const Vector< bool > *seed, Image< int > *label,
  //                              Image< int > *predecessor, bool sequential_label, long double bucket_size,
  //                              bool fifo_tie );

  // template void ImageIFT::ImageIFT( Image< int > &value, const Image< float > &mask, const Adjacency &adjacency,
  //                              PathFunction< Image, int > *function, const Vector< bool > *seed, Image< int > *label,
  //                              Image< int > *predecessor, bool sequential_label, long double bucket_size,
  //                              bool fifo_tie );
  // template void ImageIFT::ImageIFT( Image< llint > &value, const Image< float > &mask, const Adjacency &adjacency,
  //                              PathFunction< Image, llint > *function, const Vector< bool > *seed, Image< int > *label,
  //                              Image< int > *predecessor, bool sequential_label, long double bucket_size,
  //                              bool fifo_tie );
  // template void ImageIFT::ImageIFT( Image< float > &value, const Image< float > &mask, const Adjacency &adjacency,
  //                              PathFunction< Image, float > *function, const Vector< bool > *seed, Image< int > *label,
  //                              Image< int > *predecessor, bool sequential_label, long double bucket_size,
  //                              bool fifo_tie );
  // template void ImageIFT::ImageIFT( Image< double > &value, const Image< float > &mask, const Adjacency &adjacency,
  //                              PathFunction< Image, double > *function, const Vector< bool > *seed, Image< int > *label,
  //                              Image< int > *predecessor, bool sequential_label, long double bucket_size,
  //                              bool fifo_tie );

  // template void ImageIFT::ImageIFT( Image< int > &value, const Image< double > &mask, const Adjacency &adjacency,
  //                              PathFunction< Image, int > *function, const Vector< bool > *seed, Image< int > *label,
  //                              Image< int > *predecessor, bool sequential_label, long double bucket_size,
  //                              bool fifo_tie );
  // template void ImageIFT::ImageIFT( Image< llint > &value, const Image< double > &mask, const Adjacency &adjacency,
  //                              PathFunction< Image, llint > *function, const Vector< bool > *seed, Image< int > *label,
  //                              Image< int > *predecessor, bool sequential_label, long double bucket_size,
  //                              bool fifo_tie );
  // template void ImageIFT::ImageIFT( Image< float > &value, const Image< double > &mask, const Adjacency &adjacency,
  //                              PathFunction< Image, float > *function, const Vector< bool > *seed, Image< int > *label,
  //                              Image< int > *predecessor, bool sequential_label, long double bucket_size,
  //                              bool fifo_tie );
  // template void ImageIFT::ImageIFT( Image< double > &value, const Image< double > &mask, const Adjacency &adjacency,
  //                              PathFunction< Image, double > *function, const Vector< bool > *seed, Image< int > *label,
  //                              Image< int > *predecessor, bool sequential_label, long double bucket_size,
  //                              bool fifo_tie );


#endif

}

#endif

#endif
