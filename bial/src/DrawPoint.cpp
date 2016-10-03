/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2014/Mar/14
 * @brief Functions to draw points in images.
 */

#ifndef BIALDRAWPOINT_C
#define BIALDRAWPOINT_C

#include "DrawPoint.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_DrawPoint )
#define BIAL_EXPLICIT_DrawPoint
#endif

#if defined ( BIAL_EXPLICIT_DrawPoint ) || ( BIAL_IMPLICIT_BIN )

#include "Color.hpp"
#include "Image.hpp"
#include "RealColor.hpp"

namespace Bial {

  Point::Point( const Vector< float > &coord, const Color color ) try : Figure< Point >( color ), coordinate(
      coord ) {
    if( ( coord.size( ) < 2 ) || ( coord.size( ) > 3 ) ) {
      std::string msg( BIAL_ERROR( "Point must have 2 or 3 dimensions." ) );
      throw( std::runtime_error( msg ) );
    }
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


  template< class D >
  void Point::Draw( Image< D > &img ) const {
    try {
      Vector< float > displacement( img.Dims( ), 0.0f );
      Draw( img, displacement );
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
  void Point::Draw( Image< D > &img, const Vector< float > &displacement )
  const {
    try {
      COMMENT( "Verify if image dimensions match displacement dimensions.", 2 );
      if( img.Dims( ) != displacement.size( ) ) {
        std::string msg( BIAL_ERROR( "Incompatible number of dimensions between image (" + std::to_string( img.Dims( ) )
                                     + ") and displacement (" + std::to_string( displacement.size( ) ) + ")." ) );
        throw( std::logic_error( msg ) );
      }
      size_t dims = img.Dims( );
      COMMENT( "Checking if point is out of the image domain with the displacement.", 2 );
      for( size_t dms = 0; dms < dims; ++dms ) {
        if( ( coordinate( dms ) + displacement( dms ) >= img.size( dms ) ) ||
            ( coordinate( dms ) + displacement( dms ) < 0.0 ) ) {
          return;
        }
      }
      
      COMMENT( "Drawing point. Coordinates: " << coordinate + displacement, 2 );
      size_t pxl = coordinate[ 0 ] + displacement[ 0 ];
      for( size_t dms = 1; dms < dims; ++dms ) {
        pxl += ( coordinate[ dms ] + displacement[ dms ] ) * img.Displacement( dms - 1 );
      }
      COMMENT( "Computing color value.", 2 );
      float maximum = std::max< float >( 255.0f, img.Maximum( ) );
      COMMENT( "maximum: " << maximum, 0 );
      float alpha = this->color( 0 ) / 255.0f;
      float assgn_color = ( this->color( 1 ) + this->color( 2 ) + this->color( 3 ) ) * maximum / ( 3.0f * 255.0f );
      
      img( pxl ) = alpha * assgn_color + ( 1.0f - alpha ) * img( pxl );
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

  template< >
  void Point::Draw( Image< Color > &img, const Vector< float > &displacement )
  const {
    try {
      COMMENT( "Verify if image dimensions match displacement dimensions.", 2 );
      if( img.Dims( ) != displacement.size( ) ) {
        std::string msg( BIAL_ERROR( "Incompatible number of dimensions between image (" + std::to_string( img.Dims( ) )
                                     + ") and displacement (" + std::to_string( displacement.size( ) ) + ")." ) );
        throw( std::logic_error( msg ) );
      }
      size_t dims = img.Dims( );
      COMMENT( "Checking if point is out of the image domain with the displacement.", 2 );
      for( size_t dms = 0; dms < dims; ++dms ) {
        if( ( coordinate( dms ) + displacement( dms ) >= img.size( dms ) ) ||
            ( coordinate( dms ) + displacement( dms ) < 0.0 ) ) {
          return;
        }
      }

      COMMENT( "Computing color value.", 2 );
      RealColor max_color( img.Maximum( ) );
      float maximum = std::max( 255.0f, std::max( max_color( 1 ), std::max( max_color( 2 ), max_color( 3 ) ) ) );
      COMMENT( "maximum: " << maximum, 0 );
      RealColor assgn_color( this->color( 0 ), this->color( 1 ) * maximum / 255.0f, 
                             this->color( 2 ) * maximum / 255.0f, this->color( 3 ) * maximum / 255.0f );
      float alpha = assgn_color( 0 ) / 255.0f;

      COMMENT( "Drawing point. Coordinates: " << coordinate + displacement, 2 );
      size_t pxl = coordinate[ 0 ] + displacement[ 0 ];
      for( size_t dms = 1; dms < dims; ++dms ) {
        pxl += ( coordinate[ dms ] + displacement[ dms ] ) * img.Displacement( dms - 1 );
      }
      for( size_t clr = 1; clr < 4; ++clr ) {
        img( pxl )( clr ) = alpha * assgn_color( clr ) + ( 1.0f - alpha ) * img( pxl )( clr );
      }
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

  Vector< Vector< int > > Point::Pixels( ) const {
    return( Pixels( Vector< float >( coordinate.size( ), 0.0f ) ) );
  }

  Vector< Vector< int > > Point::Pixels( const Vector< float > &displacement ) const {
    return( Vector< Vector< int > >( 1, coordinate + displacement ) );
  }

  template< class O >
  void Point::Print( O &os ) const {
    try {
      if( coordinate.size( ) == 2 ) {
        os << "(" << coordinate[ 0 ] << "," << coordinate[ 1 ] << ")";
      }
      else {
        os << "(" << coordinate[ 0 ] << "," << coordinate[ 1 ] << "," << coordinate[ 2 ] << ")";
      }
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

  template< class O >
  O &operator<<( O &os, const Point &obj ) {
    obj.Print( os );
    return( os );
  }

#ifdef BIAL_EXPLICIT_DrawPoint

  template void Point::Draw( Image< int > &img ) const;
  template void Point::Draw( Image< int > &img, const Vector< float > &displacement ) const;
  template void Point::Draw( Image< llint > &img ) const;
  template void Point::Draw( Image< llint > &img, const Vector< float > &displacement ) const;
  template void Point::Draw( Image< float > &img ) const;
  template void Point::Draw( Image< float > &img, const Vector< float > &displacement ) const;
  template void Point::Draw( Image< double > &img ) const;
  template void Point::Draw( Image< double > &img, const Vector< float > &displacement ) const;
  template void Point::Draw( Image< Color > &img ) const;

  template void Point::Print( OFile &os ) const;
  template void Point::Print( std::ostream &os ) const;
  template OFile &operator<<( OFile &os, const Point &obj );
  template std::ostream &operator<<( std::ostream &os, const Point &obj );


#endif

}

#endif

#endif
