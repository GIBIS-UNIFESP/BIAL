/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Nov/19 
 * @brief Kernel iterator class.
 */

#include "Kernel.hpp"
#include "Vector.hpp"

#ifndef BIALKERNELITERATOR_H
#define BIALKERNELITERATOR_H

namespace Bial {

  template< class D >
  class Image;
  template< class D >
  class Matrix;
  
  class KernelIterator {

  private:
    const Kernel &kernel;
    Vector< size_t > data_size;
    size_t data_limit;
    size_t pixel_index;
    size_t krn_index;
    size_t position;

  public:

    /**
     * @date 2013/Nov/19 
     * @param krn: A kernel. 
     * @param dim: Matrix or image dimensions. 
     * @param index: Index of reference pixel in the kernel. 
     * @return none. 
     * @brief Basic Constructor.
     * @warning none. 
     */
    KernelIterator( const Kernel &krn, const Vector< size_t > &dim, size_t index );

    /**
     * @date 2013/Nov/19 
     * @param krn: A kernel. 
     * @param mat: A matrix. 
     * @param index: Index of reference pixel in the kernel. 
     * @return none. 
     * @brief Basic Constructor.
     * @warning none. 
     */
    template< class D > 
    KernelIterator( const Kernel &krn, const Matrix< D > &mat, size_t index );

    /**
     * @date 2014/Apr/18 
     * @param krn: A kernel. 
     * @param img: An image. 
     * @param index: Index of reference pixel in the kernel. 
     * @return none. 
     * @brief Basic Constructor.
     * @warning none. 
     */
    template< class D > 
    KernelIterator( const Kernel &krn, const Image< D > &img, size_t index );

  private:

    /**
     * @date 2013/Nov/19 
     * @param none 
     * @return none. 
     * @brief Sets position to first valid index.
     * @warning none. 
     */
    void begin( );

    /**
     * @date 2013/Nov/19 
     * @param none 
     * @return none. 
     * @brief Sets position to kernel.size( ). 
     * @warning none. 
     */
    void end( );

  public:

    /**
     * @date 2013/Nov/19 
     * @param none. 
     * @return Reference to this. 
     * @brief Sets position to the next valid element or to
     * space size if finished. 
     * @warning none. 
     */
    KernelIterator &operator++( );

    /**
     * @date 2013/Nov/19 
     * @param none. 
     * @return Copy to this, moved to next valid element. 
     * @brief Sets position to the next valid element or to space size if finished. 
     * @warning none. 
     */
    KernelIterator operator++( int );

    /**
     * @date 2013/Nov/19 
     * @param cmp: iterator to which this iterator is compared. 
     * @return True, if this position is the same as cmp position. 
     * @brief Checks if this position is the same as cmp position. 
     * @warning none. 
     */
    bool operator==( const KernelIterator &cmp ) const;

    /**
     * @date 2013/Nov/19 
     * @param cmp: iterator to which this iterator is compared. 
     * @return True, if this position is different of cmp position. 
     * @brief Checks if this position is different of cmp position. 
     * @warning none. 
     */
    bool operator!=( const KernelIterator &cmp ) const;

    /**
     * @date 2013/Nov/19 
     * @param none. 
     * @return Ponter to current element. 
     * @brief Returns a ponter to current element. 
     * @warning Does not verify if corrent element is valid. 
     */
    size_t operator*( ) const;

    /**
     * @date 2013/Nov/19 
     * @param none. 
     * @return Coefficient of current element. 
     * @brief Returns the coefficient of current element. 
     * @warning Does not verify if corrent element is valid. 
     */
    float Value( ) const;

    friend class Kernel;

  };

}

#include "KernelIterator.cpp"

#endif
