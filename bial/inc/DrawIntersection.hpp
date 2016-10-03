/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2014/Mar/14
 * @brief Functions to find intersection between forms.
 */

#include "Common.hpp"
#include "Vector.hpp"

#ifndef BIALDRAWINTERSECTION_H
#define BIALDRAWINTERSECTION_H

namespace Bial {

  class Sequence;
  class Line;

  /**
   * @date 2015/Jul/08
   * @param sequence: A sequence.
   * @param line: Pixels of a line.
   * @return A vector with the coordinates of the first intersection with the line. Returns
   * empty vector, if it they don't intersect.
   * @brief Locates the first point where the line and the sequence intersect.
   * @warning none.
   */
  Vector< float > Intersection( const Sequence &sequence, const Line &line );

  /**
   * @date 2015/Jul/08
   * @param line1, line2: Lines to be checked for intersection.
   * @return A vector with the coordinates where the lines intersect each other. Returns empty vector if they are
   * parallel.
   * @brief Computes and returns the coordinates where lines intersect each other.
   * @warning none.
   */
  Vector< float > Intersection( const Line &line1, const Line &line2 );

}

#include "DrawIntersection.cpp"

#endif
