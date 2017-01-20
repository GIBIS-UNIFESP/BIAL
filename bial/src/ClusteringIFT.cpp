/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Sep/08
 * @brief IFT algorithm for clustering purposes.
 */

#ifndef BIALCLUSTERINGIFT_C
#define BIALCLUSTERINGIFT_C

#include "ClusteringIFT.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_ClusteringIFT )
#define BIAL_EXPLICIT_ClusteringIFT
#endif

#if defined ( BIAL_EXPLICIT_ClusteringIFT ) || ( BIAL_IMPLICIT_BIN )

#include "BucketQueue.hpp"
#include "PathFunction.hpp"

namespace Bial {

  template< template< class D > class C, class D >
  ClusteringIFT< C, D >::ClusteringIFT( C< D > &value, PathFunction< C, D > *function,
                                        const Matrix< size_t > &uniform_adjacency, const size_t uniform_adjs,
                                        const Vector< Vector< size_t > > &variable_adjacency, BucketQueue *queue ) try :
    DegeneratedIFT< C, D >( value, function, queue ), uniform_adjacency( &uniform_adjacency ), 
      uniform_adjs( uniform_adjs ), variable_adjacency( variable_adjacency ), variable_adjs( nullptr ) {
      if( uniform_adjacency.size( ) == 0 ) {
        std::string msg( BIAL_ERROR( "Must have at last one adjacent in uniform adjacency." ) );
        throw( std::logic_error( msg ) );
      }
      if( uniform_adjacency.size( 0 ) != value.size( ) ) {
        std::string msg( BIAL_ERROR(
                                    "Value and uniform adjacency relation dimensions do not match. Uniform adjacency size: " +
                                    std::to_string( uniform_adjacency.size( 0 ) ) + ", value size: " +
                                    std::to_string( value.size( ) ) ) );
        throw( std::logic_error( msg ) );
      }
      if( variable_adjacency.size( ) != value.size( ) ) {
        std::string msg( BIAL_ERROR( "Value and variable adjacency relation dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      if( uniform_adjacency.size( 0 ) < uniform_adjs ) {
        std::string msg( BIAL_ERROR( "uniform_adjs must be lower than uniform_adjacency size." ) );
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

  template< template< class D > class C, class D >
  ClusteringIFT< C, D >::ClusteringIFT( C< D > &value, PathFunction< C, D > *function,
                                        const Vector< Vector< size_t > > &variable_adjacency, 
                                        const Vector< size_t > &variable_adjs, BucketQueue *queue ) try :
    DegeneratedIFT< C, D >( value, function, queue ), uniform_adjacency( nullptr ), uniform_adjs( 0 ), 
      variable_adjacency( variable_adjacency ), variable_adjs( &variable_adjs ) {
      if( variable_adjacency.size( ) != value.size( ) ) {
        std::string msg( BIAL_ERROR( "Value and variable adjacency relation dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Initializing.", 1 );
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
  void ClusteringIFT< C, D >::Run( ) {
    try {
      if( uniform_adjacency != nullptr ) { 
        COMMENT( "Running with uniform adjacency.", 1 );
        while( !this->queue->Empty( ) ) {
          size_t index = this->queue->Remove( );
          COMMENT( "Initializing removed data " << index, 4 );
          bool capable = ( this->function->*( this->function->RemoveData ) )( index, this->queue->State( index ) );
          this->queue->Finished( index );
          if( capable ) {
            COMMENT( "Propagating to fixed size nighborhood.", 4 );
            for( size_t fxd = 0; fxd < uniform_adjs; ++fxd ) {
              size_t adj_index = uniform_adjacency->operator( )( index, fxd );
              if( this->function->Capable( index, adj_index, this->queue->State( adj_index ) ) ) {
                COMMENT( "Capable from " << index << " to " << adj_index, 4 );
                D previous_value = this->value( adj_index );
                if( ( this->function->*( this->function->PropagateData ) )( index, adj_index ) ) {
                  COMMENT( "Propagation from " << index << " to " << adj_index, 4 );
                  this->queue->Update( adj_index, previous_value, this->value( adj_index ) );
                }
              }
            }
            COMMENT( "Propagating to variable size nighborhood.", 4 );
            size_t adjs = variable_adjacency( index ).size( );
            for( size_t var = 0; var < adjs; ++var ) {
              size_t adj_index = variable_adjacency( index )( var );
              if( this->function->Capable( index, adj_index, this->queue->State( adj_index ) ) ) {
                COMMENT( "Capable from " << index << " to " << adj_index, 4 );
                D previous_value = this->value( adj_index );
                if( ( this->function->*( this->function->PropagateData ) )( index, adj_index ) ) {
                  COMMENT( "Propagation from " << index << " to " << adj_index, 4 );
                  this->queue->Update( adj_index, previous_value, this->value( adj_index ) );
                }
              }
            }
          }
        }
      }
      else {
        COMMENT( "Running with variable adjacency.", 1 );
        while( !this->queue->Empty( ) ) {
          size_t index = this->queue->Remove( );
          COMMENT( "Initializing removed data " << index, 4 );
          bool capable = ( this->function->*( this->function->RemoveData ) )( index, this->queue->State( index ) );
          this->queue->Finished( index );
          if( capable ) {
            COMMENT( "Propagating from " << index << " to variable nighborhood of size: " << 
                     variable_adjs->operator( )( index ) << ".", 4 );
            for( size_t var = 0; var < variable_adjs->operator( )( index ); ++var ) {
              size_t adj_index = variable_adjacency( index ) ( var );
              COMMENT( "Propagating to neighbor " << var << ": " << adj_index << ".", 4 );
              if( this->function->Capable( index, adj_index, this->queue->State( adj_index ) ) ) {
                COMMENT( "Capable from " << index << " to " << adj_index, 4 );
                D previous_value = this->value( adj_index );
                if( ( this->function->*( this->function->PropagateData ) )( index, adj_index ) ) {
                  COMMENT( "Propagation from " << index << " to " << adj_index, 4 );
                  this->queue->Update( adj_index, previous_value, this->value( adj_index ) );
                }
              }
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
      std::string msg( e.what( ) + std::string( "\n" ) +
                       BIAL_ERROR( "Image, window end, and/or window size dimensions do not match." ) );
      throw( std::logic_error( msg ) );
    }
  }
  
#ifdef BIAL_EXPLICIT_ClusteringIFT

  template class ClusteringIFT< Vector, int >;
  template class ClusteringIFT< Vector, llint >;
  template class ClusteringIFT< Vector, float >;
  template class ClusteringIFT< Vector, double >;

  template class ClusteringIFT< Image, int >;
  template class ClusteringIFT< Image, llint >;
  template class ClusteringIFT< Image, float >;
  template class ClusteringIFT< Image, double >;

#endif

}

#endif

#endif
