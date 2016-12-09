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

  /* Non-template and template functions or member functions of a template class that are explictly instantiated. ------
  **/

  /* Adjacency ---------------------------------------------------------------------------------------------------------
  **/

  Adjacency::Adjacency( ) try : relation( 1, 1 ) {
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

  Adjacency::Adjacency( size_t dims, size_t size ) try : relation( dims, size ) {
    if( ( dims == 0 ) || ( size == 0 ) ) {
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

  Adjacency::Adjacency( const Vector< size_t > &src_displacement, const Vector< size_t > &tgt_displacement ) try
    : relation( 1, 1 ) {
      if( src_displacement.size( ) != tgt_displacement.size( ) ) {
        std::string msg( BIAL_ERROR( "Source and target displament vector sizes do not match. Given: source size: " +
                                     std::to_string( src_displacement.size( ) ) + ", target size: " +
                                     std::to_string( tgt_displacement.size( ) ) ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Copy of src and tgt vectors.", 0 );
      Vector< size_t > src( src_displacement );
      Vector< size_t > tgt( tgt_displacement );
      size_t dimensions = src.size( );

      COMMENT( "Setting src displacements lower than tgt displaments, and computing number of adjacents.", 0 );
      size_t adjacents = 1;
      for( size_t dms = 0; dms < dimensions; ++dms ) {
        if( src[ dms ] > tgt[ dms ] ) {
          std::swap( src[ dms ], tgt[ dms ] );
        }
        adjacents *= ( tgt[ dms ] - src[ dms ] + 1 );
      }
      COMMENT( "Dimensions: " << dimensions << ", Adjacents: " << adjacents << ". Computing adjacents.", 0 );
      relation = Matrix< size_t >( dimensions, adjacents );
      Vector< size_t > pxl_coord( src );
      for( size_t elm = 0; elm < adjacents; ++elm ) {
        
        COMMENT( "Pixel coord: " << pxl_coord << ". Assigning displacement coordinates.", 1 );
        for( size_t dms = 0; dms < dimensions; ++dms ) {
          relation( dms, elm ) = pxl_coord( dms );
        }
        COMMENT( "Updating coordinates.", 1 );
        for( size_t dms = 0; dms < dimensions; ++dms ) {
          if( pxl_coord( dms ) < tgt( dms ) ) {
            ++pxl_coord( dms );
            break;
          }
          else {
            pxl_coord( dms ) = src( dms );
          }
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

  template< class D > Adjacency::Adjacency( const Matrix< D > &mat ) try : relation( 1, 1 ) {
    if( ( mat.dims == 0 ) || ( mat.dim_size[ 0 ] == 0 ) ) {
      std::string msg( BIAL_ERROR( "Empty adjacency relation." ) );
      throw( std::logic_error( msg ) );
    }
    COMMENT( "Computing adjacency size.", 1 );
    size_t size = 0;
    for( size_t elm = 0; elm < mat._size; ++elm ) {
      if( mat[ elm ] != 0 ) {
        ++size;
      }
    }
    if( size == 0 ) {
      std::string msg( BIAL_ERROR( "Empty adjacency relation. Null matrix." ) );
      throw( std::logic_error( msg ) );
    }
    relation = Matrix< int >( mat.dims, size );

    COMMENT( "Setting adjacency displacements.", 1 );
    size_t adj = 0;
    for( size_t elm = 0; elm < mat._size; ++elm ) {
      if( mat[ elm ] != 0 ) {
        for( size_t dms = 0; dms < mat.Dims( ); ++dms ) {
          relation( dms, adj ) = mat.Coordinate( elm, dms ) - mat.size( dms ) / 2;
        }
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
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
    throw( std::logic_error( msg ) );
  }

  template< class D > Adjacency::Adjacency( const Image< D > &img ) try : relation( 1, 1 ) {
    if( ( img.Dims( ) == 0 ) || ( img.size( 0 ) == 0 ) ) {
      std::string msg( BIAL_ERROR( "Empty adjacency relation." ) );
      throw( std::logic_error( msg ) );
    }
    COMMENT( "Computing adjacency size.", 1 );
    size_t size = 0;
    for( size_t elm = 0; elm < img.size( ); ++elm ) {
      if( img[ elm ] != 0 ) {
        ++size;
      }
    }
    if( size == 0 ) {
      std::string msg( BIAL_ERROR( "Empty adjacency relation. Null matrix." ) );
      throw( std::logic_error( msg ) );
    }
    relation = Matrix< int >( img.Dims( ), size );

    COMMENT( "Setting adjacency displacements.", 1 );
    size_t adj = 0;
    for( size_t elm = 0; elm < img.size( ); ++elm ) {
      if( img[ elm ] != 0 ) {
        for( size_t dms = 0; dms < img.Dims( ); ++dms ) {
          relation( dms, adj ) = img.Coordinate( elm, dms ) - img.Displacement( dms ) / 2;
        }
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
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
    throw( std::logic_error( msg ) );
  }

  Adjacency::Adjacency( const Image< Color > &img ) try : relation( 1, 1 ) {
    if( ( img.Dims( ) == 0 ) || ( img.size( 0 ) == 0 ) ) {
      std::string msg( BIAL_ERROR( "Empty adjacency relation." ) );
      throw( std::logic_error( msg ) );
    }

    Image< int > gray_img( ColorSpace::ARGBtoGraybyBrightness< int >( img ) );
    *this = Adjacency( gray_img );
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

  int Adjacency::operator()( size_t elem, size_t dim ) const {
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

  Vector< size_t > Adjacency::operator()( const Vector< size_t > &dim, const Vector< size_t > &index,
                                          size_t adj_index ) const {
    try {
      COMMENT( "Running special cases for 2D and 3D.", 4 );
      if( dim.size( ) == 3 ) {
        return( Adj3( dim, index, adj_index ) );
      }
      if( dim.size( ) == 2 ) {
        return( Adj2( dim, index, adj_index ) );
      }
      COMMENT( "Computing matrix size.", 4 );
      size_t mat_size = dim( 0 );
      for( size_t idx = 1; idx < dim.size( ); ++idx ) {
        mat_size *= dim( idx );
      }
      COMMENT( "Creating adjacent vector.", 4 );
      Vector< size_t > adjacent( relation.dim_size[ 0 ], 0 );
      for( size_t dms = 0; dms < relation.dim_size[ 0 ]; ++dms ) {

        COMMENT( "Computing adjacent coordinate.", 4 );
        int adjacent_coord = index( dms ) + ( *this )( dms, adj_index );
        adjacent( dms ) = static_cast< size_t >( adjacent_coord );

        COMMENT( "Verifying if adjacent coordinate is valid.", 4 );
        if( ( adjacent_coord < 0.0 ) || ( adjacent( dms ) >= dim( dms ) ) ) {
          return( dim );
        }
      }
      return( adjacent );
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

  size_t Adjacency::operator()( const Vector< size_t > &dim, size_t position, size_t adj_index ) const {
    try {
      COMMENT( "Running special cases for 2D and 3D.", 4 );
      if( dim.size( ) == 3 ) {
        return( Adj3( dim, position, adj_index ) );
      }
      if( dim.size( ) == 2 ) {
        return( Adj2( dim, position, adj_index ) );
      }
      COMMENT( "Initializing variables containing image domain size.", 4 );
      size_t mat_size = dim( 0 );
      for( size_t idx = 1; idx < dim.size( ); ++idx ) {
        mat_size *= dim( idx );
      }
      COMMENT( "Computing adjacent index.", 4 );
      size_t result = 0;
      int dms = dim.size( ) - 1;
      for( size_t factor = mat_size / dim( dms ); dms >= 0; --dms ) {
        COMMENT( "Computing dimension coordinate.", 4 );
        int adjacent_coord = position / factor + ( *this )( dms, adj_index );
        COMMENT( "Checking if coordinate is valid.", 4 );
        if( ( adjacent_coord < 0.0 ) || ( static_cast< size_t >( adjacent_coord ) >= dim( dms ) ) ) {
          return( mat_size );
        }
        COMMENT( "Updating variables.", 4 );
        result += static_cast< size_t >( adjacent_coord ) * factor;
        position %= factor;
        factor /= dim( dms - 1 );
      }
      return( result );
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
  Vector< size_t > Adjacency::operator()( const Matrix< D > &mat, const Vector< size_t > &index,
                                          size_t adj_index ) const {
    try {
      return( ( *this )( mat.dim_size, index, adj_index ) );
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
  size_t Adjacency::operator()( const Matrix< D > &mat, size_t position, size_t adj_index ) const {
    try {
      return( ( *this )( mat.dim_size, position, adj_index ) );
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
  Vector< size_t > Adjacency::operator()( const Image< D > &img, const Vector< size_t > &index,
                                          size_t adj_index ) const {
    try {
      if( img.Dims( ) == 2 )
        return( Adj2( img.Dim( ), index, adj_index ) );
      return( Adj3( img.Dim( ), index, adj_index ) );
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
  size_t Adjacency::operator()( const Image< D > &img, size_t position, size_t adj_index ) const {
    try {
      if( img.Dims( ) == 2 )
        return( Adj2( img.Dim( ), position, adj_index ) );
      return( Adj3( img.Dim( ), position, adj_index ) );
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

  Vector< size_t > Adjacency::SortByDistance( bool increasing_order ) {
    try {
      Vector< double > local_radius( relation.dim_size[ 1 ] );
      Vector< size_t > position( relation.dim_size[ 1 ] );
      COMMENT( "Computing distance.", 1 );
      for( size_t i = 0; i < relation.dim_size[ 1 ]; ++i ) {
        local_radius( i ) = static_cast< double >( relation[ relation.dim_size[ 0 ] * i ] *
                                                   relation[ relation.dim_size[ 0 ] * i ] );
        for( size_t k = 1; k < relation.dim_size[ 0 ]; ++k ) {
          local_radius( i ) += static_cast< double >( relation[ relation.dim_size[ 0 ] * i + k ] *
                                                      relation[ relation.dim_size[ 0 ] * i + k ] );
        }
        position( i ) = i;
      }
      COMMENT( "Sorting.", 1 );
      for( size_t i = 0; i < relation.dim_size[ 1 ] - 1; ++i ) {
        size_t pivot = i;
        for( size_t j = i + 1; j < relation.dim_size[ 1 ]; ++j ) {
          if( ( ( local_radius( j ) < local_radius( pivot ) ) &&
                ( increasing_order ) ) ||
              ( ( local_radius( j ) > local_radius( pivot ) ) && ( !increasing_order ) ) ) {
            pivot = j;
          }
        }
        std::swap( position( i ), position( pivot ) );
        std::swap( local_radius( i ), local_radius( pivot ) );
        for( size_t k = 0; k < relation.dim_size[ 0 ]; ++k ) {
          std::swap( relation[ relation.dim_size[ 0 ] * i + k ], relation[ relation.dim_size[ 0 ] * pivot + k ] );
        }
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

  int Adjacency::Displacement( size_t dim, size_t elem ) const {
    try {
      return( relation( dim, elem ) );
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

  Vector< size_t > Adjacency::Adj2( const Vector< size_t > &dim, const Vector< size_t > &index,
                                    size_t adj_index ) const {
    try {
      COMMENT( "Creating resultant vector.", 4 );
      Vector< size_t > result( 2, 0 );
      COMMENT( "Computing dimension coordinate.", 4 );
      int adj_coord_0 = index[ 0 ] + ( *this )( 0, adj_index );
      result[ 0 ] = static_cast< size_t >( adj_coord_0 );
      int adj_coord_1 = index[ 1 ] + ( *this )( 1, adj_index );
      result[ 1 ] = static_cast< size_t >( adj_coord_1 );
      COMMENT( "Checking if coordinate is valid.", 4 );
      if( ( adj_coord_0 < 0.0 ) || ( adj_coord_1 < 0.0 ) || ( result[ 0 ] >= dim[ 0 ] ) || ( result[ 1 ] >= dim[ 1 ] ) )
        return( Vector< size_t >( dim[ 0 ] * dim[ 1 ] ) );
      return( result );
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

  size_t Adjacency::Adj2( const Vector< size_t > &dim, size_t position, size_t adj_index ) const {
    try {
      COMMENT( "Computing coordinates.", 4 );
      int x = ( position % dim[ 0 ] ) + relation[ relation.dim_size[ 0 ] * adj_index ];
      int y = ( position / dim[ 0 ] ) + relation[ relation.dim_size[ 0 ] * adj_index + 1 ];
      COMMENT( "Checking if coordinates are valid.", 4 );
      if( ( x < 0 ) || ( x >= static_cast< int >( dim[ 0 ] ) ) ||
          ( y < 0 ) || ( y >= static_cast< int >( dim[ 1 ] ) ) )
        return( dim[ 0 ] * dim[ 1 ] );
      else
        return( x + y * dim[ 0 ] );
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
  Vector< size_t > Adjacency::Adj2( const Matrix< D > &mat, const Vector< size_t > &index, size_t adj_index ) const {
    try {
      return( this->Adj2( mat.Dim( ), index, adj_index ) );
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
  size_t Adjacency::Adj2( const Matrix< D > &mat, size_t position, size_t adj_index ) const {
    try {
      return( this->Adj2( mat.Dim( ), position, adj_index ) );
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
  Vector< size_t > Adjacency::Adj2( const Image< D > &img, const Vector< size_t > &index, size_t adj_index ) const {
    try {
      Vector< size_t > dims( img.Dim( ) );
      dims.pop_back( );
      return( this->Adj2( dims, index, adj_index ) );
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
  size_t Adjacency::Adj2( const Image< D > &img, size_t position, size_t adj_index ) const {
    try {
      Vector< size_t > dims( img.Dim( ) );
      dims.pop_back( );
      return( this->Adj2( dims, position, adj_index ) );
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

  Vector< size_t > Adjacency::Adj3( const Vector< size_t > &dim, const Vector< size_t > &index,
                                    size_t adj_index ) const {
    try {
      COMMENT( "Creating resultant vector.", 5 );
      Vector< size_t > result( 3, 0 );
      COMMENT( "Computing dimension coordinate.", 5 );
      int adj_coord_0 = index[ 0 ] + ( *this )( 0, adj_index );
      result[ 0 ] = static_cast< size_t >( adj_coord_0 );
      int adj_coord_1 = index[ 1 ] + ( *this )( 1, adj_index );
      result[ 1 ] = static_cast< size_t >( adj_coord_1 );
      int adj_coord_2 = index[ 2 ] + ( *this )( 2, adj_index );
      result[ 2 ] = static_cast< size_t >( adj_coord_2 );
      COMMENT( "Checking if coordinate is valid.", 5 );
      if( ( adj_coord_0 < 0.0 ) || ( adj_coord_1 < 0.0 ) || ( adj_coord_2 < 0.0 ) ||
          ( result[ 0 ] >= dim[ 0 ] ) || ( result[ 1 ] >= dim[ 1 ] ) || ( result[ 2 ] >= dim[ 2 ] ) )
        return( Vector< size_t >( dim[ 0 ] * dim[ 1 ] * dim[ 2 ] ) );
      return( result );
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

  size_t Adjacency::Adj3( const Vector< size_t > &dim, size_t position, size_t adj_index ) const { 
    try {
      COMMENT( "Computing coordinates.", 5 );
      size_t xydim = dim[ 0 ] * dim[ 1 ];
      size_t adj_position = ( adj_index << 1 ) + adj_index;  // adj_position = adj_index * 3
      div_t div_pos_by_xy = std::div( static_cast< int >( position ), static_cast< int >( xydim ) );
      div_t div_rem_by_x =  std::div( div_pos_by_xy.rem, static_cast< int >( dim[ 0 ] ) );
      size_t x = static_cast< size_t >( div_rem_by_x.rem + relation[ adj_position ] );
      size_t y = static_cast< size_t >( div_rem_by_x.quot + relation[ adj_position + 1 ] );
      size_t z = static_cast< size_t >( div_pos_by_xy.quot + relation[ adj_position + 2 ] );
      // int x = ( position % dim[ 0 ] ) + relation[ adj_position ];
      // int y = ( div_pos_xydim.rem / dim[ 0 ] ) + relation[ adj_position + 1 ];
      // int z = div_pos_xydim.quot + relation[ adj_position + 2 ];
      COMMENT( "Checking if coordinates are valid.", 5 );
      if( ( x >= dim[ 0 ] ) || ( y >= dim[ 1 ] ) || ( z >= dim[ 2 ] ) )
      // if( ( x < 0 ) || ( y < 0 ) || ( z < 0 ) || ( x >= static_cast< int >( dim[ 0 ] ) ) || 
      //     ( y >= static_cast< int >( dim[ 1 ] ) ) || ( z >= static_cast< int >( dim[ 2 ] ) ) )
        return( dim.size( ) );
      else
        return( x + y * dim[ 0 ] + z * xydim ); // Usar tabela aqui.
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

  size_t Adjacency::Adj3( const Vector< size_t > &dim, size_t xydim, size_t position, size_t adj_index ) const { 
    try {
      COMMENT( "Computing coordinates.", 5 );
      size_t adj_position = ( adj_index << 1 ) + adj_index; // adj_position = adj_index * 3
      div_t div_pos_by_xy = std::div( static_cast< int >( position ), static_cast< int >( xydim ) );
      div_t div_rem_by_x =  std::div( div_pos_by_xy.rem, static_cast< int >( dim[ 0 ] ) );
      size_t x = static_cast< size_t >( div_rem_by_x.rem + relation[ adj_position ] );
      size_t y = static_cast< size_t >( div_rem_by_x.quot + relation[ adj_position + 1 ] );
      size_t z = static_cast< size_t >( div_pos_by_xy.quot + relation[ adj_position + 2 ] );
      // int x = ( position % dim[ 0 ] ) + relation[ adj_position ];
      // int y = ( div_xydim.rem / dim[ 0 ] ) + relation[ adj_position + 1 ];
      // int z = div_xydim.quot + relation[ adj_position + 2 ];
      COMMENT( "Checking if coordinates are valid.", 5 );
      if( ( x >= dim[ 0 ] ) || ( y >= dim[ 1 ] ) || ( z >= dim[ 2 ] ) )
      // if( ( x < 0 ) || ( y < 0 ) || ( z < 0 ) || ( x >= static_cast< int >( dim[ 0 ] ) ) || 
      //     ( y >= static_cast< int >( dim[ 1 ] ) ) || ( z >= static_cast< int >( dim[ 2 ] ) ) )
        return( dim.size( ) );
      else
        return( x + y * dim[ 0 ] + z * xydim ); // Usar tabela aqui.
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
  Vector< size_t > Adjacency::Adj3( const Matrix< D > &mat, const Vector< size_t > &index, size_t adj_index ) const {
    try {
      return( this->Adj3( mat.Dim( ), index, adj_index ) );
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
  size_t Adjacency::Adj3( const Matrix< D > &mat, size_t position, size_t adj_index ) const {
    try {
      return( this->Adj3( mat.Dim( ), mat.Displacement( 1 ), position, adj_index ) );
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
  Vector< size_t > Adjacency::Adj3( const Image< D > &img, const Vector< size_t > &index, size_t adj_index ) const {
    try {
      return( this->Adj3( img.Dim( ), index, adj_index ) );
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
  size_t Adjacency::Adj3( const Image< D > &img, size_t position, size_t adj_index ) const {
    try {
      return( this->Adj3( img.Dim( ), img.Displacement( 1 ), position, adj_index ) );
      // COMMENT( "Computing coordinates.", 5 );
      // Vector< size_t > dim( img.Dim( ) );
      // size_t xydim = img.Displacement( 1 );
      // size_t adj_position = ( adj_index << 1 ) + adj_index; // adj_position = adj_index * 3
      // div_t div_xydim = std::div( static_cast< int >( position ), static_cast< int >( xydim ) );
      // size_t x = static_cast< size_t >( ( position % dim[ 0 ] ) + relation[ adj_position ] );
      // size_t y = static_cast< size_t >( ( div_xydim.rem / dim[ 0 ] ) + relation[ adj_position + 1 ] );
      // size_t z = static_cast< size_t >( div_xydim.quot + relation[ adj_position + 2 ] );
      // COMMENT( "Checking if coordinates are valid.", 5 );
      // if( ( x >= dim[ 0 ] ) || ( y >= dim[ 1 ] ) || ( z >= dim[ 2 ] ) )
      //   return( dim.size( ) );
      // else
      //   return( img.Position( x, y, z ) );
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

  /* Explicit instantiations. --------------------------------------------------------------------------------------- */

  template Adjacency::Adjacency( const Matrix< int > &mat );
  template Adjacency::Adjacency( const Image< int > &img );
  template Vector< size_t > Adjacency::operator()( const Matrix< int > &mat, const Vector< size_t > &index,
                                                   size_t adj_index ) const;
  template  size_t Adjacency::operator()( const Matrix< int > &mat, size_t position, size_t adj_index ) const;
  template Vector< size_t > Adjacency::operator()( const Image< int > &img, const Vector< size_t > &index,
                                                   size_t adj_index ) const;
  template size_t Adjacency::operator()( const Image< int > &img, size_t position, size_t adj_index ) const;
  template Vector< size_t > Adjacency::Adj2( const Matrix< int > &mat, const Vector< size_t > &index,
                                             size_t adj_index ) const;
  template size_t Adjacency::Adj2( const Matrix< int > &mat, size_t position, size_t adj_index ) const;
  template Vector< size_t > Adjacency::Adj2( const Image< int > &img, const Vector< size_t > &index,
                                             size_t adj_index ) const;
  template size_t Adjacency::Adj2( const Image< int > &img, size_t position, size_t adj_index ) const;
  template Vector< size_t > Adjacency::Adj3( const Matrix< int > &mat, const Vector< size_t > &index,
                                             size_t adj_index ) const;
  template size_t Adjacency::Adj3( const Matrix< int > &mat, size_t position, size_t adj_index ) const;
  template Vector< size_t > Adjacency::Adj3( const Image< int > &img, const Vector< size_t > &index,
                                             size_t adj_index ) const;
  template size_t Adjacency::Adj3( const Image< int > &img, size_t position, size_t adj_index ) const;

  template Adjacency::Adjacency( const Matrix< llint > &mat );
  template Adjacency::Adjacency( const Image< llint > &img );
  template Vector< size_t > Adjacency::operator()( const Matrix< llint > &mat, const Vector< size_t > &index,
                                                   size_t adj_index ) const;
  template  size_t Adjacency::operator()( const Matrix< llint > &mat, size_t position, size_t adj_index ) const;
  template Vector< size_t > Adjacency::operator()( const Image< llint > &img, const Vector< size_t > &index,
                                                   size_t adj_index ) const;
  template size_t Adjacency::operator()( const Image< llint > &img, size_t position, size_t adj_index ) const;
  template Vector< size_t > Adjacency::Adj2( const Matrix< llint > &mat, const Vector< size_t > &index,
                                             size_t adj_index ) const;
  template size_t Adjacency::Adj2( const Matrix< llint > &mat, size_t position, size_t adj_index ) const;
  template Vector< size_t > Adjacency::Adj2( const Image< llint > &img, const Vector< size_t > &index,
                                             size_t adj_index ) const;
  template size_t Adjacency::Adj2( const Image< llint > &img, size_t position, size_t adj_index ) const;
  template Vector< size_t > Adjacency::Adj3( const Matrix< llint > &mat, const Vector< size_t > &index,
                                             size_t adj_index ) const;
  template size_t Adjacency::Adj3( const Matrix< llint > &mat, size_t position, size_t adj_index ) const;
  template Vector< size_t > Adjacency::Adj3( const Image< llint > &img, const Vector< size_t > &index,
                                             size_t adj_index ) const;
  template size_t Adjacency::Adj3( const Image< llint > &img, size_t position, size_t adj_index ) const;

  template Adjacency::Adjacency( const Matrix< float > &mat );
  template Adjacency::Adjacency( const Image< float > &img );
  template Vector< size_t > Adjacency::operator()( const Matrix< float > &mat, const Vector< size_t > &index,
                                                   size_t adj_index ) const;
  template  size_t Adjacency::operator()( const Matrix< float > &mat, size_t position, size_t adj_index ) const;
  template Vector< size_t > Adjacency::operator()( const Image< float > &img, const Vector< size_t > &index,
                                                   size_t adj_index ) const;
  template size_t Adjacency::operator()( const Image< float > &img, size_t position, size_t adj_index ) const;
  template Vector< size_t > Adjacency::Adj2( const Matrix< float > &mat, const Vector< size_t > &index,
                                             size_t adj_index ) const;
  template size_t Adjacency::Adj2( const Matrix< float > &mat, size_t position, size_t adj_index ) const;
  template Vector< size_t > Adjacency::Adj2( const Image< float > &img, const Vector< size_t > &index,
                                             size_t adj_index ) const;
  template size_t Adjacency::Adj2( const Image< float > &img, size_t position, size_t adj_index ) const;
  template Vector< size_t > Adjacency::Adj3( const Matrix< float > &mat, const Vector< size_t > &index,
                                             size_t adj_index ) const;
  template size_t Adjacency::Adj3( const Matrix< float > &mat, size_t position, size_t adj_index ) const;
  template Vector< size_t > Adjacency::Adj3( const Image< float > &img, const Vector< size_t > &index,
                                             size_t adj_index ) const;
  template size_t Adjacency::Adj3( const Image< float > &img, size_t position, size_t adj_index ) const;

  template Adjacency::Adjacency( const Matrix< double > &mat );
  template Adjacency::Adjacency( const Image< double > &img );
  template Vector< size_t > Adjacency::operator()( const Matrix< double > &mat, const Vector< size_t > &index,
                                                   size_t adj_index ) const;
  template  size_t Adjacency::operator()( const Matrix< double > &mat, size_t position, size_t adj_index ) const;
  template Vector< size_t > Adjacency::operator()( const Image< double > &img, const Vector< size_t > &index,
                                                   size_t adj_index ) const;
  template size_t Adjacency::operator()( const Image< double > &img, size_t position, size_t adj_index ) const;
  template Vector< size_t > Adjacency::Adj2( const Matrix< double > &mat, const Vector< size_t > &index,
                                             size_t adj_index ) const;
  template size_t Adjacency::Adj2( const Matrix< double > &mat, size_t position, size_t adj_index ) const;
  template Vector< size_t > Adjacency::Adj2( const Image< double > &img, const Vector< size_t > &index,
                                             size_t adj_index ) const;
  template size_t Adjacency::Adj2( const Image< double > &img, size_t position, size_t adj_index ) const;
  template Vector< size_t > Adjacency::Adj3( const Matrix< double > &mat, const Vector< size_t > &index,
                                             size_t adj_index ) const;
  template size_t Adjacency::Adj3( const Matrix< double > &mat, size_t position, size_t adj_index ) const;
  template Vector< size_t > Adjacency::Adj3( const Image< double > &img, const Vector< size_t > &index,
                                             size_t adj_index ) const;
  template size_t Adjacency::Adj3( const Image< double > &img, size_t position, size_t adj_index ) const;

  template Vector< size_t > Adjacency::operator()( const Image< Color > &img, const Vector< size_t > &index,
                                                   size_t adj_index ) const;
  template size_t Adjacency::operator()( const Image< Color > &img, size_t position, size_t adj_index ) const;
  template Vector< size_t > Adjacency::Adj2( const Image< Color > &img, const Vector< size_t > &index,
                                             size_t adj_index ) const;
  template size_t Adjacency::Adj2( const Image< Color > &img, size_t position, size_t adj_index ) const;
  template Vector< size_t > Adjacency::Adj3( const Image< Color > &img, const Vector< size_t > &index,
                                             size_t adj_index ) const;
  template size_t Adjacency::Adj3( const Image< Color > &img, size_t position, size_t adj_index ) const;

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
