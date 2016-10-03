/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Aug/27 
 * @brief Bit wise opperations over integer numbers.
 */

#ifndef BIALBIT_H
#define BIALBIT_H

#include "Common.hpp"

namespace Bial {
  /** @brief Gray code based functions. */
  namespace Bit {

    /**
     * @date 2015/Apr/10 
     * @param num: A gray code number. 
     * @param bit: A bit to be inverted [0, size -1]. 
     * @return Input number with bit inverted 
     * @brief Returns the input number with the expected bit inverted. 
     * @warning none. 
     */
    template< class D >
    D Invert( D num, size_t bit );

    /**
     * @date 2015/Apr/10 
     * @param num: A gray code number. 
     * @param bit: A bit to be set [0, size -1]. 
     * @param val: 0 or 1. 
     * @return Input number with bit set. 
     * @brief Returns the input number with the expected bit set to val. 
     * @warning none. 
     */
    template< class D >
    D Set( D num, size_t bit, char val );

    /**
     * @date 2015/Apr/10 
     * @param num: A gray code number. 
     * @param bit: A bit to be preset [0, size -1]. 
     * @return Input number with preset bit 
     * @brief Returns the input number with preset bit. 
     * @warning none. 
     */
    template< class D >
    D Preset( D num, size_t bit );

    /**
     * @date 2015/Apr/10 
     * @param num: A gray code number. 
     * @param bit: A bit to be reset [0, size -1]. 
     * @return Input number with reset bit. 
     * @brief Returns the input number with reset bit. 
     * @warning none. 
     */
    template< class D >
    D Reset( D num, size_t bit );

    /**
     * @date 2015/Apr/10 
     * @param num: A gray code number. 
     * @param bit: A bit to be set [0, size -1]. 
     * @return Input number with bit fliped. 
     * @brief Returns the input number with the expected bit fliped. 
     * @warning none. 
     */
    template< class D >
    D Flip( D num, size_t bit );

    /**
     * @date 2015/Apr/10 
     * @param num: A gray code number. 
     * @param bit: A bit to be checked [0, size -1]. 
     * @return true if bit is 1. 
     * @brief Returns true if bit is 1. 
     * @warning none. 
     */
    template< class D >
    bool IsOne( D num, size_t bit );

    /**
     * @date 2015/Apr/10 
     * @param num: A gray code number. 
     * @param bit: A bit to be checked [0, size -1]. 
     * @return true if bit is 0. 
     * @brief Returns true if bit is 0. 
     * @warning none. 
     */
    template< class D >
    bool IsZero( D num, size_t bit );

    /**
     * @date 2015/Apr/10 
     * @param num: A gray code number. 
     * @param bit: A bit to be checked [0, size -1]. 
     * @return bit value. 
     * @brief Returns bit value. 
     * @warning none. 
     */
    template< class D >
    D Value( D num, size_t bit );

  }

}

#include "Bit.cpp"

#endif
