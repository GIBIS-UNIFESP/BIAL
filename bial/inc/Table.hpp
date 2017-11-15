/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Jun/21
 * @brief Table class. 2 dimensional table handeling. When acessed by (x, y) operator, index x corresponds to
 * the index of the column, and index y corresponds to the index of the row. This standard comes from text printing
 * order and image display and storage in most of the computer systems.
 */

#include "Common.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"

#ifndef BIALTABLE_H
#define BIALTABLE_H

namespace Bial {

  enum class TableCellType : char {
    INT = 0,
      FLT = 1,
      STR = 2,
      };
  /**
   * @brief TableCell is the data type that composes the Table class cells.
   */
  class TableCell {
  private:
    /** @brief Indicates the type of the column. */
    TableCellType type;
    /** @brief int column */
    int _int;
    /** @brief float column */
    float _flt;
    /** @brief string column */
    std::string _str;
  public:
    /**
     * @date 2015/May/25
     * @param none.
     * @return none.
     * @brief Basic Constructor.
     * @warning none.
     */
    TableCell( );
    /**
     * @date 2015/May/25
     * @param data: data to be inserted into the cell.
     * @return none.
     * @brief Basic Constructor.
     * @warning none.
     */
    TableCell( int data );
    /**
     * @date 2015/May/25
     * @param data: data to be inserted into the cell.
     * @return none.
     * @brief Basic Constructor.
     * @warning none.
     */
    TableCell( float data );
    /**
     * @date 2015/May/25
     * @param data: data to be inserted into the cell.
     * @return none.
     * @brief Basic Constructor.
     * @warning none.
     */
    TableCell( std::string data );
    /**
     * @date 2015/May/25
     * @param other: cell to be copied.
     * @return none.
     * @brief Copy Constructors.
     * @warning none.
     */
    TableCell( const TableCell &other );
    /**
     * @date 2015/May/25
     * @param none.
     * @return none.
     * @brief Basic Destructor.
     * @warning none.
     */
    ~TableCell( ) {
    }
    /**
     * @date 2015/May/25
     * @param none.
     * @return The current data type from this cell.
     * @brief Returns the current data type of this cell.
     * @warning none.
     */
    TableCellType Type( ) const;
    /**
     * @date 2015/May/25
     * @param data: data to be inserted into the cell.
     * @return none.
     * @brief assignment operator.
     * @warning none.
     */
    void operator=( int data );
    /**
     * @date 2015/May/25
     * @param data: data to be inserted into the cell.
     * @return none.
     * @brief assignment operator.
     * @warning none.
     */
    void operator=( float data );
    /**
     * @date 2015/May/25
     * @param data: data to be inserted into the cell.
     * @return none.
     * @brief assignment operator.
     * @warning none.
     */
    void operator=( std::string data );
    /**
     * @date 2015/May/25
     * @param none.
     * @return Value of internal cell data.
     * @brief Returns the value contained in this cell.
     * @warning none.
     */
    int Int( ) const;
    /**
     * @date 2015/May/25
     * @param none.
     * @return Value of internal cell data.
     * @brief Returns the value contained in this cell.
     * @warning none.
     */
    float Flt( ) const;
    /**
     * @date 2015/May/25
     * @param none.
     * @return Value of internal cell data.
     * @brief Returns the value contained in this cell.
     * @warning none.
     */
    std::string Str( ) const;

  };

  class TableColumn {
  private:
    /** @brief data in column. */
    Vector< TableCell > _data;

  public:
    /**
     * @date 2015/May/22
     * @param rows: number of rows for an non-initialized column.
     * @return none.
     * @brief Basic Constructor.
     * @warning null table.
     */
    TableColumn( size_t rows );
    /**
     * @date 2015/May/22
     * @param data: data to be inserted into the column.
     * @return none.
     * @brief Basic Constructor.
     * @warning null table.
     */
    TableColumn( const Vector< int > &data );
    /**
     * @date 2015/May/22
     * @param data: data to be inserted into the column.
     * @return none.
     * @brief Basic Constructor.
     * @warning null table.
     */
    TableColumn( const Vector< float > &data );
    /**
     * @date 2015/May/22
     * @param data: data to be inserted into the column.
     * @return none.
     * @brief Basic Constructor.
     * @warning null table.
     */
    TableColumn( const Vector< std::string > &data );

    /**
     * @date 2015/May/22
     * @param p: a global index to be acessed.
     * @return Constant reference of data pointed by index p.
     * @brief Returns a constant reference of data pointed by index p.
     * @warning Table bounds are not verified.
     */
    const TableCell &operator[]( size_t p ) const;

    /**
     * @date 2015/May/22
     * @param p: a index to be acessed.
     * @return Reference of data pointed by index p.
     * @brief Returns the reference for data pointed by index p.
     * @warning Table bounds are not verified.
     */
    TableCell &operator[]( size_t p );

    /**
     * @date 2015/May/22
     * @param none.
     * @return Number of rows.
     * @brief Returns the number of rows.
     * @warning none.
     */
    size_t size( ) const;

  };

  class Table {

  protected:

    /*
     * For each table column data is stored in one of the following three vectors bellow. For the unused vectors, an
     * empty position is created. All columns mus have the same length.
     */
    Vector< TableColumn > _data;

  public:

    /**
     * @date 2015/May/22
     * @param none.
     * @return none.
     * @brief Basic Constructor.
     * @warning null table.
     */
    Table( );

    /**
     * @date 2015/May/22
     * @param mtx: Source maitrix to be copied.
     * @param col_name: set first column as name.
     * @param row_name: set first row as name.
     * @return none.
     * @brief Constructor from matrix. Each columns is converted to the table with appropriated type.
     * @warning Must be a matrix of numbers integers or floating points.
     */
    Table( const Matrix< int > &mtx, bool col_name = false, bool row_name = false );
    /**
     * @date 2015/May/22
     * @param mtx: Source maitrix to be copied.
     * @param col_name: set first column as name.
     * @param row_name: set first row as name.
     * @return none.
     * @brief Constructor from matrix. Each columns is converted to the table with appropriated type.
     * @warning Must be a matrix of numbers integers or floating points.
     */
    Table( const Matrix< float > &mtx, bool col_name = false, bool row_name = false );
    /**
     * @date 2015/May/22
     * @param mtx: Source maitrix to be copied.
     * @param col_name: set first column as name.
     * @param row_name: set first row as name.
     * @return none.
     * @brief Constructor from matrix. Each columns is converted to the table with appropriated type.
     * @warning Must be a matrix of numbers integers or floating points.
     */
    Table( const Matrix< std::string > &mtx, bool col_name = false, bool row_name = false );

    /**
     * @date 2015/May/22
     * @param vct: Vector to be inserted into the end of the table.
     * @return none.
     * @brief Pushes back a vector to the end of the list.
     * @warning If there is already any column in table, the new one must have the same length.
     */
    void PushBack( const Vector< int > &vct );
    /**
     * @date 2015/May/22
     * @param vct: Vector to be inserted into the end of the table.
     * @return none.
     * @brief Pushes back a vector to the end of the list.
     * @warning If there is already any column in table, the new one must have the same length.
     */
    void PushBack( const Vector< float > &vct );
    /**
     * @date 2015/May/22
     * @param vct: Vector to be inserted into the end of the table.
     * @return none.
     * @brief Pushes back a vector to the end of the list.
     * @warning If there is already any column in table, the new one must have the same length.
     */
    void PushBack( const Vector< std::string > &vct );
    /**
     * @date 2015/May/22
     * @param vct: Vector to be inserted into the end of the table.
     * @return none.
     * @brief Pushes back a vector to the end of the list.
     * @warning If there is already any column in table, the new one must have the same length.
     */
    void PushBack( const TableColumn &vct );
    /**
     * @date 2015/May/22
     * @param p: a global index to be acessed.
     * @return Constant reference of data pointed by index p.
     * @brief Returns a constant reference of data pointed by index p. It has the same effect as operator
     * data( p ).
     * @warning Table bounds are not verified.
     */
    const TableColumn &operator[]( size_t p ) const;

    /**
     * @date 2015/May/22
     * @param p: a global index to be acessed.
     * @return Reference of data pointed by index p.
     * @brief Returns the reference for data pointed by index p. It has the same effect as operator data( p ).
     * @warning Table bounds are not verified.
     */
    TableColumn &operator[]( size_t p );

    /**
     * @date 2015/May/22
     * @param p: a global index to be acessed.
     * @return Constant reference of data pointed by index p.
     * @brief Returns a constant reference of data pointed by index p. It has the same effect as operator
     * data( p ).
     * @warning Table bounds are not verified.
     */
    const TableColumn &operator()( size_t p ) const;

    /**
     * @date 2015/May/22
     * @param p: a global index to be acessed.
     * @return Reference of data pointed by index p.
     * @brief Returns the reference for data pointed by index p. It has the same effect as operator data( p ).
     * @warning Table bounds are not verified.
     */
    TableColumn &operator()( size_t p );

    /**
     * @date 2015/May/22
     * @param col: column to be acessed.
     * @param row: row to be acessed.
     * @return Constant reference of data pointed by indexes col, row.
     * @brief Returns a constant reference of data pointed by indexes col, row.
     * @warning Table bounds are not verified.
     */
    const TableCell &operator()( size_t col, size_t row ) const;

    /**
     * @date 2015/May/22
     * @param col: column to be acessed.
     * @param row: row to be acessed.
     * @return Reference of data pointed by indexes col, row.
     * @brief Returns the reference for data pointed by indexes col, row.
     * @warning Table bounds are not verified.
     */
    TableCell &operator()( size_t col, size_t row );

    /**
     * @date 2015/May/22
     * @param none.
     * @return the number of rows in table.
     * @brief Returns the number of rows in table.
     * @warning none.
     */
    size_t Rows( ) const;

    /**
     * @date 2015/May/22
     * @param none.
     * @return the number of columns in table.
     * @brief Returns the number of columns in table.
     * @warning none.
     */
    size_t Columns( ) const;

    /**
     * @date 2015/May/28
     * @param filename: Name of the file to write table in.
     * @return none.
     * @brief Writes table containts into a file.
     * @warning none.
     */
    void Write( std::string filename ) const;

    /**
     * @date 2015/May/22
     * @param os: an output stream.
     * @param transpose: if the table should be printed transposed or not.
     * @return The output stream.
     * @brief Prints table containts to output stream os.
     * @warning 'this' table must be 1D, 2D, or 3D.
     */
    template< class O >
    O &Print( O &os, bool transpose = false ) const;

    /**
     * @date 2015/May/22
     * @param none.
     * @return The dimensions of the table.
     * @brief Prints table dimensions to output stream os.
     * @warning none.
     */
    template< class O >
    O &PrintDimensions( O &os ) const;
  };

  /* @param os: output stream or file.
   * @param tbl: a table.
   * @return Reference to os.
   * @brief Prints the table in a friendly way.
   * @warning none.
   *
   * template< typename D > O& operator<<( O& os, const Table< D >& tbl );
   */
  template< class O >
  O &operator<<( O &os, const Table &tbl ) {
    tbl.Print( os );
    return( os );
  }

}

#include "Table.cpp"

#endif
