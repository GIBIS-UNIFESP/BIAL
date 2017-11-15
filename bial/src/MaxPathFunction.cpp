/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Sep/19
 * @brief Max Path function to compute the initial path-values and propagated path-values in IFT algorithm.
 *        Functions used by Watershed transform.
 */

#ifndef BIALMAXPATHFUNCTION_C
#define BIALMAXPATHFUNCTION_C

#include "MaxPathFunction.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_MaxPathFunction )
#define BIAL_EXPLICIT_MaxPathFunction
#endif
#if defined ( BIAL_EXPLICIT_MaxPathFunction ) || ( BIAL_IMPLICIT_BIN )

#include "BucketQueueElements.hpp"
#ifdef BIAL_DEBUG
#include "FileImage.hpp"
#endif
#include "Image.hpp"

/* Implementation *************************************************************************************************** */

namespace Bial {

  template< template< class D > class C, class D >
  MaxPathFunction< C, D >::MaxPathFunction( C< D > &init_value, C< int > *init_label, C< int > *init_predecessor,
                                            bool sequential_label, const C< D > &handicap ) try :
    PathFunction< C, D >( init_value, init_label, init_predecessor, sequential_label ), handicap( handicap ) {
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

  template< template< class D > class C, class D >
  MaxPathFunction< C, D >::MaxPathFunction( const MaxPathFunction< C, D > &pf ) try :
    MaxPathFunction< C, D >( *( pf.value ), pf.label, pf.predecessor, true, pf.handicap ) {
      this->next_label = pf.next_label;
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

  template< template< class D > class C, class D > MaxPathFunction< C, D >::~MaxPathFunction( ) {
  }

  template< template< class D > class C, class D >
  MaxPathFunction< C, D > MaxPathFunction< C, D >::operator=( const MaxPathFunction< C, D > &pf ) {
    try {
      MaxPathFunction< C, D > new_pf( pf );
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

  template< template< class D > class C, class D >
  bool MaxPathFunction< C, D >::RemoveSimple( size_t index, BucketState state ) {
    try {
      if( state == BucketState::INSERTED ) {
        COMMENT( "Root.", 3 );
        this->value->operator()( index ) = handicap( index );
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

  template< template< class D > class C, class D >
  bool MaxPathFunction< C, D >::RemovePredecessor( size_t index, BucketState state ) {
    try {
      if( state == BucketState::INSERTED ) {
        COMMENT( "Root.", 3 );
        this->value->operator()( index ) = handicap( index );
        this->predecessor->operator()( index ) = -1;
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

  template< template< class D > class C, class D >
  bool MaxPathFunction< C, D >::RemoveLabel( size_t index, BucketState state ) {
    try {
      if( state == BucketState::INSERTED ) {
        this->value->operator()( index ) = handicap( index );
        this->label->operator()( index ) = this->next_label;
        ++( this->next_label );
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

  template< template< class D > class C, class D >
  bool MaxPathFunction< C, D >::RemoveComplete( size_t index, BucketState state ) {
    try {
      if( state == BucketState::INSERTED ) {
        this->value->operator()( index ) = handicap( index );
        this->label->operator()( index ) = this->next_label;
        this->predecessor->operator()( index ) = -1;
        ++( this->next_label );
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

  template< template< class D > class C, class D >
  inline bool MaxPathFunction< C, D >::Capable( size_t index, size_t adj_index, BucketState adj_state ) {
    try {
      return( adj_state != BucketState::REMOVED );
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

  template< template< class D > class C, class D >
  bool MaxPathFunction< C, D >::PropagateDifferential( size_t index, size_t adj_index, size_t adj_pos ) {
    try {
      IF_DEBUG( ( this->predecessor == nullptr ) || ( this->label == nullptr ) ) {
        std::string msg( BIAL_ERROR( "Differential propragation requires predecessor and label maps." ) );
        throw( std::runtime_error( msg ) );
      }
      COMMENT( "Propagating!", 4 );
      D src_value = this->value->operator()( adj_index );
      D arc_weight = handicap( adj_index );
      D prp_value = std::max( this->value->operator()( index ), arc_weight );
      if( ( src_value > prp_value ) ||
          ( ( this->predecessor->operator()( adj_index ) == index ) &&
            ( this->label->operator()( adj_index ) != this->label->operator()( index ) ) ) ) {
        COMMENT( "propagated!", 4 );
        this->value->operator()( adj_index ) = prp_value;
        this->predecessor->operator()( adj_index ) = index;
        this->label->operator()( adj_index ) = this->label->operator()( index );
        return( true );
      }
      COMMENT( "Not propagated!", 4 );
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

  template< template< class D > class C, class D >
  bool MaxPathFunction< C, D >::Propagate( size_t index, size_t adj_index, size_t adj_pos ) {
    try {
      COMMENT( "Propagating!", 4 );
      D src_value = this->value->operator()( adj_index );
      D arc_weight = handicap( adj_index );
      D prp_value = std::max( this->value->operator()( index ), arc_weight );
      if( ( ( this->predecessor != nullptr ) && ( this->predecessor->operator()( adj_index ) == index ) ) 
          || ( src_value > prp_value ) ) {
        COMMENT( "propagated!", 4 );
        this->value->operator()( adj_index ) = prp_value;
        ( this->*this->UpdateData )( index, adj_index );
        return( true );
      }
      COMMENT( "Not propagated!", 4 );
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

  template< template< class D > class C, class D >
  bool MaxPathFunction< C, D >::Increasing( ) {
    return( true );
  }

  template< template< class D > class C, class D >
  D MaxPathFunction< C, D >::BestValue( size_t index ) {
    return( handicap[ index ] );
  }

#ifdef BIAL_EXPLICIT_MaxPathFunction

  template class MaxPathFunction< Image, int >;
  template class MaxPathFunction< Image, llint >;
  template class MaxPathFunction< Image, float >;
  template class MaxPathFunction< Image, double >;

  template class MaxPathFunction< Vector, llint >;
  template class MaxPathFunction< Vector, int >;
  template class MaxPathFunction< Vector, float >;
  template class MaxPathFunction< Vector, double >;

  template class MaxPathFunction< Matrix, int >;
  template class MaxPathFunction< Matrix, llint >;
  template class MaxPathFunction< Matrix, float >;
  template class MaxPathFunction< Matrix, double >;

#endif

}

#endif

#endif
