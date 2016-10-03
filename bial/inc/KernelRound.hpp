/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Nov/19 
 * @brief Round kernel generator.
 */

#ifndef BIALKERNELROUND_H
#define BIALKERNELROUND_H

#include "Kernel.hpp"

namespace Bial {

  namespace KernelType {

    /**
     * @date 2013/Nov/19 
     * @param radius: the radius of the circular kernel. 
     * @return kernel with circular form. 
     * @brief Creates and returns a 2D circular kernel of radius 'radius'.
     * @warning none. 
     */
    Kernel Circular( float radius = 1.0f );

    /**
     * @date 2013/Nov/19 
     * @param x_radius: the radius of the elliptic kernel in
     * axis x. 
     * @param y_radius: the radius of the elliptic kernel in
     *             axis y. 
     * @return kernel with elliptic form. 
     * @brief Creates and returns a 2D elliptic kernel of radius 'radius'.
     * @warning none. 
     */
    Kernel Elliptic( float x_radius = 1.0f, float y_radius = 1.0f );

    /**
     * @date 2013/Nov/19 
     * @param radius: the radius of the spheric kernel. 
     * @return kernel with spheric form. 
     * @brief Creates and returns a 3D spheric kernel of radius 'radius'.
     * @warning none. 
     */
    Kernel Spheric( float radius = 1.0f );

    /**
     * @date 2013/Nov/19 
     * @param radius: The radius of the spheric kernel. 
     * @param dims: The number of dimensions. 
     * @return kernel with spheric form. 
     * @brief Creates and returns a hiper-spheric kernel of radius 'radius' and with dim dimensions.
     * @warning none. 
     */
    Kernel HyperSpheric( float radius = 1.0f, size_t dims = 4 );

    /**
     * @date 2013/Nov/19 
     * @param x_radius: the radius of the ellipsoid kernel in axis x. 
     * @param y_radius: the radius of the ellipsoid kernel in axis y. 
     * @param z_radius: the radius of the ellipsoid kernel in axis z. 
     * @return kernel with ellipsoid form. 
     * @brief Creates and returns a 3D ellipsoid kernel of radius 'radius'.
     * @warning none. 
     */
    Kernel Ellipsoid( float x_radius = 1.0f, float y_radius = 1.0f, float z_radius = 1.0f );

  }

}

#include "KernelRound.cpp"

#endif
