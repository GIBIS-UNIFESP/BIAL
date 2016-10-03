/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/26 
 * Content: Hierarchical Graph for clustering.
 */

#include "Common.hpp"
#include "Vector.hpp"

#ifndef BIALHIERARCHICALGRAPH_H
#define BIALHIERARCHICALGRAPH_H

#include "Graph.hpp"
#include "GraphAdjacency.hpp"
#include "Matrix.hpp"

namespace Bial {

  template< class D >
  class Feature;

  template< class GRAPH_ADJACENCY >
  class HierarchicalGraph : public Graph< GRAPH_ADJACENCY > {

  protected:

    Vector< int > merge_label;
    Vector< int > split_label;
    Vector< double > merge_cut;
    Vector< double > split_cut;

  public:

    /**
     * @date 2014/Nov/25 
     * @param feature: feature vector containing only the subsamples. 
     * @param scl_min, scl_max: minimum and maximum scale fractions utilized for clustering. (0.0 - 1.0) 
     * @return none. 
     * @brief Initializes the object. 
     * @warning none. 
     */
    template< class D >
    void Initialize( const Feature< D > &feature, float scl_min, float scl_max );

    /**
     * @date 2014/Nov/25 
     * @param none. 
     * @return A reference to merge_label. 
     * @brief Returns a reference to merge_label. 
     * @warning none. 
     */
    Vector< int > &MergeLabel( );

    /**
     * @date 2014/Nov/25 
     * @param none. 
     * @return A reference to merge_label. 
     * @brief Returns a reference to merge_label. 
     * @warning none. 
     */
    const Vector< int > &MergeLabel( ) const;

    /**
     * @date 2014/Nov/14 
     * @param none. 
     * @return A reference to split_label. 
     * @brief Returns a reference to split_label. 
     * @warning none. 
     */
    Vector< int > &SplitLabel( );

    /**
     * @date 2014/Nov/14 
     * @param none. 
     * @return A reference to split_label. 
     * @brief Returns a reference to split_label. 
     * @warning none. 
     */
    const Vector< int > &SplitLabel( ) const;

    /**
     * @date 2014/Nov/25 
     * @param none. 
     * @return A reference to merge_cut. 
     * @brief Returns a reference to merge_cut. 
     * @warning none. 
     */
    Vector< double > &MergeCut( );

    /**
     * @date 2014/Nov/25 
     * @param none. 
     * @return A reference to merge_cut. 
     * @brief Returns a reference to merge_cut. 
     * @warning none. 
     */
    const Vector< double > &MergeCut( ) const;

    /**
     * @date 2014/Nov/14 
     * @param none. 
     * @return A reference to split_cut. 
     * @brief Returns a reference to split_cut. 
     * @warning none. 
     */
    Vector< double > &SplitCut( );

    /**
     * @date 2014/Nov/14 
     * @param none. 
     * @return A reference to split_cut. 
     * @brief Returns a reference to split_cut. 
     * @warning none. 
     */
    const Vector< double > &SplitCut( ) const;

    /**
     * @date 2014/Nov/14 
     * @param scl: Number of scale element. 
     * @return The number of clusters. 
     * @brief Computes cluster labels for spectral domain. 
     * @warning none. 
     */
    size_t Clustering( size_t scl );

    /**
     * @date 2014/Nov/20 
     * @param scl: Number of scale element. 
     * @return The number of clusters. 
     * @brief Devides clusters if a better cut is find for each of them. 
     * @warning none. 
     */
    size_t MergeClusters( size_t scl );

    /**
     * @date 2014/Nov/14 
     * @param scl: Number of scale element. 
     * @return The number of clusters. 
     * @brief Devides clusters if a better cut is find for each of them. 
     * @warning none. 
     */
    size_t SplitClusters( size_t scl );

  };

}

#include "HierarchicalGraph.cpp"

#endif
