/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Sep/19
 * @brief Path function to compute the initial path-values and propagated path-values in IFT algorithm.
 */

#ifndef BIALPATHFUNCTION_C
#define BIALPATHFUNCTION_C

#include "PathFunction.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_PathFunction )
#define BIAL_EXPLICIT_PathFunction
#endif
#if defined ( BIAL_EXPLICIT_PathFunction ) || ( BIAL_IMPLICIT_BIN )

#include "BucketQueueElements.hpp"
#ifdef BIAL_DEBUG
#include "FileImage.hpp"
#endif
#include "Image.hpp"

/* Implementation ***************************************************************************************************** */

namespace Bial {

  template< template< class D > class C, class D > 
  PathFunction< C, D >::PathFunction( C< D > &init_value, C< int > *init_label, C< int > *init_predecessor, 
                                      bool sequential_label ) try :
    value( &init_value ), label( init_label ), predecessor( init_predecessor ), next_label( -1 ) {
      if( ( ( init_label != nullptr ) && ( init_value.size( ) != init_label->size( ) ) ) ||
          ( ( init_predecessor != nullptr ) && ( init_value.size( ) != init_predecessor->size( ) ) ) ) {
        std::string msg( BIAL_ERROR( "Image dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      if( sequential_label ) {
        COMMENT( "Sequential label.", 0 );
        next_label = 0;
      }
      RemoveData = RemoveFunction( );
      PropagateData = PropagateFunction( );
      UpdateData = UpdateFunction( );
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
  PathFunction< C, D >::PathFunction( const PathFunction< C, D > &pf ) try :
    PathFunction< C, D >( *( pf.value ), pf.label, pf.predecessor, pf.next_label ) {
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
  D PathFunction< C, D >::BestValue( size_t index ) {
    std::cout << "Bestvalue pathfunction." << std::endl;
    return( ( *value )[ index ] );
  }

  template< template< class D > class C, class D >
  typename PathFunction< C, D >::RemoveFn PathFunction< C, D >::RemoveFunction( ) {
    try {
      if( next_label == -1 ) {
        if( predecessor == nullptr )
          return( &PathFunction< C, D >::RemoveSimple );
        else
          return( &PathFunction< C, D >::RemovePredecessor );
      }
      else {
        if( predecessor == nullptr )
          return( &PathFunction< C, D >::RemoveLabel );
        else
          return( &PathFunction< C, D >::RemoveComplete );
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
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< template< class D > class C, class D >
  typename PathFunction< C, D >::PropagateFn PathFunction< C, D >::PropagateFunction( ) {
    try {
      return( &PathFunction< C, D >::Propagate );
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
  typename PathFunction< C, D >::UpdateFn PathFunction< C, D >::UpdateFunction( ) {
    try {
      if( ( label != nullptr ) && ( predecessor != nullptr ) )
        return( &PathFunction< C, D >::UpdateCompleteData );
      else if( predecessor != nullptr )
        return( &PathFunction< C, D >::UpdatePredecessorData );
      else if( label != nullptr )
        return( &PathFunction< C, D >::UpdateLabelData );
      else
        return( &PathFunction< C, D >::UpdateSimpleData );
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
  void PathFunction< C, D >::UpdateCompleteData( size_t index, size_t adj_index ) {
    try {
      predecessor->operator()( adj_index ) = index;
      label->operator()( adj_index ) = label->operator()( index );
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
  void PathFunction< C, D >::UpdatePredecessorData( size_t index, size_t adj_index ) {
    try {
      predecessor->operator()( adj_index ) = index;
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
  void PathFunction< C, D >::UpdateLabelData( size_t index, size_t adj_index ) {
    try {
      COMMENT( "Updating label.", 3 );
      label->operator()( adj_index ) = label->operator()( index );
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
  void PathFunction< C, D >::UpdateSimpleData( size_t, size_t ) {
  }

  template< template< class D > class C, class D >
  void PathFunction< C, D >::DifferentialPropagation( bool set ) {
    if( ( set ) && ( label != nullptr ) && ( predecessor != nullptr ) )
      PropagateData = &PathFunction< C, D >::PropagateDifferential;
    else
      PropagateData = &PathFunction< C, D >::Propagate;
  }


#ifdef BIAL_EXPLICIT_PathFunction

  template class PathFunction< Image, int >;
  template class PathFunction< Image, llint >;
  template class PathFunction< Image, float >;
  template class PathFunction< Image, double >;

  template class PathFunction< Vector, int >;
  template class PathFunction< Vector, llint >;
  template class PathFunction< Vector, float >;
  template class PathFunction< Vector, double >;

  template class PathFunction< Matrix, int >;
  template class PathFunction< Matrix, llint >;
  template class PathFunction< Matrix, float >;
  template class PathFunction< Matrix, double >;

#endif

}

#endif

#endif
