#include "DescriptionEHD.hpp"

#include "Adjacency.hpp"
#include "Signal.hpp"

#include <fstream>
#include <stdlib.h>

namespace Bial {

  size_t EHD::FindMaxPosition( Vector< int > vet ) {
    size_t max;
    int val;

    val = vet[ 0 ];
    max = 0;
    for( size_t i = 1; i < vet.size( ); i++ ) {
      if( val < vet[ i ] ) {
        val = vet[ i ];
        max = i;
      }
    }
    return( max );
  }

  Vector< int > EHD::ConvolveFilter( int mask[ 2 ][ 2 ] ) {
    Vector< int > ori( BINS );

    ori[ 0 ] = abs( mask[ 0 ][ 0 ] + mask[ 0 ][ 1 ] - mask[ 1 ][ 0 ] - mask[ 1 ][ 1 ] );
    ori[ 1 ] = abs( mask[ 0 ][ 0 ] - mask[ 1 ][ 1 ] );
    ori[ 1 ] = ( INT16 ) RIGHT_SHIFT( DESCALE( MULTIPLY( FIX( ori[ 1 ] ),
                                                         FIX_1_4142135 ), CONST_BITS - PASS1_BITS ),
                                      CONST_BITS + PASS1_BITS );
    ori[ 2 ] = abs( mask[ 0 ][ 0 ] - mask[ 0 ][ 1 ] + mask[ 1 ][ 0 ] - mask[ 1 ][ 1 ] );
    ori[ 3 ] = abs( mask[ 0 ][ 1 ] - mask[ 1 ][ 0 ] );
    ori[ 3 ] = ( INT16 ) RIGHT_SHIFT( DESCALE( MULTIPLY( FIX( ori[ 3 ] ),
                                                         FIX_1_4142135 ), CONST_BITS - PASS1_BITS ),
                                      CONST_BITS + PASS1_BITS );
    ori[ 4 ] = abs( mask[ 0 ][ 0 ] - mask[ 0 ][ 1 ] - mask[ 1 ][ 0 ] + mask[ 1 ][ 1 ] );
    ori[ 4 ] = MULTIPLY( ori[ 4 ], 2 );

    return( ori );
  }

  EHD::EHD( FeatureDetector< int > *Fd ) : EHD( Fd->Run( ) ) {
  }

  EHD::EHD( Vector < tuple < Image< int >, Image< int >> > detected ) : FeatureExtractor< int, int >( detected ) {
    GRID = 4;
    BINS = 5;
    BLCK = 16;
  }

  void EHD::SetParameters( ParameterInterpreter *interpreter ) {
    Vector< parameter > vet;
    vet.push_back( tie( "grid", GRID ) );
    vet.push_back( tie( "bins", BINS ) );
    vet.push_back( tie( "blck", BLCK ) );

    interpreter->SetExpectedParameters( vet );
    vet = interpreter->Interpret( );

    tie( ignore, GRID ) = vet[ 0 ];
    tie( ignore, BINS ) = vet[ 1 ];
    tie( ignore, BLCK ) = vet[ 2 ];
  }

  string EHD::GetParameters( ParameterInterpreter *interpreter ) {
    Vector< parameter > vet;

    vet.push_back( tie( "grid", GRID ) );
    vet.push_back( tie( "bins", BINS ) );
    vet.push_back( tie( "blck", BLCK ) );

    interpreter->SetExpectedParameters( vet );

    return( interpreter->GetExpectedParameters( ) );
  }

  EHDfeature EHD::Run( ) {
    size_t CELL = ( GRID * GRID * BINS );
    size_t SIZE = ( CELL + 14 * BINS );

    EHDfeature feat;
    Image< int > img;
    Image< int > mask;

    Features< int > histogram;

    int level[ GRID ][ GRID ][ BLCK ][ BLCK ][ 2 ][ 2 ];
    ulong hist[ GRID ][ GRID ][ BINS ], ext[ 14 * BINS ];
    Vector< int > ori;

    size_t imgs_rows, imgs_cols;
    size_t grid_rows, grid_cols;
    size_t blck_rows, blck_cols;
    size_t mask_rows, mask_cols;

    size_t ygrid, yblck, ymask;
    size_t xgrid, xblck, xmask;

    size_t bin, cell, sum;
    for( size_t i = 0; i < this->detected.size( ); ++i ) {
      tie( img, mask ) = this->detected[ i ];
      /* initialize levels--------------------------------------------------------- */
      for( ygrid = 0; ygrid < GRID; ygrid++ ) {
        for( xgrid = 0; xgrid < GRID; xgrid++ ) {
          for( yblck = 0; yblck < BLCK; yblck++ ) {
            for( xblck = 0; xblck < BLCK; xblck++ ) {
              for( ymask = 0; ymask < 2; ymask++ ) {
                for( xmask = 0; xmask < 2; xmask++ ) {
                  level[ ygrid ][ xgrid ][ yblck ][ xblck ][ ymask ][ xmask ] = 0;
                }
              }
            }
          }
        }
      }
      /*
       * --------------------------------------------------------------------------
       * initialize hist-----------------------------------------------------------
       */
      for( ygrid = 0; ygrid < GRID; ygrid++ ) {
        for( xgrid = 0; xgrid < GRID; xgrid++ ) {
          for( bin = 0; bin < BINS; bin++ ) {
            hist[ ygrid ][ xgrid ][ bin ] = 0;
          }
        }
      }
      /* -------------------------------------------------------------------------- */

      /* quantize levels----------------------------------------------------------- */
      imgs_cols = img.size( 0 );
      imgs_rows = img.size( 1 );

      grid_rows = imgs_rows / GRID;
      grid_cols = imgs_cols / GRID;

      blck_rows = grid_rows / BLCK;
      blck_cols = grid_cols / BLCK;

      mask_rows = blck_rows / 2;
      mask_cols = blck_cols / 2;
      for( size_t row = 0; row < imgs_rows; row++ ) {
        for( size_t col = 0; col < imgs_cols; col++ ) {
          if( mask( col, row ) == 1 ) {
            int val = img[ col + row * img.size( 0 ) ];

            ygrid = GRID * row / imgs_rows;
            xgrid = GRID * col / imgs_cols;

            yblck = BLCK * ( row % grid_rows ) / grid_rows;
            xblck = BLCK * ( col % grid_cols ) / grid_cols;

            ymask = 2 * ( ( row % grid_rows ) % blck_rows ) / blck_rows;
            xmask = 2 * ( ( col % grid_cols ) % blck_cols ) / blck_cols;

            level[ ygrid ][ xgrid ] [ yblck ][ xblck ] [ ymask ][ xmask ] += val;
          }
        }
      }
      /*
       * --------------------------------------------------------------------------
       * Normalize level-----------------------------------------------------------
       */
      for( ygrid = 0; ygrid < GRID; ygrid++ ) {
        for( xgrid = 0; xgrid < GRID; xgrid++ ) {
          for( yblck = 0; yblck < BLCK; yblck++ ) {
            for( xblck = 0; xblck < BLCK; xblck++ ) {
              for( ymask = 0; ymask < 2; ymask++ ) {
                for( xmask = 0; xmask < 2; xmask++ ) {
                  level[ ygrid ][ xgrid ] [ yblck ][ xblck ] [ ymask ][ xmask ] /= ( mask_rows * mask_cols );
                }
              }
            }
          }
        }
      }
      /*
       * --------------------------------------------------------------------------
       * Compute hist--------------------------------------------------------------
       */
      for( ygrid = 0; ygrid < GRID; ygrid++ ) {
        for( xgrid = 0; xgrid < GRID; xgrid++ ) {
          for( yblck = 0; yblck < BLCK; yblck++ ) {
            for( xblck = 0; xblck < BLCK; xblck++ ) {
              ori = ConvolveFilter( level[ ygrid ][ xgrid ][ yblck ][ xblck ] );
              size_t max = FindMaxPosition( ori );
              if( ori[ max ] > 0 ) {
                hist[ ygrid ][ xgrid ][ max ]++;
              }
            }
          }
        }
      }
      /* -------------------------------------------------------------------------- */

      /* Extend histogram---------------------------------------------------------- */

      size_t k = 0;
      for( ygrid = 0; ygrid < GRID; ygrid++ ) {
        for( bin = 0; bin < BINS; bin++ ) {
          sum = 0;
          for( xgrid = 0; xgrid < GRID; xgrid++ ) {
            sum += hist[ ygrid ][ xgrid ][ bin ];
          }
          ext[ k++ ] = sum;
        }
      }
      for( xgrid = 0; xgrid < GRID; xgrid++ ) {
        for( bin = 0; bin < BINS; bin++ ) {
          sum = 0;
          for( ygrid = 0; ygrid < GRID; ygrid++ ) {
            sum += hist[ ygrid ][ xgrid ][ bin ];
          }
          ext[ k++ ] = sum;
        }
      }
      for( size_t j = 8 * BINS; j < 14 * BINS; j++ ) {
        ext[ j ] = 0;
      }
      for( ygrid = 0; ygrid < GRID; ygrid++ ) {
        for( xgrid = 0; xgrid < GRID; xgrid++ ) {
          k = ( 8 + ( ygrid / 2 * 2 + xgrid / 2 ) ) * BINS;
          for( bin = 0; bin < BINS; bin++ ) {
            ext[ k + bin ] += hist[ ygrid ][ xgrid ][ bin ];
          }
        }
      }
      for( bin = 0; bin < BINS; bin++ ) {
        ext[ 12 * BINS + bin ] = hist[ GRID / 2 ][ GRID / 2 ][ bin ] +
                                 hist[ GRID / 2 ][ GRID / 2 + 1 ][ bin ] +
                                 hist[ GRID / 2 + 1 ][ GRID / 2 ][ bin ] +
                                 hist[ GRID / 2 + 1 ][ GRID / 2 + 1 ][ bin ];
      }
      for( ygrid = 0; ygrid < GRID; ygrid++ ) {
        for( xgrid = 0; xgrid < GRID; xgrid++ ) {
          for( bin = 0; bin < BINS; bin++ ) {
            ext[ 13 * BINS + bin ] += hist[ ygrid ][ xgrid ][ bin ];
          }
        }
      }
      /* -------------------------------------------------------------------------- */

      /* Histograma---------------------------------------------------------------- */
      histogram = Features< int >( SIZE );

      k = 0;
      for( ygrid = 0; ygrid < GRID; ygrid++ ) {
        for( xgrid = 0; xgrid < GRID; xgrid++ ) {
          for( bin = 0; bin < BINS; bin++ ) {
            histogram[ k++ ] = hist[ ygrid ][ xgrid ][ bin ];
          }
        }
      }
      for( ; k < SIZE; k++ ) {
        histogram[ k ] = ext[ k - CELL ];
      }
      /*
       * --------------------------------------------------------------------------
       * Normalize histogram-------------------------------------------------------
       */
      for( ygrid = 0; ygrid < GRID; ygrid++ ) {
        for( xgrid = 0; xgrid < GRID; xgrid++ ) {
          cell = ( ygrid * GRID + xgrid ) * BINS;
          sum = 0;
          for( bin = 0; bin < BINS; bin++ ) {
            sum += histogram[ bin + cell ];
          }
          for( bin = 0; bin < BINS; bin++ ) {
            histogram[ bin + cell ] = Log( histogram[ bin + cell ], sum );
          }
        }
      }
      for( size_t j = 0; j < 14; j++ ) {
        cell = CELL + j * BINS;
        sum = 0;
        for( bin = 0; bin < BINS; bin++ ) {
          sum += histogram[ bin + cell ];
        }
        for( bin = 0; bin < BINS; bin++ ) {
          histogram[ bin + cell ] = Log( histogram[ bin + cell ], sum );
        }
      }
      feat.push_back( histogram );
      /* ---------------------------------------------------------- */
    }
    return( feat );
  }

}
