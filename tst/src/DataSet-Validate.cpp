/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2015/May/26 */
/* Content: This program is used to generate test results for dataset evaluation. */
/* Description: The inputs are a groud-truth directory and a result directory, along with the desired statistics. Inside
 * each directory, the images must have the same name in order to be found by the program. For instance, a inside
 * directories "gt" and "res", if there are two image files called "1.pgm", them the selected metric will be computed
 * between them. The results will always be in terms of mean and standard deviation. */

#include "Bit.hpp"
#include "FileImage.hpp"
#include "Image.hpp"
#include "StatisticsAverage.hpp"
#include "StatisticsDice.hpp"
#include "StatisticsKappa.hpp"
#include "StatisticsJaccard.hpp"
#include "StatisticsObsAgree.hpp"
#include "StatisticsPosNeg.hpp"
#include "StatisticsStdDev.hpp"
#include "Table.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {
  if( argc != 6 ) {
    cout << "Usage: " << argv[ 0 ] << " <groud-truth dir> <result dir> <region/border> <metric> <output_basename>"
         << endl;
    cout << "\t\tgroud-truth dir, result dir: directories containing pgm files." << endl;
    cout << "\t\tregion/border: 0-region based metric, 1-border based metric " << endl;
    cout << "\t\tmetrics: 1-TP,TN,FP,FN, 2-Dice, 4-Jaccard, 8-Kappa, 16-Observed agreement." << endl;
    cout << "\t\t\tFor multiple metrics, enter the sum of the indexes." << endl;
    return( 0 );
  }
  COMMENT( "Checking if input directories exist.", 0 );
  if( !Directory::Exists( argv[ 1 ] ) ) {
    cout << argv[ 1 ] << ": directory does not exists or not readable." << endl;
    return( 0 );
  }
  if( !Directory::Exists( argv[ 2 ] ) ) {
    cout << argv[ 2 ] << ": directory does not exists or not readable." << endl;
    return( 0 );
  }
  COMMENT( "List the files.", 0 );
  string gdt_dir( argv[ 1 ] );
  string res_dir( argv[ 2 ] );
  Vector< string > gdt_file( Directory::ListFiles( gdt_dir, string( "pgm" ) ) );
  Vector< string > res_file( Directory::ListFiles( res_dir, string( "pgm" ) ) );
  COMMENT( "Sorting files.", 0 );
  std::sort( gdt_file.begin( ), gdt_file.end( ) );
  std::sort( res_file.begin( ), res_file.end( ) );
  COMMENT( "gdt_files: " << gdt_file, 0 );
  COMMENT( "res_files: " << res_file, 0 );

  COMMENT( "Inserting separator in the end of directories name if required.", 0 );
  if( gdt_dir[ gdt_dir.size( ) - 1 ] != DIR_SEPARATOR ) {
    gdt_dir += DIR_SEPARATOR;
  }
  if( res_dir[ res_dir.size( ) - 1 ] != DIR_SEPARATOR ) {
    res_dir += DIR_SEPARATOR;
  }
  COMMENT( "Getting parameters.", 0 );
  bool is_border = static_cast< bool >( atoi( argv[ 3 ] ) );
  int metric = atoi( argv[ 4 ] );
  size_t ngdt = gdt_file.size( );
  size_t nres = res_file.size( );
  bool print_basics = Bit::IsOne( metric, 0 );
  bool print_dice = Bit::IsOne( metric, 1 );
  bool print_jacc = Bit::IsOne( metric, 2 );
  bool print_kappa = Bit::IsOne( metric, 3 );
  bool print_obs_agr = Bit::IsOne( metric, 4 );
  COMMENT( "gt files: " << ngdt << ", res files: " << nres, 0 );

  COMMENT( "Computing the number of distinct images.", 0 );
  size_t total = 0;
  size_t gdt_idx = 0, res_idx = 0;
  Vector< string > file_name( 1, "File" );
  while( ( gdt_idx < ngdt ) || ( res_idx < nres ) ) {
    if( ( gdt_idx < ngdt ) && ( res_idx < nres ) ) {
      if( gdt_file[ gdt_idx ] == res_file[ res_idx ] ) {
        file_name.push_back( gdt_file[ gdt_idx ] );
        ++gdt_idx;
        ++res_idx;
      }
      else if( gdt_file[ gdt_idx ] > res_file[ res_idx ] ) {
        file_name.push_back( res_file[ res_idx ] );
        ++res_idx;
      }
      else {
        file_name.push_back( gdt_file[ gdt_idx ] );
        ++gdt_idx;
      }
    }
    else if( gdt_idx < ngdt ) {
      file_name.push_back( gdt_file[ gdt_idx ] );
      ++gdt_idx;
    }
    else {
      file_name.push_back( res_file[ res_idx ] );
      ++res_idx;
    }
    ++total;
  }
  COMMENT( "Creating sample table.", 0 );
  Table sample;
  sample.PushBack( file_name );

  COMMENT( "Computing statistics for single images.", 0 );
  gdt_idx = 0;
  res_idx = 0;
  Vector< float > mean( 1, std::nanf( "" ) );
  Vector< float > std_dev( 1, std::nanf( "" ) );
  Vector< float > TP( 1, std::nanf( "" ) ), TN( 1, std::nanf( "" ) ), FP( 1, std::nanf( "" ) );
  Vector< float > FN( 1, std::nanf( "" ) ), dice( 1, std::nanf( "" ) ), jacc( 1, std::nanf( "" ) );
  Vector< float > kappa( 1, std::nanf( "" ) ), obs_agr( 1, std::nanf( "" ) );
  while( ( gdt_idx < ngdt ) || ( res_idx < nres ) ) {
    COMMENT( "Loading images: " << gdt_idx << ", " << res_idx, 3 );
    bool valid = true;
    Image< int > gdt;
    Image< int > res;
    if( ( gdt_idx < ngdt ) && ( res_idx < nres ) ) {
      if( gdt_file[ gdt_idx ] == res_file[ res_idx ] ) {
        COMMENT( "computing for equal file: " << gdt_file[ gdt_idx ], 3 );
        gdt = Read< int >( gdt_dir + gdt_file[ gdt_idx ] );
        res = Read< int >( res_dir + res_file[ res_idx ] );
        if( gdt.Dims( ) != res.Dims( ) ) {
          BIAL_WARNING( "Images " + gdt_dir + gdt_file[ gdt_idx ] + " and " + res_dir + res_file[ res_idx ] +
                        " have incompatible dimensions." );
          valid = false;
        }
        ++gdt_idx;
        ++res_idx;
      }
      else {
        COMMENT( "failed for different files: " << gdt_file[ gdt_idx ] << " and " << res_file[ res_idx ], 3 );
        valid = false;
        if( gdt_file[ gdt_idx ] > res_file[ res_idx ] ) {
          ++res_idx;
        }
        else {
          ++gdt_idx;
        }
      }
    }
    else {
      valid = false;
      if( gdt_idx < ngdt ) {
        COMMENT( "failed for file: " << gdt_file[ gdt_idx ], 3 );
        ++gdt_idx;
      }
      else {
        COMMENT( "failed for file: " << res_file[ res_idx ], 3 );
        ++res_idx;
      }
    }
    COMMENT( "Computing metrics.", 3 );
    if( print_basics ) {
      if( !valid ) {
        TP.push_back( std::nanf( "" ) );
        TN.push_back( std::nanf( "" ) );
        FP.push_back( std::nanf( "" ) );
        FN.push_back( std::nanf( "" ) );
      }
      else if( is_border ) {
        float TP_val, TN_val, FP_val, FN_val;
        tie( TP_val, TN_val, FP_val, FN_val ) = Statistics::PositiveNegative( res, gdt, 2.0f );
        TP.push_back( TP_val );
        TN.push_back( TN_val );
        FP.push_back( FP_val );
        FN.push_back( FN_val );
      }
      else {
        float TP_val, TN_val, FP_val, FN_val;
        tie( TP_val, TN_val, FP_val, FN_val ) = Statistics::PositiveNegative( res, gdt );
        TP.push_back( TP_val );
        TN.push_back( TN_val );
        FP.push_back( FP_val );
        FN.push_back( FN_val );
      }
    }
    if( print_dice ) {
      if( !valid ) {
        dice.push_back( std::nanf( "" ) );
      }
      else if( is_border ) {
        dice.push_back( Statistics::Dice( res, gdt, 2.0f ) );
      }
      else {
        dice.push_back( Statistics::Dice( res, gdt ) );
      }
    }
    if( print_jacc ) {
      if( !valid ) {
        jacc.push_back( std::nanf( "" ) );
      }
      else if( is_border ) {
        jacc.push_back( Statistics::Jaccard( res, gdt, 2.0f ) );
      }
      else {
        jacc.push_back( Statistics::Jaccard( res, gdt ) );
      }
    }
    if( print_kappa ) {
      if( !valid ) {
        kappa.push_back( std::nanf( "" ) );
      }
      else if( is_border ) {
        kappa.push_back( Statistics::Kappa( res, gdt, 2.0f ) );
      }
      else {
        kappa.push_back( Statistics::Kappa( res, gdt ) );
      }
    }
    if( print_obs_agr ) {
      if( !valid ) {
        obs_agr.push_back( std::nanf( "" ) );
      }
      else if( is_border ) {
        obs_agr.push_back( Statistics::ObservedAgreement( res, gdt, 2.0f ) );
      }
      else {
        obs_agr.push_back( Statistics::ObservedAgreement( res, gdt ) );
      }
    }
  }
  COMMENT( "Inserting data into sample table.", 0 );
  if( print_basics ) {
    COMMENT( "TP: " << TP, 2 );
    COMMENT( "TN: " << TN, 2 );
    COMMENT( "FP: " << FP, 2 );
    COMMENT( "FN: " << FN, 2 );
    sample.PushBack( TP );
    sample[ sample.Columns( ) - 1 ][ 0 ] = "TP";
    sample.PushBack( TN );
    sample[ sample.Columns( ) - 1 ][ 0 ] = "TN";
    sample.PushBack( FP );
    sample[ sample.Columns( ) - 1 ][ 0 ] = "FP";
    sample.PushBack( FN );
    sample[ sample.Columns( ) - 1 ][ 0 ] = "FN";
  }
  if( print_dice ) {
    COMMENT( "dice: " << dice, 2 );
    sample.PushBack( dice );
    sample[ sample.Columns( ) - 1 ][ 0 ] = "Dice";
  }
  if( print_jacc ) {
    COMMENT( "jacc: " << jacc, 2 );
    sample.PushBack( jacc );
    sample[ sample.Columns( ) - 1 ][ 0 ] = "Jaccard";
  }
  if( print_kappa ) {
    COMMENT( "kappa: " << kappa, 2 );
    sample.PushBack( kappa );
    sample[ sample.Columns( ) - 1 ][ 0 ] = "Kappa";
  }
  if( print_obs_agr ) {
    COMMENT( "obs_agr: " << obs_agr, 2 );
    sample.PushBack( obs_agr );
    sample[ sample.Columns( ) - 1 ][ 0 ] = "Observed Agreement";
  }
  COMMENT( "Creating resulting table.", 0 );
  Table res;
  Vector< string > row_name( { "", "mean", "std dev" } );
  res.PushBack( row_name );

  COMMENT( "Computing average and standard deviation.", 0 );
  float TP_mean, TN_mean, FP_mean, FN_mean, dice_mean, jacc_mean, kappa_mean, obs_agr_mean;
  float TP_std_dev, TN_std_dev, FP_std_dev, FN_std_dev, dice_std_dev, jacc_std_dev, kappa_std_dev, obs_agr_std_dev;
  if( print_basics ) {
    TP_mean = Statistics::Average( TP, 1, TP.size( ) - 1 );
    TN_mean = Statistics::Average( TN, 1, TN.size( ) - 1 );
    FP_mean = Statistics::Average( FP, 1, FP.size( ) - 1 );
    FN_mean = Statistics::Average( FN, 1, FN.size( ) - 1 );
    TP_std_dev = Statistics::StandardDeviation( TP, 1, TP.size( ) - 1, TP_mean );
    TN_std_dev = Statistics::StandardDeviation( TN, 1, TN.size( ) - 1, TN_mean );
    FP_std_dev = Statistics::StandardDeviation( FP, 1, FP.size( ) - 1, FP_mean );
    FN_std_dev = Statistics::StandardDeviation( FN, 1, FN.size( ) - 1, FN_mean );
    res.PushBack( Vector< float >( { 1.0f, TP_mean, TP_std_dev } ) );
    res[ res.Columns( ) - 1 ][ 0 ] = "TP";
    res.PushBack( Vector< float >( { 1.0f, TN_mean, TN_std_dev } ) );
    res[ res.Columns( ) - 1 ][ 0 ] = "TN";
    res.PushBack( Vector< float >( { 1.0f, FP_mean, FP_std_dev } ) );
    res[ res.Columns( ) - 1 ][ 0 ] = "FP";
    res.PushBack( Vector< float >( { 1.0f, FN_mean, FN_std_dev } ) );
    res[ res.Columns( ) - 1 ][ 0 ] = "FN";
  }
  if( print_dice ) {
    dice_mean = Statistics::Average( dice, 1, dice.size( ) - 1 );
    dice_std_dev = Statistics::StandardDeviation( dice, 1, dice.size( ) - 1, dice_mean );
    res.PushBack( Vector< float >( { 1.0f, dice_mean, dice_std_dev } ) );
    res[ res.Columns( ) - 1 ][ 0 ] = "Dice";
  }
  if( print_jacc ) {
    jacc_mean = Statistics::Average( jacc, 1, jacc.size( ) - 1 );
    jacc_std_dev = Statistics::StandardDeviation( jacc, 1, jacc.size( ) - 1, jacc_mean );
    res.PushBack( Vector< float >( { 1.0f, jacc_mean, jacc_std_dev } ) );
    res[ res.Columns( ) - 1 ][ 0 ] = "Jaccard";
  }
  if( print_kappa ) {
    kappa_mean = Statistics::Average( kappa, 1, kappa.size( ) - 1 );
    kappa_std_dev = Statistics::StandardDeviation( kappa, 1, kappa.size( ) - 1, kappa_mean );
    res.PushBack( Vector< float >( { 1.0f, kappa_mean, kappa_std_dev } ) );
    res[ res.Columns( ) - 1 ][ 0 ] = "Kappa";
  }
  if( print_obs_agr ) {
    obs_agr_mean = Statistics::Average( obs_agr, 1, obs_agr.size( ) - 1 );
    obs_agr_std_dev = Statistics::StandardDeviation( obs_agr, 1, obs_agr.size( ) - 1, obs_agr_mean );
    res.PushBack( Vector< float >( { 1.0f, obs_agr_mean, obs_agr_std_dev } ) );
    res[ res.Columns( ) - 1 ][ 0 ] = "Observed Agreement";
  }
  sample.Write( string( argv[ 5 ] ) + "-samples.csv" );
  res.Write( string( argv[ 5 ] ) + "-res.csv" );

  return( 0 );
}
