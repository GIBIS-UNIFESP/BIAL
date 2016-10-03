/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Jul/05
 * @brief Adjacency relation with round format.
 * <br> Future add-on's: none.
 */

#ifndef BIALADJACENCYROUND_C
#define BIALADJACENCYROUND_C

#include "AdjacencyRound.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_AdjacencyRound )
#define BIAL_EXPLICIT_AdjacencyRound
#endif

#if defined ( BIAL_EXPLICIT_AdjacencyRound ) || ( BIAL_IMPLICIT_BIN )

namespace Bial {

  Adjacency AdjacencyType::Circular( float radius ) {
    try {
      COMMENT( "Computing adjacency size.", 1 );
      size_t max_radius = static_cast< size_t >( radius );
      size_t max_radius_2 = static_cast< size_t >( radius * radius );
      size_t size = 0;
      for( int dy = -max_radius; dy <= static_cast< int >( max_radius ); ++dy ) {
        for( int dx = -max_radius; dx <= static_cast< int >( max_radius ); ++dx ) {
          if( ( ( dx * dx ) + ( dy * dy ) ) <= static_cast< int >( max_radius_2 ) ) {
            ++size;
          }
        }
      }
      COMMENT( "Initializing the adjacency relation. Put central element in the first position.", 1 );
      Adjacency result( 2, size );
      result( 0, 0 ) = 0.0f;
      result( 0, 1 ) = 0.0f;
      size_t i = 1;
      for( int dy = -max_radius; dy <= static_cast< int >( max_radius ); ++dy ) {
        for( int dx = -max_radius; dx <= static_cast< int >( max_radius ); ++dx ) {
          if( ( ( dx != 0 ) || ( dy != 0 ) ) &&
              ( ( ( dx * dx ) + ( dy * dy ) ) <= static_cast< int >( max_radius_2 ) ) ) {
            result( i, 0 ) = dx;
            result( i, 1 ) = dy;
            ++i;
          }
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

  Adjacency AdjacencyType::Elliptic( float x_radius, float y_radius ) {
    try {
      COMMENT( "Computing adjacency size.", 1 );
      size_t x_max_radius = static_cast< size_t >( x_radius );
      size_t y_max_radius = static_cast< size_t >( y_radius );
      size_t x_max_radius_2 = x_radius * x_radius;
      size_t y_max_radius_2 = y_radius * y_radius;
      size_t size = 0;
      for( int dy = -y_max_radius; dy <= static_cast< int >( y_max_radius ); ++dy ) {
        for( int dx = -x_max_radius; dx <= static_cast< int >( x_max_radius ); ++dx ) {
          if( static_cast< float >( dx * dx ) / x_max_radius_2 +
              static_cast< float >( dy * dy ) / y_max_radius_2 <= 1.0 ) {
            ++size;
          }
        }
      }
      COMMENT( "Initializing the adjacency relation. Put central element in the first position.", 1 );
      Adjacency result( 2, size );
      result( 0, 0 ) = 0.0f;
      result( 0, 1 ) = 0.0f;
      size_t i = 1;
      for( int dy = -y_max_radius; dy <= static_cast< int >( y_max_radius ); ++dy ) {
        for( int dx = -x_max_radius; dx <= static_cast< int >( x_max_radius ); ++dx ) {
          if( ( ( dx != 0 ) || ( dy != 0 ) ) &&
              ( static_cast< float >( dx * dx ) / x_max_radius_2 +
                static_cast< float >( dy * dy ) / y_max_radius_2 <= 1.0 ) ) {
            result( i, 0 ) = dx;
            result( i, 1 ) = dy;
            ++i;
          }
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

  Adjacency AdjacencyType::Spheric( float radius ) {
    try {
      COMMENT( "Computing adjacency size.", 1 );
      size_t max_radius = static_cast< size_t >( radius );
      size_t max_radius_2 = static_cast< size_t >( radius * radius );
      size_t size = 0;
      for( int dz = -max_radius; dz <= static_cast< int >( max_radius ); ++dz ) {
        for( int dy = -max_radius; dy <= static_cast< int >( max_radius ); ++dy ) {
          for( int dx = -max_radius; dx <= static_cast< int >( max_radius ); ++dx ) {
            if( ( dx * dx ) + ( dy * dy ) + ( dz * dz ) <= static_cast< int >( max_radius_2 ) ) {
              ++size;
            }
          }
        }
      }
      COMMENT( "Initializing the adjacency relation. Put central element in the first position.", 1 );
      Adjacency result( 3, size );
      result( 0, 0 ) = 0.0f;
      result( 0, 1 ) = 0.0f;
      result( 0, 2 ) = 0.0f;
      size_t i = 1;
      for( int dz = -max_radius; dz <= static_cast< int >( max_radius ); ++dz ) {
        for( int dy = -max_radius; dy <= static_cast< int >( max_radius ); ++dy ) {
          for( int dx = -max_radius; dx <= static_cast< int >( max_radius ); ++dx ) {
            if( ( ( dx != 0 ) || ( dy != 0 ) || ( dz != 0 ) ) &&
                ( ( ( dx * dx ) + ( dy * dy ) + ( dz * dz ) ) <= static_cast< int >( max_radius_2 ) ) ) {
              result( i, 0 ) = dx;
              result( i, 1 ) = dy;
              result( i, 2 ) = dz;
              ++i;
            }
          }
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

  Adjacency AdjacencyType::HyperSpheric( float radius, size_t dims ) {
    try {
      COMMENT( "Computing adjacency boundary box size.", 3 );
      size_t max_radius = static_cast< size_t >( radius );
      size_t max_radius_2 = static_cast< size_t >( radius * radius );
      size_t square_size = pow( 2 * max_radius + 1, dims );
      size_t size = 0;
      COMMENT( "Computing the number of adjacents.", 3 );
      Vector< int > pxl_coord( dims, -max_radius );
      for( size_t elm = 0; elm < square_size; ++elm ) {
        COMMENT( "Verifying if pixel is inside sphere.", 4 );
        size_t radius_2 = 0;
        for( size_t dms = 0; dms < dims; ++dms ) {
          radius_2 += static_cast< size_t >( pxl_coord( dms ) * pxl_coord( dms ) );
        }
        if( radius_2 <= max_radius_2 ) {
          ++size;
        }
        COMMENT( "Updating coordinates.", 4 );
        for( size_t dms = 0; dms < dims; ++dms ) {
          if( pxl_coord( dms ) < static_cast< int >( max_radius ) ) {
            ++pxl_coord( dms );
            break;
          }
          else {
            pxl_coord( dms ) = -max_radius;
          }
        }
      }
      COMMENT( "Initializing the adjacency relation. Put central element in the first position.", 3 );
      Adjacency result( dims, size );
      for( size_t dms = 0; dms < dims; ++dms ) {
        result( 0, dms ) = 0.0f;
      }
      for( size_t dms = 0; dms < dims; ++dms ) {
        pxl_coord( dms ) = -max_radius;
      }
      size_t adj_index = 1;
      for( size_t elm = 0; elm < square_size; ++elm ) {
        COMMENT( "Assigning adjacency coordinates.", 4 );
        COMMENT( "elm: " << elm << ", size: " << square_size, 3 );
        size_t radius_2 = 0;
        for( size_t dms = 0; dms < dims; ++dms ) {
          radius_2 += static_cast< size_t >( pxl_coord( dms ) * pxl_coord( dms ) );
        }
        if( ( radius_2 > 0 ) && ( radius_2 <= max_radius_2 ) ) {
          for( size_t dms = 0; dms < dims; ++dms ) {
            result( adj_index, dms ) = pxl_coord( dms );
          }
          ++adj_index;
        }
        COMMENT( "Updating coordinates.", 3 );
        for( int dms = static_cast< int >( dims ) - 1; dms >= 0; --dms ) {
          if( pxl_coord( dms ) < static_cast< int >( max_radius ) ) {
            ++pxl_coord( dms );
            break;
          }
          else
            pxl_coord( dms ) = -max_radius;
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

  Adjacency AdjacencyType::Ellipsoid( float x_radius, float y_radius, float z_radius ) {
    try {
      COMMENT( "Computing adjacency size.", 1 );
      size_t x_max_radius = static_cast< size_t >( x_radius );
      size_t y_max_radius = static_cast< size_t >( y_radius );
      size_t z_max_radius = static_cast< size_t >( z_radius );
      float x_max_radius_2 = x_radius * x_radius;
      float y_max_radius_2 = y_radius * y_radius;
      float z_max_radius_2 = z_radius * z_radius;
      size_t size = 0;
      for( int dz = -z_max_radius; dz <= static_cast< int >( z_max_radius ); ++dz ) {
        for( int dy = -y_max_radius; dy <= static_cast< int >( y_max_radius ); ++dy ) {
          for( int dx = -x_max_radius; dx <= static_cast< int >( x_max_radius ); ++dx ) {
            if( ( static_cast< float >( dx * dx ) / x_max_radius_2 + static_cast< float >( dy * dy ) / y_max_radius_2 +
                  static_cast< float >( dz * dz ) / z_max_radius_2 ) <= 1.0 ) {
              ++size;
            }
          }
        }
      }
      COMMENT( "Initializing the adjacency relation. Put central element in the first position.", 1 );
      Adjacency result( 3, size );
      result( 0, 0 ) = 0.0f;
      result( 0, 1 ) = 0.0f;
      result( 0, 2 ) = 0.0f;
      size_t i = 1;
      for( int dz = -z_max_radius; dz <= static_cast< int >( z_max_radius ); ++dz ) {
        for( int dy = -y_max_radius; dy <= static_cast< int >( y_max_radius ); ++dy ) {
          for( int dx = -x_max_radius; dx <= static_cast< int >( x_max_radius ); ++dx ) {
            if( ( ( dx != 0 ) || ( dy != 0 ) || ( dz != 0 ) ) &&
                ( static_cast< float >( dx * dx ) / x_max_radius_2 +
                  static_cast< float >( dy * dy ) / y_max_radius_2 +
                  static_cast< float >( dz * dz ) / z_max_radius_2 <= 1.0 ) ) {
              result( i, 0 ) = dx;
              result( i, 1 ) = dy;
              result( i, 2 ) = dz;
              ++i;
            }
          }
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

  Adjacency AdjacencyType::HyperEllipsoid( const Vector< float > &radius ) {
    try {
      COMMENT( "Computing adjacency boundary box size.", 3 );
      size_t dims = radius.size( );
      Vector< size_t > max_radius( dims );
      size_t box_size = 1;
      for( size_t dms = 0; dms < dims; ++dms ) {
        max_radius( dms ) = static_cast< size_t >( radius( dms ) );
        box_size *= 2 * max_radius( dms ) + 1;
      }
      COMMENT( "Computing the square of the radius to speedup processing.", 3 );
      Vector< double > max_radius_2( dims );
      for( size_t dms = 0; dms < dims; ++dms ) {
        max_radius_2( dms ) = radius( dms ) * radius( dms );
      }
      COMMENT( "Computing the number of adjacents.", 3 );
      size_t size = 0;
      Vector< int > pxl_coord( dims );
      for( size_t dms = 0; dms < dims; ++dms ) {
        pxl_coord( dms ) = -max_radius( dms );
      }
      for( size_t pxl = 0; pxl < box_size; ++pxl ) {
        COMMENT( "Verifying if pixel is inside the ellipsoid.", 3 );
        double radius_2 = 0;
        for( size_t dms = 0; dms < dims; ++dms ) {
          radius_2 += static_cast< double >( pxl_coord( dms ) * pxl_coord( dms ) ) / max_radius_2( dms );
        }
        if( radius_2 <= 1.0 ) {
          ++size;
        }
        COMMENT( "Updating coordinates.", 3 );
        for( size_t dms = 0; dms < dims; ++dms ) {
          if( pxl_coord( dms ) < static_cast< int >( max_radius( dms ) ) ) {
            ++pxl_coord( dms );
            break;
          }
          else {
            pxl_coord( dms ) = -max_radius( dms );
          }
        }
      }
      COMMENT( "Initializing the adjacency relation. Put central element in the first position.", 1 );
      Adjacency result( dims, size );
      for( size_t dms = 0; dms < dims; ++dms ) {
        result( 0, dms ) = 0.0f;
      }
      for( size_t dms = 0; dms < dims; ++dms ) {
        pxl_coord( dms ) = -max_radius( dms );
      }
      size_t adj_index = 1;
      for( size_t pxl = 0; pxl < box_size; ++pxl ) {
        COMMENT( "Assigning adjacency coordinates.", 3 );
        float radius_2 = 0;
        for( size_t dms = 0; dms < dims; ++dms ) {
          radius_2 += static_cast< double >( pxl_coord( dms ) * pxl_coord( dms ) ) / max_radius_2( dms );
        }
        if( ( radius_2 > 0.0 ) && ( radius_2 <= 1.0 ) ) {
          for( size_t dms = 0; dms < dims; ++dms ) {
            result( adj_index, dms ) = pxl_coord( dms );
          }
          ++adj_index;
        }
        COMMENT( "Updating coordinates.", 3 );
        for( int dms = static_cast< int >( dims ) - 1; dms >= 0; dms-- ) {
          if( pxl_coord( dms ) < static_cast< int >( max_radius( dms ) ) ) {
            ++pxl_coord( dms );
            break;
          }
          else {
            pxl_coord( dms ) = -max_radius( dms );
          }
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


#ifdef BIAL_EXPLICIT_AdjacencyRound

  /* Explicit instantiations. --------------------------------------------------------------------------------------- */

#endif

}

#endif

#endif
