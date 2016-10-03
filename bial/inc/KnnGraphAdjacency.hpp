/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/26 
 * Content: Knn graph adjacency.
 */

#include "Common.hpp"
#include "Vector.hpp"

#ifndef BIALKNNGRAPHADJACENCY_H
#define BIALKNNGRAPHADJACENCY_H

#include "Matrix.hpp"
#include "GraphAdjacency.hpp"

namespace Bial {

  template< class D >
  class Feature;
  class Sample;

  class KnnGraphAdjacency : public GraphAdjacency< KnnGraphAdjacency > {

  private:

    Matrix< size_t > arc;
    Matrix< double > arc_weight;
    Vector< size_t > scale;
    /** @brief  For plateau edges. */
    Vector< Vector< size_t > > plateau; 
    /** @brief  Just to return for heterogeneous adjacency size. */
    Vector< size_t > null_heterogeneous_size; 

  public:

    static const size_t MAX_SAMPLES;

    /**
     * @date 2012/Oct/09 
     * @param feature: feature vector containing only the subsamples. 
     * @param sample: sample vector. 
     * @param scl_min, scl_max: minimum and maximum scale fractions utilized for clustering. (0.0 - 1.0) 
     * @return none. 
     * @brief Initializes the object. 
     * @warning none. 
     */
    template< class D >
    void Initialize( const Feature< D > &feature, const Sample &sample, float scl_min, float scl_max );

    /**
     * @date 2014/Nov/14 
     * @param none. 
     * @return The number of different scales. 
     * @brief Returns the number of different scales. 
     * @warning none. 
     */
    size_t Scales( ) const;

    /**
     * @date 2014/Nov/26 
     * @param scl: Number of scale element. 
     * @return Sigma value. 
     * @brief Returns sigma value, that is, the variance of the gaussian of the nodes PDF. 
     * @warning none. 
     */
    double Sigma( size_t scl ) const;

    /**
     * @date 2014/Nov/26 
     * @param src: sample index. 
     * @param scl: Number of scale element. 
     * @return Number of arcs within the given scale to the source element. 
     * @brief Returns the number of adjacents to src with scale scl. 
     * @warning none. 
     */
    size_t Arcs( size_t src, size_t scl ) const;

    /**
     * @date 2014/Nov/26 
     * @param src: sample index. 
     * @param adj: Number of adjacent element. 
     * @return The 'adj'-th adjacent index from src. 
     * @brief Returns the 'adj'-th adjacent index from src. 
     * @warning none. 
     */
    size_t Arc( size_t src, size_t adj ) const;

    /**
     * @date 2014/Nov/26 
     * @param src: sample index. 
     * @param adj: Number of adjacent element. 
     * @return Weight of arc from src to its 'arc'-th adjacent. 
     * @brief Returns the weight of arc from src to its 'arc'-th adjacent. 
     * @warning none. 
     */
    double ArcWeight( size_t src, size_t adj ) const;

    /**
     * @date 2014/Jun/04 
     * @param feature: Feature vector. 
     * @param scl_min, scl_max: minimum and maximum scale fractions utilized for clustering. (0.0 - 1.0) 
     * @return A vector with neighbors based on scl_min and scl_max. 
     * @brief Computes neighbors to be used by Knn algorithm. 
     * @warning 0.0 < scl_min <= scl_max <= 1.0. 
     */
    template< class D >
    void EstimateK( const Feature< D > &feature, float scl_min, float scl_max );

    /**
     * @date 2013/Oct/17 
     * @param density: density vector. 
     * @param scl: Required scale. 
     * @param delta: Maximum density distance in a plateau. 
     * @return none. 
     * @brief Complements the k-nn adjacency relation with plateau edges,
     * avoiding assimetric adjacencies. 
     * @warning none. 
     */
    Vector< Vector< size_t > > &HeterogeneousAdjacency( const Vector< double > &density, size_t scl, double delta );

    /**
     * @date 2014/Oct/24 
     * @param none. 
     * @return none. 
     * @brief Returns the knn adjacency with requested scale. 
     * @warning none. 
     */
    const Matrix< size_t > &HomogeneousAdjacency( ) const;

    /**
     * @date 2015/Jan/16 
     * @param scl: Required scale. 
     * @return The number of elements in homogeneous adjacency. 
     * @brief Returns the number of elements in homogeneous adjacency. 
     * @warning none. 
     */
    size_t HomogeneousSize( size_t scl ) const;

    /**
     * @date 2015/Jan/16 
     * @param scl: Required scale. 
     * @return The number of elements in homogeneous adjacency. 
     * @brief Returns the number of elements in homogeneous adjacency. 
     * @warning none. 
     */
    const Vector< size_t > &HeterogeneousSize( size_t scl ) const;

    /**
     * @date 2014/Nov/26 
     * @param src: sample index. 
     * @param scl: Number of scale element. 
     * @param weight: Weight to be checked. 
     * @return True if distance lays inside src region of influence. 
     * @brief Verifies if distance lays inside src region of influence. 
     * @warning none. 
     */
    bool ValidNeighbor( size_t src, size_t scl, double distance ) const;

  };

}

#include "KnnGraphAdjacency.cpp"

#endif
