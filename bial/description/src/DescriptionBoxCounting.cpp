#include "DescriptionBoxCounting.hpp"
#include "DescriptionFeatureExtractor.hpp"
#include "GeometricsScale.hpp"
#include "Signal.hpp"

namespace Bial {

  BoxCounting::BoxCounting( FeatureDetector< int > *Fd ) try : BoxCounting( Fd->Run( ) ) {
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

  BoxCounting::BoxCounting( const Vector< std::tuple< Image< int >, Vector< size_t > > > &detected ) try : 
    FeatureExtractor< int, double >( detected ) {
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
  
  void BoxCounting::SetParameters( ParameterInterpreter *interpreter ) {
  }

  std::string BoxCounting::GetParameters( ParameterInterpreter *interpreter ) {
    return( "" );
  }

  BoxCountingfeature BoxCounting::Run( ) {
    try {
      size_t SIZE = 11;
      BoxCountingfeature feat;
      Image< int > scaled_img;
      Image< int > scaled_mask;
      for( size_t i = 0; i < this->detected.size( ); ++i ) {
        const Image< int > &img( std::get< 0 >( detected[ i ] ) );
        const Vector< size_t > &mask( std::get< 1 >( detected[ i ] ) );
        size_t x_size = img.size( 0 );
        size_t y_size = img.size( 1 );
        size_t mask_size = mask.size( );
        size_t img_size = img.size( );
        if( mask_size == 0 ) {
          std::string msg( BIAL_ERROR( "Empty mask. " ) );
          throw( std::logic_error( msg ) );
        }
        Image< int > mask_img( img.Dim( ) );
        for( size_t pxl = 0; pxl < img.size( ); ++pxl )
          mask_img[ mask[ pxl ] ] = 1;
        size_t size = 1 << ( SIZE - 1 );
        if( ( img.size( 0 ) > size ) && ( img.size( 1 ) > size ) ) {
          scaled_img = Geometrics::Scale( img, x_size / ( float ) size, y_size / ( float ) size, 1.0 );
          scaled_mask = Geometrics::Scale( mask_img, x_size / ( float ) size, y_size / ( float ) size, 1.0 );
        }
        else if( img.size( 0 ) > size ) {
          scaled_img = Geometrics::Scale( img, x_size / ( float ) size, y_size, 1.0 );
          scaled_mask = Geometrics::Scale( mask_img, x_size / ( float ) size, y_size, 1.0 );
        }
        else if( img.size( 1 ) > size ) {
          scaled_img = Geometrics::Scale( img, x_size, y_size / ( float ) size, 1.0 );
          scaled_mask = Geometrics::Scale( mask_img, x_size, y_size / ( float ) size, 1.0 );
        }
        else {
          scaled_img = Image< int >( img );
          scaled_mask = Image< int >( mask_img );
        }
        Image< int > mbb( Mbb( scaled_img, scaled_mask ) );
        x_size = mbb.size( 0 );
        y_size = mbb.size( 1 );
        COMMENT( "square image--------------------------------------------------------------", 3 );
        Image< int > sqr_img( size, size );
        int offset = size / 2 - x_size / 2 + size * ( size / 2 - y_size / 2 );
        for( size_t y = 0; y < y_size; ++y ) {
          for( size_t x = 0; x < x_size; ++x ) {
            if( mask_img( x, y ) != 0 )
              sqr_img[ ( x + offset ) + size * y ] = mbb[ x + x_size * y ];
          }
        }
        COMMENT( "histogram-----------------------------------------------------------------", 3 );
        Features< double > histogram( SIZE );
        Vector< int > counter( SIZE );
        for( size_t pot = 1; pot <= size; pot *= 2 ) {
          size_t incx = size / pot;
          size_t incy = size / pot;
          for( size_t x = 0; x < size; x += incx ) {
            for( size_t y = 0; y < size; y += incy ) {
              size_t flag = 0;
              for( size_t xlinha = x; ( ( xlinha < x + incx ) && ( flag == 0 ) ); ++xlinha ) {
                for( size_t ylinha = y; ( ( ylinha < y + incy ) && ( flag == 0 ) ); ++ylinha ) {
                  if( sqr_img[ xlinha + size * ylinha ] != 0 ) {
                    flag = 1;
                    int index = ( int ) ( log( pot ) / log( 2 ) );
                    histogram[ index ] = std::log( static_cast< double >( pot / size ) );
                    ++counter[ index ];
                  }
                }
              }
            }
          }
        }
        for( size_t j = 0; j < SIZE; ++j )
          histogram[ j ] = log( counter[ j ] );
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
