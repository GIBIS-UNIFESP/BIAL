/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2015/Jan/12 
 * @brief Plotting algorithms. Plots in GnuPlot format
 */

#ifndef BIALPlotting_C
#define BIALPlotting_C

#include "Plotting.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_Plotting )
#define BIAL_EXPLICIT_Plotting
#endif
#if defined ( BIAL_EXPLICIT_Plotting ) || ( BIAL_IMPLICIT_BIN )

#include "Feature.hpp"
#include "Signal.hpp"

namespace Bial {

  template< class D >
  void Plotting::GnuPlot2DScatter( const std::string &basename, const Feature< D > &feat, size_t x, size_t y ) {
    try {
      OFile ofile;
      COMMENT( "Opening gnuplot file.", 0 );
      ofile.exceptions( std::fstream::failbit | std::fstream::badbit );
      ofile.open( basename + ".gnp" );
      COMMENT( "Writing to gnuplot file.", 0 );
      ofile << "set palette defined ( 1 \"antiquewhite\", 3 \"aquamarine\", 5 \"beige\", 7 \"black\", 9 \"blue\", " <<
	"11 \"brown\", 13 \"chartreuse\", 15 \"coral\", 17 \"cyan\", 19 \"gold\", 21 \"gray\", 25 \"green\", " <<
	"30\"magenta\", 50\"olive\", 100 \"pink\", 200 \"red\", 300 \"skyblue\", 500 \"slategray\", " <<
	"1000 \"violet\", 2000 \"yellow\" )" << std::endl;
      ofile << "plot \"" << basename.c_str( ) << ".txt\" u 1:2:3 with points palette pt 7" << std::endl;
      ofile.close( );

      COMMENT( "Opening text file.", 0 );
      ofile.open( basename + ".txt" );
      COMMENT( "Writing to text file.", 0 );
      for( size_t elm = 0; elm < feat.Elements( ); ++elm ) {
        ofile << feat( elm, x ) << " " << feat( elm, y ) << " " << feat.Label( elm ) % 2000 << std::endl;
      }
      ofile.close( );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while writing to file." ) );
      throw( std::ios_base::failure( msg ) );
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

  void Plotting::GnuPlot2DLine( const std::string &basename, const Vector< Signal > &signal ) {
    try {
      OFile ofile;
      COMMENT( "Opening gnuplot file.", 0 );
      ofile.exceptions( std::fstream::failbit | std::fstream::badbit );
      ofile.open( basename + ".gnp" );
      COMMENT( "Writing to gnuplot file.", 0 );
      if( signal.size( ) >= 40 ) {
        ofile <<
	  "set palette defined ( 1 \"antiquewhite\", 3 \"aquamarine\", 5 \"beige\", 7 \"black\", 9 \"blue\", " <<
	  "11 \"brown\", 13 \"chartreuse\", 15 \"coral\", 17 \"cyan\", 19 \"gold\", 21 \"gray\", 25 \"green\", " <<
	  "30 \"magenta\", 50\"olive\", 100 \"pink\", 200 \"red\", 300 \"skyblue\", 500 \"slategray\", " <<
	  "1000 \"violet\", 2000 \"yellow\" )" << std::endl;
      }
      else if( signal.size( ) >= 20 ) {
        ofile <<
	  "set palette defined ( 1 \"antiquewhite\", 3 \"aquamarine\", 5 \"beige\", 7 \"black\", 9 \"blue\", " <<
	  "11 \"brown\", 13 \"chartreuse\", 15 \"coral\", 17 \"cyan\", 19 \"gold\", 21 \"gray\", 23 \"green\", " <<
	  "25 \"magenta\", 27\"olive\", 29 \"pink\", 31 \"red\", 33 \"skyblue\", 35 \"slategray\", " <<
	  "37 \"violet\", 39 \"yellow\" )" << std::endl;
      }
      else {
        ofile << "set palette defined ( 1 \"red\", 2 \"green\", 3 \"blue\", 4 \"black\", 5 \"magenta\", " <<
	  "6 \"yellow\", 7 \"cyan\", 8 \"coral\", 9 \"chartreuse\", 10 \"gold\", 11 \"gray\", 12 \"aquamarine\", " <<
	  "13 \"beige\", 14\"olive\", 15 \"pink\", 16 \"antiquewhite\", 17 \"skyblue\", 18 \"slategray\", " <<
	  "19 \"violet\", 20 \"brown\" )" << std::endl;
      }
      /* ofile << "plot \"" << basename.c_str( ) << ".txt\" u 1:2:3 with lines palette pt 7" << std::endl; */
      ofile << "plot \"" << basename.c_str( ) << ".txt\" with lines" << std::endl;
      ofile.close( );

      COMMENT( "Opening text file.", 0 );
      ofile.open( basename + ".txt" );
      COMMENT( "Writing to text file.", 0 );
      for( size_t elm = 0; elm < signal.size( ); ++elm ) {
        /* signal[ elm ].Print( std::cout ); */
        signal[ elm ].Print( ofile );
      }
      ofile.close( );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while writing to file." ) );
      throw( std::ios_base::failure( msg ) );
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

#ifdef BIAL_EXPLICIT_Plotting

  template void Plotting::GnuPlot2DScatter( const std::string &basename, const Feature< int > &feat, size_t x,
                                            size_t y );
  template void Plotting::GnuPlot2DScatter( const std::string &basename, const Feature< llint > &feat, size_t x,
                                            size_t y );
  template void Plotting::GnuPlot2DScatter( const std::string &basename, const Feature< float > &feat, size_t x,
                                            size_t y );
  template void Plotting::GnuPlot2DScatter( const std::string &basename, const Feature< double > &feat, size_t x,
                                            size_t y );

#endif

}

#endif

#endif

