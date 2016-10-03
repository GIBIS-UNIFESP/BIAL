/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/26 
 * @brief Abstract class to define graph adjacencies. 
 */

#include "Common.hpp"
#include "Sample.hpp"

#ifndef BIALGRAPHADJACENCY_H
#define BIALGRAPHADJACENCY_H

#include "Feature.hpp"

namespace Bial {

  /** @brief  Graph Operator classes. Used to compute OPF. May use Knn, LSH, etc. */
  template< class DERIVED >
  class GraphAdjacency {

  public:

    /**
     * @date 2014/Nov/14 
     * @param none. 
     * @return Maximum number of samples. 
     * @brief Returns the maximum number of samples of the derived class. 
     * @warning none. 
     */
    static size_t MaxSamples( ) {
      return( DERIVED::MAX_SAMPLES );
    }

    /**
     * @date 2014/Oct/23 
     * @param none. 
     * @return none. 
     * @brief Virutal destructor. 
     * @warning none. 
     */
    virtual ~GraphAdjacency( ) {
    }

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
    void Initialize( const Feature< D > &feature, const Sample &sample, float scl_min, float scl_max ) {
      static_cast< DERIVED* >( this )->Initialize( feature, scl_min, scl_max );
    }

    /**
     * @date 2014/Nov/14 
     * @param none. 
     * @return The number of different scales. 
     * @brief Returns the number of different scales. 
     * @warning none. 
     */
    virtual size_t Scales( ) const = 0;

    /**
     * @date 2014/Nov/26 
     * @param scl: Number of scale element. 
     * @return Sigma value. 
     * @brief Returns sigma value, that is, the variance of the gaussian of the nodes PDF. 
     * @warning none. 
     */
    virtual double Sigma( size_t scl ) const = 0;

    /**
     * @date 2014/Nov/26 
     * @param src: sample index. 
     * @param scl: Number of scale element. 
     * @return Number of arcs within the given scale to the source element. 
     * @brief Returns the number of adjacents to src with scale scl. 
     * @warning none. 
     */
    virtual size_t Arcs( size_t src, size_t scl ) const = 0;

    /**
     * @date 2014/Nov/26 
     * @param src: sample index. 
     * @param adj: Number of adjacent element. 
     * @return The 'adj'-th adjacent index from src. 
     * @brief Returns the 'adj'-th adjacent index from src. 
     * @warning none. 
     */
    virtual size_t Arc( size_t src, size_t adj ) const = 0;

    /**
     * @date 2014/Nov/26 
     * @param src: sample index. 
     * @param adj: Number of adjacent element. 
     * @return Weight of arc from src to its 'arc'-th adjacent. 
     * @brief Returns the weight of arc from src to its 'arc'-th adjacent. 
     * @warning none. 
     */
    virtual double ArcWeight( size_t src, size_t adj ) const = 0;

    /**
     * @date 2014/Nov/26 
     * @param src: sample index. 
     * @param scl: Number of scale element. 
     * @param weight: Weight to be checked. 
     * @return True if distance lays inside src region of influence. 
     * @brief Verifies if distance lays inside src region of influence. 
     * @warning none. 
     */
    virtual bool ValidNeighbor( size_t src, size_t scl, double distance ) const = 0;

  };

}

#endif
