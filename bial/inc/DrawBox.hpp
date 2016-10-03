/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2014/Mar/14
 * @brief Functions to draw boxes in images.
 */

#include "Color.hpp"
#include "Common.hpp"
#include "DrawFigure.hpp"
#include "Vector.hpp"

#ifndef BIALDRAWBOX_H
#define BIALDRAWBOX_H

namespace Bial {

  template< class D >
  class Image;
  class Adjacency;

  /**
   * @brief Box Figure class.
   */
  class Box : public Figure< Box > {

public:

    /** @brief  Lower coordinates of box. */
    Vector< float > low_coord;
    /** @brief  Size of box. */
    Vector< float > size;
    /** @brief  Weather or not box is filled. */
    bool filled;

    /**
     * @date 2014/Mar/17
     * @param low_coord: Lower coordinate value of each side.
     * @param size: Side of each box side.
     * @param color: The color of the box.
     * @param filled: Whether box is filled or not.
     * @return none.
     * @brief Basic Constructor.
     * @warning Zero is assigned to all elements. Pixel dimensions are set to 1.0. Channels are set to unknown type
     */
    Box( const Vector< float > &low_coord, const Vector< float > &size, const Color color = Color( 1 ),
         const bool filled = true );

    /**
     * @date 2014/Mar/18
     * @param img: An image.
     * @return none.
     * @brief Draws n-dimensional box in input image.
     * @warning none.
     */
    template< class D >
    void Draw( Image< D > &img ) const;

    /**
     * @date 2014/Mar/17
     * @param img: An image.
     * @param displacement: Displacement from low coordinate to start drawing.
     * @return none.
     * @brief Draws n-dimensional box in input image.
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
     * @brief Prints box information to stream.
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
  O &operator<<( O &os, const Box &obj );

}

#include "DrawBox.cpp"

#endif
