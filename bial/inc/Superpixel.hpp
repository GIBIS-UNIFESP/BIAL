/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2015/Sep/22
 * @brief BIAL_Superpixel class. Functions to call SLIC superpixel routines.
 */

#ifndef BIALSUPERPIXEL_H
#define BIALSUPERPIXEL_H

#include "Common.hpp"
#include "SLIC.hpp"

namespace Bial {

  template< class D >
  class Image;
  union Color;

  class Superpixel {

  public:
    
    /**
     * @date 2015/Set/22
     * @param img: Input 2D color image.
     * @param sup_pixel_size: Number of expected pixels inside each superpixel
     * @param compactness: The relative importance between color similarity (lower values) and spatial 
     *                     proximity (higher values).
     * @return Superpixel labeled image.
     * @brief Runs superpixel algorithm and returns the labels. Also img is returned with superpixel borders over
     *        the original content.
     * @warning Note that img returns differently from SuperVoxel function.
     */
    static Image< int > SuperPixel( Image< Color > &img, int sup_pixel_size, double compactness );

    /**
     * @date 2015/Set/22
     * @param img: Input monochromatic 3D image.
     * @param sup_voxel_size: Number of expected pixels inside each supervoxel
     * @param compactness: The relative importance between color similarity (lower values) and spatial 
     *                     proximity (higher values).
     * @return Supervoxel labeled image.
     * @brief Runs supervoxel algorithm and returns the labels. Also img is returned with supervoxel borders.
     * @warning Note that img returns differently from SuperPixel function.
     */
    template< class D >
    static Image< int > SuperVoxel( Image< D > &img, int sup_voxel_size, double compactness );
  };

}

#include "Superpixel.cpp"

#endif
