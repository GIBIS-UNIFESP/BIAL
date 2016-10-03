/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Nov/19 
 * @brief Kernel iterator class.
 */

#ifndef BIALKERNELITERATOR_C
#define BIALKERNELITERATOR_C

#include "KernelIterator.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_KernelIterator )
#define BIAL_EXPLICIT_KernelIterator
#endif
#if defined ( BIAL_EXPLICIT_KernelIterator ) || ( BIAL_IMPLICIT_BIN )

#include "Image.hpp"
#include "Matrix.hpp"

namespace Bial {

  KernelIterator::KernelIterator( const Kernel &krn, const Vector< size_t > &dim, size_t index ) try : 
    kernel( krn ), data_size( dim ), data_limit( 0 ), pixel_index( index ), krn_index( index ), position( 0 ) {
      if( dim.size( ) == 0 ) {
        std::string msg( BIAL_ERROR( "Empty space dimention vector." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Defining data_limit to return invalid reference code.", 2 );
      data_limit = dim( 0 );
      for( size_t idx = 0; idx < dim.size( ); ++idx ) {
        data_limit *= dim( idx );
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

  template< class D > KernelIterator::KernelIterator( const Kernel &krn, const Matrix< D > &mat, size_t index ) try : 
    kernel( krn ), data_size( mat.Dim( ) ), data_limit( mat.size( ) ), pixel_index( index ), krn_index( index ), 
      position( 0 ) {
      if( mat.size( ) == 0 ) {
        std::string msg( BIAL_ERROR( "Empty matrix." ) );
        throw( std::logic_error( msg ) );
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

  template< class D > KernelIterator::KernelIterator( const Kernel &krn, const Image< D > &img, size_t index ) try :
    kernel( krn ), data_size( img.Dim( ) ), data_limit( img.size( ) ), pixel_index( index ), krn_index( index ),
      position( 0 ) {
      if( img.Dims( ) == 2 )
        data_size.pop_back( );
      if( img.size( ) == 0 ) {
        std::string msg( BIAL_ERROR( "Empty matrix." ) );
        throw( std::logic_error( msg ) );
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

  void KernelIterator::begin( ) {
    try {
      krn_index = kernel( data_size, pixel_index, position );
      while( ( position < kernel.size( ) ) && ( krn_index == data_limit ) ) {
        ++position;
        if( position >= kernel.size( ) ) {
          krn_index = data_limit;
          break;
        }
        krn_index = kernel( data_size, pixel_index, position );
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

  void KernelIterator::end( ) {
    try {
      position = kernel.size( );
      krn_index = data_limit;
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

  KernelIterator &KernelIterator::operator++( ) {
    try {
      do {
        ++position;
        if( position >= kernel.size( ) ) {
          krn_index = data_limit;
          break;
        }
        krn_index = kernel( data_size, pixel_index, position ); // Here
      } while( ( position < kernel.size( ) ) && ( krn_index == data_limit ) );
      return( *this );
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
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception. Kernel size: " +
                                                                     std::to_string( kernel.size( ) ) ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  KernelIterator KernelIterator::operator++( int ) {
    try {
      KernelIterator tmp( *this );
      operator++( );
      return( tmp );
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

  bool KernelIterator::operator==( const KernelIterator &cmp ) const {
    try {
      return( position == cmp.position );
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

  bool KernelIterator::operator!=( const KernelIterator &cmp ) const {
    try {
      return( position != cmp.position );
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

  size_t KernelIterator::operator*( ) const {
    try {
      return( krn_index );
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

  float KernelIterator::Value( ) const {
    try {
      return( kernel.Value( position ) );
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

#ifdef BIAL_EXPLICIT_KernelIterator

  template KernelIterator::KernelIterator( const Kernel &krn, const Matrix< int > &mat, size_t index );
  template KernelIterator::KernelIterator( const Kernel &krn, const Image< int > &img, size_t index );
  template KernelIterator::KernelIterator( const Kernel &krn, const Matrix< llint > &mat, size_t index );
  template KernelIterator::KernelIterator( const Kernel &krn, const Image< llint > &img, size_t index );
  template KernelIterator::KernelIterator( const Kernel &krn, const Matrix< float > &mat, size_t index );
  template KernelIterator::KernelIterator( const Kernel &krn, const Image< float > &img, size_t index );
  template KernelIterator::KernelIterator( const Kernel &krn, const Matrix< double > &mat, size_t index );
  template KernelIterator::KernelIterator( const Kernel &krn, const Image< double > &img, size_t index );

#endif

}

#endif

#endif
