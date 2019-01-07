/*
 * ============================================================================
 * gzstream, C++ iostream classes wrapping the zlib compression library.
 * Copyright (C) 2001  Deepak Bandyopadhyay, Lutz Kettner
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * ============================================================================
 *
 * File          : gzstream.hpp
 * Revision      : $Revision: 1.5 $
 * Revision_date : $Date: 2002/04/26 23:30:15 $
 * Author(s)     : Deepak Bandyopadhyay, Lutz Kettner
 *
 * Standard std::streambuf implementation following Nicolai Josuttis, "The
 * Standard C++ Library".
 * ============================================================================
 */

#ifndef GZSTREAM_H
#define GZSTREAM_H

#include "Common.hpp"
#include <fstream>
#include <iostream>
#include <zlib.h>

namespace Bial {

  class gzstreambuf : public std::streambuf {
private:
    static const int bufferSize = 47 + 256; /* size of data buff */
    /* totals 512 bytes under g++ for igzstream at the end. */
    gzFile file; /* file handle for compressed file */
    char buffer[ bufferSize ]; /* data buffer */
    char opened; /* open/close state of stream */
    int mode; /* I/O mode */
    int flush_buffer( );
public:
    gzstreambuf( ) try : opened( 0 ) {
      setp( buffer, buffer + ( bufferSize - 1 ) );
      setg( buffer + 4, /* beginning of putback area */
            buffer + 4, /* read position */
            buffer + 4 ); /* end position */
      /* ASSERT: both input & output capabilities will not be used together */
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading file." ) );
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
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR(
                         "Image, window end, and/or window size dimensions do not match." ) );
      throw( std::logic_error( msg ) );
    }

    bool is_open( ) const {
      return( opened );
    }
    gzstreambuf* open( const char *name, int open_mode );
    gzstreambuf* close( );
    ~gzstreambuf( ) {
      close( );
    }

    virtual int overflow( int c = EOF );
    virtual int underflow( );
    virtual int sync( );
  };

  class gzstreambase : virtual public std::ios {
protected:
    gzstreambuf buf;
public:
    gzstreambase( ) {
      init( &buf );
    }
    gzstreambase( const char *name, int open_mode );
    ~gzstreambase( );
    void open( const char *name, int open_mode );
    void close( );
    gzstreambuf* rdbuf( ) {
      return( &buf );
    }
  };

  /*
   * ----------------------------------------------------------------------------
   * User classes. Use igzstream and ogzstream analogously to ifstream and
   * ofstream respectively. They read and write files based on the gz*
   * function interface of the zlib. Files are compatible with gzip compression.
   * ----------------------------------------------------------------------------
   */

  class igzstream : public gzstreambase, public std::istream {
public:
    igzstream( ) try : std::istream( &buf ) {
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading file." ) );
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
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR(
                         "Image, window end, and/or window size dimensions do not match." ) );
      throw( std::logic_error( msg ) );
    }

    igzstream( igzstream && ) try : std::istream( &buf ) {
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading file." ) );
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
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR(
                         "Image, window end, and/or window size dimensions do not match." ) );
      throw( std::logic_error( msg ) );
    }

    igzstream( const char *name, int open_mode = std::ios::in ) try : gzstreambase( name,
                                                                                    open_mode ), std::istream( &buf ) {
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading file." ) );
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
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR(
                         "Image, window end, and/or window size dimensions do not match." ) );
      throw( std::logic_error( msg ) );
    }

    gzstreambuf* rdbuf( ) {
      try {
        return( gzstreambase::rdbuf( ) );
      }
      catch( std::ios_base::failure &e ) {
        std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading file." ) );
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

    void open( const char *name, int open_mode = std::ios::in ) {
      try {
        gzstreambase::open( name, open_mode );
      }
      catch( std::ios_base::failure &e ) {
        std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading file." ) );
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

    bool is_open( ) const {
      try {
        return( gzstreambase::buf.is_open( ) );
      }
      catch( std::ios_base::failure &e ) {
        std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading file." ) );
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

  };

  class ogzstream : public gzstreambase, public std::ostream {
public:
    ogzstream( ) try : std::ostream( &buf ) {
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading file." ) );
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
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR(
                         "Image, window end, and/or window size dimensions do not match." ) );
      throw( std::logic_error( msg ) );
    }

    ogzstream( ogzstream && ) try {
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading file." ) );
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
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR(
                         "Image, window end, and/or window size dimensions do not match." ) );
      throw( std::logic_error( msg ) );
    }

    ogzstream( const char *name, int mode = std::ios::out ) try : gzstreambase( name, mode ), std::ostream( &buf ) {
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading file." ) );
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
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR(
                         "Image, window end, and/or window size dimensions do not match." ) );
      throw( std::logic_error( msg ) );
    }

    gzstreambuf* rdbuf( ) {
      try {
        return( gzstreambase::rdbuf( ) );
      }
      catch( std::ios_base::failure &e ) {
        std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading file." ) );
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

    void open( const char *name, int open_mode = std::ios::out ) {
      try {
        gzstreambase::open( name, open_mode );
      }
      catch( std::ios_base::failure &e ) {
        std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading file." ) );
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

    bool is_open( ) const {
      try {
        return( gzstreambase::buf.is_open( ) );
      }
      catch( std::ios_base::failure &e ) {
        std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading file." ) );
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
  };

}

/* Implementation ----------------------------------------------------------------------------------- */

#include <iostream>
#include <string.h> /* for memcpy */

namespace Bial {

  /* gzstreambuf -------------------------------------------------------------------------------------- */

  inline gzstreambuf* gzstreambuf::open( const char *name, int open_mode ) {
    try {
      if( is_open( ) ) {
        return( ( gzstreambuf* ) 0 );
      }
      mode = open_mode;
      /* no append nor read/write mode */
      if( ( mode & std::ios::ate ) || ( mode & std::ios::app ) ||
          ( ( mode & std::ios::in ) && ( mode & std::ios::out ) ) ) {
        return( ( gzstreambuf* ) 0 );
      }
      char fmode[ 10 ];
      char *fmodeptr = fmode;
      if( mode & std::ios::in ) {
        *fmodeptr++ = 'r';
      }
      else if( mode & std::ios::out ) {
        *fmodeptr++ = 'w';
      }
      *fmodeptr++ = 'b';
      *fmodeptr = '\0';
      file = gzopen( name, fmode );
      if( file == 0 ) {
        return( ( gzstreambuf* ) 0 );
      }
      opened = 1;
      return( this );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading file." ) );
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

  inline gzstreambuf* gzstreambuf::close( ) {
    try {
      if( is_open( ) ) {
        sync( );
        opened = 0;
        if( gzclose( file ) == Z_OK ) {
          return( this );
        }
      }
      return( ( gzstreambuf* ) 0 );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading file." ) );
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

  inline int gzstreambuf::underflow( ) { /* used for input buffer only */
    try {
      if( gptr( ) && ( gptr( ) < egptr( ) ) ) {
        return( *reinterpret_cast< unsigned char* >( gptr( ) ) );
      }
      if( !( mode & std::ios::in ) || !opened ) {
        return( EOF );
      }
      /* Josuttis' implementation of inbuf */
      int n_putback = gptr( ) - eback( );
      if( n_putback > 4 ) {
        n_putback = 4;
      }
      memcpy( buffer + ( 4 - n_putback ), gptr( ) - n_putback, n_putback );

      int num = gzread( file, buffer + 4, bufferSize - 4 );
      if( num <= 0 ) { /* ERROR or EOF */
        return( EOF );
      }
      /* reset buffer pointers */
      setg( buffer + ( 4 - n_putback ), /* beginning of putback area */
            buffer + 4, /* read position */
            buffer + 4 + num ); /* end of buffer */

      /* return next character */
      return( *reinterpret_cast< unsigned char* >( gptr( ) ) );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading file." ) );
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

  inline int gzstreambuf::flush_buffer( ) {
    try {
      /*
       * Separate the writing of the buffer from overflow() and
       * sync( ) operation.
       */
      int w = std::streambuf::pptr( ) - std::streambuf::pbase( );
      if( gzwrite( file, std::streambuf::pbase( ), w ) != w ) {
        return( EOF );
      }
      pbump( -w );
      return( w );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading file." ) );
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

  inline int gzstreambuf::overflow( int c ) { /* used for output buffer only */
    try {
      if( !( mode & std::ios::out ) || !opened ) {
        return( EOF );
      }
      if( c != EOF ) {
        *std::streambuf::pptr( ) = c;
        pbump( 1 );
      }
      if( flush_buffer( ) == EOF ) {
        return( EOF );
      }
      return( c );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading file." ) );
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

  inline int gzstreambuf::sync( ) {
    try {
      if( std::streambuf::pptr( ) && ( std::streambuf::pptr( ) > std::streambuf::pbase( ) ) ) {
        if( flush_buffer( ) == EOF ) {
          return( -1 );
        }
      }
      return( 0 );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading file." ) );
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

  /* gzstreambase ------------------------------------------------------------------------------------ */

  inline gzstreambase::gzstreambase( const char *name, int mode ) {
    try {
      init( &buf );
      open( name, mode );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading file." ) );
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

  inline gzstreambase::~gzstreambase( ) {
    buf.close( );
  }

  inline void gzstreambase::open( const char *name, int open_mode ) {
    try {
      if( !buf.open( name, open_mode ) ) {
        clear( rdstate( ) | std::ios::badbit );
      }
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading file." ) );
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

  inline void gzstreambase::close( ) {
    try {
      if( buf.is_open( ) ) {
        if( !buf.close( ) ) {
          clear( rdstate( ) | std::ios::badbit );
        }
      }
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading file." ) );
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

}

#endif
