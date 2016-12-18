/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2015/Apr/07 
 * @brief Edge detection with Livewire algorithm. 
 */

#ifndef BIALEDGELIVEWIRE_C
#define BIALEDGELIVEWIRE_C

#include "EdgeLiveWire.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_EdgeLiveWire )
#define BIAL_EXPLICIT_EdgeLiveWire
#endif

#if defined ( BIAL_EXPLICIT_EdgeLiveWire ) || ( BIAL_IMPLICIT_BIN )

#ifdef BIAL_DEBUG
#include "FileImage.hpp"
#endif
#include "AdjacencyRound.hpp"
#include "GradientMorphological.hpp"
#include "Image.hpp"
#include "ImageIFT.hpp"
#include "IntensityGlobals.hpp"
#include "MaxSumPathFunction.hpp"

namespace Bial {

  template< class D > std::tuple< Image< D >, Image< int > > 
  Edge::LiveWire( const Image< D > &img, const Vector< bool > &seed, float weight ) {
    COMMENT( "Computing gradient.", 0 );
    Image< D > grad = Gradient::Morphological( img );
    COMMENT( "Computing gradient complement.", 0 );
    Intensity::Complement( grad );
    size_t size = grad.size( );
    if( weight > 1 ) {
      Image< double > grad_dbl( grad );
      double min = grad.Minimum( );
      double max = grad.Maximum( );
      COMMENT( "Computing new intensities based on beta.", 0 );
      for( size_t elm = 0; elm < size; ++elm )
        grad_dbl[ elm ] = std::pow( grad_dbl[ elm ], weight );
      COMMENT( "Normalizing data into the input range.", 0 );
      grad_dbl.SetRange( min, max );
      grad = grad_dbl;
    }
    D delta = 1;
    MaxSumPathFunction< Image, D > pf( img, grad, 0.0, delta );
    grad += delta;
    COMMENT( "Computing IFT.", 0 );
    COMMENT( "Weight parameter is the control of the lazy-runner. Set it to a value lower than 1.0 for Live-Wire "
             "similarity, or to a value higher than 1.0 for River Bed similarity.", 1 );
    Image< int > predecessor( img.Dim( ), img.PixelSize( ) );
    Adjacency adj( AdjacencyType::HyperSpheric( 1.9, img.Dims( ) ) );
    for( size_t elm = 0; elm < size; ++elm ) {
      if( !seed[ elm ] )
        grad( elm ) = std::numeric_limits< D >::max( );
    }
    ImageIFT< D > ift( grad, adj, &pf, &seed, static_cast< Image< int >* >( nullptr ), &predecessor, false, delta );
    ift.Run( );
    COMMENT( "Returning maps.", 0 );
    return( std::tie( grad, predecessor ) );
  }

  template< class D > std::tuple< Image< D >, Image< int > > 
  Edge::LiveWire( const Image< D > &img, const Image< D > &msk, const Vector< bool > &seed, float weight ) {
    COMMENT( "Computing gradient.", 0 );
    Image< D > grad = Gradient::Morphological( img );
    COMMENT( "Computing gradient complement.", 0 );
    Intensity::Complement( grad );
    DEBUG_WRITE( grad, "grad-ini", 0 );
    size_t size = grad.size( );
    if( weight > 1 ) {
      Image< double > grad_dbl( grad );
      double min = grad.Minimum( );
      double max = grad.Maximum( );
      COMMENT( "Computing new intensities based on beta.", 0 );
      for( size_t elm = 0; elm < size; ++elm )
        grad_dbl[ elm ] = std::pow( grad_dbl[ elm ], weight );
      COMMENT( "Normalizing data into the input range.", 0 );
      grad_dbl.SetRange( min, max );
      grad = grad_dbl;
    }
    DEBUG_WRITE( grad, "grad", 0 );
    D delta = 1;
    MaxSumPathFunction< Image, D > pf( img, grad, 0.0, delta );
    grad += 1;
    COMMENT( "Computing IFT.", 0 );
    COMMENT( "Weight parameter is the control of the lazy-runner. Set it to a value lower than 1.0 for Live-Wire "
             "similarity, or to a value higher than 1.0 for River Bed similarity.", 1 );
    Image< int > predecessor( img.Dim( ), img.PixelSize( ) );
    Adjacency adj( AdjacencyType::HyperSpheric( 1.9, img.Dims( ) ) );
    for( size_t elm = 0; elm < size; ++elm ) {
      if( msk[ elm ] == 0 ) {
        grad[ elm ] = 0;
        //seed[ elm ] = false;
      }
      else if( !seed[ elm ] )
        grad( elm ) = std::numeric_limits< D >::max( );
    }
    ImageIFT< D > ift( grad, adj, &pf, &seed, static_cast< Image< int >* >( nullptr ), &predecessor, false, delta );
    ift.Run( );
    COMMENT( "Returning maps.", 0 );
    return( std::tie( grad, predecessor ) );
  }

#ifdef BIAL_EXPLICIT_EdgeLiveWire

  template std::tuple< Image< int >, Image< int > > Edge::LiveWire( const Image< int > &img,
                                                                    const Vector< bool > &seed, float weight );
  template std::tuple< Image< int >, Image< int > > Edge::LiveWire( const Image< int > &img,
                                                                    const Image< int > &msk,
                                                                    const Vector< bool > &seed, float weight );

  template std::tuple< Image< llint >, Image< int > > Edge::LiveWire( const Image< llint > &img,
                                                                      const Vector< bool > &seed, float weight );
  template std::tuple< Image< llint >, Image< int > > Edge::LiveWire( const Image< llint > &img,
                                                                      const Image< llint > &msk,
                                                                      const Vector< bool > &seed, float weight );

  template std::tuple< Image< float >, Image< int > > Edge::LiveWire( const Image< float > &img,
                                                                      const Vector< bool > &seed, float weight );
  template std::tuple< Image< float >, Image< int > > Edge::LiveWire( const Image< float > &img,
                                                                      const Image< float > &msk,
                                                                      const Vector< bool > &seed, float weight );

  template std::tuple< Image< double >, Image< int > > Edge::LiveWire( const Image< double > &img,
                                                                       const Vector< bool > &seed, float weight );
  template std::tuple< Image< double >, Image< int > > Edge::LiveWire( const Image< double > &img,
                                                                       const Image< double > &msk,
                                                                       const Vector< bool > &seed, float weight );

#endif

}

#endif

#endif
