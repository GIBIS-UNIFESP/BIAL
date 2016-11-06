#include "DescriptionSS.hpp"

#include "AdjacencyRound.hpp"
#include "ImageFrame.hpp"
#include "MorphologyDilation.hpp"
#include "Signal.hpp"
#include "SortingSort.hpp"

#include <fstream>
#include <stdlib.h>

#include "File.hpp"
#include "FileImage.hpp"

namespace Bial {

  SS::SS( FeatureDetector< int > *Fd ) : SS( Fd->Run( ) ) {
  }

  SS::SS( Vector < std::tuple < Image< int >, Image< int >> > detected ) : FeatureExtractor< int, double >( detected ) {
    this->SAMPLES = 120;
    this->MAXDIST = 0;
    this->SIDE = BOTH;
  }

  void SS::SetParameters( ParameterInterpreter *interpreter ) {
    Vector< parameter > vet;
    vet.push_back( std::tie( "samples", SAMPLES ) );
    vet.push_back( std::tie( "maxdist", MAXDIST ) );
    vet.push_back( std::tie( "side", SIDE ) );

    interpreter->SetExpectedParameters( vet );
    vet = interpreter->Interpret( );

    std::tie( std::ignore, SAMPLES ) = vet[ 0 ];
    std::tie( std::ignore, MAXDIST ) = vet[ 1 ];
    std::tie( std::ignore, SIDE ) = vet[ 2 ];
  }

  std::string SS::GetParameters( ParameterInterpreter *interpreter ) {
    Vector< parameter > vet;
    vet.push_back( std::tie( "samples", SAMPLES ) );
    vet.push_back( std::tie( "maxdist", MAXDIST ) );
    vet.push_back( std::tie( "side", SIDE ) );

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
      std::tie( img, mask ) = this->detected[ i ];
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

      bin = ImageOp::AddFrame( mbb, frame_img_coordinates, frame_img_size );

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
      Adjacency A = AdjacencyType::Circular( 1.5 );
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
