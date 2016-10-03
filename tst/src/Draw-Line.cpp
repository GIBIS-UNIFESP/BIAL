/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Author: Lucas Santana Lellis */
/* Date: 2014/Jul/26 */
/* Version: 1.0.00 */
/* Content: Test file. */
/* Description: Test line drawing functions. */

#include "Color.hpp"
#include "DrawLine.hpp"
#include "FileImage.hpp"
#include "Image.hpp"
#include "Vector.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 2 ) {
    cout << "Usage: " << argv[ 0 ] << " <output file name> " << endl;
    return( 0 );
  }
  try {

    COMMENT( "Creating ARGB image ( 2d image with 4 channels )", 1 );
    Image< Color > img( 800, 600 );
    Color red1 = { 255, 255, 0, 0 };
    Color red2 = { 255, 128, 0, 0 };
    Color red3 = { 255, 64, 0, 0 };
    Color green1 = { 255, 0, 255, 0 };
    Color green2 = { 255, 0, 128, 0 };
    Color green3 = { 255, 0, 64, 0 };
    Color blue1 = { 255, 0, 0, 255 };
    Color blue2 = { 255, 0, 0, 128 };
    Color blue3 = { 255, 0, 0, 64 };
    Color yellow1 = { 255, 255, 255, 0 };
    Color yellow2 = { 255, 128, 128, 0 };
    Color yellow3 = { 255, 64, 64, 0 };

    Line l1( { 0, 0 }, { 799, 599 }, red1 );
    l1.Draw( img );
    l1 = Line( { 0, 0 }, { 799, 599 }, red2 );
    l1.Draw( img, { 100, 0452 } );
    l1 = Line( { 0, 0 }, { 799, 599 }, red3 );
    l1.Draw( img, { 510, 85 } );

    Line l2( { 0, 599 }, { 799, 0 }, blue1 );
    l2.Draw( img );
    l2 = Line( { 0, 599 }, { 799, 0 }, blue2 );
    l2.Draw( img, { 125, 2 } );
    l2 = Line( { 0, 599 }, { 799, 0 }, blue3 );
    l2.Draw( img, { 5, -25 } );

    Line l3( { -5420, -1890 }, { 5400, 2000 }, green1 );
    l3.Draw( img );
    l3 = Line( { -5420, -1890 }, { 5400, 2000 }, green2 );
    l3.Draw( img, { 101, 21 } );
    l3 = Line( { -5420, -1890 }, { 5400, 2000 }, green3 );
    l3.Draw( img, { 54, 85 } );

    Line l4( { 5420, -1890 }, { -5400, 2000 }, yellow1 );
    l4.Draw( img );
    l4 = Line( { 5420, -1890 }, { -5400, 2000 }, yellow2 );
    l4.Draw( img, { 101, 21 } );
    l4 = Line( { 5420, -1890 }, { -5400, 2000 }, yellow3 );
    l4.Draw( img, { 54, 85 } );

    Write( img, argv[ 1 ] );
    return( 0 );
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
