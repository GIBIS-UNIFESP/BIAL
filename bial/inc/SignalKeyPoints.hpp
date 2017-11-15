/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/11 
 * @brief Functions to locate key points in signal such as peaks, acclivities, plateaus, etc.
 */

#ifndef BIALSIGNALKEYPOINTS_H
#define BIALSIGNALKEYPOINTS_H

#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {

  class Signal;

  namespace SignalOp {

    /**
     * @date 2012/Sep/11
     * @param sgn: Input signal.
     * @param min_bin: minimum bin range. 
     * @param max_bin: maximum bin range. 
     * @return The bin of the highest peak. 
     * @brief Finds the highest peak from bin min_bin to bin max_bin. 
     * @warning none. 
     */
    size_t Peak( const Signal &sgn, size_t min_bin, size_t max_bin );

    /**
     * @date 2014/Jan/23 
     * @param sgn: Input signal.
     * @param min_bin: minimum bin range. 
     * @param max_bin: maximum bin range. 
     * @param kernel_size: Size of used kernel. 
     * @return A vector with the local peaks. 
     * @brief Finds the bins representing the local maxima with respect to their adjacents in kernel range. 
     * @warning none. 
     */
    Vector< size_t > Peaks( const Signal &sgn, size_t min_bin, size_t max_bin, size_t kernel_size );

    /**
     * @date 2012/Sep/11 
     * @param sgn: Input signal.
     * @param signal: Input signal. 
     * @param start_bin: Bin of reference. 
     * @param kernel_size: Size of used kernel. 
     * @return The bin of the first peak to the right from start_bin at last as wide as kernel_size. 
     * @brief Finds the first peak to the right of start_bin using a kernel of size kernel_size to validade the
     * peak. Peaks that are narrower than kernel_size are ignored. 
     * @warning none. 
     */
    size_t FirstRightPeak( const Signal &sgn, size_t start_bin, size_t kernel_size );

    /**
     * @date 2012/Sep/14 
     * @param sgn: Input signal.
     * @param signal: Input signal. 
     * @param start_bin: Bin of reference. 
     * @param end_bin: Limit for the search. 
     * @param kernel_size: Size of used kernel. 
     * @return The bin of the first peak to the right from start_bin to end_bin at last as wide as kernel_size. 
     * @brief Finds the first peak to the right of start_bin up to end_bin using a kernel of size kernel_size to
     * validade the peak. Peaks that are narrower than kernel_size are ignored. 
     * @warning none. 
     */
    size_t FirstRightPeak( const Signal &sgn, size_t start_bin, size_t end_bin, size_t kernel_size );

    /**
     * @date 2012/Sep/11 
     * @param sgn: Input signal.
     * @param signal: Input signal. 
     * @param start_bin: Bin of reference. 
     * @param kernel_size: Size of used kernel. 
     * @return The bin of the first peak to the left from start_bin at last as wide as kernel_size. 
     * @brief Finds the first peak to the left of start_bin using a kernel of size kernel_size to validade the
     * peak. Peaks that are narrower than kernel_size are ignored. 
     * @warning none. 
     */
    size_t FirstLeftPeak( const Signal &sgn, size_t start_bin, size_t kernel_size );

    /**
     * @date 2012/Sep/14 
     * @param sgn: Input signal.
     * @param signal: Input signal. 
     * @param start_bin: Bin of reference. 
     * @param end_bin: Limit for the search. 
     * @param kernel_size: Size of used kernel. 
     * @return The bin of the first peak to the left from start_bin to end_bin at last as wide as kernel_size. 
     * @brief Finds the first peak to the left of start_bin up to end_bin using a kernel of size kernel_size to
     * validade the peak. Peaks that are narrower than kernel_size are ignored. 
     * @warning none. 
     */
    size_t FirstLeftPeak( const Signal &sgn, size_t start_bin, size_t end_bin, size_t kernel_size );

    /**
     * @date 2012/Sep/11 
     * @param sgn: Input signal.
     * @param min_bin: minimum bin range. 
     * @param max_bin: maximum bin range. 
     * @return The bin of the lowest valley. 
     * @brief Finds the lowest valley from bin min_bin to bin max_bin. 
     * @warning none. 
     */
    size_t Valley( const Signal &sgn, size_t min_bin, size_t max_bin );

    /**
     * @date 2014/Jan/23 
     * @param sgn: Input signal.
     * @param min_bin: minimum bin range. 
     * @param max_bin: maximum bin range. 
     * @param kernel_size: Size of used kernel. 
     * @return A vector with the local valleys. 
     * @brief Finds the bins representing the local minima with respect to their adjacents in kernel range. 
     * @warning none. 
     */
    Vector< size_t > Valleys( const Signal &sgn, size_t min_bin, size_t max_bin, size_t kernel_size );

    /**
     * @date 2012/Sep/11 
     * @param sgn: Input signal.
     * @param signal: Input signal. 
     * @param start_bin: Bin of reference. 
     * @param kernel_size: Size of used kernel. 
     * @return The bin of the first valley to the right from start_bin at last as wide as kernel_size. 
     * @brief Finds the first valley to the right of start_bin using a kernel of size kernel_size to validade the
     * valley. Valleys that are narrower than kernel_size are ignored. 
     * @warning none. 
     */
    size_t FirstRightValley( const Signal &sgn, size_t start_bin, size_t kernel_size );

    /**
     * @date 2012/Sep/11 
     * @param sgn: Input signal.
     * @param signal: Input signal. 
     * @param start_bin: Bin of reference. 
     * @param end_bin: Limit for the search. 
     * @param kernel_size: Size of used kernel. 
     * @return The bin of the first valley to the right from start_bin to end_bin at last as wide as kernel_size. 
     * @brief Finds the first valley to the right of start_bin up to end_bin using a kernel of size kernel_size to
     * validade the valley. Valleys that are narrower than kernel_size are ignored. 
     * @warning none. 
     */
    size_t FirstRightValley( const Signal &sgn, size_t start_bin, size_t end_bin, size_t kernel_size );

    /**
     * @date 2012/Sep/11 
     * @param sgn: Input signal.
     * @param signal: Input signal. 
     * @param start_bin: Bin of reference. 
     * @param kernel_size: Size of used kernel. 
     * @return The bin of the first valley to the left from start_bin at last as wide as kernel_size. 
     * @brief Finds the first valley to the left of start_bin using a kernel of size kernel_size to validade the
     * valley. Valleys that are narrower than kernel_size are ignored. 
     * @warning none. 
     */
    size_t FirstLeftValley( const Signal &sgn, size_t start_bin, size_t kernel_size );

    /**
     * @date 2012/Sep/11 
     * @param sgn: Input signal.
     * @param signal: Input signal. 
     * @param start_bin: Bin of reference. 
     * @param end_bin: Limit for the search. 
     * @param kernel_size: Size of used kernel. 
     * @return The bin of the first valley to the left from start_bin to end_bin at last as wide as kernel_size. 
     * @brief Finds the first valley to the left of start_bin up to end_bin using a kernel of size kernel_size to
     * validade the valley. Valleys that are narrower than kernel_size are ignored. 
     * @warning none. 
     */
    size_t FirstLeftValley( const Signal &sgn, size_t start_bin, size_t end_bin, size_t kernel_size );

    /**
     * @date 2012/Sep/14 
     * @param sgn: Input signal.
     * @param start_bin: A bin tha lies before the acclivity. 
     * @param kernel_size: Size of used kernel. 
     * @param signal_delta_proportion: maximum signal proportion difference accepted. 
     * @return The index of the first bin that belongs to the acclivity to the right of start_bin. 
     * @brief Detects the first bin that belongs to an acclivity, to the right of start_bin, that is greater than
     * start_bin value * ( 1.0 + signal_delta_proportion ). Sets of consecutive bins that that are narrower
     * than kernel_size are ignored. 
     * @warning none. 
     */
    size_t FirstRightAcclivity( const Signal &sgn, size_t start_bin, size_t kernel_size, 
                                float signal_delta_proportion );

    /**
     * @date 2012/Sep/14 
     * @param sgn: Input signal.
     * @param start_bin: A bin tha lies before the acclivity. 
     * @param end_bin: Limit for the search. 
     * @param kernel_size: Size of used kernel. 
     * @param signal_delta_proportion: maximum signal proportion difference accepted. 
     * @return The index of the first bin that belongs to the acclivity to the right of start_bin and limited to
     * end_bin. 
     * @brief Detects the first bin that belongs to an acclivity, to the right of start_bin and limited to
     * end_bin, that is greater than start_bin value * ( 1.0 + signal_delta_proportion ). Sets of
     * consecutive bins that that are narrower than kernel_size are ignored. 
     * @warning none. 
     */
    size_t FirstRightAcclivity( const Signal &sgn, size_t start_bin, size_t end_bin, size_t kernel_size,
                                float signal_delta_proportion );

    /**
     * @date 2012/Sep/14 
     * @param sgn: Input signal.
     * @param start_bin: A bin tha lies before the acclivity. 
     * @param kernel_size: Size of used kernel. 
     * @param signal_delta_proportion: maximum signal proportion difference accepted. 
     * @return The index of the first bin that belongs to the acclivity to the left of start_bin. 
     * @brief Detects the first bin that belongs to an acclivity, to the left of start_bin, that is greater than
     * start_bin value * ( 1.0 + signal_delta_proportion ). Sets of consecutive bins that that are narrower
     * than kernel_size are ignored. 
     * @warning none. 
     */
    size_t FirstLeftAcclivity( const Signal &sgn, size_t start_bin, size_t kernel_size, float signal_delta_proportion );

    /**
     * @date 2012/Sep/14 
     * @param sgn: Input signal.
     * @param start_bin: A bin tha lies before the acclivity. 
     * @param end_bin: Limit for the search. 
     * @param kernel_size: Size of used kernel. 
     * @param signal_delta_proportion: maximum signal proportion difference accepted. 
     * @return The index of the first bin that belongs to the acclivity to the left of start_bin and limited to
     * end_bin. 
     * @brief Detects the first bin that belongs to an acclivity, to the left of start_bin and limited to end_bin,
     * that is greater than start_bin value * ( 1.0 + signal_delta_proportion ). Sets of consecutive bins
     * that that are narrower than kernel_size are ignored. 
     * @warning none. 
     */
    size_t FirstLeftAcclivity( const Signal &sgn, size_t start_bin, size_t end_bin, size_t kernel_size,
                               float signal_delta_proportion );

    /**
     * @date 2012/Sep/14 
     * @param sgn: Input signal.
     * @param start_bin: A bin tha lies before the declivity. 
     * @param kernel_size: Size of used kernel. 
     * @param signal_delta_proportion: maximum signal proportion difference accepted. 
     * @return The index of the first bin that belongs to the declivity to the right of start_bin. 
     * @brief Detects the first bin that belongs to an declivity, to the right of start_bin, that is lower than
     * start_bin value * ( 1.0 - signal_delta_proportion ). Sets of consecutive bins that that are narrower
     * than kernel_size are ignored. 
     * @warning none. 
     */
    size_t FirstRightDeclivity( const Signal &sgn, size_t start_bin, size_t kernel_size, 
                                float signal_delta_proportion );

    /**
     * @date 2012/Sep/14 
     * @param sgn: Input signal.
     * @param start_bin: A bin tha lies before the declivity. 
     * @param end_bin: Limit for the search. 
     * @param kernel_size: Size of used kernel. 
     * @param signal_delta_proportion: maximum signal proportion difference accepted. 
     * @return The index of the first bin that belongs to the declivity to the right of start_bin and limited to
     * end_bin. 
     * @brief Detects the first bin that belongs to an declivity, to the right of start_bin and limited to
     * end_bin, that is lower than start_bin value * ( 1.0 - signal_delta_proportion ). Sets of consecutive
     * bins that that are narrower than kernel_size are ignored. 
     * @warning none. 
     */
    size_t FirstRightDeclivity( const Signal &sgn, size_t start_bin, size_t end_bin, size_t kernel_size,
                                float signal_delta_proportion );

    /**
     * @date 2012/Sep/14 
     * @param sgn: Input signal.
     * @param start_bin: A bin tha lies before the declivity. 
     * @param kernel_size: Size of used kernel. 
     * @param signal_delta_proportion: maximum signal proportion difference accepted. 
     * @return The index of the first bin that belongs to the declivity to the left of start_bin. 
     * @brief Detects the first bin that belongs to an declivity, to the left of start_bin, that is lower than
     * start_bin value * ( 1.0 - signal_delta_proportion ). Sets of consecutive bins that that are narrower
     * than kernel_size are ignored. 
     * @warning none. 
     */
    size_t FirstLeftDeclivity( const Signal &sgn, size_t start_bin, size_t kernel_size, float signal_delta_proportion );

    /**
     * @date 2012/Sep/14 
     * @param sgn: Input signal.
     * @param start_bin: A bin tha lies before the declivity. 
     * @param end_bin: Limit for the search. 
     * @param kernel_size: Size of used kernel. 
     * @param signal_delta_proportion: maximum signal proportion difference accepted. 
     * @return The index of the first bin that belongs to the declivity to the left of start_bin and limited to
     * end_bin. 
     * @brief Detects the first bin that belongs to an declivity, to the left of start_bin and limited to end_bin,
     * that is lower than start_bin value * ( 1.0 - signal_delta_proportion ). Sets of consecutive bins
     * that that are narrower than kernel_size are ignored. 
     * @warning none. 
     */
    size_t FirstLeftDeclivity( const Signal &sgn, size_t start_bin, size_t end_bin, size_t kernel_size,
                               float signal_delta_proportion );

    /**
     * @date 2012/Sep/11 
     * @param sgn: Input signal.
     * @param signal: Input signal. 
     * @param start_bin: A bin that belongs to the plateau. 
     * @param kernel_size: Size of used kernel. 
     * @param signal_delta_proportion: maximum signal proportion difference accepted for a bin to belong to the
     *             plateau. 
     * @return The index of the last bin to the right of the start_bin
     * that belongs to the plateau. 
     * @brief Detects the last occurrence, to the right of start_bin, of a bin that is not greater than start_bin
     * value * ( 1.0 + signal_delta_proportion ) and that is not lower than start_bin value * ( 1.0 -
     * signal_delta_proportion ). Sets of consecutive bins that are out of plateau signal range but are
     * narrower than kernel_size are ignored. 
     * @warning none. 
     */
    size_t PlateauRightLimit( const Signal &sgn, size_t start_bin, size_t kernel_size, float signal_delta_proportion );

    /**
     * @date 2012/Sep/11 
     * @param sgn: Input signal.
     * @param signal: Input signal. 
     * @param start_bin: A bin that belongs to the plateau. 
     * @param kernel_size: Size of used kernel. 
     * @param signal_delta_proportion: maximum signal proportion difference accepted for a bin to belong to the
     *             plateau. 
     * @param end_bin: Limit for the search. 
     * @return The index of the last bin to the right of the start_bin
     * up to end_bin that belongs to the plateau. 
     * @brief Detects the last occurrence, to the right of start_bin up to end_bin, of a bin that is not greater
     * than start_bin value * ( 1.0 + signal_delta_proportion ) and that is not lower than start_bin value
     * ( 1.0 - signal_delta_proportion ). <br> Sets of consecutive bins that are out of plateau signal range
     * but are narrower than kernel_size are ignored. 
     * @warning none. 
     */
    size_t PlateauRightLimit( const Signal &sgn, size_t start_bin, size_t end_bin, size_t kernel_size,
                              float signal_delta_proportion );

    /**
     * @date 2012/Sep/11 
     * @param sgn: Input signal.
     * @param signal: Input signal. 
     * @param start_bin: A bin that belongs to the plateau. 
     * @param kernel_size: Size of used kernel. 
     * @param signal_delta_proportion: maximum signal proportion difference accepted for a bin to belong to the
     *             plateau. 
     * @return The index of the last bin to the left of the start_bin that belongs to the plateau. 
     * @brief Detects the last occurrence, to the left of start_bin, of a bin that is not greater than start_bin
     * value * ( 1.0 + signal_delta_proportion ) and that is not lower than start_bin value * ( 1.0 -
     * signal_delta_proportion ). <br> Sets of consecutive bins that are out of plateau signal range but are
     * narrower than kernel_size are ignored. 
     * @warning none. 
     */
    size_t PlateauLeftLimit( const Signal &sgn, size_t start_bin, size_t kernel_size, float signal_delta_proportion );

    /**
     * @date 2012/Sep/11 
     * @param sgn: Input signal.
     * @param signal: Input signal. 
     * @param start_bin: A bin that belongs to the plateau. 
     * @param kernel_size: Size of used kernel. 
     * @param signal_delta_proportion: maximum signal proportion difference accepted for a bin to belong to the
     *             plateau. 
     * @param end_bin: Limit for the search. 
     * @return The index of the last bin to the left of the start_bin up to end_bin that belongs to the plateau. 
     * @brief Detects the last occurrence, to the left of start_bin up to end_bin, of a bin that is not greater
     * than start_bin value * ( 1.0 + signal_delta_proportion ) and that is not lower than start_bin value
     * ( 1.0 - signal_delta_proportion ). <br> Sets of consecutive bins that are out of plateau signal range
     * but are narrower than kernel_size are ignored. 
     * @warning none. 
     */
    size_t PlateauLeftLimit( const Signal &sgn, size_t start_bin, size_t end_bin, size_t kernel_size,
                             float signal_delta_proportion );

  }

}

#include "SignalKeyPoints.cpp"

#endif
