/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Sep/19
 * @brief Feature distance path function to compute the initial path-values and propagated path-values in IFT algorithm.
 *        Function for classification of classifiers results. Result is both in value (probabilities) and label maps.
 */

#ifndef BIALFEATUREPATHFUNCTION_C
#define BIALFEATUREPATHFUNCTION_C

#include "FeaturePathFunction.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_FeaturePathFunction )
#define BIAL_EXPLICIT_FeaturePathFunction
#endif
#if defined ( BIAL_EXPLICIT_FeaturePathFunction ) || ( BIAL_IMPLICIT_BIN )

#include "BucketQueueElements.hpp"
#include "DFIDE.hpp"
#include "Feature.hpp"
#ifdef BIAL_DEBUG
#include "FileImage.hpp"
#endif
#include "Image.hpp"

/* Implementation *************************************************************************************************** */

namespace Bial {

  template< template< class D > class C, class D > 
  FeatureDistanceFunction< C, D >::FeatureDistanceFunction( C< D > &init_value, C< int > *init_label, 
                                                            C< int > *init_predecessor, bool sequential_label, 
                                                            const Feature< D > &new_feats ) try : 
    PathFunction< C, D >( init_value, init_label, init_predecessor, sequential_label ), feats( new_feats ) {
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
  bool FeatureDistanceFunction< C, D >::RemoveSimple( size_t, BucketState ) {
    return( true );
  }

  template< template< class D > class C, class D >
  bool FeatureDistanceFunction< C, D >::RemovePredecessor( size_t index, BucketState state ) {
    try {
      if( state == BucketState::INSERTED ) {
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
  bool FeatureDistanceFunction< C, D >::RemoveLabel( size_t index, BucketState state ) {
    try {
      if( state == BucketState::INSERTED ) {
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
  bool FeatureDistanceFunction< C, D >::RemoveComplete( size_t index, BucketState state ) {
    try {
      if( state == BucketState::INSERTED ) {
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
  inline bool FeatureDistanceFunction< C, D >::Capable( int, int adj_index, BucketState adj_state ) {
    try {
      return( ( adj_state != BucketState::REMOVED ) && ( this->value->operator()( adj_index ) > 0.0 ) );
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
  bool FeatureDistanceFunction< C, D >::Propagate( int index, int adj_index ) {
    try {
      double distance = DFIDE::Distance( feats, feats, index * feats.Features( ), adj_index * feats.Features( ),
                                         feats.Features( ) );
      if( this->value->operator()( adj_index ) > distance ) {
        this->value->operator()( adj_index ) = distance;
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
  bool FeatureDistanceFunction< C, D >::Increasing( ) {
    return( true );
  }

#ifdef BIAL_EXPLICIT_FeaturePathFunction

  template class FeatureDistanceFunction< Image, int >;
  template class FeatureDistanceFunction< Image, llint >;
  template class FeatureDistanceFunction< Image, float >;
  template class FeatureDistanceFunction< Image, double >;

  template class FeatureDistanceFunction< Vector, int >;
  template class FeatureDistanceFunction< Vector, llint >;
  template class FeatureDistanceFunction< Vector, float >;
  template class FeatureDistanceFunction< Vector, double >;

  template class FeatureDistanceFunction< Matrix, int >;
  template class FeatureDistanceFunction< Matrix, llint >;
  template class FeatureDistanceFunction< Matrix, float >;
  template class FeatureDistanceFunction< Matrix, double >;

#endif

}

#endif

#endif
