/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Fábio Augusto Menocci Cappabianco */
/* Date: 2015/Oct/06 */
/* Version: 2.0.00 */
/* Content: Image display class implementation. */
/* Description: class to handle visualization. */
/* Future add-on's: unknown. */

#ifndef BIALDISPLAY_C
#define BIALDISPLAY_C

#include "Display.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_Display )
#define BIAL_EXPLICIT_Display
#endif

#if defined ( BIAL_EXPLICIT_Display ) || ( BIAL_IMPLICIT_BIN )

#include "Color.hpp"
#include "File.hpp"
#include "Image.hpp"

namespace Bial {

  std::string Display::ShowCommand( const std::string &filename ) {
    std::string command( "BIAL_Qt " + filename );
    return( command );
  }

  void Display::Show( const std::string &filename ) {
    std::string command( Display::ShowCommand( filename ) );
    int status = system( &command[ 0 ] );
    if( status != 0 ) {
      std::string msg( BIAL_ERROR( "Could not open image viewer." ) );
      throw( std::system_error( std::error_code( ), msg ) );
    }
  }

#ifdef BIAL_EXPLICIT_Display

#endif

}

#endif

#endif
