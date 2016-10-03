/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2014/Jan/07
 * @brief Class for interpolation of pixels.
 */

#ifndef BIALPIXELINTERPOLATION_H
#define BIALPIXELINTERPOLATION_H

#include "Common.hpp"

namespace Bial {

  template< class D > class Image;

  /**
   * @brief Geometric transforms that change image spatially.
   */
  class PixelInterpolation {

public:

    /**
     * @date 2014/Nov/14
     * @param none.
     * @return none.
     * @brief Virtual destructor required for virtual classes.
     * @warning none.
     */
    virtual ~PixelInterpolation( ) {
    }

    /**
     * @date 2014/Aug/17
     * @param img: An image.
     * @param x, y, z: Pixel coordinates in original.
     * @return The nearest pixel interpolation of given coordinates in img.
     * @brief Computes and returns nearest pixel interpolation of given coordinates in img.
     * @warning none.
     */
    virtual int operator()( const Image< int > &img, float x, float y ) const = 0;
    virtual llint operator()( const Image< llint > &img, float x, float y ) const = 0;
    virtual float operator()( const Image< float > &img, float x, float y ) const = 0;
    virtual double operator()( const Image< double > &img, float x, float y ) const = 0;

    virtual int operator()( const Image< int > &img, float x, float y, float z ) const = 0;
    virtual llint operator()( const Image< llint > &img, float x, float y, float z ) const = 0;
    virtual float operator()( const Image< float > &img, float x, float y, float z ) const = 0;
    virtual double operator()( const Image< double > &img, float x, float y, float z ) const = 0;
  };


  class NearestInterpolation : public PixelInterpolation {

public:

    /**
     * @date 2014/Aug/17
     * @param img: An image.
     * @param x, y, z: Pixel coordinates in original.
     * @return The nearest pixel interpolation of given coordinates in img.
     * @brief Computes and returns nearest pixel interpolation of given coordinates in img.
     * @warning none.
     */
    template< class D >
    D operator()( const Image< D > &img, float x, float y ) const;
    int operator()( const Image< int > &img, float x, float y ) const;
    llint operator()( const Image< llint > &img, float x, float y ) const;
    float operator()( const Image< float > &img, float x, float y ) const;
    double operator()( const Image< double > &img, float x, float y ) const;

    template< class D >
    D operator()( const Image< D > &img, float x, float y, float z ) const;
    int operator()( const Image< int > &img, float x, float y, float z ) const;
    llint operator()( const Image< llint > &img, float x, float y, float z ) const;
    float operator()( const Image< float > &img, float x, float y, float z ) const;
    double operator()( const Image< double > &img, float x, float y, float z ) const;

  };


  class LinearInterpolation : public PixelInterpolation {

    /**
     * @date 2014/Feb/09
     * @param img: An image.
     * @param x, y, z: Pixel coordinates in original.
     * @return The linear interpolation of given coordinates in img.
     * @brief Computes and returns linear interpolation of given coordinates in img.
     * @warning none.
     */
    template< class D >
    D operator()( const Image< D > &img, float x, float y ) const;
    int operator()( const Image< int > &img, float x, float y ) const;
    llint operator()( const Image< llint > &img, float x, float y ) const;
    float operator()( const Image< float > &img, float x, float y ) const;
    double operator()( const Image< double > &img, float x, float y ) const;

    template< class D >
    D operator()( const Image< D > &img, float x, float y, float z ) const;
    int operator()( const Image< int > &img, float x, float y, float z ) const;
    llint operator()( const Image< llint > &img, float x, float y, float z ) const;
    float operator()( const Image< float > &img, float x, float y, float z ) const;
    double operator()( const Image< double > &img, float x, float y, float z ) const;
  };

}

/* Implementation of template functions that are not explicitly instantiated --------------------------------------- **/

#include "Image.hpp"

namespace Bial {

  template< class D >
  D NearestInterpolation::operator()( const Image< D > &img, float x, float y ) const {
    size_t cx = std::round( x );
    size_t cy = std::round( y );
    if( img.ValidCoordinate( cx, cy ) )
      return( img( cx, cy ) );
    return( 0 );
  }

  template< class D >
  D LinearInterpolation::operator()( const Image< D > &img, float x, float y ) const {
    try {
      COMMENT( "Distances from input coordinates to previous discrete coordinate.", 3 );
      Vector< float > floor_dist = { x, y };
      Vector< float > ceil_dist = { x, y };
      Vector< size_t > ceil( 2 );
      Vector< size_t > floor( 2 );
      for( size_t dms = 0; dms < 2; ++dms ) {
        ceil( dms ) = std::ceil( floor_dist( dms ) );
        floor( dms ) = std::floor( floor_dist( dms ) );
        if( ceil( dms ) == floor( dms ) ) {
          if( ceil( dms ) == img.size( dms ) - 1 ) {
            --floor( dms );
          }
          else {
            ++ceil( dms );
          }
        }
        COMMENT( "Normalized distance.", 3 );
        ceil_dist( dms ) = ( ceil( dms ) - floor_dist( dms ) ) / ( ceil( dms ) - floor( dms ) );
        floor_dist( dms ) = 1.0 - ceil_dist( dms );
      }
      COMMENT( "floor: " << floor << ", ceil: " << ceil <<
               ", floor_dist: " << floor_dist << ", ceil_dist: " << ceil_dist, 3 );

      if( ( img.ValidCoordinate( floor( 0 ), floor( 1 ) ) ) && ( img.ValidCoordinate( ceil( 0 ), ceil( 1 ) ) ) ) {
        return( img( floor( 0 ), floor( 1 ) ) * ceil_dist( 0 ) * ceil_dist( 1 ) +
                img( ceil( 0 ), floor( 1 ) ) * floor_dist( 0 ) * ceil_dist( 1 ) +
                img( floor( 0 ), ceil( 1 ) ) * ceil_dist( 0 ) * floor_dist( 1 ) +
                img( ceil( 0 ), ceil( 1 ) ) * floor_dist( 0 ) * floor_dist( 1 ) );
      }
      return( 0 );
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
  D NearestInterpolation::operator()( const Image< D > &img, float x, float y, float z ) const {
    size_t cx = std::round( x );
    size_t cy = std::round( y );
    size_t cz = std::round( z );
    if( img.ValidCoordinate( cx, cy, cz ) )
      return( img( cx, cy, cz ) );
    return( 0 );
  }

  template< class D >
  D LinearInterpolation::operator()( const Image< D > &img, float x, float y, float z ) const {
    try {
      COMMENT( "Distances from input coordinates to previous discrete coordinate.", 3 );
      Vector< float > floor_dist = { x, y, z };
      Vector< float > ceil_dist = { x, y, z };
      Vector< size_t > ceil( 3 );
      Vector< size_t > floor( 3 );
      for( size_t dms = 0; dms < 3; ++dms ) {
        ceil( dms ) = std::ceil( floor_dist( dms ) );
        floor( dms ) = std::floor( floor_dist( dms ) );
        if( ceil( dms ) == floor( dms ) ) {
          if( ceil( dms ) == img.size( dms ) - 1 ) {
            --floor( dms );
          }
          else {
            ++ceil( dms );
          }
        }
        COMMENT( "Normalized distance.", 3 );
        ceil_dist( dms ) = ( ceil( dms ) - floor_dist( dms ) ) / ( ceil( dms ) - floor( dms ) );
        floor_dist( dms ) = 1.0 - ceil_dist( dms );
      }
      COMMENT( "floor: " << floor << ", ceil: " << ceil <<
               ", floor_dist: " << floor_dist << ", ceil_dist: " << ceil_dist, 3 );

      if( ( img.ValidCoordinate( floor( 0 ), floor( 1 ), floor( 2 ) ) ) && 
          ( img.ValidCoordinate( ceil( 0 ), ceil( 1 ), ceil( 2 ) ) ) ) {
        return( img( floor( 0 ), floor( 1 ), floor( 2 ) ) * ceil_dist( 0 ) * ceil_dist( 1 ) * ceil_dist( 2 ) +
                img( ceil( 0 ), floor( 1 ), floor( 2 ) ) * floor_dist( 0 ) * ceil_dist( 1 ) * ceil_dist( 2 ) +
                img( floor( 0 ), ceil( 1 ), floor( 2 ) ) * ceil_dist( 0 ) * floor_dist( 1 ) * ceil_dist( 2 ) +
                img( floor( 0 ), floor( 1 ), ceil( 2 ) ) * ceil_dist( 0 ) * ceil_dist( 1 ) * floor_dist( 2 ) +
                img( ceil( 0 ), ceil( 1 ), floor( 2 ) ) * floor_dist( 0 ) * floor_dist( 1 ) * ceil_dist( 2 ) +
                img( ceil( 0 ), floor( 1 ), ceil( 2 ) ) * floor_dist( 0 ) * ceil_dist( 1 ) * floor_dist( 2 ) +
                img( floor( 0 ), ceil( 1 ), ceil( 2 ) ) * ceil_dist( 0 ) * floor_dist( 1 ) * floor_dist( 2 ) +
                img( ceil( 0 ), ceil( 1 ), ceil( 2 ) ) * floor_dist( 0 ) * floor_dist( 1 ) * floor_dist( 2 ) );
      }
      return( 0 );
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

}

#include "PixelInterpolation.cpp"

#endif
