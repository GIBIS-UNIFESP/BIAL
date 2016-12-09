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
    bool dift_enb;
    size_t dift_elm;

  public:

    /**
     * @date 2012/Sep/19
     * @param value: Value map used in path propagation. It contains the input and output values.
     * @param mask: Valid image domain. Pixels out of the domain (i.e. mask = 0), are not computed.
     * @param adjacency: adjacency relation defining neighborhood. Assumes central element being at position 0.
     * @param function: Function used to initialize and propagate values.
     * @param seed: A boolean Vector indicating the seeds.
     * @param label_image: label map.
     * @param predecessor_image: predecessor map.
     * @param bucket_size: Size of a bucket in the bucket queue.
     * @param fifo_tie: true for fifo tiebreak, and false for lifo tiebreak.
     * @return none.
     * @brief Constructor of IFT object to run over images.
     * @warning Input image and adjacency must have compatible dimensions. Label, and predecessor maps are
     * optional.
     */
    ImageIFT( Image< D > &value, const Adjacency &adjacency, PathFunction< Image, D > *function,
              const Vector< bool > *seed = nullptr, Image< int > *label = nullptr,
              Image< int > *predecessor = nullptr, bool sequential_label = false, 
              long double bucket_size = 1.0, bool fifo_tie = true );

    /**
     * @date 2012/Sep/19
     * @param value: Value map used in path propagation. It contains the input and output values.
     * @param mask: Valid image domain. Pixels out of the domain (i.e. mask = 0), are not computed.
     * @param adjacency: adjacency relation defining neighborhood. Assumes central element being at position 0.
     * @param function: Function used to initialize and propagate values.
     * @param minimum_value: Minimum value to be processed by IFT.
     * @param value_range: The total range from minimum to maximum value processed by IFT.
     * @param seed: A boolean Vector indicating the seeds.
     * @param label_image: label map.
     * @param predecessor_image: predecessor map.
     * @param fifo_tie: true for fifo tiebreak, and false for lifo tiebreak.
     * @return none.
     * @brief Constructor of IFT object to run over images.
     * @warning Input image and adjacency must have compatible dimensions. Label, and predecessor maps are
     * optional.
     */
    ImageIFT( Image< D > &value, const Adjacency &adjacency, PathFunction< Image, D > *function, D minimum_value, 
              size_t value_range, const Vector< bool > *seed = nullptr, Image< int > *label = nullptr,
              Image< int > *predecessor = nullptr, bool sequential_label = false, bool fifo_tie = true );

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
     * @brief Enables differential IFT. Runs until target element is reached.
     * @warning none.
     */
    void EnableDifferentialIFT( size_t elm );

    /**
     * @date 2015/07/18
     * @param none.
     * @return none.
     * @brief Disables differential IFT. Runs until all elements leave the queue.
     * @warning none.
     */
    void DisableDifferentialIFT( );

    // template< class D, class D2 >
    // static void ImageIFT( Image< D > &value, const Image< D2 > &mask, const Adjacency &adjacency,
    //                       PathFunction< Image, D > *function, const Vector< bool > *seed = nullptr,
    //                       Image< int > *label = nullptr, Image< int > *predecessor = nullptr, 
    //                       bool sequential_label = false, long double bucket_size = 1.0, bool fifo_tie = true );

  };

}

#include "ImageIFT.cpp"

#endif
