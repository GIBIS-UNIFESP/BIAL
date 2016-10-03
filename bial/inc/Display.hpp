/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2015/Oct/06 */
/* Content: Image display class implementation. */
/* Description: class to handle visualization. */

#include "Common.hpp"

#ifndef BIALDISPLAY_H
#define BIALDISPLAY_H

namespace Bial {

  class Display {
    /**
     * @date 2013/Aug/09
     * @param filename: File name.
     * @return std::string with command to display image file.
     * @brief Returns a std::string with the command to display the
     * image file.
     * @warning none.
     */
    static std::string ShowCommand( const std::string &filename );

    /**
     * @date 2013/Aug/09
     * @param filename: File name.
     * @return none.
     * @brief Opens image in display.
     * @warning none.
     */
    static void Show( const std::string &filename );
  };

}

#include "Display.cpp"

#endif
