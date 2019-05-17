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

  AdjacencyIterator::AdjacencyIterator( const Adjacency &adj, size_t x_size, size_t y_size, size_t z_size ) try :
    relation( adj.Relation( ) ), x_size( x_size ), y_size( y_size ), z_size( z_size ),
      xy_size( x_size * y_size ), img_size( x_size * y_size * z_size ), adj_size( adj.size( ) ), dims( adj.Dims( ) ),
      displacement( adj.size( ) ) {
      IF_DEBUG( dims != adj.Dims( ) ) {
        std::string msg( BIAL_ERROR( "Image and adjacency dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      IF_DEBUG( img_size == 0 ) {
        std::string msg( BIAL_ERROR( "Empty image." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Computing displacement vector.", 3 );
      size_t acc_dim_size = 1;
      Vector< size_t > displacement( 3, x_size );
      displacement[ 1 ] = xy_size;
      displacement[ 2 ] = img_size;
      for( size_t dms = 0; dms < dims; ++dms ) {
        for( size_t adj_idx = 0; adj_idx < adj_size; ++adj_idx )
          displacement[ adj_idx ] += adj( adj_idx, dms ) * acc_dim_size;
        acc_dim_size = displacement[ dms ];
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

  template< class D > 
  AdjacencyIterator::AdjacencyIterator( const Image< D > &img, const Adjacency &adj ) try :
    relation( adj.Relation( ) ), x_size( img.size( 0 ) ), y_size( img.size( 1 ) ), z_size( img.size( 2 ) ),
      xy_size( img.Displacement( 1 ) ), img_size( img.size( ) ), adj_size( adj.size( ) ), dims( img.Dims( ) ), 
      displacement( adj.size( ) ) {
      IF_DEBUG( dims != adj.Dims( ) ) {
        std::string msg( BIAL_ERROR( "Image and adjacency dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      IF_DEBUG( img_size == 0 ) {
        std::string msg( BIAL_ERROR( "Empty image." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Computing displacement vector.", 3 );
      size_t acc_dim_size = 1;
      for( size_t dms = 0; dms < dims; ++dms ) {
        for( size_t adj_idx = 0; adj_idx < adj_size; ++adj_idx )
          displacement[ adj_idx ] += adj( adj_idx, dms ) * acc_dim_size;
        acc_dim_size = img.Displacement( dms );
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

  template< class D > 
  AdjacencyIterator::AdjacencyIterator( const Adjacency &adj, const Image< D > &img ) try :
    AdjacencyIterator::AdjacencyIterator( img, adj ) {
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

  AdjacencyIterator::AdjacencyIterator( const AdjacencyIterator &copy ) try :
    relation( copy.relation ), x_size( copy.x_size ), y_size( copy.y_size ), z_size( copy.z_size ),
      xy_size( copy.xy_size ), img_size( copy.img_size ), adj_size( copy.adj_size ), dims( copy.dims ),
      displacement( copy.displacement ) {
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

  size_t AdjacencyIterator::operator()( size_t src_index, size_t adj_position ) const {
    try {
      return( src_index + displacement[ adj_position ] );
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

  int AdjacencyIterator::Displacement( size_t adj_position ) const {
    try {
      return( displacement( adj_position ) );
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

  bool AdjacencyIterator::AdjIdx2( size_t src_index, size_t adj_position, size_t &adj_index ) const {
    try {
      COMMENT( "Computing coordinates.", 4 );
      size_t rel_base_index = ( adj_position << 1 ) + adj_position; // rel_base_index = adj_position * 3
      div_t div_pos_by_x = std::div( static_cast< int >( src_index ), static_cast< int >( x_size ) );
      size_t x = static_cast< size_t >( div_pos_by_x.rem + relation[ rel_base_index ] );
      size_t y = static_cast< size_t >( div_pos_by_x.quot + relation[ rel_base_index + 1 ] );
      COMMENT( "Checking if coordinates are valid.", 4 );
      if( ( x >= x_size ) || ( y >= y_size ) )
        return( false );
      adj_index = src_index + displacement[ adj_position ];
      return( true );
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

  bool AdjacencyIterator::AdjVct2( const Vector< size_t > &src_index, size_t adj_position, Vector< size_t > &adj_index )
    const {
    try {
      IF_DEBUG( ( adj_index.size( ) != src_index.size( ) ) || ( adj_index.size( ) < 2 ) ) {
        std::string msg( BIAL_ERROR( "Input vectors must have two dimensions." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Computing coordinates.", 5 );
      size_t rel_base_index = ( adj_position << 1 ) + adj_position; // rel_base_index = adj_position * 3
      adj_index[ 0 ] = static_cast< size_t >( src_index[ 0 ] + relation[ rel_base_index ] );
      adj_index[ 1 ] = static_cast< size_t >( src_index[ 1 ] + relation[ rel_base_index + 1 ] );
      COMMENT( "Checking if coordinates are valid.", 5 );
      if( ( adj_index[ 0 ] >= x_size ) || ( adj_index[ 1 ] >= y_size ) )
        return( false );
      return( true );
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

  bool AdjacencyIterator::AdjIdx( size_t src_index, size_t adj_position, size_t &adj_index ) const { 
    try {
      COMMENT( "Computing coordinates.", 4 );
      size_t rel_base_index = ( adj_position << 1 ) + adj_position;  // rel_base_index = adj_position * 3
      div_t div_pos_by_xy = std::div( static_cast< int >( src_index ), static_cast< int >( xy_size ) );
      div_t div_rem_by_x =  std::div( div_pos_by_xy.rem, static_cast< int >( x_size ) );
      size_t x = static_cast< size_t >( div_rem_by_x.rem + relation[ rel_base_index ] );
      size_t y = static_cast< size_t >( div_rem_by_x.quot + relation[ rel_base_index + 1 ] );
      size_t z = static_cast< size_t >( div_pos_by_xy.quot + relation[ rel_base_index + 2 ] );
      COMMENT( "Checking if coordinates are valid: " << x << ", " << y << ", " << z, 4 );
      if( ( x >= x_size ) || ( y >= y_size ) || ( z >= z_size ) )
        return( false );
      COMMENT( "Computing adjacent index by: " << std::to_string( src_index ) << " + " <<
               std::to_string( displacement[ adj_position ] ), 4 );
      adj_index = src_index + displacement[ adj_position ];
      return( true );
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

  bool AdjacencyIterator::AdjVct( const Vector< size_t > &src_index, size_t adj_position, Vector< size_t > &adj_index )
    const {
    try {
      IF_DEBUG( adj_index.size( ) != src_index.size( ) ) {
        std::string msg( BIAL_ERROR( "Input vectors must have same dimensions." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Computing coordinates.", 5 );
      size_t rel_base_index = ( adj_position << 1 ) + adj_position;  // rel_base_index = adj_position * 3
      adj_index[ 0 ] = static_cast< size_t >( src_index[ 0 ] + relation[ rel_base_index ] );
      adj_index[ 1 ] = static_cast< size_t >( src_index[ 1 ] + relation[ rel_base_index + 1 ] );
      adj_index[ 2 ] = static_cast< size_t >( src_index[ 2 ] + relation[ rel_base_index + 2 ] );
      COMMENT( "Checking if coordinates are valid.", 5 );
      if( ( adj_index[ 0 ] >= x_size ) || ( adj_index[ 1 ] >= y_size ) || ( adj_index[ 2 ] >= z_size ) )
        return( false );
      return( true );
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

  template AdjacencyIterator::AdjacencyIterator( const Image< int > &img, const Adjacency &adj );
  template AdjacencyIterator::AdjacencyIterator( const Image< llint > &img, const Adjacency &adj );
  template AdjacencyIterator::AdjacencyIterator( const Image< float > &img, const Adjacency &adj );
  template AdjacencyIterator::AdjacencyIterator( const Image< double > &img, const Adjacency &adj );
  template AdjacencyIterator::AdjacencyIterator( const Image< Color > &img, const Adjacency &adj );
  template AdjacencyIterator::AdjacencyIterator( const Image< RealColor > &img, const Adjacency &adj );

  template AdjacencyIterator::AdjacencyIterator( const Adjacency &adj, const Image< int > &img );
  template AdjacencyIterator::AdjacencyIterator( const Adjacency &adj, const Image< llint > &img );
  template AdjacencyIterator::AdjacencyIterator( const Adjacency &adj, const Image< float > &img );
  template AdjacencyIterator::AdjacencyIterator( const Adjacency &adj, const Image< double > &img );
  template AdjacencyIterator::AdjacencyIterator( const Adjacency &adj, const Image< Color > &img );
  template AdjacencyIterator::AdjacencyIterator( const Adjacency &adj, const Image< RealColor > &img );

#endif

}

#endif

#endif
