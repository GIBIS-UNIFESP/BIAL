/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2014/Mar/14
 * @brief 'Virtual' class to draw forms in images.
 */

#include "Color.hpp"
#include "Common.hpp"
#include "Image.hpp"
#include "Vector.hpp"

#ifndef BIALDRAWFIGURE_H
#define BIALDRAWFIGURE_H

namespace Bial {

  template< class DERIVED >
  class Figure {

public:

    /** @brief  The color of the figure. */
    Color color;

    /**
     * @date 2014/Mar/17
     * @param color: color of figure.
     * @return none.
     * @brief Basic Constructor.
     * @warning none.
     */
    Figure( const Color &color );

    /**
     * @date 2014/Mar/17
     * @param img: An image.
     * @return none.
     * @brief Draws figure in input image.
     * @warning none.
     */
    template< class D >
    void Draw( Image< D > &img );

    /**
     * @date 2014/Mar/18
     * @param img: An image.
     * @param displacement: Displacement from low coordinate to start drawing.
     * @return none.
     * @brief Draws n-dimensional figure in input image.
     * @warning none.
     */
    template< class D >
    void Draw( Image< D > &img, const Vector< float > &displacement ) const;

    /**
     * @date 2015/Jul/07
     * @param none.
     * @return Vector with pixels.
     * @brief Returns a vector with coordinates of the pixels from this Figure.
     * @warning none.
     */
    Vector< Vector< int > > Pixels( ) const;

    /**
     * @date 2015/Jul/07
     * @param displacement: Displacement from low coordinate of the figure.
     * @return Vector with pixels.
     * @brief Returns a vector with coordinates of the pixels from this figure.
     * @warning none.
     */
    Vector< Vector< int > > Pixels( const Vector< float > &displacement ) const;

    /**
     * @date 2014/Mar/18
     * @param os: an output stream.
     * @return none.
     * @brief Prints figure information to stream.
     * @warning none.
     */
    template< class O >
    void Print( O &os ) const;

  };

}

namespace Bial {

  template< class DERIVED > 
  Figure< DERIVED >::Figure( const Color &color ) try : color( color ) {
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

  template< class DERIVED > template< class D >
  void Figure< DERIVED >::Draw( Image< D > &img ) {
    static_cast< DERIVED* >( this )->Draw( img );
  }

  template< class DERIVED > template< class D >
  void Figure< DERIVED >::Draw( Image< D > &img, const Vector< float > &displacement ) const {
    static_cast< const DERIVED* >( this )->Draw( img, displacement );
  }

  template< class DERIVED >
  Vector< Vector< int > > Figure< DERIVED >::Pixels( ) const {
    return( static_cast< const DERIVED* >( this )->Pixels( ) );
  }

  template< class DERIVED >
  Vector< Vector< int > > Figure< DERIVED >::Pixels( const Vector< float > &displacement ) const {
    return( static_cast< const DERIVED* >( this )->Pixels( displacement ) );
  }

  template< class DERIVED > template< class O >
  void Figure< DERIVED >::Print( O &os ) const {
    static_cast< const DERIVED* >( this )->Print( os );
  }

}

#endif
