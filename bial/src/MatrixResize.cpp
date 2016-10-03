/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2014/Jan/07
 * @brief Operations to get sub-matrix.
 */

#ifndef BIALMATRIXRESIZE_C
#define BIALMATRIXRESIZE_C

#include "MatrixResize.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_MatrixResize )
#define BIAL_EXPLICIT_MatrixResize
#endif

#if defined ( BIAL_EXPLICIT_MatrixResize ) || ( BIAL_IMPLICIT_BIN )

namespace Bial {
  
  /* Erros aqui! rever esta função e colocar a remoção de dimensões de tamanho 1. */
  template< class D >
  Matrix< D > MatrixOp::Resize( const Matrix< D > &mat, const Vector< size_t > &new_min,
                                const Vector< size_t > &new_max ) {
    try {
      COMMENT( "Number of elements of submatrix in each dimension.", 3 );
      size_t dims = mat.Dims( );
      Vector< size_t > new_size( 3, 1 );
      Vector< size_t > min_elm( new_min );
      Vector< size_t > max_elm( new_max );
      COMMENT( "Verifying new_min and new_max vector sizes.", 3 );
      if( new_min.size( ) != new_max.size( ) ) {
        std::string msg( BIAL_ERROR( "new_min vector end new_max vector sizes do not match!" ) );
        throw( std::logic_error( msg ) );
      }
      size_t min_size = min_elm.size( );
      COMMENT( "Dealing with 2D coordinates for 2D images.", 0 );
      if( ( min_size == 2 ) && ( dims == 3 ) ) {
        min_elm.push_back( 0 );
        max_elm.push_back( mat.size( min_size ) - 1 );
      }
      COMMENT( "Verifying if new_min <= new_max <= matrix dimensions.", 3 );
      for( size_t dms = 0; dms < dims; ++dms ) {
        if( min_elm[ dms ] > max_elm[ dms ] ) {
          min_elm[ dms ] = 0;
          BIAL_WARNING( "In Dimension: " << dms << " minimum was set to 0. ( new_min > new_max )." );
        }
        if( min_elm[ dms ] >= mat.size( dms ) ) {
          min_elm[ dms ] = 0;
          BIAL_WARNING( "In Dimension: " << dms << " minimum was set to 0. ( new_min >= this_max )." );
        }
        if( max_elm[ dms ] >= mat.size( dms ) ) {
          max_elm[ dms ] = mat.size( dms ) - 1;
          BIAL_WARNING( "In Dimension: " << dms << " maximum was set to " << mat.size( dms ) - 1 <<
                        ". ( new_max >= this_max )" );
        }
        COMMENT( "Setting the submatrix dimension sizes.", 4 );
        new_size[ dms ] = max_elm[ dms ] - min_elm[ dms ] + 1;
      }
      COMMENT( "Creating resultant submatrix.", 3 );
      Matrix< D > res( new_size );
      Vector< size_t > src_pxl( min_elm );
      size_t dms, tgt_pxl = 0;
      size_t elms = mat.size( );
      do {
        if( tgt_pxl < elms ) {
          res[ tgt_pxl ] = mat( src_pxl );
          ++tgt_pxl;
        }
        COMMENT( "Updating source element coordinates.", 4 );
        for( dms = 0; ( dms < dims ) && ( src_pxl[ dms ] == max_elm[ dms ] ); ++dms ) {
          src_pxl[ dms ] = min_elm[ dms ];
        }
        if( dms < dims ) {
          ++src_pxl[ dms ];
        }
      } while( dms < dims );
      COMMENT( "Dimensions: " << res.Dim( ), 4 );
      return( res );
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
  Matrix< D > MatrixOp::Resize( const Matrix< D > &mat, const std::string &coordinates ) {
    try {
      COMMENT( "Computing coodinates from string.", 3 );
      Matrix< size_t > coords = MatrixOp::Coordinates( mat, coordinates );
      COMMENT( "Creating min and max coordinates vector.", 3 );
      Vector< size_t > new_min( coords.size( 0 ) );
      Vector< size_t > new_max( coords.size( 0 ) );
      COMMENT( "Copying matrix to vectors.", 3 );
      for( size_t dms = 0; dms < coords.size( 0 ); ++dms ) {
        new_min( dms ) = coords( dms, 0 );
        new_max( dms ) = coords( dms, 1 );
      }
      COMMENT( "Calls the operator that interpret the matrix coordinates and returns the sub-matrix.", 3 );
      Matrix< D > res( MatrixOp::Resize( mat, new_min, new_max ) );
      return( res );
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
  Matrix< size_t > MatrixOp::Coordinates( const Matrix< D > &mat, const std::string &coordinates ) {
    try {
      COMMENT( "Input string: " << coordinates, 4 );
      Vector< std::string > dim_coord; /* Create vector to hold each coordinate */
      std::stringstream ss( coordinates ); /* Coordinates string is converted to a stringstream */
      std::string buf; /* Auxliliar string */
      COMMENT( "Using getline, we can separate the coordinates that are between each comma.", 4 );
      while( getline( ss, buf, ',' ) ) {
        dim_coord.push_back( buf );
      }
      size_t dimensions = mat.Dims( );
      if( dim_coord.size( ) > dimensions ) {
        std::string msg( BIAL_ERROR( "There are more coordinates than dimensions." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Number of dimensions found: " << dim_coord.size( ), 4 );
      Matrix< size_t > new_coords( dim_coord.size( ), 2 );
      for( size_t dim = 0; dim < dim_coord.size( ); ++dim ) {
        if( dim_coord[ dim ].find( ':' ) != std::string::npos ) {
          COMMENT( "Token contains operator ':'.", 4 );
          std::string left_coord, right_coord;
          COMMENT( "Token is converted into a stringstream ss2.", 4 );
          std::stringstream ss2( dim_coord[ dim ] );
          COMMENT( "Geting the string before the : operator", 4 );
          getline( ss2, left_coord, ':' );
          COMMENT( "Trying to convert the string to a valid size_t value.", 4 );
          try {
            new_coords( dim, 0 ) = std::stoul( left_coord );
            COMMENT( "Left coord " << dim << " of operator ':' found: " << left_coord, 4 );
          }
          catch( std::exception e ) {
            new_coords( dim, 0 ) = 0;
            COMMENT( "Left coord " << dim << " not found. Using 0.", 4 );
          }
          COMMENT( "Getting the string after operator ':'", 4 );
          getline( ss2, right_coord, ':' );
          try {
            new_coords( dim, 1 ) = std::stoul( right_coord );
            COMMENT( "Right coord " << dim << " of operator ':' found: " << right_coord, 4 );
          }
          catch( std::exception e ) {
            new_coords( dim, 1 ) = mat.Size( dim ) - 1;
            COMMENT( "Right coord " << dim << " not found. Using 0.", 4 );
          }
        }
        else {
          COMMENT( "There's no ':' operator. The coordinate C inside the token is interpreted as C:C.", 4 );
          try {
            new_coords( dim, 0 ) = std::stoul( dim_coord[ dim ] );
            new_coords( dim, 1 ) = new_coords( dim, 0 );
          }
          catch( std::exception e ) {
            new_coords( dim, 0 ) = 0;
            new_coords( dim, 1 ) = mat.Size( dim ) - 1;
          }
        }
      }
      COMMENT( "Compare matrix dimension and coordinate numbers.", 4 );
      if( dim_coord.size( ) < dimensions ) {
        for( size_t dm = dim_coord.size( ); dm < dimensions; ++dm ) {
          new_coords( dm, 0 ) = 0;
          new_coords( dm, 1 ) = mat.Size( dm ) - 1;
        }
      }
      COMMENT( "Final dimensions: " << new_coords, 4 );
      return( new_coords );
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

#ifdef BIAL_EXPLICIT_MatrixResize

  template Matrix< int > MatrixOp::Resize( const Matrix< int > &mat, const Vector< size_t > &new_min, 
                                           const Vector< size_t > &new_max );
  template Matrix< int > MatrixOp::Resize( const Matrix< int > &mat, const std::string &coordinates );
  template Matrix< size_t > MatrixOp::Coordinates( const Matrix< int > &mat, const std::string &coordinates );

  template Matrix< llint > MatrixOp::Resize( const Matrix< llint > &mat, const Vector< size_t > &new_min, 
                                             const Vector< size_t > &new_max );
  template Matrix< llint > MatrixOp::Resize( const Matrix< llint > &mat, const std::string &coordinates );
  template Matrix< size_t > MatrixOp::Coordinates( const Matrix< llint > &mat, const std::string &coordinates );

  template Matrix< float > MatrixOp::Resize( const Matrix< float > &mat, const Vector< size_t > &new_min, 
                                             const Vector< size_t > &new_max );
  template Matrix< float > MatrixOp::Resize( const Matrix< float > &mat, const std::string &coordinates );
  template Matrix< size_t > MatrixOp::Coordinates( const Matrix< float > &mat, const std::string &coordinates );

  template Matrix< double > MatrixOp::Resize( const Matrix< double > &mat, const Vector< size_t > &new_min, 
                                              const Vector< size_t > &new_max );
  template Matrix< double > MatrixOp::Resize( const Matrix< double > &mat, const std::string &coordinates );
  template Matrix< size_t > MatrixOp::Coordinates( const Matrix< double > &mat, const std::string &coordinates );

#endif

}

#endif

#endif
