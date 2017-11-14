/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2014/Jan/07
 * @brief Geometric transforms for 3D visualization and printing.
 */

#ifndef BIALGEOMETRICS_H
#define BIALGEOMETRICS_H

#include "Common.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"

namespace Bial {

  template< class D > class Image;

  /**
   * @defgroup render_geom Render geometric classes.
   * @brief Classes related to computer graphics and rendering process.
   * @{
   */

  /**
   * @date 2015/Aug/21
   * @brief Simple linear interpolation
   * @param t Factor
   * @param v1 value 1
   * @param v2 value 2
   * return none.
   */
  double Lerp( double t, double v1, double v2 );

  class Point3D;
  class Normal;

  /**
   * @date 2015/Aug/21
   * @brief The Vector3D class is a Geometric representation of a vector direction in 3D space.
   */
  class Vector3D {
public:
    /* Vector Public Data
    ** @brief %Vector direction in x.
    */
    double x;
    /** @brief %Vector direction in y. */
    double y;
    /** @brief %Vector direction in z. */
    double z;

    /* Vector Public Methods
    **
    * @date 2015/Aug/21
    * @brief Vector3D basic constructor.
    */
    Vector3D( );
    /**
     * @date 2015/Aug/21
     * @brief Vector3D constructor.
     * @param xx %Vector direction in x.
     * @param yy %Vector direction in y.
     * @param zz %Vector direction in z.
     * @warning Throws if has NaN parameter.
     */
    explicit Vector3D( double xx, double yy, double zz );
    /**
     * @date 2015/Aug/21
     * @brief Conversion from Normal to Vector3D.
     * @param norm input normal.
     */
    explicit Vector3D( const Normal &norm );
    /**
     * @date 2015/Aug/21
     * @brief HasNaNs returns if the any of the components is'nt a valid number.
     * return none.
     */
    bool HasNaNs( ) const;
    /**
     * @date 2015/Aug/21
     * @brief Returns if the components match.
     * @param vec Input vector
     * return none.
     */
    bool operator==( const Vector3D &vec ) const;
    /**
     * @date 2015/Aug/21
     * @brief Returns if the components do not match.
     * @param vec Input vector to be compared.
     * return none.
     */
    bool operator!=( const Vector3D &vec ) const;
    /**
     * @date 2015/Aug/21
     * @brief Add operation, the vector components are added.
     * @param vec Input vector to be compared.
     * @warning Throws if has NaNs.
     * @return Result vector.
     */
    Vector3D operator+( const Vector3D &vec ) const;
    /**
     * @date 2015/Aug/21
     * @brief Add operation, the vector components are added.
     * @param vec Input vector
     * @return Reference to 'this' vector.
     */
    Vector3D &operator+=( const Vector3D &vec );
    /**
     * @date 2015/Aug/21
     * @brief Subtract operation, the vector components are subtracted.
     * @param vec Input vector
     * @return Result vector.
     */
    Vector3D operator-( const Vector3D &vec ) const;
    /**
     * @date 2015/Aug/21
     * @brief Subtract operation, the vector components are subtracted.
     * @param vec Input vector
     * @return Reference to 'this' vector.
     */
    Vector3D &operator-=( const Vector3D &vec );
    /**
     * @date 2015/Aug/21
     * @brief Multiply operation, the vector components are multiplied to a scalar value.
     * @param s A scalar value to be multiplied.
     * @return Result vector.
     */
    Vector3D operator*( double s ) const;
    /**
     * @date 2015/Aug/21
     * @brief Multiply operation, the vector components are multiplied to a scalar value.
     * @param s A scalar value to be multiplied.
     * @return Reference to 'this' vector.
     */
    Vector3D &operator*=( double s );
    /**
     * @date 2015/Aug/21
     * @brief Division operation, the vector components are divided by \a s.
     * @param s A scalar denominator.
     * @return Result vector.
     */
    Vector3D operator/( double s ) const;
    /**
     * @date 2015/Aug/21
     * @brief Division operation, the vector components are divided by \a s.
     * @param s A scalar denominator.
     * @return Reference to 'this' vector.
     */
    Vector3D &operator/=( double s );
    /**
     * @date 2015/Aug/21
     * @brief Returns the vector inverse.
     * @return Result vector.
     */
    Vector3D operator-( ) const;
    /**
     * @date 2015/Aug/21
     * @brief Gives the option to loop over the components of a vector.
     * @param i is the component index ( 0 for x, 1 for y, and 2 for z )
     * @return Vector component value at index \a i.
     * @warning Throws if i < 0 or i > 2
     */
    double operator[]( int i ) const;
    /**
     * @date 2015/Aug/21
     * @brief Gives the option to loop over the components of a vector.
     * @param i is the component index ( 0 for x, 1 for y, and 2 for z )
     * @return Vector component value at index \a i.
     * @warning Throws if i < 0 or i > 2
     */
    double &operator[]( int i );
    /**
     * @date 2015/Aug/21
     * @brief Returns the length of a vector. <br>
     * Given a vector \f$ \vec{v} = (x,y,z) \f$, this function returns \f$||\vec{v}|| = \sqrt{x*x+y*y+z*z} \f$.
     * @return The length of a vector.
     */
    double LengthSquared( ) const;
    /**
     * @date 2015/Aug/21
     * @brief Returns the squared length of a vector. <br>
     * Given a vector \f$ \vec{v} = (x,y,z) \f$, this function returns \f$ ||\vec{v}||^2 = {x*x+y*y+z*z} \f$.
     * @return The squared length of a vector.
     */
    double Length( ) const;
    /**
     * @date 2015/Aug/21
     * @brief Returns the normalized vector. <br>
     * Given a vector \f$ \vec{v} = (x,y,z) \f$, this function returns \f$ \frac{\vec{v}}{||\vec{v}||^2} \f$.
     * return none.
     */
    Vector3D Normalized( ) const;
    /**
     * @date 2015/Aug/21
     * @brief Conversion from Vector3D to Point3D.
     * @return Result point.
     */
    Point3D toPoint( ) const;
    /**
     * @date 2015/Aug/21
     * @brief Conversion from Bial::Vector to Vector3D.
     * @param Input vector.
     * @return Result vector.
     */
    template< class D >
    static Vector3D fromVector( const Vector< D > vec );

  };

  /**
   * @date 2015/Aug/21
   * @brief The Point3D class is a Geometric representation of a point in 3D space. <br>
   * A point is a zero­dimensional location in 3D space.
   */
  class Point3D {
public:
    /* Point3D public data
    ** @brief %Point position in x.
    */
    double x;
    /** @brief %Point position in y. */
    double y;
    /** @brief %Point position in z. */
    double z;

    /* Point3D public methods
    **
    * @date 2015/Aug/21
    * @brief Vector3D basic constructor.
    */
    Point3D( );
    /**
     * @date 2015/Aug/21
     * @brief Point3D constructor.
     * @param xx %Point direction in x.
     * @param yy %Point direction in y.
     * @param zz %Point direction in z.
     * @warning Throws if has NaNs.
     */
    explicit Point3D( double xx, double yy, double zz );
    /**
     * @date 2015/Aug/21
     * @brief HasNaNs returns if the any of the components is'nt a valid number.
     * return none.
     */
    bool HasNaNs( ) const;
    /**
     * @date 2015/Aug/21
     * @brief Returns if the positions match.
     * @param p Input point to be compared.
     * @warning Throws if has NaNs.
     * return none.
     */
    bool operator==( const Point3D &p ) const;
    /**
     * @date 2015/Aug/21
     * @brief Returns if the positions do not match.
     * @param p Input point to be compared.
     * return none.
     */
    bool operator!=( const Point3D &p ) const;
    /**
     * @date 2015/Aug/21
     * @brief Add operation, the point position is added to the vector components.
     * @param vec Input vector.
     * @return Result point.
     */
    Point3D operator+( const Vector3D &vec ) const;
    /**
     * @date 2015/Aug/21
     * @brief Add operation, the point position is added to the vector components.
     * @param vec Input vector.
     * @return Reference to 'this' point.
     */
    Point3D &operator+=( const Vector3D &vec );
    /**
     * @date 2015/Aug/21
     * @brief Subtract operation, the point positions are subtracted, generating a Vector3D.
     * @param p Input point.
     * @warning Throws if has NaNs.
     * @return Result vector3D.
     */
    Vector3D operator-( const Point3D &p ) const;
    /**
     * @date 2015/Aug/21
     * @brief Subtract operation, the point position is subtracted to the vector components, generating a Vector3D.
     * @param vec Input vector.
     * @warning Throws if has NaNs.
     * @return Result point.
     */
    Vector3D operator-( const Vector3D &vec ) const;
    /**
     * @date 2015/Aug/21
     * @brief Subtract operation, the point position is subtracted to the vector components.
     * @param vec Input vector.
     * @return Reference to 'this' point.
     */
    Point3D &operator-=( const Vector3D &vec );
    /**
     * @date 2015/Aug/21
     * @brief Add operation, the points positions are added.
     * @param p Input point.
     * @return Reference to 'this' point.
     */
    Point3D &operator+=( const Point3D &p );
    /**
     * @date 2015/Aug/21
     * @brief Add operation, the points positions are added.
     * @param p Input point.
     * @return Result point.
     */
    Point3D operator+( const Point3D &p ) const;
    /**
     * @date 2015/Aug/21
     * @brief Multiply operation, the point position components are multiplied to a scalar value.
     * @param f A scalar value to be multiplied.
     * @return Result point.
     */
    Point3D operator*( double f ) const;
    /**
     * @date 2015/Aug/21
     * @brief Multiply operation, the point position components are multiplied to a scalar value.
     * @param f A scalar value to be multiplied.
     * @return Reference to 'this' point.
     */
    Point3D &operator*=( double f );
    /**
     * @date 2015/Aug/21
     * @brief Division operation, the point position components are divided by \a s.
     * @param s A scalar denominator.
     * @return Result point.
     */
    Point3D operator/( double f ) const;
    /**
     * @date 2015/Aug/21
     * @brief Division operation, the point position components are divided by \a s.
     * @param s A scalar denominator.
     * @return Reference to 'this' point.
     */
    Point3D &operator/=( double f );
    /**
     * @date 2015/Aug/21
     * @brief Gives the option to loop over the components of the point position.
     * @param i is the component index ( 0 for x, 1 for y, and 2 for z )
     * @return Position component value at index \a i.
     * @warning Throws if i < 0 or i > 2
     */
    double operator[]( int i ) const;
    /**
     * @date 2015/Aug/21
     * @brief Gives the option to loop over the components of the point position.
     * @param i is the component index ( 0 for x, 1 for y, and 2 for z )
     * @return Position component value at index \a i.
     * @warning Throws if i < 0 or i > 2
     */
    double &operator[]( int i );
    /**
     * @date 2015/Aug/21
     * @brief Conversion from Point3D to Vector3D.
     * @return Result vector.
     * @warning Throws if has NaNs.
     */
    Vector3D toVector( ) const;

    /**
     * @date 2015/Aug/21
     * @brief Simple function used in sorting algoritms.
     * @param p Input point to be compared.
     * return none.
     */
    bool operator<( const Point3D &p ) const {
      double m1 = ( x + y + z ) / 3.f;
      double m2 = ( p.x + p.y + p.z ) / 3.f;
      return ( m1 < m2 );
    }
    /**
     * @date 2015/Aug/21
     * @brief Simple function used in sorting algoritms.
     * @param p Input point to be compared.
     * return none.
     */
    bool operator>( const Point3D &p ) const {
      double m1 = ( x + y + z ) / 3.f;
      double m2 = ( p.x + p.y + p.z ) / 3.f;
      return ( m1 > m2 );
    }
  };

  /**
   * @date 2015/Aug/21
   * @brief The Normal class is the geometric representation of a Normal in 3D space. <br>
   * A surface normal (or just normal) is a vector that is perpendicular to a surface
   * at a particular position. <br> It can be defined as the cross product of any two
   * nonparallel vectors that are tangent to the surface at a point. <br>
   * The implementations of Normal3D and Vector3D are very similar. Like vectors, normals are
   * represented by three floats x, y, and z; <br> they can be added and subtracted
   * to compute new normals; and they can be scaled and normalized. <br>
   * However, a normal cannot be added to a point, and one cannot take the cross product of two normals.
   */
  class Normal {
public:
    /* Normal public data
    ** @brief Normal direction in x.
    */
    double x;
    /** @brief Normal direction in y. */
    double y;
    /** @brief Normal direction in z. */
    double z;

    /* Normal public methods
    **
    * @date 2015/Aug/21
    * @brief Normal basic constructor.
    */
    Normal( );
    /**
     * @date 2015/Aug/21
     * @brief Normal constructor.
     * @param xx %Normal direction in x.
     * @param yy %Normal direction in y.
     * @param zz %Normal direction in z.
     * @warning Throws if has NaN parameter.
     */
    explicit Normal( double xx, double yy, double zz );
    /**
     * @date 2015/Aug/21
     * @brief Conversion from Vector3D to Normal.
     * @param vec input vector.
     */
    explicit Normal( const Vector3D &vec );
    /**
     * @date 2015/Aug/21
     * @brief HasNaNs returns if the any of the components is'nt a valid number.
     * return none.
     */
    bool HasNaNs( ) const;
    /**
     * @date 2015/Aug/21
     * @brief Returns if the components match.
     * @param norm Input normal
     * return none.
     */
    bool operator==( const Normal &norm ) const;
    /**
     * @date 2015/Aug/21
     * @brief Returns if the components do not match.
     * @param norm Input normal to be compared.
     * return none.
     */
    bool operator!=( const Normal &norm ) const;
    /**
     * @date 2015/Aug/21
     * @brief Add operation, the normal components are added.
     * @param norm Input normal to be compared.
     * @warning Throws if has NaNs.
     * @return Result normal.
     */
    Normal operator+( const Normal &norm ) const;
    /**
     * @date 2015/Aug/21
     * @brief Add operation, the normal components are added.
     * @param norm Input normal
     * @return Reference to 'this' normal.
     */
    Normal &operator+=( const Normal &norm );
    /**
     * @date 2015/Aug/21
     * @brief Subtract operation, the normal components are subtracted.
     * @param norm Input normal
     * @return Result normal.
     */
    Normal operator-( const Normal &norm ) const;
    /**
     * @date 2015/Aug/21
     * @brief Subtract operation, the normal components are subtracted.
     * @param norm Input normal
     * @return Reference to 'this' normal.
     */
    Normal &operator-=( const Normal &norm );
    /**
     * @date 2015/Aug/21
     * @brief Multiply operation, the normal components are multiplied to a scalar value.
     * @param s A scalar value to be multiplied.
     * @return Result normal.
     */
    Normal operator*( double s ) const;
    /**
     * @date 2015/Aug/21
     * @brief Multiply operation, the normal components are multiplied to a scalar value.
     * @param s A scalar value to be multiplied.
     * @return Reference to 'this' normal.
     */
    Normal &operator*=( double s );
    /**
     * @date 2015/Aug/21
     * @brief Division operation, the normal components are divided by \a s.
     * @param s A scalar denominator.
     * @return Result normal.
     */
    Normal operator/( double s ) const;
    /**
     * @date 2015/Aug/21
     * @brief Division operation, the normal components are divided by \a s.
     * @param s A scalar denominator.
     * @return Reference to 'this' normal.
     */
    Normal &operator/=( double s );
    /**
     * @date 2015/Aug/21
     * @brief Returns the normal inverse.
     * @return Result normal.
     */
    Normal operator-( ) const;
    /**
     * @date 2015/Aug/21
     * @brief Gives the option to loop over the components of a normal.
     * @param i is the component index ( 0 for x, 1 for y, and 2 for z )
     * @return Normal component value at index \a i.
     * @warning Throws if i < 0 or i > 2
     */
    double operator[]( int i ) const;
    /**
     * @date 2015/Aug/21
     * @brief Gives the option to loop over the components of a normal.
     * @param i is the component index ( 0 for x, 1 for y, and 2 for z )
     * @return Normal component value at index \a i.
     * @warning Throws if i < 0 or i > 2
     */
    double &operator[]( int i );
    /**
     * @date 2015/Aug/21
     * @brief Returns the length of a normal. <br>
     * Given a normal \f$ \vec{v} = (x,y,z) \f$, this function returns \f$||\vec{v}|| = \sqrt{x*x+y*y+z*z} \f$.
     * @return The length of a normal.
     */
    double LengthSquared( ) const;
    /**
     * @date 2015/Aug/21
     * @brief Returns the squared length of a normal. <br>
     * Given a normal \f$ \vec{v} = (x,y,z) \f$, this function returns \f$ ||\vec{v}||^2 = {x*x+y*y+z*z} \f$.
     * @return The squared length of a normal.
     */
    double Length( ) const;
    /**
     * @date 2015/Aug/21
     * @brief Returns the normalized normal. <br>
     * Given a normal \f$ \vec{v} = (x,y,z) \f$, this function returns \f$ \frac{\vec{v}}{||\vec{v}||^2} \f$.
     * return none.
     */
    Normal Normalized( ) const;

  };

  /**
   * @date 2015/Aug/21
   * @brief The Ray class is a mathematic representation of a light ray. <br>
   * A ray is a semi­infinite line specified by its origin and direction.
   */
  class Ray {
public:
    /* Ray public data
    **
    * @brief Ray origin point.
    */
    Point3D o;
    /**
     * @brief Ray direction vector.
     */
    Vector3D d;
    /**
     * @brief Minimum value for parameter t.
     */
    mutable double mint;
    /**
     * @brief Maximum value for parameter t.
     */
    mutable double maxt;
    /**
     * @brief Ray time counter used in animated objects simulation.
     */
    double time;
    /**
     * @brief The ray depth member tracks how many times light has bounced along the current path.
     */
    int depth;

    /* Ray public methods
    **
    * @brief Ray basic constructor.
    */
    Ray( );
    /**
     * @brief Ray constructor.
     * @param origin Ray origin point.
     * @param direction Ray direction vector.
     * @param start Minimum value for parameter t.
     * @param end Maximum value for parameter t.
     * @param t Ray time counter used in animated objects simulation.
     * @param d The ray depth member tracks how many times light has bounced along the current path.
     */
    Ray( const Point3D &origin,
         const Vector3D &direction,
         double start = 0.f,
         double end = INFINITY,
         double t = 0.f,
         int d = 0 );
    /**
     * @brief Ray constructor with "parent" ray. <br>
     * For convenience we also provide another ray constructor that takes a “parent” ray; the idea is that when
     * additional rays are being spawned from an intersection,
     * it’s useful to be able to copy the time value and set the depth value for the new ray based on the corresponding
     * values from the previous ray.
     * @param origin Ray origin point.
     * @param direction Ray direction vector.
     * @param parent A parent Ray.
     * @param start Minimum value for parameter t.
     * @param end Maximum value for parameter t.
     */
    Ray( const Point3D &origin, const Vector3D &direction, const Ray &parent, double start = 0.f, double end =
           INFINITY );
    /**
     * @brief Operator that makes the ray be used as a parametric function and returns a Point3D at position t along the
     * ray.<br> Given a Ray with origin \f$ O \f$ and direction \f$ \vec{d} \f$, this function returns \f$ P = O +
     *\vec{d} * t \f$.
     * @param t Input parameter \f$ t \f$.
     * @return The resulting point \f$ P \f$.
     */
    Point3D operator()( double t ) const;

  };

  /**
   * @date 2015/Aug/21
   * @brief The BBox class is a Bounding %Box with 8 vertices, and can be represented by 2 points (6coordinates).
   */
  class BBox {
public:
    /* BBox public data */
    Point3D pMin, pMax;

    /* BBox public methods */
    BBox( );

    BBox( const Point3D &p );

    BBox( const Point3D &p1, const Point3D &p2 );

    bool operator==( const BBox &other ) const;

    bool operator!=( const BBox &other ) const;

    BBox Normalized( ) const {
      return( BBox( Point3D( 0, 0, 0 ), Point3D( pMax.x - pMin.x, pMax.y - pMin.y, pMax.z - pMin.z ) ) );
    }

    friend BBox Union( const BBox &b, const Point3D &p );

    friend BBox Union( const BBox &b, const BBox &b2 );

    bool Overlaps( const BBox &b ) const;

    bool Inside( const Point3D &pt ) const;

    void Expand( double delta );

    double SurfaceArea( ) const;

    double Volume( ) const;

    int MaximumExtent( ) const;

    const Point3D &operator[]( int i ) const;

    Point3D &operator[]( int i );

    Point3D at( size_t vtx );

    Point3D Lerp( double tx, double ty, double tz ) const;

    Vector3D Offset( const Point3D &p ) const;

    void BoundingSphere( Point3D *c, double *rad ) const;

  };

  /**
   * @date 2015/Aug/21
   * @brief The Transform3D class holds affine matrices and is used to apply geometric transformations
   * over cartesian coordinates. <br>
   * The affine transformation multiplies the coordinates vector to the affine matrix.
   */
  class Transform3D {
    Matrix< double > affineMatrix, inverseMatrix;
public:
    enum : int { X = 0, Y, Z };

    /* Transform3D public methods */
    Transform3D( );

    Transform3D( const Matrix< double > &mat );

    Transform3D( const Matrix< double > &mat, const Matrix< double > &inv );

    Matrix< double > getAffineMatrix( ) const;

    void setAffineMatrix( const Matrix< double > &mat );

    Matrix< double > getInverseMatrix( ) const;

    void setInverseMatrix( const Matrix< double > &value );

    void reset( );

    Transform3D copy( );

    Transform3D Inverse( );

    bool operator==( const Transform3D &other );

    bool operator!=( const Transform3D &other );

    Transform3D operator*( const Transform3D &other ) const;

    Point3D operator()( const Point3D &pt ) const;

    void operator()( const Point3D &pt, Point3D *res ) const;

    Vector3D operator()( const Vector3D &vec ) const;

    void operator()( const Vector3D &vec, Vector3D *res ) const;

    Normal operator()( const Normal &norm ) const;

    void operator()( const Normal &norm, Normal *res ) const;

    Ray operator()( const Ray &ray ) const;

    void operator()( const Ray &ray, Ray *res ) const;

    BBox operator()( const BBox &b ) const;

    void operator()( const BBox &b, BBox *res ) const;

    Point3D operator()(double x, double y, double z) const;

    Transform3D &Translate( double dx, double dy, double dz = 0.f );

    Transform3D &Scale( double sx, double sy, double sz = 1.f );

    Transform3D &Scale( double factor );

    bool HasScale( ) const;

    Transform3D &Rotate( double degrees, int dms );

    Transform3D &RotateRadians( double rad, int dms );

    Transform3D &Rotate( double degrees, const Vector3D &axis );

    Transform3D &RotateRadians( double rad, const Vector3D &axis );

    Transform3D &Shear( double s_xy, double s_yx, double s_xz, double s_yz, double s_zx, double s_zy );

    Transform3D &LookAt( const Point3D &pos, const Point3D &look, const Vector3D &up );

    bool SwapsHandedness( ) const;
  };

  class Shape;
  /**
   * @date 2015/Aug/21
   * @brief The DifferentialGeometry class is a parametric representation of the Shape geometry.
   */
  class DifferentialGeometry {
public:
    /**
     * @brief p is the point of interest.
     */
    Point3D p;
    /**
     * @brief nn is the normalized version of the surface normal.
     * It's obtained from the cross product between dpdu and dpdv.
     */
    Normal nn;
    /**
     * @brief dpdu is the partial derivative of the surface position in u.
     */
    Vector3D dpdu;
    /**
     * @brief dpdv is the partial derivative of the surface position in v.
     */
    Vector3D dpdv;
    /**
     * @brief dndu is the partial derivative of the surface normal in u.
     * The vectors dndu amd dndv record the differential change in surface normal along the surface.
     */
    Normal dndu;
    /**
     * @brief dndv is the partial derivative of the surface normal in u.
     * The vectors dndu amd dndv record the differential change in surface normal along the surface.
     */
    Normal dndv;
    /**
     * @brief u is a parametric coordinate.
     */
    float u;
    /**
     * @brief v is a parametric coordinate.
     */
    float v;
    /**
     * @brief shape is a reference to the Shape that the differential geometry lies on.
     */
    const Shape *shape;
    /**
     * @brief DifferentialGeometry empty constructor.
     */
    DifferentialGeometry( );
    /**
     * @brief DifferentialGeometry constructor.
     * @param p is the point of interest.
     * @param dpdu is the partial derivative of the surface position in u.
     * @param dpdv is the partial derivative of the surface position in v.
     * @param dndu is the partial derivative of the surface normal in u.
     * @param dndv is the partial derivative of the surface normal in v.
     * @param u is a parametric coordinate.
     * @param v is a parametric coordinate.
     * @param sh is a reference to the Shape that the differential geometry lies on.
     */
    DifferentialGeometry( const Point3D &p,
                          const Vector3D &dpdu,
                          const Vector3D &dpdv,
                          const Normal &dndu,
                          const Normal &dndv,
                          float u,
                          float v,
                          const Shape *sh );
  };

  class FastTransform {
    Matrix< int > affineMatrix, inverseMatrix;
public:
    enum : int { X = 0, Y, Z };

    /* Transform3D public methods */
    FastTransform( );

    FastTransform( const Matrix< int > &mat );

    FastTransform( const Matrix< int > &mat, const Matrix< int > &inv );

    Matrix< int > getAffineMatrix( ) const;

    void setAffineMatrix( const Matrix< int > &mat );

    Matrix< int > getInverseMatrix( ) const;

    void setInverseMatrix( const Matrix< int > &value );

    void reset( );

    FastTransform Inverse( );

    FastTransform copy( );

    bool operator==( const FastTransform &other );

    bool operator!=( const FastTransform &other );

    FastTransform operator*( const FastTransform &other ) const;

    Point3D operator()( double x, double y, double z ) const;

    void operator()(int x, int y, int z, int *xx, int *yy, int *zz) const;

    Point3D operator()( const Point3D &pt ) const;

    void operator()( const Point3D &pt, Point3D *res ) const;

    FastTransform &Translate( int dx, int dy, int dz = 0.f );

    FastTransform &Scale( int sx, int sy, int sz = 1.f );

    FastTransform &Scale( int factor );

    FastTransform &Rotate( double degrees, int dms );

    FastTransform &RotateRadians( double rad, int dms );

    FastTransform &Shear( int s_xy, int s_yx, int s_xz, int s_yz, int s_zx, int s_zy );

  };

  Vector3D operator*( double s, const Vector3D &vec );

  Point3D operator*( double s, const Point3D &p );

  Normal operator*( double s, const Normal &norm );

  double Dot( const Vector3D &v1, const Vector3D &v2 );

  double Dot( const Normal &n1, const Normal &n2 );

  double Dot( const Normal &n1, const Vector3D &v2 );

  double Dot( const Vector3D &v1, const Normal &n2 );

  double AbsDot( const Vector3D &v1, const Vector3D &v2 );

  double AbsDot( const Normal &n1, const Normal &n2 );

  double AbsDot( const Normal &n1, const Vector3D &v2 );

  double AbsDot( const Vector3D &v1, const Normal &n2 );

  Vector3D Cross( const Vector3D &v1, const Vector3D &v2 );

  void CoordinateSystem( const Vector3D &v1, Vector3D *v2, Vector3D *v3 );

  double Distance( const Point3D &p1, const Point3D &p2 );

  double DistanceSquared( const Point3D &p1, const Point3D &p2 );

  Normal FaceForward( const Normal &n, const Vector3D &v );


  /* @param os: output stream.
   * @param vec: a vector.
   * @return Reference to os.
   * @brief Prints the element in a friendly way.
   * @warning none.
   */
  template< class O >
  O &operator<<( O &os, const Vector3D &vec ) {
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return( os );
  }
  template< class O >
  O &operator<<( O &os, const Point3D &pt ) {
    os << "(" << pt.x << ", " << pt.y << ", " << pt.z << ")";
    return( os );
  }
  template< class O >
  O &operator<<( O &os, const Normal &norm ) {
    os << "(" << norm.x << ", " << norm.y << ", " << norm.z << ")";
    return( os );
  }
  template< class O >
  O &operator<<( O &os, const Ray &ray ) {
    os << "[" << ray.o << ", " << ray.d << ", " << ray.mint << ", " << ray.maxt << ", " << ray.depth << "]";
    return( os );
  }
  template< class O >
  O &operator<<( O &os, const BBox &box ) {
    os << "[" << box.pMin << ", " << box.pMax << "]";
    return( os );
  }
  template< class O >
  O &operator<<( O &os, const Transform3D &tr ) {
    os << tr.getAffineMatrix( ) << std::endl << tr.getInverseMatrix( ) << std::endl;
    return( os );
  }
  /** @} */

}

/* Implementation of template functions that are not explicitly instantiated --------------------------------------- **/

namespace Bial {

  template< class D >
  inline Vector3D Vector3D::fromVector( const Vector< D > vec ) {
    if( vec.size( ) != 3 ) {
      throw std::invalid_argument( BIAL_ERROR( "Input vector must have size = 3." ) );
    }
    Vector3D ret;
    ret.x = static_cast< float >( vec[ 0 ] );
    ret.y = static_cast< float >( vec[ 1 ] );
    ret.z = static_cast< float >( vec[ 2 ] );
    return( ret );
  }

}

#include "Geometrics.cpp"

#endif
