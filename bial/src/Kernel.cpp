/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Nov/19 
 * @brief Kernel class.
 */

#ifndef BIALKERNEL_C
#define BIALKERNEL_C

#include "Kernel.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_Kernel )
#define BIAL_EXPLICIT_Kernel
#endif
#if defined ( BIAL_EXPLICIT_Kernel ) || ( BIAL_IMPLICIT_BIN )

#include "KernelIterator.hpp"

namespace Bial {

  Kernel::Kernel( ) try : Adjacency( 1, 1 ), coefficient( 1, 1.0f ) {
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

  Kernel::Kernel( const Adjacency &adj ) try : Adjacency( adj ), coefficient( adj.size( ), 1.0f ) {
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

  Kernel::Kernel( size_t dims, size_t size ) try : Adjacency( dims, size ), coefficient( size, 1.0f ) {
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
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Empty kernel." ) );
    throw( std::logic_error( msg ) );
  }

  template< class D > Kernel::Kernel( const Matrix< D > &mat ) try : Adjacency( mat ), coefficient( 1 ) {
    coefficient = Vector< float >( this->relation.size( ) );
    COMMENT( "Setting coefficients.", 2 );
    size_t adj = 0;
    for( size_t elm = 0; elm < mat.size( ); ++elm ) {
      if( mat[ elm ] != 0 ) {
        coefficient( adj ) = mat[ elm ];
        ++adj;
      }
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
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Empty kernel." ) );
    throw( std::logic_error( msg ) );
  }

  template< class D > Kernel::Kernel( const Image< D > &img ) try : Adjacency( img ), coefficient( 1 ) {
    coefficient = Vector< float >( this->relation.size( ) );
    COMMENT( "Setting coefficients.", 2 );
    size_t adj = 0;
    for( size_t elm = 0; elm < img.size( ); ++elm ) {
      if( img[ elm ] != 0 ) {
        coefficient( adj ) = img[ elm ];
        ++adj;
      }
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
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Empty kernel." ) );
    throw( std::logic_error( msg ) );
  }

  template< class D >
  KernelIterator Kernel::begin( const Matrix< D > &mtx, size_t pixel_index ) const {
    try {
      KernelIterator it( *this, mtx, pixel_index );
      it.begin( );
      return( it );
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
  KernelIterator Kernel::end( const Matrix< D > &mtx, size_t pixel_index ) const {
    try {
      KernelIterator it( *this, mtx, pixel_index );
      it.end( );
      return( it );
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
  KernelIterator Kernel::begin( const Image< D > &img, size_t pixel_index ) const {
    try {
      KernelIterator it( *this, img, pixel_index );
      it.begin( );
      return( it );
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
  KernelIterator Kernel::end( const Image< D > &img, size_t pixel_index ) const {
    try {
      KernelIterator it( *this, img, pixel_index );
      it.end( );
      return( it );
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

  void Kernel::Normalize( ) {
    try {
      COMMENT( "Computing sum of values.", 2 );
      float total_value = 0.0f;
      for( size_t elm = 0; elm < this->size( ); ++elm ) {
        total_value += coefficient( elm );
      }
      if( total_value == 0 ) {
        std::string msg( BIAL_ERROR( "Could not normalyze kernel coefficients. Sum of elements is zero." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Normalizing.", 2 );
      for( size_t elm = 0; elm < this->size( ); ++elm ) {
        coefficient( elm ) = coefficient( elm ) / total_value;
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

  Kernel Kernel::Normalize( ) const {
    try {
      Kernel krn( *this );
      krn.Normalize( );
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

  void Kernel::SignalNormalize( ) {
    try {
      COMMENT( "Computing sum of values.", 2 );
      float pos_total_value = 0.0f;
      float neg_total_value = 0.0f;
      for( size_t elm = 0; elm < this->size( ); ++elm ) {
        if( coefficient( elm ) > 0.0f ) {
          pos_total_value += coefficient( elm );
        }
        if( coefficient( elm ) < 0.0f ) {
          neg_total_value += coefficient( elm );
        }
      }
      COMMENT( "Normalizing.", 2 );
      for( size_t elm = 0; elm < this->size( ); ++elm ) {
        if( coefficient( elm ) > 0.0f ) {
          coefficient( elm ) = coefficient( elm ) / pos_total_value;
        }
        if( coefficient( elm ) < 0.0f ) {
          coefficient( elm ) = coefficient( elm ) / -neg_total_value;
        }
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

  Kernel Kernel::SignalNormalize( ) const {
    try {
      Kernel krn( *this );
      krn.SignalNormalize( );
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

  float Kernel::Value( size_t idx ) const {
    try {
      return( coefficient( idx ) );
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

  void Kernel::Value( size_t idx, float value ) {
    try {
      coefficient( idx ) = value;
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

  template< class O >
  O &Kernel::PrintDimensions( O &os ) const {
    try {
      os << this->relation.dim_size( 0 ) << ", " << this->relation.dim_size( 1 ) << std::endl;
      return( os );
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

  template< class O >
  O &Kernel::Print( O &os ) const {
    try {
      for( size_t elm = 0; elm < relation.dim_size( 1 ); ++elm ) {
        os << "Element " << elm << ": ";
        for( size_t dms = 0; dms < relation.dim_size( 0 ); ++dms ) {
          os << relation( dms, elm ) << " ";
        }
        os << "[" << coefficient( elm ) << "]" << std::endl;
      }
      return( os );
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

  template< class O >
  O &operator<<( O &os, const Kernel &knl ) {
    knl.Print( os );
    return( os );
  }

#ifdef BIAL_EXPLICIT_Kernel

  template Kernel::Kernel( const Matrix< int > &mat );
  template Kernel::Kernel( const Image< int > &img );
  template KernelIterator Kernel::begin( const Matrix< int > &mtx, size_t pixel_index ) const;
  template KernelIterator Kernel::end( const Matrix< int > &mtx, size_t pixel_index ) const;
  template KernelIterator Kernel::begin( const Image< int > &img, size_t pixel_index ) const;
  template KernelIterator Kernel::end( const Image< int > &img, size_t pixel_index ) const;

  template Kernel::Kernel( const Matrix< llint > &mat );
  template Kernel::Kernel( const Image< llint > &img );
  template KernelIterator Kernel::begin( const Matrix< llint > &mtx, size_t pixel_index ) const;
  template KernelIterator Kernel::end( const Matrix< llint > &mtx, size_t pixel_index ) const;
  template KernelIterator Kernel::begin( const Image< llint > &img, size_t pixel_index ) const;
  template KernelIterator Kernel::end( const Image< llint > &img, size_t pixel_index ) const;

  template Kernel::Kernel( const Matrix< float > &mat );
  template Kernel::Kernel( const Image< float > &img );
  template KernelIterator Kernel::begin( const Matrix< float > &mtx, size_t pixel_index ) const;
  template KernelIterator Kernel::end( const Matrix< float > &mtx, size_t pixel_index ) const;
  template KernelIterator Kernel::begin( const Image< float > &img, size_t pixel_index ) const;
  template KernelIterator Kernel::end( const Image< float > &img, size_t pixel_index ) const;

  template Kernel::Kernel( const Matrix< double > &mat );
  template Kernel::Kernel( const Image< double > &img );
  template KernelIterator Kernel::begin( const Matrix< double > &mtx, size_t pixel_index ) const;
  template KernelIterator Kernel::end( const Matrix< double > &mtx, size_t pixel_index ) const;
  template KernelIterator Kernel::begin( const Image< double > &img, size_t pixel_index ) const;
  template KernelIterator Kernel::end( const Image< double > &img, size_t pixel_index ) const;

  template std::ostream &Kernel::PrintDimensions( std::ostream &os ) const;
  template std::ostream &Kernel::Print( std::ostream &os ) const;
  template std::ostream &operator<<( std::ostream &os, const Kernel &knl );

  template OFile &Kernel::PrintDimensions( OFile &os ) const;
  template OFile &Kernel::Print( OFile &os ) const;
  template OFile &operator<<( OFile &os, const Kernel &knl );

#endif

}

#endif

#endif
