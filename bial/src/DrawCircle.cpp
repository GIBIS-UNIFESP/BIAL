/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2014/Mar/14
 * @brief Functions to draw circles in images.
 */

#ifndef BIALDRAWCIRCLE_C
#define BIALDRAWCIRCLE_C

#include "DrawCircle.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_DrawCircle )
#define BIAL_EXPLICIT_DrawCircle
#endif

#if defined ( BIAL_EXPLICIT_DrawCircle ) || ( BIAL_IMPLICIT_BIN )

#include "Color.hpp"
#include "Image.hpp"
#include "RealColor.hpp"

namespace Bial {

  /* Circle ------------------------------------------------------------------------------------------------------------
  **/

  Circle::Circle( const Vector< float > &center, const float radius, const Color color,
                  const bool filled ) try : Figure< Circle >( color ), center( center ), radius( radius ), filled(
      filled ) {
    if( center.size( ) != 2 ) {
      std::string msg( BIAL_ERROR( "Circle center must have 2 dimensions." ) );
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
  void Circle::Draw( Image< D > &img ) const {
    try {
      Draw( img, { 0.0f, 0.0f } );
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


  // template< class D >
  // void Circle::PlotPixels( Image< D > &img, const Vector< float > &displacement, const int dx, const int dy ) const {
  //   try {
  //     float maximum = std::max< float >( 255.0f, img.Maximum( ) );
  //     float alpha = this->color( 0 ) / 255.0f;
  //     float assgn_color = ( this->color( 1 ) + this->color( 2 ) + this->color( 3 ) ) * maximum / ( 3.0f * 255.0f );

  //     COMMENT( "Plotting Pixels (" << dx << ", " << dy << ");", 2 );
  //     const int x_ctr = displacement[ 0 ], y_ctr = displacement[ 1 ];
  //     if( img.ValidCoordinate( x_ctr + dx, y_ctr + dy ) ) {
  //       img( x_ctr + dx, y_ctr + dy ) = alpha * assgn_color + ( 1.0f - alpha ) * img( x_ctr + dx, y_ctr + dy );
  //     }
  //     if( img.ValidCoordinate( x_ctr - dx, y_ctr + dy ) ) {
  //       img( x_ctr - dx, y_ctr + dy ) = alpha * assgn_color + ( 1.0f - alpha ) * img( x_ctr - dx, y_ctr + dy );
  //     }
  //     if( img.ValidCoordinate( x_ctr - dy, y_ctr + dx ) ) {
  //       img( x_ctr - dy, y_ctr + dx ) = alpha * assgn_color + ( 1.0f - alpha ) * img( x_ctr - dy, y_ctr + dx );
  //     }
  //     if( img.ValidCoordinate( x_ctr + dy, y_ctr + dx ) ) {
  //       img( x_ctr + dy, y_ctr + dx ) = alpha * assgn_color + ( 1.0f - alpha ) * img( x_ctr + dy, y_ctr + dx );
  //     }
  //     if( img.ValidCoordinate( x_ctr + dx, y_ctr - dy ) ) {
  //       img( x_ctr + dx, y_ctr - dy ) = alpha * assgn_color + ( 1.0f - alpha ) * img( x_ctr + dx, y_ctr - dy );
  //     }
  //     if( img.ValidCoordinate( x_ctr - dx, y_ctr - dy ) ) {
  //       img( x_ctr - dx, y_ctr - dy ) = alpha * assgn_color + ( 1.0f - alpha ) * img( x_ctr - dx, y_ctr - dy );
  //     }
  //     if( img.ValidCoordinate( x_ctr + dy, y_ctr - dx ) ) {
  //       img( x_ctr + dy, y_ctr - dx ) = alpha * assgn_color + ( 1.0f - alpha ) * img( x_ctr + dy, y_ctr - dx );
  //     }
  //     if( img.ValidCoordinate( x_ctr - dy, y_ctr - dx ) ) {
  //       img( x_ctr - dy, y_ctr - dx ) = alpha * assgn_color + ( 1.0f - alpha ) * img( x_ctr - dy, y_ctr - dx );
  //     }
  //   }
  //   catch( std::bad_alloc &e ) {
  //     std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
  //     throw( std::runtime_error( msg ) );
  //   }
  //   catch( std::runtime_error &e ) {
  //     std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
  //     throw( std::runtime_error( msg ) );
  //   }
  //   catch( const std::out_of_range &e ) {
  //     std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
  //     throw( std::out_of_range( msg ) );
  //   }
  //   catch( const std::logic_error &e ) {
  //     std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
  //     throw( std::logic_error( msg ) );
  //   }
  // }

  // template< >
  // void Circle::PlotPixels( Image< Color > &img, const Vector< float > &displacement, const int dx, const int dy ) 
  //   const {
  //   try {
  //     COMMENT( "Computing color value.", 2 );
  //     RealColor max_color( img.Maximum( ) );
  //     float maximum = std::max( 255.0f, std::max( max_color( 1 ), std::max( max_color( 2 ), max_color( 3 ) ) ) );
  //      COMMENT( "maximum: " << maximum, 0 );
  //     RealColor assgn_color( this->color( 0 ), this->color( 1 ) * maximum / 255.0f, 
  //                            this->color( 2 ) * maximum / 255.0f, this->color( 3 ) * maximum / 255.0f );
  //     float alpha = assgn_color( 0 ) / 255.0f;

  //     COMMENT( "Plotting Pixels (" << dx << ", " << dy << ");", 2 );
  //     const int x_ctr = displacement[ 0 ], y_ctr = displacement[ 1 ];
  //     for( size_t clr = 1; clr < 4; ++clr ) {
  //       for( int delta_x = -dx; delta_x <= dx; delta_x += 2 * dx ) {
  //         for( int delta_y = -dy; delta_y <= dy; delta_y += 2 * dy ) {
  //           if( img.ValidCoordinate( x_ctr + delta_x, y_ctr + delta_y ) ) {
  //             img( x_ctr + delta_x, y_ctr + delta_y )( clr ) = alpha * assgn_color[ clr ] + ( 1.0f - alpha ) * 
  //               img( x_ctr + delta_x, y_ctr + delta_y )( clr );
  //           }
  //           if( img.ValidCoordinate( x_ctr + delta_y, y_ctr + delta_x ) ) {
  //             img( x_ctr + delta_y, y_ctr + delta_x )( clr ) = alpha * assgn_color[ clr ] + ( 1.0f - alpha ) * 
  //               img( x_ctr + delta_y, y_ctr + delta_x )( clr );
  //           }
  //         }
  //       }
  //     }
  //       // if( img.ValidCoordinate( x_ctr - dx, y_ctr + dy ) ) {
  //       //   img( x_ctr - dx, y_ctr + dy )( clr ) = alpha * assgn_color[ clr ] + ( 1.0f - alpha ) *
  //       //     img( x_ctr + dx, y_ctr + dy )( clr );
  //       // }
  //       // if( img.ValidCoordinate( x_ctr - dy, y_ctr + dx ) ) {
  //       //   img( x_ctr - dy, y_ctr + dx )( clr ) = alpha * assgn_color[ clr ] + ( 1.0f - alpha ) *
  //       //     img( x_ctr + dx, y_ctr + dy )( clr );
  //       // }
  //       // if( img.ValidCoordinate( x_ctr + dy, y_ctr + dx ) ) {
  //       //   img( x_ctr + dy, y_ctr + dx )( clr ) = alpha * assgn_color[ clr ] + ( 1.0f - alpha ) *
  //       //     img( x_ctr + dx, y_ctr + dy )( clr );
  //       // }
  //       // if( img.ValidCoordinate( x_ctr + dx, y_ctr - dy ) ) {
  //       //   img( x_ctr + dx, y_ctr - dy )( clr ) = alpha * assgn_color[ clr ] + ( 1.0f - alpha ) *
  //       //     img( x_ctr + dx, y_ctr + dy )( clr );
  //       // }
  //       // if( img.ValidCoordinate( x_ctr - dx, y_ctr - dy ) ) {
  //       //   img( x_ctr - dx, y_ctr - dy )( clr ) = alpha * assgn_color[ clr ] + ( 1.0f - alpha ) *
  //       //     img( x_ctr + dx, y_ctr + dy )( clr );
  //       // }
  //       // if( img.ValidCoordinate( x_ctr + dy, y_ctr - dx ) ) {
  //       //   img( x_ctr + dy, y_ctr - dx )( clr ) = alpha * assgn_color[ clr ] + ( 1.0f - alpha ) *
  //       //     img( x_ctr + dx, y_ctr + dy )( clr );
  //       // }
  //       // if( img.ValidCoordinate( x_ctr - dy, y_ctr - dx ) ) {
  //       //   img( x_ctr - dy, y_ctr - dx )( clr ) = alpha * assgn_color[ clr ] + ( 1.0f - alpha ) *
  //       //     img( x_ctr + dx, y_ctr + dy )( clr );
  //       // }
  //     }
  //   }
  //   catch( std::bad_alloc &e ) {
  //     std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
  //     throw( std::runtime_error( msg ) );
  //   }
  //   catch( std::runtime_error &e ) {
  //     std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
  //     throw( std::runtime_error( msg ) );
  //   }
  //   catch( const std::out_of_range &e ) {
  //     std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
  //     throw( std::out_of_range( msg ) );
  //   }
  //   catch( const std::logic_error &e ) {
  //     std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
  //     throw( std::logic_error( msg ) );
  //   }
  // }

  template< class D >
  void Circle::Draw( Image< D > &img, const Vector< float > &displacement ) const {
    try {
      COMMENT( "Checking if circle is completly out of image domain.", 2 );
      for( size_t dms = 0; dms < center.size( ); ++dms ) {
        if( ( center[ dms ] + displacement[ dms ] - radius >= img.size( dms ) ) ||
            ( center[ dms ] + displacement[ dms ] + radius < 0.0 ) ) {
          return;
        }
        if( radius == 0 ) {
          return;
        }
      }
      COMMENT( "Computing color value.", 2 );
      float maximum = std::max< float >( 255.0f, img.Maximum( ) );
      float alpha = this->color( 0 ) / 255.0f;
      float assgn_color = ( this->color( 1 ) + this->color( 2 ) + this->color( 3 ) ) * maximum / ( 3.0f * 255.0f );

      if( filled ) {
        COMMENT( "Filled circle: ", 1 );
        COMMENT( "Computing x square dimensions.", 1 );
        size_t xmin = center[ 0 ] + displacement[ 0 ] - radius;
        size_t xmax = center[ 0 ] + displacement[ 0 ] + radius;
        float xdisp_init = -radius;
        if( xmin > xmax ) {
          xmin = 0;
          xdisp_init = -( center[ 0 ] + displacement[ 0 ] );
        }
        if( xmax >= img.size( 0 ) ) {
          xmax = img.size( 0 ) - 1;
        }
        COMMENT( "Computing y square dimensions.", 1 );
        size_t ymin = center[ 1 ] + displacement[ 1 ] - radius;
        size_t ymax = center[ 1 ] + displacement[ 1 ] + radius;
        float ydisp_init = -radius;
        if( ymin > ymax ) {
          ymin = 0;
          ydisp_init = -( center[ 1 ] + displacement[ 1 ] );
        }
        if( ymax >= img.size( 1 ) ) {
          ymax = img.size( 1 ) - 1;
        }
        float radius2 = radius * radius;

        COMMENT( "center: " << center << ", displacement: " << displacement << ", min: " << xmin << " " << ymin <<
                 ", max: " << xmax << " " << ymax << ", disp: " << xdisp_init << " " << ydisp_init << ", radius2 : " <<
                 radius2 << ".", 1 );

        COMMENT( "Drawing filled circle.", 1 );
        for( size_t x = xmin, xdisp = xdisp_init; x <= xmax; ++x, ++xdisp ) {
          for( size_t y = ymin, ydisp = ydisp_init; y <= ymax; ++y, ++ydisp ) {
            if( xdisp * xdisp + ydisp * ydisp <= radius2 ) {
              img( x, y ) = alpha * assgn_color + ( 1.0 - alpha ) * img( x, y );
            }
          }
        }
      }
      else {
        COMMENT( "Empty circle: ", 1 );
        int x = 0, y = static_cast< int >( radius ), d = 5 - 4 * y;
        int deltaE = 8 * x + 12, deltaSE = 8 * x - 8 * y + 20;
        do {
          COMMENT( "Plotting pixels.", 3 );
          const int x_ctr = center[ 0 ] + displacement[ 0 ], y_ctr = center[ 1 ] + displacement[ 1 ];
          for( int dx = -1; dx < 2; dx += 2 ) {
            for( int dy = -1; dy < 2; dy += 2 ) {
              int delta_x = x * dx;
              int delta_y = y * dy;
              if( img.ValidCoordinate( x_ctr + delta_x, y_ctr + delta_y ) ) {
                img( x_ctr + delta_x, y_ctr + delta_y ) = alpha * assgn_color + ( 1.0f - alpha ) * 
                  img( x_ctr + delta_x, y_ctr + delta_y );
              }
              if( ( x != y ) && ( img.ValidCoordinate( x_ctr + delta_y, y_ctr + delta_x ) ) ) {
                img( x_ctr + delta_y, y_ctr + delta_x ) = alpha * assgn_color + ( 1.0f - alpha ) * 
                  img( x_ctr + delta_y, y_ctr + delta_x );
              }
              if( y == 0 )
                break;
            }
            if( x == 0 )
              break;
          }
          
          COMMENT( "Updating coordinates.", 3 );
          if( d < 0 ) {
            d += deltaE;
            deltaSE += 8;
          }
          else {
            d += deltaSE;
            deltaSE += 16;
            y--;
          }
          deltaE += 8;
          x++;
          //PlotPixels( img, center + displacement, x, y );
        } while( x < y );
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
  void Circle::Draw( Image< Color > &img, const Vector< float > &displacement ) const {
    try {
      COMMENT( "Checking if circle is completly out of image domain.", 2 );
      for( size_t dms = 0; dms < center.size( ); ++dms ) {
        if( ( center[ dms ] + displacement[ dms ] - radius >= img.size( dms ) ) ||
            ( center[ dms ] + displacement[ dms ] + radius < 0.0 ) ) {
          return;
        }
        if( radius == 0 ) {
          return;
        }
      }

      COMMENT( "Computing color value.", 2 );
      RealColor max_color( img.Maximum( ) );
      float maximum = std::max( 255.0f, std::max( max_color( 1 ), std::max( max_color( 2 ), max_color( 3 ) ) ) );
      COMMENT( "maximum: " << maximum, 0 );
      RealColor assgn_color( this->color( 0 ) / 255.0f, this->color( 1 ) * maximum / 255.0f, 
                             this->color( 2 ) * maximum / 255.0f, this->color( 3 ) * maximum / 255.0f );
      float alpha = assgn_color( 0 );

      if( filled ) {
        COMMENT( "Filled circle: ", 1 );
        COMMENT( "Computing x square dimensions.", 1 );
        size_t xmin = center[ 0 ] + displacement[ 0 ] - radius;
        size_t xmax = center[ 0 ] + displacement[ 0 ] + radius;
        float xdisp_init = -radius;
        if( xmin > xmax ) {
          xmin = 0;
          xdisp_init = -( center[ 0 ] + displacement[ 0 ] );
        }
        if( xmax >= img.size( 0 ) ) {
          xmax = img.size( 0 ) - 1;
        }
        COMMENT( "Computing y square dimensions.", 1 );
        size_t ymin = center[ 1 ] + displacement[ 1 ] - radius;
        size_t ymax = center[ 1 ] + displacement[ 1 ] + radius;
        float ydisp_init = -radius;
        if( ymin > ymax ) {
          ymin = 0;
          ydisp_init = -( center[ 1 ] + displacement[ 1 ] );
        }
        if( ymax >= img.size( 1 ) ) {
          ymax = img.size( 1 ) - 1;
        }
        float radius2 = radius * radius;

        COMMENT( "center: " << center << ", displacement: " << displacement << ", min: " << xmin << " " << ymin <<
                 ", max: " << xmax << " " << ymax << ", disp: " << xdisp_init << " " << ydisp_init << ", radius2 : " <<
                 radius2 << ".", 1 );

        COMMENT( "Drawing filled circle.", 1 );
        for( size_t x = xmin, xdisp = xdisp_init; x <= xmax; ++x, ++xdisp ) {
          for( size_t y = ymin, ydisp = ydisp_init; y <= ymax; ++y, ++ydisp ) {
            if( xdisp * xdisp + ydisp * ydisp <= radius2 ) {
              for( size_t clr = 1; clr < 4; ++clr ) {
                img( x, y )( clr ) = alpha * assgn_color[ clr ] + ( 1.0f - alpha ) * img( x, y )( clr );
              }
            }
          }
        }
      }
      else {
        COMMENT( "Empty circle: ", 1 );
        int x = 0, y = static_cast< int >( radius ), d = 5 - 4 * y;
        int deltaE = 8 * x + 12, deltaSE = 8 * x - 8 * y + 20;
        do {

          COMMENT( "Plotting pixels.", 3 );
          const int x_ctr = center[ 0 ] + displacement[ 0 ], y_ctr = center[ 1 ] + displacement[ 1 ];
          for( int dx = -1; dx < 2; dx += 2 ) {
            int delta_x = x * dx;
            for( int dy = -1; dy < 2; dy += 2 ) {
              int delta_y = y * dy;
              if( img.ValidCoordinate( x_ctr + delta_x, y_ctr + delta_y ) ) {
                for( size_t clr = 1; clr < 4; ++clr ) {
                  img( x_ctr + delta_x, y_ctr + delta_y )( clr ) = alpha * assgn_color[ clr ] + ( 1.0f - alpha ) * 
                    img( x_ctr + delta_x, y_ctr + delta_y )( clr );
                }
              }
              if( ( x != y ) && ( img.ValidCoordinate( x_ctr + delta_y, y_ctr + delta_x ) ) ) {
                for( size_t clr = 1; clr < 4; ++clr ) {
                  img( x_ctr + delta_y, y_ctr + delta_x )( clr ) = alpha * assgn_color[ clr ] + ( 1.0f - alpha ) * 
                    img( x_ctr + delta_y, y_ctr + delta_x )( clr );
                }
              }
              if( y == 0 )
                break;
            }
            if( x == 0 )
              break;
          }

          COMMENT( "Updating coordinates.", 3 );
          if( d < 0 ) {
            d += deltaE;
            deltaSE += 8;
          }
          else {
            d += deltaSE;
            deltaSE += 16;
            y--;
          }
          deltaE += 8;
          x++;
          //PlotPixels( img, center + displacement, x, y, assgn_color, alpha );
        } while( x <= y );
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

  Vector< Vector< int > > Circle::Pixels( ) const {
    return( Pixels( Vector< float >( center.size( ), 0.0f ) ) );
  }

  Vector< Vector< int > > Circle::Pixels( const Vector< float > &displacement ) const {
    try {

      Vector< Vector< int > > res;

      COMMENT( "Checking if circle is empty.", 2 );
      if( radius == 0 ) {
        return( res );
      }
      if( filled ) {
        COMMENT( "Computing x square dimensions.", 0 );
        int xmin = center[ 0 ] + displacement[ 0 ] - radius;
        int xmax = center[ 0 ] + displacement[ 0 ] + radius;
        float xdisp_init = -radius;

        COMMENT( "Computing y square dimensions.", 0 );
        int ymin = center[ 1 ] + displacement[ 1 ] - radius;
        int ymax = center[ 1 ] + displacement[ 1 ] + radius;
        float ydisp_init = -radius;

        float radius2 = radius * radius;
        COMMENT( "center: " << center << ", displacement: " << displacement << ", min: " << xmin << " " << ymin <<
                 ", max: " << xmax << " " << ymax << ", disp: " << xdisp_init << " " << ydisp_init << ", radius2 : " <<
                 radius2 << ".", 0 );

        COMMENT( "Drawing filled circle.", 0 );
        for( int x = xmin, xdisp = xdisp_init; x <= xmax; ++x, ++xdisp ) {
          for( int y = ymin, ydisp = ydisp_init; y <= ymax; ++y, ++ydisp ) {
            if( xdisp * xdisp + ydisp * ydisp <= radius2 ) {
              res.push_back( { x, y } );
            }
          }
        }
      }
      else {
        int x = 0, y = static_cast< int >( radius ), d = 5 - 4 * y;
        int deltaE = 8 * x + 12, deltaSE = 8 * x - 8 * y + 20;

        int dx = x, dy = y;
        COMMENT( "Plotting Pixels (" << dx << ", " << dy << ");", 2 );
        const int x_ctr = center[ 0 ] + displacement[ 0 ];
        const int y_ctr = center[ 1 ] + displacement[ 1 ];
        res.push_back( { x_ctr + dx, y_ctr + dy } );
        res.push_back( { x_ctr - dx, y_ctr + dy } );
        res.push_back( { x_ctr - dy, y_ctr + dx } );
        res.push_back( { x_ctr + dy, y_ctr + dx } );
        res.push_back( { x_ctr + dx, y_ctr - dy } );
        res.push_back( { x_ctr - dx, y_ctr - dy } );
        res.push_back( { x_ctr + dy, y_ctr - dx } );
        res.push_back( { x_ctr - dy, y_ctr - dx } );
        while( x < y ) {
          if( d < 0 ) {
            d += deltaE;
            deltaSE += 8;
          }
          else {
            d += deltaSE;
            deltaSE += 16;
            y--;
          }
          deltaE += 8;
          x++;

          COMMENT( "Setting pixels.", 4 );
          dx = x;
          dy = y;
          res.push_back( { x_ctr + dx, y_ctr + dy } );
          res.push_back( { x_ctr - dx, y_ctr + dy } );
          res.push_back( { x_ctr - dy, y_ctr + dx } );
          res.push_back( { x_ctr + dy, y_ctr + dx } );
          res.push_back( { x_ctr + dx, y_ctr - dy } );
          res.push_back( { x_ctr - dx, y_ctr - dy } );
          res.push_back( { x_ctr + dy, y_ctr - dx } );
          res.push_back( { x_ctr - dy, y_ctr - dx } );
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
  void Circle::Print( O &os ) const {
    try {
      if( filled ) {
        os << "Filled ";
      }
      else {
        os << "Empty ";
      }
      os << "circle" << std::endl;
      os << "Color(s): " << this->color << std::endl;
      os << "Radius: " << radius << std::endl;
      os << "Center: ";
      for( size_t dms = 0; dms < this->center.size( ); ++dms ) {
        os << center[ dms ] << " ";
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
  O &operator<<( O &os, const Circle &obj ) {
    obj.Print( os );
    return( os );
  }

#ifdef BIAL_EXPLICIT_DrawCircle

  template void Circle::Draw( Image< int > &img ) const;
  template void Circle::Draw( Image< int > &img, const Vector< float > &displacement ) const;
  template void Circle::Draw( Image< llint > &img ) const;
  template void Circle::Draw( Image< llint > &img, const Vector< float > &displacement ) const;
  template void Circle::Draw( Image< float > &img ) const;
  template void Circle::Draw( Image< float > &img, const Vector< float > &displacement ) const;
  template void Circle::Draw( Image< double > &img ) const;
  template void Circle::Draw( Image< double > &img, const Vector< float > &displacement ) const;
  template void Circle::Draw( Image< Color > &img ) const;

  template void Circle::Print( OFile &os ) const;
  template void Circle::Print( std::ostream &os ) const;
  template OFile &operator<<( OFile &os, const Circle &obj );
  template std::ostream &operator<<( std::ostream &os, const Circle &obj );

#endif

}

#endif

#endif
