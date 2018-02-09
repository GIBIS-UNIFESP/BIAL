/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2014/Jan/07
 * @brief B-Spline implementation.
 */

#ifndef BIALBSPLINE_H
#define BIALBSPLINE_H

#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {

  template< class D > class Image;

  namespace Curve {

    /**
     * @date 2015/Feb/03
     * @param vertices: Number of vertices in a dimension.
     * @param order: Order of the B-spline.
     * @return Vector of knots.
     * @brief Computes and returns the knots for the given spline.
     * @warning none.
     */
    Vector< size_t > BSplineKnot( size_t vertices, size_t order );

    /**
     * @date 2015/Feb/03
     * @param knot: vector of knots in one dimension.
     * @param order: Order of the B-spline.
     * @param parameter: Basis parameter.
     * @param vertex: set of vertices.
     * @return Vector of basis.
     * @brief Computes and returs the basis for this spline.
     * @warning none.
     */
    Vector< float > BSplineBasis( const Vector< size_t > &knot, size_t order, float parameter, size_t vertices );

    /**
     * @date 2015/Feb/03
     * @param vertices: Number of spline vertices in a dimension.
     * @param points: number of points of the output in each dimension.
     * @param order: Order of the B-spline.
     * @return B-spline points.
     * @brief Computes and returs B-spline matrix with given order and dimensions, from a set of vertices.
     * @warning none.
     */
    Matrix< float > BSpline( const Matrix< float > &vertex, const Vector< size_t > &points, size_t order );

  }

}

#include "BSpline.cpp"

#endif
