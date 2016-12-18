#include "DescriptionTAR.hpp"

#include "Geometrics.hpp"
#include "Signal.hpp"
#include "SortingSort.hpp"
#include "GeometricsScale.hpp"
#include <fstream>
#include <stdlib.h>

namespace Bial {


  Vector< double > TAR::ComputeScale( Curve contour, int n, int ts ) {
    Vector< double > tar( n );
    for( size_t i = 0; i < tar.size( ); i++ ) {
      int l = ( n + i - ts ) % n;
      int r = ( i + ts ) % n;

      tar[ i ] = 0.5 *
                 ( get< 0 >( contour[ l ] ) * get< 1 >( contour[ i ] ) + get< 0 >( contour[ i ] ) *
                   get< 1 >( contour[ r ] )
                   + get< 0 >( contour[ r ] ) * get< 1 >( contour[ l ] ) -
                   get< 0 >( contour[ r ] ) * get< 1 >( contour[ i ] ) - get< 0 >( contour[ i ] ) *
                   get< 1 >( contour[ l ] )
                   - get< 0 >( contour[ l ] ) * get< 1 >( contour[ r ] ) );
    }
    return( tar );
  }


  Vector< Vector< double > > TAR::ComputeSignature( Curve contour, int n ) {
    int nscales = ( n - 1 ) / 2;

    Vector< Vector< double > > tar( nscales );
    for( size_t ts = 0; ts < static_cast< size_t >( nscales ); ts++ ) {
      Vector< double > scale = ComputeScale( contour, n, ts + 1 );

      Vector< double > abs_scale( scale );
      abs_scale.Abs( );

      double max = abs_scale.Maximum( );
      if( max != 0 ) {
        scale /= max;
      }
      else {
        throw "divisao por zero";
      }
      tar[ ts ] = scale;

    }
    return( tar );
  }

  double TAR::ShapeComplexity( Vector< Vector< double > > tar, int samples ) {
    double sc = 0.0;

    int nscales = ( samples - 1 ) / 2;

    Vector< Vector< double > > reverse_tar( samples );
    for( size_t i = 0; i < static_cast< size_t >( samples ); i++ ) {
      for( size_t ts = 0; ts < static_cast< size_t >( nscales ); ts++ ) {
        reverse_tar[ i ].push_back( tar[ ts ][ i ] );
      }
    }
    for( size_t i = 0; i < static_cast< size_t >( samples ); i++ ) {
      double max = reverse_tar[ i ].Maximum( );
      double min = reverse_tar[ i ].Minimum( );

      sc += fabs( max - min );
    }
    sc /= samples;

    return( sc );
  }

  TAR::TAR( FeatureDetector< int > *Fd ) : TAR( Fd->Run( ) ) {
  }

  TAR::TAR( Vector < std::tuple < Image< int >, Image< int >> > detected ) : FeatureExtractor< int, double >( detected ) {
    this->SAMPLES = 128;
  }

  void TAR::SetParameters( ParameterInterpreter *interpreter ) {
    Vector< parameter > vet;
    vet.push_back( std::tie( "SAMPLES", SAMPLES ) );

    interpreter->SetExpectedParameters( vet );

    vet = interpreter->Interpret( );

    std::tie( std::ignore, SAMPLES ) = vet[ 0 ];

  }

  std::string TAR::GetParameters( ParameterInterpreter *interpreter ) {
    Vector< parameter > vet;

    vet.push_back( std::tie( "SAMPLES", SAMPLES ) );

    interpreter->SetExpectedParameters( vet );

    return( interpreter->GetExpectedParameters( ) );
  }

  TARfeature TAR::Run( ) {
    TARfeature feat;
    Image< int > img;
    Image< int > mask;

    Features< double > histogram;

    Curve contour;
    for( size_t i = 0; i < this->detected.size( ); ++i ) {
      std::tie( img, mask ) = this->detected[ i ];

      contour = ImageToCurve( img, mask );

      /* Resample------------------------------------------------------------------ */

      Image< int > img1( contour.size( ), 2 );
      for( size_t x = 0; x < contour.size( ); x++ ) {
        std::tie( img1[ x ], img1[ x + img1.size( 0 ) ] ) = contour[ x ];
      }
      Image< int > img2 = Geometrics::Scale( img1, ( ( ( float ) SAMPLES ) / contour.size( ) ), 1.0, true );

      Curve rcontour( SAMPLES );
      for( size_t x = 0; x < SAMPLES; x++ ) {
        rcontour[ x ] = std::tie( img2[ x ], img2[ x + img1.size( 0 ) ] );
      }
      /* -------------------------------------------------------------------------- */

      Vector< Vector< double > > tar = ComputeSignature( rcontour, SAMPLES );
      double sc = ShapeComplexity( tar, SAMPLES );

      /* Histogram----------------------------------------------------------------- */
      histogram = Features< double >( tar.size( ) * SAMPLES + 1 );

      size_t p = 0;
      for( size_t ts = 0; ts < tar.size( ); ts++ ) {
        for( size_t j = 0; j < tar[ ts ].size( ); j++ ) {
          histogram[ p++ ] = tar[ ts ][ j ];
        }
      }
      histogram[ p ] = sc;
      /* -------------------------------------------------------------------------- */

      feat.push_back( histogram );

    }
    return( feat );
  }

}
