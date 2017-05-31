#include "DescriptionSS.hpp"
#include "AdjacencyRound.hpp"
#include "ImageFrame.hpp"
#include "MorphologyDilation.hpp"
#include "Signal.hpp"
#include "SortingSort.hpp"
#include "File.hpp"
#include "FileImage.hpp"

namespace Bial {

  SS::SS( FeatureDetector< int > *Fd ) try : SS( Fd->Run( ) ) {
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

  SS::SS( const Vector< std::tuple< Image< int >, Vector< size_t > > > &detected ) try :
    FeatureExtractor< int, double >( detected ) {
      this->SAMPLES = 120;
      this->MAXDIST = 0;
      this->SIDE = BOTH;
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
  

  void SS::SetParameters( ParameterInterpreter *interpreter ) {
    try {
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

  std::string SS::GetParameters( ParameterInterpreter *interpreter ) {
    try {
      Vector< parameter > vet;
      vet.push_back( std::tie( "samples", SAMPLES ) );
      vet.push_back( std::tie( "maxdist", MAXDIST ) );
      vet.push_back( std::tie( "side", SIDE ) );
      interpreter->SetExpectedParameters( vet );
      return( interpreter->GetExpectedParameters( ) );
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

  SSfeature SS::Run( ) {
    try {
      SSfeature feat;
      CurveDouble output;

      for( size_t i = 0; i < detected.size( ); ++i ) {
        const Image< int > &img( std::get< 0 >( detected[ i ] ) );
        size_t x_size = img.size( 0 );
        size_t y_size = img.size( 1 );
        size_t maxdist;
        if( this->MAXDIST != 0 )
          maxdist = this->MAXDIST;
        else
          maxdist = static_cast< size_t >( std::sqrt( x_size * x_size + y_size * y_size ) );
        size_t maxcost = maxdist * maxdist;
        Image< int > mbb( Mbb( img ) );
        Vector< size_t > frame_img_coordinates;
        frame_img_coordinates.push_back( maxdist );
        frame_img_coordinates.push_back( maxdist );
        Vector< size_t > frame_img_size;
        frame_img_size.push_back( x_size + 2 * maxdist );
        frame_img_size.push_back( y_size + 2 * maxdist );
        Image< int > bin( ImageOp::AddFrame( mbb, frame_img_coordinates, frame_img_size ) );
        Image< int > label( LabelContPixel( bin ) );
        x_size = bin.size( 0 );
        y_size = bin.size( 1 );
        size_t size = bin.size( );

        COMMENT( "Compute Euclidean IFT-----------------------------------------------------", 3 );
        Image< int > cost( x_size, y_size );
        for( size_t p = 0; p < size; ++p ) {
          if( label[ p ] > 0 )
            cost[ p ] = 0;
          else
            cost[ p ] = INT_MAX;
          label[ p ] = 0;
        }
        Adjacency A( AdjacencyType::Circular( 1.5 ) );
        /* bin = Morphology::Dilate(bin,A); */
        label = Morphology::Dilate( label, A );
        size_t Lmax = 0;
        for( size_t j = 0; j < size; ++j ) {
          if( static_cast< int >( Lmax ) < label[ j ] )
            Lmax = label[ j ];
        }
        CurveDouble inner( Lmax );
        CurveDouble outer( Lmax );
        CurveDouble diff( Lmax );
        for( size_t j = 0; j < Lmax; j++ )
          get< 0 >( diff[ j ] ) = get< 0 >( inner[ j ] ) = get< 0 >( outer[ j ] )
            = static_cast< double >( ( j * SAMPLES ) / Lmax );
        COMMENT( "Compute influence areas---------------------------------------------------", 3 );
        double nin, nout;
        nin = nout = 0.0;
        for( size_t p = 0; p < size; ++p ) {
          if( ( label[ p ] > 0 ) && ( cost[ p ] <= static_cast< int >( maxcost ) ) ) {
            if( bin[ p ] != 0 ) {
              ++nin;
              get< 1 >( inner[ label[ p ] - 1 ] )++;
            }
            else {
              ++nout;
              get< 1 >( outer[ label[ p ] - 1 ] )++;
            }
          }
        }
        int maxin = INT_MIN;
        int maxout = INT_MIN;
        for( size_t j = 0; j < Lmax; ++j ) {
          if( get< 1 >( inner[ j ] ) > maxin )
            maxin = get< 1 >( inner[ j ] );
          if( get< 1 >( outer[ j ] ) > maxout )
            maxout = get< 1 >( outer[ j ] );
        }
        for( size_t j = 0; j < Lmax; ++j ) {
          if( nin > 0 )
            get< 1 >( inner[ j ] ) /= nin;
          if( nout > 0 )
            get< 1 >( outer[ j ] ) /= nout;
          get< 1 >( diff[ j ] ) = get< 1 >( outer[ j ] ) - get< 1 >( inner[ j ] );
        }
        CurveDouble ninner( SAMPLES );
        CurveDouble nouter( SAMPLES );
        CurveDouble ndiff( SAMPLES );
        for( size_t j = 0; j < SAMPLES; ++j )
          get< 0 >( ninner[ j ] ) = get< 0 >( nouter[ j ] ) = get< 0 >( ndiff[ j ] ) = j;
        for( size_t j = 0; j < Lmax; j++ ) {
          get< 1 >( ninner[ ( int ) get< 0 >( inner[ j ] ) ] ) += get< 1 >( inner[ j ] );
          get< 1 >( nouter[ ( int ) get< 0 >( outer[ j ] ) ] ) += get< 1 >( outer[ j ] );
        }
        for( size_t j = 0; j < SAMPLES; ++j )
          get< 1 >( ndiff[ j ] ) = get< 1 >( nouter[ j ] ) - get< 1 >( ninner[ j ] );
        if( this->SIDE == INTERIOR )
          output = ninner;
        else if( this->SIDE == EXTERIOR )
          output = nouter;
        else if( this->SIDE == BOTH )
          output = ndiff;
        else
          throw "Invalid \"side\" option";

        Features< double > histogram( output.size( ) );
        size_t hist_size = histogram.size( );
        for( size_t j = 0; j < hist_size; ++j )
          histogram[ j ] = get< 1 >( output[ j ] );
        feat.push_back( histogram );
      }
      return( feat );
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
