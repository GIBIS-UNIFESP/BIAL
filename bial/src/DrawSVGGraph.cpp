/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2014/Mar/14
 * @brief Functions to draw graphs in SVG file format.
 */

#ifndef BIALDRAWSVGGRAPH_C
#define BIALDRAWSVGGRAPH_C

#include "DrawSVGGraph.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_DrawSVGGraph )
#define BIAL_EXPLICIT_DrawSVGGraph
#endif

#if defined ( BIAL_EXPLICIT_DrawSVGGraph ) || ( BIAL_IMPLICIT_BIN )

#include "AdjacencyIterator.hpp"
#include "Image.hpp"

namespace Bial {

  template< class D >
  void SVGDGraph( const Image< D > &vertex, const Adjacency &adj, const std::string &filename ) {
    if( vertex.Dims( ) != 2 ) {
      std::string msg( BIAL_ERROR( "Vertex image must have 2 dimensions. Given: " +
                                   std::to_string( vertex.Dims( ) ) ) );
      throw( std::logic_error( msg ) );
    }
    COMMENT( "Opening file.", 2 );
    OFile file;
    file.exceptions( std::fstream::failbit | std::fstream::badbit );
    file.open( filename );
    COMMENT( "Writing header.", 2 );
    file << "<svg version=\"1.1\">" << std::endl;

    COMMENT( "Creating adjacency.", 2 );
    for( size_t y = 0; y < vertex.size( 1 ); ++y ) {
      for( size_t x = 0; x < vertex.size( 0 ); ++x ) {
        size_t pxl = vertex.Position( x, y );
        size_t x_pos = ( x + 1 ) * 10;
        size_t y_pos = ( y + 1 ) * 10;
        file << "  <defs>" << std::endl;
        file << "    <marker id='head' orient='auto' markerWidth='2' markerHeight='4' refX='0.1' refY='1'>" << std::endl;
        file << "      <path d='M0,0 V2 L2,1 Z' fill='black' />" << std::endl;
        file << "    </marker>" << std::endl;
        file << "  </defs>" << std::endl;
        for( AdjacencyIterator itr = begin( adj, vertex, pxl ); *itr < vertex.size( ); ++itr ) {
          size_t adj_pxl = *itr;
          if( adj_pxl != pxl ) {
            size_t x_pos_adj = x_pos + adj( itr.Position( ), 0 ) * 10;
            size_t y_pos_adj = y_pos + adj( itr.Position( ), 1 ) * 10;
            COMMENT( "Moving arrow displacement.", 5 );
            if( adj( itr.Position( ), 0 ) > 0 )
              x_pos_adj -= 2;
            else if( adj( itr.Position( ), 0 ) < 0 )
              x_pos_adj += 2;
            if( adj( itr.Position( ), 1 ) > 0 )
              y_pos_adj -= 2;
            else if( adj( itr.Position( ), 1 ) < 0 )
              y_pos_adj += 2;
            file << "<path marker-end='url(#head)' stroke-width='1' fill='none' stroke='black' d='M"
                 << x_pos << "," << y_pos << " " << x_pos_adj << "," << y_pos_adj <<"' />" << std::endl;
          }
        }
      }
    }

    COMMENT( "Creating Graph", 2 );
    for( size_t y = 0; y < vertex.size( 1 ); ++y ) {
      for( size_t x = 0; x < vertex.size( 0 ); ++x ) {
        size_t x_pos = ( x + 1 ) * 10;
        size_t y_pos = ( y + 1 ) * 10;
        file << "  <circle cx=\"" << x_pos << "\" cy=\"" << y_pos << "\" r=\"1\" fill=\"black\" />" << std::endl;
        if( vertex( x, y ) < 10 )
          file << "  <text fill=\"gray\" x=\"" << x_pos - 3 << "\" y=\"" << y_pos - 2 << "\" font-size=\"4\">"
               << vertex( x, y ) << "</text>" << std::endl;
        else if( vertex( x, y ) < 100 )
          file << "  <text fill=\"gray\" x=\"" << x_pos - 6 << "\" y=\"" << y_pos - 2 << "\" font-size=\"4\">"
               << vertex( x, y ) << "</text>" << std::endl;
        else
          file << "  <text fill=\"gray\" x=\"" << x_pos - 8 << "\" y=\"" << y_pos - 2 << "\" font-size=\"4\">"
               << vertex( x, y ) << "</text>" << std::endl;
      }
    }
    file << "</svg>" << std::endl;
    file.close( );
  }

  template< class D >
  void SVGDGraph( const Image< D > &vertex, const Vector< bool > &seed, const Adjacency &adj,
                  const std::string &filename ) {
    if( vertex.Dims( ) != 2 ) {
      std::string msg( BIAL_ERROR( "Vertex image must have 2 dimensions. Given: " +
                                   std::to_string( vertex.Dims( ) ) ) );
      throw( std::logic_error( msg ) );
    }
    COMMENT( "Opening file.", 2 );
    OFile file;
    file.exceptions( std::fstream::failbit | std::fstream::badbit );
    file.open( filename );
    COMMENT( "Writing header.", 2 );
    file << "<svg version=\"1.1\">" << std::endl;

    COMMENT( "Creating adjacency.", 2 );
    for( size_t y = 0; y < vertex.size( 1 ); ++y ) {
      for( size_t x = 0; x < vertex.size( 0 ); ++x ) {
        size_t pxl = vertex.Position( x, y );
        size_t x_pos = ( x + 1 ) * 10;
        size_t y_pos = ( y + 1 ) * 10;
        file << "  <defs>" << std::endl;
        file << "    <marker id='head' orient='auto' markerWidth='2' markerHeight='4' refX='0.1' refY='1'>" << std::endl;
        file << "      <path d='M0,0 V2 L2,1 Z' fill='black' />" << std::endl;
        file << "    </marker>" << std::endl;
        file << "  </defs>" << std::endl;
        for( AdjacencyIterator itr = begin( adj, vertex, pxl ); *itr < vertex.size( ); ++itr ) {
          size_t adj_pxl = *itr;
          if( adj_pxl != pxl ) {
            size_t x_pos_adj = x_pos + adj( itr.Position( ), 0 ) * 10;
            size_t y_pos_adj = y_pos + adj( itr.Position( ), 1 ) * 10;
            COMMENT( "Moving arrow displacement.", 5 );
            if( adj( itr.Position( ), 0 ) > 0 )
              x_pos_adj -= 2;
            else if( adj( itr.Position( ), 0 ) < 0 )
              x_pos_adj += 2;
            if( adj( itr.Position( ), 1 ) > 0 )
              y_pos_adj -= 2;
            else if( adj( itr.Position( ), 1 ) < 0 )
              y_pos_adj += 2;
            file << "<path marker-end='url(#head)' stroke-width='1' fill='none' stroke='black' d='M"
                 << x_pos << "," << y_pos << " " << x_pos_adj << "," << y_pos_adj <<"' />" << std::endl;
          }
        }
      }
    }

    COMMENT( "Creating Graph", 2 );
    for( size_t y = 0; y < vertex.size( 1 ); ++y ) {
      for( size_t x = 0; x < vertex.size( 0 ); ++x ) {
        size_t pxl = vertex.Position( x, y );
        size_t x_pos = ( x + 1 ) * 10;
        size_t y_pos = ( y + 1 ) * 10;
        if( seed( pxl ) == false )
          file << "  <circle cx=\"" << x_pos << "\" cy=\"" << y_pos << "\" r=\"1\" fill=\"black\" />" << std::endl;
        else
          file << "  <circle cx=\"" << x_pos << "\" cy=\"" << y_pos << "\" r=\"1.5\" fill=\"black\" />" << std::endl;
        if( vertex( x, y ) < 10 )
          file << "  <text fill=\"gray\" x=\"" << x_pos - 3 << "\" y=\"" << y_pos - 2 << "\" font-size=\"4\">"
               << vertex( x, y ) << "</text>" << std::endl;
        else if( vertex( x, y ) < 100 )
          file << "  <text fill=\"gray\" x=\"" << x_pos - 6 << "\" y=\"" << y_pos - 2 << "\" font-size=\"4\">"
               << vertex( x, y ) << "</text>" << std::endl;
        else
          file << "  <text fill=\"gray\" x=\"" << x_pos - 8 << "\" y=\"" << y_pos - 2 << "\" font-size=\"4\">"
               << vertex( x, y ) << "</text>" << std::endl;
      }
    }
    file << "</svg>" << std::endl;
    file.close( );
  }

  template< class D >
  void SVGDGraph( const Image< D > &vertex, const Vector< bool > &seed, const Image< int > &label, const Adjacency &adj,
                  const std::string &filename ) {
    if( vertex.Dims( ) != 2 ) {
      std::string msg( BIAL_ERROR( "Vertex image must have 2 dimensions. Given: " +
                                   std::to_string( vertex.Dims( ) ) ) );
      throw( std::logic_error( msg ) );
    }
    COMMENT( "Opening file.", 2 );
    OFile file;
    file.exceptions( std::fstream::failbit | std::fstream::badbit );
    file.open( filename );
    COMMENT( "Writing header.", 2 );
    file << "<svg version=\"1.1\">" << std::endl;

    COMMENT( "Creating adjacency.", 2 );
    for( size_t y = 0; y < vertex.size( 1 ); ++y ) {
      for( size_t x = 0; x < vertex.size( 0 ); ++x ) {
        size_t pxl = vertex.Position( x, y );
        size_t x_pos = ( x + 1 ) * 10;
        size_t y_pos = ( y + 1 ) * 10;
        file << "  <defs>" << std::endl;
        file << "    <marker id='head' orient='auto' markerWidth='2' markerHeight='4' refX='0.1' refY='1'>" << std::endl;
        file << "      <path d='M0,0 V2 L2,1 Z' fill='black' />" << std::endl;
        file << "    </marker>" << std::endl;
        file << "  </defs>" << std::endl;
        for( AdjacencyIterator itr = begin( adj, vertex, pxl ); *itr < vertex.size( ); ++itr ) {
          size_t adj_pxl = *itr;
          if( adj_pxl != pxl ) {
            x_pos = ( x + 1 ) * 10;
            y_pos = ( y + 1 ) * 10;
            size_t x_pos_adj = x_pos + adj( itr.Position( ), 0 ) * 10;
            size_t y_pos_adj = y_pos + adj( itr.Position( ), 1 ) * 10;
            COMMENT( "Moving arrow displacement.", 5 );
            if( adj( itr.Position( ), 0 ) > 0 ) {
              x_pos_adj -= 2;
              x_pos += 2;
            }
            else if( adj( itr.Position( ), 0 ) < 0 ) {
              x_pos_adj += 2;
              x_pos -= 2;
            }
            if( adj( itr.Position( ), 1 ) > 0 ) {
              y_pos_adj -= 2;
              y_pos += 2;
            }
            else if( adj( itr.Position( ), 1 ) < 0 ) {
              y_pos_adj += 2;
              y_pos -= 2;
            }
            file << "  <path marker-end='url(#head)' stroke-width='1' fill='none' stroke='black' d='M"
                 << x_pos << "," << y_pos << " " << x_pos_adj << "," << y_pos_adj <<"' />" << std::endl;
          }
        }
      }
    }

    COMMENT( "Creating Graph", 2 );
    for( size_t y = 0; y < vertex.size( 1 ); ++y ) {
      for( size_t x = 0; x < vertex.size( 0 ); ++x ) {
        size_t pxl = vertex.Position( x, y );
        size_t x_pos = ( x + 1 ) * 10;
        size_t y_pos = ( y + 1 ) * 10;
        short red = label( pxl ) % 256;
        short green = ( label( pxl ) / 256 ) % 256;
        short blue = ( label( pxl ) / ( 256 * 256 ) ) % 256;
        if( seed( pxl ) == false )
          file << "  <circle cx=\"" << x_pos << "\" cy=\"" << y_pos << "\" r=\"1\" fill=\"#"
               << std::hex << ( red < 10 ? "0" : "" ) << red << ( green < 10 ? "0" : "" ) << green
               << ( blue < 10 ? "0" : "" ) << blue << std::dec << "\" />" << std::endl;
        else
          file << "  <circle cx=\"" << x_pos << "\" cy=\"" << y_pos << "\" r=\"1.5\" fill=\"#"
               << std::hex << ( red < 10 ? "0" : "" ) << red << ( green < 10 ? "0" : "" ) << green
               << ( blue < 10 ? "0" : "" ) << blue << std::dec << "\" />" << std::endl;
        if( vertex( x, y ) < 10 )
          file << "  <text fill=\"gray\" x=\"" << x_pos - 3 << "\" y=\"" << y_pos - 2 << "\" font-size=\"4\">"
               << vertex( x, y ) << "</text>" << std::endl;
        else if( vertex( x, y ) < 100 )
          file << "  <text fill=\"gray\" x=\"" << x_pos - 6 << "\" y=\"" << y_pos - 2 << "\" font-size=\"4\">"
               << vertex( x, y ) << "</text>" << std::endl;
        else
          file << "  <text fill=\"gray\" x=\"" << x_pos - 8 << "\" y=\"" << y_pos - 2 << "\" font-size=\"4\">"
               << vertex( x, y ) << "</text>" << std::endl;
      }
    }
    file << "</svg>" << std::endl;
    file.close( );
  }

  template< class D >
  void SVGDForest( const Image< D > &cost, const Image< int > &pred, const Image< int > &label,
                   const std::string &filename ) {
    if( cost.Dims( ) != 2 ) {
      std::string msg( BIAL_ERROR( "Cost image must have 2 dimensions. Given: " +
                                   std::to_string( cost.Dims( ) ) ) );
      throw( std::logic_error( msg ) );
    }
    COMMENT( "Opening file.", 2 );
    OFile file;
    file.exceptions( std::fstream::failbit | std::fstream::badbit );
    file.open( filename );
    COMMENT( "Writing header.", 2 );
    file << "<svg version=\"1.1\">" << std::endl;

    COMMENT( "Creating Edges", 2 );
    file << "  <defs>" << std::endl;
    file << "    <marker id='head' orient='auto' markerWidth='2' markerHeight='4' refX='0.1' refY='1'>" << std::endl;
    file << "      <path d='M0,0 V2 L2,1 Z' fill='black' />" << std::endl;
    file << "    </marker>" << std::endl;
    file << "  </defs>" << std::endl;
    for( size_t y = 0; y < cost.size( 1 ); ++y ) {
      for( size_t x = 0; x < cost.size( 0 ); ++x ) {
        size_t pxl = cost.Position( x, y );
        size_t x_pos = ( x + 1 ) * 10;
        size_t y_pos = ( y + 1 ) * 10;
        size_t adj_pxl = pred( pxl );
        size_t x_adj = adj_pxl % cost.size( 0 );
        size_t y_adj = adj_pxl / cost.size( 0 );
        if( ( adj_pxl != pxl ) && ( adj_pxl != static_cast<size_t>(-1) ) ) {
          size_t x_pos_adj = ( x_adj + 1 ) * 10;
          size_t y_pos_adj = ( y_adj + 1 ) * 10;
          COMMENT( "Moving arrow displacement.", 5 );
          if( x_adj > x ) {
            x_pos_adj -= 2;
            //x_pos += 2;
          }
          else if( x_adj < x ) {
            x_pos_adj += 2;
            //x_pos -= 2;
          }
          if( y_adj > y ) {
            y_pos_adj -= 2;
            //y_pos += 2;
          }
          else if( y_adj < y ) {
            y_pos_adj += 2;
            //y_pos -= 2;
          }
          file << "  <path marker-end='url(#head)' stroke-width='1' fill='none' stroke='black' d='M"
               << x_pos << "," << y_pos << " " << x_pos_adj << "," << y_pos_adj <<"' />" << std::endl;
        }
      }
    }

    COMMENT( "Creating Graph", 2 );
    for( size_t y = 0; y < cost.size( 1 ); ++y ) {
      for( size_t x = 0; x < cost.size( 0 ); ++x ) {
        size_t pxl = cost.Position( x, y );
        size_t x_pos = ( x + 1 ) * 10;
        size_t y_pos = ( y + 1 ) * 10;
        short red = label( pxl ) % 256;
        short green = ( label( pxl ) / 256 ) % 256;
        short blue = ( label( pxl ) / ( 256 * 256 ) ) % 256;
        if( pred( pxl ) != -1 )
          file << "  <circle cx=\"" << x_pos << "\" cy=\"" << y_pos << "\" r=\"1\" fill=\"#"
               << std::hex << ( red < 10 ? "0" : "" ) << red << ( green < 10 ? "0" : "" ) << green
               << ( blue < 10 ? "0" : "" ) << blue << std::dec << "\" />" << std::endl;
        else
          file << "  <circle cx=\"" << x_pos << "\" cy=\"" << y_pos << "\" r=\"1.5\" fill=\"#"
               << std::hex << ( red < 10 ? "0" : "" ) << red << ( green < 10 ? "0" : "" ) << green
               << ( blue < 10 ? "0" : "" ) << blue << std::dec << "\" />" << std::endl;
        if( cost( x, y ) < 10 )
          file << "  <text fill=\"gray\" x=\"" << x_pos - 3 << "\" y=\"" << y_pos - 2 << "\" font-size=\"4\">"
               << cost( x, y ) << "</text>" << std::endl;
        else if( cost( x, y ) < 100 )
          file << "  <text fill=\"gray\" x=\"" << x_pos - 6 << "\" y=\"" << y_pos - 2 << "\" font-size=\"4\">"
               << cost( x, y ) << "</text>" << std::endl;
        else
          file << "  <text fill=\"gray\" x=\"" << x_pos - 8 << "\" y=\"" << y_pos - 2 << "\" font-size=\"4\">"
               << cost( x, y ) << "</text>" << std::endl;
      }
    }
    file << "</svg>" << std::endl;
    file.close( );
  }


#ifdef BIAL_EXPLICIT_DrawSVGGraph

  template void SVGDGraph( const Image< int > &vertex, const Adjacency &adj, const std::string &filename );
  template void SVGDGraph( const Image< llint > &vertex, const Adjacency &adj, const std::string &filename );
  template void SVGDGraph( const Image< float > &vertex, const Adjacency &adj, const std::string &filename );
  template void SVGDGraph( const Image< double > &vertex, const Adjacency &adj, const std::string &filename );

  template void SVGDGraph( const Image< int > &vertex, const Vector< bool > &seed, const Adjacency &adj,
                           const std::string &filename );
  template void SVGDGraph( const Image< llint > &vertex, const Vector< bool > &seed, const Adjacency &adj,
                           const std::string &filename );
  template void SVGDGraph( const Image< float > &vertex, const Vector< bool > &seed, const Adjacency &adj,
                           const std::string &filename );
  template void SVGDGraph( const Image< double > &vertex, const Vector< bool > &seed, const Adjacency &adj,
                           const std::string &filename );

  template void SVGDGraph( const Image< int > &vertex, const Vector< bool > &seed, const Image< int > &label,
                           const Adjacency &adj, const std::string &filename );
  template void SVGDGraph( const Image< llint > &vertex, const Vector< bool > &seed, const Image< int > &label,
                           const Adjacency &adj, const std::string &filename );
  template void SVGDGraph( const Image< float > &vertex, const Vector< bool > &seed, const Image< int > &label,
                           const Adjacency &adj, const std::string &filename );
  template void SVGDGraph( const Image< double > &vertex, const Vector< bool > &seed, const Image< int > &label,
                           const Adjacency &adj, const std::string &filename );

  template void SVGDForest( const Image< int > &cost, const Image< int > &pred, const Image< int > &label,
                            const std::string &filename );
  template void SVGDForest( const Image< llint > &cost, const Image< int > &pred, const Image< int > &label,
                            const std::string &filename );
  template void SVGDForest( const Image< float > &cost, const Image< int > &pred, const Image< int > &label,
                            const std::string &filename );
  template void SVGDForest( const Image< double > &cost, const Image< int > &pred, const Image< int > &label,
                            const std::string &filename );

#endif

}

#endif

#endif
