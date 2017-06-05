#include "AdjacencyRound.hpp"
#include "DescriptionFeatureExtractor.hpp"
#include "GeometricsScale.hpp"
#include "ImageResize.hpp"
#include "SortingSort.hpp"

namespace Bial {

  int Log( double value, double n ) {
    try {
      value = 255.0 * value / n;
      if( value == 0. )
        return( 0 );
      else if( value < 1. )
        return( 1 );
      else if( value < 2. )
        return( 2 );
      else if( value < 4. )
        return( 3 );
      else if( value < 8. )
        return( 4 );
      else if( value < 16. )
        return( 5 );
      else if( value < 32. )
        return( 6 );
      else if( value < 64. )
        return( 7 );
      else if( value < 128. )
        return( 8 );
      else
        return( 9 );
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

  Image< int > Mbb( const Image< int > &img, const Image< int > &mask ) {
    try {
      Vector< size_t > mins( 2 );
      Vector< size_t > maxs( 2 );
      COMMENT( "x: index 0, y: index 1", 3 );
      mins[ 0 ] = img.size( 0 ) - 1;
      mins[ 1 ] = img.size( 1 ) - 1;
      maxs[ 0 ] = 0;
      maxs[ 1 ] = 0;
      size_t x_size = img.size( 0 );
      size_t img_size = img.size( );
      for( size_t src_pxl = 0; src_pxl < img_size; ++src_pxl ) {
        if( mask[ src_pxl ] != 0 ) {
          div_t pxl_coords = std::div( static_cast< int >( src_pxl ), static_cast< int >( x_size ) );
          size_t x = pxl_coords.rem;
          size_t y = pxl_coords.quot;
          if( img[ src_pxl ] > 0 ) {
            if( x < mins[ 0 ] )
              mins[ 0 ] = x;
            if( y < mins[ 1 ] )
              mins[ 1 ] = y;
            if( x > maxs[ 0 ] )
              maxs[ 0 ] = x;
            if( y > maxs[ 1 ] )
              maxs[ 1 ] = y;
          }
        }
      }
      return( ImageOp::Resize( img, mins, maxs ) );
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

  Image< int > Mbb( const Image< int > &img ) {
    try {
      Vector< size_t > mins( 2 );
      Vector< size_t > maxs( 2 );
      COMMENT( "x: index 0, y: index 1", 3 );
      mins[ 0 ] = img.size( 0 ) - 1;
      mins[ 1 ] = img.size( 1 ) - 1;
      maxs[ 0 ] = 0;
      maxs[ 1 ] = 0;
      size_t x_size = img.size( 0 );
      size_t y_size = img.size( 1 );
      for( size_t y = 0; y < x_size; ++y ) {
        for( size_t x = 0; x < y_size; ++x ) {
          if( img[ x + x_size * y ] > 0 ) {
            if( x < mins[ 0 ] )
              mins[ 0 ] = x;
            if( y < mins[ 1 ] )
              mins[ 1 ] = y;
            if( x > maxs[ 0 ] )
              maxs[ 0 ] = x;
            if( y > maxs[ 1 ] )
              maxs[ 1 ] = y;
          }
        }
      }
      return( ImageOp::Resize( img, mins, maxs ) );
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

  bool ValidContPoint( const Image< int > &bin, const Adjacency &L, const Adjacency &R, size_t p ) {
    try {
      bool found = false;
      size_t x_size = bin.size( 0 );
      size_t adj_size = L.size( );
      size_t u_x = p % x_size;
      size_t u_y = p / x_size;
      for( size_t i = 0; i < adj_size; ++i ) {
        size_t v_x = u_x + L( i, 0 );
        size_t v_y = u_y + L( i, 1 );
        if( bin.ValidCoordinate( v_x, v_y ) ) {
          size_t q = v_x + x_size * v_y;
          if( ( bin[ q ] == 1 ) && ( p != q ) ) {
            size_t l_x = u_x + L( i, 0 );
            size_t l_y = u_y + L( i, 1 );
            size_t r_x = u_x + R( i, 0 );
            size_t r_y = u_y + R( i, 1 );
            int left_side;
            if( bin.ValidCoordinate( l_x, l_y ) )
              left_side = l_x + x_size * l_y;
            else
              left_side = -1;
            int right_side;
            if( bin.ValidCoordinate( r_x, r_y ) )
              right_side = r_x + x_size * r_y;
            else
              right_side = -1;
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

  Image< int > LabelContPixel( const Image< int > &img ) { /* MOVER PARA SEGMEN */
    try {
      size_t p;
      Adjacency A( AdjacencyType::Circular( 1.0f ) );
      FixAdj( A );
      size_t x_size = img.size( 0 );
      size_t y_size = img.size( 1 );
      size_t img_size = img.size( );
      size_t adj_size = A.size( );
      Image< int > bndr( x_size, y_size );
      for( p = 0; p < img_size; ++p ) {
        if( img[ p ] == 1 ) {
          size_t u_x = p % x_size;
          size_t u_y = p / x_size;
          for( size_t i = 1; i < adj_size; ++i ) {
            size_t v_x = u_x + A( i, 0 );
            size_t v_y = u_y + A( i, 1 );
            if( img.ValidCoordinate( v_x, v_y ) ) {
              size_t q = v_x + v_y * x_size;
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
      FixAdj( A );
      adj_size = A.size( );
      Adjacency L = AdjacencyType::LeftSide( A );
      Adjacency R = AdjacencyType::RightSide( A );
      Image< int > label( x_size, y_size );
      Image< int > color( x_size, y_size );
      Image< int > pred( x_size, y_size );
      Vector< int > LIFO( img_size );
      int last = -1;
      for( size_t j = 0; j < img_size; ++j ) {
        if( ( bndr[ j ] == 1 ) && ( color[ j ] != BLACK ) && ValidContPoint( img, L, R, j ) ) {
          ++last;
          LIFO[ last ] = j;
          color[ j ] = GRAY;
          pred[ j ] = j;
          while( last != -1 ) {
            p = LIFO[ last ];
            --last;
            color[ p ] = BLACK;
            size_t u_x = p % x_size;
            size_t u_y = p / x_size;
            for( size_t i = 1; i < adj_size; ++i ) {
              size_t v_x = u_x + A( i, 0 );
              size_t v_y = u_y + A( i, 1 );
              if( bndr.ValidCoordinate( v_x, v_y ) ) {
                size_t q = v_x + x_size * v_y;
                if( ( q == j ) && ( pred[ p ] != static_cast< int >( j ) ) ) {
                  last = -1;
                  break;
                }
                size_t w_x = u_x + L( i, 0 );
                size_t w_y = u_y + L( i, 1 );
                int left_side;
                if( bndr.ValidCoordinate( w_x, w_y ) )
                  left_side = w_x + x_size * w_y;
                else
                  left_side = -1;
                w_x = u_x + R( i, 0 );
                w_y = u_y + R( i, 1 );
                int right_side;
                if( bndr.ValidCoordinate( w_x, w_y ) )
                  right_side = w_x + x_size * w_y;
                else
                  right_side = -1;
                if( ( bndr[ q ] == 1 ) && ( color[ q ] != BLACK ) &&
                    ( ( ( left_side != -1 ) && ( right_side != -1 ) && ( img[ left_side ] != img[ right_side ] ) ) ||
                      ( ( left_side == -1 ) && ( right_side != -1 ) && ( img[ right_side ] == 1 ) ) ||
                      ( ( right_side == -1 ) && ( left_side != -1 ) && ( img[ left_side ] == 1 ) ) ) ) {
                  pred[ q ] = p;
                  if( color[ q ] == WHITE ) {
                    ++last;
                    LIFO[ last ] = q;
                    color[ q ] = GRAY;
                  }
                }
              }
            }
          }
          int r = p;
          int l = 1;
          while( pred[ p ] != static_cast< int >( p ) ) {
            label[ p ] = l;
            p = pred[ p ];
            ++l;
          }
          if( r != static_cast< int >( p ) )
            label[ p ] = l;
        }
      }
      return( label );
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

  double find_angle( int deltax, int deltay ) {
    try {
      double angle;
      const double pi = 3.141592653589793;
      if( ( deltax == 0 ) && ( deltay == 0 ) )
        angle = 0.0;
      else {
        angle = std::atan( ( 10.0 * std::abs( deltax ) ) / ( 10.0 * std::abs( deltay ) ) );
        angle = angle * 180.0 / pi;
        if( ( deltax <= 0 ) && ( deltay >= 0 ) )
          angle = 360.0 - angle;
        else if( ( deltax <= 0 ) && ( deltay <= 0 ) )
          angle = 180.0 + angle;
        else if( ( deltax >= 0 ) && ( deltay <= 0 ) )
          angle = 180.0 - angle;
      }
      return( angle );
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

  Curve ImageToCurve( const Image< int > &img, const Vector< size_t > &mask ) {
    try {
      Image< int > contour( LabelContPixel( img ) );
      Vector< int > curve_x, curve_y, curve_z;
      size_t mask_size = mask.size( );
      size_t x_size = contour.size( 0 );
      for( size_t pxl_idx = 0; pxl_idx < mask_size; ++pxl_idx ) {
        size_t src_pxl = mask[ pxl_idx ];
        if( contour[ src_pxl ] != 0 ) {
          curve_x.push_back( src_pxl % x_size );
          curve_y.push_back( src_pxl / x_size );
          curve_z.push_back( contour[ src_pxl ] );
        }
      }
      if( curve_z.size( ) == 0 )
        throw "empty image";
      Vector< int > order( Sorting::Sort( curve_z, true ) );
      size_t order_size = order.size( );
      Sorting::Sort( curve_x, order );
      Sorting::Sort( curve_y, order );
      Curve curve( order_size );
      for( size_t i = 0; i < order_size; ++i )
        curve[ i ] = std::make_tuple( curve_x[ i ], curve_y[ i ] );
      return( curve );
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

  Image< Color > RgbToHmmd( const Image< Color > &img ) {
    try {
      Image< Color > nova( img.size( 0 ), img.size( 1 ) );
      size_t img_size = img.size( );
      for( size_t i = 0; i < img_size; ++i ) {
        float r = img[ i ].channel[ 1 ] / 255.0f;
        float g = img[ i ].channel[ 2 ] / 255.0f;
        float b = img[ i ].channel[ 3 ] / 255.0f;
        float minimum = std::min( r, std::min( g, b ) );
        float maximum = std::max( r, std::max( g, b ) );
        float h;
        if( maximum == minimum )
          h = 0.0;
        else {
          float f = ( r == minimum ) ? g - b : ( ( g == minimum ) ? b - r : r - g );
          int k = ( r == minimum ) ? 3 : ( ( g == minimum ) ? 5 : 1 );
          h = k - f / ( maximum - minimum );
        }
        float s = ( maximum + minimum ) / 2;
        float d = maximum - minimum;
        nova[ i ].channel[ 1 ] = std::round( std::abs( 255. / 6 * h ) );
        nova[ i ].channel[ 2 ] = std::round( std::abs( 255. * s ) );
        nova[ i ].channel[ 3 ] = std::round( std::abs( 255. * d ) );
      }
      return( nova );
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

  Image< Color > RgbToHsv( const Image< Color > &img ) {
    try {
      Image< Color > nova( img.size( 0 ), img.size( 1 ) );
      size_t img_size = img.size( );
      for( size_t i = 0; i < img_size; ++i ) {
        float r = img[ i ].channel[ 1 ] / 255.0f;
        float g = img[ i ].channel[ 2 ] / 255.0f;
        float b = img[ i ].channel[ 3 ] / 255.0f;
        float minimum = std::min( r, std::min( g, b ) );
        float maximum = std::max( r, std::max( g, b ) );
        float h = 0.0f;
        float s = 0.0f;
        float v = maximum;
        if( maximum != minimum ) {
          float f = ( r == minimum ) ? g - b : ( ( g == minimum ) ? b - r : r - g );
          int k = ( r == minimum ) ? 3 : ( ( g == minimum ) ? 5 : 1 );
          h = ( ( float ) k - f / ( maximum - minimum ) );
          s = ( maximum - minimum ) / maximum;
          v = maximum;
        }
        nova[ i ].channel[ 1 ] = std::round( std::abs( 255. / 6 * h ) );
        nova[ i ].channel[ 2 ] = std::round( std::abs( 255. * s ) );
        nova[ i ].channel[ 3 ] = std::round( std::abs( 255. * v ) );
      }
      return( nova );
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

  Adjacency FixAdj( const Adjacency &adj ) {
    try {
      Adjacency fixed = adj;
      if( adj.size( ) == 5 ) {
        fixed( 1, 0 ) = -1;
        fixed( 1, 1 ) = 0;
        fixed( 2, 0 ) = 0;
        fixed( 2, 1 ) = -1;
        return( fixed );
      }
      else if( fixed.size( ) == 9 ) {
        fixed( 1, 0 ) = -1; fixed( 1, 1 ) = 0;
        fixed( 2, 0 ) = -1; fixed( 2, 1 ) = -1;
        fixed( 3, 0 ) = 0; fixed( 3, 1 ) = -1;
        fixed( 4, 0 ) = 1; fixed( 4, 1 ) = -1;
        fixed( 5, 0 ) = 1; fixed( 5, 1 ) = 0;
        fixed( 6, 0 ) = 1; fixed( 6, 1 ) = 1;
        fixed( 7, 0 ) = 0; fixed( 7, 1 ) = 1;
        fixed( 8, 0 ) = -1; fixed( 8, 1 ) = 1;
        return( fixed );
      }
      else
        throw std::invalid_argument( "Invalid input Adjacency size, should be 5 or 9." );
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

  namespace AdjacencyType {

    Adjacency AdjacencyBox( int ncols, int nrows ) {
      try {
        if( ncols % 2 == 0 )
          ++ncols;
        if( nrows % 2 == 0 )
          ++nrows;
        Adjacency B( ncols * nrows, 2 );
        size_t i = 1;
        for( int dy = -nrows / 2; dy <= nrows / 2; ++dy ) {
          for( int dx = -ncols / 2; dx <= ncols / 2; ++dx ) {
            if( ( dx != 0 ) || ( dy != 0 ) ) {
              B( i, 0 ) = dx;
              B( i, 1 ) = dy;
              i++;
            }
          }
        }
        COMMENT( "place the central pixel at first", 3 );
        B( 0, 0 ) = 0;
        B( 1, 0 ) = 0;
        return( B );
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

    Adjacency LeftSide( const Adjacency &adj ) {
      try {
        size_t adj_size = adj.size( );
        Adjacency L( adj_size, 2 );
        for( size_t i = 0; i < adj_size; ++i ) {
          float d = std::sqrt( adj( i, 0 ) * adj( i, 0 ) + adj( i, 1 ) * adj( i, 1 ) );
          if( d != 0 ) {
            L( i, 0 ) = std::round( ( adj( i, 0 ) / 2.0f ) + ( adj( i, 1 ) / d ) );
            L( i, 1 ) = std::round( ( adj( i, 1 ) / 2.0f ) - ( adj( i, 0 ) / d ) );
          }
        }
        return( L );
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

    Adjacency RightSide( const Adjacency &adj ) {
      try {
        size_t adj_size = adj.size( );
        Adjacency R( adj_size, 2 );
        for( size_t i = 0; i < adj_size; ++i ) {
          float d = std::sqrt( adj( i, 0 ) * adj( i, 0 ) + adj( i, 1 ) * adj( i, 1 ) );
          if( d != 0 ) {
            if( d != 0 ) {
              R( i, 0 ) = std::round( ( adj( i, 0 ) / 2.0f ) - ( adj( i, 1 ) / d ) );
              R( i, 1 ) = std::round( ( adj( i, 1 ) / 2.0f ) + ( adj( i, 0 ) / d ) );
            }
          }
        }
        return( R );
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

}
