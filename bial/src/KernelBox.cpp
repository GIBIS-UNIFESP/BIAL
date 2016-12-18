/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Nov/19 
 * @brief Box kernel generator.
 */

#ifndef BIALKERNELBOX_C
#define BIALKERNELBOX_C

#include "KernelBox.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_KernelBox )
#define BIAL_EXPLICIT_KernelBox
#endif
#if defined ( BIAL_EXPLICIT_KernelBox ) || ( BIAL_IMPLICIT_BIN )

namespace Bial {

  Kernel KernelType::Box( size_t dimensions, size_t side_lenght ) {
    try {
      COMMENT( "Creating kernel.", 2 );
      size_t elements = std::pow( side_lenght, dimensions );
      Kernel krn = Kernel( elements, dimensions );
      COMMENT( "Setting kernel displacements.", 2 );
      int max_side = side_lenght / 2;
      int min_side = -max_side + ( ( side_lenght + 1 ) % 2 ); /* Even lenghts contain one less element. */
      Vector< int > delta( dimensions, max_side );
      for( size_t elm = 0; elm < elements; ++elm ) {
        COMMENT( "Updating coordinates", 2 );
        for( size_t dms = 0; dms < dimensions; ++dms ) {
          if( delta( dms ) < max_side ) {
            ++delta( dms );
            break;
          }
          else {
            delta( dms ) = min_side;
          }
        }
        COMMENT( "Assigning displacements", 2 );
        for( size_t dms = 0; dms < dimensions; ++dms ) {
          krn( elm, dms ) = delta( dms );
        }
      }
      return( krn );
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

#ifdef BIAL_EXPLICIT_KernelBox


#endif

}

#endif

#endif
