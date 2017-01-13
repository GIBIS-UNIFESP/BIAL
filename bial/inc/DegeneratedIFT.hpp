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
    //typename PathFunction< C, D >::RemoveFn RemoveData;
    //typename PathFunction< C, D >::UpdateFn UpdateData;

  public:

    /**
     * @date 2012/Sep/19
     * @param value: Value map used in path propagation. It contains the input and output values.
     * @param function: Function used to initialize and propagate values.
     * @param Queue: Bucket queue to propagate the paths.
     * @return none.
     * @brief Constructor.
     * @warning none.
     */
    DegeneratedIFT( C< D > &value_image, PathFunction< C, D > *function, BucketQueue *queue );

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
     * @return none.
     * @brief Inserts the seeds into the bucket queue.
     * @warning none.
     */
    void InsertSeeds( const Vector< bool > &seed );

  };

}

#include "DegeneratedIFT.cpp"

#endif


  // protected:

  //   /**
  //    * @date 2013/Dec/04
  //    * @param seed: A boolean Vector indicating the seeds.
  //    * @param bucket_size: Size of a bucket in the bucket queue.
  //    * @param fifo_tie: true for fifo tiebreak, and false for lifo tiebreak.
  //    * @return none.
  //    * @brief Initialization of all IFT functionality including path function and bucket queue.
  //    * @warning none.
  //    */
  //   void Initialize( const Vector< bool > *seed, ldbl bucket_size, bool fifo_tie );

  //   /**
  //    * @date 2013/Dec/04
  //    * @param seed: A boolean Vector indicating the seeds.
  //    * @param minimum_value: Minimum value to be processed by IFT.
  //    * @param value_range: The total range from minimum to maximum value processed by IFT.
  //    * @param fifo_tie: true for fifo tiebreak, and false for lifo tiebreak.
  //    * @return none.
  //    * @brief Initialization of all IFT functionality including path function and bucket queue.
  //    * @warning none.
  //    */
  //   void Initialize( const Vector< bool > *seed, ldbl minimum_value, size_t value_range, 
  //                    bool fifo_tie );
