/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Sep/19
 * @brief Hierarchical path function to compute the initial path-values and propagated path-values in IFT algorithm.
 *        Function used by optimum path forest clustering and classification for hierarchical implementation.
 *        Result is normaly in label map. NOT READY. DO NOT USE.
 */

#ifndef BIALHIERARCHICALPATHFUNCTION_C
#define BIALHIERARCHICALPATHFUNCTION_C

#include "HierarchicalPathFunction.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_HierarchicalPathFunction )
#define BIAL_EXPLICIT_HierarchicalPathFunction
#endif
#if defined ( BIAL_EXPLICIT_HierarchicalPathFunction ) || ( BIAL_IMPLICIT_BIN )

#include "BucketQueueElements.hpp"
#ifdef BIAL_DEBUG
#include "FileImage.hpp"
#endif
#include "Image.hpp"

/* Implementation ***************************************************************************************************** */

namespace Bial {

  template< template< class D > class C, class D >
  HierarchicalPathFunction< C, D >::HierarchicalPathFunction( const C< D > &handicap, C< int > *merge_label, 
                                                              C< int > *split_label, D new_bucket_size ) try :
    PathFunction< C, D >( ), handicap( handicap ), merge_label( merge_label ), split_label( split_label ),
      new_merge_label( ), bucket_size( new_bucket_size ) {
      COMMENT( "Computing the number of labels to initialize new_merge_label.", 1 );
      int labels = 0;
      for( size_t lbl = 0; lbl < merge_label->size( ); ++lbl ) {
        if( labels < merge_label->operator()( lbl ) + 1 ) {
          labels = merge_label->operator()( lbl ) + 1;
        }
      }
      new_merge_label = Vector< int >( labels, -1 );
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
  HierarchicalPathFunction< C, D >::HierarchicalPathFunction( const HierarchicalPathFunction< C, D > &pf ) try : 
    HierarchicalPathFunction< C, D >( pf.handicap, pf.merge_label, pf.split_label, pf.bucket_size ) {
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

  template< template< class D > class C, class D > HierarchicalPathFunction< C, D >::~HierarchicalPathFunction( ) {
  }

  template< template< class D > class C, class D >
  HierarchicalPathFunction< C, D >
  HierarchicalPathFunction< C, D >::operator=( const HierarchicalPathFunction< C, D > &pf ) {
    try {
      return( HierarchicalPathFunction< C, D >( pf ) );
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
  void HierarchicalPathFunction< C, D >::Initialize( C< D > &init_value, C< int > *init_label,
                                                     C< int > *init_predecessor, bool sequential_label ) {
    try {
      if( init_label == nullptr ) {
        BIAL_ERROR( "This pathfunction must be used with label." );
      }
      PathFunction< C, D >::Initialize( init_value, init_label, init_predecessor, sequential_label );
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
  bool HierarchicalPathFunction< C, D >::RemoveSimple( size_t index, BucketState state ) {
    try {
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
  bool HierarchicalPathFunction< C, D >::RemoveLabel( size_t index, BucketState state ) {
    try {
      COMMENT( "merge_label->operator()( index ): " << merge_label->operator()( index ), 4 );
      if( state == BucketState::INSERTED ) {
        this->value->operator()( index ) = handicap( index );
        if( new_merge_label( merge_label->operator()( index ) ) == -1 ) {
          this->label->operator()( index ) = this->next_label;
          new_merge_label( merge_label->operator()( index ) ) = this->next_label;
          ++( this->next_label );
          COMMENT( "new_merge_label( merge_label->operator()( index ) ): "
                   << new_merge_label( merge_label->operator()( index ) ), 4 );
          return( true );
        }
        else {
          this->label->operator()( index ) = new_merge_label( merge_label->operator()( index ) );
          COMMENT( "new_merge_label( merge_label->operator()( index ) ): "
                   << new_merge_label( merge_label->operator()( index ) ), 4 );
          return( true );
        }
      }
      else if( new_merge_label( merge_label->operator()( index ) ) == -1 ) {
        new_merge_label( merge_label->operator()( index ) ) = this->label->operator()( index );
        COMMENT( "new_merge_label( merge_label->operator()( index ) ): "
                 << new_merge_label( merge_label->operator()( index ) ), 4 );
        return( true );
      }
      COMMENT( "new_merge_label( merge_label->operator()( index ) ): "
               << new_merge_label( merge_label->operator()( index ) ), 4 );
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
  inline bool HierarchicalPathFunction< C, D >::Capable( int index, int adj_index, BucketState ) {
    try {
      return( ( split_label->operator()( index ) == split_label->operator()( adj_index ) ) &&
              ( ( ( new_merge_label( merge_label->operator()( adj_index ) ) ) == -1 ) ||
                ( ( new_merge_label( merge_label->operator()( adj_index ) ) ) ==
                  this->label->operator()( index ) ) ) &&
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
  bool HierarchicalPathFunction< C, D >::Propagate( int index, int adj_index ) {
    try {
      D src_value = this->value->operator()( adj_index );
      COMMENT( "Checking conditions related to merge and split label. Merge_label( index ): " << 
               merge_label->operator()( index ) << " Merge_label( adj_index ): " << 
               merge_label->operator()( adj_index ) << " New_merge_label( merge_label( index ) ): " <<
               new_merge_label( merge_label->operator()( index ) ) << " New_merge_label( merge_label( adj_index ) ): "
               << new_merge_label( merge_label->operator()( adj_index ) ), 4 );
      if( ( merge_label->operator()( index ) == merge_label->operator()( adj_index ) ) &&
          ( ( new_merge_label( merge_label->operator()( index ) ) ) == this->label->operator()( index ) ) ) {
        this->value->operator()( adj_index ) = src_value;
        return( true );
      }
      COMMENT( "Checking maximum path function propagation value.", 4 );
      D prp_value = std::min( this->value->operator()( index ), handicap( adj_index ) );
      if( src_value < prp_value ) {
        COMMENT( "Propagating according to max function.", 4 );
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

  template< template< class D > class C, class D >
  bool HierarchicalPathFunction< C, D >::Increasing( ) {
    return( false );
  }

#ifdef BIAL_EXPLICIT_HierarchicalPathFunction

  template class HierarchicalPathFunction< Image, int >;
  template class HierarchicalPathFunction< Image, llint >;
  template class HierarchicalPathFunction< Image, float >;
  template class HierarchicalPathFunction< Image, double >;

  template class HierarchicalPathFunction< Vector, int >;
  template class HierarchicalPathFunction< Vector, llint >;
  template class HierarchicalPathFunction< Vector, float >;
  template class HierarchicalPathFunction< Vector, double >;

  template class HierarchicalPathFunction< Matrix, int >;
  template class HierarchicalPathFunction< Matrix, llint >;
  template class HierarchicalPathFunction< Matrix, float >;
  template class HierarchicalPathFunction< Matrix, double >;

#endif

}

#endif

#endif
