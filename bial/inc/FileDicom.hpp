/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Aug/08 
 * @brief Reading dicom files.
 * @warning Only a small set of dicom files may open correctly. Probably not even possible to implement all.
 */

#include "Common.hpp"

#ifndef BIALFILEDICOM_H
#define BIALFILEDICOM_H

namespace Bial {

  template< class D >
  class Image;

  /**
   * @date 2013/Oct/28
   * @param filename: Source filename to be readed.
   * @return A reference to the created scene.
   * @brief Open a Dicom file for reading and returns it.
   * @warning none.
   */
  template< class D >
  static Image< D > ReadDicom( const std::string &filename );
}

/* Implementation --------------------------------------------------------------------------------------------------- */

#include "Color.hpp"
#include "DicomHeader.hpp"

namespace Bial {

  template< class D >
  Image< D > ReadDicom( const std::string &filename ) {
    try {
      DicomHeader dhdr;
      return( dhdr.readFile< D >( filename ) );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error opening/reading/closing Dicom file." ) );
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
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< >
  Image< Color > ReadDicom( const std::string &filename ) {
    try {
      DicomHeader dhdr;
      return( dhdr.readFile< Color >( filename ) );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error opening/reading/closing Dicom file." ) );
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
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

}

#endif
