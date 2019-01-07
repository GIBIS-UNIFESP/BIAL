/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @file PNMHeader.hpp
 * @author Fábio Augusto Menocci Cappabianco 
 * @date 2013/Aug/03 
 * @version 1.0.00
 * @brief
 * Content: PNM Header class 
 * <br> Description: PNM Header class. 
 * <br> Future add-on's: Read function, or read string constructor. 
 */

#include "Common.hpp"
#include "Vector.hpp"

#ifndef BIALPNMHEADER_H
#define BIALPNMHEADER_H

namespace Bial {
  /**
   * @brief PNM Header class. 
   */
  class PNMHeader {
  private:
    Vector< std::string > comment;
  public:

    /**
     * @date 2013/Aug/06 
     * @param cmt: Comment contents 
     * @param line: Comment line 
     * @return none. 
     * @brief Set comment of line 'line' with contents cmt. 
     * @warning May overwrite older comments. 
     */
    void Comment( const std::string &cmt, const size_t line );

    /**
     * @date 2013/Aug/06 
     * @param line: Comment line 
     * @return Comment contents from line 'line'. 
     * @brief Returns comment contents from line 'line'. 
     * @warning none. 
     */
    std::string Comment( const size_t line ) const;

    /**
     * @date 2013/Aug/06 
     * @param none. 
     * @return The line of the first comment. 
     * @brief Returns the line of the first comment. If there
     * are no comments, returns -1. 
     * @warning none. 
     */
    int First( ) const;

    /**
     * @date 2013/Aug/06 
     * @param none. 
     * @return The line of the last comment. 
     * @brief Returns the line of the last comment. If there are
     * no comments, returns -1. 
     * @warning none. 
     */
    int Last( ) const;
  };

}

#endif
