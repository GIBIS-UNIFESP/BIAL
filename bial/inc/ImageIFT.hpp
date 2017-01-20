/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Sep/08
 * @brief IFT algorithm running on Images.
 */

#include "Adjacency.hpp"
#include "Common.hpp"
#include "Image.hpp"
#include "Vector.hpp"

#ifndef BIALIMAGEIFT_H
#define BIALIMAGEIFT_H

#include "DegeneratedIFT.hpp"
#include "PathFunction.hpp"

namespace Bial {

  template< class D >
  class ImageIFT : public DegeneratedIFT< Image, D > {

    const Adjacency &adjacency;
    size_t stop_elm;

  public:

    /**
     * @date 2012/Sep/19
     * @param value: Value map used in path propagation. It contains the input and output values.
     * @param mask: Valid image domain. Pixels out of the domain (i.e. mask = 0), are not computed.
     * @param adjacency: adjacency relation defining neighborhood. Assumes central element being at position 0.
     * @param function: Function used to initialize and propagate values.
     * @param Queue: Bucket queue to propagate the paths.
     * @return none.
     * @brief Constructor of IFT object to run over images.
     * @warning Input image and adjacency must have compatible dimensions. Label, and predecessor maps are
     * optional.
     */
    ImageIFT( Image< D > &value, const Adjacency &adjacency, PathFunction< Image, D > *function, BucketQueue *queue );

    /**
     * @date 2012/Sep/19
     * @param none.
     * @return none.
     * @brief Runs IFT algorithm over image.
     * @warning none.
     */
    void Run( );

    /**
     * @date 2015/07/18
     * @param elm: Target element.
     * @return none.
     * @brief Enables stop element IFT. Runs until target element is reached.
     * @warning none.
     */
    void EnableStopElement( size_t elm );

    /**
     * @date 2015/07/18
     * @param none.
     * @return none.
     * @brief Disables stop element IFT. Runs until all elements leave the queue.
     * @warning none.
     */
    void DisableStopElement( );

    // template< class D, class D2 >
    // static void ImageIFT( Image< D > &value, const Image< D2 > &mask, const Adjacency &adjacency,
    //                       PathFunction< Image, D > *function, const Vector< bool > *seed = nullptr,
    //                       long double bucket_size = 1.0, bool fifo_tie = true );

  };

}

#include "ImageIFT.cpp"

#endif
