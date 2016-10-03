/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Jun/26 
 * @brief Functions for Feature I/O.
 */

#include "Common.hpp"

#ifndef BIALFILEFEATURE_H
#define BIALFILEFEATURE_H

#include "Feature.hpp"

namespace Bial {

  namespace FileFeature {

    /**
     * @date 2014/Oct/21 
     * @param filename: Name of input file with .opf extension. 
     * @return A feature vector. 
     * @brief Reads data from file and initializes a Feature vector with it. 
     * @warning none. 
     */
    template< class D >
    Feature< D > Read( const std::string &filename );

    /**
     * @date 2014/Oct/21 
     * @param filename: Name of input file with .opf extension. 
     * @return none. 
     * @brief Writes data to file. 
     * @warning none. 
     */
    template< class D >
    void Write( const Feature< D > &feat, const std::string &filename );

  }

}

#include "FileFeature.cpp"

#endif
