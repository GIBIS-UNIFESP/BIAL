/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Sep/19
 * @brief Geodesic distance path function to compute the initial path-values and propagated path-values in IFT 
 *        algorithm. Used to compute the distance from a set of pixels to the rest of the image.
 */

#ifndef BIALGEODESICPATHFUNCTION_H
#define BIALGEODESICPATHFUNCTION_H

#include "Common.hpp"
#include "Image.hpp"
#include "PathFunction.hpp"

namespace Bial {

  enum class BucketState : char;

  template< class D >
  class GeodesicDistancePathFunction : public PathFunction< Image, D > {

  protected:

    /**
     * @brief handicap container (Vector, Matrix, Image, etc).
     */
    Image< D > handicap;
    /**
     * @brief auxiliar vector to compute distances faster.
     */
    Vector< double > dists;

  public:

    /**
     * @date 2014/Jan/08
     * @param init_value: Reference for initial value container.
     * @param init_label: Reference for initial label container.
     * @param init_predecessor: Reference for predecessor container.
     * @param sequential_label: Sets labeling sequentially.
     * @param handicap: Handicap value used to initiate cost.
     * @return none.
     * @brief Basic constructor.
     * @warning none.
     */
    GeodesicDistancePathFunction( Image< D > &init_value, Image< int > *init_label, Image< int > *init_predecessor,
                                  bool sequential_label, const Adjacency &adj, const Image< D > &handicap );

    /**
     * @date 2014/Jan/08
     * @param pf: path-function to be copied.
     * @return none.
     * @brief Copy constructor.
     * @warning none.
     */
    GeodesicDistancePathFunction( const GeodesicDistancePathFunction< D > &pf );

    /**
     * @date 2014/Jan/08
     * @param none.
     * @return none.
     * @brief Destructor.
     * @warning none.
     */
    ~GeodesicDistancePathFunction( );

    /**
     * @date 2014/Jan/08
     * @param pf: path-function to be assigned.
     * @return none.
     * @brief Assignment operator.
     * @warning none.
     */
    GeodesicDistancePathFunction< D > operator=( const GeodesicDistancePathFunction< D > &pf );

    /**
     * @date 2014/Jan/08
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
     * @date 2014/Jan/08
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
     * @date 2014/Jan/08
     * @param source: Source pixel index.
     * @param target: Adjacent pixel index.
     * @return True if path-value is propagated.
     * @brief Updates adjacent pixel values and returns true if path_function is propagated.
     * @warning none.
     */
    bool Propagate( size_t index, size_t adj_index, size_t adj_pos );

    /**
     * @date 2014/Jan/08
     * @param none.
     * @return True for increasing and false for decreasing values.
     * @brief Returns whether this path function uses increasing or decreasing values.
     * @warning none.
     */
    bool Increasing( );

  };

}

#include "GeodesicPathFunction.cpp"

#endif
