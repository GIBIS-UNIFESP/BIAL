/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Jun/21
 * @brief Table class. 2 dimensional table handeling. When acessed by (x, y) operator, index x corresponds to
 * the index of the column, and index y corresponds to the index of the row. This standard comes from text printing
 * order and image display and storage in most of the computer systems.
 */

#ifndef BIALTABLE_C
#define BIALTABLE_C

#include "Table.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_Table )
#define BIAL_EXPLICIT_Table
#endif
#if defined ( BIAL_EXPLICIT_Table ) || ( BIAL_IMPLICIT_BIN )

namespace Bial {

  /* TebleCell *********************************************************************************************************/

  TableCell::TableCell( ) : type( TableCellType::INT ), _int( 0 ), _flt( 0.0f ), _str( ) {
  }

  TableCell::TableCell( int data ) : type( TableCellType::INT ), _int( data ), _flt( 0.0f ), _str( ) {
  }

  TableCell::TableCell( float data ) : type( TableCellType::FLT ), _int( 0 ), _flt( data ), _str( ) {
  }

  TableCell::TableCell( std::string data ) : type( TableCellType::STR ), _int( 0 ), _flt( 0.0f ), _str( data ) {
  }

  TableCell::TableCell( const TableCell &other ) : type( other.type ), _int( other._int ), _flt( other._flt ),
							  _str( other._str ) {
  }

  TableCellType TableCell::Type( ) const {
    return( type );
  }

  void TableCell::operator=( int data ) {
    type = TableCellType::INT;
    _int = data;
  }

  void TableCell::operator=( float data ) {
    type = TableCellType::FLT;
    _flt = data;
  }

  void TableCell::operator=( std::string data ) {
    type = TableCellType::STR;
    _str = data;
  }

  int TableCell::Int( ) const {
    if( type == TableCellType::INT ) {
      return( _int );
    }
    std::string msg( BIAL_ERROR( "Accessing int containt of an non-int cell." ) );
    throw( std::logic_error( msg ) );
  }

  float TableCell::Flt( ) const {
    if( type == TableCellType::FLT ) {
      return( _flt );
    }
    std::string msg( BIAL_ERROR( "Accessing float containt of an non-float cell." ) );
    throw( std::logic_error( msg ) );
  }

  std::string TableCell::Str( ) const {
    if( type == TableCellType::STR ) {
      return( _str );
    }
    std::string msg( BIAL_ERROR( "Accessing string containt of an non-string cell." ) );
    throw( std::logic_error( msg ) );
  }


  /* TableColumn *******************************************************************************************************/

  TableColumn::TableColumn( size_t rows ) try : _data( rows ) {
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

  TableColumn::TableColumn( const Vector< int > &data ) try : _data( data.size( ) ) {
    for( size_t row = 0; row < _data.size( ); ++row ) {
      _data[ row ] = data[ row ];
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

  TableColumn::TableColumn( const Vector< float > &data ) try : _data( data.size( ) ) {
    for( size_t row = 0; row < _data.size( ); ++row ) {
      _data[ row ] = data[ row ];
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

  TableColumn::TableColumn( const Vector< std::string > &data ) try : _data( data.size( ) ) {
    for( size_t row = 0; row < _data.size( ); ++row ) {
      _data[ row ] = data[ row ];
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

  const TableCell &TableColumn::operator[]( size_t p ) const {
#ifdef BIAL_DEBUG
    if( p < _data.size( ) ) {
      return( _data.at( p ) );
    }
    std::string msg( BIAL_ERROR( "Out of range element. Given: " + std::to_string( p ) + ", Expected: < " +
                                 std::to_string( _data.size( ) ) ) );
    throw( std::logic_error( msg ) );
#else
    return( _data[ p ] );
#endif
  }

  TableCell &TableColumn::operator[]( size_t p ) {
#ifdef BIAL_DEBUG
    if( p < _data.size( ) ) {
      return( _data.at( p ) );
    }
    std::string msg( BIAL_ERROR( "Out of range element. Given: " + std::to_string( p ) + ", Expected: < " +
                                 std::to_string( _data.size( ) ) ) );
    throw( std::logic_error( msg ) );
#else
    return( _data[ p ] );
#endif
  }

  size_t TableColumn::size( ) const {
    return( _data.size( ) );
  }


  /* Table *************************************************************************************************************/

  Table::Table( ) : _data( ) {
  }

  Table::Table( const Matrix< int > &mtx, bool col_name, bool row_name ) try : 
    _data( Vector< TableColumn >( mtx.size( 0 ) + ( col_name ? 0 : 1 ), 
                                  Vector < int >( mtx.size( 1 ) + ( row_name ? 0 : 1 ) ) ) ) {
      if( mtx.Dims( ) != 2 ) {
	std::string msg( BIAL_ERROR( "Input matrix to build table must be 2-dimensional." ) );
	throw( std::logic_error( msg ) );
      }
      if( !col_name ) {
	for( size_t x = 0; x < Columns( ); ++x ) {
	  _data[ x ][ 0 ] = std::to_string( x );
	}
      }
      if( !row_name ) {
	for( size_t y = 1; y < Rows( ); ++y ) {
	  _data[ 0 ][ y ] = std::to_string( y );
	}
      }
      for( size_t y = 0; y < mtx.size( 1 ); ++y ) {
	for( size_t x = 0; x < mtx.size( 0 ); ++x ) {
	  _data[ x + ( col_name ? 0 : 1 ) ][ y + ( row_name ? 0 : 1 ) ] = mtx( x, y );
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

  Table::Table( const Matrix< float > &mtx, bool col_name, bool row_name ) try : 
    _data( Vector< TableColumn >( mtx.size( 0 ) + ( col_name ? 0 : 1 ), 
                                  Vector< float >( mtx.size( 1 ) + ( row_name ? 0 : 1 ) ) ) ) {
      if( mtx.Dims( ) != 2 ) {
	std::string msg( BIAL_ERROR( "Input matrix to build table must be 2-dimensional." ) );
	throw( std::logic_error( msg ) );
      }
      if( !col_name ) {
	COMMENT( "label column.", 0 );
	for( size_t x = 0; x < Columns( ); ++x ) {
	  _data[ x ][ 0 ] = std::to_string( x );
	}
      }
      if( !row_name ) {
	COMMENT( "label row.", 0 );
	for( size_t y = 1; y < Rows( ); ++y ) {
	  _data[ 0 ][ y ] = std::to_string( y );
	}
      }
      COMMENT( "Creating data table.", 0 );
      for( size_t y = 0; y < mtx.size( 1 ); ++y ) {
	for( size_t x = 0; x < mtx.size( 0 ); ++x ) {
	  _data[ x + ( col_name ? 0 : 1 ) ][ y + ( row_name ? 0 : 1 ) ] = mtx( x, y );
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

  Table::Table( const Matrix< std::string > &mtx, bool col_name, bool row_name ) try :
    _data( Vector< TableColumn >( mtx.size( 0 ) + ( col_name ? 0 : 1 ), 
                                  Vector< int >( mtx.size( 1 ) + ( row_name ? 0 : 1 ) ) ) ) {
      if( mtx.Dims( ) != 2 ) {
	std::string msg( BIAL_ERROR( "Input matrix to build table must be 2-dimensional." ) );
	throw( std::logic_error( msg ) );
      }
      if( !col_name ) {
	for( size_t x = 0; x < Columns( ); ++x ) {
          COMMENT( "x: " << x, 0 );
	  _data[ x ][ 0 ] = std::to_string( x );
	}
      }
      if( !row_name ) {
	for( size_t y = 1; y < Rows( ); ++y ) {
          COMMENT( "y: " << y, 0 );
	  _data[ 0 ][ y ] = std::to_string( y );
	}
      }
      for( size_t y = 0; y < mtx.size( 1 ); ++y ) {
	for( size_t x = 0; x < mtx.size( 0 ); ++x ) {
          COMMENT( "x: " << x << ", y: " << y, 0 );
	  _data[ x + ( col_name ? 0 : 1 ) ][ y + ( row_name ? 0 : 1 ) ] = mtx( x, y );
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

  void Table::PushBack( const Vector< int > &vct ) {
    try {
      if( ( _data.size( ) == 0 ) || ( _data[ 0 ].size( ) == vct.size( ) ) ) {
        _data.push_back( vct );
      }
      else {
        std::string msg( BIAL_ERROR( "Input vector dimensions do not match table rows. Given: " +
                                     std::to_string( vct.size( ) ) + ", expected: " +
                                     ( _data.size( ) == 0 ? 0 : std::to_string( _data[ 0 ].size( ) ) ) ) );
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
  }

  void Table::PushBack( const Vector< float > &vct ) {
    try {
      if( ( _data.size( ) == 0 ) || ( _data[ 0 ].size( ) == vct.size( ) ) ) {
        COMMENT( "Inserting element.", 0 );
        _data.push_back( vct );
      }
      else {
        std::string msg( BIAL_ERROR( "Input vector dimensions do not match table rows. Given: " +
                                     std::to_string( vct.size( ) ) + ", expected: " +
                                     ( _data.size( ) == 0 ? 0 : std::to_string( _data[ 0 ].size( ) ) ) ) );
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
  }

  void Table::PushBack( const Vector< std::string > &vct ) {
    try {
      if( ( _data.size( ) == 0 ) || ( _data[ 0 ].size( ) == vct.size( ) ) ) {
        _data.push_back( vct );
      }
      else {
        std::string msg( BIAL_ERROR( "Input vector dimensions do not match table rows. Given: " +
                                     std::to_string( vct.size( ) ) + ", expected: " +
                                     ( _data.size( ) == 0 ? 0 : std::to_string( _data[ 0 ].size( ) ) ) ) );
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
  }

  void Table::PushBack( const TableColumn &vct ) {
    try {
      if( ( _data.size( ) == 0 ) || ( _data[ 0 ].size( ) == vct.size( ) ) ) {
        _data.push_back( vct );
      }
      else {
        std::string msg( BIAL_ERROR( "Input vector dimensions do not match table rows. Given: " +
                                     std::to_string( vct.size( ) ) + ", expected: " +
                                     ( _data.size( ) == 0 ? 0 : std::to_string( _data[ 0 ].size( ) ) ) ) );
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
  }

  const TableColumn &Table::operator[]( size_t p ) const {
#ifdef BIAL_DEBUG
    if( p < _data.size( ) ) {
      return( _data.at( p ) );
    }
    std::string msg( BIAL_ERROR( "Out of range element. Given: " + std::to_string( p ) + ", Expected: < " +
                                 std::to_string( _data.size( ) ) ) );
    throw( std::logic_error( msg ) );
#else
    return( _data[ p ] );
#endif
  }

  TableColumn &Table::operator[]( size_t p ) {
#ifdef BIAL_DEBUG
    if( p < _data.size( ) ) {
      return( _data.at( p ) );
    }
    std::string msg( BIAL_ERROR( "Out of range element. Given: " + std::to_string( p ) + ", Expected: < " +
                                 std::to_string( _data.size( ) ) ) );
    throw( std::logic_error( msg ) );
#else
    return( _data[ p ] );
#endif
  }

  const TableColumn &Table::operator()( size_t p ) const {
#ifdef BIAL_DEBUG
    if( p < _data.size( ) ) {
      return( _data.at( p ) );
    }
    std::string msg( BIAL_ERROR( "Out of range element. Given: " + std::to_string( p ) + ", Expected: < " +
                                 std::to_string( _data.size( ) ) ) );
    throw( std::logic_error( msg ) );
#else
    return( _data[ p ] );
#endif
  }

  TableColumn &Table::operator()( size_t p ) {
#ifdef BIAL_DEBUG
    if( p < _data.size( ) ) {
      return( _data.at( p ) );
    }
    std::string msg( BIAL_ERROR( "Out of range element. Given: " + std::to_string( p ) + ", Expected: < " +
                                 std::to_string( _data.size( ) ) ) );
    throw( std::logic_error( msg ) );
#else
    return( _data[ p ] );
#endif
  }

  const TableCell &Table::operator()( size_t col, size_t row ) const {
#ifdef BIAL_DEBUG
    if( ( col < _data.size( ) ) && ( row < _data[ col ].size( ) ) ) {
      return( _data[ col ][ row ] );
    }
    std::string msg( BIAL_ERROR( "Out of range element." ) );
    throw( std::logic_error( msg ) );
#else
    return( _data[ col ][ row ] );
#endif
  }

  TableCell &Table::operator()( size_t col, size_t row ) {
#ifdef BIAL_DEBUG
    if( ( col < _data.size( ) ) && ( row < _data[ col ].size( ) ) ) {
      return( _data[ col ][ row ] );
    }
    std::string msg( BIAL_ERROR( "Out of range element." ) );
    throw( std::logic_error( msg ) );
#else
    return( _data[ col ][ row ] );
#endif
  }

  size_t Table::Rows( ) const {
    if( _data.size( ) == 0 ) {
      return( 0 );
    }
    return( _data[ 0 ].size( ) );
  }

  size_t Table::Columns( ) const {
    return( _data.size( ) );
  }

  void Table::Write( std::string filename ) const {
    COMMENT( "Opening file.", 2 );
    OFile file;
    file.exceptions( std::fstream::failbit | std::fstream::badbit );
    file.open( filename );

    COMMENT( "Writing table.", 2 );
    file << *this;
    file.close( );
  }

  template< class O >
  O &Table::Print( O &os, bool transpose ) const {
    if( !transpose ) {
      for( size_t y = 0; y < Rows( ); ++y ) {
        for( size_t x = 0; x < Columns( ); ++x ) {
          switch( _data[ x ][ y ].Type( ) ) {
	  case TableCellType::INT: {
	    os << _data[ x ][ y ].Int( );
	    break;
	  }
	  case TableCellType::FLT: {
	    os << _data[ x ][ y ].Flt( );
	    break;
	  }
	  default: {
	    os << "\"" << _data[ x ][ y ].Str( ).c_str( ) << "\"";
	    break;
	  }
          }
          if( x != Columns( ) - 1 ) {
            os << ", ";
          }
        }
        if( y != Rows( ) - 1 ) {
          os << std::endl;
        }
      }
    }
    else {
      for( size_t x = 0; x < Columns( ); ++x ) {
        for( size_t y = 0; y < Rows( ); ++y ) {
          switch( _data[ x ][ y ].Type( ) ) {
	  case TableCellType::INT: {
	    os << _data[ x ][ y ].Int( );
	    break;
	  }
	  case TableCellType::FLT: {
	    os << _data[ x ][ y ].Flt( );
	    break;
	  }
	  default: {
	    os << "\"" << _data[ x ][ y ].Str( ).c_str( ) << "\"";
	    break;
	  }
          }
          if( y != Rows( ) - 1 ) {
            os << ", ";
          }
        }
        if( x != Columns( ) - 1 ) {
          os << std::endl;
        }
      }
    }
    return( os );
  }

  template< class O >
  O &Table::PrintDimensions( O &os ) const {
    os << Columns( ) << ", " << Rows( );
    return( os );
  }


#ifdef BIAL_EXPLICIT_Table

  template OFile &Table::Print( OFile &os, bool transpose ) const;
  template OFile &Table::PrintDimensions( OFile &os ) const;
  template std::ostream &Table::Print( std::ostream &os, bool transpose ) const;
  template std::ostream &Table::PrintDimensions( std::ostream &os ) const;

#endif

}

#endif

#endif

