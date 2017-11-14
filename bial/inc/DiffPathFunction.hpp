/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Sep/19
 * @brief Diff path function to compute the initial path-values and propagated path-values in IFT algorithm.
 *        Used by live-wire.
 */

#ifndef BIALDIFFPATHFUNCTION_H
#define BIALDIFFPATHFUNCTION_H

#include "Common.hpp"
#include "PathFunction.hpp"

namespace Bial {

  enum class BucketState : char;

  template< template< class D > class C, class D >
  class DiffPathFunction : public PathFunction< C, D > {

  protected:

    /**
     * @brief  Handicap container (Vector, Matrix, Image, etc).
     */
    C< D > handicap;

  public:

    /**
     * @date 2015/Apr/14
     * @param init_value: Reference for initial value container.
     * @param init_label: Reference for initial label container.
     * @param init_predecessor: Reference for predecessor container.
     * @param sequential_label: Sets labeling sequentially.
     * @param handicap: Handicap value used to initialize cost.
     * @return none.
     * @brief Basic constructor.
     * @warning none.
     */
    DiffPathFunction( C< D > &init_value, C< int > *init_label, C< int > *init_predecessor, bool sequential_label,
                      const C< D > &handicap );

    /**
     * @date 2015/Apr/14
     * @param pf: path-function to be copied.
     * @return none.
     * @brief Copy constructor.
     * @warning none.
     */
    DiffPathFunction( const DiffPathFunction< C, D > &pf );

    /**
     * @date 2015/Apr/14
     * @param none.
     * @return none.
     * @brief Destructor.
     * @warning none.
     */
    ~DiffPathFunction( );

    /**
     * @date 2015/Apr/14
     * @param pf: path-function to be assigned.
     * @return none.
     * @brief Assignment operator.
     * @warning none.
     */
    DiffPathFunction< C, D > operator=( const DiffPathFunction< C, D > &pf );

    /**
     * @date 2015/Apr/14
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
     * @date 2015/Apr/14
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
     * @date 2015/Apr/14
     * @param source: Source pixel index.
     * @param target: Adjacent pixel index.
     * @return True if path-value is propagated.
     * @brief Updates adjacent pixel values and returns true if path_function is propagated.
     * @warning none.
     */
    bool Propagate( size_t index, size_t adj_index, size_t adj_pos );

    /**
     * @date 2015/Apr/14
     * @param none.
     * @return True for increasing and false for decreasing values.
     * @brief Returns whether this path function uses increasing or decreasing values.
     * @warning none.
     */
    bool Increasing( );

  };

}

#include "DiffPathFunction.cpp"

#endif
