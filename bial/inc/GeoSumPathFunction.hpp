/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Sep/19
 * @brief Geodesic restriction sum path function to compute the initial path-values and propagated path-values in IFT
 *        algorithm. This function is used to compute predecessor map for Geodesic Star Functions.
 */

#ifndef BIALGEOSUMPATHFUNCTION_H
#define BIALGEOSUMPATHFUNCTION_H

#include "Common.hpp"
#include "Image.hpp"
#include "PathFunction.hpp"

namespace Bial {

  enum class BucketState : char;

  template< class D >
  class GeodesicRestrictionPathFunction : public PathFunction< Image, D > {

  protected:
    /**
     * @brief Reference to source image.
     */
    const Image< D > &intensity;
    /**
     * @brief Handicap container (Vector, Matrix, Image, etc).
     */
    Image< D > handicap;
    /**
     * @brief
     * Orientation weight. -1.0 to 1.0. (negative) for dark objects, (positive) for light objects, (zero)
     * for non-oriented edges. Default value is 0.0.
     */
    double alpha;
    /**
     * @brief
     * Weight to control behavior between gradient and distance weight. <br>
     * Lower: more similar to geodesic distance. <br>
     * Higher: more similar to gradient. Default value = 0.5
		 */
    double beta;

  public:

    /**
     * @date 2013/Jul/02
     * @param new_beta: weight of the geodesic funtion. Closer to 0 is more similar to euclidean distance. Higher
     * values is closer to SumPathFunction.
     * @return none.
     * @brief Basic constructor.
     * @warning none.
     */
    GeodesicRestrictionPathFunction( const Image< D > &handicap, const Image< D > &new_intensity, 
                                     double new_alpha = 0.0, double new_beta = 0.5 );

    /**
     * @date 2013/Jul/01
     * @param pf: path-function to be copied.
     * @return none.
     * @brief Copy constructor.
     * @warning none.
     */
    GeodesicRestrictionPathFunction( const GeodesicRestrictionPathFunction< D > &pf );

    /**
     * @date 2013/Jul/01
     * @param none.
     * @return none.
     * @brief Destructor.
     * @warning none.
     */
    ~GeodesicRestrictionPathFunction( );

    /**
     * @date 2013/Jul/01
     * @param pf: path-function to be assigned.
     * @return none.
     * @brief Assignment operator.
     * @warning none.
     */
    GeodesicRestrictionPathFunction< D > operator=( const GeodesicRestrictionPathFunction< D > &pf );

    /**
     * @date 2012/Oct/02
     * @param init_value: Reference for initial value container.
     * @param init_label: Reference for initial label container.
     * @param init_predecessor: Reference for predecessor container.
     * @param sequential_label: Sets labeling sequentially.
     * @return none.
     * @brief Initializes object attributes.
     * @warning This function is called automatically by IFT constructor.
     */
    void Initialize( Image< D > &init_value, Image< int > *init_label, Image< int > *init_predecessor, 
                     bool sequential_label );

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
     * @brief Sets initial value for root pixel of index 'index'. Also sets its label value.
     * @warning none.
     */
    bool RemoveLabel( size_t index, BucketState state );

    /**
     * @date 2013/Oct/14
     * @param source: Source pixel index.
     * @param target: Adjacent pixel index.
     * @return True if it is possible to propagate.
     * @brief Checks if index can offer better value to adj_index, prior to computing it by Propagate
     * function. This function makes IFT faster.
     * @warning none.
     */
    bool Capable( int index, int adj_index, BucketState adj_state );

    /**
     * @date 2013/Jun/28
     * @param source: Source pixel index.
     * @param target: Adjacent pixel index.
     * @return True if path-value is propagated.
     * @brief Updates adjacent pixel values and returns true if path_function is propagated.
     * @warning none.
     */
    bool Propagate( int index, int adj_index );

    /**
     * @date 2012/Sep/19
     * @param none.
     * @return True for increasing and false for decreasing values.
     * @brief Returns whether this path function uses increasing or decreasing values.
     * @warning none.
     */
    bool Increasing( );

  };

}

#include "GeoSumPathFunction.cpp"

#endif
