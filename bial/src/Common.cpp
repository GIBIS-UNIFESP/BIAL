/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Jun/21 */
/* Content: Commonly used macros */
/* Description: Some macros used by many classes. */

#ifndef BIALCOMMON_C
#define BIALCOMMON_C

#include "Common.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_Common )
#define BIAL_EXPLICIT_Common
#endif

#if defined ( BIAL_EXPLICIT_Common ) || ( BIAL_IMPLICIT_BIN )

namespace Bial {

#ifdef IS_WINDOWS_PLATFORM

  FILE* Bial_popen( const char *command, const char *type ) {
    return( _popen( command, type ) );
  }

  int Bial_pclose( FILE *stream ) {
    return( _pclose( stream ) );
  }

  std::string DirectoryExistsCommand( const std::string &dirname ) {
    return( "dir " + dirname + " &> nul" );
  }

  std::string FileDirectoryListCommand( const std::string &dirname ) {
    return( "dir /B " + dirname );
  }

  std::string DirectoryListCommand( const std::string &dirname ) {
    return( "dir /B/AD " + dirname );
  }

  std::string FileListCommand( const std::string &dirname ) {
    return( "dir /B/A-D " + dirname );
  }

  std::string FileListCommand( const std::string &dirname, const std::string &extension ) {
    if( dirname[ dirname.size( ) - 1 ] == '\\' ) {
      if( extension[ 0 ] == '.' ) {
        return( "dir /B/A-D " + dirname + "*" + extension );
      }
      return( "dir /B/A-D " + dirname + "*." + extension );
    }
    if( extension[ 0 ] == '.' ) {
      return( "dir /B/A-D " + dirname + "\\*" + extension );
    }
    return( "dir /B/A-D " + dirname + "\\*." + extension );
  }

  std::string MakeDirCommand( const std::string &dirname ) {
    return( "mkdir " + dirname );
  }

#else

  FILE* Bial_popen( const char *command, const char *type ) {
    return( popen( command, type ) );
  }

  int Bial_pclose( FILE *stream ) {
    return( pclose( stream ) );
  }

  std::string DirectoryExistsCommand( const std::string &dirname ) {
    return( "ls " + dirname + " &> /dev/null" );
  }

  std::string FileDirectoryListCommand( const std::string &dirname ) {
    return( "ls -1 " + dirname );
  }

  std::string DirectoryListCommand( const std::string &dirname ) {
    return( "ls -1F " + dirname + " | grep \"/\" | tr -d \"/\" " );
  }

  std::string FileListCommand( const std::string &dirname ) {
    return( "ls -1F " + dirname + " | grep -v \"/\" | tr -d \"*@\" " );
  }

  std::string FileListCommand( const std::string &dirname, const std::string &extension ) {
    return( "ls -1F " + dirname + " | grep -v \"/\" | grep \"" + extension + "\" | tr -d \"*@\" " );
  }

  std::string MakeDirCommand( const std::string &dirname ) {
    return( "mkdir -p " + dirname );
  }

#endif

#ifdef BIAL_DEBUG
  int verbose = BIAL_DEBUG;
#else
  int verbose = 0;
#endif

}

#endif

#endif
