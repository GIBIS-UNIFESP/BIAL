/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Jul/05
 * @brief Adjacency relation with indexes following gray code sequency.
 * <br> Future add-on's: none.
 */

#ifndef BIALADJACENCYGRAYCODE_C
#define BIALADJACENCYGRAYCODE_C

#include "AdjacencyGrayCode.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_AdjacencyGrayCode )
#define BIAL_EXPLICIT_AdjacencyGrayCode
#endif

#if defined ( BIAL_EXPLICIT_AdjacencyGrayCode ) || ( BIAL_IMPLICIT_BIN )

namespace Bial {

  Adjacency AdjacencyType::Gray8( ) {
    try {
      Adjacency result( 2, 8 );
      COMMENT( "Upper left with index 0.", 4 );
      result( 0, 0 ) = -1.0f;
      result( 0, 1 ) = -1.0f;
      COMMENT( "Upper middle with index 1.", 4 );
      result( 1, 0 ) = 0.0f;
      result( 1, 1 ) = -1.0f;
      COMMENT( "Upper right with index 3.", 4 );
      result( 3, 0 ) = 1.0f;
      result( 3, 1 ) = -1.0f;
      COMMENT( "Middle left with index 2.", 4 );
      result( 2, 0 ) = -1.0f;
      result( 2, 1 ) = 0.0f;
      COMMENT( "Middle right with index 7.", 4 );
      result( 7, 0 ) = 1.0f;
      result( 7, 1 ) = 0.0f;
      COMMENT( "Lower left with index 6.", 4 );
      result( 6, 0 ) = -1.0f;
      result( 6, 1 ) = 1.0f;
      COMMENT( "Lower middle with index 4.", 4 );
      result( 4, 0 ) = 0.0f;
      result( 4, 1 ) = 1.0f;
      COMMENT( "Lower right with index 5.", 4 );
      result( 5, 0 ) = 1.0f;
      result( 5, 1 ) = 1.0f;
      return( result );
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

#ifdef BIAL_EXPLICIT_AdjacencyGrayCode

  /* Explicit instantiations. --------------------------------------------------------------------------------------- */

#endif

}

#endif

#endif
