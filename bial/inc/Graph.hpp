/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/26 
 * Content: Graph structure for clustering.
 */

#include "Common.hpp"
#include "Sample.hpp"
#include "Vector.hpp"

#ifndef BIALGRAPH_H
#define BIALGRAPH_H

#include "GraphAdjacency.hpp"

namespace Bial {

  template< class D >
  class Feature;

  template< class GRAPH_ADJACENCY >
  class Graph {

  protected:

    Sample sample;
    Vector< double > density;
    Vector< double > cut;
    Vector< size_t > ordered;
    Vector< int > label;
    GRAPH_ADJACENCY adjacency;

  public:

    /**
     * @date 2014/Nov/14 
     * @param none. 
     * @return Maximum number of samples. 
     * @brief Returns the maximum number of samples of the derived class. 
     * @warning none. 
     */
    size_t MaxSamples( ) const;

    /**
     * @date 2014/Nov/14 
     * @param feature: feature vector containing only the subsamples. 
     * @param scl_min, scl_max: minimum and maximum scale fractions utilized for clustering. (0.0 - 1.0) 
     * @return none. 
     * @brief Initializes the object. 
     * @warning none. 
     */
    template< class D >
    void Initialize( const Feature< D > &feature, float scl_min, float scl_max );

    /**
     * @date 2014/Nov/14 
     * @param none. 
     * @return The number of different scales. 
     * @brief Returns the number of different scales. 
     * @warning none. 
     */
    size_t Scales( ) const;

    /**
     * @date 2014/Nov/25 
     * @param none. 
     * @return The number of nodes. 
     * @brief Returns the number of nodes. 
     * @warning none. 
     */
    size_t Samples( ) const;

    /**
     * @date 2014/Nov/14 
     * @param none. 
     * @return A reference to label. 
     * @brief Returns a reference to label. 
     * @warning none. 
     */
    Vector< int > &Label( );

    /**
     * @date 2014/Nov/14 
     * @param none. 
     * @return A reference to label. 
     * @brief Returns a reference to label. 
     * @warning none. 
     */
    const Vector< int > &Label( ) const;

    /**
     * @date 2012/Oct/09 
     * @param scl: Required scale. 
     * @return The bucket size for the IFT queue and PathFunction. 
     * @brief Computes PDF for spectral domain. 
     * @warning none. 
     */
    double PDF( size_t scl );

    /**
     * @date 2014/Nov/14 
     * @param scl: Number of scale element. 
     * @return The number of clusters. 
     * @brief Computes cluster labels for spectral domain. 
     * @warning none. 
     */
    size_t Clustering( size_t scl );

    /**
     * @date 2012/Oct/09 
     * @param scl: Number of scale element. 
     * @return The normalized cut value. 
     * @brief Computes the normalized cut of the graph labeled with chosen scale. 
     * @warning none. 
     */
    double NormalizedCut( size_t spl ) const;

    /**
     * @date 2014/Nov/11 
     * @param label: the labels of the samples to be considered. 
     * @param scl: Number of scale element. 
     * @return A vector with the normalized cut of each example of the graph labeled with given scale. 
     * @brief Computes the normalized cut of each example of the graph labeled with given scale. 
     * @warning none. 
     */
    Vector< double > SampleNormalizedCut( const Vector< int > &label, size_t scl ) const;

    /**
     * @date 2014/Nov/14 
     * @param scl: Number of scale element. 
     * @return none. 
     * @brief Computes the normalized cut of each example of the graph labeled with chosen scale and assigns to
     * the initial cut. 
     * @warning none. 
     */
    void SetCut( size_t scl );

    /**
     * @date 2014/Oct/23 
     * @param feature: Feature vector. 
     * @param scl: Number of scale element. 
     * @return none. 
     * @brief Propagates labels from subsample to samples. Just an interface to call multi-thread, GPU based or
     * other more specialized methods. 
     * @warning none. 
     */
    template< class D >
    void PropagateLabel( Feature< D > &feature, size_t scl ) const;

    /**
     * @date 2013/Jul/07 
     * @param feature: Feature vector. 
     * @param kng: Knn graph composed from samples and feature vectors. 
     * @param scl: Number of scale element. 
     * @param thread: Thread number. 
     * @param total_threads: Number of threads. 
     * @return none. 
     * @brief Propagates labels from subsample to samples. 
     * @warning none. 
     */
    template< class D >
    void PropagateLabelThread( Feature< D > &feature, size_t scl, size_t thread, size_t total_threads ) const;

    /**
     * @date 2015/Jan/12 
     * @param basename: File basename. A txt file for dots and a gnp file for gnuplot instructions. 
     * @param feat: A feature vecture from which two features will be plotted. 
     * @param scale: The scale of the plotting. 
     * @param x, y: The number of the features that will be plotted. 
     * @return none. 
     * @brief Plots 2D scatter from x and y features from given graph. Colors are labels and size are
     * densities. 
     * @warning none. 
     */
    template< class D >
    void GnuPlot2DScatter( const std::string &basename, const Feature< D > &feat, size_t scale, size_t x = 0,
                           size_t y = 1 );
  };

}

#include "Graph.cpp"

#endif
