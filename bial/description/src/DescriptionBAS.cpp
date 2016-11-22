#include "DescriptionBAS.hpp"

#include "Adjacency.hpp"
#include "GeometricsScale.hpp"
#include "Signal.hpp"

#include <fstream>
#include <stdlib.h>

namespace Bial {

  BAS::BAS( FeatureDetector< int > *Fd ) : BAS( Fd->Run( ) ) {
  }

  BAS::BAS( Vector < std::tuple < Image< int >, Image< int >> > detected ) : FeatureExtractor< int, double >( detected ) {
    this->SAMPLES = 40;
  }

  void BAS::SetParameters( ParameterInterpreter *interpreter ) {
    Vector< parameter > vet;
    vet.push_back( std::tie( "SAMPLES", SAMPLES ) );

    interpreter->SetExpectedParameters( vet );

    vet = interpreter->Interpret( );

    std::tie( std::ignore, SAMPLES ) = vet[ 0 ];

  }

  std::string BAS::GetParameters( ParameterInterpreter *interpreter ) {
    Vector< parameter > vet;

    vet.push_back( std::tie( "SAMPLES", SAMPLES ) );

    interpreter->SetExpectedParameters( vet );

    return( interpreter->GetExpectedParameters( ) );
  }

  BASfeature BAS::Run( ) {
    BASfeature feat;
    Image< int > img;
    Image< int > mask;

    Features< double > histogram;

    Curve contour, curve, rcurve;
    for( size_t i = 0; i < this->detected.size( ); ++i ) {
      std::tie( img, mask ) = this->detected[ i ];

      contour = ImageToCurve( img, mask );

      /* Extract Features---------------------------------------------------------- */
      int x1, x2, x3, y1, y2, y3, curvelength;
      double angle_1, angle_2, angle_curve, total, previous;
      int delta_x, delta_y;

      Vector< int > mean( contour.size( ) );
      Vector< int > second( contour.size( ) );
      Vector< int > third( contour.size( ) );

      curvelength = ( int ) ( contour.size( ) / 2 );

      Vector< int > bearing_array( curvelength - 1 );
      for( size_t j = 0; j < contour.size( ); j++ ) {
        total = 0.0;
        std::tie( x1, y1 ) = contour[ ( ( j - 1 ) + contour.size( ) ) % contour.size( ) ];
        std::tie( x2, y2 ) = contour[ j ];
        std::tie( x3, y3 ) = contour[ ( ( j + 1 ) + contour.size( ) ) % contour.size( ) ];

        delta_x = x1 - x2;
        delta_y = -( y1 - y2 );
        angle_1 = find_angle( delta_x, delta_y );
        delta_x = x3 - x2;
        delta_y = -( y3 - y2 );
        angle_2 = find_angle( delta_x, delta_y );
        if( angle_1 >= angle_2 ) {
          angle_curve = angle_1 - angle_2;
        }
        else {
          angle_curve = 360.0 + angle_1 - angle_2;
        }
        total += angle_curve;

        bearing_array[ 0 ] = ( int ) angle_curve;

        previous = angle_curve;
        for( size_t k = 2; k < curvelength; k++ ) {
          std::tie( x1, y1 ) = contour[ ( ( j - k ) + contour.size( ) ) % contour.size( ) ];
          std::tie( x2, y2 ) = contour[ j ];
          std::tie( x3, y3 ) = contour[ ( ( j + k ) + contour.size( ) ) % contour.size( ) ];

          delta_x = x1 - x2;
          delta_y = -( y1 - y2 );
          angle_1 = find_angle( delta_x, delta_y );
          delta_x = x3 - x2;
          delta_y = -( y3 - y2 );
          angle_2 = find_angle( delta_x, delta_y );
          if( angle_1 >= angle_2 ) {
            angle_curve = angle_1 - angle_2;
          }
          else {
            angle_curve = 360.0 + angle_1 - angle_2;
          }
          if( k > 3 ) {
            if( ( ( angle_curve - previous ) > 180 ) ) {
              angle_curve = angle_curve - 360.0;
            }
            else if( ( ( previous - angle_curve ) > 180 ) ) {
              angle_curve = angle_curve + 360.0;
            }
          }
          bearing_array[ k - 1 ] = ( int ) angle_curve;
          total += angle_curve;
          previous = angle_curve;
        }
        mean[ j ] = ( int ) ( total / ( double ) ( curvelength - 1 ) );

        total = 0.0;
        for( size_t k = 0; k < curvelength - 1; k++ ) {
          total += pow( ( bearing_array[ k ] - mean[ j ] ), 2.0 );
        }
        second[ j ] = pow( total / ( double ) ( curvelength - 2 ), 0.5 );

        total = 0.0;
        for( size_t k = 0; k < curvelength - 1; k++ ) {
          total += pow( abs( bearing_array[ k ] - mean[ j ] ), 3.0 );
        }
        third[ j ] = pow( total / ( double ) ( curvelength - 2 ), ( 1.0 / 3.0 ) );

      }
      /* -------------------------------------------------------------------------- */


      /* Resample------------------------------------------------------------------ */

      Image< int > img1( mean.size( ), 3 );
      for( size_t x = 0; x < mean.size( ); x++ ) {
        img1[ x ] = mean[ x ];
        img1[ x + img1.size( 0 ) * 1 ] = second[ x ];
        img1[ x + img1.size( 0 ) * 2 ] = third[ x ];
      }
      Image< int > img2 = Geometrics::Scale( img1, ( ( ( float ) SAMPLES ) / mean.size( ) ), 1, 1, true );

      /* histogram----------------------------------------------------------------- */
      histogram = Features< double >( img2.size( ) );
      for( size_t j = 0; j < img2.size( ); j++ ) {
        histogram[ j ] = img2[ j ];
      }
      /* -------------------------------------------------------------------------- */

      feat.push_back( histogram );
    }
    return( feat );
  }
}
