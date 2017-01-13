/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Sep/19
 * @brief Max sum path function to compute the initial path-values and propagated path-values in IFT algorithm.
 *        This function is has a mixed behavior of maximum and sum path functions. It may adopt a direction.
 */

#ifndef BIALMAXSUMPATHFUNCTION_H
#define BIALMAXSUMPATHFUNCTION_H

#include "Common.hpp"
#include "PathFunction.hpp"

namespace Bial {

  enum class BucketState : char;

  template< template< class D > class C, class D >
  class MaxSumPathFunction : public PathFunction< C, D > {

  protected:
    /**
     * @brief Reference to source image.
     */
    const C< D > &intensity;

    /**
     * @brief Handicap container (Vector, Matrix, Image, etc).
     */
    C< D > handicap;

    /**
     * @brief Orientation weight. -1.0 to 1.0. (negative) for dark objects, (positive) for light objects, (zero)
     * for non-oriented edges. Default value is 0.0.
     */
    double alpha;

    /**
     * @brief Weight to control behavior between sum and max path functions. Lower: more similar to
     * sum. Higher: more similar to max. Default value = 0.5
     */
    double beta;

  public:

    /**
     * @date 2015/Jun/24
     * @param init_value: Reference for initial value container.
     * @param init_label: Reference for initial label container.
     * @param init_predecessor: Reference for predecessor container.
     * @param sequential_label: Sets labeling sequentially.
     * @param new_intensity: Source image intensity. Value image is a gradient.
     * @param new_alpha: Alpha weight to orient the edges.
     * @param new_beta: weight of the sum funtion. Closer to 0 is more similar to sum path function. Higher
     *                  values is closer to max path function.
     * @return none.
     * @brief Basic constructor.
     * @warning none.
     */
    MaxSumPathFunction( C< D > &init_value, C< int > *init_label, C< int > *init_predecessor, bool sequential_label,
                        const C< D > &new_intensity, const C< D > &handicap, double new_alpha = 0.0,
                        double new_beta = 0.5 );

    /**
     * @date 2015/Jun/24
     * @param pf: path-function to be copied.
     * @return none.
     * @brief Copy constructor.
     * @warning none.
     */
    MaxSumPathFunction( const MaxSumPathFunction< C, D > &pf );

    /**
     * @date 2015/Jun/24
     * @param none.
     * @return none.
     * @brief Destructor.
     * @warning none.
     */
    ~MaxSumPathFunction( );

    /**
     * @date 2015/Jun/24
     * @param pf: path-function to be assigned.
     * @return none.
     * @brief Assignment operator.
     * @warning none.
     */
    MaxSumPathFunction< C, D > operator=( const MaxSumPathFunction< C, D > &pf );

    /**
     * @date 2015/Jun/24
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
     * @date 2015/Jun/24
     * @param source: Source pixel index.
     * @param target: Adjacent pixel index.
     * @return True if it is possible to propagate.
     * @brief Checks if index can offer better value to adj_index, prior to computing it by Propagate
     * function. This function makes IFT faster.
     * @warning none.
     */
    bool Capable( int index, int adj_index, BucketState adj_state );

    /**
     * @date 2015/Jun/24
     * @param source: Source pixel index.
     * @param target: Adjacent pixel index.
     * @return True if path-value is propagated.
     * @brief Updates adjacent pixel values and returns true if path_function is propagated.
     * @warning none.
     */
    bool Propagate( int index, int adj_index );

    /**
     * @date 2015/Jun/24
     * @param none.
     * @return True for increasing and false for decreasing values.
     * @brief Returns whether this path function uses increasing or decreasing values.
     * @warning none.
     */
    bool Increasing( );

  };

}

#include "MaxSumPathFunction.cpp"

#endif
