/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/11 
 * @brief Functions to write signal into file.
 */

#ifndef BIALFILESIGNAL_H
#define BIALFILESIGNAL_H

#include "Common.hpp"

namespace Bial {

  class Signal;

  /**
   * @date 2014/Feb/12 
   * @param filename: A filename. 
   * @return none. 
   * @brief Writes signal containts file. It writes a pair composed by frequency and value per line. 
   * @warning none. 
   */
  void Write( const Signal &sgn, const std::string &filename );

}

#include "FileSignal.cpp"

#endif
