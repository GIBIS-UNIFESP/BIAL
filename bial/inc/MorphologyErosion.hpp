/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/21 
 * @brief Morphological erosion over images. 
 */

#ifndef BIALMORPHOLOGYEROSION_H
#define BIALMORPHOLOGYEROSION_H

#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {

  template< class D >
  class Image;
  class Adjacency;
  /**
   * @brief  Morphological opperations over images. 
   */
  namespace Morphology {

    /**
     * @date 2012/Sep/21 
     * @param image: Input image. 
     * @return Erosion of input image. 
     * @brief Computes the erosion of input image using spherical adjacency relation of one pixel of radius. 
     * @warning none. 
     */
    template< class D >
    Image< D > Erode( const Image< D > &image );

    /**
     * @date 2015/Jun/16 
     * @param image: Input image. 
     * @param mask: Input mask.
     * @return Erosion of input image. 
     * @brief Computes the erosion of input image using spherical adjacency relation of one pixel of radius,
     * restricted to the input mask. 
     * @warning none. 
     */
    // template< class D >
    // Image< D > Erode( const Image< D > &image, const Image< D > &mask );

    /**
     * @date 2012/Sep/21 
     * @param image: Input image. 
     * @param adjacency: Adjacency relation used by erosion opperation. 
     * @return Erosion of input image. 
     * @brief Computes the erosion of input image using input adjacency relation. Interface to call multi-thread,
     * GPU, FPGA implementations. 
     * @warning none. 
     */
    template< class D >
    Image< D > Erode( const Image< D > &image, const Adjacency &adjacency );

    /**
     * @date 2013/Sep/25 
     * @param image: Input image. 
     * @param adjacency: Adjacency relation used by erosion opperation. 
     * @param result: resulting image. 
     * @param thread: Thread number. 
     * @param total_threads: Number of threads. 
     * @return Erosion of input image. 
     * @brief Computes the erosion of input image using input adjacency relation. Multi-thread implementation. 
     * @warning none. 
     */
    template< class D >
    void ErodeThreads( const Image< D > &image, const Adjacency &adjacency, Image< D > &result,
                              size_t thread, size_t total_threads );

    /**
     * @date 2015/Jun/16 
     * @param image: Input image. 
     * @param mask: Input mask.
     * @param adjacency: Adjacency relation used by erosion opperation. 
     * @param result: resulting image. 
     * @param thread: Thread number. 
     * @param total_threads: Number of threads. 
     * @return Erosion of input image. 
     * @brief Computes the erosion of input image restricted to input mask, using input adjacency
     * relation. Multi-thread implementation 
     * @warning none. 
     */
    // template< class D >
    // void ErodeMaskThreads( const Image< D > &image, const Image< D > &mask, const Adjacency &adjacency,
    //                               Image< D > &result, size_t thread, size_t total_threads );

    /**
     * @date 2015/Jun/16 
     * @param image: Input image. 
     * @param mask: Input mask.
     * @param adjacency: Adjacency relation used by erosion opperation. 
     * @return Erosion of input image. 
     * @brief Computes the erosion of input image, restricted to input mask, using input adjacency
     * relation. Interface to call multi-thread, GPU, FPGA implementations. 
     * @warning none. 
     */
    // template< class D >
    // Image< D > Erode( const Image< D > &image, const Image< D > &mask, const Adjacency &adjacency );

    /**
     * @date 2013/Dec/03 
     * @param image: Binary input image. 
     * @return Erosion of input binary image. 
     * @brief Computes the erosion of input image using spherical adjacency relation of one pixel of radius. 
     * @warning none. 
     */
    template< class D >
    Image< D > ErodeBin( const Image< D > &image, const Adjacency &adjacency );

    /**
     * @date 2013/Dec/03 
     * @param image: Input image. 
     * @param adjacency: Adjacency relation used by erosion opperation. 
     * @param seeds: list of propagation seeds. 
     * @param result: resulting image. 
     * @param thread: Thread number. 
     * @param total_threads: Number of threads. 
     * @return Erosion of input image. 
     * @brief Computes the erosion of input image using input adjacency relation. Multi-thread implementation. 
     * @warning none. 
     */
    template< class D >
    void ErodeBinThreads( const Image< D > &image, const Adjacency &adjacency, const Vector< size_t > &seeds,
                                 Image< D > &result, size_t thread, size_t total_threads );

  }

}

#include "MorphologyErosion.cpp"

#endif
