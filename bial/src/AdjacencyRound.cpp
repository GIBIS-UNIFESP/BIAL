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
      Adjacency result( size, 2 );
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
      Adjacency result( size, 2 );
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
            if( ( dx * dx ) + ( dy * dy ) + ( dz * dz ) <= static_cast< int >( max_radius_2 ) )
              ++size;
          }
        }
      }
      COMMENT( "Initializing the adjacency relation. Put central element in the first position.", 1 );
      Adjacency result( size, 3 );
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
      IF_DEBUG( ( dims > 3 ) || ( dims < 2 ) ) {
        std::string msg( BIAL_ERROR( "Adjacency relation must have 2 or 3 dimensions." ) );
        throw( std::logic_error( msg ) );
      }
      if( dims == 2 )
        return( AdjacencyType::Circular( radius ) );
      return( AdjacencyType::Spheric( radius ) );
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
                  static_cast< float >( dz * dz ) / z_max_radius_2 ) <= 1.0 )
              ++size;
          }
        }
      }
      COMMENT( "Initializing the adjacency relation. Put central element in the first position.", 1 );
      Adjacency result( size, 3 );
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
      IF_DEBUG( ( radius.size( ) > 3 ) || ( radius.size( ) < 2 ) ) {
        std::string msg( BIAL_ERROR( "Adjacency relation must have 2 or 3 dimensions." ) );
        throw( std::logic_error( msg ) );
      }
      IF_DEBUG( radius.size( ) == 2 )
        return( AdjacencyType::Elliptic( radius[ 0 ], radius[ 1 ] ) );
      return( AdjacencyType::Ellipsoid( radius[ 0 ], radius[ 1 ], radius[ 2 ] ) );
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

  Adjacency AdjacencyType::Circunference( float radius ) {
    try {
      COMMENT( "Computing adjacency size.", 1 );
      size_t max_radius = static_cast< size_t >( radius );
      size_t max_radius_2 = static_cast< size_t >( radius * radius );
      size_t size = 0;
      for( int dy = -max_radius; dy <= static_cast< int >( max_radius ); ++dy ) {
        for( int dx = -max_radius; dx <= static_cast< int >( max_radius ); ++dx ) {
          if( ( ( dx * dx ) + ( dy * dy ) ) <= static_cast< int >( max_radius_2 ) ) {
            if( ( ( dx < 0 ) && ( ( ( dx - 1 ) * ( dx - 1 ) ) + ( dy * dy ) ) > static_cast< int >( max_radius_2 ) ) ||
                ( ( dy < 0 ) && ( ( dx * dx ) + ( ( dy - 1 ) * ( dy - 1 ) ) ) > static_cast< int >( max_radius_2 ) ) ||
                ( ( dx > 0 ) && ( ( ( dx + 1 ) * ( dx + 1 ) ) + ( dy * dy ) ) > static_cast< int >( max_radius_2 ) ) ||
                ( ( dy > 0 ) && ( ( dx * dx ) + ( ( dy + 1 ) * ( dy + 1 ) ) ) > static_cast< int >( max_radius_2 ) ) )
            ++size;
          }
        }
      }
      COMMENT( "Initializing the adjacency relation. Put central element in the first position.", 1 );
      Adjacency result( size, 2 );
      size_t i = 0;
      for( int dy = -max_radius; dy <= static_cast< int >( max_radius ); ++dy ) {
        for( int dx = -max_radius; dx <= static_cast< int >( max_radius ); ++dx ) {
          if( ( ( dx != 0 ) || ( dy != 0 ) ) &&
              ( ( ( dx * dx ) + ( dy * dy ) ) <= static_cast< int >( max_radius_2 ) ) ) {
            if( ( ( dx < 0 ) && ( ( ( dx - 1 ) * ( dx - 1 ) ) + ( dy * dy ) ) > static_cast< int >( max_radius_2 ) ) ||
                ( ( dy < 0 ) && ( ( dx * dx ) + ( ( dy - 1 ) * ( dy - 1 ) ) ) > static_cast< int >( max_radius_2 ) ) ||
                ( ( dx > 0 ) && ( ( ( dx + 1 ) * ( dx + 1 ) ) + ( dy * dy ) ) > static_cast< int >( max_radius_2 ) ) ||
                ( ( dy > 0 ) && ( ( dx * dx ) + ( ( dy + 1 ) * ( dy + 1 ) ) ) > static_cast< int >( max_radius_2 ) ) ) {
              result( i, 0 ) = dx;
              result( i, 1 ) = dy;
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


#ifdef BIAL_EXPLICIT_AdjacencyRound

  /* Explicit instantiations. --------------------------------------------------------------------------------------- */

#endif

}

#endif

#endif
