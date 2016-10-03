/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2014/Mar/14
 * @brief Functions to draw circles in images.
 */

#include "Color.hpp"
#include "Common.hpp"
#include "DrawFigure.hpp"
#include "Vector.hpp"

#ifndef BIALDRAWCIRCLE_H
#define BIALDRAWCIRCLE_H

namespace Bial {

  template< class D >
  class Image;
  class Adjacency;

  class Circle : public Figure< Circle > {
public:
    Vector< float > center;
    float radius;
    bool filled;

    /**
     * @date 2014/Dec/06
     * @version 1.0.00
     * @param radius: Radius of the circle
     * @param color: The color of the circle.
     * @param filled: Whether circle is filled or not.
     * @return none.
     * @brief Basic Constructor.
     * @warning Only availble to 2D images
     */
    Circle( const Vector< float > &center, const float radius = 1.0, const Color color = Color( 1 ),
            const bool filled = true );

    /**
     * @date 2014/Dec/06
     * @version 1.0.00
     * @param img: An image.
     * @return none.
     * @brief Draws 2D circle in input image.
     * @warning circle displacement (center) is set to the center of the image.
     */
    template< class D >
    void Draw( Image< D > &img ) const;

    /**
     * @date 2014/Dec/06
     * @version 1.0.00
     * @param img: An image.
     * @param displacement: Center of the circle.
     * @return none.
     * @brief Draws 2D circle in input image.
     * @warning none.
     */
    template< class D >
    void Draw( Image< D > &img, const Vector< float > &displacement ) const;

    /**
     * @date 2015/Jul/07
     * @param none.
     * @return Vector with pixels.
     * @brief Returns a vector with coordinates of the pixels from this Figure.
     * @warning none.
     */
    Vector< Vector< int > > Pixels( ) const;

    /**
     * @date 2015/Jul/07
     * @param displacement: Displacement from low coordinate of the figure.
     * @return Vector with pixels.
     * @brief Returns a vector with coordinates of the pixels from this figure.
     * @warning none.
     */
    Vector< Vector< int > > Pixels( const Vector< float > &displacement ) const;

    /**
     * @date 2014/Dec/06
     * @version 1.0.00
     * @param os: an output stream.
     * @return none.
     * @brief Prints circle information to stream.
     * @warning none.
     */
    template< class O >
    void Print( O &os ) const;

protected:

    /**
     * @date 2014/Dec/06
     * @version 1.0.00
     * @param img: An image.
     * @param displacement: Center of the circle.
     * @param dx: horizontal displacement of the pixel
     * @param dy: vertical displacement of the pixel
     * @return none.
     * @brief Plot 8 pixels based on calculated displacement.
     * @warning none.
     */
    template< class D >
    void PlotPixels( Image< D > &img, const Vector< float > &displacement, const int dx, const int dy ) const;

  };

  /**
   * @date 2015/Jul/08
   * @param os An output stream class.
   * @param obj The object to be written into the output stream.
   * @return the output stream.
   * @brief Prints object features into the output stream.
   * @warning none.
   */
  template< class O >
  O &operator<<( O &os, const Circle &obj );

}

#include "DrawCircle.cpp"

#endif
