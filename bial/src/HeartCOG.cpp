/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/12/12 
 * @brief Heart center of gravity estimation.
 */

#ifndef BIALHEARTCOG_C
#define BIALHEARTCOG_C

#include "HeartCOG.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_HeartCOG )
#define BIAL_EXPLICIT_HeartCOG
#endif
#if defined ( BIAL_EXPLICIT_HeartCOG ) || ( BIAL_IMPLICIT_BIN )

#include "DrawBox.hpp"
#include "Image.hpp"
#include "Integral.hpp"
#include "NiftiHeader.hpp"

namespace Bial {

  template< class D >
  Vector< size_t > Heart::WeightedCOG( const Image< D > &img, const std::string &orientation ) {
    size_t dimensions = img.Dims( );
    if( dimensions != 3 ) {
      std::string msg( BIAL_ERROR( "Invalid number of dimensions. Expected 3. Given: " + std::to_string( dimensions ) ) );
      throw( std::logic_error( msg ) );
    }
    try {
      COMMENT( "Setting initial variables.", 1 );
      float window_length = 100.0; /* In millimeter */
      D max_intensity = img.Maximum( );
      COMMENT( "Initial central coordinate estimation = center of the image.", 1 );
      Vector< size_t > initial_COG( img.Dim( ) );
      for( size_t dms = 0; dms < dimensions; ++dms ) {
        initial_COG( dms ) /= 2;
      }
      COMMENT( "Getting heart orientation.", 1 );
      size_t axial_dim = NiftiHeader::AxialDimension( orientation );
      size_t coronal_dim = NiftiHeader::CoronalDimension( orientation );
      size_t sagittal_dim = NiftiHeader::SagittalDimension( orientation );
      bool superior_inferior = NiftiHeader::SuperiorInferior( orientation );
      bool anterior_posterior = NiftiHeader::AnteriorPosterior( orientation );
      COMMENT( "Computing integral image.", 1 );
      Image< double > integral = Integral::IntegralImage( img );
      COMMENT( "Initializing optimization variables.", 1 );
      Vector< size_t > best_COG( initial_COG );
      float max_similarity = std::numeric_limits< float >::min( );
      COMMENT( "Initializing box dimensions and positions.", 1 );
      COMMENT( "Central box size. => Contains the heart.", 1 );
      Vector< size_t > central_box_end( initial_COG );
      Vector< size_t > central_box_size( dimensions );
      COMMENT( "Left box size. => Contains left lug.", 1 );
      Vector< size_t > left_box_end( central_box_end );
      Vector< size_t > left_box_size( central_box_size );
      COMMENT( "Right box size. => Contains right lugs.", 1 );
      Vector< size_t > right_box_end( left_box_end );
      Vector< size_t > right_box_size( left_box_size );
      COMMENT( "Inferior box size. => Abdominal organs.", 1 );
      Vector< size_t > inferior_box_end( right_box_end );
      Vector< size_t > inferior_box_size( right_box_size );
      COMMENT( "Anterior box size. => Spine.", 1 );
      /*
       * Vector< size_t > anterior_box_end( initial_COG );
       * Vector< size_t > anterior_box_size( dimensions );
       */
      COMMENT( "Computing search window dimensions in pixels and initializing its position.", 1 );
      Vector< size_t > search_window_size( dimensions );
      for( size_t dms = 0; dms < dimensions; ++dms ) {
        search_window_size( dms ) = static_cast< size_t >( img.PixelSize( dms ) * window_length );
      }
      Vector< size_t > search_window_start( dimensions, 0 );
      /*
       * if( superior_inferior )
       *        std::cout << "superior_inferior." << std::endl;
       * else
       *        std::cout << "inferior_superior." << std::endl;
       */
      for( search_window_start( 2 ) = 0; search_window_start( 2 ) < search_window_size( 2 );
           ++search_window_start( 2 ) ) {
        for( search_window_start( 1 ) = 0; search_window_start( 1 ) < search_window_size( 1 );
             ++search_window_start( 1 ) ) {
          for( search_window_start( 0 ) = 0; search_window_start( 0 ) < search_window_size( 0 );
               ++search_window_start( 0 ) ) {
            COMMENT( "Setting search_window_size shift from initial COG position.", 1 );
            Vector< size_t > COG( initial_COG );
            size_t dms;
            for( dms = 0; dms < dimensions; ++dms ) {
              int COG_coordinate = static_cast< int >( initial_COG( dms ) + search_window_start( dms ) ) -
		static_cast< int >( search_window_size( dms ) / 2 );
              if( ( COG_coordinate < 0 ) || ( COG_coordinate >= static_cast< int >( integral.size( dms ) ) ) ) {
                break;
              }
              COG( dms ) = static_cast< size_t >( COG_coordinate );
            }
            if( dms == dimensions ) { /* Valid position. */
              COMMENT( "box size changing loop. 110mm to 145mm according to reference.", 1 );
              COMMENT( "That is 127.5mm +- 17.5mm, or 127.5 */ 1.137254902.", 1 );
              for( double size_factor_power = -1.0; size_factor_power <= 1.0; size_factor_power += 0.2 ) {
                double size_factor = std::pow( 1.137254902, size_factor_power );
                COMMENT( "Computing similarity for current reference pixel.", 1 );
                COMMENT( "Computing rectangle size.", 1 );
                COMMENT( "Central box size. => Contains the heart.", 1 );
                central_box_size = Vector< size_t >( dimensions ); /* central */
                central_box_size( axial_dim ) = static_cast< size_t >( size_factor * 127.5 /
                                                                       img.PixelSize( axial_dim ) + 0.5 );
                central_box_size( coronal_dim ) = static_cast< size_t >( size_factor * 120.0 /
                                                                         img.PixelSize( coronal_dim ) + 0.5 );
                central_box_size( sagittal_dim ) = static_cast< size_t >( size_factor * 127.5 /
                                                                          img.PixelSize( sagittal_dim ) + 0.5 );
                left_box_size = Vector< size_t >( central_box_size ); /* left */
                left_box_size( coronal_dim ) = static_cast< size_t >( size_factor * 192.25 /
                                                                      img.PixelSize( sagittal_dim ) + 0.5 );
                left_box_size( sagittal_dim ) = static_cast< size_t >( size_factor * 32.0 /
                                                                       img.PixelSize( sagittal_dim ) + 0.5 );
                right_box_size = Vector< size_t >( left_box_size ); /* right */
                inferior_box_size = Vector< size_t >( right_box_size ); /* inferior */
                inferior_box_size( axial_dim ) = static_cast< size_t >( size_factor * 47.22 /
                                                                        img.PixelSize( axial_dim ) + 0.5 );
                inferior_box_size( coronal_dim ) = static_cast< size_t >( size_factor * 120.0 /
                                                                          img.PixelSize( coronal_dim ) + 0.5 );
                inferior_box_size( sagittal_dim ) = static_cast< size_t >( size_factor * 255.0 /
                                                                           img.PixelSize( sagittal_dim ) + 0.5 );
                /*
                 * anterior_box_size = Vector< size_t >( dimensions ); // anterior
                 * anterior_box_size( axial_dim ) = static_cast< size_t >( size_factor * 127.5 /
                 *                                                          img.PixelSize( axial_dim ) + 0.5 );
                 * anterior_box_size( coronal_dim ) = static_cast< size_t >( size_factor * 18.89 /
                 *                                                            img.PixelSize( coronal_dim ) + 0.5 );
                 * anterior_box_size( sagittal_dim ) = static_cast< size_t >( size_factor * 18.89 /
                 *                                                             img.PixelSize( sagittal_dim ) + 0.5 );
                 */
                COMMENT( "Computing rectangle positions.", 1 );
                central_box_end = COG; /* Central */
                central_box_end( axial_dim ) += static_cast< size_t >( central_box_size( axial_dim ) / 2.0 );
                central_box_end( coronal_dim ) += static_cast< size_t >( central_box_size( coronal_dim ) / 2.0 );
                central_box_end( sagittal_dim ) += static_cast< size_t >( central_box_size( sagittal_dim ) / 2.0 );
                left_box_end = COG; /* Left */ // ERRADO!!!!! Falta utilizar os deslocamentos aqui!!!
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
                /*
                 * anterior_box_end = COG; // Anterior
                 * anterior_box_end( axial_dim ) += static_cast< size_t >( central_box_size( axial_dim ) / 2.0 );
                 * if( anterior_posterior ) {
                 *   anterior_box_end( coronal_dim ) += static_cast< size_t >( central_box_size( coronal_dim ) / 2.0 +
                 *                                                              2.0 * anterior_box_size( coronal_dim )
                 * );
                 * }
                 * else {
                 *   anterior_box_end( coronal_dim ) -= static_cast< size_t >( central_box_size( coronal_dim ) / 2.0 +
                 *                                                              2.0 * anterior_box_size( coronal_dim )
                 * );
                 * }
                 * anterior_box_end( sagittal_dim ) += static_cast< size_t >( anterior_box_size( sagittal_dim ) / 2.0 );
                 */
                COMMENT( "Computing box density.", 1 );
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
                /*
                 * double den_anterior = -6.0 * max_intensity;
                 * if( integral.ValidCoordinate( anterior_box_end ) )
                 *   den_anterior = Integral::WindowDensity( integral, anterior_box_end, anterior_box_size );
                 */
                COMMENT( "Total similarity -> Change here to add or remove one of the densities.", 1 );
                double total_similarity = den_center + ( max_intensity - den_right ) + ( max_intensity - den_left )
		  + den_inferior; /* + den_anterior; */
                COMMENT( "Updating similarity maximum function value.", 1 );
                if( total_similarity > max_similarity ) {
                  max_similarity = total_similarity;
                  best_COG = COG;
                  COMMENT( "Updating rectangle image.", 1 );
                  Vector< float > central_box_begin( ( Vector< float >( central_box_end ) ) );
                  Vector< float > left_box_begin( ( Vector< float >( left_box_end ) ) );
                  Vector< float > right_box_begin( ( Vector< float >( right_box_end ) ) );
                  Vector< float > inferior_box_begin( ( Vector< float >( inferior_box_end ) ) );
                  /* Vector< float > anterior_box_begin( ( Vector< float >( anterior_box_end ) ) ); */
                  for( size_t dms = 0; dms < dimensions; ++dms ) {
                    central_box_begin( dms ) -= central_box_size( dms );
                    left_box_begin( dms ) -= left_box_size( dms );
                    right_box_begin( dms ) -= right_box_size( dms );
                    inferior_box_begin( dms ) -= inferior_box_size( dms );
                    /* anterior_box_begin( dms ) -= anterior_box_size( dms ); */
                  }
                }
              } /* end-for size_factor */
            } /* end-if valid dimensions. */
          } /* end-for x */
        } /* end-for y */
      } /* end-for z */
      /* } while( dms < dimensions ); */
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
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Invalid input orientation string." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< class D >
  Vector< size_t > Heart::WeightedCOG( const Image< D > &img, const std::string &orientation, 
                                       Vector< Box > &best_rectangles, Image< float > &similarity ) {
    size_t dimensions = img.Dims( );
    if( dimensions != 3 ) {
      std::string msg( BIAL_ERROR( "Invalid number of dimensions. Expected 3." ) );
      throw( std::logic_error( msg ) );
    }
    try {
      COMMENT( "Setting initial variables.", 1 );
      float window_length = 140.0; /* In millimeter */
      D max_intensity = img.Maximum( );
      COMMENT( "Initial central coordinate estimation = center of the image.", 1 );
      Vector< size_t > initial_COG( img.Dim( ) );
      for( size_t dms = 0; dms < dimensions; ++dms ) {
        initial_COG( dms ) /= 2;
      }
      COMMENT( "Getting heart orientation.", 1 );
      size_t axial_dim = NiftiHeader::AxialDimension( orientation );
      size_t coronal_dim = NiftiHeader::CoronalDimension( orientation );
      size_t sagittal_dim = NiftiHeader::SagittalDimension( orientation );
      bool superior_inferior = NiftiHeader::SuperiorInferior( orientation );
      bool anterior_posterior = NiftiHeader::AnteriorPosterior( orientation );
      COMMENT( "Computing integral image.", 1 );
      Image< double > integral = Integral::IntegralImage( img );
      COMMENT( "Initializing optimization variables.", 1 );
      Vector< size_t > best_COG( initial_COG );
      float max_similarity = std::numeric_limits< float >::min( );
      COMMENT( "Initializing output data.", 1 );
      /* best_rectangles = Vector< Box >( 5, Box( Vector< float >( dimensions, 0.0 ), */
      best_rectangles = Vector< Box >( 4, Box( Vector< float >( dimensions, 0.0 ), Vector< float >( dimensions, 1.0 ),
                                               Color( 1, 255, 255, 255 ), true ) );
      similarity.Set( 0.0 );
      COMMENT( "Initializing box dimensions and positions.", 1 );
      COMMENT( "Central box size. => Contains the heart.", 1 );
      Vector< size_t > central_box_end( initial_COG );
      Vector< size_t > central_box_size( dimensions );
      COMMENT( "Left box size. => Contains left lung.", 1 );
      Vector< size_t > left_box_end( central_box_end );
      Vector< size_t > left_box_size( central_box_size );
      COMMENT( "Right box size. => Contains right lung.", 1 );
      Vector< size_t > right_box_end( left_box_end );
      Vector< size_t > right_box_size( left_box_size );
      COMMENT( "Inferior box size. => Abdominal organs.", 1 );
      Vector< size_t > inferior_box_end( right_box_end );
      Vector< size_t > inferior_box_size( right_box_size );
      COMMENT( "Computing search window dimensions in pixels and initializing its position.", 1 );
      Vector< size_t > search_window_size( dimensions );
      for( size_t dms = 0; dms < dimensions; ++dms ) {
        search_window_size( dms ) = static_cast< size_t >( img.PixelSize( dms ) * window_length );
      }
      Vector< size_t > search_window_start( dimensions, 0 );
      size_t dms;
      /*
       * if( superior_inferior )
       *        std::cout << "superior_inferior." << std::endl;
       * else
       *        std::cout << "inferior_superior." << std::endl;
       */
      COMMENT( "COG position changing loop.", 1 );
      do {
        COMMENT( "Setting search_window_size shift from initial COG position.", 3 );
        Vector< size_t > COG( initial_COG );
        for( dms = 0; dms < dimensions; ++dms ) {
          int COG_coordinate = static_cast< int >( initial_COG( dms ) + search_window_start( dms ) ) -
	    static_cast< int >( search_window_size( dms ) / 2 );
          if( ( COG_coordinate < 0 ) || ( COG_coordinate >= static_cast< int >( integral.size( dms ) ) ) ) {
            break;
          }
          COG( dms ) = static_cast< size_t >( COG_coordinate );
        }
        COMMENT( "box size changing loop. 110mm to 145mm according to reference.", 3 );
        COMMENT( "That is 127.5mm +- 17.5mm, or 127.5 */ 1.137254902.", 3 );
        if( dms == dimensions ) { /* Valid position. */
          for( double size_factor_power = -1.0; size_factor_power <= 1.0; size_factor_power += 0.2 ) {
            double size_factor = std::pow( 1.137254902, size_factor_power );
            COMMENT( "size_factor: " << size_factor, 3 );
            COMMENT( "Computing similarity for current reference pixel.", 3 );
            COMMENT( "Computing rectangle size.", 3 );
            COMMENT( "Central box size. => Contains the heart.", 3 );
            central_box_size = Vector< size_t >( dimensions ); /* central */
            central_box_size( axial_dim ) = static_cast< size_t >( size_factor * 128.0 /
                                                                   img.PixelSize( axial_dim ) + 0.5 );
            central_box_size( coronal_dim ) = static_cast< size_t >( size_factor * 128.0 /
                                                                     img.PixelSize( coronal_dim ) + 0.5 );
            central_box_size( sagittal_dim ) = static_cast< size_t >( size_factor * 128.0 /
                                                                      img.PixelSize( sagittal_dim ) + 0.5 );
            left_box_size = Vector< size_t >( central_box_size ); /* left */
            left_box_size( coronal_dim ) = static_cast< size_t >( size_factor * 160.0 /
                                                                  img.PixelSize( sagittal_dim ) + 0.5 );
            left_box_size( sagittal_dim ) = static_cast< size_t >( size_factor * 16.0 /
                                                                   img.PixelSize( sagittal_dim ) + 0.5 );
            right_box_size = Vector< size_t >( left_box_size ); /* right */
            inferior_box_size = Vector< size_t >( right_box_size ); /* inferior */
            inferior_box_size( axial_dim ) = static_cast< size_t >( size_factor * 48.0 /
                                                                    img.PixelSize( axial_dim ) + 0.5 );
            inferior_box_size( coronal_dim ) = static_cast< size_t >( size_factor * 96.0 /
                                                                      img.PixelSize( coronal_dim ) + 0.5 );
            inferior_box_size( sagittal_dim ) = static_cast< size_t >( size_factor * 256.0 /
                                                                       img.PixelSize( sagittal_dim ) + 0.5 );
            COMMENT( "Computing rectangle positions.", 3 );
            central_box_end = COG; /* Central */
            central_box_end( axial_dim ) += static_cast< size_t >( central_box_size( axial_dim ) / 2.0 );
            central_box_end( coronal_dim ) += static_cast< size_t >( central_box_size( coronal_dim ) / 2.0 );
            central_box_end( sagittal_dim ) += static_cast< size_t >( central_box_size( sagittal_dim ) / 2.0 );
            left_box_end = COG; /* Left */
            left_box_end( axial_dim ) += static_cast< size_t >( left_box_size( axial_dim ) / 2.0 );
            if( superior_inferior ) {
              left_box_end( axial_dim ) -= static_cast< size_t >( left_box_size( axial_dim ) / 8.0 );
            }
            else {
              left_box_end( axial_dim ) += static_cast< size_t >( left_box_size( axial_dim ) / 8.0 );
            }
            left_box_end( coronal_dim ) += static_cast< size_t >( left_box_size( coronal_dim ) / 2.0 );
            if( anterior_posterior ) {
              left_box_end( coronal_dim ) += static_cast< size_t >( left_box_size( coronal_dim ) / 4.0 );
            }
            else {
              left_box_end( coronal_dim ) -= static_cast< size_t >( left_box_size( coronal_dim ) / 4.0 );
            }
            left_box_end( sagittal_dim ) -= static_cast< size_t >( central_box_size( sagittal_dim ) / 2.0 );
            right_box_end = left_box_end; /* Right */
            right_box_end( sagittal_dim ) = COG( sagittal_dim ) +
	      static_cast< size_t >( central_box_size( sagittal_dim ) / 2.0 +
				     right_box_size( sagittal_dim ) );
            inferior_box_end = COG; /* Inferior */
            if( superior_inferior ) {
              inferior_box_end( axial_dim ) += static_cast< size_t >( 0.5 * central_box_size( axial_dim ) +
                                                                      1.5 * inferior_box_size( axial_dim ) );
            }
            else if( inferior_box_end( axial_dim ) > static_cast< size_t >( 0.5 * central_box_size( axial_dim ) +
                                                                            0.5 * inferior_box_size( axial_dim ) ) ) {
              inferior_box_end( axial_dim ) -= static_cast< size_t >( 0.5 * central_box_size( axial_dim ) +
                                                                      0.5 * inferior_box_size( axial_dim ) );
            }
            else {
              inferior_box_end( axial_dim ) = 0;
            }
            inferior_box_end( coronal_dim ) += static_cast< size_t >( 2.0 * inferior_box_size( coronal_dim ) / 3.0 );
            inferior_box_end( sagittal_dim ) += static_cast< size_t >( inferior_box_size( sagittal_dim ) / 2.0 );
            COMMENT( "Computing box density.", 3 );
            double den_center = Integral::WindowDensity( integral, central_box_end, central_box_size );
            double den_left = Integral::WindowDensity( integral, left_box_end, left_box_size );
            double den_right = Integral::WindowDensity( integral, right_box_end, right_box_size );
            double den_inferior = Integral::WindowDensity( integral, inferior_box_end, inferior_box_size );
            double total_similarity = den_center + ( max_intensity - den_right ) + ( max_intensity - den_left )
	      + den_inferior;
            similarity( COG ) = total_similarity;
            COMMENT( "Updating similarity maximum function value.", 3 );
            if( total_similarity > max_similarity ) {
              COMMENT( "max_intensity: " << max_intensity << ", total_similarity: " << total_similarity, 3 );
              COMMENT( "den_center: " << den_center << ", den_left: " << den_left, 3 );
              COMMENT( ", den_right: " << den_right << ", den_inferior: " << den_inferior, 3 );
              COMMENT( "size_left: " << Integral::WindowVolume( left_box_end ), 3 );
              COMMENT( ", sum_left: " << Integral::WindowIntegralValue( integral, left_box_end, left_box_size ), 3 );
              COMMENT( COG, 3 );
              COMMENT( "size_factor_power: " << size_factor_power, 3 );
              COMMENT( "size_factor: " << size_factor, 3 );
              max_similarity = total_similarity;
              best_COG = COG;
              COMMENT( "Updating rectangle image.", 3 );
              Vector< float > central_box_begin( ( Vector< float >( central_box_end ) ) );
              Vector< float > left_box_begin( ( Vector< float >( left_box_end ) ) );
              Vector< float > right_box_begin( ( Vector< float >( right_box_end ) ) );
              Vector< float > inferior_box_begin( ( Vector< float >( inferior_box_end ) ) );
              for( size_t dms = 0; dms < dimensions; ++dms ) {
                central_box_begin( dms ) -= central_box_size( dms );
                left_box_begin( dms ) -= left_box_size( dms );
                right_box_begin( dms ) -= right_box_size( dms );
                inferior_box_begin( dms ) -= inferior_box_size( dms );
              }
              COMMENT( "Drawing boxes.", 3 );
              best_rectangles( 0 ) = Box( central_box_begin, Vector< float >( central_box_size ),
                                          Color( 1, 255, 0, 0 ), true );
              best_rectangles( 1 ) = Box( left_box_begin, Vector< float >( left_box_size ),
                                          Color( 1, 0, 255, 0 ), true );
              best_rectangles( 2 ) = Box( right_box_begin, Vector< float >( right_box_size ),
                                          Color( 1, 0, 0, 255 ), true );
              best_rectangles( 3 ) = Box( inferior_box_begin, Vector< float >( inferior_box_size ),
                                          Color( 1, 255, 0, 0 ), true );
            }
          } /* end-if valid dimensions. */
        } /*
           * end-for size_factor
           * Updating reference pixel coordinates.
           */
        for( dms = 0; ( dms < dimensions ) &&
	       ( search_window_start( dms ) == search_window_size( dms ) - 1 ); ++dms ) {
          search_window_start( dms ) = 0;
        }
        if( dms < dimensions ) {
          ++search_window_start( dms );
        }
      } while( dms < dimensions );
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
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Invalid input orientation string." ) );
      throw( std::logic_error( msg ) );
    }
  }

#ifdef BIAL_EXPLICIT_HeartCOG

  template Vector< size_t > Heart::WeightedCOG( const Image< int > &img, const std::string &orientation );
  template Vector< size_t > Heart::WeightedCOG( const Image< int > &img, const std::string &orientation,
                                                Vector< Box > &best_rectangles, Image< float > &similarity );
  template Vector< size_t > Heart::WeightedCOG( const Image< llint > &img, const std::string &orientation );
  template Vector< size_t > Heart::WeightedCOG( const Image< llint > &img, const std::string &orientation,
                                                Vector< Box > &best_rectangles, Image< float > &similarity );
  template Vector< size_t > Heart::WeightedCOG( const Image< float > &img, const std::string &orientation );
  template Vector< size_t > Heart::WeightedCOG( const Image< float > &img, const std::string &orientation,
                                                Vector< Box > &best_rectangles, Image< float > &similarity );
  template Vector< size_t > Heart::WeightedCOG( const Image< double > &img, const std::string &orientation );
  template Vector< size_t > Heart::WeightedCOG( const Image< double > &img, const std::string &orientation,
                                                Vector< Box > &best_rectangles, Image< float > &similarity );

#endif

}

#endif

#endif
