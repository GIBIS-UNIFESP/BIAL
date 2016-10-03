/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2014/Apr/07
 * @brief Image class declaration. Handles 2D and 3D images.
 */

#include "Array.hpp"
#include "Common.hpp"
#include "Matrix.hpp"
#include "PNMHeader.hpp"
#include "Vector.hpp"

#ifndef BIALIMAGE_H
#define BIALIMAGE_H

namespace Bial {

  class DicomHeader;
  template< class D >
  class Image;

  /**
   * @brief The Image class is used to handle N-Dimensional images, and has support to Read and Write image files.
   */
  template< class D = int >
  class Image {

    template< class D2 >
    friend class Image;

  protected:
    /**
     * @brief All pixel's information here. Dimension order: space -> color.
     */
    Matrix< D > _data;
    /**
     * @brief Quick access reference for data.
     */
    D *qk_data;
    /**
     * @brief Dimensions of the pixels (dpi). Its size is the number of dimensions.
     */
    Vector< float > pixel_size;
    /**
     * @brief Pointer to qk_data element in the case of 2D image access by coordinates.
     */
    D **y_table;
    /**
     * @brief Pointer to y_table element in the case of 3D image access by coordinates.
     */
    D ***z_table;

    /**
     * @date 2016/Oct/30
     * @param none.
     * @return none.
     * @brief Creates y_table and z_table with correct references to qk_data.
     * @warning none
     */
    void CreateTables( );

    /**
     * @date 2016/Oct/30
     * @param none.
     * @return none.
     * @brief Destroys y_table and z_table.
     * @warning none
     */
    void DestroyTables( );

  public:
    /**
     * @date 2014/Oct/16
     * @param none.
     * @return none.
     * @brief Basic Constructor.
     * @warning 2D image with 1 pixel.
     */
    Image( );

    /**
     * @date 2013/Jun/21
     * @param spc_dim: Vector with image dimensions.
     * @return none.
     * @brief Basic Constructor. Two to three dimensions.
     * @warning Zero is assigned to all elements. Pixel dimensions are set to 1.0.
     */
    Image( const Vector< size_t > &spc_dim );

    /**
     * @date 2015/Set/10
     * @param new_data: Pointer to data allocated elsewhere.
     * @param new_dim: Image dimensions.
     * @return none.
     * @brief Basic Constructor. Two to four dimensions. This constructor works as a wrapper for data already 
     * allocated. It will use the same space pointed by new_data to manipulate the image. Use this for compatibility
     * with other data structures, in a easy convertion to Image class.
     * @warning Data is not deallocated automatcally, since it may be stored in the stack. Also, DO NOT USE this 
     * constructure in Verbose or Debug compilation mode, as it will try to access invalid memory positions. Just
     * compile without any Verbosity or Debug flags.
     */
    Image( D *new_data, const Vector< size_t > &new_dim );

    /**
     * @date 2013/Jun/21
     * @param spc_dim: Vector with image dimensions. Must be two or three.
     *@param pixel_size: array with pixel dimensions. Must be two or three.
     * @return none.
     * @brief Basic Constructor.
     * @warning Zero is assigned to all elements.
     */
    Image( const Vector< size_t > &spc_dim, const Vector< float > &pixel_size );

    /**
     * @date 2012/Jul/02
     * @param spc_dim_0: image dimension 0.
     * @param spc_dim_1: image dimension 1.
     * @return none.
     * @brief Basic Constructor for 2D image.
     * @warning Zero is assigned to all elements. Pixel dimensions are set to 1.0.
     */
    Image( size_t spc_dim_0, size_t spc_dim_1 );

    /**
     * @date 2013/Aug/03
     * @param spc_dim_0: image dimension 0.
     * @param spc_dim_1: image dimension 1.
     * @param spc_dim_2: image dimension 2.
     * @return none.
     * @brief Basic Constructor for 3D image.
     * @warning Zero is assigned to all elements. Pixel dimensions are set to 1.0.
     */
    Image( size_t spc_dim_0, size_t spc_dim_1, size_t spc_dim_2 );

    /**
     * @date 2013/Aug/03
     * @param mtx: Data matrix.
     * @return none.
     * @brief Basic Constructor for 3D image.
     * @warning Zero is assigned to all elements. Pixel dimensions are set to 1.0.
     */
    Image( const Matrix< D > &mtx );

    /**
     * @date 2013/Aug/03
     * @param mtx: Data matrix.
     * @param pixel_size: Dimensions of the pixels.
     * @return none.
     * @brief Basic Constructor for 3D image.
     * @warning Zero is assigned to all elements. Pixel dimensions are set to 1.0.
     */
    Image( const Matrix< D > &mtx, const Vector< float > &pixel_size );

    /**
     * @date 2013/Jun/27
     * @param img: Source image to be clonned.
     * @return none.
     * @brief Copy constructor.
     * @warning none.
     */
    template< class D2 >
    Image( const Image< D2 > &img );

    /**
     * @date 2013/Aug/07
     * @version 1.0.00
     * @param filename: File name.
     * @return none.
     * @brief Copy Constructor.
     * @warning none.
     */
    Image( const Image< D > &img );

    /**
     * @date 2014/Oct/12
     * @version 1.0.00
     * @param img: Other image.
     * @return none.
     * @brief Move Constructor.
     * @warning none.
     */
    Image( Image< D > &&img );

    /*
     * @date 2014/Oct/12
     * @param none.
     * @return none.
     * @brief Basic destructor.
     * @warning none.
     */
    ~Image( ) {
      COMMENT( "Destroy image.", 3 );
      DestroyTables( );
    }
    
    /**
     * @date 2014/Oct/11
     * @param other: Image to be assigned to this.
     * @return none.
     * @brief Assigns 'other'image to this image.
     * @warning none.
     */
    Image< D > &operator=( const Image< D > &other );

    /**
     * @date 2015/Feb/27
     * @param other: Image to be assigned to this.
     * @return none.
     * @brief Assigns 'other'image to this image, moving the content.
     * @warning none.
     */
    Image< D > &operator=( Image< D > &&other );

    /**
     * @date 2015/Oct/05
     * @param scalar: Scalar to be assigned.
     * @return Referente to this image.
     * @brief Assigns scalar to all image pixels.
     * @warning none.
     */
    Image< D > &operator=( D scalar );

    /**
     * @date 2014/Oct/10
     * @param other: Image to be assigned to this.
     * @return none.
     * @brief Assigns 'other'image to this image.
     * @warning none.
     */
    template< class D2 >
    Image< D > &operator=( const Image< D2 > &other );

    /**
     * @date 2014/Apr/08
     * @param p: a global index to be acessed.
     * @return Constant reference of data pointed by index p.
     * @brief Returns a constant reference of data pointed by index p. Same as operator()( p ).
     * @warning Image bounds are not verified.
     */
    const D &operator[]( size_t p ) const;

    /**
     * @date 2014/Apr/08
     * @param p: a global index to be acessed.
     * @return Reference of data pointed by index p.
     * @brief Returns the reference for data pointed by index p. Same as operator()( p ).
     * @warning Image bounds are not verified.
     */
    D &operator[]( size_t p );

    /**
     * @date 2012/Jun/21
     * @param p: a global index to be acessed.
     * @return Constant reference of data pointed by index p.
     * @brief Returns constant reference of data pointed by index p. It has the same effect as operator
     * data[ p ].
     * @warning Image bounds are not verified.
     */
    const D &operator()( size_t p ) const;

    /**
     * @date 2012/Jun/21
     * @param p: a global index to be acessed.
     * @return Reference of data pointed by index p.
     * @brief Returns the reference for data pointed by index p. It has the same effect as operator data[ p ].
     * @warning Image bounds are not verified.
     */
    D &operator()( size_t p );

    /**
     * @date 2012/Jun/21
     * @param p_0: first index to be acessed.
     * @param p_1: second index to be acessed.
     * @return Constant reference of data pointed by index ( p_0, p_1 ).
     * @brief Returns constant reference of data pointed by index ( p_0, p_1 ). It has the same effect as operator
     * data[ p_0 + p_1 * dim_size[ 0 ] ].
     * @warning Image dimensions and bounds are not verified. Designed for 2D matrices. The behavior is undefined for
     * other dimensions.
     */
    const D &operator()( size_t p_0, size_t p_1 ) const;

    /**
     * @date 2012/Jun/21
     * @param p_0: first index to be acessed.
     * @param p_1: second index to be acessed.
     * @return Reference of data pointed by index ( p_0, p_1 ).
     * @brief Returns the reference for data pointed by index ( p_0, p_1 ). It has the same effect as operator
     * data[ p_0 + p_1 * dim_size[ 0 ] ].
     * @warning Image dimensions and bounds are not verified. Designed for 2D images. The behavior is undefined for
     * other dimensions.
     */
    D &operator()( size_t p_0, size_t p_1 );

    /**
     * @date 2012/Jun/21
     * @param p_0: first index to be acessed.
     * @param p_1: second index to be acessed.
     * @param p_2: third index to be acessed.
     * @return Constant reference of data pointed by index ( p_0, p_1, p_2 ).
     * @brief Returns constant reference of data pointed by index ( p_0, p_1, p_2 ). It has the same effect as
     * operator data[ p_0 + p_1 * dim_size[ 0 ] + p_2 * dim_size[ 0 ] * dim_size[ 1 ] ].
     * @warning Image dimensions and bounds are not verified. Designed for 3D images. The behavior is undefined for
     * other dimensions.
     */
    const D &operator()( size_t p_0, size_t p_1, size_t p_2 ) const;

    /**
     * @date 2012/Jun/21
     * @param p_0: first index to be acessed.
     * @param p_1: second index to be acessed.
     * @param p_2: third index to be acessed.
     * @return Reference of data pointed by index ( p_0, p_1, p_2 ).
     * @brief Returns the reference for data pointed by index ( p_0, p_1 ). It has the same effect as operator
     * data[ p_0 + p_1 * dim_size[ 0 ] + p_2 * dim_size[ 0 ] * dim_size[ 1 ] ].
     * @warning Image dimensions and bounds are not verified. Designed for 3D images. The behavior is undefined for
     * other dimensions.
     */
    D &operator()( size_t p_0, size_t p_1, size_t p_2 );

    /**
     * @date 2013/Jun/25
     * @param p: Vector of indexes to be acessed.
     * @return Constant reference of data pointed by index Vector p.
     * @brief Returns constant reference of data pointed by index ( p[ 0 ], p[ 1 ], [ p[ 2 ] ] ). It has the same
     * effect as operator data[ p_0 + p_1 * dim_size[ 0 ] + [ p_2 * dim_size[ 0 ] * dim_size[ 1 ] ] ].
     * @warning Image dimensions and input vector bounds are not verified. Designed for 2D or 3D images.
     */
    const D &operator()( const Vector< size_t > &p ) const;

    /**
     * @date 2013/Jun/25
     * @param p: Vector of indexes to be acessed.
     * @return Reference of data pointed by index Vector p.
     * @brief Returns a reference of data pointed by index ( p[ 0 ], p[ 1 ], [ p[ 2 ] ] ). It has the same effect
     * as operator data[ p_0 + p_1 * dim_size[ 0 ] + [ p_2 * dim_size[ 0 ] * dim_size[ 1 ] ] ].
     * @warning Image dimensions and input vector bounds are not verified. Designed for 2D or 3D images.
     */
    D &operator()( const Vector< size_t > &p );

    /**
     * @date 2014/Apr/10
     * @param none.
     * @return iterator begin() of data.
     * @brief Returns iterator begin() of data.
     * @warning none.
     */
    typename std::vector< D >::iterator begin( ) noexcept;

    /**
     * @date 2014/Apr/10
     * @param none.
     * @return const_iterator begin() of data Vector.
     * @brief Returns const_iterator begin() of data Vector.
     * @warning none.
     */
    typename std::vector< D >::const_iterator begin( ) const noexcept;

    /**
     * @date 2014/Apr/10
     * @param none.
     * @return iterator end() of data.
     * @brief Returns iterator end() of data.
     * @warning none.
     */
    typename std::vector< D >::iterator end( ) noexcept;

    /**
     * @date 2014/Apr/10
     * @param none.
     * @return const_iterator end() of data.
     * @brief Returns const_iterator end() of data.
     * @warning none.
     */
    typename std::vector< D >::const_iterator end( ) const noexcept;

    /**
     * @date 2014/Apr/10
     * @param none.
     * @return iterator rbegin() of data Vector.
     * @brief Returns the iterator rbegin() of data Vector.
     * @warning none.
     */
    typename std::vector< D >::reverse_iterator rbegin( ) noexcept;

    /**
     * @date 2014/Apr/10
     * @param none.
     * @return const_iterator rbegin() of data Vector.
     * @brief Returns const_iterator rbegin() of data Vector.
     * @warning none.
     */
    typename std::vector< D >::const_reverse_iterator rbegin( ) const noexcept;

    /**
     * @date 2014/Apr/10
     * @param none.
     * @return iterator rend() of data Vector.
     * @brief Returns iterator rend() of data Vector.
     * @warning none.
     */
    typename std::vector< D >::reverse_iterator rend( ) noexcept;

    /**
     * @date 2014/Apr/10
     * @param none.
     * @return const_iterator rend() of data Vector.
     * @brief Returns const_iterator rend() of data Vector.
     * @warning none.
     */
    typename std::vector< D >::const_reverse_iterator rend( ) const noexcept;

    /**
     * @date 2015/Apr/06
     * @param none.
     * @return iterator cbegin() of data Vector.
     * @brief Returns the iterator cbegin() of data Vector.
     * @warning none.
     */
    typename std::vector< D >::const_iterator cbegin( ) const noexcept;

    /**
     * @date 2015/Apr/06
     * @param none.
     * @return iterator cend() of data Vector.
     * @brief Returns the iterator cend() of data Vector.
     * @warning none.
     */
    typename std::vector< D >::const_iterator cend( ) const noexcept;

    /**
     * @date 2015/Apr/06
     * @param none.
     * @return iterator crbegin() of data Vector.
     * @brief Returns the iterator crbegin() of data Vector.
     * @warning none.
     */
    typename std::vector< D >::const_reverse_iterator crbegin( ) const noexcept;

    /**
     * @date 2015/Apr/06
     * @param none.
     * @return iterator crend() of data Vector.
     * @brief Returns the iterator crend() of data Vector.
     * @warning none.
     */
    typename std::vector< D >::const_reverse_iterator crend( ) const noexcept;

    /**
     * @date 2014/Apr/10
     * @param none.
     * @return A pointer to data in Matrix.
     * @brief Returns a pointer to data in Matrix.
     * @warning none.
     */
    const Matrix< D > &DataMatrix( ) const;

    /**
     * @date 2015/Feb/25
     * @param none.
     * @return A pointer to data in Matrix.
     * @brief Returns a pointer to data in Matrix.
     * @warning none.
     */
    Matrix< D > &DataMatrix( );

    /**
     * @date 2015/Mar/11
     * @param none.
     * @return A pointer to data in Matrix.
     * @brief Returns a pointer to data in Matrix.
     * @warning none.
     */
    const D* Data( ) const;

    /**
     * @date 2015/Mar/11
     * @param none.
     * @return A pointer to data in Matrix.
     * @brief Returns a pointer to data in Matrix.
     * @warning none.
     */
    D* Data( );

    /**
     * @date 2015/Mar/11
     * @param none.
     * @return A pointer to data in Matrix.
     * @brief Returns a pointer to data in Matrix.
     * @warning none.
     */
    const D* data( ) const;

    /**
     * @date 2015/Mar/11
     * @param none.
     * @return A pointer to data in Matrix.
     * @brief Returns a pointer to data in Matrix.
     * @warning none.
     */
    D* data( );

    /**
     * @date 2014/Apr/10
     * @param none.
     * @return The image size.
     * @brief Returns the image size.
     * @warning none.
     */
    size_t Size( ) const;

    /**
     * @date 2014/Apr/10
     * @param none.
     * @return The image size.
     * @brief Returns the image size. Same as Size( ), just for compatibility with std containers.
     * @warning none.
     */
    size_t size( ) const;

    /**
     * @date 2014/Apr/10
     * @param position: Dimension index.
     * @param dimension: An image dimension.
     * @return The coordinate of position in given dimension.
     * @brief Returns the coordinate of position in given dimension.
     * @warning Dimensions are not checked.
     */
    size_t Coordinate( size_t position, size_t dimension ) const;

    /**
     * @date 2014/Apr/10
     * @param index: element position.
     * @return The coordinates of the input position in this image.
     * @brief Returns the coordinates of the input position in this image.
     * @warning Dimensions are not checked.
     */
    Vector< size_t > Coordinates( size_t position ) const;

    /**
     * @date 2014/Apr/10
     * @param p_0: dim 0 coordinate.
     * @param p_1: dim 1 coordinate.
     * @return The position corresponding to the input coordinates in this image.
     * @brief Returns the position corresponding to the input coordinates in this image.
     * @warning none.
     */
    size_t Position( size_t p_0, size_t p_1 ) const;

    /**
     * @date 2014/Apr/10
     * @param p_0: dim 0 coordinate.
     * @param p_1: dim 1 coordinate.
     * @param p_2: dim 2 coordinate.
     * @return The position corresponding to the input coordinates in this image.
     * @brief Returns the position corresponding to the input coordinates in this image.
     * @warning none.
     */
    size_t Position( size_t p_0, size_t p_1, size_t p_2 ) const;

    /**
     * @date 2014/Apr/10
     * @param coordinates: element coordinates.
     * @return The position corresponding to the input coordinates in this image.
     * @brief Returns the position corresponding to the input coordinates in this image.
     * @warning none.
     */
    size_t Position( const Vector< size_t > &coordinates ) const;

    /**
     * @date 2015/Jul/07
     * @param coordinates: element coordinates.
     * @return The position corresponding to the input coordinates in this image.
     * @brief Returns the position corresponding to the input coordinates in this image.
     * @warning none.
     */
    size_t Position( const Vector< int > &coordinates ) const;

    /**
     * @date 2015/Jul/07
     * @param coordinates: element coordinates.
     * @return The position corresponding to the input coordinates in this image.
     * @brief Returns the position corresponding to the input coordinates in this image.
     * @warning none.
     */
    size_t Position( const Vector< float > &coordinates ) const;

    /**
     * @date 2014/Apr/10
     * @param val: value to be set to the entire image.
     * @return none.
     * @brief Sets val to each element of the entire image.
     * @warning none.
     */
    void Set( D val );

    /**
     * @date 2014/Apr/10
     * @param none.
     * @return A Vector with image dimensions.
     * @brief Returns a Vector with image dimensions.
     * @warning It is a copy of the dimension Vector.
     */
    Vector< size_t > Dim( ) const;

    /**
     * @date 2014/Apr/10
     * @param none.
     * @return Minimum size among dimensions.
     * @brief Returns the minimum size among dimensions.
     * @warning none.
     */
    size_t MinDim( ) const;

    /**
     * @date 2014/Apr/10
     * @param none.
     * @return Maximum size among dimensions.
     * @brief Returns the maximum size among dimensions.
     * @warning none.
     */
    size_t MaxDim( ) const;

    /**
     * @date 2014/Apr/10
     * @param dms: The required dimension.
     * @return Image dimension dms.
     * @brief Returns image dimension dms.
     * @warning none.
     */
    size_t Size( size_t dms ) const;

    /**
     * @date 2014/Apr/10
     * @param dms: The required dimension.
     * @return Image dimension dms.
     * @brief Returns image dimension dms. Just for symmetry with size( ) and Size( ) functions.
     * @warning none.
     */
    size_t size( size_t dms ) const;

    /**
     * @date 2014/Apr/10
     * @param none.
     * @return The number of dimensions of the image.
     * @brief Returns the number of dimensions of the image.
     * @warning none.
     */
    size_t Dims( ) const;

    /**
     * @date 2014/Apr/10
     * @param dms: The required dimension.
     * @return Position displacement of one unit in dimension dms.
     * @brief Returns the position displacement of one unit in dimension dms.
     * @warning none.
     */
    size_t Displacement( size_t dms ) const;

    /**
     * @date 2014/Apr/10
     * @param p: global index of the data being accessed.
     * @return Constant reverence of value contained in position p of data Vector.
     * @brief Reads the constant reverence of value contained at position p of data Vector.
     * @warning Works for matrices of any dimension. Index p is a global position.
     */
    const D &at( size_t p ) const;

    /**
     * @date 2014/Apr/10
     * @param p: global index of the data being accessed.
     * @return Reference to position p of data Vector.
     * @brief Reads the value contained at position p of data Vector and returns its reference.
     * @warning Works for matrices of any dimension. Index p is a global position.
     */
    D & at( size_t p );

    /**
     * @date 2014/Apr/10
     * @param p_0: first index of the data being accessed.
     * @param p_1: second index of the data being accessed.
     * @return Constant reverence of value contained in position p_0, p_1 of data Vector.
     * @brief Reads the constant reverence of value contained at position p_0, p_1 of data Vector.
     * @warning 2D image access.
     */
    const D &at( size_t p_0, size_t p_1 ) const;

    /**
     * @date 2014/Apr/10
     * @param p_0: first index of the data being accessed.
     * @param p_1: second index of the data being accessed.
     * @return Reference to position p_0, p_1 of data Vector.
     * @brief Reads the value contained at position p_0, p_1 of data Vector and returns its reference.
     * @warning 2D image access.
     */
    D &at( size_t p_0, size_t p_1 );

    /**
     * @date 2014/Apr/10
     * @param p_0: first index of the data being accessed.
     * @param p_1: second index of the data being accessed.
     * @param p_2: third index of the data being accessed.
     * @return Constant reverence of value contained in position p_0, p_1, p_2 of data Vector.
     * @brief Reads the constant reverence of value contained at position p_0, p_1, p_2 of data Vector.
     * @warning 3D image access.
     */
    const D &at( size_t p_0, size_t p_1, size_t p_2 ) const;

    /**
     * @date 2014/Apr/10
     * @param p_0: first index of the data being accessed.
     * @param p_1: second index of the data being accessed.
     * @param p_2: third index of the data being accessed.
     * @return Reference to position p_0, p_1, p_2 of data Vector.
     * @brief Reads the value contained at position p_0, p_1, p_2 of data Vector and returns its
     * reference.
     * @warning 3D image access.
     */
    D &at( size_t p_0, size_t p_1, size_t p_2 );

    /**
     * @date 2014/Apr/10
     * @param p_0: first index of the data being accessed.
     * @param p_1: second index of the data being accessed.
     * @param p_2: third index of the data being accessed.
     * @param p_3: fourth index of the data being accessed.
     * @return Constant reverence of value contained in position p_0, p_1, p_2, p_3 of data Vector.
     * @brief Reads the constant reverence of value contained at position p_0, p_1, p_2, p_3 of data
     * Vector.
     * @warning 4D image access.
     */
    const D &at( size_t p_0, size_t p_1, size_t p_2, size_t p_3 ) const;

    /**
     * @date 2014/Apr/10
     * @param p_0: first index of the data being accessed.
     * @param p_1: second index of the data being accessed.
     * @param p_2: third index of the data being accessed.
     * @param p_3: fourth index of the data being accessed.
     * @return Reference to position p_0, p_1, p_2, p_3 of data Vector.
     * @brief Reads the value contained at position p_0, p_1, p_2, p_3 of data Vector and returns its
     * reference.
     * @warning 4D image access.
     */
    D &at( size_t p_0, size_t p_1, size_t p_2, size_t p_3 );

    /**
     * @date 2014/Apr/10
     * @param p: Vector of indexes to be acessed.
     * @return Constant reference of data pointed by index Vector p.
     * @brief Returns constant reference of data pointed by index ( p[ 0 ], p[ 1 ], p[ 2 ], ... ). It has the same
     * effect as operator data[ p_0 + p_1 * acc_dim_size[ 0 ] + p_2 * acc_dim_size[ 1 ] ... ].
     * @warning none.
     */
    const D &at( const Vector< size_t > &p ) const;

    /**
     * @date 2014/Apr/10
     * @param p: Vector of indexes to be acessed.
     * @return Constant reference of data pointed by index Vector p.
     * @brief Returns constant reference of data pointed by index ( p[ 0 ], p[ 1 ], p[ 2 ], ... ). It has the same
     * effect as operator data[ p_0 + p_1 * acc_dim_size[ 0 ] + p_2 * acc_dim_size[ 1 ] ... ].
     * @warning none.
     */
    D &at( const Vector< size_t > &p );

    /* Image functions */

    /**
     * @date 2012/Jun/21
     * @param none.
     * @return Pixel dimensions.
     * @brief Returns a Vector with all pixel dimensions.
     * @warning It is a copy of the pixel dimension vector.
     */
    Vector< float > PixelSize( ) const;

    /**
     * @date 2013/Aug/09
     * @param val: new pixel dimensions.
     * @return none.
     * @brief Update pixel dimensions.
     * @warning none.
     */
    void PixelSize( const Vector< float > &val );

    /**
     * @date 2012/Jun/21
     * @param dms: A dimension.
     * @return Pixel dimension dms.
     * @brief Returns pixel dimension dms.
     * @warning none.
     */
    float PixelSize( size_t dms ) const;

    /**
     * @date 2013/Aug/09
     * @param dms: The required dimension.
     * @param val: Dimension value.
     * @return none.
     * @brief Assigns val to pixel dimension dms.
     * @warning none.
     */
    void PixelSize( size_t dms, float val );

    /**
     * @date 2013/Sep/26
     * @param pxl: Coordinates of a pixel.
     * @return True if coordinates are in image domain.
     * @brief Verifies if a given pixel coordinate is in image domain.
     * @warning Does not verify pxl and image dimensions match.
     */
    bool ValidCoordinate( const Vector< size_t > &pxl ) const;

    /**
     * @date 2014/Nov/06
     * @param x, y: Coordinates of a pixel.
     * @return True if coordinates are in image domain.
     * @brief Verifies if a given pixel coordinate is in image domain.
     * @warning Does not verify pxl and image dimensions match.
     */
    bool ValidCoordinate( size_t x, size_t y ) const;

    /**
     * @date 2014/Nov/06
     * @param x, y, z: Coordinates of a pixel.
     * @return True if coordinates are in image domain.
     * @brief Verifies if a given pixel coordinate is in image
     * domain.
     * @warning Does not verify pxl and image dimensions match.
     */
    bool ValidCoordinate( size_t x, size_t y, size_t z ) const;

    /**
     * @date 2014/Apr/23
     * @param min: minimum value of the new intensity scale
     *        max: maximum value of the new intensity scale
     * @return none
     * @brief Recalculate the intensity scale of the data by the following formula:
     *        img[ pxl ] = min + ( img[ pxl ] - img_min ) * ( max - min ) / ( img_max - img_min )
     * @warning none.
     */
    template< class D2 >
    void SetRange( D2 min, D2 max );

    /**
     * @date 2013/Jul/16 
     * @param none.
     * @return Minimum value in Image.
     * @brief Returns the minimum value in Image.
     * @warning none.
     */
    D Minimum( ) const;

    /**
     * @date 2013/Jul/16
     * @param none.
     * @return Maximum value in Image.
     * @brief Returns the maximum value in Image.
     * @warning none.
     */
    D Maximum( ) const;

    /**
     * @date 2012/Jun/21
     * @param none.
     * @return os: an output stream.
     * @brief Prints image dimensions to output stream os.
     * @warning none.
     */
    template< class O >
    void PrintPixelSize( O &os ) const;

    /**
     * @date 2014/Apr/14
     * @param os: an output stream.
     * @return none.
     * @brief Prints image containts to output stream os.
     * @warning none.
     */
    template< class O >
    void Print( O &os ) const;

    /**
     * @date 2014/Apr/22
     * @param os: an output stream.
     * @return none.
     * @brief Prints image dimensions to output stream os.
     * @warning none.
     */
    template< class O >
    void PrintDimensions( O &os ) const;    
  };

  /**
   * @param os: output stream.
   *             img: an image.
   * @return Reference to os.
   * @brief Prints the image in a friendly way.
   * @warning none.
   */
  template< class D, class O >
  O &operator<<( O &os, const Image< D > &img );

  /**
   * @date 2012/Jul/02
   * @param img1, img2: Input images.
   * @return Sum the input images element by element.
   * @brief Computes the sum of the images and returns the result.
   * @warning Images must have the same dimensions.
   */
  template< class D >
  Image< D > operator+( const Image< D > &img1, const Image< D > &img2 );

  /**
   * @date 2012/Jul/02
   * @param img1, img2: Input images.
   * @return Reference to img1.
   * @brief Computes the sum of the images, assigns the result to img1, and returns the result.
   * @warning Images must have the same dimensions.
   */
  template< class D >
  Image< D > &operator+=( Image< D > &img1, const Image< D > &img2 );

  /**
   * @date 2012/Jul/02
   * @param img1, img2: Input images.
   * @return Difference of the input images element by element.
   * @brief Computes the difference of the images and returns the result.
   * @warning Images must have the same dimensions.
   */
  template< class D >
  Image< D > operator-( const Image< D > &img1, const Image< D > &img2 );

  /**
   * @date 2012/Jul/02
   * @param other: Other image.
   * @return Reference to img1.
   * @brief Computes the difference between input images, assigns the result to img1, and returns the result.
   * @warning Images must have the same dimensions.
   */
  template< class D >
  Image< D > &operator-=( Image< D > &img1, const Image< D > &img2 );

  /**
   * @date 2012/Jul/02
   * @param img1, img2: Input images.
   * @return Product of the input images element by element.
   * @brief Computes the product of the images and returns the result.
   * @warning Images must have the same dimensions.
   */
  template< class D >
  Image< D > operator*( const Image< D > &img1, const Image< D > &img2 );

  /**
   * @date 2012/Jul/02
   * @param other: Other image.
   * @return Reference to img1.
   * @brief Computes the product between input images, assigns the result to img1, and returns the result.
   * @warning Images must have the same dimensions.
   */
  template< class D >
  Image< D > &operator*=( Image< D > &img1, const Image< D > &img2 );

  /**
   * @date 2012/Jul/02
   * @param img1, img2: Input images.
   * @return Quotient of the input images element by element.
   * @brief Computes the quotient of the images and returns the result.
   * @warning Images must have the same dimensions.
   */
  template< class D >
  Image< D > operator/( const Image< D > &img1, const Image< D > &img2 );

  /**
   * @date 2012/Jul/02
   * @param other: Other image.
   * @return Reference to img1.
   * @brief Computes the quotient between input images, assigns the result to img1, and returns the result.
   * @warning Images must have the same dimensions.
   */
  template< class D >
  Image< D > &operator/=( Image< D > &img1, const Image< D > &img2 );

  /**
   * @date 2012/Jul/02
   * @param scalar: a scalar.
   * @param img: Input image.
   * @return Sums scalar to input image pixel values.
   * @brief Computes the sum of the scalar to input image pixel values and returns the result.
   * @warning none.
   */
  template< class D >
  Image< D > operator+( const Image< D > &img, double scalar );
  template< class D >
  Image< D > operator+( double scalar, const Image< D > &img );

  /**
   * @date 2012/Jul/02
   * @param scalar: a scalar.
   * @param img: Input image.
   * @return Reference to img.
   * @brief Computes the sum of the scalar to input image pixel values and assigns the result to img.
   * @warning none.
   */
  template< class D >
  Image< D > &operator+=( Image< D > &img, double scalar );

  /**
   * @date 2012/Jul/02
   * @param scalar: a scalar.
   * @param img: Input image.
   * @return Subtracts scalar from input image pixel values, or vice-versa.
   * @brief Computes the differente between scalar and input image pixel values and returns the result.
   * @warning none.
   */
  template< class D >
  Image< D > operator-( const Image< D > &img, double scalar );
  template< class D >
  Image< D > operator-( double scalar, const Image< D > &img );

  /**
   * @date 2012/Jul/02
   * @param scalar: a scalar.
   * @param img: Input image.
   * @return Reference to img.
   * @brief Computes the difference between the scalar and input image pixel values and assigns the result to img.
   * @warning none.
   */
  template< class D >
  Image< D > &operator-=( Image< D > &img, double scalar );

  /**
   * @date 2012/Jul/02
   * @param scalar: a scalar.
   * @param img: Input image.
   * @return Multiplies the scalar to input image pixel values.
   * @brief Computes the product of the scalar and input image pixel values and returns the result.
   * @warning none.
   */
  template< class D >
  Image< D > operator*( const Image< D > &img, double scalar );
  template< class D >
  Image< D > operator*( double scalar, const Image< D > &img );

  /**
   * @date 2012/Jul/02
   * @param scalar: a scalar.
   * @param img: Input image.
   * @return Reference to img.
   * @brief Computes the product of the scalar and input image pixel values and assigns the result to img.
   * @warning none.
   */
  template< class D >
  Image< D > &operator*=( Image< D > &img, double scalar );

  /**
   * @date 2012/Jul/02
   * @param scalar: a scalar.
   * @param img: Input image.
   * @return Divides the scalar by the input image pixel values, or vice-versa.
   * @brief Computes the quotient between the scalar and the input image pixel values and returns the result.
   * @warning none.
   */
  template< class D >
  Image< D > operator/( const Image< D > &img, double scalar );
  template< class D >
  Image< D > operator/( double scalar, const Image< D > &img );

  /**
   * @date 2012/Jul/02
   * @param scalar: a scalar.
   * @param img: Input image.
   * @return Reference to img.
   * @brief Computes the quotient between the scalar and the input image pixel values and assigns the result to img.
   * @warning none.
   */
  template< class D >
  Image< D > &operator/=( Image< D > &img, double scalar );

  /**
   * @date 2013/Nov/06
   * @param img: input image.
   * @return Image whose pixel values are the absolute value of the input image pixel values.
   * @brief Computes the absolute value of image pixel values as return an image with the result.
   * @warning none.
   */
  template< class D >
  Image< D > Abs( const Image< D > &img );

}

/* Implementation ---------------------------------------------------------------------------------------------------- */

namespace Bial {

  template< class D > template< class O >
  void Image< D >::PrintPixelSize( O &os ) const {
    size_t dims = Dims( );
    for( size_t pxl = 0; pxl < dims; ++pxl )
      os << pixel_size[ pxl ] << " ";
    os << std::endl;
  }

  template< class D > template< class O >
  void Image< D >::Print( O &os ) const {
    size_t dims = Dims( );
    size_t dim_x = _data.dim_size( 0 );
    size_t dim_y = _data.dim_size( 1 );
    if( dims == 2 ) {
      COMMENT( "dims == 2.", 2 );
      for( size_t y = 0; y < dim_y; ++y ) {
        for( size_t x = 0; x < dim_x; ++x ) {
          os << _data( x, y );
          os << " ";
        }
        os << std::endl;
      }
    }
    else {
      COMMENT( "dims == 3.", 2 );
      size_t dim_z = _data.dim_size( 2 );
      for( size_t z = 0; z < dim_z; ++z ) {
        os << "( , , " << z << "):" << std::endl;
        for( size_t y = 0; y < dim_y; ++y ) {
          for( size_t x = 0; x < dim_x; ++x ) {
            os << _data( x, y, z );
            os << " ";
          }
          os << std::endl;
        }
        os << std::endl;
      }
    }
  }

  template< class D > template< class O >
  void Image< D >::PrintDimensions( O &os ) const {
    COMMENT( "Printing all dimensions.", 2 );
    size_t dims = Dims( );
    for( size_t dms = 0; dms < dims; ++dms )
      os << _data.dim_size( dms ) << " ";
    os << std::endl;
  }

  template< class D, class O >
  O &operator<<( O &os, const Image< D > &img ) {
    img.Print( os );
    return( os );
  }

}

#include "Image.cpp"

#endif
