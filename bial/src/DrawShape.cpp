/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2014/Mar/14
 * @brief Functions to draw forms in images.
 */

#ifndef BIALDRAWSHAPE_C
#define BIALDRAWSHAPE_C

#include "DrawShape.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_DrawShape )
#define BIAL_EXPLICIT_DrawShape
#endif

#if defined ( BIAL_EXPLICIT_DrawShape ) || ( BIAL_IMPLICIT_BIN )

#include "AdjacencyRound.hpp"
#include "AdjacencyIterator.hpp"
#include "Image.hpp"

namespace Bial {

  /* Shape */
  /* Initializing global variable */
  uint32_t Shape::nextShapeId = 1;

  Shape::Shape( const Transform3D *o2w, const Transform3D *w2o,
                bool ro ) : ObjectToWorld( o2w ), WorldToObject( w2o ), ReverseOrientation( ro ),
    TransformSwapsHandedness(
      o2w->SwapsHandedness( ) ), shapeId( nextShapeId++ ) {
    if( !o2w || !w2o ) {
      throw std::runtime_error( BIAL_ERROR( "Transforms cannot be NULL" ) );
    }
  }

  BBox Shape::WorldBound( ) const {
    return( ( *ObjectToWorld )( ObjectBound( ) ) );
  }

  bool Shape::CanIntersect( ) const {
    return( true );
  }

  void Shape::Refine( Vector< std::shared_ptr< Shape > > &refined ) const {
    throw std::runtime_error( BIAL_ERROR( "Unimplemented method called." ) );
  }

  bool Shape::Intersect( const Ray &ray, float *tHit, float *rayEpsilon, DifferentialGeometry *dg ) const {
    throw std::runtime_error( BIAL_ERROR( "Unimplemented method called." ) );
    return( false );
  }

  bool Shape::IntersectP( const Ray &ray ) const {
    throw std::runtime_error( BIAL_ERROR( "Unimplemented method called." ) );
    return( false );
  }

  void Shape::GetShadingGeometry( const Transform3D &obj2world,
                                  const DifferentialGeometry &dg,
                                  DifferentialGeometry *dgShading ) const {
    *dgShading = dg;
  }

  float Shape::Area( ) const {
    throw std::runtime_error( BIAL_ERROR( "Unimplemented method called." ) );
    return( 0.0 );
  }

  TriangleMesh::TriangleMesh( const Transform3D *o2w, const Transform3D *w2o, bool ro, size_t nt, size_t nv,
                              const size_t *vi, const Point3D *P, const Normal *N, const Vector3D *S, const float *uv ) 
    : Shape( o2w, w2o, ro ), ntris( nt ), nverts( nv ), n( NULL ), s( NULL ), uvs( NULL ) { /*, alphaTexture(atex)*/
    vertexIndex = new size_t[ 3 * ntris ];
    memcpy( vertexIndex, vi, 3 * ntris * sizeof( size_t ) );
    if( N ) {
      n = new Normal[ nverts ];
      memcpy( n, N, nverts * sizeof( Normal ) );
    }
    if( S ) {
      s = new Vector3D[ nverts ];
      memcpy( s, S, nverts * sizeof( Vector3D ) );
    }
    if( uv ) {
      uvs = new float[ 2 * nverts ];
      memcpy( uvs, uv, 2 * nverts * sizeof( float ) );
    }
    p = new Point3D[ nverts ];
    for( size_t i = 0; i < nverts; ++i ) {
      p[ i ] = ( *ObjectToWorld )( P[ i ] );
    }
  }

  TriangleMesh::~TriangleMesh( ) {
    delete[] vertexIndex;
    delete[] n;
    delete[] s;
    delete[] uvs;
    delete[] p;
  }

  BBox TriangleMesh::ObjectBound( ) const {
    BBox objectBounds;
    for( size_t i = 0; i < nverts; ++i ) {
      objectBounds = Union( objectBounds, ( *WorldToObject )( p[ i ] ) );
    }
    return( objectBounds );
  }

  BBox TriangleMesh::WorldBound( ) const {
    BBox worldBounds;
    for( size_t i = 0; i < nverts; ++i ) {
      worldBounds = Union( worldBounds, p[ i ] );
    }
    return( worldBounds );
  }

  bool TriangleMesh::CanIntersect( ) const {
    return( false );
  }

  void TriangleMesh::Refine( Vector< std::shared_ptr< Shape > > &refined ) const {
    for( size_t i = 0; i < nverts; ++i ) {
      refined.push_back( std::shared_ptr< Triangle >( new Triangle( ObjectToWorld, WorldToObject, ReverseOrientation,
                                                                    ( TriangleMesh* ) this, i ) ) );
    }
  }

  Triangle::Triangle( const Transform3D *o2w, const Transform3D *w2o, bool ro, TriangleMesh *m, size_t n ) :
    Shape( o2w, w2o, ro ), mesh( m ) {
    v = &mesh->vertexIndex[ 3 * n ];
  }

  BBox Triangle::ObjectBound( ) const {
    const Point3D &p1 = mesh->p[ v[ 0 ] ];
    const Point3D &p2 = mesh->p[ v[ 1 ] ];
    const Point3D &p3 = mesh->p[ v[ 2 ] ];
    return( Union( BBox( ( *WorldToObject )( p1 ), ( *WorldToObject )( p2 ) ),
                   ( *WorldToObject )( p3 ) ) );
  }

  BBox Triangle::WorldBound( ) const {
    const Point3D &p1 = mesh->p[ v[ 0 ] ];
    const Point3D &p2 = mesh->p[ v[ 1 ] ];
    const Point3D &p3 = mesh->p[ v[ 2 ] ];
    return( Union( BBox( p1, p2 ), p3 ) );
  }

  bool Triangle::Intersect( const Ray &ray, float *tHit, float *rayEpsilon, DifferentialGeometry *dg ) const {
    /* Compute s1. */
    const Point3D &p1 = mesh->p[ v[ 0 ] ];
    const Point3D &p2 = mesh->p[ v[ 1 ] ];
    const Point3D &p3 = mesh->p[ v[ 2 ] ];
    Vector3D e1 = p2 - p1;
    Vector3D e2 = p3 - p1;
    Vector3D s1 = Cross( ray.d, e2 );
    float divisor = Dot( s1, e1 );
    if( divisor == 0 ) {
      return( false );
    }
    float invDivisor = 1.f / divisor;
    /* Compute first baricentric coordinate. */
    Vector3D d = ray.o - p1;
    float b1 = Dot( d, s1 ) * invDivisor;
    if( ( b1 < 0. ) || ( b1 > 1. ) ) {
      return( false );
    }
    /* Compute second baricentric coordinate. */
    Vector3D s2 = Cross( d, e1 );
    float b2 = Dot( ray.d, s2 ) * invDivisor;
    if( ( b2 < 0. ) || ( b2 > 1. ) ) {
      return( false );
    }
    /* Compute t to intersection point. */
    float t = Dot( e2, s2 ) * invDivisor;
    if( ( t < ray.mint ) || ( t > ray.maxt ) ) {
      return( false );
    }
    /* Compute triangle partial derivates. */
    Vector3D dpdu, dpdv;
    float uvs[ 3 ][ 2 ];
    GetUVs( uvs );
    /* Compute deltas for triangle partial derivatives */
    float du1 = uvs[ 0 ][ 0 ] - uvs[ 2 ][ 0 ];
    float du2 = uvs[ 1 ][ 0 ] - uvs[ 2 ][ 0 ];
    float dv1 = uvs[ 0 ][ 1 ] - uvs[ 2 ][ 1 ];
    float dv2 = uvs[ 1 ][ 1 ] - uvs[ 2 ][ 1 ];
    Vector3D dp1 = p1 - p3;
    Vector3D dp2 = p2 - p3;
    float determinant = du1 * dv2 - dv2 * du2;
    if( determinant == 0.f ) {
      /* Handle zero determinant for triangle partial derivative matrix. */
      CoordinateSystem( Cross( e2, e1 ).Normalized( ), &dpdu, &dpdv );
    }
    else {
      float invdet = 1.f / determinant;
      dpdu = ( dv2 * dp1 - dv1 * dp2 ) * invdet;
      dpdv = ( -du2 * dp1 + du1 * dp2 ) * invdet;
    }
    /* Interpolate (u,v) triangle parametric coordinates. */
    float b0 = 1 - b1 - b2;
    float tu = b0 * uvs[ 0 ][ 0 ] + b1 * uvs[ 1 ][ 0 ] + b2 * uvs[ 2 ][ 0 ];
    float tv = b0 * uvs[ 0 ][ 1 ] + b1 * uvs[ 1 ][ 1 ] + b2 * uvs[ 2 ][ 1 ];
    /* Test intersection against alpha texture, if present. */
/*
 *    if(mesh->alphaTexture) {
 *      DifferentialGeometry dgLocal(ray(t),dpdu,dpdv, Normal(0,0,0), Normal(0,0,0),
 *                                   tu, tv, this);
 *      if(mesh->alphaTexture->Evaluate(dgLocal) == 0.0f){
 *        return false;
 *      }
 *    }
 */
    /* Fill in DifferentialGeometry from triangle hit */
    *dg = DifferentialGeometry( ray( t ), dpdu, dpdv, Normal( 0, 0, 0 ),
                                Normal( 0, 0, 0 ), tu, tv, this );
    *tHit = t;
    *rayEpsilon = 1e-3f * ( *tHit );
    return( true );
  }

  void Triangle::GetUVs( float uv[ 3 ][ 2 ] ) const {
    if( mesh->uvs ) {
      uv[ 0 ][ 0 ] = mesh->uvs[ 2 * v[ 0 ] ];
      uv[ 0 ][ 1 ] = mesh->uvs[ 2 * v[ 0 ] + 1 ];
      uv[ 1 ][ 0 ] = mesh->uvs[ 2 * v[ 1 ] ];
      uv[ 1 ][ 1 ] = mesh->uvs[ 2 * v[ 1 ] + 1 ];
      uv[ 2 ][ 0 ] = mesh->uvs[ 2 * v[ 2 ] ];
      uv[ 2 ][ 1 ] = mesh->uvs[ 2 * v[ 2 ] + 1 ];
    }
    else {
      uv[ 0 ][ 0 ] = 0.;
      uv[ 0 ][ 1 ] = 0.;
      uv[ 1 ][ 0 ] = 1.;
      uv[ 1 ][ 1 ] = 0.;
      uv[ 2 ][ 0 ] = 1.;
      uv[ 2 ][ 1 ] = 1.;
    }
  }

  float Triangle::Area( ) const {
    const Point3D &p1 = mesh->p[ v[ 0 ] ];
    const Point3D &p2 = mesh->p[ v[ 1 ] ];
    const Point3D &p3 = mesh->p[ v[ 2 ] ];
    return( 0.5f * Cross( p2 - p1, p3 - p1 ).Length( ) );
  }

  void Triangle::GetShadingGeometry( const Transform3D &obj2world,
                                     const DifferentialGeometry &dg,
                                     DifferentialGeometry *dgShading ) const {
    if( !mesh->n && !mesh->s ) {
      *dgShading = dg;
      return;
    }
    /*
     * Initialize Triangle shading geometry with n and s
     * TODO - Terminar - PDF Cap. 3 página 40.
     */
  }

  void TriangleMesh::ExportSTLA( std::string filename ) const {
    try {
      OFile file;
      file.open( filename );
      file << "solid triangle mesh" << std::endl;
      for( size_t facet = 0; facet < ntris; ++facet ) {
        /* FIXME : Interpolate triangle normal. */
        Normal norm;
        if( n ) {
          for( int vtx = 0; vtx < 3; ++vtx ) {
            const Normal &vtxNorm = n[ vertexIndex[ 3 * facet + vtx ] ];
            norm += vtxNorm;
          }
          norm /= 3.0;
        }
        else {
          const Point3D &p1 = p[ vertexIndex[ 3 * facet ] ];
          const Point3D &p2 = p[ vertexIndex[ 3 * facet + 1 ] ];
          const Point3D &p3 = p[ vertexIndex[ 3 * facet + 2 ] ];
          Vector3D cross = Cross( p2 - p1, p3 - p1 );
          norm = Normal( cross.x, cross.y, cross.z );
        }
        file << "  facet normal " << norm.x << " " << norm.y << " " << norm.x << std::endl;
        file << "    outer loop" << std::endl;
        for( int vtx = 0; vtx < 3; ++vtx ) {
          const Point3D &vertex = p[ vertexIndex[ 3 * facet + vtx ] ];
          file << "      vertex " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
        }
        file << "    endloop" << std::endl;
        file << "  endfacet" << std::endl;
      }
      file << "endsolid" << std::endl;
      file.close();
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

  void TriangleMesh::ExportSTLB( std::string filename ) const {
    try {
      OFile file;
      file.open( filename, std::ofstream::binary );
      /* Binary STL file header */
      char text[ 80 ] = "Binary STL File - Biomedical Image Analysis Library";
      file.write( reinterpret_cast< const char* >( &text[ 0 ] ), sizeof( text ) );
      /* Writing UINT32 number to describe the number of triangles */
      uint32_t nTriangles = ntris;
      file.write( reinterpret_cast< const char* >( &nTriangles ), sizeof( nTriangles ) );
      /* Writing triangles*/
      for( size_t facet = 0; facet < ntris; ++facet ) {
        /* Calculating facet normal */
        Normal norm;
        if( n ) {
          for( int vtx = 0; vtx < 3; ++vtx ) {
            const Normal &vtxNorm = n[ vertexIndex[ 3 * facet + vtx ] ];
            norm += vtxNorm;
          }
          norm /= 3.0;
        }
        else {
          const Point3D &p1 = p[ vertexIndex[ 3 * facet ] ];
          const Point3D &p2 = p[ vertexIndex[ 3 * facet + 1 ] ];
          const Point3D &p3 = p[ vertexIndex[ 3 * facet + 2 ] ];
          Vector3D cross = Cross( p2 - p1, p3 - p1 );
          norm = Normal( cross.x, cross.y, cross.z );
        }
        float vec[ 3 ];
        /*        REAL32[3] – Normal vector */
        vec[ 0 ] = static_cast< float >( norm.x );
        vec[ 1 ] = static_cast< float >( norm.y );
        vec[ 2 ] = static_cast< float >( norm.z );
        file.write( reinterpret_cast< const char* >( &vec[ 0 ] ), sizeof( vec ) );
        /*        REAL32[3] – Vertex 1, 2 and 3 */
        for( int vtx = 0; vtx < 3; ++vtx ) {
          const Point3D &vertex = p[ vertexIndex[ 3 * facet + vtx ] ];
          vec[ 0 ] = static_cast< float >( vertex.x );
          vec[ 1 ] = static_cast< float >( vertex.y );
          vec[ 2 ] = static_cast< float >( vertex.z );
          file.write( reinterpret_cast< const char* >( &vec[ 0 ] ), sizeof( vec ) );
        }
        /*        UINT16 – Attribute byte count*/
        uint16_t abc = 0;
        file.write( reinterpret_cast< const char* >( &abc ), sizeof( abc ) );
      }
      file.close();
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


  TriangleMesh* TriangleMesh::ReadSTLB( std::string filename ) {
    try {
      IFile file;
      file.exceptions( std::fstream::failbit | std::fstream::badbit );
      file.open( filename );

      char text[ 80 ];
      /* Binary STL file header */
      file.read( reinterpret_cast< char* >( &( text[ 0 ] ) ), sizeof( text ) );
      /* Reading UINT32 number to describe the number of triangles */
      uint32_t nTriangles;
      file.read( reinterpret_cast< char* >( &nTriangles ), sizeof( nTriangles ) );
      Vector< Point3D > vertices( nTriangles * 3 );
      Vector< Normal > normals( nTriangles * 3 );
      Vector< size_t > tris( nTriangles * 3 );
      /* Reading triangles */
      for( size_t facet = 0; facet < nTriangles; ++facet ) {
        float norm[3], vec[ 3 ];
        /*        REAL32[3] – Normal vector */
        file.read( reinterpret_cast< char* >( &( vec[ 0 ] ) ), sizeof( vec ) );
        /*        REAL32[3] – Vertex 1, 2 and 3 */
        for( int vtx = 0; vtx < 3; ++vtx ) {
          normals[ facet * 3 + vtx ] = Normal( norm[ 0 ], norm[ 1 ], norm[ 2 ] );
          file.read( reinterpret_cast< char* >( &( vec[ 0 ] ) ), sizeof( vec ) );
          vertices[ facet * 3 + vtx ] = Point3D( vec[ 0 ], vec[ 1 ], vec[ 2 ] );
          tris[ facet * 3 + vtx ] = facet * 3 + vtx;
        }
        /*        UINT16 – Attribute byte count*/
        uint16_t abc;
        file.read( reinterpret_cast< char* >( &abc ), sizeof( abc ) );
      }

      file.close( );
      return( new TriangleMesh( new Transform3D( ), new Transform3D( ),
                                false, tris.size( ) / 3, vertices.size( ), &tris[ 0 ], &vertices[ 0 ] ) );
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

  size_t TriangleMesh::getNtris() const {
    return ntris;
  }

  size_t TriangleMesh::getNverts() const {
    return nverts;
  }

  size_t * TriangleMesh::getVertexIndex() const {
    return vertexIndex;
  }

  Point3D * TriangleMesh::getP() const {
    return p;
  }

  Normal * TriangleMesh::getN() const {
    return n;
  }

  Vector3D * TriangleMesh::getS() const {
    return s;
  }

  float * TriangleMesh::getUvs() const {
    return uvs;
  }

#ifdef BIAL_EXPLICIT_DrawShape

#endif

}

#endif

#endif
