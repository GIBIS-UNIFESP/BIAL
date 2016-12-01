/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Sep/08
 * @brief Degenerated IFT algorithm. Used for segmentation by MST method.
 */

#include "Image.hpp"
#include "Vector.hpp"

#ifndef BIALDEGENERATEDIFT_H
#define BIALDEGENERATEDIFT_H

#include "BucketQueue.hpp"
#include "PathFunction.hpp"

namespace Bial {

  template< template< class D > class C, class D >
  class DegeneratedIFT {

  protected:

    /** @brief Degenerated IFT attributes. */
    BucketQueue *queue;
    C< D > &value;
    PathFunction< C, D > *function;
    C< int > *label;
    C< int > *predecessor;
    long double bucket_size;
    typename PathFunction< C, D >::RemoveFn RemoveData;
    typename PathFunction< C, D >::UpdateFn UpdateData;

  public:

    /**
     * @date 2012/Sep/19
     * @param value: Value map used in path propagation. It contains the input and output values.
     * @param function: Function used to initialize and propagate values.
     * @param seed: A boolean Vector indicating the seeds.
     * @param label: label map.
     * @param predecessor: predecessor map.
     * @param sequential_label: If true, automatically assigns and increases labels as roots are removed from queue.
     * @param bucket_size: Size of a bucket in the bucket queue.
     * @param fifo_tie: true for fifo tiebreak, and false for lifo tiebreak.
     * @return none.
     * @brief Constructor. Depending on the provided maps, propagation function is different.
     * @warning none.
     */
    DegeneratedIFT( C< D > &value_image, PathFunction< C, D > *function, const Vector< bool > *seed = nullptr,
                    C< int > *label = nullptr, C< int > *predecessor = nullptr, bool sequential_label = false,
                    long double bucket_size = 1.0, bool fifo_tie = true );

    /**
     * @date 2012/Sep/19
     * @param none.
     * @return none.
     * @brief Destructor. Destroys the bucket queue.
     * @warning none.
     */
    ~DegeneratedIFT( );

    /**
     * @date 2012/Sep/19
     * @param none.
     * @return none.
     * @brief Runs degenerated IFT.
     * @warning none.
     */
    void Run( );

    /**
     * @date 2013/Dec/04
     * @param seed: A boolean Vector indicating the seeds.
     * @param bucket_size: Size of a bucket in the bucket queue.
     * @param sequential_label: If true, automatically assigns and increases labels as roots are removed from queue.
     * @param fifo_tie: true for fifo tiebreak, and false for lifo tiebreak.
     * @return none.
     * @brief Inserts the seeds into the bucket queue. Mostly used to continue IFT in multiple iterations.
     * @warning none.
     */
    void InsertSeeds( const Vector< bool > &seed );


  protected:

    /**
     * @date 2013/Dec/04
     * @param seed: A boolean Vector indicating the seeds.
     * @param bucket_size: Size of a bucket in the bucket queue.
     * @param sequential_label: If true, automatically assigns and increases labels as roots are removed from queue.
     * @param fifo_tie: true for fifo tiebreak, and false for lifo tiebreak.
     * @return none.
     * @brief Initialization of all IFT functionality including path function and bucket queue.
     * @warning none.
     */
    void Initialize( const Vector< bool > *seed, bool sequential_label, ldbl bucket_size, bool fifo_tie );

  };

}

#include "DegeneratedIFT.cpp"

#endif
