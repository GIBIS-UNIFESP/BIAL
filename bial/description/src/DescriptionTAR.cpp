#include "DescriptionTAR.hpp"
#include "Geometrics.hpp"
#include "Signal.hpp"
#include "SortingSort.hpp"
#include "GeometricsScale.hpp"

namespace Bial {

  Vector< double > TAR::ComputeScale( const Curve &contour, size_t n, size_t ts ) {
    try {
      Vector< double > tar( n );
      for( size_t i = 0; i < tar.size( ); ++i ) {
        size_t l = ( n + i - ts ) % n;
        size_t r = ( i + ts ) % n;
        tar[ i ] = 0.5 * ( get< 0 >( contour[ l ] ) * get< 1 >( contour[ i ] ) + get< 0 >( contour[ i ] ) *
                           get< 1 >( contour[ r ] ) + get< 0 >( contour[ r ] ) * get< 1 >( contour[ l ] ) -
                           get< 0 >( contour[ r ] ) * get< 1 >( contour[ i ] ) - get< 0 >( contour[ i ] ) *
                           get< 1 >( contour[ l ] ) - get< 0 >( contour[ l ] ) * get< 1 >( contour[ r ] ) );
      }
      return( tar );
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

  Vector< Vector< double > > TAR::ComputeSignature( const Curve &contour, size_t n ) {
    try {
      size_t nscales = ( n - 1 ) / 2;
      Vector< Vector< double > > tar( nscales );
      for( size_t ts = 0; ts < static_cast< size_t >( nscales ); ++ts ) {
        Vector< double > scale = ComputeScale( contour, n, ts + 1 );
        Vector< double > abs_scale( scale );
        abs_scale.Abs( );
        double max = abs_scale.Maximum( );
        if( max != 0 )
          scale /= max;
        else
        {
          std::cout<< "divisao por zero";
          throw "divisao por zero";
        }
        tar[ ts ] = scale;
      }
      return( tar );
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

  double TAR::ShapeComplexity( const Vector< Vector< double > > &tar, size_t samples ) {
    try {
      std::cout << "\n\nalive\n\n" << '\n';
      double sc = 0.0;
      size_t nscales = ( samples - 1 ) / 2;
      Vector< Vector< double > > reverse_tar( samples );
      for( size_t i = 0; i < static_cast< size_t >( samples ); ++i ) {
        for( size_t ts = 0; ts < static_cast< size_t >( nscales ); ++ts )
          reverse_tar[ i ].push_back( tar[ ts ][ i ] );
      }
      for( size_t i = 0; i < static_cast< size_t >( samples ); ++i ) {
        double max = reverse_tar[ i ].Maximum( );
        double min = reverse_tar[ i ].Minimum( );
        sc += std::abs( max - min );
      }
      sc /= samples;
      return( sc );
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

  TAR::TAR( FeatureDetector< int > *Fd ) try : TAR( Fd->Run( ) ) {
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

  TAR::TAR( const Vector< std::tuple< Image< int >, Vector< size_t > > > &detected ) try :
    FeatureExtractor< int, double >( detected ) {
      this->SAMPLES = 128;
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

  void TAR::SetParameters( ParameterInterpreter *interpreter ) {
    try {
      Vector< parameter > vet;
      vet.push_back( std::tie( "SAMPLES", SAMPLES ) );
      interpreter->SetExpectedParameters( vet );
      vet = interpreter->Interpret( );
      std::tie( std::ignore, SAMPLES ) = vet[ 0 ];
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

  std::string TAR::GetParameters( ParameterInterpreter *interpreter ) {
    try {
      Vector< parameter > vet;
      vet.push_back( std::tie( "SAMPLES", SAMPLES ) );
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

  TARfeature TAR::Run( ) {
    try {
      TARfeature feat;
      for( size_t i = 0; i < this->detected.size( ); ++i ) {
        const Image< int > &img( std::get< 0 >( detected[ i ] ) );
        const Vector< size_t > &mask( std::get< 1 >( detected[ i ] ) );
        Curve contour( ImageToCurve( img, mask ) );
        size_t cont_size = contour.size( );
        COMMENT( "Resample------------------------------------------------------------------", 3 );
        Image< int > img1( cont_size, 2 );
        for( size_t x = 0; x < cont_size; ++x )
          std::tie( img1[ x ], img1[ x + cont_size ] ) = contour[ x ];
        Image< int > img2( Geometrics::Scale( img1, ( ( ( float ) SAMPLES ) / cont_size ), 1.0, true ) );
        Curve rcontour( SAMPLES );
        for( size_t x = 0; x < SAMPLES; ++x )
          rcontour[ x ] = std::tie( img2[ x ], img2[ x + cont_size ] );
        std::cout<<"still alive\n\n\n\n\n";
        Vector< Vector< double > > tar = ComputeSignature( rcontour, SAMPLES );
        double sc = ShapeComplexity( tar, SAMPLES );
        COMMENT( "Histogram-----------------------------------------------------------------", 3 );
        Features< double > histogram( tar.size( ) * SAMPLES + 1 );
        size_t p = 0;
        for( size_t ts = 0; ts < tar.size( ); ++ts ) {
          for( size_t j = 0; j < tar[ ts ].size( ); ++j )
            histogram[ p++ ] = tar[ ts ][ j ];
        }
        histogram[ p ] = sc;
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
