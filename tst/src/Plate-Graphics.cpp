/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2014/Oct/23 */
/* Content: Test file. */
/* Description: Test with plate detection. */

#include "File.hpp"
#include "Plotting.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc < 2 ) {
    cout << "Usage: " << argv[ 0 ] << " <input dir> <output file>" << endl;
    return( 0 );
  }
  COMMENT( "Reading directory files.", 0 );
  Vector< string > files = Directory::List( argv[ 1 ], false );

  COMMENT( "Getting statistics from files.", 0 );
  /* Vector< float > data; */
  size_t TP;
  size_t FP;
  size_t FN;
  double total_TP = 0.0;
  double total_FP = 0.0;
  double total_FN = 0.0;
  IFile fi;
  for( size_t fl = 0; fl < files.size( ); ++fl ) {
    if( files[ fl ].rfind( "stats" ) != string::npos ) {
      fi.open( string( argv[ 1 ] ) + DIR_SEPARATOR + files[ fl ] );
      if( !fi.is_open( ) ) {
        std::string msg( BIAL_ERROR( "Error while opening file " + string( argv[ 1 ] ) + DIR_SEPARATOR + files[ fl ] ) );
        throw( std::ios_base::failure( msg ) );
      }
      COMMENT( "Reading content.", 0 );
      fi.ignore( 5 );
      fi >> TP;
      total_TP += TP;

      fi.ignore( 5 );
      fi >> FP;
      total_FP += FP;

      fi.ignore( 5 );
      fi >> FN;
      total_FN += FN;
      fi.close( );
    }
  }
  COMMENT( "Writing result.", 0 );
  OFile of;
  of.open( argv[ 2 ] );
  of << "TP  = " << total_TP << endl;
  of << "FP  = " << total_FP << endl;
  of << "FN  = " << total_FN << endl;
  of << "PPV = " << total_TP / ( total_TP + total_FP ) << endl;
  of << "TPR = " << total_TP / ( total_TP + total_FN ) << endl;
  of.close( );

  return( 0 );
}
