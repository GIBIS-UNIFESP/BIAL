/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2015/Aug/21
 * @brief Algorithms to check if tow images have the same dimensions and containts.
 */

#ifndef BIALIMAGEEQUALS_H
#define BIALIMAGEEQUALS_H

#include "Common.hpp"
#include "Image.hpp"

namespace Bial {

  namespace ImageOp {

    /**
     * @date 2015/Aug/21
     * @param file1 First image path.
     * @param file2 Second image path.
     * @return Returns true if images are accessible and equal both in size and contents.
     * @brief Loads and compares two images.
     * classes.
     * @warning none.
     */
    bool EqualImages( const std::string &file1, const std::string &file2 );

    /*
     * @date 2015/Aug/21
     * @param file1 First image.
     * @param file2 Second image.
     * @return Returns true if images are equal both in size and contents.
     * @brief Compares two image contents.
     * @warning none.
     */
    template< class D, class D2 >
    bool EqualImages( const Image< D > &img1, const Image< D2 > &img2 );

  }

}

#include "ImageEquals.cpp"

#endif /* TESTHELPER_HPP */
