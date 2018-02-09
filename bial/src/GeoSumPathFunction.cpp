/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Sep/19
 * @brief Geodesic restriction sum path function to compute the initial path-values and propagated path-values in IFT
 *        algorithm. This function is used to compute predecessor map for Geodesic Star Functions.
 */

#ifndef BIALGEOSUMPATHFUNCTION_C
#define BIALGEOSUMPATHFUNCTION_C

#include "GeoSumPathFunction.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_GeoSumPathFunction )
#define BIAL_EXPLICIT_GeoSumPathFunction
#endif
#if defined ( BIAL_EXPLICIT_GeoSumPathFunction ) || ( BIAL_IMPLICIT_BIN )

#include "BucketQueueElements.hpp"
#include "DFIDE.hpp"
#ifdef BIAL_DEBUG
#include "FileImage.hpp"
#endif

/* Implementation *************************************************************************************************** */

namespace Bial {

  template< class D >
  GeodesicRestrictionPathFunction< D >::GeodesicRestrictionPathFunction( Image< D > &init_value, 
                                                                         Image< int > *init_label,
                                                                         Image< int > *init_predecessor, 
                                                                         bool sequential_label, 
                                                                         const Image< D > &handicap,
                                                                         const Image< D > &new_intensity,
                                                                         const Adjacency &adj,
                                                                         double new_alpha, double new_beta ) try : 
    PathFunction< Image, D >( init_value, init_label, init_predecessor, sequential_label ), intensity( new_intensity ),
      handicap( handicap ), alpha( new_alpha ), beta( new_beta ), dists( ) {
      if( intensity.Dims( ) != init_value.Dims( ) ) {
        std::string msg( BIAL_ERROR( "Intensity and value image dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      if( ( alpha < -1.0 ) || ( alpha > 1.0 ) ) {
        std::string msg( BIAL_ERROR( "Invalid alpha. Expected: -1.0 to 1.0. Given: " + 
                                     std::to_string( alpha ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      if( ( beta < 0.0 ) || ( beta > 4.0 ) ) {
        std::string msg( BIAL_ERROR( "Invalid beta. Expected: 0.0 to 4.0. Given: " + std::to_string( beta ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      size_t adj_size = adj.size( );
      dists = Vector< double >( adj_size, 0.0 );
      for( size_t elm = 0; elm < adj_size; ++elm )
        dists[ elm ] = std::sqrt( adj( elm, 0 ) * adj( elm, 0 ) + adj( elm, 1 ) * adj( elm, 1 ) + 
                                  adj( elm, 2 ) * adj( elm, 2 ) );
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
  GeodesicRestrictionPathFunction< D >::
  GeodesicRestrictionPathFunction( const GeodesicRestrictionPathFunction< D > &pf )
    try : GeodesicRestrictionPathFunction< D >( *( pf.value ), pf.label, pf.predecessor, true, pf.handicap, 
                                                pf.intensity, Adjacency( ), pf.alpha, pf.beta ) {
      this->next_label = pf.next_label;
      dists = pf.dists;
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
  GeodesicRestrictionPathFunction< D >::~GeodesicRestrictionPathFunction( ) {
  }

  template< class D >
  GeodesicRestrictionPathFunction< D >
  GeodesicRestrictionPathFunction< D >::operator=( const GeodesicRestrictionPathFunction< D > &pf ) {
    try {
      return( GeodesicRestrictionPathFunction< D >( pf ) );
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
  bool GeodesicRestrictionPathFunction< D >::RemoveSimple( size_t, BucketState ) {
    return( true );
  }

  template< class D >
  bool GeodesicRestrictionPathFunction< D >::RemovePredecessor( size_t index, BucketState state ) {
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

  template< class D >
  bool GeodesicRestrictionPathFunction< D >::RemoveLabel( size_t index, BucketState state ) {
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

  template< class D >
  bool GeodesicRestrictionPathFunction< D >::RemoveComplete( size_t index, BucketState state ) {
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

  template< class D >
  inline bool GeodesicRestrictionPathFunction< D >::Capable( size_t index, size_t adj_index, BucketState adj_state ) {
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
  bool GeodesicRestrictionPathFunction< D >::PropagateDifferential( size_t index, size_t adj_index, size_t adj_pos ) {
    try {
      D src_value = this->value->operator()( adj_index );
      COMMENT( "Computing arc weight.", 3 );
      double arc_weight = handicap( index ) + handicap( adj_index );
      COMMENT( "Orienting edges.", 3 );
      double fraction = 0;
      // if( intensity[ index ] > intensity[ adj_index ] )
      //   fraction = std::abs( alpha );
      // else if( intensity[ index ] < intensity[ adj_index ] )
      //   fraction = -std::abs( alpha );
      // if( alpha < 0.0 )
      //   fraction = -fraction;
      if( ( intensity[ index ] - intensity[ adj_index ] ) * alpha >= 0.0 )
        fraction = std::abs( alpha );
      else
        fraction = -std::abs( alpha );
      arc_weight = std::round( arc_weight * ( 1.0 + fraction ) );
      COMMENT( "Suppressing non-zero.", 3 );
      ++arc_weight;
      COMMENT( "Computing spacial distance.", 3 );
      double distance = dists( adj_pos );
      COMMENT( "Propagated value.", 3 );
      D prp_value = static_cast< D >( this->value->operator()( index ) + std::pow( arc_weight, beta )
                                      - 1.0 + distance );
      COMMENT( "Updating value.", 3 );
      if( ( src_value > prp_value ) ||
          ( ( this->predecessor->operator()( adj_index ) == static_cast< int >( index ) ) &&
            ( this->label->operator()( adj_index ) != this->label->operator()( index ) ) ) ) {
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

  template< class D >
  bool GeodesicRestrictionPathFunction< D >::Propagate( size_t index, size_t adj_index, size_t adj_pos ) {
    try {
      D src_value = this->value->operator()( adj_index );
      COMMENT( "Computing arc weight.", 3 );
      double arc_weight = handicap( index ) + handicap( adj_index );
      COMMENT( "Orienting edges.", 3 );
      double fraction = 0;
      // if( intensity[ index ] > intensity[ adj_index ] ) {
      //   fraction = std::abs( alpha );
      // }
      // else if( intensity[ index ] < intensity[ adj_index ] ) {
      //   fraction = -std::abs( alpha );
      // }
      // if( alpha < 0.0 ) {
      //   fraction = -fraction;
      // }
      double tmp = ( intensity[ index ] - intensity[ adj_index ] ) * alpha;
      if( tmp > 0.0 )
        fraction = std::abs( alpha );
      else if( tmp < 0.0 )
        fraction = -std::abs( alpha );
      arc_weight = std::round( arc_weight * ( 1.0 + fraction ) );
      COMMENT( "Suppressing non-zero.", 3 );
      ++arc_weight;
      COMMENT( "Computing spacial distance.", 3 );
      double distance = dists( adj_pos );
      COMMENT( "Propagated value.", 3 );
      D prp_value = static_cast< D >( this->value->operator()( index ) + std::pow( arc_weight, beta )
                                      - 1.0 + distance );
      COMMENT( "Updating value.", 3 );
      if( src_value > prp_value ) {
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

  template< class D >
  bool GeodesicRestrictionPathFunction< D >::Increasing( ) {
    return( true );
  }

#ifdef BIAL_EXPLICIT_GeoSumPathFunction

  template class GeodesicRestrictionPathFunction< int >;
  template class GeodesicRestrictionPathFunction< llint >;
  template class GeodesicRestrictionPathFunction< float >;
  template class GeodesicRestrictionPathFunction< double >;

#endif

}

#endif

#endif
