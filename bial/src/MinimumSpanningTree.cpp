/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @file MinimumSpanningTree.hpp
 * @author Fábio Augusto Menocci Cappabianco
 * @date 2012/Sep/26
 * @version 1.0.00
 * @brief
 * Content: Minimum spanning tree class
 * <br> Description: Minimum spanning three algorithm.
 * <br> Future add-on's: Instead of fixed bucket size, use adaptive bucket
 * size, according to the minimum distance in the
 * feature space.
 */

#ifndef BIALMINIMUMSPANNINGTREE_C
#define BIALMINIMUMSPANNINGTREE_C

#include "MinimumSpanningTree.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_MinimumSpanningTree )
#define BIAL_EXPLICIT_MinimumSpanningTree
#endif
#if defined ( BIAL_EXPLICIT_MinimumSpanningTree ) || ( BIAL_IMPLICIT_BIN )

#include "DegeneratedIFT.hpp"
#include "FeaturePathFunction.hpp"

namespace Bial {

  template< template< class D > class C, class D >
  void MinimumSpanningTree::MST( const Feature< D > &feature, C< D > &distance_map, C< int > &predecessor_map ) {
    try {
      if( ( feature.Elements( ) != distance_map.size( ) ) || ( distance_map.size( ) != predecessor_map.size( ) ) ) {
        std::string msg( std::string( __FILE__ ) + ": " + std::to_string( __LINE__ ) + ": " + std::string(
                            __FUNCTION__ ) +
                         ": error: Number of feature vector, distance and predecessor container sizes do not match." );
        throw( std::logic_error( msg ) );
      }
      /* initialization */
      D bucket_size = 1.0;
      Vector< bool > seed( distance_map.size( ), false );
      seed( 0 ) = true;
      FeatureDistanceFunction< C, D > path_function( feature );
      size_t size = distance_map.size( );
      for( size_t elm = 0; elm < size; ++elm ) {
        if( seed( elm ) )
          distance_map( elm ) = 0;
        else
          distance_map( elm ) = std::numeric_limits< D >::max( );
      }
      DegeneratedIFT< C, D > ift( distance_map, &path_function, &seed, static_cast< C< int >* >( nullptr ), 
                                    &predecessor_map, false, bucket_size, true );
      ift.Run( );
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

#ifdef BIAL_EXPLICIT_MinimumSpanningTree

  template void MinimumSpanningTree::MST( const Feature< int > &feature, Image< int > &distance_map, Image< int > &predecessor_map );
  template void MinimumSpanningTree::MST( const Feature< int > &feature, Matrix< int > &distance_map, Matrix< int > &predecessor_map );
  template void MinimumSpanningTree::MST( const Feature< int > &feature, Vector< int > &distance_map, Vector< int > &predecessor_map );

  template void MinimumSpanningTree::MST( const Feature< llint > &feature, Image< llint > &distance_map, Image< int > &predecessor_map );
  template void MinimumSpanningTree::MST( const Feature< llint > &feature, Matrix< llint > &distance_map, Matrix< int > &predecessor_map );
  template void MinimumSpanningTree::MST( const Feature< llint > &feature, Vector< llint > &distance_map, Vector< int > &predecessor_map );

  template void MinimumSpanningTree::MST( const Feature< float > &feature, Image< float > &distance_map, Image< int > &predecessor_map );
  template void MinimumSpanningTree::MST( const Feature< float > &feature, Matrix< float > &distance_map, Matrix< int > &predecessor_map );
  template void MinimumSpanningTree::MST( const Feature< float > &feature, Vector< float > &distance_map, Vector< int > &predecessor_map );

  template void MinimumSpanningTree::MST( const Feature< double > &feature, Image< double > &distance_map, Image< int > &predecessor_map );
  template void MinimumSpanningTree::MST( const Feature< double > &feature, Matrix< double > &distance_map, Matrix< int > &predecessor_map );
  template void MinimumSpanningTree::MST( const Feature< double > &feature, Vector< double > &distance_map, Vector< int > &predecessor_map );

#endif

}

#endif

#endif
