/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Sep/19
 * @brief Max Path function to compute the initial path-values and propagated path-values in IFT algorithm.
 *        Functions used by Watershed transform.
 */

#ifndef BIALMAXPATHFUNCTION_H
#define BIALMAXPATHFUNCTION_H

#include "Common.hpp"
#include "PathFunction.hpp"

namespace Bial {

  enum class BucketState : char;

  template< template< class D > class C, class D >
  class MaxPathFunction : public PathFunction< C, D > {

  protected:

    /** @brief  Handicap container (Vector, Matrix, Image, etc). */
    C< D > handicap;

  public:

    /**
     * @date 2013/Jul/02
     * @param init_value: Reference for initial value container.
     * @param init_label: Reference for initial label container.
     * @param init_predecessor: Reference for predecessor container.
     * @param sequential_label: Sets labeling sequentially.
     * @param handicap: initial value to paths.
     * @return none.
     * @brief Basic constructor.
     * @warning none.
     */
    MaxPathFunction( C< D > &init_value, C< int > *init_label, C< int > *init_predecessor, bool sequential_label, 
                     const C< D > &handicap );

    /**
     * @date 2013/Jul/01
     * @param pf: path-function to be copied.
     * @return none.
     * @brief Copy constructor.
     * @warning none.
     */
    MaxPathFunction( const MaxPathFunction< C, D > &pf );
    
    /**
     * @date 2013/Jul/01
     * @param none.
     * @return none.
     * @brief Destructor.
     * @warning none.
     */
    ~MaxPathFunction( );

    /**
     * @date 2013/Jul/01
     * @param pf: path-function to be assigned.
     * @return none.
     * @brief Assignment operator.
     * @warning none.
     */
    MaxPathFunction< C, D > operator=( const MaxPathFunction< C, D > &pf );

    /**
     * @date 2012/Sep/25
     * @param index: The index of the pixel to be initalized.
     * @return Whether this node can propagate or not.
     * @brief Sets initial value for root pixel of index 'index'.
     * @warning none.
     */
    bool RemoveSimple( size_t index, BucketState state );

    /**
     * @date 2014/Dec/05
     * @param index: The index of the pixel to be initalized.
     * @return Whether this node can propagate or not.
     * @brief Sets initial value for root pixel of index 'index'. Also sets its predecessor value.
     * @warning none.
     */
    bool RemovePredecessor( size_t index, BucketState state );

    /**
     * @date 2014/Dec/05
     * @param index: The index of the pixel to be initalized.
     * @return Whether this node can propagate or not.
     * @brief Sets initial value for root pixel of index 'index'. Also sets its label value.
     * @warning none.
     */
    bool RemoveLabel( size_t index, BucketState state );

    /**
     * @date 2012/Sep/25
     * @param index: The index of the pixel to be initalized.
     * @return Whether this node can propagate or not.
     * @brief Sets initial value for root pixel of index 'index'. Also sets its predecessor and label value.
     * @warning none.
     */
    bool RemoveComplete( size_t index, BucketState state );

    /**
     * @date 2013/Oct/14
     * @param source: Source pixel index.
     * @param target: Adjacent pixel index.
     * @return True if it is possible to propagate.
     * @brief Checks if index can offer better value to adj_index, prior to computing it by Propagate
     * function. This function makes IFT faster.
     * @warning none.
     */
    bool Capable( size_t index, size_t adj_index, BucketState adj_state );

    /**
     * @date 2017/Jan/20
     * @param source: Source pixel index.
     * @param target: Adjacent pixel index.
     * @return True if path-value is propagated.
     * @brief Updates adjacent pixel values and returns true if path_function is propagated. This is used with
     *        differential IFT.
     * @warning none.
     */
    bool PropagateDifferential( size_t index, size_t adj_index, size_t adj_pos );

    /**
     * @date 2013/Jun/28
     * @param source: Source pixel index.
     * @param target: Adjacent pixel index.
     * @return True if path-value is propagated.
     * @brief Updates adjacent pixel values and returns true if path_function is propagated.
     * @warning none.
     */
    bool Propagate( size_t index, size_t adj_index, size_t adj_pos );

    /**
     * @date 2012/Sep/19
     * @param none.
     * @return True for increasing and false for decreasing values.
     * @brief Returns whether this path function uses increasing or decreasing values.
     * @warning none.
     */
    bool Increasing( );

    /**
     * @date 2016/Nov/17
     * @param index: Index of an element.
     * @return The best value that this element can achieve.
     * @brief Function used for IFT with backtracking. It usually returns the handicap of pathfunctions that contain
     *        this kind of map.
     * @warning none.
     */
    D BestValue( size_t index );

  };

}

#include "MaxPathFunction.cpp"

#endif
