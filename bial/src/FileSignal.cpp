/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/11 
 * @brief Functions to write signal into file.
 */

#ifndef BIALFILESIGNAL_C
#define BIALFILESIGNAL_C

#include "FileSignal.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_FileSignal )
#define BIAL_EXPLICIT_FileSignal
#endif

#if defined ( BIAL_EXPLICIT_FileSignal ) || ( BIAL_IMPLICIT_BIN )

#include "File.hpp"
#include "Signal.hpp"

namespace Bial {

  void Write( const Signal &sgn, const std::string &filename ) {
    try {
      OFile ofile;
      size_t size = sgn.size( );
      ofile.open( filename );
      ofile << size << std::endl;
      for( size_t it = 0; it < size; ++it )
        ofile << sgn.Data( it ) << " " << sgn[ it ] << std::endl;
      ofile.close( );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while writing to file." ) );
      throw( std::ios_base::failure( msg ) );
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

#ifdef BIAL_EXPLICIT_FileSignal

#endif

}

#endif

#endif
