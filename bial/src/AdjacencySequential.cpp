/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2018/Dec/17
 * @brief Adjacency relation with sequential clock-wise indexes.
 * <br> Future add-on's: none.
 */

#ifndef BIALADJACENCYSEQUENTIAL_C
#define BIALADJACENCYSEQUENTIAL_C

#include "AdjacencySequential.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_AdjacencySequential )
#define BIAL_EXPLICIT_AdjacencySequential
#endif

#if defined ( BIAL_EXPLICIT_AdjacencySequential ) || ( BIAL_IMPLICIT_BIN )

namespace Bial {

  Adjacency AdjacencyType::Sequential( float radius ) {
    try {
      if( ( radius < 1.0f ) || ( radius >= 2.0 ) ) {
	std::string msg( BIAL_ERROR( "Adjacency relation with radius lower than 1.0, or equal to or greater than 2.0." ) );
	throw( std::logic_error( msg ) );
      }
      if( radius < std::sqrt( 2.0f ) ) {
	COMMENT( "1 pixel radius.", 0 );
	Adjacency result( 4, 2 );
	COMMENT( "Upper middle with index 1.", 4 ); // 0 - N
	result( 0, 0 ) = 0.0f;
	result( 0, 1 ) = -1.0f;
	COMMENT( "Middle right with index 7.", 4 ); // 1 - E
	result( 1, 0 ) = 1.0f;
	result( 1, 1 ) = 0.0f;
	COMMENT( "Lower middle with index 4.", 4 ); // 2 - S
	result( 2, 0 ) = 0.0f;
	result( 2, 1 ) = 1.0f;
	COMMENT( "Middle left with index 2.", 4 ); // 3 - W
	result( 3, 0 ) = -1.0f;
	result( 3, 1 ) = 0.0f;
	return( result );
      }
      else {
	Adjacency result( 8, 2 );
	COMMENT( "Upper left with index 0.", 4 ); // 0 - NW
	result( 0, 0 ) = -1.0f;
	result( 0, 1 ) = -1.0f;
	COMMENT( "Upper middle with index 1.", 4 ); // 1 - N
	result( 1, 0 ) = 0.0f;
	result( 1, 1 ) = -1.0f;
	COMMENT( "Upper right with index 3.", 4 ); // 2 - NE
	result( 2, 0 ) = 1.0f;
	result( 2, 1 ) = -1.0f;
	COMMENT( "Middle right with index 7.", 4 ); // 3 - E
	result( 3, 0 ) = 1.0f;
	result( 3, 1 ) = 0.0f;
	COMMENT( "Lower right with index 5.", 4 ); // 4 - SE
	result( 4, 0 ) = 1.0f;
	result( 4, 1 ) = 1.0f;
	COMMENT( "Lower middle with index 4.", 4 ); // 5 - S
	result( 5, 0 ) = 0.0f;
	result( 5, 1 ) = 1.0f;
	COMMENT( "Lower left with index 6.", 4 ); // 6 - SW
	result( 6, 0 ) = -1.0f;
	result( 6, 1 ) = 1.0f;
	COMMENT( "Middle left with index 2.", 4 ); // 7 - W
	result( 7, 0 ) = -1.0f;
	result( 7, 1 ) = 0.0f;
	return( result );
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

#ifdef BIAL_EXPLICIT_AdjacencySequential

  /* Explicit instantiations. --------------------------------------------------------------------------------------- */

#endif

}

#endif

#endif
