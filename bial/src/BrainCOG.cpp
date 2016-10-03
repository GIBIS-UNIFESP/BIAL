/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Aug/27 
 * @brief Functions to find the COG of a brain image. 
 */

#ifndef BIALBRAINCOG_C
#define BIALBRAINCOG_C

#include "BrainCOG.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_BrainCOG )
#define BIAL_EXPLICIT_BrainCOG
#endif

#if defined ( BIAL_EXPLICIT_BrainCOG ) || ( BIAL_IMPLICIT_BIN )

#include "BinaryCOG.hpp"
#include "Image.hpp"
#include "Integral.hpp"
#include "NiftiHeader.hpp"
#include "SegmentationOtsuThreshold.hpp"

namespace Bial {

  namespace Brain {

    template< class D >
    size_t HeadTopSlice( const Image< D > &img, const std::string &orientation ) {
      if( img.Dims( ) != 3 ) {
        std::string msg( BIAL_ERROR( "Invalid number of dimensions. Expected 3." ) );
        throw( std::logic_error( msg ) );
      }
      try {
        COMMENT( "Computing otsu threshold.", 0 );
        D otsu_threshold = Segmentation::OtsuThreshold( img );
        COMMENT( "Defining dimensions for axial, sagittal, and coronal slices.", 0 );
        size_t axial_dim = NiftiHeader::AxialDimension( orientation );
        size_t coronal_dim = NiftiHeader::CoronalDimension( orientation );
        size_t sagittal_dim = NiftiHeader::SagittalDimension( orientation );
        COMMENT( "Defining superior -> inferior direction.", 0 );
        bool superior_inferior = NiftiHeader::SuperiorInferior( orientation );
        COMMENT( "Defining sagittal, coronal, and axial displacements.", 0 );
        size_t axial_displacement = 1;
        if( axial_dim > 0 ) {
          axial_displacement = img.Displacement( axial_dim - 1 );
        }
        size_t coronal_displacement = 1;
        if( coronal_dim > 0 ) {
          coronal_displacement = img.Displacement( coronal_dim - 1 );
        }
        size_t sagittal_displacement = 1;
        if( sagittal_dim > 0 ) {
          sagittal_displacement = img.Displacement( sagittal_dim - 1 );
        }
        COMMENT( "Setting loop thresholds", 0 );
        // Matrix< D > max = Matrix< D >( img.size( 0 ) );
        int axl_n = static_cast< int >( img.size( axial_dim ) * axial_displacement );
        size_t crl_n = img.size( coronal_dim ) * coronal_displacement;
        size_t sgt_n = img.size( sagittal_dim ) * sagittal_displacement;

        COMMENT( "Locating axial cut slice.", 0 );
        size_t pxl = 0;
        if( superior_inferior ) {
          for( int axl = 0; axl < axl_n; axl += axial_displacement ) {
            for( size_t crl = 0; crl < crl_n; crl += coronal_displacement ) {
              for( size_t sgt = 0; sgt < sgt_n; sgt += sagittal_displacement ) {
                pxl = axl + crl + sgt;
                if( img[ pxl ] >= otsu_threshold ) {
                  return( axl / axial_displacement );
                }
              }
            }
          }
        }
        else { /* inferior_superior */
          for( int axl = axl_n - axial_displacement; axl >= 0; axl -= axial_displacement ) {
            for( size_t crl = 0; crl < crl_n; crl += coronal_displacement ) {
              for( size_t sgt = 0; sgt < sgt_n; sgt += sagittal_displacement ) {
                pxl = axl + crl + sgt;
                if( img[ pxl ] >= otsu_threshold ) {
                  return( axl / axial_displacement );
                }
              }
            }
          }
        }
        std::string msg( BIAL_ERROR( "Blank image." ) );
        throw( std::domain_error( msg ) );
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
    Vector< size_t > COGbyIntensityThreshold( const Image< D > &img ) {
      try {
        if( img.Dims( ) != 3 ) {
          std::string msg( BIAL_ERROR( "Invalid number of dimensions. Expected 3." ) );
          throw( std::logic_error( msg ) );
        }
        D max = img.Maximum( );
        D mean = max / 2.0;
        Image< D > bin( img.Dim( ), img.PixelSize( ) );
        for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
          if( img( pxl ) >= mean )
            bin( pxl ) = 1;
        }
        return( Binary::COG( bin ) );
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
    Vector< size_t > COGbyWeightedIntensity( const Image< D > &img ) {
      try {
        if( img.Dims( ) != 3 ) {
          std::string msg( BIAL_ERROR( "Invalid number of dimensions. Expected 3." ) );
          throw( std::logic_error( msg ) );
        }
        D max = img.Maximum( );
        COMMENT( "Computing local and total weights.", 0 );
        Vector< double > coord_weight( img.Dims( ), 0.0 );
        double total_weight = 0.0;
        for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
          if( img[ pxl ] == 0.0 ) {
            continue;
          }
          Vector< size_t > coord = img.Coordinates( pxl );
          double weight = ( static_cast< double >( img[ pxl ] ) ) / ( static_cast< double >( max ) );
          for( size_t dms = 0; dms < img.Dims( ); ++dms ) {
            coord_weight( dms ) += weight * coord( dms );
          }
          total_weight += weight;
        }
        COMMENT( "Computing resulting COG.", 0 );
        Vector< size_t > result( img.Dims( ) );
        if( total_weight == 0.0 ) {
          for( size_t dms = 0; dms < img.Dims( ); ++dms ) {
            result( dms ) = img.size( dms ) / 2;
          }
        }
        else {
          for( size_t dms = 0; dms < img.Dims( ); ++dms ) {
            result( dms ) = static_cast< size_t >( coord_weight( dms ) / total_weight + 0.5 ); /* changed std::round +0.5
                                                                                               **/
          }
        }
        return( result );
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
    Vector< size_t > COGbyHeadTopDistance( const Image< D > &img, const std::string &orientation ) {
      if( img.Dims( ) != 3 ) {
        std::string msg( BIAL_ERROR( "Invalid number of dimensions. Expected 3." ) );
        throw( std::logic_error( msg ) );
      }
      try {
        float brain_axial_size_mm = 82.0; /* Brain axial size in millimeters */
        float brain_axial_std_dev_mm = 25.0; /* Brain axial size standard deviation in in millimeters */
        size_t head_top = HeadTopSlice( img, orientation );
        size_t axial_dim = NiftiHeader::AxialDimension( orientation );
        bool superior_inferior = NiftiHeader::SuperiorInferior( orientation );
        float brain_axial_size = brain_axial_size_mm / img.PixelSize( axial_dim );
        float brain_axial_std_dev = brain_axial_std_dev_mm / img.PixelSize( axial_dim );
        Vector< double > coord_weight( img.Dims( ), 0.0 );
        double total_weight = 0.0;
        D max = img.Maximum( );

        COMMENT( "Computing local and total weights.", 0 );
        for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
          if( img[ pxl ] == 0.0 ) {
            continue;
          }
          Vector< size_t > coord = img.Coordinates( pxl );
          size_t axial_dist;
          if( superior_inferior ) {
            COMMENT( "Superior-inferior orientation.", 4 );
            if( coord( axial_dim ) < head_top ) {
              continue;
            }
            axial_dist = coord( axial_dim ) - head_top;
          }
          else {
            COMMENT( "Inferior-superior orientation.", 4 );
            if( coord( axial_dim ) > head_top ) {
              continue;
            }
            axial_dist = head_top - coord( axial_dim );
          }
          COMMENT( "Axial distance weight.", 4 );
          double distance_weight = 1.0;
          if( axial_dist > brain_axial_size ) {
            distance_weight = exp( -pow( axial_dist - brain_axial_size, 2.0 ) /
                                   ( 2.0 * pow( brain_axial_std_dev, 2.0 ) ) );
          }
          COMMENT( "Defining the intensity weight.", 4 );
          double intensity_weight = ( static_cast< double >( img[ pxl ] ) ) / ( static_cast< double >( max ) );
          double weight = intensity_weight * distance_weight;
          for( size_t dms = 0; dms < img.Dims( ); ++dms ) {
            coord_weight( dms ) += weight * coord( dms );
          }
          total_weight += weight;
        }
        COMMENT( "Computing resulting COG.", 0 );
        Vector< size_t > result( img.Dims( ) );
        if( total_weight == 0.0 ) {
          for( size_t dms = 0; dms < img.Dims( ); ++dms ) {
            result( dms ) = img.size( dms ) / 2;
          }
        }
        else {
          for( size_t dms = 0; dms < img.Dims( ); ++dms ) {
            result( dms ) = static_cast< size_t >( coord_weight( dms ) / total_weight + 0.5 ); /* changed std::round + 0.5
                                                                                               **/
          }
        }
        return( result );
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
    Vector< size_t > COGbyIntegralImage( const Image< D > &img, const std::string &orientation, 
                                              MRIModality modality ) {
      if( img.Dims( ) != 3 ) {
        std::string msg( BIAL_ERROR( "Invalid number of dimensions. Expected 3." ) );
        throw( std::logic_error( msg ) );
      }
      try {
        COMMENT( "Setting initial variables.", 0 );
        float window_length = 20.0;
        D max_intensity = img.Maximum( );
        Vector< size_t > initial_COG = COGbyHeadTopDistance( img, orientation );
        size_t axial_dim = NiftiHeader::AxialDimension( orientation );
        size_t coronal_dim = NiftiHeader::CoronalDimension( orientation );
        size_t sagittal_dim = NiftiHeader::SagittalDimension( orientation );
        bool superior_inferior = NiftiHeader::SuperiorInferior( orientation );
        COMMENT( "Computing integral image.", 0 );
        Image< double > integral = Integral::IntegralImage( img );
        COMMENT( "Computing search window dimensions.", 0 );
        Vector< size_t > search_window_size( img.Dims( ) );
        for( size_t dms = 0; dms < img.Dims( ); ++dms ) {
          search_window_size( dms ) = static_cast< size_t >( img.PixelSize( dms ) * window_length );
        }
        Vector< size_t > search_window_start( img.Dims( ), 0 );
        COMMENT( "Initializing optimization variables.", 0 );
        Vector< size_t > best_COG( initial_COG );
        float max_similarity = std::numeric_limits< float >::min( );
        size_t dms;
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
          COMMENT( "Computing similarity for current reference pixel.", 3 );
          if( dms == integral.Dims( ) ) { /* Valid position. */
            COMMENT( "Central box similarity.", 4 );
            Vector< size_t > box_end( COG );
            Vector< size_t > box_size( img.Dims( ) );
            box_size( sagittal_dim ) = static_cast< size_t >( 50.0 / img.PixelSize( sagittal_dim ) + 0.5 );
            box_end( sagittal_dim ) += static_cast< size_t >( 25.0 / img.PixelSize( sagittal_dim ) + 0.5 );
            box_size( axial_dim ) = static_cast< size_t >( 60.0 / img.PixelSize( axial_dim ) + 0.5 );
            box_end( axial_dim ) += static_cast< size_t >( 30.0 / img.PixelSize( axial_dim ) + 0.5 );
            box_size( coronal_dim ) = static_cast< size_t >( 50.0 / img.PixelSize( coronal_dim ) + 0.5 );
            box_end( coronal_dim ) += static_cast< size_t >( 25.0 / img.PixelSize( coronal_dim ) + 0.5 );
            double den_center = 6.0 * max_intensity;
            if( modality == MRIModality::PD ) {
              den_center = -6.0 * max_intensity;
            }
            if( integral.ValidCoordinate( box_end ) ) {
              den_center = Integral::WindowDensity( integral, box_end, box_size );
            }
            COMMENT( "Left and right boxes similarity.", 4 );
            box_size( sagittal_dim ) = static_cast< size_t >( 30.0 / img.PixelSize( sagittal_dim ) + 0.5 );
            box_end( sagittal_dim ) -= static_cast< size_t >( 50.0 / img.PixelSize( sagittal_dim ) + 0.5 );
            double den_left = -6.0 * max_intensity;
            if( integral.ValidCoordinate( box_end ) ) {
              den_left = Integral::WindowDensity( integral, box_end, box_size );
            }
            box_end( sagittal_dim ) += static_cast< size_t >( 80.0 / img.PixelSize( sagittal_dim ) + 0.5 );
            double den_right = -6.0 * max_intensity;
            if( integral.ValidCoordinate( box_end ) ) {
              den_right = Integral::WindowDensity( integral, box_end, box_size );
            }
            COMMENT( "Superior box similarity.", 4 );
            box_size( sagittal_dim ) = static_cast< size_t >( 50.0 / img.PixelSize( sagittal_dim ) + 0.5 );
            box_end( sagittal_dim ) -= static_cast< size_t >( 30.0 / img.PixelSize( sagittal_dim ) + 0.5 );
            box_size( axial_dim ) = static_cast< size_t >( 30.0 / img.PixelSize( axial_dim ) + 0.5 );
            if( superior_inferior ) {
              box_end( axial_dim ) -= static_cast< size_t >( 50.0 / img.PixelSize( axial_dim ) + 0.5 );
            }
            else { /* if( inferior_superior ) */
              box_end( axial_dim ) += static_cast< size_t >( 30.0 / img.PixelSize( axial_dim ) + 0.5 );
            }
            double den_superior = -6.0 * max_intensity;
            if( integral.ValidCoordinate( box_end ) ) {
              den_superior = Integral::WindowDensity( integral, box_end, box_size );
            }
            COMMENT( "Anterior and posterior boxes similarity.", 4 );
            box_size( axial_dim ) = static_cast< size_t >( 50.0 / img.PixelSize( axial_dim ) + 0.5 );
            if( superior_inferior ) {
              box_end( axial_dim ) += static_cast< size_t >( 50.0 / img.PixelSize( axial_dim ) + 0.5 );
            }
            else { /* if( inferior_superior ) */
              box_end( axial_dim ) -= static_cast< size_t >( 30.0 / img.PixelSize( axial_dim ) + 0.5 );
            }
            box_size( coronal_dim ) = static_cast< size_t >( 30.0 / img.PixelSize( coronal_dim ) + 0.5 );
            box_end( coronal_dim ) -= static_cast< size_t >( 50.0 / img.PixelSize( coronal_dim ) + 0.5 );
            double den_anterior = -6.0 * max_intensity;
            if( integral.ValidCoordinate( box_end ) ) {
              den_anterior = Integral::WindowDensity( integral, box_end, box_size );
            }
            box_end( coronal_dim ) += static_cast< size_t >( 80.0 / img.PixelSize( coronal_dim ) + 0.5 );
            double den_posterior = -6.0 * max_intensity;
            if( integral.ValidCoordinate( box_end ) ) {
              den_posterior = Integral::WindowDensity( integral, box_end, box_size );
            }
            COMMENT( "Total similarity.", 4 );
            float similarity;
            if( modality == MRIModality::T2 ) {
              similarity = ( max_intensity - den_center ) + den_right + den_left + den_superior +
                den_anterior + den_posterior;
            }
            else if( modality == MRIModality::PD ) {
              similarity = 4 * den_center + den_right + den_left + den_superior + den_anterior + den_posterior;
            }
            else if( modality == MRIModality::T1 ) {
              similarity = ( max_intensity - den_center ) + den_right + den_left + den_superior + den_anterior +
                den_posterior;
            }
            else {
              std::string msg( BIAL_ERROR( "Invalid image modality." ) );
              throw( std::logic_error( msg ) );
            }
            COMMENT( "Updating similarity maximum function value.", 4 );
            if( similarity > max_similarity ) {
              max_similarity = similarity;
              best_COG = COG;
            }
          }
          COMMENT( "Updating reference pixel coordinates.", 3 );
          for( dms = 0; ( dms < integral.Dims( ) ) && ( search_window_start( dms ) == search_window_size( dms ) - 1 );
               ++dms ) {
            search_window_start( dms ) = 0;
          }
          if( dms < integral.Dims( ) ) {
            search_window_start( dms )++;
          }
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

#ifdef BIAL_EXPLICIT_BrainCOG

    template size_t HeadTopSlice( const Image< int > &img, const std::string &orientation );
    template Vector< size_t > COGbyIntensityThreshold( const Image< int > &img );
    template Vector< size_t > COGbyWeightedIntensity( const Image< int > &img );
    template Vector< size_t > COGbyHeadTopDistance( const Image< int > &img, const std::string &orientation );
    template Vector< size_t > COGbyIntegralImage( const Image< int > &img, const std::string &orientation, 
                                                  MRIModality modality );

    template size_t HeadTopSlice( const Image< llint > &img, const std::string &orientation );
    template Vector< size_t > COGbyIntensityThreshold( const Image< llint > &img );
    template Vector< size_t > COGbyWeightedIntensity( const Image< llint > &img );
    template Vector< size_t > COGbyHeadTopDistance( const Image< llint > &img, const std::string &orientation );
    template Vector< size_t > COGbyIntegralImage( const Image< llint > &img, const std::string &orientation, 
                                                  MRIModality modality );
    
    template size_t HeadTopSlice( const Image< float > &img, const std::string &orientation );
    template Vector< size_t > COGbyIntensityThreshold( const Image< float > &img );
    template Vector< size_t > COGbyWeightedIntensity( const Image< float > &img );
    template Vector< size_t > COGbyHeadTopDistance( const Image< float > &img, const std::string &orientation );
    template Vector< size_t > COGbyIntegralImage( const Image< float > &img, const std::string &orientation, 
                                                  MRIModality modality );
  
    template size_t HeadTopSlice( const Image< double > &img, const std::string &orientation );
    template Vector< size_t > COGbyIntensityThreshold( const Image< double > &img );
    template Vector< size_t > COGbyWeightedIntensity( const Image< double > &img );
    template Vector< size_t > COGbyHeadTopDistance( const Image< double > &img, const std::string &orientation );
    template Vector< size_t > COGbyIntegralImage( const Image< double > &img, const std::string &orientation, 
                                                  MRIModality modality );

#endif

  }

}

#endif

#endif
