/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2014/Mar/14
 * @brief Functions to draw forms in images.
 */

#include "Color.hpp"
#include "Common.hpp"
#include "DrawFigure.hpp"
#include "Geometrics.hpp"
#include "Vector.hpp"

#ifndef BIALDRAWSHAPE_H
#define BIALDRAWSHAPE_H

namespace Bial {

  template< class D >
  class Image;
  class Adjacency;

  class Shape {
public:
    /* Shape public data */

    /**
     * @brief ObjectToWorld is the object's transformation matrix.
     */
    const Transform3D *ObjectToWorld;
    /**
     * @brief WorldToObject is the inverse object's transformation matrix.
     */
    const Transform3D *WorldToObject;
    /**
     * @brief ReverseOrientation indicates whether their surface normal
     *   directions should be reversed from the default.
     */
    const bool ReverseOrientation;
    /**
     * @brief TransformSwapsHandedness stores the result of the Transform::SwapsHandedness()
     *  call for their object­to­world transformation.
     */
    const bool TransformSwapsHandedness;
    /**
     * @brief shapeId is the Shape's unique id.
     */
    const uint32_t shapeId;
    /**
     * @brief nextShapeId is a static attribute that is incremented at each Shape Construction.
     */
    static uint32_t nextShapeId;

    /** Shape public methods
    **
    * @brief Shape Constructor
    * @param o2w Object to World transform matrix.
    * @param w2o World to Object transform matrix.
    * @param ro indicates if the surface normal should be reversed.
    */
    Shape( const Transform3D *o2w, const Transform3D *w2o, bool ro );
    /**
     * @brief ObjectBound is the shape bounding box.
     * @return
     */
    virtual BBox ObjectBound( ) const = 0;
    /**
     * @brief WorldBound is the Shape's Bounding box transformed by the
     *  ObjectToWorld transform matrix.
     * @return
     */
    virtual BBox WorldBound( ) const;
    /**
     * @brief CanIntersect indicates that a shape can compute ray intersections,
     *  so only shapes that are nonintersectable need to override this method.
     * @return
     */
    virtual bool CanIntersect( ) const;
    /**
     * @brief Refine is a method that splits the shape into a group of new shapes,
     *  some of which may be intersectable and some of which may need further refinement.
     * @param refined
     */
    virtual void Refine( Vector< std::shared_ptr< Shape > > &refined ) const;
    /**
     * @brief Intersect returns geometric information about a single ray­shape intersection
     *  corresponding to the first intersection, if any, in the [mint, maxt] parametric range along the ray.
     * @param ray
     * @param tHit
     * @param rayEpsilon
     * @param dg
     * @return
     */
    virtual bool Intersect( const Ray &ray, float *tHit, float *rayEpsilon, DifferentialGeometry *dg ) const;
    /**
     * @brief IntersectP is a predicate function that determines whether or not an intersection
     *  occurs, without returning any details about the intersection itself.
     * @param ray
     * @return
     */
    virtual bool IntersectP( const Ray &ray ) const;
    /**
     * @brief GetShadingGeometry
     * @param obj2world
     * @param dg
     * @param dgShading
     */
    virtual void GetShadingGeometry( const Transform3D &obj2world,
                                     const DifferentialGeometry &dg,
                                     DifferentialGeometry *dgShading ) const;
    /**
     * @brief Area calculates the surface Area.
     * @return surface area.
     */
    float Area( ) const;
    /**
     * @brief Print assings Shape information to stream.
     * @param os an output stream.
     */
    template< class O >
    void Print( O &os ) const;
  };

  /**
   *  template< class D>
   *  class Texture {
   */

  /**  }; */

  class Triangle;

  class TriangleMesh : public Shape {
    friend class Triangle;
protected:
    /**
     * @brief ntris
     */
    size_t ntris;
    /**
     * @brief nverts
     */
    size_t nverts;
    /**
     * @brief vertexIndex
     */
    size_t *vertexIndex;
    /**
     * @brief p
     */
    Point3D *p;
    /**
     * @brief n
     */
    Normal *n;
    /**
     * @brief s
     */
    Vector3D *s;
    /**
     * @brief uvs
     */
    float *uvs;
    /*
     * @date 2015/Aug/21
     * @brief alphaTexture
     *  std::shared_ptr<Texture<float>> alphaTexture;
     */

public:

    /**
     * @date 2015/Aug/21
     * @brief TriangleMesh Constructor
     * @param o2w Object to World transform matrix.
     * @param w2o World to Object transform matrix.
     * @param ro indicates if the surface normal should be reversed.
     * @param nt is the number of triangles.
     * @param nv is the number of vertices.
     * @param vi is a pointer to an array of vertex indices. For the ith triangle,
     *  its three vertex positions are P[vi[3*i]],P[vi[3*i+1]], and P[vi[3*i+2]];
     * @param p is an array of nv vertex positions.
     * @param n is an optional array of normal vectors, one per vertex in the mesh.
     *  If present, these are interpolated across triangle faces to compute shading
     *  differential geometry.
     * @param s is an optional array of tangent vectors, one per vertex in the mesh.
     *  These are also used to compute shading geometry.
     * @param uv is an optional array of parametric (u,v) values, one for each vetex.
     * @param atex is an optional Alpha Mask texture, which can be used to cut away
     *  part of the triangle's surface.
     */
    TriangleMesh( const Transform3D *o2w,
                  const Transform3D *w2o,
                  bool ro,
                  size_t nt,
                  size_t nv,
                  const size_t *vi,
                  const Point3D *P,
                  const Normal *N = NULL,
                  const Vector3D *S = NULL,
                  const float *uv = NULL );
    /*
     *    TriangleMesh(const Transform3D * o2w, const Transform3D * w2o, bool ro, size_t nt, size_t nv,
     *                 const size_t *vi, const Point3D *P, const Normal *N = NULL, const Vector3D *S = NULL,
     *                 const float *uv = NULL, const std::shared_ptr<Texture<float>> &atex =
     * std::shared_ptr<Texture<float>>());
     */

    /**
     * @date 2015/Aug/21
     * @brief TriangleMesh Destructor.
     */
    ~TriangleMesh( );
    /**
     * @date 2015/Aug/21
     * @brief ObjectBound is the shape bounding box.
     * @return
     */
    virtual BBox ObjectBound( ) const;
    /**
     * @date 2015/Aug/21
     * @brief WorldBound is the Shape's Bounding box transformed by the
     *  ObjectToWorld transform matrix.
     * @return
     */
    virtual BBox WorldBound( ) const;
    /**
     * @date 2015/Aug/21
     * @brief CanIntersect indicates that a shape can compute ray intersections,
     *  so only shapes that are nonintersectable need to override this method.
     * @return
     */
    virtual bool CanIntersect( ) const;
    /**
     * @date 2015/Aug/21
     * @brief Refine is a method that splits the shape into a group of new shapes,
     *  some of which may be intersectable and some of which may need further refinement.
     * @param refined
     */
    virtual void Refine( Vector< std::shared_ptr< Shape > > &refined ) const;
    /**
     * @date 2015/Aug/21
     * @brief Print assings Shape information to stream.
     * @param os an output stream.
     */
    template< class O >
    void Print( O &os ) const;
    /**
     * @date 2015/Aug/21
     * @brief Exports the triange mesh to a ASCII STL file.
     * @param filename: Output stl file name.
     */
    void ExportSTLA( std::string filename ) const;

    /**
     * @date 2015/Aug/21
     * @brief Exports the triange mesh to a Brinary STL file.
     * @param filename: Output stl file name.
     */
    void ExportSTLB( std::string filename ) const;

    /**
     * @date 2015/Aug/21
     * @brief Imports the triangle mesh from a Binary STL file.
     * @param filename: Input stl file name.
     */
    static TriangleMesh* ReadSTLB( std::string filename );

    size_t getNtris() const;
    size_t getNverts() const;
    size_t * getVertexIndex() const;
    Point3D * getP() const;
    Normal * getN() const;
    Vector3D * getS() const;
    float * getUvs() const;
  };

  /**
   * @brief Simple triangle Shape, used in TriangleMesh
   */
  class Triangle : public Shape {
public:
    /**
     * @brief mesh
     */
    std::shared_ptr< TriangleMesh > mesh;
    /**
     * @brief v
     */
    size_t *v;
    /**
     * @brief Triangle Constructor
     * @param o2w Object to World transform matrix.
     * @param w2o World to Object transform matrix.
     * @param ro indicates if the surface normal should be reversed.
     * @param m is a reference to the parent TriangleMesh
     * @param n is the triangle index in the mesh.
     */
    Triangle( const Transform3D *o2w, const Transform3D *w2o, bool ro, TriangleMesh *m, size_t n );
    /**
     * @brief ObjectBound is the shape bounding box.
     * @return
     */
    virtual BBox ObjectBound( ) const;
    /**
     * @brief WorldBound is the Shape's Bounding box transformed by the
     *  ObjectToWorld transform matrix.
     * @return
     */
    virtual BBox WorldBound( ) const;
    /**
     * @brief Intersect returns geometric information about a single ray­shape intersection
     *  corresponding to the first intersection, if any, in the [mint, maxt] parametric range along the ray.
     * @param ray
     * @param tHit
     * @param rayEpsilon
     * @param dg
     * @return
     */
    virtual bool Intersect( const Ray &ray, float *tHit, float *rayEpsilon, DifferentialGeometry *dg ) const;
    /**
     * @brief GetUVS
     * @param uv
     */
    void GetUVs( float uv[ 3 ][ 2 ] ) const;
    /**
     * @brief Area calculates the surface Area.
     * @return surface area.
     */
    float Area( ) const;
    /**
     * @brief GetShadingGeometry
     * @param obj2world
     * @param dg
     * @param dgShading
     */
    virtual void GetShadingGeometry( const Transform3D &obj2world,
                                     const DifferentialGeometry &dg,
                                     DifferentialGeometry *dgShading ) const;

  };

}

/* Implementation ----------------------------------------------------------------------------------------------------
 **/
#include "Adjacency.hpp"
#include "Image.hpp"
#include "Signal.hpp"

namespace Bial {

  template< class O >
  void Shape::Print( O &os ) const {
    try {
      os << "o2w: " << std::endl << *ObjectToWorld
         << "w2o: " << std::endl << *WorldToObject
         << "ro: " << ReverseOrientation << std::endl
         << "tsh: " << TransformSwapsHandedness << std::endl
         << "id: " << shapeId << std::endl;
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
  void TriangleMesh::Print( O &os ) const {
    try {
      os << "Triangle mesh" << std::endl;
      Shape::Print( os );
      os << "ntris: " << ntris << std::endl
         << "nverts: " << nverts << std::endl
         << "vertexIndex: " << vertexIndex << std::endl;
      // os << "p: [";
      // if( p && ( nverts > 0 ) ) {
      //   os << p[ 0 ];
      //   for( size_t i = 1; i < nverts; ++i ) {
      //     os << ", " << p[ i ];
      //   }
      // }
      // os << "]" << std::endl
      //    << "n: [";
      // if( n && ( ntris > 0 ) ) {
      //   os << n[ 0 ];
      //   for( size_t i = 1; i < ntris; ++i ) {
      //     os << ", " << n[ i ];
      //   }
      // }
      // os << "]" << std::endl
      //    << "s: [";
      // if( s && ( nverts > 0 ) ) {
      //   os << s[ 0 ];
      //   for( size_t i = 1; i < nverts; ++i ) {
      //     os << ", " << s[ i ];
      //   }
      // }
      // os << "]" << std::endl
      //    << "uvs: [";
      // if( uvs && ( nverts > 0 ) ) {
      //   os << "(" << uvs[ 0 ] << ", " << uvs[ 1 ] << ")";
      //   for( size_t i = 2; i < nverts * 2; i += 2 ) {
      //     os << "; " << "(" << uvs[ i ] << ", " << uvs[ i + 1 ] << ")";
      //   }
      // }
      // os << "]" << std::endl;
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

#include "DrawShape.cpp"

#endif
