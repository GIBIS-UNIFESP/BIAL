/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Aug/27 
 * @brief Functions to strip the skull from the brain image.
 */

#ifndef BIALBRAINSKULLSTRIPPING_C
#define BIALBRAINSKULLSTRIPPING_C

#include "BrainSkullStripping.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_BrainSkullStripping )
#define BIAL_EXPLICIT_BrainSkullStripping
#endif

#if defined ( BIAL_EXPLICIT_BrainSkullStripping ) || ( BIAL_IMPLICIT_BIN )

#include "DrawBox.hpp"
#include "Histogram.hpp"
#include "Image.hpp"
#include "ImageResize.hpp"
#include "Integral.hpp"
#include "NiftiHeader.hpp"
#include "SegmentationGeoStar.hpp"
#include "Signal.hpp"

namespace Bial {

  namespace Brain {

    template< class D >
    Vector< size_t > Location( const Image< D > &img, const std::string &orientation, Vector< Box > &best_rectangles,
                               Image< float > &similarity ) {
      if( img.Dims( ) != 3 ) {
        std::string msg( BIAL_ERROR( "Invalid number of dimensions. Expected 3." ) );
        throw( std::logic_error( msg ) );
      }
      /*
       * Boxes: brain:      Sagittal: 53-127;  Axial: 131-187; Coronal: 89-176
       *        cerebellum: Sagittal: 57-126;  Axial: 95-121;  Coronal: 141-176
       *        neck:       Sagittal: 56-130;  Axial: 44-76;   Coronal: 69-169
       *        scalf top   Sagittal: 58-123;  Axial: 220-227; Coronal: 94-166
       *        scalf left  Sagittal: 13-19;   Axial: 114-174; Coronal: 100-166
       *        scalf right Sagittal: 160-165; Axial: 118-177; Coronal: 98-158
       *        scalf anter Sagittal: 62-118;  Axial: 150-179; Coronal: 31-38
       *        scalf poste Sagittal: 59-115;  Axial: 118-177; Coronal: 108-164
       */
      try {
        COMMENT( "Setting initial variables.", 0 );
        float window_length = 100.0; /* In millimeter */
        D max_intensity = img.Maximum( );
        COMMENT( "Initial central coordinate estimation = center of the image.", 0 );
        Vector< size_t > initial_COG( img.Dim( ) );
        for( size_t dms = 0; dms < img.Dims( ); ++dms ) {
          initial_COG( dms ) /= 2;
        }
        COMMENT( "Getting heart orientation.", 0 );
        size_t axial_dim = NiftiHeader::AxialDimension( orientation );
        size_t coronal_dim = NiftiHeader::CoronalDimension( orientation );
        size_t sagittal_dim = NiftiHeader::SagittalDimension( orientation );
        bool superior_inferior = NiftiHeader::SuperiorInferior( orientation );
        bool anterior_posterior = NiftiHeader::AnteriorPosterior( orientation );
        COMMENT( "Computing integral image.", 0 );
        Image< double > integral = Integral::IntegralImage( img );
        COMMENT( "Initializing optimization variables.", 0 );
        Vector< size_t > best_COG( initial_COG );
        float max_similarity = std::numeric_limits< float >::min( );
        COMMENT( "Initializing output data.", 0 );
        /* best_rectangles = Vector< Box >( 5, Box( Vector< float >( img.Dims( ), 0.0 ), */
        best_rectangles = Vector< Box >( 4, Box( Vector< float >( img.Dims( ), 0.0 ), 
                                                 Vector< float >( img.Dims( ), 1.0 ),
                                                 Color( 1, 255, 255, 255 ), true ) );
        similarity.Set( 0.0 );
        COMMENT( "Initializing box dimensions and positions.", 0 );
        COMMENT( "Central box size. => Contains the heart.", 1 );
        Vector< size_t > central_box_end( initial_COG );
        Vector< size_t > central_box_size( img.Dims( ) );
        COMMENT( "Left box size. => Contains left lug.", 1 );
        Vector< size_t > left_box_end( central_box_end );
        Vector< size_t > left_box_size( central_box_size );
        COMMENT( "Right box size. => Contains right lugs.", 1 );
        Vector< size_t > right_box_end( left_box_end );
        Vector< size_t > right_box_size( left_box_size );
        COMMENT( "Inferior box size. => Abdominal organs.", 1 );
        Vector< size_t > inferior_box_end( right_box_end );
        Vector< size_t > inferior_box_size( right_box_size );
        COMMENT( "Computing search window dimensions in pixels and initializing its position.", 0 );
        Vector< size_t > search_window_size( img.Dims( ) );
        for( size_t dms = 0; dms < img.Dims( ); ++dms ) {
          search_window_size( dms ) = static_cast< size_t >( img.PixelSize( dms ) * window_length );
        }
        Vector< size_t > search_window_start( img.Dims( ), 0 );
        size_t dms;

        COMMENT( "COG position changing loop.", 2 );
        do {
          COMMENT( "Setting search_window_size shift from initial COG position.", 3 );
          Vector< size_t > COG( initial_COG );
          for( dms = 0; dms < integral.Dims( ); ++dms ) {
            int COG_coordinate = static_cast< int >( initial_COG( dms ) + search_window_start( dms ) ) -
              static_cast< int >( search_window_size( dms ) / 2 );
            if( ( COG_coordinate < 0 ) || ( COG_coordinate >= static_cast< int >( integral.size( dms ) ) ) ) {
              break;
            }
            COG( dms ) = static_cast< size_t >( COG_coordinate );
          }
          COMMENT( "box size changing loop. 110mm to 145mm according to reference.", 4 );
          COMMENT( "That is 127.5mm +- 17.5mm, or 127.5 */ 1.137254902.", 4 );
          for( double size_factor_power = -1.0; size_factor_power <= 1.0; size_factor_power += 0.2 ) {
            double size_factor = std::pow( 1.137254902, size_factor_power );
            COMMENT( "Computing similarity for current reference pixel.", 4 );
            if( dms == integral.Dims( ) ) { /* Valid position. */
              COMMENT( "Computing rectangle size.", 4 );
              COMMENT( "Central box size. => Contains the heart.", 4 );
              central_box_size = Vector< size_t >( img.Dims( ) ); /* central */
              central_box_size( axial_dim ) = static_cast< size_t >( size_factor * 127.5 /
                                                                     img.PixelSize( axial_dim ) + 0.5 );
              central_box_size( coronal_dim ) = static_cast< size_t >( size_factor * 127.5 /
                                                                       img.PixelSize( coronal_dim ) + 0.5 );
              central_box_size( sagittal_dim ) = static_cast< size_t >( size_factor * 127.5 /
                                                                        img.PixelSize( sagittal_dim ) + 0.5 );
              left_box_size = Vector< size_t >( central_box_size ); /* left */
              left_box_size( coronal_dim ) = static_cast< size_t >( size_factor * 191.25 /
                                                                    img.PixelSize( sagittal_dim ) + 0.5 );
              left_box_size( sagittal_dim ) = static_cast< size_t >( size_factor * 32.0 /
                                                                     img.PixelSize( sagittal_dim ) + 0.5 );
              right_box_size = Vector< size_t >( left_box_size ); /* right */
              inferior_box_size = Vector< size_t >( right_box_size ); /* inferior */
              inferior_box_size( axial_dim ) = static_cast< size_t >( size_factor * 47.22 /
                                                                      img.PixelSize( axial_dim ) + 0.5 );
              inferior_box_size( coronal_dim ) = static_cast< size_t >( size_factor * 127.5 /
                                                                        img.PixelSize( coronal_dim ) + 0.5 );
              inferior_box_size( sagittal_dim ) = static_cast< size_t >( size_factor * 255.0 /
                                                                         img.PixelSize( sagittal_dim ) + 0.5 );
              COMMENT( "Computing rectangle positions.", 4 );
              central_box_end = COG; /* Central */
              central_box_end( axial_dim ) += static_cast< size_t >( central_box_size( axial_dim ) / 2.0 );
              central_box_end( coronal_dim ) += static_cast< size_t >( central_box_size( coronal_dim ) / 2.0 );
              central_box_end( sagittal_dim ) += static_cast< size_t >( central_box_size( sagittal_dim ) / 2.0 );
              left_box_end = COG; /* Left */
              left_box_end( axial_dim ) += static_cast< size_t >( left_box_size( axial_dim ) / 2.0 );
              left_box_end( coronal_dim ) += static_cast< size_t >( central_box_size( coronal_dim ) / 2.0 );
              if( anterior_posterior ) {
                left_box_end( coronal_dim ) += static_cast< size_t >( left_box_size( coronal_dim ) / 3.0 );
              }
              left_box_end( sagittal_dim ) -= static_cast< size_t >( central_box_size( sagittal_dim ) / 2.0 );
              right_box_end = COG; /* Right */
              right_box_end( axial_dim ) += static_cast< size_t >( right_box_size( axial_dim ) / 2.0 );
              right_box_end( coronal_dim ) += static_cast< size_t >( central_box_size( coronal_dim ) / 2.0 );
              if( anterior_posterior ) {
                right_box_end( coronal_dim ) += static_cast< size_t >( right_box_size( coronal_dim ) / 3.0 );
              }
              right_box_end( sagittal_dim ) += static_cast< size_t >( central_box_size( sagittal_dim ) / 2.0
                                                                      + right_box_size( sagittal_dim ) );
              inferior_box_end = COG; /* Inferior */
              if( superior_inferior ) {
                inferior_box_end( axial_dim ) += static_cast< size_t >( 0.5 * central_box_size( axial_dim ) +
                                                                        1.5 * inferior_box_size( axial_dim ) );
              }
              else {
                inferior_box_end( axial_dim ) -= static_cast< size_t >( 0.5 * central_box_size( axial_dim ) +
                                                                        0.5 * inferior_box_size( axial_dim ) );
              }
              inferior_box_end( coronal_dim ) += static_cast< size_t >( inferior_box_size( coronal_dim ) / 2.0 );
              inferior_box_end( sagittal_dim ) += static_cast< size_t >( inferior_box_size( sagittal_dim ) / 2.0 );
              COMMENT( "Computing box density.", 4 );
              double den_center = -6.0 * max_intensity;
              if( integral.ValidCoordinate( central_box_end ) ) {
                den_center = Integral::WindowDensity( integral, central_box_end, central_box_size );
              }
              double den_left = +6.0 * max_intensity;
              if( integral.ValidCoordinate( left_box_end ) ) {
                den_left = Integral::WindowDensity( integral, left_box_end, left_box_size );
              }
              double den_right = +6.0 * max_intensity;
              if( integral.ValidCoordinate( right_box_end ) ) {
                den_right = Integral::WindowDensity( integral, right_box_end, right_box_size );
              }
              double den_inferior = -6.0 * max_intensity;
              if( integral.ValidCoordinate( inferior_box_end ) ) {
                den_inferior = Integral::WindowDensity( integral, inferior_box_end, inferior_box_size );
              }
              COMMENT( "Computing total similarity.", 4 );
              double total_similarity = den_center + ( max_intensity - den_right ) + ( max_intensity - den_left )
                + den_inferior; /* + den_anterior; */
              COMMENT( "den_center: " << den_center << ", den_right: " << den_right << ", den_left: " << den_left <<
                       "den_inferior: " << den_inferior << "max_intensity: " << max_intensity << ", total_similarity: "
                       << total_similarity, 4 );
              if( total_similarity < 0.0 ) {
                similarity( COG ) = 0.0;
              }
              else {
                similarity( COG ) = total_similarity;
              }
              COMMENT( "Updating similarity maximum function value.", 4 );
              if( total_similarity > max_similarity ) {
                max_similarity = total_similarity;
                best_COG = COG;
                COMMENT( "Updating rectangle image.", 4 );
                Vector< float > central_box_begin( ( Vector< float >( central_box_end ) ) );
                Vector< float > left_box_begin( ( Vector< float >( left_box_end ) ) );
                Vector< float > right_box_begin( ( Vector< float >( right_box_end ) ) );
                Vector< float > inferior_box_begin( ( Vector< float >( inferior_box_end ) ) );
                /* Vector< float > anterior_box_begin( ( Vector< float >( anterior_box_end ) ) ); */
                for( size_t dms = 0; dms < integral.Dims( ); ++dms ) {
                  central_box_begin( dms ) -= central_box_size( dms );
                  left_box_begin( dms ) -= left_box_size( dms );
                  right_box_begin( dms ) -= right_box_size( dms );
                  inferior_box_begin( dms ) -= inferior_box_size( dms );
                  /* anterior_box_begin( dms ) -= anterior_box_size( dms ); */
                }
                COMMENT( "Drawing boxes.", 4 );
                best_rectangles( 0 ) = Box( central_box_begin, Vector< float >( central_box_size ),
                                            Color( 1, 255, 0, 0 ), true );
                best_rectangles( 1 ) = Box( left_box_begin, Vector< float >( left_box_size ),
                                            Color( 1, 0, 255, 0 ), true );
                best_rectangles( 2 ) = Box( right_box_begin, Vector< float >( right_box_size ),
                                            Color( 1, 0, 0, 255 ), true );
                best_rectangles( 3 ) = Box( inferior_box_begin, Vector< float >( inferior_box_size ),
                                            Color( 1, 255, 0, 255 ), true );
              }
            } /* end-if valid dimensions. */
            COMMENT( "Updating reference pixel coordinates.", 4 );
            for( dms = 0; ( dms < integral.Dims( ) ) &&
                   ( search_window_start( dms ) == search_window_size( dms ) - 1 ); ++dms ) {
              search_window_start( dms ) = 0;
            }
            if( dms < integral.Dims( ) ) {
              ++search_window_start( dms );
            }
          } /* end-for size_factor */
        } while( dms < integral.Dims( ) );
        return( best_COG );
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

    template< class D >
    Image< D > SkullStripping( const Image< D > &img, const std::string &orientation ) {
      try {
        COMMENT( "Computing COG and boxes.", 2 );
        Vector< Box > best_rectangles;
        Image< float > similarity( img );
        Vector< size_t > cog = Location( img, orientation, best_rectangles, similarity );
        COMMENT( "Creating subimages containing each box region.", 2 );
        COMMENT( "Central box subimage.", 2 );
        Vector< size_t > central_low_coord = Vector< size_t >( best_rectangles( 0 ).low_coord );
        Vector< size_t > central_high_coord = Vector< size_t >( central_low_coord + best_rectangles( 0 ).size - 1 );
        Image< D > central_img( ImageOp::Resize( img, central_low_coord, central_high_coord ) ); /* Size of box. */
        COMMENT( "Left box subimage.", 2 );
        Vector< size_t > left_low_coord = Vector< size_t >( best_rectangles( 1 ).low_coord );
        Vector< size_t > left_high_coord = Vector< size_t >( left_low_coord + best_rectangles( 1 ).size - 1 );
        Image< D > left_img( ImageOp::Resize( img, left_low_coord, left_high_coord ) ); /* Size of box. */
        COMMENT( "Right box subimage.", 2 );
        Vector< size_t > right_low_coord = Vector< size_t >( best_rectangles( 2 ).low_coord );
        Vector< size_t > right_high_coord = Vector< size_t >( right_low_coord + best_rectangles( 2 ).size - 1 );
        Image< D > right_img( ImageOp::Resize( img, right_low_coord, right_high_coord ) ); /* Size of box. */
        COMMENT( "Inferior box subimage.", 2 );
        Vector< size_t > inferior_low_coord = Vector< size_t >( best_rectangles( 3 ).low_coord );
        Vector< size_t > inferior_high_coord = Vector< size_t >( inferior_low_coord + best_rectangles( 3 ).size - 1 );
        Image< D > inferior_img( ImageOp::Resize( img, inferior_low_coord, inferior_high_coord ) ); /* Size of box. */
        COMMENT( "Computing histogram of each subimages.", 2 );
        COMMENT( "Central histogram.", 2 );
        Signal central_hist( SignalType::ZeroStartHistogram( central_img ) );
        COMMENT( "Left histogram.", 2 );
        Signal left_hist( SignalType::ZeroStartHistogram( left_img ) );
        COMMENT( "Right histogram.", 2 );
        Signal right_hist( SignalType::ZeroStartHistogram( right_img ) );
        COMMENT( "Finding peak of each histogram.", 2 );
        COMMENT( "Central peak.", 2 );
        size_t central_bin = central_hist.MaximumFrequencyBin( );
        COMMENT( "Left peak.", 2 );
        size_t left_bin = left_hist.MaximumFrequencyBin( );
        COMMENT( "Right peak.", 2 );
        size_t right_bin = right_hist.MaximumFrequencyBin( );

        COMMENT( "Computing external seeds.", 2 );
        Vector< size_t > ext_seeds;
        COMMENT( "Inserting all pixels in inferior box.", 2 );
        for( size_t z = inferior_low_coord( 2 ); z <= inferior_high_coord( 2 ); ++z ) {
          for( size_t y = inferior_low_coord( 1 ); y <= inferior_high_coord( 1 ); ++y ) {
            for( size_t x = inferior_low_coord( 0 ); x <= inferior_high_coord( 0 ); ++x ) {
              size_t pxl = x + y*img.Displacement( 0 ) + z*img.Displacement( 1 );
              ext_seeds.push_back( pxl );
            }
          }
        }
        COMMENT( "Inserting dark pixels in left box ( peak -10 to peak + 10 )", 2 );
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
        COMMENT( "Inserting dark pixels in right box ( peak -10 to peak + 10 ).", 2 );
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
        COMMENT( "Computing internal seeds.", 2 );
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
        COMMENT( "Segmenting the brain.", 2 );
        double alpha = 0.75; /* alpha: 0.0 0.25 0.5 0.75 e 1.0 */
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

#ifdef BIAL_EXPLICIT_BrainSkullStripping


    template Vector< size_t > Location( const Image< int > &img, const std::string &orientation,
                                        Vector< Box > &best_rectangles, Image< float > &similarity );
    template Image< int > SkullStripping( const Image< int > &img, const std::string &orientation );
    
    template Vector< size_t > Location( const Image< llint > &img, const std::string &orientation,
                                        Vector< Box > &best_rectangles, Image< float > &similarity );
    template Image< llint > SkullStripping( const Image< llint > &img, const std::string &orientation );

    template Vector< size_t > Location( const Image< float > &img, const std::string &orientation,
                                        Vector< Box > &best_rectangles, Image< float > &similarity );
    template Image< float > SkullStripping( const Image< float > &img, const std::string &orientation );
  
    template Vector< size_t > Location( const Image< double > &img, const std::string &orientation,
                                        Vector< Box > &best_rectangles, Image< float > &similarity );
    template Image< double > SkullStripping( const Image< double > &img, const std::string &orientation );

#endif

  }

}

#endif

#endif
