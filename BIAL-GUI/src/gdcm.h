/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @file GDCM.hpp
 * @author Lucas Santana Lellis
 * @date 2014/Dec/3
 * @version 1.0.00
 * @brief
 * Content: Bial GDCM class
 * <br> Description: Wrapper to the GDCM library
 */
#include "Common.hpp"
#include "Image.hpp"
#include "MultiImage.hpp"
#include "guiimage.h"
#ifndef BIALGDCM_HPP
#define BIALGDCM_HPP
namespace gdcm {
  class Image;
}

class GDCM {
public:
  static Bial::MultiImage OpenGImage( const std::string &filename );
};

#endif /* BIALGDCM_HPP */
