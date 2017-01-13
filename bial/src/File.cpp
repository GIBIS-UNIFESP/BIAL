/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2013/Aug/08
 * @brief File and Directory Handling.
 * @warning random access from IFile does not work for gziped files, since gzstream does not support it.
 */

#ifndef BIALFILE_C
#define BIALFILE_C

#include "File.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_File )
#define BIAL_EXPLICIT_File
#endif

#if defined ( BIAL_EXPLICIT_File ) || ( BIAL_IMPLICIT_BIN )

namespace Bial {

  /* Terminal -------------------------------------------------------------------- */

  bool Terminal::Exists( ) {
    try {
      int has_terminal = std::system( nullptr );
      if( has_terminal == 0 ) {
        return( false );
      }
      return( true );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  /* Directory ------------------------------------------------------------------- */

  bool Directory::Exists( const std::string &dir_name ) {
    try {
      std::string command( DirectoryExistsCommand( dir_name ) );
      COMMENT( "Command: " << command, 3 );
      int exists = std::system( &command[ 0 ] );
      if( exists != 0 ) {
        return( false );
      }
      return( true );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  Vector< std::string > Directory::ListDirectories( const std::string &dir_name, bool recursive ) {

    try {
      COMMENT( "Cheking if terminal is available.", 4 );
      if( !Terminal::Exists( ) ) {
        std::string msg( BIAL_ERROR( "Terminal not available." ) );
        throw( std::ios_base::failure( msg ) );
      }
      COMMENT( "Reading root directory.", 4 );
      std::string command = DirectoryListCommand( dir_name );
      FILE *dirs = Bial_popen( &command[ 0 ], "r" );
      if( !dirs ) {
        std::string msg( BIAL_ERROR( "Directory " + dir_name + " does not exists or is not accessible." ) );
        throw( std::ios_base::failure( msg ) );
      }
      COMMENT( "Listing directories.", 4 );
      Vector< std::string > result;
      while( !std::feof( dirs ) ) {
        char buffer[ 128 ];
        if( fgets( buffer, 128, dirs ) != nullptr ) {

          COMMENT( "Setting subdirs names in resulting list.", 4 );
          std::string sub_name = dir_name;
          if( sub_name[ sub_name.size( ) - 1 ] != DIR_SEPARATOR ) { /* Inserting separator character if necessary. */
            sub_name += DIR_SEPARATOR;
          }
          sub_name += buffer;
          sub_name.erase( sub_name.size( ) - 1 ); /* remove '\n'. */
          result.push_back( sub_name );

          COMMENT( "Recursively include subdirs.", 4 );
          if( recursive ) {
            Vector< std::string > sub_result = Directory::ListDirectories( sub_name, recursive );
            result.insert( result.begin( ), sub_result.begin( ), sub_result.end( ) );
          }
        }
      }
      Bial_pclose( dirs );
      return( result );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while accessing directory." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  Vector< std::string > Directory::ListFiles( const std::string &dir_name, bool recursive ) {
    try {
      COMMENT( "Cheking if terminal is available.", 4 );
      if( !Terminal::Exists( ) ) {
        std::string msg( BIAL_ERROR( "Terminal not available." ) );
        throw( std::ios_base::failure( msg ) );
      }
      COMMENT( "Recursive search.", 4 );
      Vector< std::string > result;
      if( recursive ) {
        COMMENT( "Getting subdirectories recursively.", 4 );
        Vector< std::string > subdirs = Directory::ListDirectories( dir_name, true );
        COMMENT( "Getting files in each subdirectory.", 4 );
        for( size_t idx = 0; idx < subdirs.size( ); ++idx ) {
          Vector< std::string > sub_result = Directory::ListFiles( subdirs( idx ), false );
          result.insert( result.begin( ), sub_result.begin( ), sub_result.end( ) );
        }
      }
      COMMENT( "Checking for directory accessibility.", 4 );
      std::string command = FileListCommand( dir_name );
      FILE *files = Bial_popen( &command[ 0 ], "r" );
      if( !files ) {
        std::string msg( BIAL_ERROR( "Directory " + dir_name + " not accessible." ) );
        throw( std::ios_base::failure( msg ) );
      }
      COMMENT( "Getting files.", 4 );
      while( !std::feof( files ) ) {
        char buffer[ 128 ];
        if( fgets( buffer, 128, files ) != nullptr ) {
          buffer[ strlen( buffer ) - 1 ] = '\0'; /* remove '\n'. */
          result.push_back( std::string( buffer ) );
        }
      }
      Bial_pclose( files );

      return( result );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while accessing directory." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  Vector< std::string > Directory::ListFiles( const std::string &dir_name,
                                                     const std::string &extension,
                                                     bool recursive ) {
    try {
      COMMENT( "Cheking if terminal is available.", 2 );
      if( !Terminal::Exists( ) ) {
        std::string msg( BIAL_ERROR( "Terminal not available." ) );
        throw( std::ios_base::failure( msg ) );
      }
      COMMENT( "Recursive search.", 2 );
      Vector< std::string > result;
      if( recursive ) {
        COMMENT( "Getting subdirectories recursively.", 4 );
        Vector< std::string > subdirs = Directory::ListDirectories( dir_name, true );
        COMMENT( "Getting files in each subdirectory.", 4 );
        for( size_t idx = 0; idx < subdirs.size( ); ++idx ) {
          Vector< std::string > sub_result = Directory::ListFiles( subdirs( idx ), extension, false );
          result.insert( result.begin( ), sub_result.begin( ), sub_result.end( ) );
        }
      }
      COMMENT( "Checking for directory accessibility.", 4 );
      std::string command = FileListCommand( dir_name, extension );
      COMMENT( "Command: " << command, 4 );
      FILE *files = Bial_popen( &command[ 0 ], "r" );
      if( !files ) {
        std::string msg( BIAL_ERROR( "Directory " + dir_name + " not accessible." ) );
        throw( std::ios_base::failure( msg ) );
      }
      COMMENT( "Getting files.", 4 );
      while( !std::feof( files ) ) {
        char buffer[ 128 ];
        if( fgets( buffer, 128, files ) != nullptr ) {
          buffer[ strlen( buffer ) - 1 ] = '\0'; /* remove '\n'. */
          result.push_back( std::string( buffer ) );
        }
      }
      Bial_pclose( files );
      COMMENT( "Files found: " << result, 4 );

      return( result );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while accessing directory." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  Vector< std::string > Directory::List( const std::string &dir_name, bool recursive ) {
    try {
      Vector< std::string > result = Directory::ListFiles( dir_name, recursive );
      Vector< std::string > subdirs = Directory::ListDirectories( dir_name, recursive );
      result.insert( result.begin( ), subdirs.begin( ), subdirs.end( ) );

      return( result );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while accessing directory." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  /* IFile ----------------------------------------------------------------------- */

  IFile::IFile( ) try : gz_file( ), std_file( ), gziped( false ) {
  }
  catch( std::ios_base::failure &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while accessing directory." ) );
    throw( std::ios_base::failure( msg ) );
  }
  catch( std::system_error &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
    throw( std::system_error( std::error_code( ), msg ) );
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

  IFile::IFile( const char *filename,
                       std::ios_base::openmode mode ) try : gz_file( ), std_file( ), gziped( false ) {
    std::string extension =
      File::ToLowerExtension( filename, static_cast< size_t >( std::max( 0, static_cast< int >( strlen( filename ) - 4 ) ) ) );
    if( extension.rfind( ".gz" ) != std::string::npos ) {

      COMMENT( "Check for random access in compressed file.", 2 );
      if( ( ( mode & std::ios::app ) == std::ios::app ) || ( ( mode & std::ios::ate ) == std::ios::ate ) ) {
        std::string msg( BIAL_ERROR( "No support for random access on gzip files." ) );
        throw( std::ios_base::failure( msg ) );
      }
      gz_file.open( filename, mode );
      gziped = true;
    }
    else {
      std_file.open( filename, mode );
    }
  }
  catch( std::ios_base::failure &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Could not open file." ) );
    throw( std::ios_base::failure( msg ) );
  }
  catch( std::system_error &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
    throw( std::system_error( std::error_code( ), msg ) );
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

  IFile::IFile( const std::string &filename,
                       std::ios_base::openmode mode ) try : gz_file( ), std_file( ), gziped( false ) {
    std::string extension =
      File::ToLowerExtension( filename, static_cast< size_t >( std::max( 0, static_cast< int >( filename.size( ) - 4 ) ) ) );
    if( extension.rfind( ".gz" ) != std::string::npos ) {

      COMMENT( "Check for random access in compressed file.", 2 );
      if( ( ( mode & std::ios::app ) == std::ios::app ) || ( ( mode & std::ios::ate ) == std::ios::ate ) ) {
        std::string msg( BIAL_ERROR( "No support for random access on gzip files." ) );
        throw( std::ios_base::failure( msg ) );
      }
      gz_file.open( filename.c_str( ), mode );
      gziped = true;
    }
    else {
      std_file.open( filename.c_str( ), mode );
    }
  }
  catch( std::ios_base::failure &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Could not open file." ) );
    throw( std::ios_base::failure( msg ) );
  }
  catch( std::system_error &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
    throw( std::system_error( std::error_code( ), msg ) );
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

  IFile::~IFile( ) {
    if( is_open( ) ) {
      close( );
    }
  }

  int IFile::peek( ) {
    try {
      if( gziped ) {
        return( gz_file.peek( ) );
      }
      else {
        return( std_file.peek( ) );
      }
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Could not access file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  void IFile::open( const char *filename, std::ios_base::openmode mode ) {
    try {
      open( std::string( filename ), mode );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Could not open file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  void IFile::open( const std::string &filename, std::ios_base::openmode mode ) {
    try {
      COMMENT( "Opening file: " << filename, 4 );
      if( is_open( ) ) {
        if( gziped ) {
          gz_file.close( );
        }
        else {
          std_file.close( );
        }
      }
      std::string extension =
        File::ToLowerExtension( filename,
                                static_cast< size_t >( std::max( 0, static_cast< int >( filename.size( ) ) - 4 ) ) );
      if( extension.rfind( ".gz" ) != std::string::npos ) {

        COMMENT( "Check for random access in compressed file.", 4 );
        if( ( ( mode & std::ios::app ) == std::ios::app ) || ( ( mode & std::ios::ate ) == std::ios::ate ) ) {
          std::string msg( BIAL_ERROR( "No support for random access on gzip files." ) );
          throw( std::ios_base::failure( msg ) );
        }
        gz_file.open( filename.c_str( ), mode );
        gziped = true;
      }
      else {
        std_file.open( filename.c_str( ), mode );
        gziped = false;
      }
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Could not open file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  bool IFile::is_open( ) const {
    try {
      if( gziped ) {
        return( gz_file.is_open( ) );
      }
      else {
        return( std_file.is_open( ) );
      }
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while accessing file flags." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  bool IFile::good( ) const {
    try {
      if( gziped ) {
        return( gz_file.good( ) );
      }
      else {
        return( std_file.good( ) );
      }
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while accessing file flags." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  bool IFile::eof( ) const {
    try {
      if( gziped ) {
        return( gz_file.eof( ) );
      }
      else {
        return( std_file.eof( ) );
      }
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while accessing file flags." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  bool IFile::fail( ) const {
    try {
      if( gziped ) {
        return( gz_file.fail( ) );
      }
      else {
        return( std_file.fail( ) );
      }
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while accessing file flags." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  bool IFile::bad( ) const {
    try {
      if( gziped ) {
        return( gz_file.bad( ) );
      }
      else {
        return( std_file.bad( ) );
      }
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while accessing file flags." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  void IFile::close( ) {
    try {
      if( gziped ) {
        gz_file.close( );
      }
      else {
        std_file.close( );
      }
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Could not close file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &IFile::operator>>( bool &val ) {
    try {
      if( gziped ) {
        gz_file >> val;
      }
      else {
        std_file >> val;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &IFile::operator>>( short &val ) {
    try {
      if( gziped ) {
        gz_file >> val;
      }
      else {
        std_file >> val;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &IFile::operator>>( unsigned short &val ) {
    try {
      if( gziped ) {
        gz_file >> val;
      }
      else {
        std_file >> val;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &IFile::operator>>( int &val ) {
    try {
      if( gziped ) {
        gz_file >> val;
      }
      else {
        std_file >> val;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &IFile::operator>>( unsigned int &val ) {
    try {
      if( gziped ) {
        gz_file >> val;
      }
      else {
        std_file >> val;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &IFile::operator>>( long &val ) {
    try {
      if( gziped ) {
        gz_file >> val;
      }
      else {
        std_file >> val;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &IFile::operator>>( unsigned long &val ) {
    try {
      if( gziped ) {
        gz_file >> val;
      }
      else {
        std_file >> val;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &IFile::operator>>( long long &val ) {
    try {
      if( gziped ) {
        gz_file >> val;
      }
      else {
        std_file >> val;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &IFile::operator>>( unsigned long long &val ) {
    try {
      if( gziped ) {
        gz_file >> val;
      }
      else {
        std_file >> val;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &IFile::operator>>( float &val ) {
    try {
      if( gziped ) {
        gz_file >> val;
      }
      else {
        std_file >> val;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &IFile::operator>>( double &val ) {
    try {
      if( gziped ) {
        gz_file >> val;
      }
      else {
        std_file >> val;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &IFile::operator>>( long double &val ) {
    try {
      if( gziped ) {
        gz_file >> val;
      }
      else {
        std_file >> val;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &IFile::operator>>( void* &val ) {
    try {
      if( gziped ) {
        gz_file >> val;
      }
      else {
        std_file >> val;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &IFile::operator>>( std::streambuf *sb ) {
    try {
      if( gziped ) {
        gz_file >> sb;
      }
      else {
        std_file >> sb;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &IFile::operator>>( std::istream & ( *pf )( std::istream & ) ) {
    try {
      if( gziped ) {
        gz_file >> pf;
      }
      else {
        std_file >> pf;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &IFile::operator>>( std::ios & ( *pf )( std::ios & ) ) {
    try {
      if( gziped ) {
        gz_file >> pf;
      }
      else {
        std_file >> pf;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &IFile::operator>>( std::ios_base & ( *pf )( std::ios_base & ) ) {
    try {
      if( gziped ) {
        gz_file >> pf;
      }
      else {
        std_file >> pf;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  std::streamsize IFile::gcount( ) {
    try {
      if( gziped ) {
        std::string msg( BIAL_ERROR( "No support for random access on gzip files." ) );
        throw( std::ios_base::failure( msg ) );
      }
      return( std_file.gcount( ) );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while accessing file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  int IFile::get( ) {
    try {
      if( gziped ) {
        return( gz_file.get( ) );
      }
      else {
        return( std_file.get( ) );
      }
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &IFile::get( char &c ) {
    try {
      if( gziped ) {
        gz_file.get( c );
      }
      else {
        std_file.get( c );
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &IFile::get( char *s, std::streamsize n ) {
    try {
      if( gziped ) {
        gz_file.get( s, n );
      }
      else {
        std_file.get( s, n );
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &IFile::get( char *s, std::streamsize n, char delim ) {
    try {
      if( gziped ) {
        gz_file.get( s, n, delim );
      }
      else {
        std_file.get( s, n, delim );
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &IFile::get( std::streambuf &sb ) {
    try {
      if( gziped ) {
        gz_file.get( sb );
      }
      else {
        std_file.get( sb );
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &IFile::get( std::streambuf &sb, char delim ) {
    try {
      if( gziped ) {
        gz_file.get( sb, delim );
      }
      else {
        std_file.get( sb, delim );
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &IFile::getline( char *s, std::streamsize n ) {
    try {
      if( gziped ) {
        gz_file.getline( s, n );
      }
      else {
        std_file.getline( s, n );
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &IFile::getline( char *s, std::streamsize n, char delim ) {
    try {
      if( gziped ) {
        gz_file.getline( s, n, delim );
      }
      else {
        std_file.getline( s, n, delim );
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &IFile::ignore( std::streamsize n, int delim ) {
    try {
      if( gziped ) {
        gz_file.ignore( n, delim );
      }
      else {
        std_file.ignore( n, delim );
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &IFile::read( char *s, std::streamsize n ) {
    try {
      if( gziped ) {
        gz_file.read( s, n );
      }
      else {
        std_file.read( s, n );
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &IFile::seekg( std::streampos pos ) {
    try {
      if( gziped ) {
        std::string msg( BIAL_ERROR( "No support for random access on gzip files." ) );
        throw( std::ios_base::failure( msg ) );
      }
      else {
        std_file.seekg( pos );
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &IFile::seekg( std::streamoff off, std::ios_base::seekdir way ) {
    try {
      if( gziped ) {
        std::string msg( BIAL_ERROR( "No support for random access on gzip files." ) );
        throw( std::ios_base::failure( msg ) );
      }
      else {
        std_file.seekg( off, way );
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  std::ios_base::iostate IFile::exceptions( ) const {
    try {
      if( gziped ) {
        return( gz_file.exceptions( ) );
      }
      else {
        return( std_file.exceptions( ) );
      }
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while accessing file flags." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  void IFile::exceptions( std::ios_base::iostate except ) {
    try {
      if( gziped ) {
        gz_file.exceptions( except );
      }
      else {
        std_file.exceptions( except );
      }
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while accessing file flags." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  bool IFile::IsGziped( ) {
    return( gziped );
  }

  /* IFile non-member functions. */

  IFile &getline( IFile &file, std::string &str, char delim ) {
    try {
      if( file.gziped ) {
        getline( file.gz_file, str, delim );
      }
      else {
        getline( file.std_file, str, delim );
      }
      return( file );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &getline( IFile && file, std::string &str, char delim ) {
    try {
      if( file.gziped ) {
        getline( file.gz_file, str, delim );
      }
      else {
        getline( file.std_file, str, delim );
      }
      return( file );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &getline( IFile &file, std::string &str ) {
    try {
      if( file.gziped ) {
        getline( file.gz_file, str );
      }
      else {
        getline( file.std_file, str );
      }
      return( file );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &getline( IFile && file, std::string &str ) {
    try {
      if( file.gziped ) {
        getline( file.gz_file, str );
      }
      else {
        getline( file.std_file, str );
      }
      return( file );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &operator>>( IFile &file, char &c ) {
    try {
      if( file.gziped ) {
        file.gz_file >> c;
      }
      else {
        file.std_file >> c;
      }
      return( file );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &operator>>( IFile &file, signed char &c ) {
    try {
      if( file.gziped ) {
        file.gz_file >> c;
      }
      else {
        file.std_file >> c;
      }
      return( file );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &operator>>( IFile &file, unsigned char &c ) {
    try {
      if( file.gziped ) {
        file.gz_file >> c;
      }
      else {
        file.std_file >> c;
      }
      return( file );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &operator>>( IFile &file, char *s ) {
    try {
      if( file.gziped ) {
        file.gz_file >> s;
      }
      else {
        file.std_file >> s;
      }
      return( file );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &operator>>( IFile &file, signed char *s ) {
    try {
      if( file.gziped ) {
        file.gz_file >> s;
      }
      else {
        file.std_file >> s;
      }
      return( file );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  IFile &operator>>( IFile &file, unsigned char *s ) {
    try {
      if( file.gziped ) {
        file.gz_file >> s;
      }
      else {
        file.std_file >> s;
      }
      return( file );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  /* OFile -------------------------------------------------------------------------------- */

  OFile::OFile( ) try : gz_file( ), std_file( ), gziped( false ) {
  }
  catch( std::ios_base::failure &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while accessing file." ) );
    throw( std::ios_base::failure( msg ) );
  }
  catch( std::system_error &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
    throw( std::system_error( std::error_code( ), msg ) );
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


  OFile::OFile( const char *filename,
                       std::ios_base::openmode mode ) : gz_file( ), std_file( ), gziped( false ) {
    try {
      std::string extension =
        File::ToLowerExtension( filename,
                                static_cast< size_t >( std::max( 0, static_cast< int >( strlen( filename ) - 4 ) ) ) );
      if( extension.rfind( ".gz" ) != std::string::npos ) {

        COMMENT( "Check for random access in compressed file.", 2 );
        if( ( ( mode & std::ios::app ) == std::ios::app ) || ( ( mode & std::ios::ate ) == std::ios::ate ) ) {
          std::string msg( BIAL_ERROR( "No support for random access on gzip files." ) );
          throw( std::ios_base::failure( msg ) );
        }
        gz_file.open( filename, mode );
        gziped = true;
      }
      else {
        std_file.open( filename, mode );
      }
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Could not open specified file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  OFile::OFile( const std::string &filename,
                       std::ios_base::openmode mode ) : gz_file( ), std_file( ), gziped( false ) {
    try {
      std::string extension =
        File::ToLowerExtension( filename, static_cast< size_t >( std::max( 0, static_cast< int >( filename.size( ) - 3 ) ) ) );
      if( extension.rfind( ".gz" ) != std::string::npos ) {

        COMMENT( "Check for random access in compressed file.", 2 );
        if( ( ( mode & std::ios::app ) == std::ios::app ) || ( ( mode & std::ios::ate ) == std::ios::ate ) ) {
          std::string msg( BIAL_ERROR( "No support for random access on gzip files." ) );
          throw( std::ios_base::failure( msg ) );
        }
        gz_file.open( filename.c_str( ), mode );
        gziped = true;
      }
      else {
        std_file.open( filename.c_str( ), mode );
      }
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Could not open specified file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  OFile::~OFile( ) {
    if( is_open( ) ) {
      close( );
    }
  }

  void OFile::open( const char *filename, std::ios_base::openmode mode ) {
    try {
      open( std::string( filename ), mode );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Could not open specified file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  void OFile::open( const std::string &filename, std::ios_base::openmode mode ) {
    try {
      std::string extension =
        File::ToLowerExtension( filename,
                                static_cast< size_t >( std::max( 0, static_cast< int >( filename.size( ) ) - 4 ) ) );
      if( extension.rfind( ".gz" ) != std::string::npos ) {

        COMMENT( "Check for random access in compressed file.", 2 );
        if( ( ( mode & std::ios::app ) == std::ios::app ) || ( ( mode & std::ios::ate ) == std::ios::ate ) ) {
          std::string msg( std::string( "\n" ) + BIAL_ERROR( "No support for random access on gzip files." ) );
          throw( std::ios_base::failure( msg ) );
        }
        gz_file.open( filename.c_str( ), mode );
        gziped = true;
      }
      else {
        std_file.open( filename.c_str( ), mode );
        gziped = false;
      }
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Could not open specified file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  bool OFile::is_open( ) const {
    try {
      if( gziped ) {
        return( gz_file.is_open( ) );
      }
      else {
        return( std_file.is_open( ) );
      }
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while accessing file flags." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  bool OFile::eof( ) const {
    try {
      if( gziped ) {
        return( gz_file.eof( ) );
      }
      else {
        return( std_file.eof( ) );
      }
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while accessing file flags." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  void OFile::close( ) {
    try {
      if( gziped ) {
        gz_file.close( );
      }
      else {
        std_file.close( );
      }
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Could not close file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  OFile &OFile::operator<<( bool val ) {
    try {
      if( gziped ) {
        gz_file << val;
      }
      else {
        std_file << val;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while writing to file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  OFile &OFile::operator<<( short val ) {
    try {
      if( gziped ) {
        gz_file << val;
      }
      else {
        std_file << val;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while writing to file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  OFile &OFile::operator<<( unsigned short val ) {
    try {
      if( gziped ) {
        gz_file << val;
      }
      else {
        std_file << val;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while writing to file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  OFile &OFile::operator<<( int val ) {
    try {
      if( gziped ) {
        gz_file << val;
      }
      else {
        std_file << val;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while writing to file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  OFile &OFile::operator<<( unsigned int val ) {
    try {
      if( gziped ) {
        gz_file << val;
      }
      else {
        std_file << val;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while writing to file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  OFile &OFile::operator<<( long val ) {
    try {
      if( gziped ) {
        gz_file << val;
      }
      else {
        std_file << val;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while writing to file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  OFile &OFile::operator<<( unsigned long val ) {
    try {
      if( gziped ) {
        gz_file << val;
      }
      else {
        std_file << val;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while writing to file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  OFile &OFile::operator<<( long long val ) {
    try {
      if( gziped ) {
        gz_file << val;
      }
      else {
        std_file << val;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while writing to file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  OFile &OFile::operator<<( unsigned long long val ) {
    try {
      if( gziped ) {
        gz_file << val;
      }
      else {
        std_file << val;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while writing to file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  OFile &OFile::operator<<( float val ) {
    try {
      if( gziped ) {
        gz_file << val;
      }
      else {
        std_file << val;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while writing to file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  OFile &OFile::operator<<( double val ) {
    try {
      if( gziped ) {
        gz_file << val;
      }
      else {
        std_file << val;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while writing to file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  OFile &OFile::operator<<( long double val ) {
    try {
      if( gziped ) {
        gz_file << val;
      }
      else {
        std_file << val;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while writing to file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  OFile &OFile::operator<<( void *val ) {
    try {
      if( gziped ) {
        gz_file << val;
      }
      else {
        std_file << val;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while writing to file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  OFile &OFile::operator<<( std::streambuf *sb ) {
    try {
      if( gziped ) {
        gz_file << sb;
      }
      else {
        std_file << sb;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while writing to file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  OFile &OFile::operator<<( OFile & ( *pf )( OFile & ) ) {
    try {
      if( gziped ) {
        gz_file << pf;
      }
      else {
        std_file << pf;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while writing to file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  OFile &OFile::operator<<( std::ostream & ( *pf )( std::ostream & ) ) {
    try {
      if( gziped ) {
        gz_file << pf;
      }
      else {
        std_file << pf;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while writing to file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  OFile &OFile::operator<<( std::ios & ( *pf )( std::ios & ) ) {
    try {
      if( gziped ) {
        gz_file << pf;
      }
      else {
        std_file << pf;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while writing to file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  OFile &OFile::operator<<( std::ios_base & ( *pf )( std::ios_base & ) ) {
    try {
      if( gziped ) {
        gz_file << pf;
      }
      else {
        std_file << pf;
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while writing to file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  OFile &OFile::put( char c ) {
    try {
      if( gziped ) {
        gz_file.put( c );
      }
      else {
        std_file.put( c );
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while writing to file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  OFile &OFile::write( const char *s, std::streamsize n ) {
    try {
      if( gziped ) {
        gz_file.write( s, n );
      }
      else {
        std_file.write( s, n );
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while writing to file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  std::streampos OFile::tellp( ) {
    try {
      if( gziped ) {
        std::string msg( BIAL_ERROR( "No support for random access on gzip files." ) );
        throw( std::ios_base::failure( msg ) );
      }
      else {
        return( std_file.tellp( ) );
      }
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading file's position." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  OFile &OFile::seekp( std::streampos pos ) {
    try {
      if( gziped ) {
        std::string msg( BIAL_ERROR( "No support for random access on gzip files." ) );
        throw( std::ios_base::failure( msg ) );
      }
      else {
        std_file.seekp( pos );
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  OFile &OFile::seekp( std::streamoff off, std::ios_base::seekdir way ) {
    try {
      if( gziped ) {
        std::string msg( BIAL_ERROR( "No support for random access on gzip files." ) );
        throw( std::ios_base::failure( msg ) );
      }
      else {
        std_file.seekp( off, way );
      }
      return( *this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while reading from file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  std::ios_base::iostate OFile::exceptions( ) const {
    try {
      if( gziped ) {
        return( gz_file.exceptions( ) );
      }
      else {
        return( std_file.exceptions( ) );
      }
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while accessing file flags." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  void OFile::exceptions( std::ios_base::iostate except ) {
    try {
      if( gziped ) {
        gz_file.exceptions( except );
      }
      else {
        std_file.exceptions( except );
      }
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while accessing file flags." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  bool OFile::IsGziped( ) {
    return( gziped );
  }

  /* OFile non-member functions. */

  OFile &operator<<( OFile &file, char c ) {
    try {
      if( file.gziped ) {
        file.gz_file << c;
      }
      else {
        file.std_file << c;
      }
      return( file );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while writing to file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  OFile &operator<<( OFile &file, signed char c ) {
    try {
      if( file.gziped ) {
        file.gz_file << c;
      }
      else {
        file.std_file << c;
      }
      return( file );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while writing to file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  OFile &operator<<( OFile &file, unsigned char c ) {
    try {
      if( file.gziped ) {
        file.gz_file << c;
      }
      else {
        file.std_file << c;
      }
      return( file );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while writing to file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  OFile &operator<<( OFile &file, const char *s ) {
    try {
      if( file.gziped ) {
        file.gz_file << s;
      }
      else {
        file.std_file << s;
      }
      return( file );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while writing to file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  OFile &operator<<( OFile &file, const signed char *s ) {
    try {
      if( file.gziped ) {
        file.gz_file << s;
      }
      else {
        file.std_file << s;
      }
      return( file );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while writing to file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  OFile &operator<<( OFile &file, const unsigned char *s ) {
    try {
      if( file.gziped ) {
        file.gz_file << s;
      }
      else {
        file.std_file << s;
      }
      return( file );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error while writing to file." ) );
      throw( std::ios_base::failure( msg ) );
    }
    catch( std::system_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "System error, while accessing I/O." ) );
      throw( std::system_error( std::error_code( ), msg ) );
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

  /* File ---------------------------------------------------------------------------------- */

  std::string File::Extension( const std::string &filename, size_t pos ) {
    try {
      size_t found = filename.find( '.', std::max( static_cast< size_t >( 1 ), pos ) );
      if( found != std::string::npos ) {
        return( filename.substr( found ) );
      }
      return( "" );
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

  std::string File::ToLowerExtension( const std::string &filename, size_t pos ) {
    try {
      std::string extension( Extension( filename, pos ) );
      transform( extension.begin( ), extension.end( ), extension.begin( ), ::tolower );
      return( extension );
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

  std::string File::Directory( const std::string &filename, size_t pos ) {
    try {
      size_t found = filename.rfind( DIR_SEPARATOR, std::max( static_cast< size_t >( 0 ), pos ) );
      if( found != std::string::npos ) {
        return( filename.substr( 0, found + 1 ) );
      }
      return( "" );
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

  std::string File::Basename( const std::string &filename, bool keep_directory ) {
    try {
      size_t ext_pos = filename.find( '.', 1 );
      if( ext_pos == std::string::npos ) {
        ext_pos = filename.size( );
      }
      if( keep_directory ) {
        return( filename.substr( 0, ext_pos ) );
      }
      size_t dir_pos = filename.rfind( DIR_SEPARATOR, std::string::npos );
      if( dir_pos == std::string::npos ) {
        return( filename.substr( 0, ext_pos ) );
      }
      return( filename.substr( dir_pos + 1, ext_pos ) );
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

  std::string File::LocateCaseSensitiveFileName( const std::string &filename ) {
    try {
      std::string base = File::Basename( filename, true );
      std::string extension = File::Extension( filename );
      size_t ext_position = filename.find( extension ) + 1;
      std::string testname( base + File::ToLowerExtension( filename ) );
      size_t possibilities = pow( 2, extension.size( ) - 1 );
      for( size_t cnt = 0; cnt < possibilities; ++cnt ) {
        std::ifstream file( testname );
        if( file.good( ) ) {
          file.close( );
          return( testname );
        }
        COMMENT( "testing all string possibilities like a binary odometer: 0 for lowercase and 1 for uppercase.", 4 );
        for( size_t pos = ext_position; pos < filename.size( ); ++pos ) {
          if( islower( testname[ pos ] ) ) {
            testname[ pos ] = std::toupper( testname[ pos ] );
            break;
          }
          else {
            testname[ pos ] = std::tolower( testname[ pos ] );
          }
        }
      }
      return( "" );
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

#ifdef BIAL_EXPLICIT_File

#endif

}

#endif

#endif
