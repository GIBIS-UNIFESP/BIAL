/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @file MarchingCubes.hpp
 * @author Lucas Santana Lellis 
 * @date 2015/Aug/21
 * @version 1.0.00
 * @brief
 * Content: Marching Cubes Algorithm
 * <br> Description: Algorithm proposed by Lorensen and Cline to extract triangle meshes from isosurfaces.
 * <br> Future add-on's: unknown. 
 */

#ifndef MARCHINGCUBES_H
#define MARCHINGCUBES_H

#include "Common.hpp"
#include "DrawShape.hpp"
#include "Geometrics.hpp"
#include "Image.hpp"

namespace Bial {
  /**
   * @brief The Cell class is used in MarchingCubes algorithm to describe a single marching cube data.
   */
  class Cell {

  public:
    /**
     * @brief Position of each cell vertex.
     */
    std::array< Vector3D, 8 > p;
    /**
     * @brief %Intensity of each cell vertex.
     */
    std::array< float, 8 > val;
    /**
     * @brief %Normal of each cell vertex.
     */
    std::array< int, 8 > dir;
    /**
     * @brief %Cell index.
     */
    unsigned char idx;
    /**
     * @brief Calculates the cell index (idx) based on the vertex positions and values. <BR>
     * The cell index can be represented as a 8-bit number, each vertex is represented by one bit.
     * @param Isolevel is used to determine if the vertex is positive or negative.
     */
    void calcIdx( float isolevel );
    /**
     * @brief Simple function to print the index to console.
     */
    void printIdx( );
    /**
     * @brief Function to set the attributes related to each vertex.
     * @param idx %Vertex index ( It's not the cell index )
     * @param v   %Intensity at vertex[idx]
     * @param dir %Normal at vertex[idx]
     * @param pos Position at vertex[idx]
     */
    void set( size_t idx, float v, int dir, const Vector3D &pos );
  };


  /** @brief Algorithm proposed by Lorensen and Cline to extract triangle meshes from isosurfaces. */
  class MarchingCubes {

  public:
    /**
     * @brief triTable
     */
    static char triTable[ 256 ][ 16 ];
    /**
     * @brief edgeTable
     */
    static int edgeTable[ 256 ];
    /**
     * @brief exec
     * @param img
     * @param isolevel
     * @return
     */
    static TriangleMesh* exec( const Image< int > &img, float isolevel );
    /**
     * @brief Binary
     * @param img
     * @param mask
     * @param isolevel
     * @return
     */
    static TriangleMesh* Binary( const Image< int > &img, const Image< int > &mask, float isolevel );
    /**
     * @brief getVertexList
     * @param vertexList
     * @param cell
     * @param isolevel
     */
    static void getVertexList( std::array< Vector3D, 12 > &vertexList, const Cell &cell, float isolevel );
    /**
     * @brief getNormalsList
     * @param normsList
     * @param cell
     * @param isolevel
     */
    static void getNormalsList( std::array< Normal, 12 > &normsList, const Cell &cell, float isolevel );
    /**
     * @brief VertexInterp
     * @param isolevel
     * @param p1
     * @param p2
     * @param valp1
     * @param valp2
     * @return
     */
    static Vector3D VertexInterp( float isolevel, const Vector3D &p1, const Vector3D &p2, float valp1, float valp2 );
    /**
     * @brief NormalInterp
     * @param isolevel
     * @param dir1
     * @param dir2
     * @param valp1
     * @param valp2
     * @return
     */
    static Normal NormalInterp( float isolevel, int dir1, int dir2, float valp1, float valp2 );
    /**
     * @brief getNormal
     * @param dir
     * @return
     */
    static Normal getNormal( int dir );
    /**
     * @brief Polygonize
     * @param cell
     * @param isolevel
     * @param tris
     * @param vertices
     * @param normals
     * @return
     */
    static int Polygonize( const Cell &cell,
                           float isolevel,
                           Vector< size_t > &tris,
                           Vector< Point3D > &vertices,
                           Vector< Normal > &normals );
    /**
     * @brief printEdges
     * @param idx
     */
    static void printEdges( int idx );
    /**
     * @brief printTris
     * @param idx
     */
    static void printTris( int idx );
    /**
     * @brief printVertexList
     * @param vertexList
     */
    static void printVertexList( const std::array< Vector3D, 12 > &vertexList );
  };
}

#include "MarchingCubes.cpp"

#endif /* MARCHINGCUBES_H */
