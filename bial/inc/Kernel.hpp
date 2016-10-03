/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Nov/19 
 * @brief Kernel class.
 */

#ifndef BIALKERNEL_H
#define BIALKERNEL_H

#include "Adjacency.hpp"
#include "Common.hpp"
#include "Image.hpp"
#include "Matrix.hpp"

namespace Bial {

  class KernelIterator;
  /**
   * @brief  Kernel and forward iteratior related to a matrix. 
   */
  class Kernel : public Adjacency {

  private:

    Vector< float > coefficient;

  public:

    /**
     * @date 2013/Nov/19 
     * @param none 
     * @return none. 
     * @brief Basic Constructor.
     * @warning Uninitialized kernel is created. 
     */
    Kernel( );

    /**
     * @date 2013/Nov/19 
     * @param none 
     * @return none. 
     * @brief Basic Constructor.
     * @warning Uninitialized kernel is created. 
     */
    Kernel( const Adjacency &adj );

    /**
     * @date 2013/Nov/19 
     * @param dims: number of dimensions of the kernel. Should be greater or equal to the image that it
     * will operate on. 
     * @param size: number of elements of the kernel. 
     * @return none. 
     * @brief Basic Constructor.
     * @warning Uninitialized kernel is created. 
     */
    Kernel( size_t dims, size_t size );

    /**
     * @date 2013/Nov/19 
     * @param mat: A matrix. 
     * @return none. 
     * @brief Basic Constructor. Build a kernel based on a matrix. Non-zero elements are set as adjacents and
     * coefficients are matrix values. Kernel centralized in dim / 2. 
     * @warning Uninitialized kernel is created. 
     */
    template< class D > 
    Kernel( const Matrix< D > &mat );

    /**
     * @date 2014/Apr/18 
     * @param img: An image. 
     * @return none. 
     * @brief Basic Constructor. Build a kernel based on a image spatial dimensions. Non-zero elements are set as
     * adjacents and coefficients are image values. Kernel centralized in dim / 2. 
     * @warning Uninitialized kernel is created. 
     */
    template< class D > 
    Kernel( const Image< D > &img );

    /**
     * @date 2012/Sep/24 
     * @param mtx: A matrix for adjacency relation iteration. 
     * @param pixel_index: The pixel whose adjacency will be iterated. 
     * @return Iterator to first valid adjacent element. 
     * @brief returns an iterator to first valid adjacent element. 
     * @warning none. 
     */
    template< class D >
    KernelIterator begin( const Matrix< D > &mtx, size_t pixel_index ) const;

    /**
     * @date 2012/Sep/24 
     * @param mtx: A matrix for adjacency relation iteration. 
     * @param pixel_index: The pixel whose adjacency will be iterated. 
     * @return Iterator to position after the end of elements. 
     * @brief Returns an iterator to position after the end of elements.
     * @warning none. 
     */
    template< class D >
    KernelIterator end( const Matrix< D > &mtx, size_t pixel_index ) const;

    /**
     * @date 2014/Apr/18 
     * @param img: An image for adjacency relation iteration. 
     * @param pixel_index: The pixel whose adjacency will be iterated. 
     * @return Iterator to first valid adjacent element. 
     * @brief returns an iterator to first valid adjacent element. 
     * @warning none. 
     */
    template< class D >
    KernelIterator begin( const Image< D > &img, size_t pixel_index ) const;

    /**
     * @date 2014/Apr/18 
     * @param img: An image for adjacency relation iteration. 
     * @param pixel_index: The pixel whose adjacency will be iterated. 
     * @return Iterator to position after the end of elements. 
     * @brief Returns an iterator to position after the end of elements.
     * @warning none. 
     */
    template< class D >
    KernelIterator end( const Image< D > &img, size_t pixel_index ) const;

    /**
     * @date 2013/Nov/22 
     * @param none. 
     * @return none. 
     * @brief Normalizes the sum of the indexes of this kernel. 
     * @warning none. 
     */
    void Normalize( );

    /**
     * @date 2013/Nov/22 
     * @param none. 
     * @return Normalized a kernel. 
     * @brief Returns a normalized kernel of this kernel. 
     * @warning none. 
     */
    Kernel Normalize( ) const;

    /**
     * @date 2013/Nov/22 
     * @param none. 
     * @return none. 
     * @brief Normalizes positive and negative coefficients of this kernel separately. 
     * @warning none. 
     */
    void SignalNormalize( );

    /**
     * @date 2013/Nov/22 
     * @param none. 
     * @return Signal normalized a kernel. 
     * @brief Returns a kernel with positive and negative coefficients normalized separately. 
     * @warning none. 
     */
    Kernel SignalNormalize( ) const;

    /**
     * @date 2013/Nov/19 
     * @param idx: Kernel element index. 
     * @return The Coefficient value of index idx. 
     * @brief Returns the coefficient value of index idx.
     * @warning idx is not verified for adjacency bounds. 
     */
    float Value( size_t idx ) const;

    /**
     * @date 2013/Nov/19 
     * @param idx: Kernel element index. 
     * @param value: Coefficient value. 
     * @return none. 
     * @brief Assigns value to the coefficient of index idx.
     * @warning idx is not verified for adjacency bounds. 
     */
    void Value( size_t idx, float value );

    /**
     * @date 2013/Nov/19 
     * @param os: an output stream. 
     * @return os output stream. 
     * @brief Prints kernel dimensions to output stream os. 
     * @warning none. 
     */
    template< class O >
    O &PrintDimensions( O &os ) const;

    /**
     * @date 2015/Mar/26 
     * @param os: an output stream. 
     * @return The output stream. 
     * @brief Prints kernel to output stream os. 
     * @warning none. 
     */
    template< class O >
    O &Print( O &os ) const;
  };

  /* @param os: output stream. 
   * @param knl: a kernel. 
   * @return Reference to os. 
   * @brief Prints the kernel relation in a friendly way. 
   * @warning none. 
   */
  template< class O >
  O &operator<<( O &os, const Kernel &knl );

}

#include "Kernel.cpp"

#endif
