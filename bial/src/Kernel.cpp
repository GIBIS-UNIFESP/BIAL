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

namespace Bial {

  Kernel::Kernel( ) try : Adjacency( 1, 3 ), coefficient( 1, 1.0f ) {
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

  Kernel::Kernel( size_t size, size_t dims ) try : Adjacency( size, dims ), coefficient( size, 1.0f ) {
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

  const float &Kernel::Value( size_t idx ) const {
    try {
      return( coefficient[ idx ] );
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

  float &Kernel::Value( size_t idx ) {
    try {
      return( coefficient[ idx ] );
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
