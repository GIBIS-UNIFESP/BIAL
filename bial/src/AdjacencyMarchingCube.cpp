/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Jul/05
 * @brief Adjacency relation to be used over an image or matrix.
 * <br> Future add-on's: none.
 */

#ifndef BIALADJACENCYMARCHINGCUBE_C
#define BIALADJACENCYMARCHINGCUBE_C

#include "AdjacencyMarchingCube.hpp"
#include "Image.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_AdjacencyMarchingCube )
#define BIAL_EXPLICIT_AdjacencyMarchingCube
#endif

#if defined ( BIAL_EXPLICIT_AdjacencyMarchingCube ) || ( BIAL_IMPLICIT_BIN )

namespace Bial {

  Adjacency AdjacencyType::MarchingCube( ) {
    try {
      COMMENT( "Initializing the adjacency relation. Put central element in the first position.", 1 );
      /* Lorensen's marching cube: */
      /* http://paulbourke.net/geometry/polygonise/ marching cube: */
      Adjacency adj( 3, 8 );
      //adj.relation.Set( 0.f );
      adj( 0, 2 ) = 1.f; /* V0 = (0,0,1) */
      adj( 1, 0 ) = adj( 1, 2 ) = 1.f; /* V1 = (1,0,1) */
      adj( 2, 0 ) = 1.f; /* V2 = (1,0,0) */
      /* V3 = (0,0,0) */
      adj( 4, 1 ) = adj( 4, 2 ) = 1.f; /* V4 = (0,1,1) */
      adj( 5, 0 ) = adj( 5, 1 ) = adj( 5, 2 ) = 1.f; /* V5 = (1,1,1) */
      adj( 6, 0 ) = adj( 6, 1 ) = 1.f; /* V6 = (1,1,0) */
      adj( 7, 1 ) = 1.f; /* V7 = (0,1,0) */
      return( adj );
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

#ifdef BIAL_EXPLICIT_AdjacencyMarchingCube

  /* Explicit instantiations. --------------------------------------------------------------------------------------- */

#endif

}

#endif

#endif
