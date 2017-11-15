/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2014/Jan/07
 * @brief B-Spline implementation.
 */

#ifndef BIALBSPLINE_C
#define BIALBSPLINE_C

#include "BSpline.hpp"
#include "Matrix.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_BSpline )
#define BIAL_EXPLICIT_BSpline
#endif

#if defined ( BIAL_EXPLICIT_BSpline ) || ( BIAL_IMPLICIT_BIN )

namespace Bial {

  /* Spline ---------------------------------------------------------------------------------------------------- **/

  Vector< size_t > Curve::BSplineKnot( size_t vertices, size_t order ) {
    try {
      COMMENT( "Computing knots dimensions.", 1 );
      size_t knots = vertices + order;

      COMMENT( "Creating knots vector.", 1 );
      Vector< size_t > knot( knots, 0 );

      COMMENT( "Assigning knots values.", 1 );
      for( size_t elm = 1; elm < knots; ++elm ) {
        if( ( elm > order ) && ( elm < vertices ) ) {
          knot( elm ) = knot( elm - 1 ) + 1;
        }
        else {
          knot( elm ) = knot( elm - 1 );
        }
      }
      return( knot );
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

  Vector< float > Curve::BSplineBasis( const Vector< size_t > &knot, size_t order, float parameter,
                                            size_t vertices ) {
    try {
      Vector< float > partial( knot.size( ), 0.0 );
      COMMENT( "Computing first order basis.", 3 );
      for( size_t knt = 0; knt < knot.size( ) - 1; ++knt ) {
        if( ( parameter >= knot( knt ) ) && ( parameter < knot( knt + 1 ) ) ) {
          partial( knt ) = 1.0f;
        }
        else {
          partial( knt ) = 0.0f;
        }
      }
      COMMENT( "Computing higher order basis.", 3 );
      for( size_t ord = 1; ord < order; ++ord ) {
        for( size_t knt = 0; knt < knot.size( ) - ord; ++knt ) {
          float low_recursion = 0.0f;
          if( partial( knt ) != 0 ) {
            low_recursion = ( ( parameter - knot( knt ) ) * partial( knt ) ) / ( knot( knt + ord ) - knot( knt ) );
          }
          float high_recursion = 0.0f;
          if( partial( knt + 1 ) != 0 ) {
            high_recursion = ( ( knot( knt + ord + 1 ) - parameter ) *
                               partial( knt + 1 ) ) / ( knot( knt + ord + 1 ) - knot( knt + 1 ) );
          }
          partial( knt ) = low_recursion + high_recursion;
        }
      }
      COMMENT( "Getting the last basis.", 3 );
      if( parameter - static_cast< float >( knot( knot.size( ) - 1 ) ) < 5e-6 ) {
        partial( vertices - 1 ) = 1.0f;
      }
      COMMENT( "Assigning to resultant vector.", 3 );
      Vector< float > res( vertices, 0.0 );
      for( size_t elm = 0; elm < vertices; ++elm ) {
        res( elm ) = partial( elm );
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

  Matrix< float > Curve::BSpline( const Matrix< float > &vertex, const Vector< size_t > &points, size_t order ) {
    try {
      COMMENT( "Checking for errors in the number of dimensions.", 1 );
      if( vertex.Dims( ) != points.size( ) ) {
        std::string msg( BIAL_ERROR( "Incompatible points and vertices dimensions. Given: vertex dim: " +
                                     std::to_string( vertex.Dims( ) ) + ", points size: " +
                                     std::to_string( points.size( ) ) ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Checking for errors in number of vertices and order.", 1 );
      if( vertex.size( 0 ) + order % 2 == 0 ) {
        std::string msg( BIAL_ERROR( "Number of vertices plus order must be an odd number. Given: " +
                                     std::to_string( vertex.size( 0 ) + order ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      if( vertex.size( 0 ) < order + 1 ) {
        std::string msg( BIAL_ERROR( "Number of vertices must be greater than the order plus one. Given: vertices: " +
                                     std::to_string( vertex.size( 0 ) ) + ", order: " + std::to_string( order ) + "."
                                     ) );
        throw( std::logic_error( msg ) );
      }
      if( vertex.Dims( ) == 1 ) {
        COMMENT( "1D spline.", 0 );
        COMMENT( "Creating knots sizes.", 0 );
        size_t knots( vertex.size( 0 ) + order );
        COMMENT( "Creating knots.", 0 );
        Vector< size_t > knot_x( Curve::BSplineKnot( vertex.size( 0 ), order ) );
        COMMENT( "Computing the steps of parameters.", 0 );
        float step_x = static_cast< float >( knot_x( knots - 1 ) ) / static_cast< float >( points( 0 ) - 1 );
        COMMENT( "Creating resultant matrix.", 0 );
        Matrix< float > res( points );
        res.Set( 0.0f );
        COMMENT( "Computing the B-spline points.", 0 );
        float parameter_x = 0.0f;
        for( size_t pnt_x = 0; pnt_x < points( 0 ); ++pnt_x, parameter_x += step_x ) {
          if( static_cast< float >( knot_x( knots - 1 ) ) - parameter_x < 5e-6 ) {
            COMMENT( "Setting parameter to the last knot for the last iteration.", 4 );
            parameter_x = static_cast< float >( knot_x( knots - 1 ) );
          }
          Vector< float > basis_x = Curve::BSplineBasis( knot_x, order, parameter_x, vertex.size( 0 ) );
          for( size_t vtx_x = 0; vtx_x < vertex.size( 0 ); ++vtx_x ) {
            if( basis_x( vtx_x ) != 0.0f ) {
              res( pnt_x ) += vertex( vtx_x ) * basis_x( vtx_x );
            }
          }
        }
        return( res );
      }
      if( vertex.Dims( ) == 2 ) {
        COMMENT( "2D spline.", 0 );
        COMMENT( "Creating knots sizes.", 0 );
        Vector< size_t > knots = { vertex.size( 0 ) + order, vertex.size( 1 ) + order };
        COMMENT( "Creating knots.", 0 );
        Vector< size_t > knot_x( Curve::BSplineKnot( vertex.size( 0 ), order ) );
        Vector< size_t > knot_y( Curve::BSplineKnot( vertex.size( 1 ), order ) );
        COMMENT( "Computing the steps of parameters.", 0 );
        float step_x = static_cast< float >( knot_x( knots( 0 ) - 1 ) ) / static_cast< float >( points( 0 ) - 1 );
        float step_y = static_cast< float >( knot_y( knots( 1 ) - 1 ) ) / static_cast< float >( points( 1 ) - 1 );
        COMMENT( "Creating resultant matrix.", 0 );
        Matrix< float > res( points );
        res.Set( 0.0f );
        COMMENT( "Computing the B-spline points.", 0 );
        float parameter_x = 0.0f;
        for( size_t pnt_x = 0; pnt_x < points( 0 ); ++pnt_x, parameter_x += step_x ) {
          if( static_cast< float >( knot_x( knots( 0 ) - 1 ) ) - parameter_x < 5e-6 ) {
            parameter_x = static_cast< float >( knot_x( knots( 0 ) - 1 ) );
          }
          Vector< float > basis_x = Curve::BSplineBasis( knot_x, order, parameter_x, vertex.size( 0 ) );
          float parameter_y = 0.0f;
          for( size_t pnt_y = 0; pnt_y < points( 1 ); ++pnt_y, parameter_y += step_y ) {
            if( static_cast< float >( knot_y( knots( 1 ) - 1 ) ) - parameter_y < 5e-6 ) {
              parameter_y = static_cast< float >( knot_y( knots( 1 ) - 1 ) );
            }
            Vector< float > basis_y = Curve::BSplineBasis( knot_y, order, parameter_y, vertex.size( 1 ) );
            for( size_t vtx_x = 0; vtx_x < vertex.size( 0 ); ++vtx_x ) {
              if( basis_x( vtx_x ) != 0.0f ) {
                for( size_t vtx_y = 0; vtx_y < vertex.size( 1 ); ++vtx_y ) {
                  if( basis_y( vtx_y ) != 0.0f ) {
                    float basis_val = basis_x( vtx_x ) * basis_y( vtx_y );
                    res( pnt_x, pnt_y ) += vertex( vtx_x, vtx_y ) * basis_val;
                  }
                }
              }
            }
          }
        }
        return( res );
      }
      if( vertex.Dims( ) == 3 ) {
        COMMENT( "3D spline.", 0 );
        COMMENT( "Creating knots sizes.", 0 );
        Vector< size_t > knots = { vertex.size( 0 ) + order, vertex.size( 1 ) + order, vertex.size( 2 ) + order };
        COMMENT( "Creating knots.", 0 );
        Vector< size_t > knot_x( Curve::BSplineKnot( vertex.size( 0 ), order ) );
        Vector< size_t > knot_y( Curve::BSplineKnot( vertex.size( 1 ), order ) );
        Vector< size_t > knot_z( Curve::BSplineKnot( vertex.size( 2 ), order ) );
        COMMENT( "knot_x: " << knot_x << ", knot_y: " << knot_y << ", knot_z: " << knot_z, 0 );
        COMMENT( "Computing the steps of parameters.", 0 );
        float step_x = static_cast< float >( knot_x( knots( 0 ) - 1 ) ) / static_cast< float >( points( 0 ) );
        float step_y = static_cast< float >( knot_y( knots( 1 ) - 1 ) ) / static_cast< float >( points( 1 ) );
        float step_z = static_cast< float >( knot_z( knots( 2 ) - 1 ) ) / static_cast< float >( points( 2 ) );
        COMMENT( "step_x: " << step_x << ", step_y: " << step_y << ", step_z: " << step_z, 0 );
        COMMENT( "Creating resultant matrix.", 0 );
        Matrix< float > res( points );
        res.Set( 0.0f );
        COMMENT( "Computing the B-spline points.", 0 );
        float parameter_x = 0.0f;
        for( size_t pnt_x = 0; pnt_x < points( 0 ); ++pnt_x, parameter_x += step_x ) {
          if( static_cast< float >( knot_x( knots( 0 ) - 1 ) ) - parameter_x < 5e-6 ) {
            parameter_x = static_cast< float >( knot_x( knots( 0 ) - 1 ) );
          }
          Vector< float > basis_x = Curve::BSplineBasis( knot_x, order, parameter_x, vertex.size( 0 ) );
          COMMENT( "basis_x: " << basis_x, 4 );
          float parameter_y = 0.0f;
          for( size_t pnt_y = 0; pnt_y < points( 1 ); ++pnt_y, parameter_y += step_y ) {
            if( static_cast< float >( knot_y( knots( 1 ) - 1 ) ) - parameter_y < 5e-6 ) {
              parameter_y = static_cast< float >( knot_y( knots( 1 ) - 1 ) );
            }
            Vector< float > basis_y = Curve::BSplineBasis( knot_y, order, parameter_y, vertex.size( 1 ) );
            float parameter_z = 0.0f;
            for( size_t pnt_z = 0; pnt_z < points( 2 ); ++pnt_z, parameter_z += step_z ) {
              if( static_cast< float >( knot_z( knots( 2 ) - 1 ) ) - parameter_z < 5e-6 ) {
                parameter_z = static_cast< float >( knot_z( knots( 2 ) - 1 ) );
              }
              Vector< float > basis_z = Curve::BSplineBasis( knot_z, order, parameter_z, vertex.size( 2 ) );
              for( size_t vtx_x = 0; vtx_x < vertex.size( 0 ); ++vtx_x ) {
                if( basis_x( vtx_x ) != 0.0f ) {
                  for( size_t vtx_y = 0; vtx_y < vertex.size( 1 ); ++vtx_y ) {
                    if( basis_y( vtx_y ) != 0.0f ) {
                      for( size_t vtx_z = 0; vtx_z < vertex.size( 2 ); ++vtx_z ) {
                        if( basis_z( vtx_z ) != 0.0f ) {
                          float basis_val = basis_x( vtx_x ) * basis_y( vtx_y ) * basis_z( vtx_z );
                          res( pnt_x, pnt_y, pnt_z ) += vertex( vtx_x, vtx_y, vtx_z ) * basis_val;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
        return( res );
      }
      std::string msg( BIAL_ERROR( "Input matrix must be 1D, 2D, or 3D." ) );
      throw( std::logic_error( msg ) );
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
