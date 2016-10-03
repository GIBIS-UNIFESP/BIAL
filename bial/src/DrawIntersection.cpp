/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2014/Mar/14
 * @brief Functions to find intersection between forms.
 */

#ifndef BIALDRAWINTERSECTION_C
#define BIALDRAWINTERSECTION_C

#include "DrawIntersection.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_DrawIntersection )
#define BIAL_EXPLICIT_DrawIntersection
#endif

#if defined ( BIAL_EXPLICIT_DrawIntersection ) || ( BIAL_IMPLICIT_BIN )

#include "AdjacencyRound.hpp"
#include "DrawLine.hpp"
#include "DrawSequence.hpp"

namespace Bial {

  Vector< float > Intersection( const Sequence &sequence, const Line &line ) {
    try {
      COMMENT( "Getting line pixels.", 2 );
      const Vector< Vector< int > > line_point( line.Pixels( ) );

      COMMENT( "Searching for intersection.", 2 );
      const Adjacency adj = AdjacencyType::Circular( 1.45 );
      for( size_t adj_idx = 0; adj_idx < adj.size( ); ++adj_idx ) {
        COMMENT( "Running through all adjacency.", 4 );
        for( size_t crv_pos = 0; crv_pos < sequence.point.size( ); ++crv_pos ) {
          COMMENT( "Running through all specified circunference sequence pixels.", 4 );
          COMMENT( "Getting the adjacent x and y coordinates.", 4 );
          int adj_x = std::round( ( sequence.point[ crv_pos ][ 0 ] ) + adj( adj_idx, 0 ) );
          int adj_y = std::round( ( sequence.point[ crv_pos ][ 1 ] ) + adj( adj_idx, 1 ) );
          for( size_t line_pos = 0; line_pos < line_point.size( ); ++line_pos ) {
            COMMENT( "Running through all line points.", 4 );
            if( ( line_point[ line_pos ][ 0 ] == adj_x ) && ( line_point[ line_pos ][ 1 ] == adj_y ) ) {
              COMMENT( "Found cross point. Returning pixel with position " << adj_x << ", " << adj_y << ".", 4 );
              return( Vector< float >( { static_cast< float >( line_point[ line_pos ][ 0 ] ),
                                         static_cast< float >( line_point[ line_pos ][ 1 ] ) } ) );
            }
          }
        }
      }
      COMMENT( "Returning empty vector.", 2 );
      return( Vector< float >( ) );
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

  Vector< float > Intersection( const Line &line1, const Line &line2 ) {
    try {
      float x1 = line1.first_coord[ 0 ], x2 = line1.last_coord[ 0 ];
      float x3 = line2.first_coord[ 0 ], x4 = line2.last_coord[ 0 ];
      float y1 = line1.first_coord[ 1 ], y2 = line1.last_coord[ 1 ];
      float y3 = line2.first_coord[ 1 ], y4 = line2.last_coord[ 1 ];
      const float det = ( ( x1 - x2 ) * ( y3 - y4 ) ) - ( ( y1 - y2 ) * ( x3 - x4 ) );
      const float s = ( ( ( x1 - x2 ) * ( y2 - y4 ) ) - ( ( y1 - y2 ) * ( x2 - x4 ) ) ) / det;
      const float t = ( ( ( x3 - x4 ) * ( y2 - y4 ) ) - ( ( y3 - y4 ) * ( x2 - x4 ) ) ) / det;
      COMMENT( "det: " << det << ", s: " << s << ", t: " << t << ".", 1 );
      if( ( det == 0.0f ) || ( s < 0.0f ) || ( s > 1.0f ) || ( t < 0.0f ) || ( t > 1.0f ) ) {
        return( Vector< float >( ) );
      }
      Vector< float > Coord( 2 );
      Coord[ 0 ] = x4 + ( x3 - x4 ) * s;
      Coord[ 1 ] = y4 + ( y3 - y4 ) * s;
      return( Coord );
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

#endif

#endif
