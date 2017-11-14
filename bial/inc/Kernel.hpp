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
     * @param size: number of elements of the kernel. 
     * @param dims: number of dimensions of the kernel. 
     * @return none. 
     * @brief Basic Constructor.
     * @warning Uninitialized kernel is created. 
     */
    Kernel( size_t size, size_t dims );

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
     * @return Reference to kernel coefficient.
     * @brief Returns the coefficient value of index idx.
     * @warning none.
     */
    const float &Value( size_t idx ) const;

    /**
     * @date 2013/Nov/19 
     * @param idx: Kernel element index. 
     * @return Reference to kernel coefficient.
     * @brief Returns the coefficient value of index idx.
     * @warning none.
     */
    float &Value( size_t idx );

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
