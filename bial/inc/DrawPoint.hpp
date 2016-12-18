/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2014/Mar/14
 * @brief Functions to draw points in images.
 */

#include "Color.hpp"
#include "Common.hpp"
#include "DrawFigure.hpp"
#include "Vector.hpp"

#ifndef BIALDRAWPOINT_H
#define BIALDRAWPOINT_H

namespace Bial {

  template< class D >
  class Image;

  class Point : public Figure< Point > {

public:
    Vector< float > coordinate; /* Coordinates of a point in 2 or 3 dimensions. */

    /**
     * @date 2015/Jul/01
     * @param coord: Coordinates of the point.
     * @param color: The color of the point.
     * @return none.
     * @brief Basic Constructor for 2 and 3 dimensional point.
     * @warning none.
     */
    Point( const Vector< float > &coord, const Color color = Color( 1 ) );

    /**
     * @date 2015/Jul/01
     * @param img: An image.
     * @return none.
     * @brief Draws n-dimensional point in input image.
     * @warning none.
     */
    template< class D >
    void Draw( Image< D > &img ) const;

    /**
     * @date 2015/Jul/01
     * @param img: An image.
     * @param displacement: Displacement from low coordinate to start drawing.
     * @return none.
     * @brief Draws n-dimensional point in input image.
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
     * @date 2014/Mar/18
     * @param os: an output stream.
     * @return none.
     * @brief Prints point information to stream.
     * @warning none.
     */
    template< class O >
    void Print( O &os ) const;

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
  O &operator<<( O &os, const Point &obj );

}

#include "DrawPoint.cpp"

#endif
