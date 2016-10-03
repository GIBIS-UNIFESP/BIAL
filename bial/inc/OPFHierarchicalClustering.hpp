/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/26 
 * @brief OPF hierarchical clustering algorithms. NOT WORKING YET.
 */

#include "Common.hpp"

#ifndef BIALOPFHIERARCHICALCLUSTERING_H
#define BIALOPFHIERARCHICALCLUSTERING_H

namespace Bial {

  template< class D >
  class Feature;
  template< class GRAPH_ADJACENCY >
  class Graph;
  template< class GRAPH_ADJACENCY >
  class HierarchicalGraph;

  namespace OPF {

    /**
     * @date 2014/Nov/04 
     * @param feature: Feature vector. 
     * @param scale_min, scale_max: minimum and maximum scale. From 0.0 to 1.0.
     * @param graph: the graph used to compute OPF. Default: KnnGraph for function with no parameter.
     * @return Number of clusters. 
     * @brief Computes OPF clustering based only on the feature space, using sampling and a graph. It tries to
     * find the best scale for each cluster. 
     * @warning scale_min must be smaller than of equal to scale_min. 
     */
    template< class D >
    size_t HierarchicalClustering( Feature< D > &feature, float scale_min, float scale_max );
    template< class D, class GRAPH_ADJACENCY >
    size_t HierarchicalClustering( Feature< D > &feature, float scale_min, float scale_max,
                                   HierarchicalGraph< GRAPH_ADJACENCY > &graph );

  }

}

#include "OPFHierarchicalClustering.cpp"

#endif
