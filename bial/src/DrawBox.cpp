/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2014/Mar/14
 * @brief Functions to draw boxes in images.
 */

#ifndef BIALDRAWBOX_C
#define BIALDRAWBOX_C

#include "DrawBox.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_DrawBox )
#define BIAL_EXPLICIT_DrawBox
#endif

#if defined ( BIAL_EXPLICIT_DrawBox ) || ( BIAL_IMPLICIT_BIN )

#include "Color.hpp"
#include "Image.hpp"
#include "RealColor.hpp"

namespace Bial {

  /* Box ----------------------------------------------------------------------------------------------------------- **/

  Box::Box( const Vector< float > &low_coord, const Vector< float > &size, const Color color, const bool filled ) try 
    : Figure< Box >( color ), low_coord( low_coord ), size( size ), filled( filled ) {

      COMMENT( "Checking if low coordinate dimensions matches size dimensions.", 2 );
      if( low_coord.size( ) != size.size( ) ) {
        std::string msg( BIAL_ERROR( "Incompatible number of dimensions between low coordinate and size vectors." ) );
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

  template< class D >
  void Box::Draw( Image< D > &img ) const {
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
  void Box::Draw( Image< D > &img, const Vector< float > &displacement )
  const {
    try {
      COMMENT( "Verify if image dimensions match low coordinate dimensions.", 2 );
      if( img.Dims( ) != low_coord.size( ) ) {
        std::string msg( BIAL_ERROR( "Incompatible number of dimensions of image. Given: " + 
                                     std::to_string( img.Dims( ) ) + ", expected: " + 
                                     std::to_string( low_coord.size( ) ) + ", given: " + 
                                     std::to_string( img.Dims( ) ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      size_t dimensions = img.Dims( );
      COMMENT( "Verify if displacement dimensions match low coordinate dimensions.", 2 );
      if( displacement.size( ) != low_coord.size( ) ) {
        std::string msg( BIAL_ERROR(
                           "Incompatible number of dimensions between displacement and lower dimension pixel." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Checking if box is completly out of image domain with the displacement or if its size is zero.", 2 );
      for( size_t dms = 0; dms < dimensions; ++dms ) {
        if( ( low_coord( dms ) + displacement( dms ) >= img.size( dms ) ) ||
            ( low_coord( dms ) + size( dms ) - 1.0 + displacement( dms ) < 0.0 ) ) {
          return;
        }
        if( size( dms ) == 0 ) {
          return;
        }
      }
      COMMENT( "Computing drawing coordinates and size of the box.", 2 );
      Vector< size_t > init_coord( 3, 0 );
      Vector< size_t > last_coord( 3, 0 );
      size_t total_elms = 1;
      for( size_t dms = 0; dms < dimensions; ++dms ) {
        COMMENT( "Checking for box boundaries inside the image domain.", 2 );
        if( low_coord( dms ) + displacement( dms ) <= 0.0 ) {
          init_coord( dms ) = 0;
        }
        else {
          init_coord( dms ) = low_coord( dms ) + displacement( dms );
        }
        if( low_coord( dms ) + size( dms ) + displacement( dms ) >= img.size( dms ) ) {
          last_coord( dms ) = img.size( dms ) - 1;
        }
        else {
          last_coord( dms ) = low_coord( dms ) + displacement( dms ) + size( dms ) - 0.5;
        }
        COMMENT( "Updating the number of elements in image domain.", 2 );
        total_elms *= ( last_coord( dms ) - init_coord( dms ) + 1 );
      }
      COMMENT( "Computing image dims: ", 2 );
      Vector< size_t > dims( img.Dim( ) );
      COMMENT( "Computing color value.", 2 );
      float maximum = std::max< float >( 255.0f, img.Maximum( ) );
      float alpha = this->color( 0 ) / 255.0f;
      float assgn_color = ( this->color( 1 ) + this->color( 2 ) + this->color( 3 ) ) * maximum / 3.0f ;
      if( filled ) {
        COMMENT( "Drawing filled box from: (" << init_coord << ") to (" << last_coord << ").", 2 );
        for( size_t z = init_coord( 2 ); z <= last_coord( 2 ); ++z ) {
          for( size_t y = init_coord( 1 ); y <= last_coord( 1 ); ++y ) {
            for( size_t x = init_coord( 0 ); x <= last_coord( 0 ); ++x ) {
              size_t pxl = x + y * dims[ 0 ] + z * dims[ 1 ];
              img( pxl ) = alpha * assgn_color + ( 1.0f - alpha ) * img( pxl );
            }
          }
        }
      }
      else {
        COMMENT( "Drawing empty box from: (" << init_coord << ") to (" << last_coord << ").", 2 );
        if( img.Dims( ) == 3 ) {
          COMMENT( "Filling z surfaces.", 2 );
          for( size_t y = init_coord( 1 ); y <= last_coord( 1 ); ++y ) {
            for( size_t x = init_coord( 0 ); x <= last_coord( 0 ); ++x ) {
              size_t pxl = x + y * dims[ 0 ] + init_coord( 2 ) * dims[ 1 ];
              img( pxl ) = alpha * assgn_color + ( 1.0f - alpha ) * img( pxl );
            }
          }
          if( init_coord( 2 ) != last_coord( 2 ) ) {
            for( size_t y = init_coord( 1 ); y <= last_coord( 1 ); ++y ) {
              for( size_t x = init_coord( 0 ); x <= last_coord( 0 ); ++x ) {
                size_t pxl = x + y * dims[ 0 ] + last_coord( 2 ) * dims[ 1 ];
                img( pxl ) = alpha * assgn_color + ( 1.0f - alpha ) * img( pxl );
              }
            }
          }
        }
        COMMENT( "Filling y surfaces.", 2 );
        for( size_t z = init_coord( 2 ); z <= last_coord( 2 ); ++z ) {
          for( size_t x = init_coord( 0 ); x <= last_coord( 0 ); ++x ) {
            size_t pxl = x + init_coord( 1 ) * dims[ 0 ] + z * dims[ 1 ];
            img( pxl ) = alpha * assgn_color + ( 1.0f - alpha ) * img( pxl );
          }
        }
        if( init_coord( 1 ) != last_coord( 1 ) ) {
          for( size_t z = init_coord( 2 ); z <= last_coord( 2 ); ++z ) {
            for( size_t x = init_coord( 0 ); x <= last_coord( 0 ); ++x ) {
              size_t pxl = x + last_coord( 1 ) * dims[ 0 ] + z * dims[ 1 ];
              img( pxl ) = alpha * assgn_color + ( 1.0f - alpha ) * img( pxl );
            }
          }
        }
        COMMENT( "Filling x surfaces.", 2 );
        for( size_t z = init_coord( 2 ); z <= last_coord( 2 ); ++z ) {
          for( size_t y = init_coord( 1 ); y <= last_coord( 1 ); ++y ) {
            size_t pxl = init_coord( 0 ) + y * dims[ 0 ] + z * dims[ 1 ];
            img( pxl ) = alpha * assgn_color + ( 1.0f - alpha ) * img( pxl );
          }
        }
        if( init_coord( 0 ) != last_coord( 0 ) ) {
          for( size_t z = init_coord( 2 ); z <= last_coord( 2 ); ++z ) {
            for( size_t y = init_coord( 1 ); y <= last_coord( 1 ); ++y ) {
              size_t pxl = last_coord( 0 ) + y * dims[ 0 ] + z * dims[ 1 ];
              img( pxl ) = alpha * assgn_color + ( 1.0f - alpha ) * img( pxl );
            }
          }
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
  void Box::Draw( Image< Color > &img, const Vector< float > &displacement )
  const {
    try {
      COMMENT( "Verify if image dimensions match low coordinate dimensions.", 2 );
      size_t img_dims = img.Dims( );
      if( img_dims != low_coord.size( ) ) {
        std::string msg( BIAL_ERROR( "Incompatible number of dimensions of image. Given: " + 
                                     std::to_string( img_dims ) + ", expected: " + 
                                     std::to_string( low_coord.size( ) ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Verify if displacement dimensions match low coordinate dimensions.", 2 );
      if( displacement.size( ) != low_coord.size( ) ) {
        std::string msg( BIAL_ERROR(
                           "Incompatible number of dimensions between displacement and lower dimension pixel." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Checking if box is completly out of image domain with the displacement or if its size is zero.", 2 );
      for( size_t dms = 0; dms < img_dims; ++dms ) {
        if( ( low_coord( dms ) + displacement( dms ) >= img.size( dms ) ) ||
            ( low_coord( dms ) + size( dms ) - 1.0 + displacement( dms ) < 0.0 ) ) {
          return;
        }
        if( size( dms ) == 0 ) {
          return;
        }
      }
      COMMENT( "Computing drawing coordinates and size of the box.", 2 );
      Vector< size_t > init_coord( 3, 0 );
      Vector< size_t > last_coord( 3, 0 );
      size_t total_elms = 1;
      for( size_t dms = 0; dms < img_dims; ++dms ) {

        COMMENT( "Checking for box boundaries inside the image domain.", 2 );
        if( low_coord( dms ) + displacement( dms ) <= 0.0 ) {
          init_coord( dms ) = 0;
        }
        else {
          init_coord( dms ) = low_coord( dms ) + displacement( dms );
        }
        if( low_coord( dms ) + size( dms ) + displacement( dms ) >= img.size( dms ) ) {
          last_coord( dms ) = img.size( dms ) - 1;
        }
        else {
          last_coord( dms ) = low_coord( dms ) + displacement( dms ) + size( dms ) - 0.5;
        }
        COMMENT( "Updating the number of elements in image domain.", 2 );
        total_elms *= ( last_coord( dms ) - init_coord( dms ) + 1 );
      }

      COMMENT( "Computing color value.", 2 );
      RealColor max_color( img.Maximum( ) );
      float maximum = std::max( 255.0f, std::max( max_color( 1 ), std::max( max_color( 2 ), max_color( 3 ) ) ) );
      COMMENT( "maximum: " << maximum, 0 );
      RealColor assgn_color( this->color( 0 ), this->color( 1 ) * maximum, 
                             this->color( 2 ) * maximum, this->color( 3 ) * maximum );
      float alpha = assgn_color( 0 ) / 255.0f;

      if( filled ) {
        if( img_dims == 2 ) {
          COMMENT( "Drawing filled box from: (" << init_coord << ") to (" << last_coord << ").", 2 );
          for( size_t y = init_coord( 1 ); y <= last_coord( 1 ); ++y ) {
            for( size_t x = init_coord( 0 ); x <= last_coord( 0 ); ++x ) {
              for( size_t clr = 1; clr < 4; ++clr ) {
                img( x, y )( clr ) = alpha * assgn_color( clr ) + ( 1.0f - alpha ) * img( x, y )( clr );
              }
            }
          }
        }
        else {
          COMMENT( "Drawing filled box from: (" << init_coord << ") to (" << last_coord << ").", 2 );
          for( size_t z = init_coord( 2 ); z <= last_coord( 2 ); ++z ) {
            for( size_t y = init_coord( 1 ); y <= last_coord( 1 ); ++y ) {
              for( size_t x = init_coord( 0 ); x <= last_coord( 0 ); ++x ) {
                for( size_t clr = 1; clr < 4; ++clr ) {
                  img( x, y, z )( clr ) = alpha * assgn_color( clr ) + ( 1.0f - alpha ) * img( x, y, z )( clr );
                }
              }
            }
          }
        }
      }
      else {
        COMMENT( "Drawing empty box from: (" << init_coord << ") to (" << last_coord << ").", 2 );
        if( img_dims == 2 ) {
          COMMENT( "Filling y surfaces.", 2 );
          for( size_t x = init_coord( 0 ); x <= last_coord( 0 ); ++x ) {
            for( size_t clr = 1; clr < 4; ++clr ) {
              img( x, init_coord( 1 ) )( clr ) = alpha * assgn_color( clr ) + ( 1.0f - alpha ) * 
                img( x, init_coord( 1 ) )( clr );
            }
          }
          if( init_coord( 1 ) != last_coord( 1 ) ) {
            for( size_t x = init_coord( 0 ); x <= last_coord( 0 ); ++x ) {
              for( size_t clr = 1; clr < 4; ++clr ) {
                img( x, last_coord( 1 ) )( clr ) = alpha * assgn_color( clr ) + ( 1.0f - alpha ) * 
                  img( x, last_coord( 1 ) )( clr );
              }
            }
          }
          COMMENT( "Filling x surfaces.", 2 );
          for( size_t y = init_coord( 1 ); y <= last_coord( 1 ); ++y ) {
            for( size_t clr = 1; clr < 4; ++clr ) {
              img( init_coord( 0 ), y )( clr ) = alpha * assgn_color( clr ) + ( 1.0f - alpha ) * 
                img( init_coord( 0 ), y )( clr );
            }
          }
          if( init_coord( 0 ) != last_coord( 0 ) ) {
            for( size_t y = init_coord( 1 ); y <= last_coord( 1 ); ++y ) {
              for( size_t clr = 1; clr < 4; ++clr ) {
                img( last_coord( 0 ), y )( clr ) = alpha * assgn_color( clr ) + ( 1.0f - alpha ) * 
                  img( last_coord( 0 ), y )( clr );
              }
            }
          }
        }

        else {
          COMMENT( "Filling z surfaces.", 2 );
          for( size_t y = init_coord( 1 ); y <= last_coord( 1 ); ++y ) {
            for( size_t x = init_coord( 0 ); x <= last_coord( 0 ); ++x ) {
              for( size_t clr = 1; clr < 4; ++clr ) {
                img( x, y, init_coord( 2 ) )( clr ) = alpha * assgn_color( clr ) + ( 1.0f - alpha ) * 
                  img( x, y, init_coord( 2 ) )( clr );
              }
            }
          }
          if( init_coord( 2 ) != last_coord( 2 ) ) {
            for( size_t y = init_coord( 1 ); y <= last_coord( 1 ); ++y ) {
              for( size_t x = init_coord( 0 ); x <= last_coord( 0 ); ++x ) {
                for( size_t clr = 1; clr < 4; ++clr ) {
                  img( x, y, last_coord( 2 ) )( clr ) = alpha * assgn_color( clr ) + ( 1.0f - alpha ) * 
                    img( x, y, last_coord( 2 ) )( clr );
                }
              }
            }
          }
          COMMENT( "Filling y surfaces.", 2 );
          for( size_t z = init_coord( 2 ); z <= last_coord( 2 ); ++z ) {
            for( size_t x = init_coord( 0 ); x <= last_coord( 0 ); ++x ) {
              for( size_t clr = 1; clr < 4; ++clr ) {
                img( x, init_coord( 1 ), z )( clr ) = alpha * assgn_color( clr ) + ( 1.0f - alpha ) * 
                  img( x, init_coord( 1 ), z )( clr );
              }
            }
          }
          if( init_coord( 1 ) != last_coord( 1 ) ) {
            for( size_t z = init_coord( 2 ); z <= last_coord( 2 ); ++z ) {
              for( size_t x = init_coord( 0 ); x <= last_coord( 0 ); ++x ) {
                for( size_t clr = 1; clr < 4; ++clr ) {
                  img( x, last_coord( 1 ), z )( clr ) = alpha * assgn_color( clr ) + ( 1.0f - alpha ) * 
                    img( x, last_coord( 1 ), z )( clr );
                }
              }
            }
          }
          COMMENT( "Filling x surfaces.", 2 );
          for( size_t z = init_coord( 2 ); z <= last_coord( 2 ); ++z ) {
            for( size_t y = init_coord( 1 ); y <= last_coord( 1 ); ++y ) {
              for( size_t clr = 1; clr < 4; ++clr ) {
                img( init_coord( 0 ), y, z )( clr ) = alpha * assgn_color( clr ) + ( 1.0f - alpha ) * 
                  img( init_coord( 0 ), y, z )( clr );
              }
            }
          }
          if( init_coord( 0 ) != last_coord( 0 ) ) {
            for( size_t z = init_coord( 2 ); z <= last_coord( 2 ); ++z ) {
              for( size_t y = init_coord( 1 ); y <= last_coord( 1 ); ++y ) {
                for( size_t clr = 1; clr < 4; ++clr ) {
                  img( last_coord( 0 ), y, z )( clr ) = alpha * assgn_color( clr ) + ( 1.0f - alpha ) * 
                    img( last_coord( 0 ), y, z )( clr );
                }
              }
            }
          }
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

  Vector< Vector< int > > Box::Pixels( ) const {
    return( Pixels( Vector< float >( low_coord.size( ), 0.0f ) ) );
  }

  Vector< Vector< int > > Box::Pixels( const Vector< float > &displacement ) const {
    try {
      COMMENT( "Verify if displacement dimensions match low coordinate dimensions.", 2 );
      if( displacement.size( ) != low_coord.size( ) ) {
        std::string msg( BIAL_ERROR(
                           "Incompatible number of dimensions between displacement and lower dimension pixel." ) );
        throw( std::logic_error( msg ) );
      }
      Vector< Vector< int > > res;

      COMMENT( "Computing drawing coordinates and size of the box.", 2 );
      Vector< int > init_coord( low_coord.size( ), 0 );
      Vector< int > last_coord( low_coord.size( ), 0 );
      for( size_t dms = 0; dms < low_coord.size( ); ++dms ) {
        COMMENT( "Setting box boundaries.", 2 );
        init_coord( dms ) = low_coord( dms ) + displacement( dms );
        last_coord( dms ) = low_coord( dms ) + displacement( dms ) + size( dms ) - 0.5;
      }
      if( filled ) {
        COMMENT( "Printing filled box from: (" << init_coord << ") to (" << last_coord << ").", 2 );
        if( low_coord.size( ) == 3 ) {
          for( int z = init_coord( 2 ); z <= last_coord( 2 ); ++z ) {
            for( int y = init_coord( 1 ); y <= last_coord( 1 ); ++y ) {
              for( int x = init_coord( 0 ); x <= last_coord( 0 ); ++x ) {
                res.push_back( { x, y, z } );
              }
            }
          }
        }
        else if( low_coord.size( ) == 2 ) {
          for( int y = init_coord( 1 ); y <= last_coord( 1 ); ++y ) {
            for( int x = init_coord( 0 ); x <= last_coord( 0 ); ++x ) {
              res.push_back( { x, y } );
            }
          }
        }
      }
      else {
        COMMENT( "Drawing empty box from: (" << init_coord << ") to (" << last_coord << ").", 2 );
        if( low_coord.size( ) == 3 ) {
          COMMENT( "Filling z surfaces.", 2 );
          for( int y = init_coord( 1 ); y <= last_coord( 1 ); ++y ) {
            for( int x = init_coord( 0 ); x <= last_coord( 0 ); ++x ) {
              res.push_back( { x, y, init_coord[ 2 ] } );
            }
          }
          if( init_coord( 2 ) != last_coord( 2 ) ) {
            for( int y = init_coord( 1 ); y <= last_coord( 1 ); ++y ) {
              for( int x = init_coord( 0 ); x <= last_coord( 0 ); ++x ) {
                res.push_back( { x, y, last_coord[ 2 ] } );
              }
            }
          }
          COMMENT( "Filling y surfaces.", 2 );
          for( int z = init_coord( 2 ); z <= last_coord( 2 ); ++z ) {
            for( int x = init_coord( 0 ); x <= last_coord( 0 ); ++x ) {
              res.push_back( { x, init_coord[ 1 ], z } );
            }
          }
          if( init_coord( 1 ) != last_coord( 1 ) ) {
            for( int z = init_coord( 2 ); z <= last_coord( 2 ); ++z ) {
              for( int x = init_coord( 0 ); x <= last_coord( 0 ); ++x ) {
                res.push_back( { x, last_coord[ 1 ], z } );
              }
            }
          }
          COMMENT( "Filling x surfaces.", 2 );
          for( int z = init_coord( 2 ); z <= last_coord( 2 ); ++z ) {
            for( int y = init_coord( 1 ); y <= last_coord( 1 ); ++y ) {
              res.push_back( { init_coord[ 0 ], y, z } );
            }
          }
          if( init_coord( 0 ) != last_coord( 0 ) ) {
            for( int z = init_coord( 2 ); z <= last_coord( 2 ); ++z ) {
              for( int y = init_coord( 1 ); y <= last_coord( 1 ); ++y ) {
                res.push_back( { last_coord[ 0 ], y, z } );
              }
            }
          }
        }
        else if( low_coord.size( ) == 2 ) {
          COMMENT( "Filling x sides.", 2 );
          for( int y = init_coord( 1 ); y <= last_coord( 1 ); ++y ) {
            res.push_back( { init_coord( 0 ), y } );
          }
          if( init_coord( 0 ) != last_coord( 0 ) ) {
            for( int y = init_coord( 1 ); y <= last_coord( 1 ); ++y ) {
              res.push_back( { last_coord( 0 ), y } );
            }
          }
          COMMENT( "Filling x sides.", 2 );
          for( int x = init_coord( 0 ); x <= last_coord( 0 ); ++x ) {
            res.push_back( { x, init_coord( 1 ) } );
          }
          if( init_coord( 1 ) != last_coord( 1 ) ) {
            for( int x = init_coord( 0 ); x <= last_coord( 0 ); ++x ) {
              res.push_back( { x, last_coord( 1 ) } );
            }
          }
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
  void Box::Print( O &os ) const {
    try {
      if( filled ) {
        os << "Filled ";
      }
      else {
        os << "Empty ";
      }
      os << "box: with " << size.size( ) << " dimensions." << std::endl;

      os << "Color(s): " << this->color << std::endl;

      os << "Dimensions: ";
      for( size_t dms = 0; dms < size.size( ); ++dms ) {
        os << low_coord( dms ) << "-" << low_coord( dms ) + size( dms ) - 1 << "; ";
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
  O &operator<<( O &os, const Box &obj ) {
    obj.Print( os );
    return( os );
  }

#ifdef BIAL_EXPLICIT_DrawBox

  template void Box::Draw( Image< int > &img ) const;
  template void Box::Draw( Image< int > &img, const Vector< float > &displacement ) const;
  template void Box::Draw( Image< llint > &img ) const;
  template void Box::Draw( Image< llint > &img, const Vector< float > &displacement ) const;
  template void Box::Draw( Image< float > &img ) const;
  template void Box::Draw( Image< float > &img, const Vector< float > &displacement ) const;
  template void Box::Draw( Image< double > &img ) const;
  template void Box::Draw( Image< double > &img, const Vector< float > &displacement ) const;
  template void Box::Draw( Image< Color > &img ) const;

  template void Box::Print( OFile &os ) const;
  template void Box::Print( std::ostream &os ) const;
  template OFile &operator<<( OFile &os, const Box &obj );
  template std::ostream &operator<<( std::ostream &os, const Box &obj );

#endif

}

#endif

#endif
