/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/26 
 * @brief OPF spatial clustering algorithms. 
 */

#include "Common.hpp"

#ifndef BIALOPFSPATIALCLUSTERING_H
#define BIALOPFSPATIALCLUSTERING_H

namespace Bial {

  class Adjacency;
  template< class D >
  class Image;
  template< class D >
  class Feature;

  namespace OPF {

    /**
     * @date 2013/Nov/26 
     * @param feature: Feature vector. 
     * @param label: label map. 
     * @param adjacency: adjacency relation for spacial constraints. 
     * @param intensity_fraction: fraction from 0.0 to 1.0 of the maximum intensity set for adjacent pixels. 
     * @return Number of clusters. 
     * @brief Computes OPF clustering based on the feature and image space, using complete graph. 
     * @warning Feature and init label must have compatible dimensions. 
     */
    template< class D >
    size_t SpatialClustering( const Feature< D > &feature, Image< int > &label, const Adjacency &adjacency,
                              float intensity_fraction );

    /**
     * @date 2012/Nov/26 
     * @param feature: Feature vector. 
     * @param label: label map. May be a std random access container, Bial::Image, or Bial::Matrix class. 
     * @param adjacency: adjacency relation for spacial constraints. 
     * @param intensity_fraction: fraction from 0.0 to 1.0 of the maximum intensity set for adjacent pixels. 
     * @return The bucket size for the IFT queue and PathFunction. 
     * @brief Computes PDF for 'neighbors' neighbours. 
     * @warning none. 
     */
    template< class D >
    float MaxWeight( const Feature< D > &feature, Image< int > &label, const Adjacency &adjacency,
                     float intensity_fraction );

    /**
     * @date 2013/Dec/10 
     * @param feature: Feature vector. 
     * @param label: label map. May be a std random access container, Bial::Image, or Bial::Matrix class. 
     * @param adjacency: adjacency relation for spacial constraints. 
     * @param max_distance: Maximum distance among pixels. 
     * @param thread: Thread number. 
     * @param total_threads: Number of threads. 
     * @return The bucket size for the IFT queue and PathFunction. 
     * @brief Computes PDF for 'neighbors' neighbours. 
     * @warning none. 
     */
    template< class D >
    void MaxWeightThread( const Feature< D > &feature, Image< int > &label, const Adjacency &adjacency,
                          float &max_distance, size_t thread, size_t total_threads );

    /**
     * @date 2012/Nov/26 
     * @param feature: Feature vector. 
     * @param adjacency: adjacency relation for spacial constraints. 
     * @param density: Samples density. Used as value map in IFT. 
     * @param max_weight: maximum arc weight in feature space. 
     * @return The bucket size for the IFT queue and PathFunction. 
     * @brief Computes PDF in spatial and spectral domains. 
     * @warning none. 
     */
    template< class D >
    float PDF( const Feature< D > &feature, const Adjacency &adjacency, Image< float > &density,
               float max_weight );

    /**
     * @date 2013/Dec/10 
     * @param feature: Feature vector. 
     * @param adjacency: adjacency relation for spacial constraints. 
     * @param density: Samples density. Used as value map in IFT. 
     * @param sigma: Sigma(variance) value for exponential PDF. 
     * @param max_dens_diff: Maximum density difference found among samples. 
     * @param thread: Thread number. 
     * @param total_threads: Number of threads. 
     * @return The bucket size for the IFT queue and PathFunction. 
     * @brief Computes PDF in spatial and spectral domains. 
     * @warning none. 
     */
    template< class D >
    float PDFThread( const Feature< D > &feature, const Adjacency &adjacency, Image< float > &density,
                     float sigma, float &max_dens_diff, size_t thread, size_t total_threads );

  }

}

#include "OPFSpatialClustering.cpp"

#endif
