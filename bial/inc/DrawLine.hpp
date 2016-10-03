/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2014/Mar/14
 * @brief Functions to draw lines in images.
 */

#include "Color.hpp"
#include "Common.hpp"
#include "DrawFigure.hpp"
#include "Vector.hpp"

#ifndef BIALDRAWLINE_H
#define BIALDRAWLINE_H

namespace Bial {

  template< class D >
  class Image;
  class Adjacency;

  class Line : public Figure< Line > {

public:

    /** @brief  First coordinate of the Line. */
    Vector< float > first_coord;
    /** @brief  Last coordinate of the Line. */
    Vector< float > last_coord;

    /**
     * @date 2014/Dec/08
     * @version 1.0.00
     * @param first_coord: First coordinate of the Line.
     * @param first_coord: Last coordinate of the Line.
     * @param color: The color of the line.
     * @return none.
     * @brief Basic Constructor.
     */
    Line( const Vector< float > &first_coord, const Vector< float > &last_coord, const Color color = Color( 1 ) );

    /**
     * @date 2015/Jul/08
     * @param pnt_0, pnt_1: points between which besection line should to be computed.
     * @param length: Resulting line length.
     * @return A bisection line between the given points.
     * @brief Static Constructor. Returns a bisection line between the given points, centralized between the
     * points.
     * @warning none.
     */
    static Line BisectionLine( const Vector< float > &pnt_0, const Vector< float > &pnt_1, float length );

    /**
     * @date 2014/Mar/18
     * @version 1.0.00
     * @param img: An image.
     * @return none.
     * @brief Draws 2D or 3D line in input image.
     * @warning none.
     */
    template< class D >
    void Draw( Image< D > &img ) const;

    /**
     * @date 2014/Mar/17
     * @version 1.0.00
     * @param img: An image.
     * @param displacement: Displacement from line's coordinates to start drawing.
     * @return none.
     * @brief Draws 2D or 3D line in input image.
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
     * @version 1.0.00
     * @param os: an output stream.
     * @return none.
     * @brief Prints line information to stream.
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
  O &operator<<( O &os, const Line &obj );

}

#include "DrawLine.cpp"

#endif
