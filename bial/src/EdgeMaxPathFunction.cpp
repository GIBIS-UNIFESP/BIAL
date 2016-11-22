/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Sep/19
 * @brief Edge Max Path function to compute the initial path-values and propagated path-values in IFT algorithm.
 *        Function used by optimum path foresting clustering and classification. Result is normaly in label map.
 */

#ifndef BIALEDGEMAXPATHFUNCTION_C
#define BIALEDGEMAXPATHFUNCTION_C

#include "EdgeMaxPathFunction.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_EdgeMaxPathFunction )
#define BIAL_EXPLICIT_EdgeMaxPathFunction
#endif
#if defined ( BIAL_EXPLICIT_EdgeMaxPathFunction ) || ( BIAL_IMPLICIT_BIN )

#include "BucketQueueElements.hpp"
#ifdef BIAL_DEBUG
#include "FileImage.hpp"
#endif
#include "Image.hpp"

/* Implementation ***************************************************************************************************** */

namespace Bial {

  template< class D >
  EdgeMaxPathFunction< D >::EdgeMaxPathFunction( const Image< D > &handicap, bool force_root, D new_bucket_size ) try
    : PathFunction< Image, D >( ), handicap( handicap ), force_root( force_root ), bucket_size( new_bucket_size ) {
      DEBUG_WRITE( handicap, "handicap", 0 );
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
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Empty kernel." ) );
    throw( std::logic_error( msg ) );
  }

  template< class D >
  EdgeMaxPathFunction< D >::EdgeMaxPathFunction( const EdgeMaxPathFunction< D > &pf ) try :
    EdgeMaxPathFunction< D >( pf.handicap, pf.force_root, pf.bucket_size ) {
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
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Empty kernel." ) );
    throw( std::logic_error( msg ) );
  }

  template< class D > EdgeMaxPathFunction< D >::~EdgeMaxPathFunction( ) {
  }
  
  template< class D >
  EdgeMaxPathFunction< D > EdgeMaxPathFunction< D >::operator=( const EdgeMaxPathFunction< D > &pf ) {
    try {
      EdgeMaxPathFunction< D > new_pf( pf );
      return( new_pf );
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

  template< class D >
  void EdgeMaxPathFunction< D >::Initialize( Image< D > &init_value, Image< int > *init_label,
                                             Image< int > *init_predecessor, bool sequential_label ) {
    try {
      PathFunction< Image, D >::Initialize( init_value, init_label, init_predecessor, sequential_label );
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

  template< class D >
  bool EdgeMaxPathFunction< D >::RemoveSimple( size_t index, BucketState state ) {
    try {
      if( state == BucketState::INSERTED ) {
        COMMENT( "Root.", 3 );
        if( force_root )
          this->value->operator()( index ) = 0;
        else {
          D init_val = 0;
          size_t dimensions = handicap.Dims( );
          Vector< size_t > src_coord( handicap.Coordinates( index ) );
          Vector< size_t > max_dim( handicap.Size( ) );
          for( size_t dms = 0; dms < dimensions; ++dms ) {
            D delta = 0;
            Vector< size_t > adj_coord( src_coord );
            --adj_coord( dms );
            if( adj_coord( dms ) < max_dim( dms ) )
              delta += handicap( adj_coord );
            else
              delta += handicap( src_coord );
            adj_coord( dms ) += 2;
            if( adj_coord( dms ) < max_dim( dms ) )
              delta -= handicap( adj_coord );
            else
              delta -= handicap( src_coord );
            if( delta > 0 )
              init_val += delta;
            else
              init_val -= delta;
          }
          this->value->operator()( index ) = init_val;
        }
      }
      return( true );
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

  template< class D >
  bool EdgeMaxPathFunction< D >::RemoveLabel( size_t index, BucketState state ) {
    try {
      if( state == BucketState::INSERTED ) {
        this->label->operator()( index ) = this->next_label;
        ++( this->next_label );
      }
      return( RemoveSimple( index, state ) );
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

  template< class D >
  inline bool EdgeMaxPathFunction< D >::Capable( int index, int adj_index, BucketState adj_state ) {
    try {
      return( ( adj_state != BucketState::REMOVED ) &&
              ( this->value->operator()( index ) < this->value->operator()( adj_index ) ) );
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

  template< class D >
  bool EdgeMaxPathFunction< D >::Propagate( int index, int adj_index ) {
    try {
      D src_value = this->value->operator()( adj_index );
      D arc_weight = static_cast< D >( std::abs( static_cast< double >( handicap( adj_index ) ) - handicap( index ) ) );
      D prp_value = std::max( this->value->operator()( index ), arc_weight );
      if( src_value > prp_value ) {
        COMMENT( "Propagating: From: " << index << " to: " << adj_index << ". Old value: " << 
                 this->value->operator()( adj_index ) << ", propagated value : " << prp_value << 
                 " arc_weight: " << arc_weight << " handicap src: " << handicap( index ) << 
                 " handicap tgt: " << handicap( adj_index ) << ".", 3 );
        this->value->operator()( adj_index ) = prp_value;
        return( true );
      }
      return( false );
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

  template< class D >
  bool EdgeMaxPathFunction< D >::Increasing( ) {
    return( true );
  }

#ifdef BIAL_EXPLICIT_EdgeMaxPathFunction

  template class EdgeMaxPathFunction< int >;
  template class EdgeMaxPathFunction< llint >;
  template class EdgeMaxPathFunction< float >;
  template class EdgeMaxPathFunction< double >;

#endif

}

#endif

#endif
