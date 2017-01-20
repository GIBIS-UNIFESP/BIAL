/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Sep/19
 * @brief Diff path function to compute the initial path-values and propagated path-values in IFT algorithm.
 *        Used by live-wire.
 */

#ifndef BIALDIFFPATHFUNCTION_C
#define BIALDIFFPATHFUNCTION_C

#include "DiffPathFunction.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_DiffPathFunction )
#define BIAL_EXPLICIT_DiffPathFunction
#endif
#if defined ( BIAL_EXPLICIT_DiffPathFunction ) || ( BIAL_IMPLICIT_BIN )

#include "BucketQueueElements.hpp"
#ifdef BIAL_DEBUG
#include "FileImage.hpp"
#endif
#include "Image.hpp"

/* Implementation ***************************************************************************************************** */

namespace Bial {

  template< template< class D > class C, class D >
  DiffPathFunction< C, D >::DiffPathFunction( C< D > &init_value, C< int > *init_label, C< int > *init_predecessor,
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
  DiffPathFunction< C, D >::DiffPathFunction( const DiffPathFunction< C, D > &pf ) try :
    DiffPathFunction< C, D >( *( pf.value ), pf.label, pf.predecessor, true, handicap ) {
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

  template< template< class D > class C, class D > DiffPathFunction< C, D >::~DiffPathFunction( ) {
  }

  template< template< class D > class C, class D >
  DiffPathFunction< C, D > DiffPathFunction< C, D >::operator=( const DiffPathFunction< C, D > &pf ) {
    try {
      return( DiffPathFunction< C, D >( pf ) );
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
  bool DiffPathFunction< C, D >::RemoveSimple( size_t index, BucketState state ) {
    try {
      COMMENT( "Removing element.", 3 );
      if( state == BucketState::INSERTED ) {
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
  bool DiffPathFunction< C, D >::RemovePredecessor( size_t index, BucketState state ) {
    try {
      COMMENT( "Removing element.", 3 );
      if( state == BucketState::INSERTED ) {
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
  bool DiffPathFunction< C, D >::RemoveLabel( size_t index, BucketState state ) {
    try {
      COMMENT( "Removing element.", 3 );
      if( state == BucketState::INSERTED ) {
        this->value->operator()( index ) = handicap( index );
        COMMENT( "Next label: " << this->next_label << ".", 3 );
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
  bool DiffPathFunction< C, D >::RemoveComplete( size_t index, BucketState state ) {
    try {
      COMMENT( "Removing element.", 3 );
      if( state == BucketState::INSERTED ) {
        this->value->operator()( index ) = handicap( index );
        COMMENT( "Next label: " << this->next_label << ".", 3 );
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
  inline bool DiffPathFunction< C, D >::Capable( size_t index, size_t adj_index, BucketState adj_state ) {
    try {
      return( ( adj_state != BucketState::REMOVED ) &&
              ( this->value->operator()( index ) > this->value->operator()( adj_index ) ) );
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
  bool DiffPathFunction< C, D >::PropagateDifferential( size_t index, size_t adj_index ) {
    try {
      D src_value = this->value->operator()( adj_index );
      D prp_value = this->value->operator()( index ) - handicap( adj_index );
      COMMENT( "Trying to propagate from " << index << " to " << adj_index << ".", 4 );
      COMMENT( "orig value " << src_value << ", tgt value: " << prp_value << ".", 4 );
      if( src_value < prp_value ) {
        COMMENT( "Conquested.", 4 );
        this->value->operator()( adj_index ) = prp_value;
        ( this->*this->UpdateData )( index, adj_index );
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

  template< template< class D > class C, class D >
  bool DiffPathFunction< C, D >::Propagate( size_t index, size_t adj_index ) {
    try {
      D src_value = this->value->operator()( adj_index );
      D prp_value = this->value->operator()( index ) - handicap( adj_index );
      COMMENT( "Trying to propagate from " << index << " to " << adj_index << ".", 4 );
      COMMENT( "orig value " << src_value << ", tgt value: " << prp_value << ".", 4 );
      if( src_value < prp_value ) {
        COMMENT( "Conquested.", 4 );
        this->value->operator()( adj_index ) = prp_value;
        ( this->*this->UpdateData )( index, adj_index );
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

  template< template< class D > class C, class D >
  bool DiffPathFunction< C, D >::Increasing( ) {
    return( false );
  }

#ifdef BIAL_EXPLICIT_DiffPathFunction

  template class DiffPathFunction< Image, int >;
  template class DiffPathFunction< Image, llint >;
  template class DiffPathFunction< Image, float >;
  template class DiffPathFunction< Image, double >;

  template class DiffPathFunction< Vector, int >;
  template class DiffPathFunction< Vector, llint >;
  template class DiffPathFunction< Vector, float >;
  template class DiffPathFunction< Vector, double >;

  template class DiffPathFunction< Matrix, int >;
  template class DiffPathFunction< Matrix, llint >;
  template class DiffPathFunction< Matrix, float >;
  template class DiffPathFunction< Matrix, double >;

#endif

}

#endif

#endif
