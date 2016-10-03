/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @file FuzzyCMeans.hpp
 * @author Fábio Augusto Menocci Cappabianco 
 * @date 2012/Jun/25 
 * @version 1.0.00
 * @brief
 * Content: FuzzyCMeans class 
 * <br> Description: Implementation of Fuzzy C-Means clustering algorithm
 * for 3D images. 
 */

#include "Common.hpp"
#include "Feature.hpp"
#include "Image.hpp"
#include "Vector.hpp"

#ifndef BIALCMEANS_H
#define BIALCMEANS_H

namespace Bial {

  template< class D >
  class FuzzyCMeans {

  private:

    const Feature< D > &feats;
    Feature< D > centroids;
    Matrix< double > membership;
    float m;
    double epsilon;
    size_t max_iterations;

  public:

    /**
     * @date 2012/Jun/25 
     * @param feats: 3D image feature vector.
     * @param clusters: number of expected clusters. 
     * @param m: power of degree of membership. m > 1. Suggested: 2
     * @param epsilon: termination criterion. 0.0 < epsilon < 1.0. Values closer to 0.0, are more precise, but
     *             demands more computational effort.
     * @return none. 
     * @brief Basic constructor. 
     * @warning none. 
     */
    FuzzyCMeans( const Feature< D > &new_feats, int new_clusters, float new_m, double new_epsilon, 
                 size_t nmax_iterations = 30 );

  private:

    /* membership: matrix of fuzzy degree of membership. 
     * @date 2012/Jun/25 
     * @param none.
     * @return Maximum membership difference considering each centroid/feature pair between consecutive iterations. 
     * @brief Update the membership matriz. 
     * @warning Both membership matriz and centroid positions must be already inicialized. 
     */
    double MembershipUpdate( );

    /* membership: matrix of fuzzy degree of membership. 
     * @date 2012/Jun/25 
     * @param none.
     * @return none. 
     * @brief Update the centroid positions. 
     * @warning Both membership matriz and centroid positions must be already inicialized. 
     */
    void CentroidUpdate( );

  public:

    /**
     * @date 2012/Jun/25 
     * @param verbose: verbose mode.
     * @return The fuzzy degree of membership. 
     * @brief Computes fuzzy c-means from 3D image pixels, according to the choosen feature vector. 
     * @warning It is the simplest method that does not takes into account image inhomogeneity. If centroid positions
     * are not initialized, void UniformCentroidInitialization() is called.
     */
    Matrix< double > Run( bool verbose = false );

    /**
     * @date 2012/Jun/26 
     * @param none.
     * @return The fuzzy degree of membership. 
     * @brief Returns the matrix containing the fuzzy degree of
     * membership computed by fuzzy C-Means. 
     * @warning none. 
     */
    Matrix< double > GetMembership( ) const;

    /**
     * @date 2012/Jun/25 
     * @param none.
     * @return none. 
     * @brief Initializes centroid positions of the clusters with
     * uniform space between them. 
     * @warning none. 
     */
    void UniformCentroidInitialization( );

    /**
     * @date 2012/Aug/20 
     * @param os: an output stream. 
     * @param itr: Number of the iteration. 
     * @param max_change: Maximum change from last iteration. 
     * @return The output stream. 
     * @brief Prints Fuzzy c-means iteration status to output stream os. 
     * @warning none. 
     */
    template< class O >
    O &PrintIteration( O &os, int itr, double max_change ) const;

  };

}

#include "FuzzyCMeans.cpp"

#endif
