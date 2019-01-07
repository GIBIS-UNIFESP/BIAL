/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @file Common.hpp
 * @author Fábio Augusto Menocci Cappabianco
 * @date 2012/Jun/21
 * @version 1.0.00
 * @brief
 * Content: Commonly used macros, types, static variables and functions.
 * <br> Description: Some macros used by many classes.
 */

#ifndef BIALCOMMON_H
#define BIALCOMMON_H

#ifndef __cplusplus
#define __cplusplus
#endif

#define _USE_MATH_DEFINES

#include <algorithm>
#include <array>
#include <cfloat>
#include <cmath>
#include <complex>
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <future>
#include <iomanip>
#include <iostream>
#include <limits>
#include <mutex>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <system_error>
#include <thread>
#include <tuple>
#include <typeinfo>
#include <vector>

/* Defining smaller names for some types */
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef long long llint;
typedef unsigned long ulint;
typedef unsigned long long ullint;
typedef long double ldbl;
typedef std::complex< double > bial_complex;
/* Defining the standard output functions. */

/**
 * @defgroup macros Bial macros
 * @{
 * @def COMMENT( exp, num )
 *
 * @brief
 * Use COMMENT to write comments throughout your code, in order to keep it self explanatory.<BR>
 * Use instead of C or C++ default comments in the begining of code blocks to describe what the block is expected to
 * do.<BR>
 * If you want to make a small comment in the end of a line, then, do not use COMMENT.<BR>
 * COMMENT was designed to both describe the code and for debugging purposes. Therefore, use it strictly according to
 * the instructions here to avoid having a code that is unclear or too verbose.<BR>
 *
 *
 * COMMENT has two parameters. The first is a stream containg the comment describing what the block does, and possibly
 * print variable's values for debugging. The second parameter is the verbosity level. Level 0, means that the
 * comment will always be printed in debugging mode. Higher levels may or may not be printed, depending on the
 * verbosity level selected during code compilation.<BR>
 *
 *
 * Therefore, follow the instructions to select the level of verbosity of your comment:<BR>
 *  - <b>Level 0</b> -> Basically a comment that you would make in your code describing high level blocks, that are not
 * often
 * repeated (less than 5 times) for the main purpuses of your code.<BR>
 *  - <b>Level 1</b> -> Also comments describing blocks that may repeaty a few times (less than 20). This level may also
 * contain debugging information as the value of a variable or a small container that can be written in few lines
 * (less than 5). Exception handling comments should be set to this verbosity level.<BR>
 *  - <b>Level 2</b> -> Comments that may be repeated several times (less than 100). This verbosity level helps to
 * identify
 * bugs that happen in some loops without blocking the std::out for too long. Ou may also print variables or containers
 * that use several lines (less than 20);<BR>
 *  - <b>Level 3</b> -> Comments that are exausted repeated or data with possibly more than 20 lines. The output for
 * this
 * verbosity level might require to be piped to a file.<BR>
 * If you are not sure between two verbosity levels, pick the greater. If you have no idea about what the verbosity
 * level is, please select 3 or 4, indicating that you postponed the decision.
 *  - <b>Level 4</b> -> Comments that mostly no one would like to print. They will generate thousands of debuging lines
 * with
 * enabled. Use it also if this comment is merely descriptive of less important part of the code.<BR>
 *
 * Here follow some examples. Note that they are hypothetical, because this depends on the context.<BR>
 * @code
 *    COMMENT( "Segmenting brain tissues.", 0 );
 *    COMMENT( "Computing the median value of feature vector.", 1 ); // Supposing you have 4 or less feature vectors.
 *    COMMENT( "Tissue mean value." << mean, 1 ); // Mean value is computed a few times.
 *    COMMENT( "Printing the patient's information.", 2 );
 *    COMMENT( "Starting clustering with k neighbors.", 2 ); //k varies from 10 to 30.
 *    COMMENT( "pxl: " << plx, 3 ); // inside triple for loop or printing all pixel of 3D image.
 *    COMMENT( "Computing sum of intensities of each pixel." <<, 4 ); // inside loop for all pixel of an image.
 * @endcode
 *
 * To get verbose results while compiling your code, compile it setting BIAL_DEBUG with a non-negative value.
 * If you are using makefile in tst subfolder, type:
 * make <your_program> verbose=<level>
 * where <your_program> is the name of the program, and <level> is the verbosity level you want to print.
 * If you want to use a debugger of sanity checker along with BIAL comments, than compile with
 * make <your_program> debug=<level>
 * where again <your_program> is the name of the program, and <level> is the verbosity level you want to print.
 *
 *  @param exp Explanatory
 * @param num Verbosity level
 */

/**
 * \def BIAL_WARNING( exp )
 * @brief
 * Use BIAL_WARNING to print a message to the output stream warn the user that something bad may have happend, but the
 * program will continue to be executed.
 */

/**
 * \def BIAL_ERROR( exp )
 * @brief
 * Use BIAL_ERROR to compose runtime error messages. Note that ERROR generates a string, instead of a stream, because it
 * is better to throw an exception, rather than exiting the program. <br> This way, the exception may be threated by a
 * higher level program, like an user IDE.
 * @}
 */

/**
 * @brief The Bial namespace holds all library functions.
 */
namespace Bial {

  /**
   * @brief Determines the COMMENT macro verbosity.
   */
  extern int verbose;
}
#ifdef BIAL_DEBUG
#define COMMENT( exp, num )                                             \
  if( Bial::verbose > num ) {						\
    std::cerr << __FILE__ << ": " << __LINE__ << ": " << __FUNCTION__ << " => " << exp << std::endl; }
#else
#define COMMENT( exp, num ) { }
#define DEBUG_WRITE( obj, exp, num ) { }
#endif

#define BIAL_WARNING( exp )						\
  std::cerr << __FILE__ << ": " << __LINE__ << ": " << __FUNCTION__ << " Warning: " << exp << std::endl;

#define BIAL_ERROR( exp )						\
  std::string( __FILE__ ) + ": " + std::to_string( __LINE__ ) + ": " + std::string( __FUNCTION__ ) + ": Error: " + exp

/* Macros _At_ and QK_... translated as _data.at( exp ) in debug mode or as qk_data[ exp ] in execution mode. */
#ifdef BIAL_DEBUG
#define QK_DATA( elm ) _data.at( elm )
#else
#define QK_DATA( elm ) qk_data[ elm ]
#endif

#ifdef BIAL_DEBUG
#define IF_DEBUG( exp ) if( exp )
#else
#define IF_DEBUG( exp ) if( false )
#endif



#define BIAL_ASSERT( value, exp )                                       \
  { if( !( value ) ) {                                                  \
      throw std::logic_error( BIAL_ERROR( std::string( "ASSERT failed : " ) + exp ) ) } }
/**
 * @defgroup common Bial common
 * @brief Bial global functions and classes
 * @{
 */

namespace Bial {

  /* Defining types for iterators. */

  /**
   * @brief  Commonly used macros, types, static variables and functions.
   */
  class Common {

  public:

    /**
     * @date 2014/May/01
     * @param randomize: Fixed or variable pseudo-random sampling.
     * @return none.
     * @brief Generates numbers in pseudo-random or fixed sequence.
     * @warning none.
     */
    static void Randomize( bool random ) {
      if( random ) {
        srand( time( NULL ) );
      }
      else {
        srand( 0 );
      }
    }

  };

  const double Pi = std::atan( 1.0 ) * 4.0;

  /**
   * @date 2015/Jul/22
   * @version 1.0.00
   * @brief Useful function to compare floating point numbers.
   * @warning none.
   */
  inline bool Equals( float value, float other, float factor ) {
    return( ( value >= ( other - factor ) ) && ( value < other + factor ) );
  }
  inline bool Equals( double value, double other, double factor ) {
    return( ( value >= ( other - factor ) ) && ( value < other + factor ) );
  }

  /**
   * @date 2015/Jul/22
   * @version 1.0.00
   * @brief Useful function to convert degrees ro radians.
   * @warning none.
   */
  inline float Radians( float angle ) {
    return( angle * ( Pi / 180.f ) );
  }
  inline double Radians( double angle ) {
    return( angle * ( Pi / 180.0 ) );
  }

  /**
   * @date 2015/Jul/22
   * @version 1.0.00
   * @brief Useful function to convert radians to degrees.
   * @warning none.
   */
  inline float Degrees( float rad ) {
    return( rad * ( 180.f / Pi ) );
  }
  inline double Degrees( double rad ) {
    return( rad * ( 180.0 / Pi ) );
  }

  /**
   * @date 2013/Oct/29
   * @version 1.0.00
   * @brief Same as popen for Linux and Apple, and _popen for
   * Windows.
   * @warning none.
   */
  FILE* Bial_popen( const char *command, const char *type );

  /**
   * @date 2013/Oct/29
   * @version 1.0.00
   * @brief Same as pclose for Linux and Apple, and _pclose for
   * Windows.
   * @warning none.
   */
  int Bial_pclose( FILE *stream );

  /**
   * @date 2013/Oct/29
   * @version 1.0.00
   * @param dirname: name of a directory.
   * @return A command for terminal to check if directory exists.
   * @brief Generates a command for terminal to check if
   * directory exists.
   * @warning none.
   */
  std::string DirectoryExistsCommand( const std::string &dirname );

  /**
   * @date 2013/Oct/29
   * @version 1.0.00
   * @param dirname: name of a directory.
   * @return A command for terminal to list files and directories in given directory.
   * @brief Generates a command for terminal to list files and directories in given directory.
   * @warning none.
   */
  std::string FileDirectoryListCommand( const std::string &dirname );

  /**
   * @date 2013/Oct/29
   * @version 1.0.00
   * @param dirname: name of a directory.
   * @return A command for terminal to list subdirectories in given directory.
   * @brief Generates a command for terminal to list subdirectories in given directory.
   * @warning none.
   */
  std::string DirectoryListCommand( const std::string &dirname );

  /**
   * @date 2013/Oct/29
   * @version 1.0.00
   * @param dirname: name of a directory.
   * @return A command for terminal to list files in given directory.
   * @brief Generates a command for terminal to list files in given directory.
   * @warning none.
   */
  std::string FileListCommand( const std::string &dirname );

  /**
   * @date 2015/May/25
   * @version 1.0.00
   * @param dirname: name of a directory.
   * @param extension: a file extension.
   * @return A command for terminal to list files in given directory with a given extension.
   * @brief Generates a command for terminal to list files in given directory with a given extension.
   * @warning none.
   */
  std::string FileListCommand( const std::string &dirname, const std::string &extension );

  /**
   * @date 2015/Apr/18
   * @version 1.0.00
   * @param dirname: name of a directory.
   * @return A command for terminal to list files in given directory.
   * @brief Generates a command for terminal to create a directory.
   * @warning none.
   */
  std::string MakeDirCommand( const std::string &dirname );

#ifdef _WIN32
#define IS_WINDOWS_PLATFORM
#endif
#ifdef _WIN64
#define IS_WINDOWS_PLATFORM
#endif

#ifdef IS_WINDOWS_PLATFORM
#define DIR_SEPARATOR '\\'
#else
#define DIR_SEPARATOR '/'
#endif
}

#ifdef BIAL_DEBUG
#include "FileImage.hpp"
#endif

/**
 * }@
 */

// #include "Adjacency.hpp"
// #include "Array.hpp"
// #include "Binary.hpp"
// #include "Bit.hpp"
// #include "Brain.hpp"
// #include "BucketQueue.hpp"
// #include "Color.hpp"
// #include "Common.hpp"
// #include "Dicomdir.h"
// #include "DicomHeader.hpp"
// #include "DiffusionFunction.hpp"
// #include "DistanceFunction.hpp"
// #include "Draw.hpp"
// #include "Edge.hpp"
// #include "Feature.hpp"
// #include "File.hpp"
// #include "Filtering.hpp"
// #include "FuzzyCMeans.hpp"
// #include "GDCM.hpp"
// #include "Geometrics.hpp"
// #include "Gradient.hpp"
// #include "Graph.hpp"
// #include "gzstream.hpp"
// #include "Heart.hpp"
// #include "Hough.hpp"
// #include "IFT.hpp"
// #include "Image.hpp"
// #include "Integral.hpp"
// #include "Intensity.hpp"
// #include "Kernel.hpp"
// #include "Limit.hpp"
// #include "MarchingCubes.hpp"
// #include "Matrix.hpp"
// #include "MinimumSpanningTree.hpp"
// #include "Morphology.hpp"
// #include "MRIModality.hpp"
// #include "NiftiHeader.hpp"
// #include "OPF.hpp"
// #include "Optimization.hpp"
// #include "PathFunction.hpp"
// #include "Plotting.hpp"
// #include "PNMHeader.hpp"
// #include "RandomQueue.hpp"
// #include "Sample.hpp"
// #include "Segmentation.hpp"
// #include "Signal.hpp"
// #include "Sorting.hpp"
// #include "SpatialTransform.hpp"
// #include "Static.hpp"
// #include "Statistics.hpp"
// #include "Table.hpp"
// #include "TestHelper.hpp"
// #include "Transform.hpp"
// #include "Vector.hpp"

#endif
