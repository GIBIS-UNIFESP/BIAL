/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2014/Mar/14
 * @brief Functions to draw lines in images.
 */

#ifndef BIALDRAWLINE_C
#define BIALDRAWLINE_C

#include "DrawLine.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_DrawLine )
#define BIAL_EXPLICIT_DrawLine
#endif

#if defined ( BIAL_EXPLICIT_DrawLine ) || ( BIAL_IMPLICIT_BIN )

#include "Color.hpp"
#include "Image.hpp"
#include "RealColor.hpp"

namespace Bial {

  /* Line ---------------------------------------------------------------------------------------------------------- **/

  Line::Line( const Vector< float > &first_coord, const Vector< float > &last_coord,
              const Color color ) try : Figure< Line >( color ), first_coord( first_coord ), last_coord(
      last_coord ) {
    /* Checking if begin coordinate dimensions matches end coordidate dimensions. */
    if( first_coord.size( ) != last_coord.size( ) ) {
      std::string msg( BIAL_ERROR(
                         "Incompatible number of dimensions between begin coordinate and end coordinate vectors." ) );
      throw( std::logic_error( msg ) );
    }
    if( ( first_coord.size( ) < 2 ) || ( first_coord.size( ) > 3 ) ) {
      std::string msg( BIAL_ERROR( "Image must have 2 or 3 dimensions!" ) );
      throw( std::logic_error( msg ) );
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

  Line Line::BisectionLine( const Vector< float > &pnt_0, const Vector< float > &pnt_1, float length ) {
    try {
      COMMENT( "Getting point coordinates.", 2 );
      float x0 = pnt_0[ 0 ], x1 = pnt_1[ 0 ];
      float y0 = pnt_0[ 1 ], y1 = pnt_1[ 1 ];
      COMMENT( "Computing mean coordinates.", 2 );
      float mx = ( x0 + x1 ) / 2.0;
      float my = ( y0 + y1 ) / 2.0;
      COMMENT( "Computing slopes.", 2 );
      float slope = ( x0 - x1 ) / ( y1 - y0 );
      float inv_slope = ( y1 - y0 ) / ( x0 - x1 );
      COMMENT( "Computing minimum and maximum coordinates for the bisection.", 2 );
      float min_x = std::min( x0, x1 ) - length / 2.0f;
      float max_x = std::max( x0, x1 ) + length / 2.0f;
      float min_y = std::min( y0, y1 ) - length / 2.0f;
      float max_y = std::max( y0, y1 ) + length / 2.0f;
      COMMENT( "Computing initial bisection coordinates.", 2 );
      float xi = min_x, xf = max_x;
      float yi = min_y, yf = max_y;
      if( y1 == y0 ) {
        COMMENT( "Horizontal line.", 3 );
        xi = mx;
        xf = mx;
      }
      else if( x0 == x1 ) {
        COMMENT( "Vertical line.", 3 );
        yi = my;
        yf = my;
      }
      else {
        if( std::abs( slope ) <= 1.0 ) {
          COMMENT( "High slope line.", 3 );
          yi = my - ( mx - min_x ) * slope;
          yf = my + ( max_x - mx ) * slope;
        }
        else {
          COMMENT( "Low slope line.", 3 );
          xi = mx - ( my - min_y ) * inv_slope;
          xf = mx + ( max_y - my ) * inv_slope;
        }
      }
      return( Line( { { xi, yi } }, { { xf, yf } } ) );
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
  void Line::Draw( Image< D > &img ) const {
    try {
      COMMENT( "Verify if image dimensions match line coordinate dimensions.", 2 );
      if( img.Dims( ) != first_coord.size( ) ) {
        std::string msg( BIAL_ERROR( "Incompatible number of dimensions between image and line coordinates." ) );
        throw( std::logic_error( msg ) );
      }
      Draw( img, Vector< float >( img.Dims( ), 0.0f ) );
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
  void Line::Draw( Image< D > &img, const Vector< float > &displacement ) const {
    try {
      COMMENT( "Verify if image dimensions match line coordinate dimensions.", 2 );
      if( img.Dims( ) != first_coord.size( ) ) {
        std::string msg( BIAL_ERROR( "Incompatible number of dimensions between image and line coordinates." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Verify if displacement dimensions match line coordinate dimensions.", 2 );
      if( displacement.size( ) != first_coord.size( ) ) {
        std::string msg( BIAL_ERROR( "Incompatible number of dimensions between displacement and line coordinates." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Computing color value.", 2 );
      float maximum = std::max< float >( 255.0f, img.Maximum( ) );
      float alpha = this->color( 0 ) / 255.0f;
      float assgn_color = ( this->color( 1 ) + this->color( 2 ) + this->color( 3 ) ) * maximum / ( 3.0f * 255.0f );

      COMMENT( "Bresenham method.", 2 );
      int x = static_cast< int >( displacement[ 0 ] ) + static_cast< int >( first_coord[ 0 ] );
      int y = static_cast< int >( displacement[ 1 ] ) + static_cast< int >( first_coord[ 1 ] );
      int z = ( img.Dims( ) > 2 ) ? ( static_cast< int >( displacement[ 2 ] + first_coord[ 2 ] ) ) : 0;
      COMMENT( "Start = (" << x << ", " << y << ", " << z << ").", 3 );
      const int xf = static_cast< int >( displacement[ 0 ] ) + static_cast< int >( last_coord[ 0 ] );
      const int yf = static_cast< int >( displacement[ 1 ] ) + static_cast< int >( last_coord[ 1 ] );
      const int zf = ( img.Dims( ) > 2 ) ? ( static_cast< int >( displacement[ 2 ] + last_coord[ 2 ] ) ) : 0;
      COMMENT( "End = (" << xf << ", " << yf << ", " << zf << ").", 3 );

      int xd = 0, yd = 0, zd = 0;
      const int dx = xf - x;
      const int dy = yf - y;
      const int dz = zf - z;
      COMMENT( "Difference = (" << dx << ", " << dy << ", " << dz << ").", 3 );
      const int ax = std::abs( dx ) << 1;
      const int ay = std::abs( dy ) << 1;
      const int az = std::abs( dz ) << 1;
      COMMENT( "Abs. Difference = (" << ax << ", " << ay << ", " << az << ").", 3 );
      const int sx = ( dx < 0 ) ? -1 : 1;
      const int sy = ( dy < 0 ) ? -1 : 1;
      const int sz = ( dz < 0 ) ? -1 : 1;
      COMMENT( "Slope = (" << sx << ", " << sy << ", " << sz << ").", 3 );
      const int x_sz = static_cast< int >( img.Size( 0 ) );
      const int y_sz = static_cast< int >( img.Size( 1 ) );
      const int z_sz = ( img.Dims( ) > 2 ) ? ( static_cast< int >( img.Size( 2 ) ) ) : 1;
      COMMENT( "Image size = (" << x_sz << ", " << y_sz << ", " << z_sz << ").", 3 );
      const int zFactor = x_sz * y_sz;
      if( ax >= std::max( ay, az ) ) {
        COMMENT( "X dominant.", 3 );
        yd = ay - ( ax >> 1 );
        zd = az - ( ax >> 1 );
        while( true ) {
          if( ( x >= 0 ) && ( x < x_sz ) && ( y >= 0 ) && ( y < y_sz ) && ( z >= 0 ) && ( z < z_sz ) ) {
            img[ x + y * x_sz + z * zFactor ] =
              alpha * assgn_color + ( 1.0f - alpha ) * img( x + y * x_sz + z * zFactor );
          }
          if( x == xf ) {
            break;
          }
          if( yd >= 0 ) {
            y += sy;
            yd -= ax;
          }
          if( zd >= 0 ) {
            z += sz;
            zd -= ax;
          }
          x += sx;
          yd += ay;
          zd += az;
        }
      }
      else if( ay >= std::max( ax, az ) ) {
        COMMENT( "Y dominant.", 3 );
        xd = ax - ( ay >> 1 );
        zd = az - ( ay >> 1 );
        while( true ) {
          if( ( x >= 0 ) && ( x < x_sz ) && ( y >= 0 ) && ( y < y_sz ) && ( z >= 0 ) && ( z < z_sz ) ) {
            img[ x + y * x_sz + z * zFactor ] =
              alpha * assgn_color + ( 1.0f - alpha ) * img( x + y * x_sz + z * zFactor );
          }
          if( y == yf ) {
            break;
          }
          if( xd >= 0 ) {
            x += sx;
            xd -= ay;
          }
          if( zd >= 0 ) {
            z += sz;
            zd -= ay;
          }
          y += sy;
          xd += ax;
          zd += az;
        }
      }
      else if( az >= std::max( ax, ay ) ) {
        COMMENT( "Z dominant.", 3 );
        xd = ax - ( az >> 1 );
        yd = ay - ( az >> 1 );
        while( true ) {
          if( ( x >= 0 ) && ( x < x_sz ) && ( y >= 0 ) && ( y < y_sz ) && ( z >= 0 ) && ( z < z_sz ) ) {
            img[ x + y * x_sz + z * zFactor ] =
              alpha * assgn_color + ( 1.0f - alpha ) * img( x + y * x_sz + z * zFactor );
          }
          if( z == zf ) {
            break;
          }
          if( xd >= 0 ) {
            x += sx;
            xd -= az;
          }
          if( yd >= 0 ) {
            y += sy;
            yd -= az;
          }
          z += sz;
          xd += ax;
          yd += ay;
        }
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

  template< >
  void Line::Draw( Image< Color > &img, const Vector< float > &displacement ) const {
    try {
      COMMENT( "Verify if image dimensions match line coordinate dimensions.", 2 );
      if( img.Dims( ) != first_coord.size( ) ) {
        std::string msg( BIAL_ERROR( "Incompatible number of dimensions between image and line coordinates." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Verify if displacement dimensions match line coordinate dimensions.", 2 );
      if( displacement.size( ) != first_coord.size( ) ) {
        std::string msg( BIAL_ERROR( "Incompatible number of dimensions between displacement and line coordinates." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Computing color value.", 2 );
      RealColor max_color( img.Maximum( ) );
      float maximum = std::max( 255.0f, std::max( max_color( 1 ), std::max( max_color( 2 ), max_color( 3 ) ) ) );
      COMMENT( "maximum: " << maximum, 0 );
      RealColor assgn_color( this->color( 0 ), this->color( 1 ) * maximum / 255.0f,
                             this->color( 2 ) * maximum / 255.0f, this->color( 3 ) * maximum / 255.0f );
      float alpha = assgn_color( 0 ) / 255.0f;

      COMMENT( "Bresenham method.", 2 );
      int x = static_cast< int >( displacement[ 0 ] ) + static_cast< int >( first_coord[ 0 ] );
      int y = static_cast< int >( displacement[ 1 ] ) + static_cast< int >( first_coord[ 1 ] );
      int z = ( img.Dims( ) > 2 ) ? ( static_cast< int >( displacement[ 2 ] + first_coord[ 2 ] ) ) : 0;
      COMMENT( "Start = (" << x << ", " << y << ", " << z << ").", 3 );
      const int xf = static_cast< int >( displacement[ 0 ] ) + static_cast< int >( last_coord[ 0 ] );
      const int yf = static_cast< int >( displacement[ 1 ] ) + static_cast< int >( last_coord[ 1 ] );
      const int zf = ( img.Dims( ) > 2 ) ? ( static_cast< int >( displacement[ 2 ] + last_coord[ 2 ] ) ) : 0;
      COMMENT( "End = (" << xf << ", " << yf << ", " << zf << ").", 3 );

      int xd = 0, yd = 0, zd = 0;
      const int dx = xf - x;
      const int dy = yf - y;
      const int dz = zf - z;
      COMMENT( "Difference = (" << dx << ", " << dy << ", " << dz << ").", 3 );
      const int ax = std::abs( dx ) << 1;
      const int ay = std::abs( dy ) << 1;
      const int az = std::abs( dz ) << 1;
      COMMENT( "Abs. Difference = (" << ax << ", " << ay << ", " << az << ").", 3 );
      const int sx = ( dx < 0 ) ? -1 : 1;
      const int sy = ( dy < 0 ) ? -1 : 1;
      const int sz = ( dz < 0 ) ? -1 : 1;
      COMMENT( "Slope = (" << sx << ", " << sy << ", " << sz << ").", 3 );
      const int x_sz = static_cast< int >( img.Size( 0 ) );
      const int y_sz = static_cast< int >( img.Size( 1 ) );
      const int z_sz = ( img.Dims( ) > 2 ) ? ( static_cast< int >( img.Size( 2 ) ) ) : 1;
      COMMENT( "Image size = (" << x_sz << ", " << y_sz << ", " << z_sz << ").", 3 );
      const int zFactor = x_sz * y_sz;
      if( ax >= std::max( ay, az ) ) {
        COMMENT( "X dominant.", 3 );
        yd = ay - ( ax >> 1 );
        zd = az - ( ax >> 1 );
        while( true ) {
          if( ( x >= 0 ) && ( x < x_sz ) && ( y >= 0 ) && ( y < y_sz ) && ( z >= 0 ) && ( z < z_sz ) ) {
            for( size_t clr = 1; clr < 4; ++clr ) {
              img( x + y * x_sz + z * zFactor ) ( clr ) =
                alpha * assgn_color[ clr ] + ( 1.0f - alpha ) * img( x + y * x_sz + z * zFactor ) ( clr );
            }
          }
          if( x == xf ) {
            break;
          }
          if( yd >= 0 ) {
            y += sy;
            yd -= ax;
          }
          if( zd >= 0 ) {
            z += sz;
            zd -= ax;
          }
          x += sx;
          yd += ay;
          zd += az;
        }
      }
      else if( ay >= std::max( ax, az ) ) {
        COMMENT( "Y dominant.", 3 );
        xd = ax - ( ay >> 1 );
        zd = az - ( ay >> 1 );
        while( true ) {
          if( ( x >= 0 ) && ( x < x_sz ) && ( y >= 0 ) && ( y < y_sz ) && ( z >= 0 ) && ( z < z_sz ) ) {
            for( size_t clr = 1; clr < 4; ++clr ) {
              img( x + y * x_sz + z * zFactor ) ( clr ) =
                alpha * assgn_color[ clr ] + ( 1.0f - alpha ) * img( x + y * x_sz + z * zFactor ) ( clr );
            }
          }
          if( y == yf ) {
            break;
          }
          if( xd >= 0 ) {
            x += sx;
            xd -= ay;
          }
          if( zd >= 0 ) {
            z += sz;
            zd -= ay;
          }
          y += sy;
          xd += ax;
          zd += az;
        }
      }
      else if( az >= std::max( ax, ay ) ) {
        COMMENT( "Z dominant.", 3 );
        xd = ax - ( az >> 1 );
        yd = ay - ( az >> 1 );
        while( true ) {
          if( ( x >= 0 ) && ( x < x_sz ) && ( y >= 0 ) && ( y < y_sz ) && ( z >= 0 ) && ( z < z_sz ) ) {
            for( size_t clr = 1; clr < 4; ++clr ) {
              img( x + y * x_sz + z * zFactor ) ( clr ) =
                alpha * assgn_color[ clr ] + ( 1.0f - alpha ) * img( x + y * x_sz + z * zFactor ) ( clr );
            }
          }
          if( z == zf ) {
            break;
          }
          if( xd >= 0 ) {
            x += sx;
            xd -= az;
          }
          if( yd >= 0 ) {
            y += sy;
            yd -= az;
          }
          z += sz;
          xd += ax;
          yd += ay;
        }
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

  Vector< Vector< int > > Line::Pixels( ) const {
    return( Pixels( Vector< float >( first_coord.size( ), 0.0f ) ) );
  }

  Vector< Vector< int > > Line::Pixels( const Vector< float > &displacement ) const {
    try {
      COMMENT( "Verify if displacement dimensions match line coordinate dimensions.", 2 );
      if( displacement.size( ) != first_coord.size( ) ) {
        std::string msg( BIAL_ERROR( "Incompatible number of dimensions between displacement and line coordinates." ) );
        throw( std::logic_error( msg ) );
      }
      Vector< Vector< int > > res;

      COMMENT( "Bresenham method.", 2 );
      int x = static_cast< int >( displacement[ 0 ] ) + static_cast< int >( first_coord[ 0 ] );
      int y = static_cast< int >( displacement[ 1 ] ) + static_cast< int >( first_coord[ 1 ] );
      int z = ( first_coord.size( ) > 2 ) ? ( static_cast< int >( displacement[ 2 ] + first_coord[ 2 ] ) ) : 0;
      COMMENT( "Start = (" << x << ", " << y << ", " << z << ").", 3 );
      const int xf = static_cast< int >( displacement[ 0 ] ) + static_cast< int >( last_coord[ 0 ] );
      const int yf = static_cast< int >( displacement[ 1 ] ) + static_cast< int >( last_coord[ 1 ] );
      const int zf = ( first_coord.size( ) > 2 ) ? ( static_cast< int >( displacement[ 2 ] + last_coord[ 2 ] ) ) : 0;
      COMMENT( "End = (" << xf << ", " << yf << ", " << zf << ").", 3 );

      int xd = 0, yd = 0, zd = 0;
      const int dx = xf - x;
      const int dy = yf - y;
      const int dz = zf - z;
      COMMENT( "Difference = (" << dx << ", " << dy << ", " << dz << ").", 3 );
      const int ax = std::abs( dx ) << 1;
      const int ay = std::abs( dy ) << 1;
      const int az = std::abs( dz ) << 1;
      COMMENT( "Abs. Difference = (" << ax << ", " << ay << ", " << az << ").", 3 );
      const int sx = ( dx < 0 ) ? -1 : 1;
      const int sy = ( dy < 0 ) ? -1 : 1;
      const int sz = ( dz < 0 ) ? -1 : 1;
      COMMENT( "Slope = (" << sx << ", " << sy << ", " << sz << ").", 3 );
      if( ax >= std::max( ay, az ) ) {
        COMMENT( "X dominant.", 3 );
        yd = ay - ( ax >> 1 );
        zd = az - ( ax >> 1 );
        while( true ) {
          if( first_coord.size( ) == 2 ) {
            res.push_back( { x, y } );
          }
          else if( first_coord.size( ) == 3 ) {
            res.push_back( { x, y, z } );
          }
          /* COMMENT("X = " << x << ", XF = " << xf << ".", 3); */
          if( x == xf ) {
            break;
          }
          if( yd >= 0 ) {
            y += sy;
            yd -= ax;
          }
          if( zd >= 0 ) {
            z += sz;
            zd -= ax;
          }
          x += sx;
          yd += ay;
          zd += az;
        }
      }
      else if( ay >= std::max( ax, az ) ) {
        COMMENT( "Y dominant.", 3 );
        xd = ax - ( ay >> 1 );
        zd = az - ( ay >> 1 );
        while( true ) {
          if( first_coord.size( ) == 2 ) {
            res.push_back( { x, y } );
          }
          else if( first_coord.size( ) == 3 ) {
            res.push_back( { x, y, z } );
          }
          if( y == yf ) {
            break;
          }
          if( xd >= 0 ) {
            x += sx;
            xd -= ay;
          }
          if( zd >= 0 ) {
            z += sz;
            zd -= ay;
          }
          y += sy;
          xd += ax;
          zd += az;
        }
      }
      else if( az >= std::max( ax, ay ) ) {
        COMMENT( "Z dominant.", 3 );
        xd = ax - ( az >> 1 );
        yd = ay - ( az >> 1 );
        while( true ) {
          if( first_coord.size( ) == 2 ) {
            res.push_back( { x, y } );
          }
          else if( first_coord.size( ) == 3 ) {
            res.push_back( { x, y, z } );
          }
          if( z == zf ) {
            break;
          }
          if( xd >= 0 ) {
            x += sx;
            xd -= az;
          }
          if( yd >= 0 ) {
            y += sy;
            yd -= az;
          }
          z += sz;
          xd += ax;
          yd += ay;
        }
      }
      return( res );
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
  void Line::Print( O &os ) const {
    try {
      os << "Line" << std::endl;
      os << "Color(s): ";
      for( size_t clr = 0; clr < 4; ++clr ) {
        os << this->color[ clr ] << " ";
      }
      os << std::endl;
      os << "Dimensions: ";
      for( size_t dms = 0; dms < first_coord.size( ); ++dms ) {
        os << first_coord[ dms ] << "-" << last_coord[ dms ] << "; ";
      }
      os << std::endl;
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
  O &operator<<( O &os, const Line &obj ) {
    obj.Print( os );
    return( os );
  }

#ifdef BIAL_EXPLICIT_DrawLine

  template void Line::Draw( Image< int > &img ) const;
  template void Line::Draw( Image< int > &img, const Vector< float > &displacement ) const;
  template void Line::Draw( Image< llint > &img ) const;
  template void Line::Draw( Image< llint > &img, const Vector< float > &displacement ) const;
  template void Line::Draw( Image< float > &img ) const;
  template void Line::Draw( Image< float > &img, const Vector< float > &displacement ) const;
  template void Line::Draw( Image< double > &img ) const;
  template void Line::Draw( Image< double > &img, const Vector< float > &displacement ) const;
  template void Line::Draw( Image< Color > &img ) const;

  template void Line::Print( OFile &os ) const;
  template void Line::Print( std::ostream &os ) const;
  template OFile &operator<<( OFile &os, const Line &obj );
  template std::ostream &operator<<( std::ostream &os, const Line &obj );

#endif

}

#endif

#endif
