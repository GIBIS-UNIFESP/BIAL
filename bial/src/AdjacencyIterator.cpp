/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Jul/05
 * @brief Adjacency iterator class used as a forward iteratior for adjacency relation.
 * <br> Future add-on's: none.
 */

#ifndef BIALADJACENCYITERATOR_C
#define BIALADJACENCYITERATOR_C

#include "AdjacencyIterator.hpp"
#include "Image.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_AdjacencyIterator )
#define BIAL_EXPLICIT_AdjacencyIterator
#endif

#if defined ( BIAL_EXPLICIT_AdjacencyIterator ) || ( BIAL_IMPLICIT_BIN )

#include "Adjacency.hpp"
#include "Color.hpp"

namespace Bial {

  /* AdjacencyIterator ------------------------------------------------------------------------------------------------
  **/

  AdjacencyIterator::AdjacencyIterator( const Adjacency &adj, const Vector< size_t > &dim, size_t index )
  try : adjacency( adj ), dim_size( dim ), data_limit( 0 ), pixel_index( index ), adj_index( index ), position( 0 ) {
    if( dim.size( ) == 0 ) {
      std::string msg( BIAL_ERROR( "Empty space dimention vector." ) );
      throw( std::logic_error( msg ) );
    }
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

  template< class D > AdjacencyIterator::AdjacencyIterator( const Adjacency &adj, const Matrix< D > &mat, size_t index )
    try : adjacency( adj ), dim_size( mat.Dim( ) ), data_limit( mat.size( ) ), pixel_index( index ),
            adj_index( index ), position( 0 ) {
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

  template< class D > AdjacencyIterator::AdjacencyIterator( const Adjacency &adj, const Image< D > &img, size_t index )
    try : adjacency( adj ), dim_size( img.Dim( ) ), data_limit( img.size( ) ), pixel_index( index ), adj_index( index ),
            position( 0 ) {
      if( img.Dims( ) == 2 )
        dim_size.pop_back( );
      if( img.size( ) == 0 ) {
        std::string msg( BIAL_ERROR( "Empty image." ) );
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

  void AdjacencyIterator::begin( ) {
    try {
      adj_index = adjacency( dim_size, pixel_index, position );
      while( ( position < adjacency.size( ) ) && ( adj_index == data_limit ) ) {
        ++position;
        if( position >= adjacency.size( ) ) {
          adj_index = data_limit;
          break;
        }
        adj_index = adjacency( dim_size, pixel_index, position );
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

  void AdjacencyIterator::end( ) {
    try {
      position = adjacency.size( );
      adj_index = data_limit;
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

  AdjacencyIterator &AdjacencyIterator::operator++( ) {
    try {
      do {
        ++position;
        if( position >= adjacency.size( ) ) {
          adj_index = data_limit;
          break;
        }
        adj_index = adjacency( dim_size, pixel_index, position );
      } while( ( position < adjacency.size( ) ) && ( adj_index == data_limit ) );
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
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  AdjacencyIterator AdjacencyIterator::operator++( int ) {
    try {
      AdjacencyIterator tmp( *this );
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

  bool AdjacencyIterator::operator==( const AdjacencyIterator &cmp ) const {
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

  bool AdjacencyIterator::operator!=( const AdjacencyIterator &cmp ) const {
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

  size_t AdjacencyIterator::operator*( ) const {
    return( adj_index );
  }

  size_t AdjacencyIterator::Position( ) const {
    return( position );
  }

  template< >
  AdjacencyIterator begin( const Adjacency &adj, const Vector< size_t > &vct, size_t pixel_index ) {
    try {
      AdjacencyIterator it( adj, vct, pixel_index );
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

  template< >
  AdjacencyIterator end( const Adjacency &adj, const Vector< size_t > &vct, size_t pixel_index ) {
    try {
      AdjacencyIterator it( adj, vct, pixel_index );
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
  AdjacencyIterator begin( const Adjacency &adj, const Matrix< D > &mtx, size_t pixel_index ) {
    try {
      AdjacencyIterator it( adj, mtx, pixel_index );
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
  AdjacencyIterator end( const Adjacency &adj, const Matrix< D > &mtx, size_t pixel_index ) {
    try {
      AdjacencyIterator it( adj, mtx, pixel_index );
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
  AdjacencyIterator begin( const Adjacency &adj, const Image< D > &img, size_t pixel_index ) {
    try {
      AdjacencyIterator it( adj, img, pixel_index );
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
  AdjacencyIterator end( const Adjacency &adj, const Image< D > &img, size_t pixel_index ) {
    try {
      AdjacencyIterator it( adj, img, pixel_index );
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


#ifdef BIAL_EXPLICIT_AdjacencyIterator

  /* Non template functions of non template classes. -------------------------------------------------------------------
  **/


  /* Explicit instantiations. ------------------------------------------------------------------------------------------
  **/

  template AdjacencyIterator begin( const Adjacency &adj, const Matrix< int > &mtx, size_t pixel_index );
  template AdjacencyIterator end( const Adjacency &adj, const Matrix< int > &mtx, size_t pixel_index );
  template AdjacencyIterator begin( const Adjacency &adj, const Image< int > &img, size_t pixel_index );
  template AdjacencyIterator end( const Adjacency &adj, const Image< int > &img, size_t pixel_index );
  template AdjacencyIterator::AdjacencyIterator( const Adjacency &adj, const Matrix< int > &mat, size_t index );
  template AdjacencyIterator::AdjacencyIterator( const Adjacency &adj, const Image< int > &mat, size_t index );

  template AdjacencyIterator begin( const Adjacency &adj, const Matrix< llint > &mtx, size_t pixel_index );
  template AdjacencyIterator end( const Adjacency &adj, const Matrix< llint > &mtx, size_t pixel_index );
  template AdjacencyIterator begin( const Adjacency &adj, const Image< llint > &img, size_t pixel_index );
  template AdjacencyIterator end( const Adjacency &adj, const Image< llint > &img, size_t pixel_index );
  template AdjacencyIterator::AdjacencyIterator( const Adjacency &adj, const Matrix< llint > &mat, size_t index );
  template AdjacencyIterator::AdjacencyIterator( const Adjacency &adj, const Image< llint > &mat, size_t index );

  template AdjacencyIterator begin( const Adjacency &adj, const Matrix< float > &mtx, size_t pixel_index );
  template AdjacencyIterator end( const Adjacency &adj, const Matrix< float > &mtx, size_t pixel_index );
  template AdjacencyIterator begin( const Adjacency &adj, const Image< float > &img, size_t pixel_index );
  template AdjacencyIterator end( const Adjacency &adj, const Image< float > &img, size_t pixel_index );
  template AdjacencyIterator::AdjacencyIterator( const Adjacency &adj, const Matrix< float > &mat, size_t index );
  template AdjacencyIterator::AdjacencyIterator( const Adjacency &adj, const Image< float > &mat, size_t index );

  template AdjacencyIterator begin( const Adjacency &adj, const Matrix< double > &mtx, size_t pixel_index );
  template AdjacencyIterator end( const Adjacency &adj, const Matrix< double > &mtx, size_t pixel_index );
  template AdjacencyIterator begin( const Adjacency &adj, const Image< double > &img, size_t pixel_index );
  template AdjacencyIterator end( const Adjacency &adj, const Image< double > &img, size_t pixel_index );
  template AdjacencyIterator::AdjacencyIterator( const Adjacency &adj, const Matrix< double > &mat, size_t index );
  template AdjacencyIterator::AdjacencyIterator( const Adjacency &adj, const Image< double > &mat, size_t index );

  template AdjacencyIterator begin( const Adjacency &adj, const Image< Color > &img, size_t pixel_index );
  template AdjacencyIterator end( const Adjacency &adj, const Image< Color > &img, size_t pixel_index );
  template AdjacencyIterator::AdjacencyIterator( const Adjacency &adj, const Image< Color > &mat, size_t index );

#endif

}

#endif

#endif
