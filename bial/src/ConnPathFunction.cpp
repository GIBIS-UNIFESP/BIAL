/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Sep/19
 * @brief Connected components path function to compute the initial path-values and propagated path-values in IFT
 *        algorithm. Function to label connected components in image. Result comes normaly from label map.
 */

#ifndef BIALCONNPATHFUNCTION_C
#define BIALCONNPATHFUNCTION_C

#include "ConnPathFunction.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_ConnPathFunction )
#define BIAL_EXPLICIT_ConnPathFunction
#endif
#if defined ( BIAL_EXPLICIT_ConnPathFunction ) || ( BIAL_IMPLICIT_BIN )

#include "BucketQueueElements.hpp"
#ifdef BIAL_DEBUG
#include "FileImage.hpp"
#endif
#include "Image.hpp"

/* Implementation *************************************************************************************************** */

namespace Bial {

  template< template< class D > class C, class D >
  ConnPathFunction< C, D >::ConnPathFunction( C< D > &init_value, C< int > *init_label, C< int > *init_predecessor, 
                                              bool sequential_label, const C< D > &handicap, const C< D > &component ) 
    try : PathFunction< C, D >( init_value, init_label, init_predecessor, sequential_label ), handicap( handicap ), 
            component( component ) {
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
  ConnPathFunction< C, D >::ConnPathFunction( const ConnPathFunction< C, D > &pf ) try :
    ConnPathFunction< C, D >( *( pf.value ), pf.label, pf.predecessor, true, pf.handicap, pf.component ) {
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

  template< template< class D > class C, class D > ConnPathFunction< C, D >::~ConnPathFunction( ) {
  }

  template< template< class D > class C, class D >
  ConnPathFunction< C, D > ConnPathFunction< C, D >::operator=( const ConnPathFunction< C, D > &pf ) {
    try {
      return( ConnPathFunction< C, D >( pf ) );
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
  bool ConnPathFunction< C, D >::RemoveSimple( size_t index, BucketState state ) {
    try {
      COMMENT( "Removing " << index, 4 );
      if( state == BucketState::INSERTED ) {
        this->value->operator()( index ) = 0.0;
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
  bool ConnPathFunction< C, D >::RemovePredecessor( size_t index, BucketState state ) {
    try {
      COMMENT( "Removing " << index, 4 );
      if( state == BucketState::INSERTED ) {
        this->value->operator()( index ) = 0.0;
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
  bool ConnPathFunction< C, D >::RemoveLabel( size_t index, BucketState state ) {
    try {
      COMMENT( "Removing " << index, 4 );
      if( state == BucketState::INSERTED ) {
        this->value->operator()( index ) = 0.0;
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
  bool ConnPathFunction< C, D >::RemoveComplete( size_t index, BucketState state ) {
    try {
      COMMENT( "Removing " << index, 4 );
      if( state == BucketState::INSERTED ) {
        this->value->operator()( index ) = 0.0;
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
  inline bool ConnPathFunction< C, D >::Capable( size_t index, size_t adj_index, BucketState adj_state ) {
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

  template< template< class D > class C, class D >
  bool ConnPathFunction< C, D >::PropagateDifferential( size_t index, size_t adj_index, size_t adj_pos ) {
    try {
      D src_value = this->value->operator()( adj_index );
      D prp_value = std::max( this->value->operator()( index ), handicap( adj_index ) );
      if( ( src_value > prp_value ) && ( component( index ) == component( adj_index ) ) ) {
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
  bool ConnPathFunction< C, D >::Propagate( size_t index, size_t adj_index, size_t adj_pos ) {
    try {
      D src_value = this->value->operator()( adj_index );
      D prp_value = std::max( this->value->operator()( index ), handicap( adj_index ) );
      if( ( src_value > prp_value ) && ( component( index ) == component( adj_index ) ) ) {
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
  bool ConnPathFunction< C, D >::Increasing( ) {
    return( true );
  }

#ifdef BIAL_EXPLICIT_ConnPathFunction

  template class ConnPathFunction< Image, int >;
  template class ConnPathFunction< Image, llint >;
  template class ConnPathFunction< Image, float >;
  template class ConnPathFunction< Image, double >;

  template class ConnPathFunction< Vector, int >;
  template class ConnPathFunction< Vector, llint >;
  template class ConnPathFunction< Vector, float >;
  template class ConnPathFunction< Vector, double >;

  template class ConnPathFunction< Matrix, int >;
  template class ConnPathFunction< Matrix, llint >;
  template class ConnPathFunction< Matrix, float >;
  template class ConnPathFunction< Matrix, double >;

#endif

}

#endif

#endif
