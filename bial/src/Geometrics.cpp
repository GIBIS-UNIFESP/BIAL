/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2014/Jan/07
 * @brief Geometric transforms for 3D visualization and printing.
 */

#ifndef BIALGEOMETRICS_C
#define BIALGEOMETRICS_C

#include "Geometrics.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_Geometrics )
#define BIAL_EXPLICIT_Geometrics
#endif

#if defined ( BIAL_EXPLICIT_Geometrics ) || ( BIAL_IMPLICIT_BIN )

#include "DrawShape.hpp"
#include "MatrixDeterminant.hpp"
#include "MatrixIdentity.hpp"
#include "MatrixInverse.hpp"
#include "MatrixTranspose.hpp"

namespace Bial {

  Vector3D::Vector3D( ) : x( 0.0 ), y( 0.0 ), z( 0.0 ) {
  }

  Vector3D::Vector3D( double xx, double yy, double zz ) : x( xx ), y( yy ), z( zz ) {
    if( HasNaNs( ) ) {
      throw std::runtime_error( BIAL_ERROR( "Should not create Vector3D with NANs." ) );
    }
  }

  Vector3D::Vector3D( const Normal &norm ) : x( norm.x ), y( norm.y ), z( norm.z ) {
    if( HasNaNs( ) ) {
      throw std::runtime_error( BIAL_ERROR( "Should not create Vector3D with NANs." ) );
    }
  }

  bool Vector3D::HasNaNs( ) const {
    return( std::isnan( x ) || std::isnan( y ) || std::isnan( z ) );
  }

  bool Vector3D::operator==( const Vector3D &vec ) const {
    return( x == vec.x && y == vec.y && z == vec.z );
  }

  bool Vector3D::operator!=( const Vector3D &vec ) const {
    return( x != vec.x || y != vec.y || z != vec.z );
  }

  Vector3D Vector3D::operator+( const Vector3D &vec ) const {
    return( Vector3D( x + vec.x, y + vec.y, z + vec.z ) );
  }

  Vector3D &Vector3D::operator+=( const Vector3D &vec ) {
    x += vec.x;
    y += vec.y;
    z += vec.z;
    return( *this );
  }

  Vector3D Vector3D::operator-( const Vector3D &vec ) const {
    return( Vector3D( x - vec.x, y - vec.y, z - vec.z ) );
  }

  Vector3D &Vector3D::operator-=( const Vector3D &vec ) {
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;
    return( *this );
  }

  Vector3D Vector3D::operator*( double s ) const {
    return( Vector3D( s * x, s * y, s * z ) );
  }

  Vector3D &Vector3D::operator*=( double s ) {
    x *= s;
    y *= s;
    z *= s;
    return( *this );
  }

  Vector3D Vector3D::operator/( double s ) const {
    if( s == 0 ) {
      throw std::runtime_error( BIAL_ERROR( "Should not divide by zero." ) );
    }
    const double inv = 1.0 / s;
    return( Vector3D( inv * x, inv * y, inv * z ) );
  }

  Vector3D &Vector3D::operator/=( double s ) {
    if( s == 0 ) {
      throw std::runtime_error( BIAL_ERROR( "Should not divide by zero." ) );
    }
    const double inv = 1.0 / s;
    x *= inv;
    y *= inv;
    z *= inv;
    return( *this );
  }

  Vector3D Vector3D::operator-( ) const {
    return( Vector3D( -x, -y, -z ) );
  }

  double Vector3D::operator[]( int i ) const {
    if( ( i < 0 ) || ( i > 2 ) ) {
      throw std::runtime_error( BIAL_ERROR( "Index out of range." ) );
    }
    return( ( &x )[ i ] );
  }

  double &Vector3D::operator[]( int i ) {
    if( ( i < 0 ) || ( i > 2 ) ) {
      throw std::runtime_error( BIAL_ERROR( "Index out of range." ) );
    }
    return( ( &x )[ i ] );
  }

  double Vector3D::LengthSquared( ) const {
    return( x * x + y * y + z * z );
  }

  double Vector3D::Length( ) const {
    return( std::sqrt( LengthSquared( ) ) );
  }

  Vector3D Vector3D::Normalized( ) const {
    try {
      return( ( *this ) / Length( ) );
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

  Point3D Vector3D::toPoint( ) const {
    return( Point3D( x, y, z ) );
  }

  Point3D::Point3D( ) : x( 0.0 ), y( 0.0 ), z( 0.0 ) {
  }

  Point3D::Point3D( double xx, double yy, double zz ) : x( xx ), y( yy ), z( zz ) {
    if( HasNaNs( ) ) {
      throw std::runtime_error( BIAL_ERROR( "Should not create Point3D with NANs." ) );
    }
  }

  bool Point3D::HasNaNs( ) const {
    return( std::isnan( x ) || std::isnan( y ) || std::isnan( z ) );
  }

  bool Point3D::operator==( const Point3D &p ) const {
    return( x == p.x && y == p.y && z == p.z );
  }

  bool Point3D::operator!=( const Point3D &p ) const {
    return( x != p.x || y != p.y || z != p.z );
  }

  Point3D Point3D::operator+( const Vector3D &vec ) const {
    return( Point3D( x + vec.x, y + vec.y, z + vec.z ) );
  }

  Point3D &Point3D::operator+=( const Vector3D &vec ) {
    x += vec.x;
    y += vec.y;
    z += vec.z;
    return( *this );
  }

  Vector3D Point3D::operator-( const Point3D &p ) const {
    return( Vector3D( x - p.x, y - p.y, z - p.z ) );
  }

  Vector3D Point3D::operator-( const Vector3D &vec ) const {
    return( Vector3D( x - vec.x, y - vec.y, z - vec.z ) );
  }

  Point3D &Point3D::operator-=( const Vector3D &vec ) {
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;
    return( *this );
  }

  Point3D &Point3D::operator+=( const Point3D &p ) {
    x += p.x;
    y += p.y;
    z += p.z;
    return( *this );
  }

  Point3D Point3D::operator+( const Point3D &p ) const {
    return( Point3D( x + p.x, y + p.y, z + p.z ) );
  }

  Point3D Point3D::operator*( double f ) const {
    return( Point3D( f * x, f * y, f * z ) );
  }

  Point3D &Point3D::operator*=( double f ) {
    x *= f;
    y *= f;
    z *= f;
    return( *this );
  }

  Point3D Point3D::operator/( double f ) const {
    double inv = 1 / f;
    return( Point3D( x * inv, y * inv, z * inv ) );
  }

  Point3D &Point3D::operator/=( double f ) {
    double inv = 1 / f;
    x *= inv;
    y *= inv;
    z *= inv;
    return( *this );
  }

  double &Point3D::operator[]( int i ) {
    if( ( i < 0 ) || ( i > 2 ) ) {
      throw std::runtime_error( BIAL_ERROR( "Index out of range." ) );
    }
    return( ( &x )[ i ] );
  }

  Vector3D Point3D::toVector( ) const {
    return( Vector3D( x, y, z ) );
  }

  double Point3D::operator[]( int i ) const {
    if( ( i < 0 ) || ( i > 2 ) ) {
      throw std::runtime_error( BIAL_ERROR( "Index out of range." ) );
    }
    return( ( &x )[ i ] );
  }

  Normal::Normal( ) : x( 0.0 ), y( 0.0 ), z( 0.0 ) {
  }

  Normal::Normal( double xx, double yy, double zz ) : x( xx ), y( yy ), z( zz ) {
    if( HasNaNs( ) ) {
      throw std::runtime_error( BIAL_ERROR( "Should not create Vector3D with NANs." ) );
    }
  }

  Normal::Normal( const Vector3D &v ) : x( v.x ), y( v.y ), z( v.z ) {
  }

  bool Normal::HasNaNs( ) const {
    return( std::isnan( x ) || std::isnan( y ) || std::isnan( z ) );
  }

  bool Normal::operator==( const Normal &vec ) const {
    return( x == vec.x && y == vec.y && z == vec.z );
  }

  bool Normal::operator!=( const Normal &vec ) const {
    return( x != vec.x || y != vec.y || z != vec.z );
  }

  Normal Normal::operator+( const Normal &vec ) const {
    return( Normal( x + vec.x, y + vec.y, z + vec.z ) );
  }

  Normal &Normal::operator+=( const Normal &vec ) {
    x += vec.x;
    y += vec.y;
    z += vec.z;
    return( *this );
  }

  Normal Normal::operator-( const Normal &vec ) const {
    return( Normal( x - vec.x, y - vec.y, z - vec.z ) );
  }

  Normal &Normal::operator-=( const Normal &vec ) {
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;
    return( *this );
  }

  Normal Normal::operator*( double s ) const {
    return( Normal( s * x, s * y, s * z ) );
  }

  Normal &Normal::operator*=( double s ) {
    x *= s;
    y *= s;
    z *= s;
    return( *this );
  }

  Normal Normal::operator/( double s ) const {
    if( s == 0 ) {
      throw std::runtime_error( BIAL_ERROR( "Should not divide by zero." ) );
    }
    const double inv = 1.0 / s;
    return( Normal( inv * x, inv * y, inv * z ) );
  }

  Normal &Normal::operator/=( double s ) {
    if( s == 0 ) {
      throw std::runtime_error( BIAL_ERROR( "Should not divide by zero." ) );
    }
    const double inv = 1.0 / s;
    x *= inv;
    y *= inv;
    z *= inv;
    return( *this );
  }

  Normal Normal::operator-( ) const {
    return( Normal( -x, -y, -z ) );
  }

  double Normal::operator[]( int i ) const {
    if( ( i < 0 ) || ( i > 2 ) ) {
      throw std::runtime_error( BIAL_ERROR( "Index out of range." ) );
    }
    return( ( &x )[ i ] );
  }

  double &Normal::operator[]( int i ) {
    if( ( i < 0 ) || ( i > 2 ) ) {
      throw std::runtime_error( BIAL_ERROR( "Index out of range." ) );
    }
    return( ( &x )[ i ] );
  }

  double Normal::LengthSquared( ) const {
    return( x * x + y * y + z * z );
  }

  double Normal::Length( ) const {
    return( std::sqrt( LengthSquared( ) ) );
  }

  Normal Normal::Normalized( ) const {
    return( ( *this ) / Length( ) );
  }

  Vector3D operator*( double s, const Vector3D &vec ) {
    return( vec * s );
  }

  double Dot( const Vector3D &v1, const Vector3D &v2 ) {
    if( v1.HasNaNs( ) || v2.HasNaNs( ) ) {
      throw std::runtime_error( BIAL_ERROR( "v1 and v2 should not have NaNs." ) );
    }
    return( v1.x * v2.x + v1.y * v2.y + v1.z * v2.z );
  }

  double AbsDot( const Vector3D &v1, const Vector3D &v2 ) {
    if( v1.HasNaNs( ) || v2.HasNaNs( ) ) {
      throw std::runtime_error( BIAL_ERROR( "v1 and v2 should not have NaNs." ) );
    }
    return( std::abs( v1.x * v2.x + v1.y * v2.y + v1.z * v2.z ) );
  }

  Vector3D Cross( const Vector3D &v1, const Vector3D &v2 ) {
    return( Vector3D(
              ( v1.y * v2.z ) - ( v1.z * v2.y ),
              ( v1.z * v2.x ) - ( v1.x * v2.z ),
              ( v1.x * v2.y ) - ( v1.y * v2.x )
              ) );
  }

  void CoordinateSystem( const Vector3D &v1, Vector3D *v2, Vector3D *v3 ) {
    if( std::abs( v1.x ) > std::abs( v1.y ) ) {
      double invLen = 1.0 / std::sqrt( v1.x * v1.x + v1.z * v1.z );
      *v2 = Vector3D( -v1.z * invLen, 0.0, v1.x * invLen );
    }
    else {
      double invLen = 1.0 / std::sqrt( v1.y * v1.y + v1.z * v1.z );
      *v2 = Vector3D( 0.0, v1.z * invLen, -v1.y * invLen );
    }
    *v3 = Cross( v1, *v2 );
  }

  double Distance( const Point3D &p1, const Point3D &p2 ) {
    return( ( p1 - p2 ).Length( ) );
  }

  double DistanceSquared( const Point3D &p1, const Point3D &p2 ) {
    return( ( p1 - p2 ).LengthSquared( ) );
  }

  Normal operator*( double s, const Normal &norm ) {
    return( norm * s );
  }

  double Dot( const Normal &n1, const Normal &n2 ) {
    if( n1.HasNaNs( ) || n2.HasNaNs( ) ) {
      throw std::runtime_error( BIAL_ERROR( "n1 and n2 should not have NaNs." ) );
    }
    return( n1.x * n2.x + n1.y * n2.y + n1.z * n2.z );
  }

  double AbsDot( const Normal &n1, const Normal &n2 ) {
    if( n1.HasNaNs( ) || n2.HasNaNs( ) ) {
      throw std::runtime_error( BIAL_ERROR( "n1 and n2 should not have NaNs." ) );
    }
    return( std::abs( n1.x * n2.x + n1.y * n2.y + n1.z * n2.z ) );
  }

  double Dot( const Normal &n1, const Vector3D &v2 ) {
    if( n1.HasNaNs( ) || v2.HasNaNs( ) ) {
      throw std::runtime_error( BIAL_ERROR( "n1 and v2 should not have NaNs." ) );
    }
    return( n1.x * v2.x + n1.y * v2.y + n1.z * v2.z );
  }

  double Dot( const Vector3D &v1, const Normal &n2 ) {
    if( v1.HasNaNs( ) || n2.HasNaNs( ) ) {
      throw std::runtime_error( BIAL_ERROR( "v1 and n2 should not have NaNs." ) );
    }
    return( v1.x * n2.x + v1.y * n2.y + v1.z * n2.z );
  }

  double AbsDot( const Normal &n1, const Vector3D &v2 ) {
    if( n1.HasNaNs( ) || v2.HasNaNs( ) ) {
      throw std::runtime_error( BIAL_ERROR( "n1 and v2 should not have NaNs." ) );
    }
    return( std::abs( n1.x * v2.x + n1.y * v2.y + n1.z * v2.z ) );
  }

  double AbsDot( const Vector3D &v1, const Normal &n2 ) {
    if( v1.HasNaNs( ) || n2.HasNaNs( ) ) {
      throw std::runtime_error( BIAL_ERROR( "v1 and n2 should not have NaNs." ) );
    }
    return( std::abs( v1.x * n2.x + v1.y * n2.y + v1.z * n2.z ) );
  }

  Normal FaceForward( const Normal &n, const Vector3D &v ) {
    return( ( Dot( n, v ) < 0.0 ) ? -n : n );
  }

  Ray::Ray( ) : mint( 0.0 ), maxt( INFINITY ), time( 0.0 ), depth( 0 ) {

  }

  Ray::Ray( const Point3D &origin, const Vector3D &direction, double start, double end, double t,
            int d ) : o( origin ), d( direction ), mint( start ), maxt( end ), time( t ), depth( d ) {
  }

  Ray::Ray( const Point3D &origin, const Vector3D &direction, const Ray &parent, double start,
            double end ) : o( origin ), d( direction ), mint( start ), maxt( end ), time( parent.time ),
    depth( parent.depth + 1 ) {
  }

  Point3D Ray::operator()( double t ) const {
    return( o + d * t );
  }

  BBox::BBox( ) : pMin( INFINITY, INFINITY, INFINITY ), pMax( -INFINITY, -INFINITY, -INFINITY ) {
  }

  BBox::BBox( const Point3D &p ) : pMin( p ), pMax( p ) {
  }

  BBox::BBox( const Point3D &p1, const Point3D &p2 ) : 
    pMin( std::min( p1.x, p2.x ), std::min( p1.y, p2.y ), std::min( p1.z, p2.z ) ), 
    pMax( std::max( p1.x, p2.x ), std::max( p1.y, p2.y ), std::max( p1.z, p2.z ) ) {
  }

  bool BBox::operator==( const BBox &other ) const {
    return( pMin == other.pMin && pMax == other.pMax );
  }

  bool BBox::operator!=( const BBox &other ) const {
    return( pMin != other.pMin || pMax != other.pMax );
  }

  BBox Union( const BBox &b, const BBox &b2 ) {
    BBox ret;
    ret.pMin.x = std::min( b.pMin.x, b2.pMin.x );
    ret.pMin.y = std::min( b.pMin.y, b2.pMin.y );
    ret.pMin.z = std::min( b.pMin.z, b2.pMin.z );
    ret.pMax.x = std::max( b.pMax.x, b2.pMax.x );
    ret.pMax.y = std::max( b.pMax.y, b2.pMax.y );
    ret.pMax.z = std::max( b.pMax.z, b2.pMax.z );
    return( ret );
  }

  BBox Union( const BBox &b, const Point3D &p ) {
    BBox ret;
    ret.pMin.x = std::min( b.pMin.x, p.x );
    ret.pMin.y = std::min( b.pMin.y, p.y );
    ret.pMin.z = std::min( b.pMin.z, p.z );
    ret.pMax.x = std::max( b.pMax.x, p.x );
    ret.pMax.y = std::max( b.pMax.y, p.y );
    ret.pMax.z = std::max( b.pMax.z, p.z );
    return( ret );
  }

  bool BBox::Overlaps( const BBox &b ) const {
    bool x = ( pMax.x >= b.pMin.x ) && ( pMin.x <= b.pMax.x );
    bool y = ( pMax.y >= b.pMin.y ) && ( pMin.y <= b.pMax.y );
    bool z = ( pMax.z >= b.pMin.z ) && ( pMin.z <= b.pMax.z );
    return( x && y && z );
  }

  bool BBox::Inside( const Point3D &pt ) const {
    return( pt.x >= pMin.x && pt.x <= pMax.x &&
            pt.y >= pMin.y && pt.y <= pMax.y &&
            pt.z >= pMin.z && pt.z <= pMax.z );
  }

  void BBox::Expand( double delta ) {
    pMin -= Vector3D( delta, delta, delta );
    pMax += Vector3D( delta, delta, delta );
  }

  double BBox::SurfaceArea( ) const {
    Vector3D d = pMax - pMin;
    return( 2.0 * ( d.x * d.y + d.x * d.z + d.y * d.z ) );
  }

  double BBox::Volume( ) const {
    Vector3D d = pMax - pMin;
    return( d.x * d.y * d.z );
  }

  int BBox::MaximumExtent( ) const {
    Vector3D diag = pMax - pMin;
    if( ( diag.x > diag.y ) && ( diag.x > diag.z ) ) {
      return( 0 );
    }
    else if( diag.y > diag.z ) {
      return( 1 );
    }
    else {
      return( 2 );
    }
  }

  const Point3D &BBox::operator[]( int i ) const {
    if( ( i != 0 ) && ( i != 1 ) ) {
      throw std::runtime_error( BIAL_ERROR( "Index out of range." ) );
    }
    return( ( &pMin )[ i ] );
  }

  Point3D &BBox::operator[]( int i ) {
    if( ( i != 0 ) && ( i != 1 ) ) {
      throw std::runtime_error( BIAL_ERROR( "Index out of range." ) );
    }
    return( ( &pMin )[ i ] );
  }

  Point3D BBox::at( size_t vtx ) {
    /*
     * Lorensen's marching cube.
     *    switch (vtx) {
     *    case 0:
     *      return pMin;                                                       //V0=(0,0,0)
     *    case 1:
     *      return Point3D(pMin.x + pMax.x, pMin.y         , pMin.z         ); //V1=(1,0,0)
     *    case 2:
     *      return Point3D(pMin.x         , pMin.y + pMax.y, pMin.z         ); //V2=(0,1,0)
     *    case 3:
     *      return Point3D(pMin.x + pMax.x, pMin.y + pMax.y, pMin.z         ); //V3=(1,1,0)
     *    case 4:
     *      return Point3D(pMin.x         , pMin.y         , pMin.z + pMax.z); //V4=(0,0,1)
     *    case 5:
     *      return Point3D(pMin.x + pMax.x, pMin.y         , pMin.z + pMax.z); //V5=(1,0,1)
     *    case 6:
     *      return Point3D(pMin.x         , pMin.y + pMax.y, pMin.z + pMax.z); //V6=(0,1,1)
     *    case 7:
     *      return Point3D(pMin.x + pMax.x, pMin.y + pMax.y, pMin.z + pMax.z); //V7=(1,1,1)
     *    default:
     *      throw std::runtime_error(BIAL_ERROR("Index out of range."));
     *      break;
     *    }
     * http://paulbourke.net/geometry/polygonise/ marching cube
     */
    switch( vtx ) {
        case 0: {
        return( Point3D( pMin.x, pMin.y, pMin.z + pMax.z ) ); /* V0=(0,0,1) */
      }
        case 1: {
        return( Point3D( pMin.x + pMax.x, pMin.y, pMin.z + pMax.z ) ); /* V1=(1,0,1) */
      }
        case 2: {
        return( Point3D( pMin.x + pMax.x, pMin.y, pMin.z ) ); /* V2=(1,0,0) */
      }
        case 3: {
        return( pMin ); /* V3=(0,0,0) */
      }
        case 4: {
        return( Point3D( pMin.x, pMin.y + pMax.y, pMin.z + pMax.z ) ); /* V4=(0,1,1) */
      }
        case 5: {
        return( Point3D( pMin.x + pMax.x, pMin.y + pMax.y, pMin.z + pMax.z ) ); /* V5=(1,1,1) */
      }
        case 6: {
        return( Point3D( pMin.x + pMax.x, pMin.y + pMax.y, pMin.z ) ); /* V6=(1,1,0) */
      }
        case 7: {
        return( Point3D( pMin.x, pMin.y + pMax.y, pMin.z ) ); /* V2=(0,1,0) */
      }
        default: {
        throw std::runtime_error( BIAL_ERROR( "Index out of range." ) );
        break;
      }
    }
  }

  Point3D BBox::Lerp( double tx, double ty, double tz ) const {
    return( Point3D( Bial::Lerp( tx, pMin.x, pMax.x ), Bial::Lerp( ty, pMin.y, pMax.y ),
                     Bial::Lerp( tz, pMin.z, pMax.z ) ) );
  }


  Vector3D BBox::Offset( const Point3D &p ) const {
    const Vector3D d1 = p - pMin, d2 = pMax - pMin;
    return( Vector3D( d1.x / d2.x, d1.y / d2.y, d1.z / d2.z ) );
  }

  void BBox::BoundingSphere( Point3D *c, double *rad ) const {
    *c = .5f * pMin + .5f * pMax;
    *rad = Inside( *c ) ? Distance( *c, pMax ) : 0.0;
  }

  double Lerp( double t, double v1, double v2 ) {
    return( ( 1.0 - t ) * v1 + t * v2 );
  }

  Point3D operator*( double s, const Point3D &p ) {
    return( Point3D( p.x * s, p.y * s, p.z * s ) );
  }

  /* Transform3D */

  Transform3D::Transform3D( ) : affineMatrix( 4, 4 ) {
    reset( );
  }

  Transform3D::Transform3D( const Matrix< double > &mat ) :
    affineMatrix( mat ), inverseMatrix( MatrixOp::Inverse( mat ) ) {
    if( mat.Dims( ) != 2 ) {
      std::string msg( BIAL_ERROR( "Invalid number of dimensions. Expected 2." ) );
      throw( std::logic_error( msg ) );
    }
    if( ( mat.size( 0 ) != 4 ) && ( mat.size( 1 ) != 4 ) ) {
      std::string msg( BIAL_ERROR( "Matrix must be a 4x4 matrix." ) );
      throw( std::logic_error( msg ) );
    }
  }

  Transform3D::Transform3D( const Matrix< double > &mat, const Matrix< double > &inv ) :
    affineMatrix( mat ), inverseMatrix( inv ) {
    if( mat.Dims( ) != 2 ) {
      std::string msg( BIAL_ERROR( "Matrix has invalid number of dimensions. Expected 2." ) );
      throw( std::logic_error( msg ) );
    }
    if( ( mat.size( 0 ) != 4 ) && ( mat.size( 1 ) != 4 ) ) {
      std::string msg( BIAL_ERROR( "Matrix must be a 4x4 matrix." ) );
      throw( std::logic_error( msg ) );
    }
    if( inv.Dims( ) != 2 ) {
      std::string msg( BIAL_ERROR( "Inverse matrix has invalid number of dimensions. Expected 2." ) );
      throw( std::logic_error( msg ) );
    }
    if( ( inv.size( 0 ) != 4 ) && ( inv.size( 1 ) != 4 ) ) {
      std::string msg( BIAL_ERROR( "Inverse Matrix must be a 4x4 matrix." ) );
      throw( std::logic_error( msg ) );
    }
  }

  Matrix< double > Transform3D::getAffineMatrix( ) const {
    return( affineMatrix );
  }

  void Transform3D::setAffineMatrix( const Matrix< double > &mat ) {
    try {
      if( mat.Dims( ) != 2 ) {
        std::string msg( BIAL_ERROR( "Invalid number of dimensions. Expected 2." ) );
        throw( std::logic_error( msg ) );
      }
      if( ( mat.size( 0 ) != 4 ) && ( mat.size( 1 ) != 4 ) ) {
        std::string msg( BIAL_ERROR( "Matrix must be a 4x4 matrix." ) );
        throw( std::logic_error( msg ) );
      }
      affineMatrix = mat;
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

  Matrix< double > Transform3D::getInverseMatrix( ) const {
    return( inverseMatrix );
  }

  void Transform3D::setInverseMatrix( const Matrix< double > &value ) {
    inverseMatrix = value;
  }

  void Transform3D::reset( ) {
    try {
      MatrixOp::Identity( affineMatrix );
      inverseMatrix = affineMatrix;
      MatrixOp::Inverse( inverseMatrix );
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

  Transform3D Transform3D::copy( ) {
    return( *this );
  }

  Transform3D Transform3D::Inverse( ) {
    return( Transform3D( inverseMatrix, affineMatrix ) );
  }

  bool Transform3D::operator==( const Transform3D &other ) {
    /* FIXME: The Matrix == operator does not work. */
    return( affineMatrix.Equals( other.affineMatrix, 0.001 ) && inverseMatrix.Equals( other.inverseMatrix, 0.001 ) );
  }

  bool Transform3D::operator!=( const Transform3D &other ) {
    return( !( ( *this ) == other ) );
  }

  Transform3D Transform3D::operator*( const Transform3D &other ) const {
    Matrix< double > m1 = affineMatrix * other.affineMatrix;
    Matrix< double > m2 = other.inverseMatrix * inverseMatrix;
    return( Transform3D( m1, m2 ) );
  }

  Point3D Transform3D::operator()( const Point3D &pt ) const {
    try {
      double x = pt.x, y = pt.y, z = pt.z;
      Point3D res;
      res.x = affineMatrix[ 0 ] * x + affineMatrix[ 1 ] * y + affineMatrix[ 2 ] * z + affineMatrix[ 3 ];
      res.y = affineMatrix[ 4 ] * x + affineMatrix[ 5 ] * y + affineMatrix[ 6 ] * z + affineMatrix[ 7 ];
      res.z = affineMatrix[ 8 ] * x + affineMatrix[ 9 ] * y + affineMatrix[ 10 ] * z + affineMatrix[ 11 ];
      double w = affineMatrix[ 12 ] * x + affineMatrix[ 13 ] * y + affineMatrix[ 14 ] * z + affineMatrix[ 15 ];
      if( w == 0.0 ) {
        throw std::runtime_error( BIAL_ERROR( "W should not be 0!" ) );
      }
      if( w != 1.0 ) {
        res /= w;
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

  Point3D Transform3D::operator()( double x, double y, double z ) const {
    try {
      Point3D res;
      res.x = affineMatrix[ 0 ] * x + affineMatrix[ 1 ] * y + affineMatrix[ 2 ] * z + affineMatrix[ 3 ];
      res.y = affineMatrix[ 4 ] * x + affineMatrix[ 5 ] * y + affineMatrix[ 6 ] * z + affineMatrix[ 7 ];
      res.z = affineMatrix[ 8 ] * x + affineMatrix[ 9 ] * y + affineMatrix[ 10 ] * z + affineMatrix[ 11 ];
      double w = affineMatrix[ 12 ] * x + affineMatrix[ 13 ] * y + affineMatrix[ 14 ] * z + affineMatrix[ 15 ];
      if( w == 0.0 ) {
        throw std::runtime_error( BIAL_ERROR( "W should not be 0!" ) );
      }
      if( w != 1.0 ) {
        res /= w;
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


  void Transform3D::operator()( const Point3D &pt, Point3D *res ) const {
    try {
      if( !res ) {
        throw std::runtime_error( BIAL_ERROR( "Res cannot be NULL" ) );
      }
      double x = pt.x, y = pt.y, z = pt.z;
      res->x = affineMatrix[ 0 ] * x + affineMatrix[ 1 ] * y + affineMatrix[ 2 ] * z + affineMatrix[ 3 ];
      res->y = affineMatrix[ 4 ] * x + affineMatrix[ 5 ] * y + affineMatrix[ 6 ] * z + affineMatrix[ 7 ];
      res->z = affineMatrix[ 8 ] * x + affineMatrix[ 9 ] * y + affineMatrix[ 10 ] * z + affineMatrix[ 11 ];
      double w = affineMatrix[ 12 ] * x + affineMatrix[ 13 ] * y + affineMatrix[ 14 ] * z + affineMatrix[ 15 ];
      if( w == 0.0 ) {
        throw std::runtime_error( BIAL_ERROR( "W should not be 0!" ) );
      }
      if(w != 1.0) {
        (*res) /= w;
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

  Vector3D Transform3D::operator()( const Vector3D &vec ) const {
    try {
      double x = vec.x, y = vec.y, z = vec.z;
      Vector3D res;
      res.x = affineMatrix[ 0 ] * x + affineMatrix[ 1 ] * y + affineMatrix[ 2 ] * z;
      res.y = affineMatrix[ 4 ] * x + affineMatrix[ 5 ] * y + affineMatrix[ 6 ] * z;
      res.z = affineMatrix[ 8 ] * x + affineMatrix[ 9 ] * y + affineMatrix[ 10 ] * z;
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

  void Transform3D::operator()( const Vector3D &vec, Vector3D *res ) const {
    try {
      if( !res ) {
        throw std::runtime_error( BIAL_ERROR( "Res cannot be NULL" ) );
      }
      double x = vec.x, y = vec.y, z = vec.z;
      res->x = affineMatrix[ 0 ] * x + affineMatrix[ 1 ] * y + affineMatrix[ 2 ] * z;
      res->y = affineMatrix[ 4 ] * x + affineMatrix[ 5 ] * y + affineMatrix[ 6 ] * z;
      res->z = affineMatrix[ 8 ] * x + affineMatrix[ 9 ] * y + affineMatrix[ 10 ] * z;
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

  Normal Transform3D::operator()( const Normal &norm ) const {
    try {
      double x = norm.x, y = norm.y, z = norm.z;
      Normal res;
      res.x = affineMatrix[ 0 ] * x + affineMatrix[ 4 ] * y + affineMatrix[ 8 ] * z;
      res.y = affineMatrix[ 1 ] * x + affineMatrix[ 5 ] * y + affineMatrix[ 9 ] * z;
      res.z = affineMatrix[ 2 ] * x + affineMatrix[ 6 ] * y + affineMatrix[ 10 ] * z;
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

  void Transform3D::operator()( const Normal &norm, Normal *res ) const {
    try {
      if( !res ) {
        throw std::runtime_error( BIAL_ERROR( "Res cannot be NULL" ) );
      }
      double x = norm.x, y = norm.y, z = norm.z;
      res->x = affineMatrix[ 0 ] * x + affineMatrix[ 4 ] * y + affineMatrix[ 8 ] * z;
      res->y = affineMatrix[ 1 ] * x + affineMatrix[ 5 ] * y + affineMatrix[ 9 ] * z;
      res->z = affineMatrix[ 2 ] * x + affineMatrix[ 6 ] * y + affineMatrix[ 10 ] * z;
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

  Ray Transform3D::operator()( const Ray &ray ) const {
    try {
      Ray res = ray;
      ( *this )( res.o, &res.o );
      ( *this )( res.d, &res.d );
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

  void Transform3D::operator()( const Ray &ray, Ray *res ) const {
    try {
      ( *this )( ray.o, &( res->o ) );
      ( *this )( ray.d, &( res->d ) );
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

  BBox Transform3D::operator()( const BBox &b ) const {
    try {
      const Transform3D &M = *this;
      BBox res( M( b.pMin ) );
      res = Union( res, M( Point3D( b.pMax.x, b.pMin.y, b.pMin.z ) ) );
      res = Union( res, M( Point3D( b.pMin.x, b.pMax.y, b.pMin.z ) ) );
      res = Union( res, M( Point3D( b.pMin.x, b.pMin.y, b.pMax.z ) ) );
      res = Union( res, M( Point3D( b.pMin.x, b.pMax.y, b.pMax.z ) ) );
      res = Union( res, M( Point3D( b.pMax.x, b.pMax.y, b.pMin.z ) ) );
      res = Union( res, M( Point3D( b.pMax.x, b.pMin.y, b.pMax.z ) ) );
      res = Union( res, M( Point3D( b.pMax.x, b.pMax.y, b.pMax.z ) ) );
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

  void Transform3D::operator()( const BBox &b, BBox *res ) const {
    try {
      const Transform3D &M = *this;
      ( *res ) = M( b.pMin );
      ( *res ) = Union( ( *res ), M( Point3D( b.pMax.x, b.pMin.y, b.pMin.z ) ) );
      ( *res ) = Union( ( *res ), M( Point3D( b.pMin.x, b.pMax.y, b.pMin.z ) ) );
      ( *res ) = Union( ( *res ), M( Point3D( b.pMin.x, b.pMin.y, b.pMax.z ) ) );
      ( *res ) = Union( ( *res ), M( Point3D( b.pMin.x, b.pMax.y, b.pMax.z ) ) );
      ( *res ) = Union( ( *res ), M( Point3D( b.pMax.x, b.pMax.y, b.pMin.z ) ) );
      ( *res ) = Union( ( *res ), M( Point3D( b.pMax.x, b.pMin.y, b.pMax.z ) ) );
      ( *res ) = Union( ( *res ), M( Point3D( b.pMax.x, b.pMax.y, b.pMax.z ) ) );
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

  Transform3D &Transform3D::Translate( double dx, double dy, double dz ) {
    try {
      Matrix< double > mat( 4, 4 );
      MatrixOp::Identity( mat );

      mat( 3, 0 ) = dx;
      mat( 3, 1 ) = dy;
      mat( 3, 2 ) = dz;

      affineMatrix *= mat;

      mat( 3, 0 ) = -dx;
      mat( 3, 1 ) = -dy;
      mat( 3, 2 ) = -dz;

      inverseMatrix = mat * inverseMatrix;

      return( *this );
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

  Transform3D &Transform3D::Scale( double sx, double sy, double sz ) {
    try {
      /*
       *      if( ( sx <= 0.0001 ) || ( sy <= 0.0001 ) || ( sz <= 0.0001 ) ) {
       *        std::string msg( BIAL_ERROR( "Zero pixel dimension in output image." ) );
       *        throw( std::logic_error( msg ) );
       *      }
       */

      Matrix< double > mat( 4, 4 );
      MatrixOp::Identity( mat );

      mat( 0, 0 ) = sx;
      mat( 1, 1 ) = sy;
      mat( 2, 2 ) = sz;

      affineMatrix *= mat;

      mat( 0, 0 ) = 1.0 / sx;
      mat( 1, 1 ) = 1.0 / sy;
      mat( 2, 2 ) = 1.0 / sz;

      inverseMatrix = mat * inverseMatrix;

      return( *this );
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

  Transform3D &Transform3D::Scale( double factor ) {
    try {
      /*
       *      if( factor <= 0.0001 ) {
       *        std::string msg( BIAL_ERROR( "Zero pixel dimension in output image." ) );
       *        throw( std::logic_error( msg ) );
       *      }
       */
      return( Scale( factor, factor, factor ) );
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

  bool Transform3D::HasScale( ) const {
    try {
      double la2 = ( *this )( Vector3D( 1, 0, 0 ) ).LengthSquared( );
      double lb2 = ( *this )( Vector3D( 0, 1, 0 ) ).LengthSquared( );
      double lc2 = ( *this )( Vector3D( 0, 0, 1 ) ).LengthSquared( );
      return( !Equals( la2, 1.0, .001 ) || !Equals( lb2, 1.0, .001 ) || !Equals( lc2, 1.0, .001 ) );
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

  Transform3D &Transform3D::Rotate( double degrees, int dms ) {
    try {
      return( RotateRadians( Radians( degrees ), dms ) );
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

  Transform3D &Transform3D::RotateRadians( double rad, int dms ) {
    try {
      Matrix< double > mat( 4, 4 );
      MatrixOp::Identity( mat );

      double s = std::sin( rad );
      double c = std::cos( rad );
      if( dms == 0 ) {
        COMMENT( "Rotation around x axis.", 1 );
        mat( 1, 1 ) = c;
        mat( 1, 2 ) = -s;
        mat( 2, 1 ) = s;
        mat( 2, 2 ) = c;
      }
      else if( dms == 1 ) {
        COMMENT( "Rotation around y axis.", 1 );
        mat( 0, 0 ) = c;
        mat( 0, 2 ) = s;
        mat( 2, 0 ) = -s;
        mat( 2, 2 ) = c;
      }
      else if( dms == 2 ) {
        COMMENT( "Rotation around z axis.", 1 );
        mat( 0, 0 ) = c;
        mat( 0, 1 ) = -s;
        mat( 1, 0 ) = s;
        mat( 1, 1 ) = c;
      }
      else {
        std::string msg( BIAL_ERROR( "Invalid axis. Expected: 0 <= axis <= 2." ) );
        throw( std::logic_error( msg ) );
      }
      affineMatrix *= mat;

      MatrixOp::Transpose( mat );

      inverseMatrix = mat * inverseMatrix;

      return( *this );
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

  Transform3D &Transform3D::Rotate( double degrees, const Vector3D &axis ) {
    try {
      return( RotateRadians( Radians( degrees ), axis ) );
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

  Transform3D &Transform3D::RotateRadians( double rad, const Vector3D &axis ) {
    try {
      Matrix< double > mat( 4, 4 );
      MatrixOp::Identity( mat );

      Vector3D a = axis.Normalized( );
      double s = std::sin( rad );
      double c = std::cos( rad );

      mat( 0, 0 ) = a.x * a.x + ( 1.0 - a.x * a.x ) * c;
      mat( 0, 1 ) = a.x * a.y * ( 1.0 - c ) - a.z * s;
      mat( 0, 2 ) = a.x * a.z * ( 1.0 - c ) + a.y * s;

      mat( 1, 0 ) = a.y * a.x * ( 1.0 - c ) + a.z * s;
      mat( 1, 1 ) = a.y * a.y + ( 1.0 - a.y * a.y ) * c;
      mat( 1, 2 ) = a.y * a.z * ( 1.0 - c ) - a.x * s;

      mat( 2, 0 ) = a.z * a.x * ( 1.0 - c ) - a.y * s;
      mat( 2, 1 ) = a.z * a.y * ( 1.0 - c ) + a.x * s;
      mat( 2, 2 ) = a.z * a.z + ( 1.0 - a.z * a.z ) * c;

      affineMatrix *= mat;
      MatrixOp::Transpose( mat );

      inverseMatrix = mat * inverseMatrix;

      return( *this );
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

  Transform3D &Transform3D::Shear( double s_xy, double s_yx, double s_xz, double s_yz, double s_zx, double s_zy ) {
    try {
      Matrix< double > mat( 4, 4 );
      MatrixOp::Identity( mat );

      mat( 0, 1 ) = s_xy;
      mat( 0, 2 ) = s_xz;
      mat( 1, 0 ) = s_yx;
      mat( 1, 2 ) = s_yz;
      mat( 2, 0 ) = s_zx;
      mat( 2, 1 ) = s_zy;

      affineMatrix *= mat;

      mat( 1, 0 ) = -s_xy;
      mat( 2, 0 ) = -s_xz;
      mat( 0, 1 ) = -s_yx;
      mat( 2, 1 ) = -s_yz;
      mat( 0, 2 ) = -s_zx;
      mat( 1, 2 ) = -s_zy;

      inverseMatrix = mat * inverseMatrix;

      return( *this );
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

  Transform3D &Transform3D::LookAt( const Point3D &pos, const Point3D &look, const Vector3D &up ) {
    try {
      Matrix< double > mat( 4, 4 );
      MatrixOp::Identity( mat );

      Vector3D dir = Vector3D( look - pos ).Normalized( );
      Vector3D left = Cross( up.Normalized( ), dir );
      Vector3D newUp = Cross( dir, left );

      mat( 0, 0 ) = left.x;
      mat( 1, 0 ) = left.y;
      mat( 2, 0 ) = left.z;

      mat( 0, 1 ) = newUp.x;
      mat( 1, 1 ) = newUp.y;
      mat( 2, 1 ) = newUp.z;

      mat( 0, 2 ) = dir.x;
      mat( 1, 2 ) = dir.y;
      mat( 2, 2 ) = dir.z;

      mat( 0, 3 ) = pos.x;
      mat( 1, 3 ) = pos.y;
      mat( 2, 3 ) = pos.z;

      inverseMatrix = mat * inverseMatrix;

      MatrixOp::Inverse( mat );

      affineMatrix *= mat;

      return( *this );
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

  bool Transform3D::SwapsHandedness( ) const {
    return( MatrixOp::Determinant( affineMatrix ) < 0.0 );
  }

  DifferentialGeometry::DifferentialGeometry( ) : u( 0. ), v( 0. ), shape( NULL ) {
  }

  DifferentialGeometry::DifferentialGeometry( const Point3D &p, const Vector3D &dpdu, const Vector3D &dpdv, 
                                              const Normal &dndu, const Normal &dndv, float u, float v,
                                              const Shape *sh ) : 
    p( p ), dpdu( dpdu ), dpdv( dpdv ), dndu( dndu ), dndv( dndv ), u( u ), v( v ), shape( sh ) {
    COMMENT( "Initializing DifferentialGeometry from parameters.", 2 );
    nn = Normal( Cross( dpdu, dpdv ) ).Normalized( );
    COMMENT( "Adjusting normal based on orientation and handedness", 2 );
    if( shape && ( shape->ReverseOrientation ^ shape->TransformSwapsHandedness ) ) {
      nn *= -1.0;
    }
  }


  /* FastTransform */

  FastTransform::FastTransform( ) : affineMatrix( 4, 4 ), inverseMatrix( 4, 4 ) {
    reset( );
  }

  FastTransform::FastTransform( const Matrix< int > &mat ) : 
    affineMatrix( mat ), inverseMatrix( MatrixOp::Inverse( mat ) ) {
    if( mat.Dims( ) != 2 ) {
      std::string msg( BIAL_ERROR( "Invalid number of dimensions. Expected 2." ) );
      throw( std::logic_error( msg ) );
    }
    if( ( mat.size( 0 ) != 4 ) && ( mat.size( 1 ) != 4 ) ) {
      std::string msg( BIAL_ERROR( "Matrix must be a 4x4 matrix." ) );
      throw( std::logic_error( msg ) );
    }
  }

  FastTransform::FastTransform( const Matrix< int > &mat,
                            const Matrix< int > &inv ) : affineMatrix( mat ), inverseMatrix( inv ) {
    if( mat.Dims( ) != 2 ) {
      std::string msg( BIAL_ERROR( "Matrix has invalid number of dimensions. Expected 2." ) );
      throw( std::logic_error( msg ) );
    }
    if( ( mat.size( 0 ) != 4 ) && ( mat.size( 1 ) != 4 ) ) {
      std::string msg( BIAL_ERROR( "Matrix must be a 4x4 matrix." ) );
      throw( std::logic_error( msg ) );
    }
    if( inv.Dims( ) != 2 ) {
      std::string msg( BIAL_ERROR( "Inverse matrix has invalid number of dimensions. Expected 2." ) );
      throw( std::logic_error( msg ) );
    }
    if( ( inv.size( 0 ) != 4 ) && ( inv.size( 1 ) != 4 ) ) {
      std::string msg( BIAL_ERROR( "Inverse Matrix must be a 4x4 matrix." ) );
      throw( std::logic_error( msg ) );
    }
  }

  Matrix< int > FastTransform::getAffineMatrix( ) const {
    return( affineMatrix );
  }

  void FastTransform::setAffineMatrix( const Matrix< int > &mat ) {
    try {
      if( mat.Dims( ) != 2 ) {
        std::string msg( BIAL_ERROR( "Invalid number of dimensions. Expected 2." ) );
        throw( std::logic_error( msg ) );
      }
      if( ( mat.size( 0 ) != 4 ) && ( mat.size( 1 ) != 4 ) ) {
        std::string msg( BIAL_ERROR( "Matrix must be a 4x4 matrix." ) );
        throw( std::logic_error( msg ) );
      }
      affineMatrix = mat;
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

  Matrix< int > FastTransform::getInverseMatrix( ) const {
  return( affineMatrix );
}

void FastTransform::setInverseMatrix( const Matrix< int > &inv ) {
  try {
    if( inv.Dims( ) != 2 ) {
      std::string msg( BIAL_ERROR( "Invalid number of dimensions. Expected 2." ) );
      throw( std::logic_error( msg ) );
    }
    if( ( inv.size( 0 ) != 4 ) && ( inv.size( 1 ) != 4 ) ) {
      std::string msg( BIAL_ERROR( "Matrix must be a 4x4 matrix." ) );
      throw( std::logic_error( msg ) );
    }
    inverseMatrix = inv;
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

  void FastTransform::reset( ) {
    try {
      MatrixOp::Identity( affineMatrix );
      inverseMatrix = affineMatrix;
      MatrixOp::Inverse( inverseMatrix );
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

  FastTransform FastTransform::Inverse( ) {
    return( FastTransform( inverseMatrix, affineMatrix ) );
  }

  FastTransform FastTransform::copy( ) {
    return( *this );
  }

  bool FastTransform::operator==( const FastTransform &other ) {
    return( affineMatrix.Equals( other.affineMatrix, 0.0 )
            && inverseMatrix.Equals( other.inverseMatrix, 0.0 ) );
  }

  bool FastTransform::operator!=( const FastTransform &other ) {
    return( !(*this == other));
  }

  FastTransform FastTransform::operator*( const FastTransform &other ) const {
    Matrix< double > m1 = affineMatrix * other.affineMatrix;
    Matrix< double > m2 = other.inverseMatrix * inverseMatrix;
    return( FastTransform( m1, m2 ) );
  }

  void FastTransform::operator()( int x, int y, int z, int *xx, int *yy, int *zz ) const {
    try {
      ( *xx ) = affineMatrix[ 0 ] * x + affineMatrix[ 1 ] * y + affineMatrix[ 2 ] * z + affineMatrix[ 3 ];
      ( *yy ) = affineMatrix[ 4 ] * x + affineMatrix[ 5 ] * y + affineMatrix[ 6 ] * z + affineMatrix[ 7 ];
      ( *zz ) = affineMatrix[ 8 ] * x + affineMatrix[ 9 ] * y + affineMatrix[ 10 ] * z + affineMatrix[ 11 ];
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

  Point3D FastTransform::operator()( const Point3D &pt ) const {
  try {
    int x = pt.x, y = pt.y, z = pt.z;
    Point3D res;
    res.x = affineMatrix[ 0 ] * x + affineMatrix[ 1 ] * y + affineMatrix[ 2 ] * z + affineMatrix[ 3 ];
    res.y = affineMatrix[ 4 ] * x + affineMatrix[ 5 ] * y + affineMatrix[ 6 ] * z + affineMatrix[ 7 ];
    res.z = affineMatrix[ 8 ] * x + affineMatrix[ 9 ] * y + affineMatrix[ 10 ] * z + affineMatrix[ 11 ];
    double w = affineMatrix[ 12 ] * x + affineMatrix[ 13 ] * y + affineMatrix[ 14 ] * z + affineMatrix[ 15 ];
    if(w != 1.0){
      res /= w;
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

Point3D FastTransform::operator()( double x, double y, double z ) const {
  try {
    Point3D res;
    res.x = affineMatrix[ 0 ] * x + affineMatrix[ 1 ] * y + affineMatrix[ 2 ] * z + affineMatrix[ 3 ];
    res.y = affineMatrix[ 4 ] * x + affineMatrix[ 5 ] * y + affineMatrix[ 6 ] * z + affineMatrix[ 7 ];
    res.z = affineMatrix[ 8 ] * x + affineMatrix[ 9 ] * y + affineMatrix[ 10 ] * z + affineMatrix[ 11 ];
    double w = affineMatrix[ 12 ] * x + affineMatrix[ 13 ] * y + affineMatrix[ 14 ] * z + affineMatrix[ 15 ];
    if(w != 1.0){
      res /= w;
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

void FastTransform::operator()( const Point3D &pt, Point3D *res ) const {
  try {
    if( !res ) {
      throw std::runtime_error( BIAL_ERROR( "Res cannot be NULL" ) );
    }
    int x = pt.x, y = pt.y, z = pt.z;
    res->x = affineMatrix[ 0 ] * x + affineMatrix[ 1 ] * y + affineMatrix[ 2 ] * z + affineMatrix[ 3 ];
    res->y = affineMatrix[ 4 ] * x + affineMatrix[ 5 ] * y + affineMatrix[ 6 ] * z + affineMatrix[ 7 ];
    res->z = affineMatrix[ 8 ] * x + affineMatrix[ 9 ] * y + affineMatrix[ 10 ] * z + affineMatrix[ 11 ];
    double w = affineMatrix[ 12 ] * x + affineMatrix[ 13 ] * y + affineMatrix[ 14 ] * z + affineMatrix[ 15 ];
    if(w != 1.0){
      (*res) /= w;
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

  FastTransform &FastTransform::Translate( int dx, int dy, int dz ) {
    try {
      Matrix< int > mat( 4, 4 );
      MatrixOp::Identity( mat );

      mat( 3, 0 ) = dx;
      mat( 3, 1 ) = dy;
      mat( 3, 2 ) = dz;

      affineMatrix *= mat;

      mat( 3, 0 ) = -dx;
      mat( 3, 1 ) = -dy;
      mat( 3, 2 ) = -dz;

      inverseMatrix = mat * inverseMatrix;

      return( *this );
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

  FastTransform &FastTransform::Scale( int sx, int sy, int sz ) {
    try {
      Matrix< int > mat( 4, 4 );
      MatrixOp::Identity( mat );

      mat( 0, 0 ) = sx;
      mat( 1, 1 ) = sy;
      mat( 2, 2 ) = sz;

      affineMatrix *= mat;

      mat( 0, 0 ) = 1.0 / sx;
      mat( 1, 1 ) = 1.0 / sy;
      mat( 2, 2 ) = 1.0 / sz;

      inverseMatrix = mat * inverseMatrix;

      return( *this );
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

  FastTransform &FastTransform::Scale( int factor ) {
    try {
      return( Scale( factor, factor, factor ) );
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

  FastTransform &FastTransform::Rotate( double degrees, int dms ) {
    try {
      return( RotateRadians( Radians( degrees ), dms ) );
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

  FastTransform &FastTransform::RotateRadians( double rad, int dms ) {
    try {
      Matrix< int > mat( 4, 4 );
      MatrixOp::Identity( mat );

      int s = round( std::sin( rad ) );
      int c = round( std::cos( rad ) );
      if( dms == 0 ) {
        COMMENT( "Rotation around x axis.", 1 );
        mat( 1, 1 ) = c;
        mat( 1, 2 ) = -s;
        mat( 2, 1 ) = s;
        mat( 2, 2 ) = c;
      }
      else if( dms == 1 ) {
        COMMENT( "Rotation around y axis.", 1 );
        mat( 0, 0 ) = c;
        mat( 0, 2 ) = s;
        mat( 2, 0 ) = -s;
        mat( 2, 2 ) = c;
      }
      else if( dms == 2 ) {
        COMMENT( "Rotation around z axis.", 1 );
        mat( 0, 0 ) = c;
        mat( 0, 1 ) = -s;
        mat( 1, 0 ) = s;
        mat( 1, 1 ) = c;
      }
      else {
        std::string msg( BIAL_ERROR( "Invalid axis. Expected: 0 <= axis <= 2." ) );
        throw( std::logic_error( msg ) );
      }
      affineMatrix *= mat;

      MatrixOp::Transpose( mat );

      inverseMatrix = mat * inverseMatrix;

      return( *this );
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

  FastTransform &FastTransform::Shear( int s_xy, int s_yx, int s_xz, int s_yz, int s_zx, int s_zy ) {
    try {
      Matrix< int > mat( 4, 4 );
      MatrixOp::Identity( mat );

      mat( 0, 1 ) = s_xy;
      mat( 0, 2 ) = s_xz;
      mat( 1, 0 ) = s_yx;
      mat( 1, 2 ) = s_yz;
      mat( 2, 0 ) = s_zx;
      mat( 2, 1 ) = s_zy;

      affineMatrix *= mat;

      mat( 1, 0 ) = -s_xy;
      mat( 2, 0 ) = -s_xz;
      mat( 0, 1 ) = -s_yx;
      mat( 2, 1 ) = -s_yz;
      mat( 0, 2 ) = -s_zx;
      mat( 1, 2 ) = -s_zy;

      inverseMatrix = mat * inverseMatrix;

      return( *this );
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
