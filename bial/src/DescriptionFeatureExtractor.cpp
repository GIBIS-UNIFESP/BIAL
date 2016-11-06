#include "DescriptionFeatureExtractor.hpp"
#include "AdjacencyRound.hpp"
namespace Bial {

  int Log( double value, double n ) {
    value = 255.0 * value / n;
    if( value == 0. ) {
      return( 0 );
    }
    else if( value < 1. ) {
      return( 1 );
    }
    else if( value < 2. ) {
      return( 2 );
    }
    else if( value < 4. ) {
      return( 3 );
    }
    else if( value < 8. ) {
      return( 4 );
    }
    else if( value < 16. ) {
      return( 5 );
    }
    else if( value < 32. ) {
      return( 6 );
    }
    else if( value < 64. ) {
      return( 7 );
    }
    else if( value < 128. ) {
      return( 8 );
    }
    else {
      return( 9 );
    }
  }

  Image< int > Mbb( Image< int > img, Image< int > mask ) {
    Vector< size_t > mins( 2 );
    Vector< size_t > maxs( 2 );

    /* x: index 0, y: index 1 */
    mins[ 0 ] = img.size( 0 ) - 1;
    mins[ 1 ] = img.size( 1 ) - 1;

    maxs[ 0 ] = 0;
    maxs[ 1 ] = 0;
    for( size_t y = 0; y < img.size( 1 ); y++ ) {
      for( size_t x = 0; x < img.size( 0 ); x++ ) {
        if( ( img[ x + img.size( 0 ) * y ] > 0 ) && ( mask( x, y ) != 0 ) ) {
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

  bool ValidContPoint( Image< int > bin, Adjacency L, Adjacency R, int p ) {
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


  Image< int > LabelContPixel( Image< int > img ) { /* MOVER PARA SEGMEN */
    int u_x, u_y, v_x, v_y, w_x, w_y, q, p, left_side, right_side;

    Adjacency A = AdjacencyType::Circular( 1.0f );
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
    A = AdjacencyType::Circular( 1.5f );
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


  double find_angle( int deltax, int deltay ) {
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


  Curve ImageToCurve( Image< int > img, Image< int > mask ) {
    Image< int > contour = LabelContPixel( img );

    Vector< int > order, curve_x, curve_y, curve_z;
    for( size_t i = 0; i < contour.size( ); i++ ) {
      if( ( contour[ i ] != 0 ) && ( mask[ i ] != 0 ) ) {
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
      curve[ i ] = std::make_tuple( curve_x[ i ], curve_y[ i ] );
    }
    return( curve );

  }

  Image< Color > RgbToHmmd( Image< Color > img ) {
    float r, g, b, h, s, d, minimum, maximum, f;
    int k;

    Image< Color > nova( img.size( 0 ), img.size( 1 ) );
    for( int i = 0; i < img.size( ); i++ ) {
      r = ( float ) img[ i ].channel[ 1 ] / 255.0;
      g = ( float ) img[ i ].channel[ 2 ] / 255.0;
      b = ( float ) img[ i ].channel[ 3 ] / 255.0;

      minimum = std::min( r, std::min( g, b ) );
      maximum = std::max( r, std::max( g, b ) );
      if( maximum == minimum ) {
        h = 0.0;
      }
      else {
        f = ( r == minimum ) ? g - b : ( ( g == minimum ) ? b - r : r - g );
        k = ( r == minimum ) ? 3 : ( ( g == minimum ) ? 5 : 1 );

        h = ( ( float ) k - f / ( maximum - minimum ) );
      }
      s = ( maximum + minimum ) / 2;
      d = maximum - minimum;

      nova[ i ].channel[ 1 ] = round( abs( 255. / 6 * h ) );
      nova[ i ].channel[ 2 ] = round( abs( 255. * s ) );
      nova[ i ].channel[ 3 ] = round( abs( 255. * d ) );
    }
    return( nova );
  }


  Image< Color > RgbToHsv( Image< Color > img ) {
    float r, g, b, h, s, v, minimum, maximum, f;
    int k;

    Image< Color > nova( img.size( 0 ), img.size( 1 ) );
    for( int i = 0; i < img.size( ); i++ ) {
      r = ( float ) img[ i ].channel[ 1 ] / 255.0;
      g = ( float ) img[ i ].channel[ 2 ] / 255.0;
      b = ( float ) img[ i ].channel[ 3 ] / 255.0;

      minimum = std::min( r, std::min( g, b ) );
      maximum = std::max( r, std::max( g, b ) );
      if( maximum == minimum ) {
        h = 0.0;
        s = 0.0;
        v = maximum;
      }
      else {
        f = ( r == minimum ) ? g - b : ( ( g == minimum ) ? b - r : r - g );
        k = ( r == minimum ) ? 3 : ( ( g == minimum ) ? 5 : 1 );

        h = ( ( float ) k - f / ( maximum - minimum ) );
        s = ( maximum - minimum ) / maximum;
        v = maximum;
      }
      nova[ i ].channel[ 1 ] = round( abs( 255. / 6 * h ) );
      nova[ i ].channel[ 2 ] = round( abs( 255. * s ) );
      nova[ i ].channel[ 3 ] = round( abs( 255. * v ) );
    }
    return( nova );
  }

}
