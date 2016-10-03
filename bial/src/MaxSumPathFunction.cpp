/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Sep/19
 * @brief Max sum path function to compute the initial path-values and propagated path-values in IFT algorithm.
 *        This function is has a mixed behavior of maximum and sum path functions. It may adopt a direction.
 */

#ifndef BIALMAXSUMPATHFUNCTION_C
#define BIALMAXSUMPATHFUNCTION_C

#include "MaxSumPathFunction.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_MaxSumPathFunction )
#define BIAL_EXPLICIT_MaxSumPathFunction
#endif
#if defined ( BIAL_EXPLICIT_MaxSumPathFunction ) || ( BIAL_IMPLICIT_BIN )

#include "BucketQueueElements.hpp"
#ifdef BIAL_DEBUG
#include "FileImage.hpp"
#endif
#include "Image.hpp"

/* Implementation ***************************************************************************************************** */

namespace Bial {

  template< template< class D > class C, class D >
  MaxSumPathFunction< C, D >::MaxSumPathFunction( const C< D > &intensity, const C< D > &handicap, double new_alpha, 
                                                  double new_beta ) try :
    PathFunction< C, D >( ), intensity( intensity ), handicap( handicap ), alpha( new_alpha ), beta( new_beta ) {
      if( ( alpha < -1.0 ) || ( alpha > 1.0 ) ) {
        std::string msg( BIAL_ERROR( "Invalid alpha. Expected: -1.0 to 1.0. Given: " +
                                     std::to_string( alpha ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      if( ( beta < 0.0 ) || ( beta > 10.0 ) ) {
        std::string msg( BIAL_ERROR( "Invalid beta. Expected: 0.0 to 10.0. Given: " + std::to_string( beta ) + "." ) );
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
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Empty kernel." ) );
    throw( std::logic_error( msg ) );
  }

  template< template< class D > class C, class D >
  MaxSumPathFunction< C, D >::MaxSumPathFunction( const MaxSumPathFunction< C, D > &pf )
    try : MaxSumPathFunction< C, D >( pf.intensity, pf.handicap, pf.alpha, pf.beta ) {
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

  template< template< class D > class C, class D > MaxSumPathFunction< C, D >::~MaxSumPathFunction( ) {
  }

  template< template< class D > class C, class D >
  MaxSumPathFunction< C, D > MaxSumPathFunction< C, D >::operator=( const MaxSumPathFunction< C, D > &pf ) {
    try {
      return( MaxSumPathFunction< C, D >( pf ) );
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
  void MaxSumPathFunction< C, D >::Initialize( C< D > &init_value, C< int > *init_label, C< int > *init_predecessor, 
                                               bool sequential_label ) {
    try {
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
  bool MaxSumPathFunction< C, D >::RemoveSimple( size_t index, BucketState state ) {
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
  bool MaxSumPathFunction< C, D >::RemoveLabel( size_t index, BucketState state ) {
    try {
      COMMENT( "Removing element.", 3 );
      if( state == BucketState::INSERTED ) {
        COMMENT( "Next label: " << this->next_label << ".", 3 );
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
  bool MaxSumPathFunction< C, D >::Capable( int index, int adj_index, BucketState ) {
    try {
      return( this->value->operator()( index ) < this->value->operator()( adj_index ) );
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
  bool MaxSumPathFunction< C, D >::Propagate( int index, int adj_index ) {
    try {
      D src_value = this->value->operator()( adj_index );
      COMMENT( "Computing arc weight.", 3 );
      double arc_weight = ( handicap( index ) + handicap( adj_index ) ) / 2.0;
      COMMENT( "Orienting edges.", 3 );
      double fraction = 0.0;
      if( intensity[ index ] > intensity[ adj_index ] ) {
        fraction = std::abs( alpha );
      }
      else if( intensity[ index ] < intensity[ adj_index ] ) {
        fraction = -std::abs( alpha );
      }
      if( alpha < 0.0 ) {
        fraction = -fraction;
      }
      arc_weight = std::round( arc_weight * ( 1.0 + fraction ) );
      COMMENT( "Suppressing non-zero.", 3 );
      ++arc_weight;
      COMMENT( "Propagated value.", 3 );
      D prp_value = static_cast< D >( this->value->operator()( index ) + arc_weight - 1.0 );
      COMMENT( "Updating value.", 3 );
      if( src_value > prp_value ) {
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
  bool MaxSumPathFunction< C, D >::Increasing( ) {
    return( true );
  }

#ifdef BIAL_EXPLICIT_MaxSumPathFunction

  template class MaxSumPathFunction< Image, int >;
  template class MaxSumPathFunction< Image, llint >;
  template class MaxSumPathFunction< Image, float >;
  template class MaxSumPathFunction< Image, double >;

  template class MaxSumPathFunction< Vector, int >;
  template class MaxSumPathFunction< Vector, llint >;
  template class MaxSumPathFunction< Vector, float >;
  template class MaxSumPathFunction< Vector, double >;

  template class MaxSumPathFunction< Matrix, int >;
  template class MaxSumPathFunction< Matrix, llint >;
  template class MaxSumPathFunction< Matrix, float >;
  template class MaxSumPathFunction< Matrix, double >;

#endif

}

#endif

#endif
