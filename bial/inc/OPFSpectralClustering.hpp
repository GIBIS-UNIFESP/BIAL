/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/26 
 * @brief OPF spectral clustering algorithms. 
 */

#include "Common.hpp"

#ifndef BIALOPFSPECTRALCLUSTERING_H
#define BIALOPFSPECTRALCLUSTERING_H

namespace Bial {

  template< class D >
  class Feature;
  template< class GRAPH_ADJACENCY >
  class Graph;

  namespace OPF {

    /**
     * @date 2013/Jul/05 
     * @param feature: Feature vector. 
     * @param scale_min, scale_max: minimum and maximum scale. From 0.0 to 1.0.
     * @param graph: the graph used to compute OPF. Default: KnnGraph for function with no parameter.
     * @return Number of clusters. 
     * @brief Computes OPF clustering based only on the feature space, using sampling and a graph. 
     * @warning scale_min must be smaller than of equal to scale_min. 
     */
    template< class D >
    size_t SpectralClustering( Feature< D > &feature, float scale_min, float scale_max );
    template< class D, class GRAPH_ADJACENCY >
    size_t SpectralClustering( Feature< D > &feature, float scale_min, float scale_max,
                               Graph< GRAPH_ADJACENCY > &graph );

    /**
     * @date 2014/Apr/30 
     * @param feature: Feature vector. 
     * @param clusters: Number of expected clusters.
     * @param graph: the graph used to compute OPF.
     * @return Number of clusters. 
     * @brief Computes OPF clustering based only on the feature space, using sampling and k-nn arcs. 
     * @warning none. 
     */
    template< class D >
    size_t SpectralClustering( Feature< D > &feature, size_t clusters );
    template< class D, class GRAPH_ADJACENCY >
    size_t SpectralClustering( Feature< D > &feature, size_t clusters, Graph< GRAPH_ADJACENCY > &graph );

  }

}

#include "OPFSpectralClustering.cpp"

#endif
