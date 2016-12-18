/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Jul/05
 * @brief Adjacency relation with square format.
 * <br> Future add-on's: none.
 */

#ifndef BIALADJACENCYSQUARE_C
#define BIALADJACENCYSQUARE_C

#include "AdjacencySquare.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_AdjacencySquare )
#define BIAL_EXPLICIT_AdjacencySquare
#endif

#if defined ( BIAL_EXPLICIT_AdjacencySquare ) || ( BIAL_IMPLICIT_BIN )

#include "Image.hpp"
#include "Matrix.hpp"
#include "Color.hpp"
#include "ColorRGB.hpp"

namespace Bial {

  Adjacency AdjacencyType::Square( const Vector< size_t > &src_displacement, const Vector< size_t > &tgt_displacement )
  {
    try {
      size_t dimensions = src_displacement.size( );
      IF_DEBUG( dimensions != tgt_displacement.size( ) ) {
        std::string msg( BIAL_ERROR( "Source and target displament vector sizes do not match. Given: source size: " +
                                     std::to_string( dimensions ) + ", target size: " +
                                     std::to_string( tgt_displacement.size( ) ) ) );
        throw( std::logic_error( msg ) );
      }
      IF_DEBUG( ( dimensions < 2 ) || ( dimensions > 3 ) ) {
        std::string msg( BIAL_ERROR( "Adjacency must have 2 or 3 dimensions." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Copy of src and tgt vectors.", 0 );
      Vector< size_t > src( src_displacement );
      Vector< size_t > tgt( tgt_displacement );
      COMMENT( "Setting src displacements lower than tgt displaments, and computing number of adjacents.", 0 );
      size_t adjacents = 1;
      for( size_t dms = 0; dms < dimensions; ++dms ) {
        if( src[ dms ] > tgt[ dms ] ) {
          std::swap( src[ dms ], tgt[ dms ] );
        }
        adjacents *= ( tgt[ dms ] - src[ dms ] + 1 );
      }
      COMMENT( "Dimensions: " << dimensions << ", Adjacents: " << adjacents << ". Computing adjacents.", 0 );
      Adjacency adj( adjacents, dimensions );
      Vector< size_t > pxl_coord( src );
      for( size_t elm = 0; elm < adjacents; ++elm ) {
        COMMENT( "Pixel coord: " << pxl_coord << ". Assigning displacement coordinates.", 1 );
        for( size_t dms = 0; dms < dimensions; ++dms )
          adj( elm, dms ) = pxl_coord( dms );
        COMMENT( "Updating coordinates.", 1 );
        for( size_t dms = 0; dms < dimensions; ++dms ) {
          if( pxl_coord( dms ) < tgt( dms ) ) {
            ++pxl_coord( dms );
            break;
          }
          else
            pxl_coord( dms ) = src( dms );
        }
      }
      return( adj );
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

  template< class D > 
  Adjacency AdjacencyType::Square( const Matrix< D > &mat ) {
    try {
      size_t dims = mat.Dims( );
      size_t elms = mat.size( );
      IF_DEBUG( ( dims < 2 ) || ( dims > 3 ) ) {
        std::string msg( BIAL_ERROR( "Adjacency must have 2 or 3 dimensions." ) );
        throw( std::logic_error( msg ) );
      }
      IF_DEBUG( elms == 0 ) {
        std::string msg( BIAL_ERROR( "Empty adjacency relation." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Computing adjacency size.", 1 );
      size_t size = 0;
      for( size_t elm = 0; elm < elms; ++elm ) {
        if( mat[ elm ] != 0 )
          ++size;
      }
      IF_DEBUG( size == 0 ) {
        std::string msg( BIAL_ERROR( "Empty adjacency relation. Null matrix." ) );
        throw( std::logic_error( msg ) );
      }
      Adjacency adj( size, dims );
      COMMENT( "Setting adjacency displacements.", 1 );
      size_t idx = 0;
      for( size_t elm = 0; elm < elms; ++elm ) {
        if( mat[ elm ] != 0 ) {
          for( size_t dms = 0; dms < dims; ++dms )
            adj( idx, dms ) = mat.Coordinate( elm, dms ) - mat.size( dms ) / 2;
          ++idx;
        }
      }
      return( adj );
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
  
  template< class D > 
  Adjacency AdjacencyType::Square( const Image< D > &img ) {
    try {
      size_t dims = img.Dims( );
      size_t elms = img.size( );
      IF_DEBUG( ( dims < 2 ) || ( dims > 3 ) ) {
        std::string msg( BIAL_ERROR( "Adjacency must have 2 or 3 dimensions." ) );
        throw( std::logic_error( msg ) );
      }
      IF_DEBUG( elms == 0 ) {
        std::string msg( BIAL_ERROR( "Empty adjacency relation." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Computing adjacency size.", 1 );
      size_t size = 0;
      for( size_t elm = 0; elm < elms; ++elm ) {
        if( img[ elm ] != 0 )
          ++size;
      }
      if( size == 0 ) {
        std::string msg( BIAL_ERROR( "Empty adjacency relation. Null matrix." ) );
        throw( std::logic_error( msg ) );
      }
      Adjacency adj( size, dims );
      COMMENT( "Setting adjacency displacements.", 1 );
      size_t idx = 0;
      for( size_t elm = 0; elm < elms; ++elm ) {
        if( img[ elm ] != 0 ) {
          for( size_t dms = 0; dms < dims; ++dms )
            adj( idx, dms ) = img.Coordinate( elm, dms ) - img.Displacement( dms ) / 2;
          ++idx;
        }
      }
      return( adj );
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

  Adjacency AdjacencyType::Square( const Image< Color > &img ) {
    try {
      IF_DEBUG( ( img.Dims( ) < 2 ) || ( img.Dims( ) > 3 ) ) {
        std::string msg( BIAL_ERROR( "Adjacency must have 2 or 3 dimensions." ) );
        throw( std::logic_error( msg ) );
      }
      IF_DEBUG( img.size( ) == 0 ) {
        std::string msg( BIAL_ERROR( "Empty adjacency relation." ) );
        throw( std::logic_error( msg ) );
      }
      Image< int > gray_img( ColorSpace::ARGBtoGraybyBrightness< int >( img ) );
      return( AdjacencyType::Square( gray_img ) );
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
  
#ifdef BIAL_EXPLICIT_AdjacencySquare

  /* Explicit instantiations. --------------------------------------------------------------------------------------- */

#endif

}

#endif

#endif
