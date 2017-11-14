/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Aug/27 
 * @brief Class for computing brain histogram based intensities, such as peaks, valleys, mean, minimum, and maximum.
 */

#ifndef BIALBRAININTENSITIES_C
#define BIALBRAININTENSITIES_C

#include "BrainIntensities.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_BrainIntensities )
#define BIAL_EXPLICIT_BrainIntensities
#endif

#if defined ( BIAL_EXPLICIT_BrainIntensities ) || ( BIAL_IMPLICIT_BIN )

#include "Histogram.hpp"
#include "Image.hpp"
#include "Signal.hpp"
#include "SignalAccumulate.hpp"
#include "SignalEmptyBins.hpp"
#include "SignalKeyPoints.hpp"
#include "SignalMedianFilter.hpp"
#include "SignalOtsuThreshold.hpp"

namespace Bial {

  namespace Brain {

    template< class D > 
    BrainIntensities::BrainIntensities( const Image< D > &img ) {
      try {
        COMMENT( "Computing median histogram.", 0 );
        Signal hist = SignalType::Histogram( img );
        Signal median_hist = SignalOp::RemoveEmptyBins( hist );
        Vector< size_t > hist_index = SignalOp::MapBins( median_hist, hist );
        median_hist[ 0 ] = 0; /* Remove background. */
        SignalOp::MedianFilter( median_hist, 5 + ( median_hist.Bins( ) / 1000 ) );

        COMMENT( "Computing intensity amplitude.", 0 );
        int amplitude = median_hist.MaximumFrequency( );
        size_t histo_size = median_hist.Bins( );
        size_t kernel_size = 4 + histo_size / 100;
        COMMENT( "Computing intensity extremities. amplitude: " << amplitude << ", histo size: " << histo_size <<
                 ", kernel size: " << kernel_size, 0 );
        size_t it = SignalOp::FirstRightValley( median_hist, 1, kernel_size - 1 );
        COMMENT( "Got first valley.", 0 );
        while( median_hist[ it ] <= 0.05 * amplitude ) 
          ++it;
        COMMENT( "Walked amplitude.", 0 );
        size_t minimum_bin = it;
        it = median_hist.Bins( ) - 2;
        COMMENT( "From max bin.", 0 );
        while( median_hist[ it ] <= 0.3 * amplitude )
          --it;
        COMMENT( "Walked amplitude.", 0 );
        size_t maximum_bin = it;

        COMMENT( "Finding otsu threshold coordinates.", 0 );
        size_t otsu_threshold;
        otsu_threshold = SignalOp::OtsuThreshold( hist );
        it = 0;
        while( hist_index( it ) < otsu_threshold )
          ++it;
        otsu_threshold = it;
        COMMENT( "Computing histogram mean for voxels that are greater than Otsu threshold.", 0 );
        double sum = 0.0;
        size_t mean = 0;
        for( it = otsu_threshold; it < histo_size; it++ ) {
          sum += static_cast< double >( median_hist[ it ] * hist_index( it ) );
        }
        sum /= SignalOp::AccumulatedFrequency( median_hist, otsu_threshold );
        for( it = 0; hist_index( it ) < static_cast< size_t >( sum ); it++ ) {
        }
        mean = it;

        COMMENT( "Trying to jump a small CSF peak.", 0 );
        it = otsu_threshold;
        if( median_hist[ it ] >= 0.2 * amplitude ) {
          while( median_hist[ it ] >= 0.2 * amplitude ) {
            it++;
          }
        }
        COMMENT( "Ensuring that did not jump all peaks and reaches the begining of the declivity.", 0 );
        if( it >= maximum_bin - kernel_size / 10 ) { /* It was maximum_bin - step. step = step < 4 ? 4 : step. */
          it = otsu_threshold;
        }
        else {
          while( median_hist[ it ] <= 0.2 * amplitude ) {
            it++;
          }
        }
        COMMENT( "Ensuring that did not jump GM peak. If so, set it to the maximum bin up to Otsu threshold.", 0 );
        if( it > mean ) {
          it = std::max( minimum_bin, otsu_threshold );
        }
        COMMENT( "Computing the second peak.", 0 );
        size_t second_peak_bin = SignalOp::FirstRightPeak( median_hist, it, mean, kernel_size );
        COMMENT( "Finding the third peak.", 0 );
        size_t third_peak_bin = SignalOp::FirstLeftPeak( median_hist, maximum_bin, mean, kernel_size );
        COMMENT( "Finding the second valley.", 0 );
        size_t second_valley_bin = SignalOp::Valley( median_hist, second_peak_bin, third_peak_bin );

        COMMENT( "Testing for abnormal histograms with more or less than two peaks.", 0 );
        COMMENT( "Moving to the first declivity after second peak.", 0 );
        size_t dark_declivity = SignalOp::FirstRightDeclivity( median_hist, second_peak_bin, kernel_size, 0.05 );
        COMMENT( "Moving to the first declivity after dark peak.", 0 );
        size_t light_declivity = SignalOp::FirstLeftDeclivity( median_hist, third_peak_bin, kernel_size, 0.05 );
        if( dark_declivity < light_declivity ) {
          COMMENT( "If dark and light peaks are not part of a plateau, there may be valleys and peaks between them.",
                   0 );
          COMMENT( "Moving to the first valley after dark_declivity.", 0 );
          size_t dark_valley = SignalOp::FirstRightValley( median_hist, dark_declivity, light_declivity, kernel_size );
          COMMENT( "Moving to the first valley before light_declivity.", 0 );
          size_t light_valley = SignalOp::FirstLeftValley( median_hist, light_declivity, dark_declivity, kernel_size );
          size_t dark_acclivity = SignalOp::FirstRightAcclivity( median_hist, dark_valley, maximum_bin, kernel_size, 
                                                                 0.05 );
          COMMENT( "Moving to first acclivity after dark_valley.", 0 );
          size_t light_acclivity = SignalOp::FirstLeftAcclivity( median_hist, light_valley, minimum_bin, kernel_size,
                                                                 0.05 );
          COMMENT( "Moving to first acclivity before light_valley.", 0 );
          if( dark_acclivity < light_acclivity ) {
            COMMENT( "There is at least another peak. Set the peaks at the maximum before and after mean intensity.", 
                     0 );
            second_valley_bin = mean;
            second_peak_bin = SignalOp::Peak( median_hist, minimum_bin, second_valley_bin );
            third_peak_bin = SignalOp::Peak( median_hist, second_valley_bin, maximum_bin );
          }
        }
        else if( median_hist[ second_peak_bin ] > median_hist[ third_peak_bin ] * 1.06 ) {
          COMMENT( "There are a dark peak and a large light plateau including dark/light transition.", 0 );
          COMMENT( "Refining dark peak.", 0 );
          size_t plateau_begin = SignalOp::PlateauLeftLimit( median_hist, second_peak_bin, kernel_size, 0.05 );
          size_t plateau_end = SignalOp::PlateauRightLimit( median_hist, second_peak_bin, kernel_size, 0.05 );
          second_peak_bin = ( plateau_begin + plateau_end ) / 2;
          COMMENT( "Refining light peak.", 0 );
          plateau_begin = SignalOp::PlateauLeftLimit( median_hist, third_peak_bin, kernel_size, 0.05 );
          plateau_end = SignalOp::PlateauRightLimit( median_hist, third_peak_bin, kernel_size, 0.05 );
          second_valley_bin = plateau_begin + ( plateau_end - plateau_begin ) / 2;
          third_peak_bin = plateau_end - ( plateau_end - plateau_begin ) / 4;
        }
        else if( median_hist[ third_peak_bin ] > median_hist[ second_peak_bin ] * 1.06 ) {
          COMMENT( "There are a light peak and a large dark plateau including dark/light transition.", 0 );
          COMMENT( "Refining light peak.", 0 );
          size_t plateau_begin = SignalOp::PlateauLeftLimit( median_hist, third_peak_bin, kernel_size, 0.05 );
          size_t plateau_end = SignalOp::PlateauRightLimit( median_hist, third_peak_bin, kernel_size, 0.05 );
          third_peak_bin = ( plateau_begin + plateau_end ) / 2;
          COMMENT( "Refining dark peak.", 0 );
          plateau_begin = SignalOp::PlateauLeftLimit( median_hist, second_peak_bin, kernel_size, 0.05 );
          plateau_end = SignalOp::PlateauRightLimit( median_hist, second_peak_bin, kernel_size, 0.05 );
          second_peak_bin = plateau_begin + ( plateau_end - plateau_begin ) / 4;
          second_valley_bin = plateau_end - ( plateau_end - plateau_begin ) / 2;
        }
        else {
          COMMENT( "Light and dark peaks compose an unique plateau. Set gm peak to a reasonable position.", 0 );
          size_t plateau_begin = SignalOp::PlateauLeftLimit( median_hist, second_peak_bin, kernel_size, 0.15 );
          COMMENT( "Locating the begining of the plateau.", 0 );
          size_t plateau_end = SignalOp::PlateauRightLimit( median_hist, third_peak_bin, kernel_size, 0.15 );
          COMMENT( "Locating the end of the plateau.", 0 );
          second_peak_bin = plateau_begin + ( plateau_end - plateau_begin ) / 4;
          third_peak_bin = plateau_end - ( plateau_end - plateau_begin ) / 4;
          second_valley_bin = ( plateau_begin + plateau_end ) / 2;
        }

        COMMENT( "Finding the first peak and the first valley.", 0 );
        size_t first_valley_bin = ( minimum_bin + second_peak_bin ) / 2;
        size_t first_peak_bin = ( minimum_bin + first_valley_bin ) / 2;

        COMMENT( "Getting the frequency of all landmarks.", 0 );
        minimum = hist.Data( hist_index( minimum_bin ) );
        maximum = hist.Data( hist_index( maximum_bin ) );
        first_peak = hist.Data( hist_index( first_peak_bin ) );
        first_valley = hist.Data( hist_index( first_valley_bin ) );
        second_peak = hist.Data( hist_index( second_peak_bin ) );
        second_valley = hist.Data( hist_index( second_valley_bin ) );
        third_peak = hist.Data( hist_index( third_peak_bin ) );
      }
      catch( std::bad_alloc &e ) {
        std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
        throw( std::runtime_error( msg ) );
      }
      catch( std::runtime_error &e ) {
        std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
        throw( std::runtime_error( msg ) );
      }
      catch( const std::out_of_range &e ) {
        std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
        throw( std::out_of_range( msg ) );
      }
      catch( const std::logic_error &e ) {
        std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
        throw( std::logic_error( msg ) );
      }
    }

#ifdef BIAL_EXPLICIT_BrainIntensities

    template BrainIntensities::BrainIntensities( const Image< int > &img );
    template BrainIntensities::BrainIntensities( const Image< llint > &img );
    template BrainIntensities::BrainIntensities( const Image< float > &img );
    template BrainIntensities::BrainIntensities( const Image< double > &img );

#endif

  }

}

#endif

#endif
