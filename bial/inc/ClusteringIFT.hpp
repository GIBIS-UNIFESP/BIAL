/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Sep/08
 * @brief IFT algorithm for clustering purposes.
 */

#ifndef BIALCLUSTERINGIFT_H
#define BIALCLUSTERINGIFT_H

#include "BucketQueue.hpp"
#include "DegeneratedIFT.hpp"
#include "Image.hpp"
#include "PathFunction.hpp"
#include "Vector.hpp"

namespace Bial {

  template< template< class D > class C, class D >
  class ClusteringIFT : public DegeneratedIFT< C, D > {

  private:

    const Matrix< size_t > *uniform_adjacency;
    const size_t uniform_adjs;
    const Vector< Vector< size_t > > &variable_adjacency;
    const Vector< size_t > *variable_adjs;

  public:

    /**
     * @date 2012/Sep/19
     * @param value: Value map used in path propagation. It contains the input and output values.
     * @param function: Function used to initialize and propagate values.
     * @param uniform_adjacency: fixed number of neighbors adjacency.
     * @param uniform_adjs: Number of uniform adjacents to be used. The matrix may contain more.
     * @param variable_adjacency: variable number of neighbors adjacency.
     * @param Queue: Bucket queue to propagate the paths.
     * @return none.
     * @brief IFT clustering used by Knn OPF method.
     * @warning Must specify at least one of the adjacencies.
     */
     ClusteringIFT( C< D > &value, PathFunction< C, D > *function, const Matrix< size_t > &uniform_adjacency,
                    const size_t uniform_adjs, const Vector< Vector< size_t > > &variable_adjacency, 
                    BucketQueue *queue );

    /**
     * @date 2012/Sep/19
     * @param value: Value map used in path propagation. It contains the input and output values.
     * @param function: Function used to initialize and propagate values.
     * @param variable_adjacency: variable number of neighbors adjacency.
     * @param variable_adjs: Number of variable adjacents to be used. The matrix may contain more.
     * @param Queue: Bucket queue to propagate the paths.
     * @return none.
     * @brief IFT clustering used by LSH OPF method.
     * @warning none.
     */
    ClusteringIFT( C< D > &value, PathFunction< C, D > *function, const Vector< Vector< size_t > > &variable_adjacency,
                   const Vector< size_t > &variable_adjs, BucketQueue *queue );

    /**
     * @date 2012/Sep/19
     * @param none.
     * @return none.
     * @brief Runs degenerated IFT.
     * @warning none.
     */
    void Run( );

  };

}

#include "ClusteringIFT.cpp"

#endif
