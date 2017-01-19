/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Jul/05
 * @brief Adjacency relation with round format.
 * <br> Future add-on's: none.
 */

#include "Adjacency.hpp"
#include "Common.hpp"
#include "Vector.hpp"

#ifndef BIALADJACENCYROUND_H
#define BIALADJACENCYROUND_H

/* Declarations -------------------------------------------------------------------------------------------------------- */

namespace Bial {

  namespace AdjacencyType {

    /**
     * @date 2012/Jul/06
     * @param radius: the radius of the circular adjacency.
     * @return adjacency relation with circular form.
     * @brief Create and return a 2D circular adjacency relation of radius 'radius'.
     * @warning none.
     */
    Adjacency Circular( float radius = 1.0 );

    /**
     * @date 2012/Jul/06
     * @param x_radius: the radius of the elliptic adjacency in axis x.
     * @param y_radius: the radius of the elliptic adjacency in axis y.
     * @return adjacency relation with elliptic form.
     * @brief Create and return a 2D elliptic adjacency relation of radius 'radius'.
     * @warning none.
     */
    Adjacency Elliptic( float x_radius = 1.0, float y_radius = 1.0 );

    /**
     * @date 2012/Jul/06
     * @param radius: the radius of the spheric adjacency.
     * @return adjacency relation with spheric form.
     * @brief Create and return a 3D spheric adjacency relation of radius 'radius'.
     * @warning none.
     */
    Adjacency Spheric( float radius = 1.0 );

    /**
     * @date 2012/Sep/21
     * @param radius: The radius of the spheric adjacency.
     * @param dims: The number of dimensions.
     * @return adjacency relation with spheric form.
     * @brief Create and return a hyper-spheric adjacency relation of radius 'radius' and with dim dimensions.
     * @warning none.
     */
    Adjacency HyperSpheric( float radius = 1.0, size_t dims = 4 );

    /**
     * @date 2012/Jul/06
     * @param x_radius: the radius of the ellipsoid adjacency in axis x.
     * @param y_radius: the radius of the ellipsoid adjacency in axis y.
     * @param z_radius: the radius of the ellipsoid adjacency in axis z.
     * @return adjacency relation with ellipsoid form.
     * @brief Create and return a 3D ellipsoid adjacency relation of radius 'radius'.
     * @warning none.
     */
    Adjacency Ellipsoid( float x_radius = 1.0, float y_radius = 1.0, float z_radius = 1.0 );

    /**
     * @date 2013/Nov/14
     * @param radius: vector with hyper ellipse radii.
     * @return adjacency relation with hyper ellipsoid form.
     * @brief Create and return a ND hyper ellipsoid adjacency relation of radii given by 'radius'.
     * @warning none.
     */
    Adjacency HyperEllipsoid( const Vector< float > &radius );

    /**
     * @date 2012/Jul/06
     * @param radius: the radius of the circunference adjacency.
     * @return adjacency relation with circunference form.
     * @brief Create and return a 2D circunference adjacency relation of radius 'radius'.
     * @warning none.
     */
    Adjacency Circunference( float radius );

  }

}

#include "AdjacencyRound.cpp"

#endif
