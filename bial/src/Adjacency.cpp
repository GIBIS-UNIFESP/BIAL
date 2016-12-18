/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Jul/05
 * @brief Adjacency relation to be used over an image or matrix.
 */

#ifndef BIALADJACENCY_C
#define BIALADJACENCY_C

#include "Adjacency.hpp"
#include "Image.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_Adjacency )
#define BIAL_EXPLICIT_Adjacency
#endif

#if defined ( BIAL_EXPLICIT_Adjacency ) || ( BIAL_IMPLICIT_BIN )

#include "Color.hpp"
#include "ColorRGB.hpp"

namespace Bial {

  /* Adjacency ----------------------------------------------------------------------------------------------------- **/

  Adjacency::Adjacency( ) try : relation( 1, 2 ) {
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

  Adjacency::Adjacency( size_t size, size_t dims ) try : relation( dims, size ) {
    IF_DEBUG( ( dims < 2 ) || ( dims > 3 ) ) {
      std::string msg( BIAL_ERROR( "Adjacency must have 2 or 3 dimensions. Given: " + std::to_string( dims ) ) );
      throw( std::logic_error( msg ) );
    }
    if( size == 0 ) {
      std::string msg( BIAL_ERROR( "Empty adjacency relation." ) );
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

  const int &Adjacency::operator()( size_t elem, size_t dim ) const {
    try {
      return( relation[ relation.dim_size[ 0 ] * elem + dim ] );
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

  int &Adjacency::operator()( size_t elem, size_t dim ) {
    try {
      return( relation[ relation.dim_size[ 0 ] * elem + dim ] );
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

  Vector< size_t > Adjacency::SortByDistance( bool increasing_order ) {
    try {
      Vector< double > local_radius( relation.dim_size[ 1 ] );
      Vector< size_t > position( relation.dim_size[ 1 ] );
      COMMENT( "Computing distance.", 1 );
      for( size_t i = 0; i < relation.dim_size[ 1 ]; ++i ) {
        local_radius( i ) = static_cast< double >( relation[ relation.dim_size[ 0 ] * i ] *
                                                   relation[ relation.dim_size[ 0 ] * i ] );
        for( size_t k = 1; k < relation.dim_size[ 0 ]; ++k )
          local_radius( i ) += static_cast< double >( relation[ relation.dim_size[ 0 ] * i + k ] *
                                                      relation[ relation.dim_size[ 0 ] * i + k ] );
        position( i ) = i;
      }
      COMMENT( "Sorting.", 1 );
      for( size_t i = 0; i < relation.dim_size[ 1 ] - 1; ++i ) {
        size_t pivot = i;
        for( size_t j = i + 1; j < relation.dim_size[ 1 ]; ++j ) {
          if( ( ( local_radius( j ) < local_radius( pivot ) ) && ( increasing_order ) ) ||
              ( ( local_radius( j ) > local_radius( pivot ) ) && ( !increasing_order ) ) )
            pivot = j;
        }
        std::swap( position( i ), position( pivot ) );
        std::swap( local_radius( i ), local_radius( pivot ) );
        for( size_t k = 0; k < relation.dim_size[ 0 ]; ++k )
          std::swap( relation[ relation.dim_size[ 0 ] * i + k ], relation[ relation.dim_size[ 0 ] * pivot + k ] );
      }
      return( position );
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

  size_t Adjacency::Dims( ) const {
    try {
      return( relation.dim_size[ 0 ] );
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

  size_t Adjacency::Size( ) const {
    try {
      return( relation.dim_size[ 1 ] );
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

  size_t Adjacency::size( ) const {
    try {
      return( relation.dim_size[ 1 ] );
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

  const Matrix< int > &Adjacency::Relation( ) const {
    try {
      return( relation );
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
  O &Adjacency::PrintDimensions( O &os ) const {
    try {
      os << relation.dim_size[ 0 ] << ", " << relation.dim_size[ 1 ] << std::endl;
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
  O &Adjacency::Print( O &os ) const {
    try {
      for( size_t elm = 0; elm < relation.dim_size[ 1 ]; ++elm ) {
        os << "Element " << elm << ": ";
        for( size_t dms = 0; dms < relation.dim_size[ 0 ]; ++dms ) {
          os << relation( dms, elm ) << " ";
        }
        os << std::endl;
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
  O &operator<<( O &os, const Adjacency &adj ) {
    adj.Print( os );
    return( os );
  }

#ifdef BIAL_EXPLICIT_Adjacency

  template std::ostream &Adjacency::PrintDimensions( std::ostream &os ) const;
  template std::ostream &Adjacency::Print( std::ostream &os ) const;
  template std::ostream &operator<<( std::ostream &os, const Adjacency &adj );

  template OFile &Adjacency::PrintDimensions( OFile &os ) const;
  template OFile &Adjacency::Print( OFile &os ) const;
  template OFile &operator<<( OFile &os, const Adjacency &adj );

#endif

}

#endif

#endif
