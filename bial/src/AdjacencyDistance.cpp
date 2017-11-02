/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Jul/05
 * @brief Adjacency iterator class used as a forward iteratior for adjacency relation.
 * <br> Future add-on's: none.
 */

#ifndef BIALADJACENCYDISTANCE_C
#define BIALADJACENCYDISTANCE_C

#include "AdjacencyDistance.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_AdjacencyIterator )
#define BIAL_EXPLICIT_AdjacencyIterator
#endif

#if defined ( BIAL_EXPLICIT_AdjacencyIterator ) || ( BIAL_IMPLICIT_BIN )

namespace Bial {

  AdjacencyDistance::AdjacencyDistance( const Adjacency &adj ) try :
    distance( adj.size( ), 0.0f ) {
      COMMENT( "Computing distance vector.", 0 );
      size_t distance_size = distance.size( );
      size_t dimensions = adj.dims( );
      for( size_t elm = 0; elm < distance_size; ++elm ) {
        for( size_t dms = 0; dms < dimensions; ++dms )
          distance[ elm ] += adj( elm, dms ) * adj( elm, dms );
        distance[ elm ] = std::sqrt( distance[ elm ] );
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

  const float AdjacencyDistance::&operator[]( size_t elm ) const {
    try {
      return( distance[ elm ] );
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
  
#ifdef BIAL_EXPLICIT_AdjacencyDistance
  
#endif

}

#endif

#endif
