/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2015/Apr/07 
 * @brief Edge detection with Riverbed algorithm. 
 */

#ifndef BIALEDGERIVERBED_C
#define BIALEDGERIVERBED_C

#include "EdgeRiverBed.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_EdgeRiverBed )
#define BIAL_EXPLICIT_EdgeRiverBed
#endif

#if defined ( BIAL_EXPLICIT_EdgeRiverBed ) || ( BIAL_IMPLICIT_BIN )

#ifdef BIAL_DEBUG
#include "FileImage.hpp"
#endif
#include "AdjacencyRound.hpp"
#include "FastIncreasingFifoBucketQueue.hpp"
#include "GradientMorphological.hpp"
#include "Image.hpp"
#include "ImageIFT.hpp"
#include "IntensityGlobals.hpp"
#include "LocalMaxPathFunction.hpp"

namespace Bial {

  template< class D >
  std::tuple< Image< D >, Image< int > > Edge::RiverBed( const Image< D > &img, const Vector< bool > &seed ) {
    COMMENT( "Computing gradient.", 0 );
    Image< D > grad = Gradient::Morphological( img );
    COMMENT( "Computing gradient complement.", 0 );
    Intensity::Complement( grad );
    Image< int > predecessor( img.Dim( ), img.PixelSize( ) );
    COMMENT( "Computing IFT.", 0 );
    LocalMaxPathFunction< Image, D > pf( grad, nullptr, &predecessor, false );
    size_t size = grad.size( );
    FastIncreasingFifoBucketQueue queue( size, 0, grad.Maximum( ) + 1 );
    Adjacency adj( AdjacencyType::HyperSpheric( 1.1, img.Dims( ) ) );
    ImageIFT< D > ift( grad, adj, &pf, &queue );
    ift.InsertSeeds( seed );
    ift.Run( );
    COMMENT( "Returning maps.", 0 );
    return( std::tie( grad, predecessor ) );
  }

  template< class D >
  std::tuple< Image< D >, Image< int > > 
  Edge::RiverBed( const Image< D > &img, const Image< D > &msk, const Vector< bool > &seed ) {
    COMMENT( "Computing gradient.", 0 );
    Image< D > grad = Gradient::Morphological( img );
    COMMENT( "Computing gradient complement.", 0 );
    Intensity::Complement( grad );
    // DEBUG_WRITE( grad.DataMatrix( ), "grad_rb", 4 );
    Image< int > predecessor( img.Dim( ), img.PixelSize( ) );
    COMMENT( "Computing IFT.", 0 );
    LocalMaxPathFunction< Image, D > pf( grad, nullptr, &predecessor, false );
    size_t size = img.size( );
    FastIncreasingFifoBucketQueue queue( size, 0, grad.Maximum( ) + 1 );
    Adjacency adj( AdjacencyType::HyperSpheric( 1.9, img.Dims( ) ) );
    for( size_t elm = 0; elm < size; ++elm ) {
      if( msk[ elm ] == 0 )
        grad[ elm ] = 0;
    }
    ImageIFT< D > ift( grad, adj, &pf, &queue );
    ift.InsertSeeds( seed );
    ift.Run( );
    COMMENT( "Returning maps.", 0 );
    return( std::tie( grad, predecessor ) );
  }

#ifdef BIAL_EXPLICIT_EdgeRiverBed

  template std::tuple< Image< int >, Image< int > > Edge::RiverBed( const Image< int > &img, 
                                                                    const Vector< bool > &seed );
  template std::tuple< Image< int >, Image< int > > Edge::RiverBed( const Image< int > &img,
                                                                    const Image< int > &msk,
                                                                    const Vector< bool > &seed );

  template std::tuple< Image< llint >, Image< int > > Edge::RiverBed( const Image< llint > &img,
                                                                      const Vector< bool > &seed );
  template std::tuple< Image< llint >, Image< int > > Edge::RiverBed( const Image< llint > &img,
                                                                      const Image< llint > &msk,
                                                                      const Vector< bool > &seed );

  template std::tuple< Image< float >, Image< int > > Edge::RiverBed( const Image< float > &img,
                                                                      const Vector< bool > &seed );
  template std::tuple< Image< float >, Image< int > > Edge::RiverBed( const Image< float > &img,
                                                                      const Image< float > &msk,
                                                                      const Vector< bool > &seed );

  template std::tuple< Image< double >, Image< int > > Edge::RiverBed( const Image< double > &img,
                                                                       const Vector< bool > &seed );
  template std::tuple< Image< double >, Image< int > > Edge::RiverBed( const Image< double > &img,
                                                                       const Image< double > &msk,
                                                                       const Vector< bool > &seed );

#endif

}

#endif

#endif
