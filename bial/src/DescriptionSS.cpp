#include "DescriptionSS.hpp"

#include "Adjacency.hpp"
#include "Geometrics.hpp"
#include "Morphology.hpp"
#include "Signal.hpp"
#include "Sorting.hpp"

#include <fstream>
#include <stdlib.h>

#include "File.hpp"
#include "FileImage.hpp"

namespace Bial {

  Image< int > SS::Mbb( Image< int > img ) {
    Vector< size_t > mins( 2 );
    Vector< size_t > maxs( 2 );

    /* x: index 0, y: index 1 */
    mins[ 0 ] = img.size( 0 ) - 1;
    mins[ 1 ] = img.size( 1 ) - 1;

    maxs[ 0 ] = 0;
    maxs[ 1 ] = 0;
    for( size_t y = 0; y < img.size( 1 ); y++ ) {
      for( size_t x = 0; x < img.size( 0 ); x++ ) {
        if( img[ x + img.size( 0 ) * y ] > 0 ) {
          if( x < mins[ 0 ] ) {
            mins[ 0 ] = x;
          }
          if( y < mins[ 1 ] ) {
            mins[ 1 ] = y;
          }
          if( x > maxs[ 0 ] ) {
            maxs[ 0 ] = x;
          }
          if( y > maxs[ 1 ] ) {
            maxs[ 1 ] = y;
          }
        }
      }
    }
    return( img( mins, maxs ) );
  }


  bool SS::ValidContPoint( Image< int > bin, Adjacency L, Adjacency R, int p ) {
    int left_side, right_side;

    int u_x, u_y, v_x, v_y, l_x, l_y, r_x, r_y;

    bool found = false;

    u_x = p % bin.size( 0 );
    u_y = p / bin.size( 0 );
    for( size_t i = 0; i < L.size( ); i++ ) {
      v_x = u_x + L( i, 0 );
      v_y = u_y + L( i, 1 );
      if( bin.ValidCoordinate( v_x, v_y ) ) {
        size_t q = v_x + bin.size( 0 ) * v_y;
        if( ( bin[ q ] == 1 ) && ( p != q ) ) {
          l_x = u_x + L( i, 0 );
          l_y = u_y + L( i, 1 );

          r_x = u_x + R( i, 0 );
          r_y = u_y + R( i, 1 );
          if( bin.ValidCoordinate( l_x, l_y ) ) {
            left_side = l_x + bin.size( 0 ) * l_y;
          }
          else {
            left_side = -1;
          }
          if( bin.ValidCoordinate( r_x, r_y ) ) {
            right_side = r_x + bin.size( 0 ) * r_y;
          }
          else {
            right_side = -1;
          }
          if( ( ( left_side != -1 ) && ( right_side != -1 ) && ( bin[ left_side ] != bin[ right_side ] ) ) ||
              ( ( left_side == -1 ) && ( right_side != -1 ) && ( bin[ right_side ] == 1 ) ) ||
              ( ( right_side == -1 ) && ( left_side != -1 ) && ( bin[ left_side ] == 1 ) ) ) {
            found = true;
            break;
          }
        }
      }
    }
    return( found );
  }


  Image< int > SS::LabelContPixel( Image< int > img ) { /* MOVER PARA SEGMEN */
    int u_x, u_y, v_x, v_y, w_x, w_y, q, p, left_side, right_side;

    Adjacency A = Adjacency::Circular( 1.0f );
    Adjacency::FixAdj( A );

    Image< int > bndr( img.size( 0 ), img.size( 1 ) );
    for( p = 0; p < img.size( ); p++ ) {
      if( img[ p ] == 1 ) {
        u_x = p % img.size( 0 );
        u_y = p / img.size( 0 );
        for( size_t i = 1; i < A.size( ); i++ ) {
          v_x = u_x + A( i, 0 );
          v_y = u_y + A( i, 1 );
          if( img.ValidCoordinate( v_x, v_y ) ) {
            q = v_x + v_y * img.size( 0 );
            if( img[ q ] == 0 ) {
              bndr[ p ] = 1;
              break;
            }
          }
          else {
            bndr[ p ] = 1;
            break;
          }
        }
      }
    }
    A = Adjacency::Circular( 1.5f );
    Adjacency::FixAdj( A );

    Adjacency L = Adjacency::LeftSide( A );
    Adjacency R = Adjacency::RightSide( A );

    Image< int > label( bndr.size( 0 ), bndr.size( 1 ) );
    Image< int > color( bndr.size( 0 ), bndr.size( 1 ) );
    Image< int > pred( bndr.size( 0 ), bndr.size( 1 ) );

    Vector< int > LIFO( bndr.size( ) );


    int last = -1;
    for( size_t j = 0; j < bndr.size( ); j++ ) {
      if( ( bndr[ j ] == 1 ) && ( color[ j ] != BLACK ) && ValidContPoint( img, L, R, j ) ) {
        last++;
        LIFO[ last ] = j;
        color[ j ] = GRAY;
        pred[ j ] = j;
        while( last != -1 ) {
          p = LIFO[ last ];
          last--;
          color[ p ] = BLACK;
          u_x = p % bndr.size( 0 );
          u_y = p / bndr.size( 0 );
          for( size_t i = 1; i < A.size( ); i++ ) {
            v_x = u_x + A( i, 0 );
            v_y = u_y + A( i, 1 );
            if( bndr.ValidCoordinate( v_x, v_y ) ) {
              q = v_x + bndr.size( 0 ) * v_y;
              if( ( q == j ) && ( pred[ p ] != j ) ) {
                last = -1;
                break;
              }
              w_x = u_x + L( i, 0 );
              w_y = u_y + L( i, 1 );
              if( bndr.ValidCoordinate( w_x, w_y ) ) {
                left_side = w_x + bndr.size( 0 ) * w_y;
              }
              else {
                left_side = -1;
              }
              w_x = u_x + R( i, 0 );
              w_y = u_y + R( i, 1 );
              if( bndr.ValidCoordinate( w_x, w_y ) ) {
                right_side = w_x + bndr.size( 0 ) * w_y;
              }
              else {
                right_side = -1;
              }
              if( ( bndr[ q ] == 1 ) && ( color[ q ] != BLACK ) &&
                  ( ( ( left_side != -1 ) && ( right_side != -1 ) && ( img[ left_side ] != img[ right_side ] ) ) ||
                    ( ( left_side == -1 ) && ( right_side != -1 ) && ( img[ right_side ] == 1 ) ) ||
                    ( ( right_side == -1 ) && ( left_side != -1 ) && ( img[ left_side ] == 1 ) ) ) ) {
                pred[ q ] = p;
                if( color[ q ] == WHITE ) {
                  last++;
                  LIFO[ last ] = q;
                  color[ q ] = GRAY;
                }
              }
            }
          }
        }
        int r = p;
        int l = 1;
        while( pred[ p ] != p ) {
          label[ p ] = l;
          p = pred[ p ];
          l++;
        }
        if( r != p ) {
          label[ p ] = l;
        }
      }
    }
    return( label );
  }


  double SS::find_angle( int deltax, int deltay ) {
    double angle;
    double pi;

    pi = 22.0 / 7.0;
    if( ( deltax == 0 ) && ( deltay == 0 ) ) {
      angle = 0.0;
    }
    else {
      angle = atan( ( 10.0 * abs( deltax ) ) / ( 10.0 * abs( deltay ) ) );
      angle = angle * 180.0 / pi;
      if( ( deltax <= 0 ) && ( deltay >= 0 ) ) {
        angle = 360.0 - angle;
      }
      else if( ( deltax <= 0 ) && ( deltay <= 0 ) ) {
        angle = 180.0 + angle;
      }
      else if( ( deltax >= 0 ) && ( deltay <= 0 ) ) {
        angle = 180.0 - angle;
      }
    }
    return( angle );
  }


  Curve SS::ImageToCurve( Image< int > img ) {
    Image< int > contour = LabelContPixel( img );

    Vector< int > order, curve_x, curve_y, curve_z;
    for( size_t i = 0; i < contour.size( ); i++ ) {
      if( contour[ i ] != 0 ) {
        curve_x.push_back( ( int ) ( i % contour.size( 0 ) ) );
        curve_y.push_back( ( int ) ( i / contour.size( 0 ) ) );
        curve_z.push_back( contour[ i ] );
      }
    }
    if( curve_z.size( ) == 0 ) {
      throw "empty image";
    }
    order = Sorting::Sort( curve_z, true );

    Sorting::Sort( curve_x, order );
    Sorting::Sort( curve_y, order );

    Curve curve( order.size( ) );
    for( size_t i = 0; i < order.size( ); i++ ) {
      curve[ i ] = make_tuple( curve_x[ i ], curve_y[ i ] );
    }
    return( curve );
  }

  SS::SS( FeatureDetector< int > *Fd ) : SS( Fd->Run( ) ) {
  }

  SS::SS( Vector < tuple < Image< int >, Image< int >> > detected ) : FeatureExtractor< int, double >( detected ) {
    this->SAMPLES = 120;
    this->MAXDIST = 0;
    this->SIDE = BOTH;
  }

  void SS::SetParameters( ParameterInterpreter *interpreter ) {
    Vector< parameter > vet;
    vet.push_back( tie( "samples", SAMPLES ) );
    vet.push_back( tie( "maxdist", MAXDIST ) );
    vet.push_back( tie( "side", SIDE ) );

    interpreter->SetExpectedParameters( vet );
    vet = interpreter->Interpret( );

    tie( ignore, SAMPLES ) = vet[ 0 ];
    tie( ignore, MAXDIST ) = vet[ 1 ];
    tie( ignore, SIDE ) = vet[ 2 ];
  }

  string SS::GetParameters( ParameterInterpreter *interpreter ) {
    Vector< parameter > vet;
    vet.push_back( tie( "samples", SAMPLES ) );
    vet.push_back( tie( "maxdist", MAXDIST ) );
    vet.push_back( tie( "side", SIDE ) );

    interpreter->SetExpectedParameters( vet );

    return( interpreter->GetExpectedParameters( ) );
  }

  SSfeature SS::Run( ) {
    size_t maxdist;

    SSfeature feat;
    Image< int > img;
    Image< int > mask;

    Features< double > histogram;

    CurveDouble inner;
    CurveDouble outer;
    CurveDouble diff;
    CurveDouble ninner;
    CurveDouble nouter;
    CurveDouble ndiff;
    CurveDouble output;

    Image< int > mbb;
    Image< int > bin;
    Image< int > label;
    for( size_t i = 0; i < detected.size( ); ++i ) {
      tie( img, mask ) = this->detected[ i ];
      if( this->MAXDIST != 0 ) {
        maxdist = this->MAXDIST;
      }
      else {
        maxdist = ( int ) sqrt( get< 0 >( detected[ i ] ).size( 0 ) * get< 0 >( detected[ i ] ).size( 0 ) +
                                get< 0 >( detected[ i ] ).size( 1 ) * get< 0 >( detected[ i ] ).size( 1 ) );
      }
      size_t maxcost = maxdist * maxdist;

      mbb = Mbb( img );

      Vector< size_t > frame_img_coordinates;
      frame_img_coordinates.push_back( maxdist );
      frame_img_coordinates.push_back( maxdist );

      Vector< size_t > frame_img_size;
      frame_img_size.push_back( img.size( 0 ) + 2 * maxdist );
      frame_img_size.push_back( img.size( 1 ) + 2 * maxdist );

      bin = Geometrics::AddFrame( mbb, frame_img_coordinates, frame_img_size );

      label = LabelContPixel( bin );

      /* Compute Euclidean IFT----------------------------------------------------- */

      Image< int > cost( bin.size( 0 ), bin.size( 1 ) );
      for( size_t p = 0; p < bin.size( ); p++ ) {
        if( label[ p ] > 0 ) {
          cost[ p ] = 0;
        }
        else {
          cost[ p ] = INT_MAX;
          label[ p ] = 0;
        }
      }
      Adjacency A = Adjacency::Circular( 1.5 );
      /* bin = Morphology::Dilate(bin,A); */
      label = Morphology::Dilate( label, A );

      Write( bin, "bin.pgm" );
      Write( label, "label.pgm" );
      Write( cost, "cost.pgm" );

      size_t Lmax = 0;
      for( size_t j = 0; j < label.size( ); j++ ) {
        if( Lmax < label[ j ] ) {
          Lmax = label[ j ];
        }
      }
      inner = CurveDouble( Lmax );
      outer = CurveDouble( Lmax );
      diff = CurveDouble( Lmax );
      for( size_t j = 0; j < Lmax; j++ ) {
        get< 0 >( diff[ j ] ) = get< 0 >( inner[ j ] ) = get< 0 >( outer[ j ] ) = ( double ) ( j * SAMPLES ) / Lmax;
      }
      /* -------------------------------------------------------------------------- */

      /* Compute influence areas--------------------------------------------------- */
      double nin, nout;
      nin = nout = 0.0;
      for( size_t p = 0; p < bin.size( ); p++ ) {
        if( ( label[ p ] > 0 ) && ( cost[ p ] <= maxcost ) ) {
          if( bin[ p ] != 0 ) {
            nin++;
            get< 1 >( inner[ label[ p ] - 1 ] )++;
          }
          else {
            nout++;
            get< 1 >( outer[ label[ p ] - 1 ] )++;
          }
        }
      }
      int maxin = INT_MIN;
      int maxout = INT_MIN;
      for( size_t j = 0; j < Lmax; j++ ) {
        if( get< 1 >( inner[ j ] ) > maxin ) {
          maxin = get< 1 >( inner[ j ] );
        }
        if( get< 1 >( outer[ j ] ) > maxout ) {
          maxout = get< 1 >( outer[ j ] );
        }
      }
      for( size_t j = 0; j < Lmax; j++ ) {
        if( nin > 0 ) {
          get< 1 >( inner[ j ] ) /= nin;
        }
        if( nout > 0 ) {
          get< 1 >( outer[ j ] ) /= nout;
        }
        get< 1 >( diff[ j ] ) = get< 1 >( outer[ j ] ) - get< 1 >( inner[ j ] );
      }
      ninner = CurveDouble( SAMPLES );
      nouter = CurveDouble( SAMPLES );
      ndiff = CurveDouble( SAMPLES );
      for( size_t j = 0; j < SAMPLES; j++ ) {
        get< 0 >( ninner[ j ] ) = get< 0 >( nouter[ j ] ) = get< 0 >( ndiff[ j ] ) = j;
      }
      for( size_t j = 0; j < Lmax; j++ ) {
        get< 1 >( ninner[ ( int ) get < 0 > ( inner[ j ] ) ] ) += get< 1 >( inner[ j ] );
        get< 1 >( nouter[ ( int ) get < 0 > ( outer[ j ] ) ] ) += get< 1 >( outer[ j ] );
      }
      for( size_t j = 0; j < SAMPLES; j++ ) {
        get< 1 >( ndiff[ j ] ) = get< 1 >( nouter[ j ] ) - get< 1 >( ninner[ j ] );
      }
      if( this->SIDE == INTERIOR ) {
        output = ninner;
      }
      else if( this->SIDE == EXTERIOR ) {
        output = nouter;
      }
      else if( this->SIDE == BOTH ) {
        output = ndiff;
      }
      else {
        throw "Invalid \"side\" option";
      }
      /* -------------------------------------------------------------------------- */

      histogram = Features< double >( output.size( ) );
      for( size_t j = 0; j < histogram.size( ); j++ ) {
        histogram[ j ] = get< 1 >( output[ j ] );
      }
      feat.push_back( histogram );
    }
    return( feat );
  }

}
