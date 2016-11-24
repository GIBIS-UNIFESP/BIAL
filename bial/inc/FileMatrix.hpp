/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Aug/08 
 * @brief Reading and writing matrices.
 */

#include "Common.hpp"

#ifndef BIALFILEMATRIX_H
#define BIALFILEMATRIX_H

namespace Bial {

  template< class D >
  class Matrix;

  /**
   * @date 2015/Jun/09
   * @param filename: Name of the file conatining matrix.
   * @return The read matrix.
   * @brief Reads matrix from file.
   * @warning none.
   */
  template< class D >
  static Matrix< D > ReadMatrix( const std::string &filename );

  /**
   * @date 2015/Jun/09
   * @param mtx: Input matrix.
   * @param filename: Name of the file conatining matrix.
   * @return none.
   * @brief Writes matrix into file.
   * @warning none.
   */
  template< class D >
  static void Write( const Matrix< D > &mtx, const std::string &filename );

  /**
   * @date 2015/Jun/09
   * @param mtx: Input matrix.
   * @param filename: Name of the file conatining matrix.
   * @return none.
   * @brief Writes matrix into file.
   * @warning none.
   */
  template< class D >
  static void WriteMatrix( const Matrix< D > &mtx, const std::string &filename );

}

/* Implementation --------------------------------------------------------------------------------------------------- */

#include "File.hpp"

namespace Bial {

  template< class D >
  Matrix< D > ReadMatrix( const std::string &filename ) {
    try {
      COMMENT( "Opening Bial matrix file.", 2 );
      IFile file;
      file.exceptions( std::fstream::failbit | std::fstream::badbit );
      file.open( filename );
      COMMENT( "Checking magic number.", 2 );
      int mgc_nbr;
      file.read( reinterpret_cast< char* >( &mgc_nbr ), sizeof( int ) );
      if( mgc_nbr == 5023 ) {
        COMMENT( "Reading dimension size.", 2 );
        Vector< int > size( 2 );
        file.read( reinterpret_cast< char* >( &size[ 0 ] ), sizeof( int ) );
        file.read( reinterpret_cast< char* >( &size[ 1 ] ), sizeof( int ) );
        COMMENT( "Read size: " << size, 2 );
        COMMENT( "Creating matrix.", 2 );
        Matrix< D > res( size );
        COMMENT( "Getting data type.", 2 );
        int type;
        file.read( reinterpret_cast< char* >( &type ), sizeof( int ) );
        COMMENT( "Read type: " << static_cast< int >( type ), 2 );
        COMMENT( "Getting number of elements.", 2 );
        size_t mat_size = res.size( );
        if( type == 8 ) {
          COMMENT( "Reading 8 bit integer data.", 2 );
          unsigned char *data8;
          data8 = new unsigned char[ mat_size ];
          file.read( reinterpret_cast< char* >( data8 ), sizeof( unsigned char ) * mat_size );
          for( size_t elm = 0; elm < mat_size; ++elm ) {
            res( elm ) = static_cast< D >( data8[ elm ] );
          }
          delete[] data8;
        }
        else if( type == 16 ) {
          COMMENT( "Reading 16 bit integer data.", 2 );
          unsigned short *data16;
          data16 = new unsigned short[ mat_size ];
          file.read( reinterpret_cast< char* >( data16 ), sizeof( unsigned short ) * mat_size );
          for( size_t elm = 0; elm < mat_size; ++elm ) {
            res( elm ) = static_cast< D >( data16[ elm ] );
          }
          delete[] data16;
        }
        else if( type == 32 ) {
          COMMENT( "Reading 32 bit integer data.", 2 );
          int *data32;
          data32 = new int[ mat_size ];
          file.read( reinterpret_cast< char* >( data32 ), sizeof( int ) * mat_size );
          for( size_t elm = 0; elm < mat_size; ++elm ) {
            res( elm ) = static_cast< D >( data32[ elm ] );
          }
          delete[] data32;
        }
        else if( type == 64 ) {
          COMMENT( "Reading 64 bit integer data.", 2 );
          long long *data64;
          data64 = new long long[ mat_size ];
          file.read( reinterpret_cast< char* >( data64 ), sizeof( long long ) * mat_size );
          for( size_t elm = 0; elm < mat_size; ++elm ) {
            res( elm ) = static_cast< D >( data64[ elm ] );
          }
          delete[] data64;
        }
        else if( type == -32 ) {
          COMMENT( "Reading 32 bit float data.", 2 );
          float *fdata32;
          fdata32 = new float[ mat_size ];
          file.read( reinterpret_cast< char* >( fdata32 ), sizeof( float ) * mat_size );
          for( size_t elm = 0; elm < mat_size; ++elm ) {
            res( elm ) = static_cast< D >( fdata32[ elm ] );
          }
          delete[] fdata32;
        }
        else { /* if( type == -64 ) */
          COMMENT( "Reading 64 bit float data.", 2 );
          double *fdata64;
          fdata64 = new double[ mat_size ];
          file.read( reinterpret_cast< char* >( fdata64 ), sizeof( double ) * mat_size );
          for( size_t elm = 0; elm < mat_size; ++elm ) {
            res( elm ) = static_cast< D >( fdata64[ elm ] );
          }
          delete[] fdata64;
        }
        file.close( );
        return( res );
      }
      COMMENT( "Not bial 2D matrix type.", 2 );
      std::string msg( BIAL_ERROR( "Not bial 2D matrix type. Wrong magic string." ) );
      throw( std::logic_error( msg ) );
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

  template< class D >
  void WriteMatrix( const Matrix< D > &mtx, const std::string &filename ) {
    Write< D >( mtx, filename );
  }

  template< class D >
  void Write( const Matrix< D > &mtx, const std::string &filename ) {
    try {
      COMMENT( "Insert here matrix type control.", 2 );
      if( mtx.Dims( ) != 2 ) {
        std::string msg( BIAL_ERROR( "Invalid number of dimensions. Expected 2." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Opening file.", 2 );
      OFile file;
      file.exceptions( std::fstream::failbit | std::fstream::badbit );
      file.open( filename );
      COMMENT( "Writing magic number and matrix dimensions.", 2 );
      int mgc_nbr = 5023;
      Vector< int > size( mtx.Dim( ) );
      file.write( reinterpret_cast< const char* >( &mgc_nbr ), sizeof( int ) );
      file.write( reinterpret_cast< const char* >( &size[ 0 ] ), sizeof( int ) );
      file.write( reinterpret_cast< const char* >( &size[ 1 ] ), sizeof( int ) );
      COMMENT( "Getting maximum intensity.", 2 );
      D Imax = mtx.Maximum( );
      COMMENT( "Writing data.", 2 );
      if( static_cast< D >( 1.1f ) != static_cast< D >( 1.0f ) ) {
        COMMENT( "floating point data.", 2 );
        if( sizeof( D ) == 4 ) {
          COMMENT( "32 bit float point matrix.", 2 );
          int code = -32;
          file.write( reinterpret_cast< char* >( &code ), sizeof( int ) );
          file.write( reinterpret_cast< const char* >( &mtx( 0 ) ), sizeof( float ) * mtx.size( ) );
        }
        else { /* double data -> 64 bit. */
          COMMENT( "64 bit float point matrix.", 2 );
          int code = -64;
          file.write( reinterpret_cast< char* >( &code ), sizeof( int ) );
          file.write( reinterpret_cast< const char* >( &mtx( 0 ) ), sizeof( double ) * mtx.size( ) );
        }
      }
      else if( Imax < 256 ) {
        COMMENT( "8 bit integer point matrix.", 2 );
        int code = 8;
        file.write( reinterpret_cast< char* >( &code ), sizeof( int ) );
        Vector< unsigned char > data8( mtx.size( ) );
        for( size_t elm = 0; elm < mtx.size( ); ++elm ) {
          data8[ elm ] = static_cast< unsigned char >( mtx( elm ) );
        }
        file.write( reinterpret_cast< const char* >( &data8[ 0 ] ), sizeof( unsigned char ) * mtx.size( ) );
      }
      else if( Imax < 65536 ) {
        COMMENT( "16 bit integer point matrix.", 2 );
        int code = 16;
        file.write( reinterpret_cast< char* >( &code ), sizeof( int ) );
        Vector< unsigned short > data16( mtx.size( ) );
        for( size_t elm = 0; elm < mtx.size( ); ++elm ) {
          data16[ elm ] = static_cast< unsigned short >( mtx( elm ) );
        }
        file.write( reinterpret_cast< const char* >( &data16[ 0 ] ), sizeof( unsigned short ) * mtx.size( ) );
      }
      else if( Imax < 2147483648 ) {
        COMMENT( "32 bit integer point matrix.", 2 );
        int code = 32;
        file.write( reinterpret_cast< char* >( &code ), sizeof( int ) );
        Vector< int > data32( mtx.size( ) );
        for( size_t elm = 0; elm < mtx.size( ); ++elm ) {
          data32[ elm ] = static_cast< int >( mtx( elm ) );
        }
        file.write( reinterpret_cast< const char* >( &data32[ 0 ] ), sizeof( int ) * mtx.size( ) );
      }
      else { /* 64 bit integer. */
        COMMENT( "64 bit integer point matrix.", 2 );
        int code = 64;
        file.write( reinterpret_cast< char* >( &code ), sizeof( int ) );
        file.write( reinterpret_cast< const char* >( &mtx( 0 ) ), sizeof( int ) * mtx.size( ) );
      }
      file.close( );
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

}

#endif
