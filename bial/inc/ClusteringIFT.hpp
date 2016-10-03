/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Sep/08
 * @brief IFT algorithm for clustering purposes.
 */

#ifndef BIALCLUSTERINGIFT_H
#define BIALCLUSTERINGIFT_H

#include "Adjacency.hpp"
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
     * @param seed: A boolean Vector indicating the seeds.
     * @param label: label map.
     * @param predecessor: predecessor map.
     * @param bucket_size: Size of a bucket in the bucket queue.
     * @param fifo_tie: true for fifo tiebreak, and false for lifo tiebreak.
     * @return none.
     * @brief IFT clustering used by Knn OPF method.
     * @warning Label, and predecessor maps are optional. Must specify at least one of the adjacencies.
     */
     ClusteringIFT( C< D > &value, PathFunction< C, D > *function, const Matrix< size_t > &uniform_adjacency,
                    const size_t uniform_adjs, const Vector< Vector< size_t > > &variable_adjacency,
                    const Vector< bool > *seed = nullptr, C< int > *label = nullptr, C< int > *predecessor = nullptr,
                    bool sequential_label = false,  long double bucket_size = 1.0, bool fifo_tie = true );

    /**
     * @date 2012/Sep/19
     * @param value: Value map used in path propagation. It contains the input and output values.
     * @param function: Function used to initialize and propagate values.
     * @param variable_adjacency: variable number of neighbors adjacency.
     * @param variable_adjs: Number of variable adjacents to be used. The matrix may contain more.
     * @param seed: A boolean Vector indicating the seeds.
     * @param label: label map.
     * @param predecessor: predecessor map.
     * @param bucket_size: Size of a bucket in the bucket queue.
     * @param fifo_tie: true for fifo tiebreak, and false for lifo tiebreak.
     * @return none.
     * @brief IFT clustering used by LSH OPF method.
     * @warning Label, and predecessor maps are optional. Must specify at least one of the adjacencies.
     */
    ClusteringIFT( C< D > &value, PathFunction< C, D > *function, const Vector< Vector< size_t > > &variable_adjacency,
                   const Vector< size_t > &variable_adjs, const Vector< bool > *seed = nullptr, 
                   C< int > *label = nullptr, C< int > *predecessor = nullptr, bool sequential_label = false, 
                   long double bucket_size = 1.0, bool fifo_tie = true );

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
