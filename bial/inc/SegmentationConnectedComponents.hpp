/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2016/Jun/27 
 * @brief Find the connected components and select some among them.
 */

#ifndef BIALSEGMENTATIONCONNECTEDCOMPONENTS_H
#define BIALSEGMENTATIONCONNECTEDCOMPONENTS_H

#include "Common.hpp"

namespace Bial {

  class Adjacency;
  template< class D >
  class Image;

  namespace Segmentation {
    
    /**
     * @date 2016/Jun/27 
     * @param input: Input image.
     * @param adj: An adjacency relation.
     * @return Image with each connected component labeled with a distinct label.
     * @brief Returns an image with each connected component labeled with a distinct label. Connectness is defined *
     * according to given adjacency relation. Every non-zero intensity pixel is considered belonging to the object. Only *
     * zere intensity pixels belong to the background.
     * @warning none. 
     */
    template< class D >
    Image< int > ConnectedComponents( const Image< D > &input, const Adjacency &adj );

    /**
     * @date 2016/Jun/27 
     * @param input: Input labeled component image.
     * @param fraction: The fraction of components that should be removed.
     * @return Labeled component image containing only the largest compoenents, according to the given fraction.
     * @brief The given fraction refers to the total number of pixels in the components, not to the number of *
     * components itself.
     * @warning none. 
     */
    Image< int > RemoveSmallComponents( const Image< int > &label, float fraction );

  }

}

#include "SegmentationConnectedComponents.cpp"

#endif
