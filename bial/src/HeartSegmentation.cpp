/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/12/12 
 * @brief Heart segmentation functions 
 */

#ifndef BIALHEARTSEGMENTATION_C
#define BIALHEARTSEGMENTATION_C

#include "HeartSegmentation.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_HeartSegmentation )
#define BIAL_EXPLICIT_HeartSegmentation
#endif
#if defined ( BIAL_EXPLICIT_HeartSegmentation ) || ( BIAL_IMPLICIT_BIN )

#include "DrawBox.hpp"
#include "HeartCOG.hpp"
#include "Histogram.hpp"
#include "Image.hpp"
#include "ImageResize.hpp"
#include "SegmentationGeoStar.hpp"
#include "Signal.hpp"

namespace Bial {

  template< class D >
  Image< D > Heart::Segmentation( const Image< D > &img, const std::string &orientation ) {
    try {
      COMMENT( "Computing COG and boxes.", 1 );
      Vector< Box > best_rectangles;
      Image< float > similarity( img );
      Vector< size_t > cog = Heart::WeightedCOG( img, orientation, best_rectangles, similarity );
      COMMENT( "Creating subimages containing each box region.", 1 );
      COMMENT( "Central box subimage.", 1 );
      Vector< size_t > central_low_coord = Vector< size_t >( best_rectangles( 0 ).low_coord );
      Vector< size_t > central_high_coord = Vector< size_t >( central_low_coord + best_rectangles( 0 ).size - 1 );
      Image< D > central_img( ImageOp::Resize( img, central_low_coord, central_high_coord ) ); /* Size of box. */
      COMMENT( "Left box subimage.", 1 );
      Vector< size_t > left_low_coord = Vector< size_t >( best_rectangles( 1 ).low_coord );
      Vector< size_t > left_high_coord = Vector< size_t >( left_low_coord + best_rectangles( 1 ).size - 1 );
      Image< D > left_img( ImageOp::Resize( img, left_low_coord, left_high_coord ) ); /* Size of box. */
      COMMENT( "Right box subimage.", 1 );
      Vector< size_t > right_low_coord = Vector< size_t >( best_rectangles( 2 ).low_coord );
      Vector< size_t > right_high_coord = Vector< size_t >( right_low_coord + best_rectangles( 2 ).size - 1 );
      Image< D > right_img( ImageOp::Resize( img, right_low_coord, right_high_coord ) ); /* Size of box. */
      COMMENT( "Inferior box subimage.", 1 );
      Vector< size_t > inferior_low_coord = Vector< size_t >( best_rectangles( 3 ).low_coord );
      Vector< size_t > inferior_high_coord = Vector< size_t >( inferior_low_coord + best_rectangles( 3 ).size - 1 );
      Image< D > inferior_img( ImageOp::Resize( img, inferior_low_coord, inferior_high_coord ) ); /* Size of box. */
      COMMENT( "Computing histogram of each subimages.", 1 );
      COMMENT( "Central histogram.", 1 );
      Signal central_hist( SignalType::ZeroStartHistogram( central_img ) );
      COMMENT( "Left histogram.", 1 );
      Signal left_hist( SignalType::ZeroStartHistogram( left_img ) );
      COMMENT( "Right histogram.", 1 );
      Signal right_hist( SignalType::ZeroStartHistogram( right_img ) );
      COMMENT( "Finding peak of each histogram.", 1 );
      COMMENT( "Central peak.", 1 );
      size_t central_bin = central_hist.MaximumFrequencyBin( );
      COMMENT( "Left peak.", 1 );
      size_t left_bin = left_hist.MaximumFrequencyBin( );
      COMMENT( "Right peak.", 1 );
      size_t right_bin = right_hist.MaximumFrequencyBin( );
      COMMENT( "Computing external seeds.", 1 );
      Vector< size_t > ext_seeds;
      COMMENT( "Inserting all pixels in inferior box.", 1 );
      for( size_t z = inferior_low_coord( 2 ); z <= inferior_high_coord( 2 ); ++z ) {
        for( size_t y = inferior_low_coord( 1 ); y <= inferior_high_coord( 1 ); ++y ) {
          for( size_t x = inferior_low_coord( 0 ); x <= inferior_high_coord( 0 ); ++x ) {
            size_t pxl = x + y*img.Displacement( 0 ) + z*img.Displacement( 1 );
            ext_seeds.push_back( pxl );
          }
        }
      }
      COMMENT( "Inserting dark pixels in left box ( peak -10 to peak + 10 ).", 1 );
      D min_val = left_bin - 10.0;
      D max_val = left_bin + 10.0;
      for( size_t z = left_low_coord( 2 ); z <= left_high_coord( 2 ); ++z ) {
        for( size_t y = left_low_coord( 1 ); y <= left_high_coord( 1 ); ++y ) {
          for( size_t x = left_low_coord( 0 ); x <= left_high_coord( 0 ); ++x ) {
            size_t pxl = x + y*img.Displacement( 0 ) + z*img.Displacement( 1 );
            if( ( img[ pxl ] >= min_val ) && ( img[ pxl ] <= max_val ) ) {
              ext_seeds.push_back( pxl );
            }
          }
        }
      }
      COMMENT( "Inserting dark pixels in right box ( peak -10 to peak + 10 ).", 1 );
      min_val = right_bin - 10.0;
      max_val = right_bin + 10.0;
      for( size_t z = right_low_coord( 2 ); z <= right_high_coord( 2 ); ++z ) {
        for( size_t y = right_low_coord( 1 ); y <= right_high_coord( 1 ); ++y ) {
          for( size_t x = right_low_coord( 0 ); x <= right_high_coord( 0 ); ++x ) {
            size_t pxl = x + y*img.Displacement( 0 ) + z*img.Displacement( 1 );
            if( ( img[ pxl ] >= min_val ) && ( img[ pxl ] <= max_val ) ) {
              ext_seeds.push_back( pxl );
            }
          }
        }
      }
      COMMENT( "Computing internal seeds.", 1 );
      Vector< size_t > int_seeds;
      min_val = central_bin - 10.0;
      max_val = central_bin + 10.0;
      for( size_t z = central_low_coord( 2 ); z <= central_high_coord( 2 ); ++z ) {
        for( size_t y = central_low_coord( 1 ); y <= central_high_coord( 1 ); ++y ) {
          for( size_t x = central_low_coord( 0 ); x <= central_high_coord( 0 ); ++x ) {
            size_t pxl = x + y*img.Displacement( 0 ) + z*img.Displacement( 1 );
            if( ( img[ pxl ] >= min_val ) && ( img[ pxl ] <= max_val ) ) {
              int_seeds.push_back( pxl );
            }
          }
        }
      }
      COMMENT( "Segmenting the heart.", 1 );
      double alpha = 1.0; /* alpha: 0.0 0.25 0.5 0.75 e 1.0 */
      double beta = 1.0; /* beta: 0.0 1.0 e 2.0 */
      return( Segmentation::OrientedGeodesicStar( img, int_seeds, ext_seeds, alpha, beta ) );
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

#ifdef BIAL_EXPLICIT_HeartSegmentation

  template Image< int > Heart::Segmentation( const Image< int > &img, const std::string &orientation );
  template Image< llint > Heart::Segmentation( const Image< llint > &img, const std::string &orientation );
  template Image< float > Heart::Segmentation( const Image< float > &img, const std::string &orientation );
  template Image< double > Heart::Segmentation( const Image< double > &img, const std::string &orientation );

#endif

}

#endif

#endif
