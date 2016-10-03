/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2014/Mar/14
 * @brief Functions to draw forms in images.
 */

#include "Color.hpp"
#include "Common.hpp"
#include "DrawFigure.hpp"
#include "Geometrics.hpp"
#include "Vector.hpp"

#ifndef BIALDRAWSEQUENCE_H
#define BIALDRAWSEQUENCE_H

namespace Bial {

  template< class D >
  class Image;
  class Adjacency;

  class Sequence : public Figure< Sequence > {

public:
    Vector< Vector< float > > point; /* coordinates of all sequence points. Access: [ index ][ dimension ]. */

public:
    /**
     * @date 2015/Apr/29
     * @param label: Label image used just to get its dimensions.
     * @param start_pxl: One of the sequence extremeties.
     * @param adj: An adjacency relation to be used to follow sequence points.
     * @return none.
     * @brief basic constructor. Creates a sequence of connected points from a connected component that begins at
     * start_pxl and have the same label.
     * @warning none.
     */
    template< class D >
    Sequence( const Image< D > &label, size_t start_pxl, Adjacency &adj, const Color color = Color( 1 ) );

    /**
     * @date 2014/Mar/18
     * @version 1.0.00
     * @param img: An image.
     * @return none.
     * @brief Draws 2D or 3D sequence in input image.
     * @warning none.
     */
    template< class D >
    void Draw( Image< D > &img ) const;

    /**
     * @date 2014/Mar/17
     * @version 1.0.00
     * @param img: An image.
     * @param displacement: Displacement from sequence's coordinates to start drawing.
     * @return none.
     * @brief Draws 2D or 3D sequence in input image.
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
     * @brief Prints sequence information to stream.
     * @warning none.
     */
    template< class O >
    void Print( O &os ) const;

private:
    /**
     * @date 2015/Apr/29
     * @param label: Label image used just to get its dimensions.
     * @param start_pxl: One of the sequence extremeties.
     * @param visited: Auxiliary image to mark the visited pixels.
     * @param adj: An adjacency relation to be used to follow sequence points.
     * @return none.
     * @brief Auxiliary function for the constructor to follow the sequence of points recursively.
     * @warning none.
     */
    template< class D >
    void SequenceCreator( const Image< D > &label, size_t start_pxl, Image< int > &visited, Adjacency &adj );

public:
    /**
     * @date 2015/Jul/08
     * @param none.
     * @return The number of pixels in this sequence.
     * @brief Returns the number of pixels in this sequence.
     * @warning none.
     */
    double size( ) const;

    /**
     * @date 2015/Jul/14
     * @param min_sequence_size: Minimum size of sequence to be validated.
     * @param min_sequence_size: Maximum size of sequence to be validated.
     * @return 'false' if the sequence is out of required standards.
     * @brief Checks if the sequence in the expected range.
     * @warning none.
     */
    bool ValidateSize( float min_sequence_size, float max_sequence_size = std::numeric_limits< float >::max( ) );

    /**
     * @date 2015/Jul/14
     * @param center: Expected center for a circle composed by this sequence.
     * @param min_center_dist: Minimum distance from expected to estimated center.
     * @param max_center_dist: Maximum distance from expected to estimated center.
     * @param min_radius: Minimum expected mean radius to estimated center.
     * @param min_radius: Maximum expected mean radius to estimated center.
     * @param min_circularity: Minimum circularity of the sequence given the estimated radius and center.
     * @param max_circularity: Maximum circularity of the sequence given the estimated radius and center.
     * @return 'false' if the sequence is out of required standards.
     * @brief Calculate and updates sequence's Center, Radius, and Circularity variables.
     * @warning The sequence of points must be sorted.
     */
    bool ValidateCircularity( Vector< float > &center,
                              float min_center_dist,
                              float max_center_dist = std::numeric_limits< float >::max( ),
                              float min_radius = 0.0f,
                              float max_radius = std::numeric_limits< float >::max( ),
                              float min_circularity = 0.0f,
                              float max_circularity = std::numeric_limits< float >::max( ) );

    /**
     * @date 2015/Jul/08
     * @param max_radius: Maximum expected radius.
     * @return The center of the estimated circle.
     * @brief Calculates the center of an estimated circle to the sequence.
     * @warning none.
     */
    Vector< float > CircleCenter( float max_radius );

    /**
     * @date 2015/Jul/08
     * @param center: Estimated circle center to the sequence.
     * @param begin: First index of the sequence to take into account.
     * @param last: Index of the sequence to take into account.
     * @return The radius of this circle sequence.
     * @brief Calculates the sequence's radius using Euclidian distance.
     * @warning none.
     */
    float MeanRadius( const Vector< float > &center, size_t begin, size_t end );

    /**
     * @date 2015/Jul/08
     * @param center: Estimated circle center to the sequence.
     * @param radius: Mean radius of the sequence.
     * @param begin: First index of the sequence to take into account.
     * @param last: Index of the sequence to take into account.
     * @return The radius of this circle sequence.
     * @brief Calculates the sequence's circularity given its center and radius.
     * @warning none.
     */
    float Circularity( const Vector< float > &center, float radius, size_t begin, size_t end );
  };

  /**
   * @date 2016/Aug/11
   * @param os An output stream class.
   * @param obj The object to be written into the output stream.
   * @return the output stream.
   * @brief Prints object features into the output stream.
   * @warning none.
   */
  template< class O >
  O &operator<<( O &os, const Sequence &obj );

}

#include "DrawSequence.cpp"

#endif
