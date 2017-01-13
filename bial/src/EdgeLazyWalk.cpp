/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2015/Apr/07 
 * @brief Edge detection with LazyWalk algorithm. 
 */

#ifndef BIALEDGELAZYWALK_C
#define BIALEDGELAZYWALK_C

#include "EdgeLazyWalk.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_EdgeLazyWalk )
#define BIAL_EXPLICIT_EdgeLazyWalk
#endif

#if defined ( BIAL_EXPLICIT_EdgeLazyWalk ) || ( BIAL_IMPLICIT_BIN )

#ifdef BIAL_DEBUG
#include "FileImage.hpp"
#endif
#include "AdjacencyRound.hpp"
#include "FastIncreasingFifoBucketQueue.hpp"
#include "GradientMorphological.hpp"
#include "Image.hpp"
#include "ImageIFT.hpp"
#include "IntensityGlobals.hpp"
#include "MaxPathFunction.hpp"

namespace Bial {

  template< class D >
  std::tuple< Image< D >, Image< int > > Edge::LazyWalk( const Image< D > &img, const Vector< bool > &seed ) {
    COMMENT( "Computing gradient.", 0 );
    Image< D > grad( Gradient::Morphological( img ) );
    COMMENT( "Computing gradient complement.", 0 );
    Intensity::Complement( grad );
    COMMENT( "Computing IFT.", 0 );
    D delta = 1.0;
    Image< D > handicap( grad );
    grad += delta;
    D maximum = grad.Maximum( );
    Image< int > predecessor( img.Dim( ), img.PixelSize( ) );
    MaxPathFunction< Image, D > pf( grad, nullptr, &predecessor, false, handicap );
    Adjacency adj( AdjacencyType::HyperSpheric( 1.9, img.Dims( ) ) );
    size_t size = grad.size( );
    FastIncreasingFifoBucketQueue queue( size, 0, maximum + 1 );
    for( size_t elm = 0; elm < size; ++elm ) {
      if( seed[ elm ] )
        queue.Insert( elm, grad[ elm ] );
      else
        grad( elm ) = std::numeric_limits< D >::max( );
    }
    ImageIFT< D > ift( grad, adj, &pf, &queue );
    ift.Run( );
    COMMENT( "Returning maps.", 0 );
    return( std::tie( grad, predecessor ) );
  }

  template< class D >
  std::tuple< Image< D >, Image< int > > 
  Edge::LazyWalk( const Image< D > &img, const Image< D > &msk, const Vector< bool > &seed ) {
    COMMENT( "Computing gradient.", 0 );
    Image< D > grad = Gradient::Morphological( img );
    COMMENT( "Computing gradient complement.", 0 );
    Intensity::Complement( grad );
    COMMENT( "Computing IFT.", 0 );
    D delta = 1.0;
    Image< D > handicap( grad );
    grad += delta;
    D maximum = grad.Maximum( );
    Image< int > predecessor( img.Dim( ), img.PixelSize( ) );
    MaxPathFunction< Image, D > pf( grad, nullptr, &predecessor, false, handicap );
    Adjacency adj( AdjacencyType::HyperSpheric( 1.9, img.Dims( ) ) );
    size_t size = grad.size( );
    FastIncreasingFifoBucketQueue queue( size, 0, maximum + 1 );
    for( size_t elm = 0; elm < size; ++elm ) {
      if( msk[ elm ] == 0 )
        grad[ elm ] = 0;
      else if( seed[ elm ] )
        queue.Insert( elm, grad[ elm ] );
      else
        grad( elm ) = std::numeric_limits< D >::max( );
    }
    ImageIFT< D > ift( grad, adj, &pf, &queue );
    ift.Run( );
    COMMENT( "Returning maps.", 0 );
    return( std::tie( grad, predecessor ) );
  }

#ifdef BIAL_EXPLICIT_EdgeLazyWalk

  template std::tuple< Image< int >, Image< int > > 
  Edge::LazyWalk( const Image< int > &img, const Vector< bool > &seed );
  template std::tuple< Image< int >, Image< int > > 
  Edge::LazyWalk( const Image< int > &img, const Image< int > &msk, const Vector< bool > &seed );

  template std::tuple< Image< llint >, Image< int > > 
  Edge::LazyWalk( const Image< llint > &img, const Vector< bool > &seed );
  template std::tuple< Image< llint >, Image< int > > 
  Edge::LazyWalk( const Image< llint > &img, const Image< llint > &msk, const Vector< bool > &seed );

  template std::tuple< Image< float >, Image< int > > 
  Edge::LazyWalk( const Image< float > &img, const Vector< bool > &seed );
  template std::tuple< Image< float >, Image< int > > 
  Edge::LazyWalk( const Image< float > &img, const Image< float > &msk, const Vector< bool > &seed );

  template std::tuple< Image< double >, Image< int > > 
  Edge::LazyWalk( const Image< double > &img, const Vector< bool > &seed );
  template std::tuple< Image< double >, Image< int > > 
  Edge::LazyWalk( const Image< double > &img, const Image< double > &msk, const Vector< bool > &seed );

#endif

}

#endif

#endif
