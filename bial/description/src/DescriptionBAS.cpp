#include "DescriptionBAS.hpp"
#include "GeometricsScale.hpp"
#include "Signal.hpp"

namespace Bial {

  BAS::BAS( FeatureDetector< int > *Fd ) try : BAS( Fd->Run( ) ) {
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

  BAS::BAS( const Vector< std::tuple< Image< int >, Vector< size_t > > > &detected ) try : 
    FeatureExtractor< int, double >( detected ) {
      this->SAMPLES = 40;
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

  void BAS::SetParameters( ParameterInterpreter *interpreter ) {
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

  std::string BAS::GetParameters( ParameterInterpreter *interpreter ) {
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
  
  BASfeature BAS::Run( ) {
    try {
      BASfeature feat;
      for( size_t i = 0; i < this->detected.size( ); ++i ) {
        const Image< int > &img( std::get< 0 >( detected[ i ] ) );
        const Vector< size_t > &mask( std::get< 1 >( detected[ i ] ) );
        size_t mask_size = mask.size( );
        if( mask_size == 0 ) {
          std::string msg( BIAL_ERROR( "Empty mask. " ) );
          throw( std::logic_error( msg ) );
        }
        Curve contour( ImageToCurve( img, mask ) );
        size_t cont_size = contour.size( );
        COMMENT( "Extract Features----------------------------------------------------------", 3 );
        Vector< int > mean( cont_size );
        Vector< int > second( cont_size );
        Vector< int > third( cont_size );
        size_t curvelength = cont_size / 2;
        Vector< int > bearing_array( curvelength - 1 );
        for( size_t j = 0; j < cont_size; j++ ) {
          double total = 0.0;
          int x1, y1;
          std::tie( x1, y1 ) = contour[ ( ( j - 1 ) + cont_size ) % cont_size ];
          int x2, y2;
          std::tie( x2, y2 ) = contour[ j ];
          int x3, y3;
          std::tie( x3, y3 ) = contour[ ( ( j + 1 ) + cont_size ) % cont_size ];
          int delta_x = x1 - x2;
          int delta_y = -( y1 - y2 );
          double angle_1 = find_angle( delta_x, delta_y );
          delta_x = x3 - x2;
          delta_y = -( y3 - y2 );
          double angle_2 = find_angle( delta_x, delta_y );
          double angle_curve;
          if( angle_1 >= angle_2 )
            angle_curve = angle_1 - angle_2;
          else
            angle_curve = 360.0 + angle_1 - angle_2;
          total += angle_curve;
          bearing_array[ 0 ] = static_cast< int >( angle_curve );
          double previous = angle_curve;
          for( size_t k = 2; k < static_cast< size_t >( curvelength ); ++k ) {
            std::tie( x1, y1 ) = contour[ ( ( j - k ) + cont_size ) % cont_size ];
            std::tie( x2, y2 ) = contour[ j ];
            std::tie( x3, y3 ) = contour[ ( ( j + k ) + cont_size ) % cont_size ];
            delta_x = x1 - x2;
            delta_y = -( y1 - y2 );
            angle_1 = find_angle( delta_x, delta_y );
            delta_x = x3 - x2;
            delta_y = -( y3 - y2 );
            angle_2 = find_angle( delta_x, delta_y );
            if( angle_1 >= angle_2 )
              angle_curve = angle_1 - angle_2;
            else
              angle_curve = 360.0 + angle_1 - angle_2;
            if( k > 3 ) {
              if( ( ( angle_curve - previous ) > 180 ) )
                angle_curve = angle_curve - 360.0;
              else if( ( ( previous - angle_curve ) > 180 ) )
                angle_curve = angle_curve + 360.0;
            }
            bearing_array[ k - 1 ] = static_cast< int >( angle_curve );
            total += angle_curve;
            previous = angle_curve;
          }
          mean[ j ] = static_cast< int >( total / ( curvelength - 1 ) );
          total = 0.0;
          for( size_t k = 0; k < static_cast< size_t >( curvelength - 1 ); k++ )
            total += std::pow( ( bearing_array[ k ] - mean[ j ] ), 2.0 );
          second[ j ] = std::pow( total / ( double ) ( curvelength - 2 ), 0.5 );
          total = 0.0;
          for( size_t k = 0; k < static_cast< size_t >( curvelength - 1 ); k++ )
            total += std::pow( std::abs( bearing_array[ k ] - mean[ j ] ), 3.0 );
          third[ j ] = std::pow( total / ( double ) ( curvelength - 2 ), ( 1.0 / 3.0 ) );
        }
        COMMENT( "Resample------------------------------------------------------------------", 3 );
        Image< int > img1( cont_size, 3 );
        size_t x_size = img1.size( 0 );
        for( size_t x = 0; x < cont_size; ++x ) {
          img1[ x ] = mean[ x ];
          img1[ x + x_size * 1 ] = second[ x ];
          img1[ x + x_size * 2 ] = third[ x ];
        }
        Image< int > img2( Geometrics::Scale( img1, ( static_cast< float >( SAMPLES ) ) / cont_size, 1, 1, true ) );
        size_t img_size = img2.size( );
        COMMENT( "histogram-----------------------------------------------------------------", 3 );
        Features< double > histogram( img_size );
        for( size_t j = 0; j < img_size; ++j )
          histogram[ j ] = img2[ j ];
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

