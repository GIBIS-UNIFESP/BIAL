/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2014/Mar/14
 * @brief Functions to draw forms in images.
 */

#ifndef BIALDRAWSEQUENCE_C
#define BIALDRAWSEQUENCE_C

#include "DrawSequence.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_DrawSequence )
#define BIAL_EXPLICIT_DrawSequence
#endif

#if defined ( BIAL_EXPLICIT_DrawSequence ) || ( BIAL_IMPLICIT_BIN )

#include "AdjacencyIterator.hpp"
#include "Color.hpp"
#include "DrawLine.hpp"
#include "DrawIntersection.hpp"
#include "Image.hpp"
#include "RealColor.hpp"

namespace Bial {

  template< class D > Sequence::Sequence( const Image< D > &label, size_t start_pxl, Adjacency &adj,
                                          const Color color ) try : Figure< Sequence >( color ) {
    COMMENT( "Inserting pixels into the sequence.", 2 );
    Image< int > visited( label.Dim( ) );
    SequenceCreator( label, start_pxl, visited, adj );
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

  template< class D >
  void Sequence::Draw( Image< D > &img ) const {
    try {
      if( point.size( ) == 0 ) {
        return;
      }
      COMMENT( "Verify if image dimensions match sequence point coordinate dimensions.", 2 );
      if( img.Dims( ) != point[ 0 ].size( ) ) {
        std::string msg( BIAL_ERROR( std::string( "Incompatible number of dimensions between image and sequence" ) +
                                     "point coordinates." ) );
        throw( std::logic_error( msg ) );
      }
      Draw( img, Vector< float >( img.Dims( ), 0.0f ) );
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
  void Sequence::Draw( Image< D > &img, const Vector< float > &displacement ) const {
    try {
      if( point.size( ) == 0 ) {
        return;
      }
      COMMENT( "Verify if image dimensions match sequence point coordinate dimensions.", 2 );
      if( img.Dims( ) != point[ 0 ].size( ) ) {
        std::string msg( BIAL_ERROR( std::string( "Incompatible number of dimensions between image and sequence" ) +
                                     "point coordinates." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Verify if displacement dimensions match sequence point coordinate dimensions.", 2 );
      if( displacement.size( ) != point[ 0 ].size( ) ) {
        std::string msg( BIAL_ERROR( std::string( "Incompatible number of dimensions between image and sequence" ) +
                                     "point coordinates." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Computing color value.", 2 );
      float maximum = std::max< float >( 255.0f, img.Maximum( ) );
      float alpha = this->color( 0 ) / 255.0f;
      float assgn_color = ( this->color( 1 ) + this->color( 2 ) + this->color( 3 ) ) * maximum / ( 3.0f * 255.0f );

      COMMENT( "Drawing Sequence.", 2 );
      for( size_t pnt = 0; pnt < point.size( ); ++pnt ) {
        size_t pxl = point[ pnt ][ 0 ] + displacement[ 0 ];
        for( size_t dms = 1; dms < img.Dims( ); ++dms ) {
          pxl += ( point[ pnt ][ dms ] + displacement[ dms ] ) * img.Displacement( dms - 1 );
        }
        img( pxl ) = alpha * assgn_color + ( 1.0f - alpha ) * img( pxl );
      }
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

  template< >
  void Sequence::Draw( Image< Color > &img, const Vector< float > &displacement ) const {
    try {
      if( point.size( ) == 0 ) {
        return;
      }
      COMMENT( "Verify if image dimensions match sequence point coordinate dimensions.", 2 );
      if( img.Dims( ) != point[ 0 ].size( ) ) {
        std::string msg( BIAL_ERROR( std::string( "Incompatible number of dimensions between image and sequence" ) +
                                     "point coordinates." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Verify if displacement dimensions match sequence point coordinate dimensions.", 2 );
      if( displacement.size( ) != point[ 0 ].size( ) ) {
        std::string msg( BIAL_ERROR( std::string( "Incompatible number of dimensions between image and sequence" ) +
                                     "point coordinates." ) );
        throw( std::logic_error( msg ) );
      }

      COMMENT( "Computing color value.", 2 );
      RealColor max_color( img.Maximum( ) );
      float maximum = std::max( 255.0f, std::max( max_color( 1 ), std::max( max_color( 2 ), max_color( 3 ) ) ) );
      COMMENT( "maximum: " << maximum, 0 );
      RealColor assgn_color( this->color( 0 ), this->color( 1 ) * maximum / 255.0f,
                             this->color( 2 ) * maximum / 255.0f, this->color( 3 ) * maximum / 255.0f );
      float alpha = assgn_color( 0 ) / 255.0f;

      COMMENT( "Drawing Sequence.", 2 );
      for( size_t pnt = 0; pnt < point.size( ); ++pnt ) {
        size_t pxl = point[ pnt ][ 0 ] + displacement[ 0 ];
        for( size_t dms = 1; dms < img.Dims( ); ++dms ) {
          pxl += ( point[ pnt ][ dms ] + displacement[ dms ] ) * img.Displacement( dms - 1 );
        }
        for( size_t clr = 1; clr < 4; ++clr ) {
          img( pxl )( clr ) = alpha * assgn_color( clr ) + ( 1.0f - alpha ) * img( pxl )( clr );
          pxl += img.size( );
        }
      }
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

  Vector< Vector< int > > Sequence::Pixels( ) const {
    if( point.size( ) == 0 ) {
      return( Vector< size_t >( ) );
    }
    return( Pixels( Vector< float >( point[ 0 ].size( ), 0.0f ) ) );
  }

  Vector< Vector< int > > Sequence::Pixels( const Vector< float > &displacement ) const {
    try {
      if( point.size( ) == 0 ) {
        return( Vector< Vector< int > >( ) );
      }
      COMMENT( "Verify if displacement dimensions match sequence point coordinate dimensions.", 2 );
      if( displacement.size( ) != point[ 0 ].size( ) ) {
        std::string msg( BIAL_ERROR( std::string( "Incompatible number of dimensions between image and sequence" ) +
                                     "point coordinates." ) );
        throw( std::logic_error( msg ) );
      }
      Vector< Vector< int > > res;
      if( point[ 0 ].size( ) == 2 ) {
        COMMENT( "2D pixels.", 2 );
        for( size_t pnt = 0; pnt < point.size( ); ++pnt ) {
          res.push_back( { static_cast< int >( point[ pnt ][ 0 ] ), static_cast< int >( point[ pnt ][ 1 ] ) } );
        }
      }
      else if( point[ 0 ].size( ) == 3 ) {
        COMMENT( "3D pixels.", 2 );
        for( size_t pnt = 0; pnt < point.size( ); ++pnt ) {
          res.push_back( { static_cast< int >( point[ pnt ][ 0 ] ), static_cast< int >( point[ pnt ][ 1 ] ),
                           static_cast< int >( point[ pnt ][ 2 ] ) } );
        }
      }
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

  template< class O >
  void Sequence::Print( O &os ) const {
    try {
      os << "Sequence of points" << std::endl;
      os << "Color(s): ";
      for( size_t clr = 0; clr < 4; ++clr ) {
        os << this->color[ clr ] << " ";
      }
      os << std::endl;
      os << "Points: ";
      if( point[ 0 ].size( ) == 2 ) {
        COMMENT( "2D pixels.", 2 );
        for( size_t pnt = 0; pnt < point.size( ); ++pnt ) {
          os << point[ pnt ][ 0 ] << ", " << point[ pnt ][ 1 ] << std::endl;
        }
      }
      else if( point[ 0 ].size( ) == 3 ) {
        COMMENT( "3D pixels.", 2 );
        for( size_t pnt = 0; pnt < point.size( ); ++pnt ) {
          os << point[ pnt ][ 0 ] << ", " << point[ pnt ][ 1 ] << ", " << point[ pnt ][ 2 ] << std::endl;
        }
      }
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
  void Sequence::SequenceCreator( const Image< D > &label, size_t start_pxl, Image< int > &visited,
                                  Adjacency &adj ) {
    try {
      point.push_back( label.Coordinates( start_pxl ) );
      visited[ start_pxl ] = 1;
      const D value = label[ start_pxl ];
      AdjacencyIterator adj_itr( label, adj );
      size_t adj_size = adj.size( );
      size_t adj_pxl;
      for( size_t idx = 1; idx < adj_size; ++idx ) {
        if( ( adj_itr.AdjIdx( start_pxl, idx, adj_pxl ) ) && ( label[ adj_pxl ] == value ) &&
            ( !visited[ adj_pxl ] ) ) {
          SequenceCreator( label, adj_pxl, visited, adj );
          return;
        }
      }
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

  double Sequence::size( ) const {
    return( static_cast< double >( point.size( ) ) );
  }

  bool Sequence::ValidateSize( float min_sequence_size, float max_sequence_size ) {
    if( ( min_sequence_size < 10.0f ) || ( min_sequence_size > max_sequence_size ) ) {
      std::string msg( BIAL_ERROR( "Invalid minimum and maximum sequence sizes. Expected maximum >= minimum >= 10. " +
                                   "Given: Minimum: " + std::to_string( min_sequence_size ) + ", maximum: " +
                                   std::to_string( max_sequence_size ) ) );
      throw( std::runtime_error( msg ) );
    }
    if( ( point.size( ) < min_sequence_size ) || ( point.size( ) > max_sequence_size ) ) {
      return( false );
    }
    return( true );
  }

  bool Sequence::ValidateCircularity( Vector< float > &center, float min_center_dist, float max_center_dist,
                                      float min_radius, float max_radius, float min_circularity,
                                      float max_circularity ) {
    try {
      COMMENT( "Verifying input parameters.", 1 );
      if( point.size( ) < 10 ) {
        std::string msg( BIAL_ERROR( "Sequence size must be greater or equal to 10. Given: " +
                                     std::to_string( point.size( ) ) ) );
        throw( std::runtime_error( msg ) );
      }
      if( ( center.size( ) != point[ 0 ].size( ) ) && ( center.size( ) != 2 ) ) {
        std::string msg( BIAL_ERROR( "Center dimensions and sequence dimensions must be equal to 2. Given: " +
                                     " center dimensions: " + std::to_string(
                                       center.size( ) ) + ", sequence dimensions: "
                                     + std::to_string( point[ 0 ].size( ) ) ) );
        throw( std::runtime_error( msg ) );
      }
      if( min_center_dist > max_center_dist ) {
        std::string msg( BIAL_ERROR( "Invalid minimum and maximum distances to center. Expected maximum >= minimum. " +
                                     "Given: Minimum: " + std::to_string( min_center_dist ) + ", maximum: " +
                                     std::to_string( max_center_dist ) ) );
        throw( std::runtime_error( msg ) );
      }
      if( min_radius > max_radius ) {
        std::string msg( BIAL_ERROR( "Invalid minimum and maximum radius. Expected maximum >= minimum. " +
                                     "Given: Minimum: " + std::to_string( min_radius ) + ", maximum: " +
                                     std::to_string( max_radius ) ) );
        throw( std::runtime_error( msg ) );
      }
      if( min_circularity > max_circularity ) {
        std::string msg( BIAL_ERROR( "Invalid minimum and maximum circularity. Expected maximum >= minimum. " +
                                     "Given: Minimum: " + std::to_string( min_circularity ) + ", maximum: " +
                                     std::to_string( max_circularity ) ) );
        throw( std::runtime_error( msg ) );
      }
      COMMENT( "Computing estimated center.", 2 );
      Vector< float > center_coord( CircleCenter( max_radius > 10000.0 ? 10000.0 : max_radius ) );
      if( center_coord.empty( ) ) {
        COMMENT( "Crosspoint does not exists.", 2 );
        return( false );
      }
      COMMENT( "Checking if center is in image range.", 2 );
      float center_dist = std::sqrt( ( center_coord[ 0 ] - center[ 0 ] ) * ( center_coord[ 0 ] - center[ 0 ] ) +
                                     ( center_coord[ 1 ] - center[ 1 ] ) * ( center_coord[ 1 ] - center[ 1 ] ) );
      if( ( center_dist < min_center_dist ) || ( center_dist > max_center_dist ) ) {
        COMMENT( "Estimated center is out of range.", 2 );
        return( false );
      }
      COMMENT( "Checking if it is a circle sequence with the expected dimensions.", 2 );
      float radius = MeanRadius( center_coord, 0, point.size( ) );
      if( ( radius < min_radius ) || ( radius > max_radius ) ) {
        COMMENT( "Radius is out of range.", 2 );
        return( false );
      }
      COMMENT( "Checking if the circularity of the sequence is within the expected dimensions.", 2 );
      float circularity = Circularity( center_coord, radius, 0, point.size( ) );
      if( ( circularity < min_circularity ) || ( circularity > max_circularity ) ) {
        COMMENT( "Circularity is out of range.", 2 );
        return( false );
      }
      return( true );
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

  Vector< float > Sequence::CircleCenter( float max_radius ) {
    COMMENT( "Size passed: " << point.size( ) << ".", 1 );
    Vector< float > ext1( *( point.begin( ) + 5 ) );
    Vector< float > ext2( *( point.end( ) - 5 ) );
    COMMENT( "Creating the bisection of the sequence extremities.", 2 );
    Line bsl( Line::BisectionLine( ext1, ext2, max_radius ) );
    COMMENT( "Intersection between bisection line and the sequence.", 2 );
    Vector< float > crosspoint( Intersection( *this, bsl ) );
    COMMENT( "Verify if found crosspoint.", 2 );
    if( crosspoint.empty( ) ) {
      return( Vector< float >( ) );
    }
    COMMENT( "Crosspoint passed: " << crosspoint << ".", 1 );
    COMMENT( "Finding the coordinates of the bisections between the crosspoint, and the extremities coordinates", 2 );
    Line ln1( Line::BisectionLine( ext1, crosspoint, max_radius ) );
    Line ln2( Line::BisectionLine( ext2, crosspoint, max_radius ) );
    COMMENT( "Intersection of these two bisections.", 2 );
    return( Intersection( ln1, ln2 ) );
  }

  float Sequence::MeanRadius( const Vector< float > &center, size_t begin, size_t end ) {
    try {
      double total = 0.0;
      size_t elms = point.size( );
      for( size_t pos = begin; pos < end; ++pos ) {
        double elm_x = point[ pos ][ 0 ];
        double elm_y = point[ pos ][ 1 ];
        double dist_x = elm_x - center[ 0 ];
        double dist_y = elm_y - center[ 1 ];
        total += std::sqrt( ( dist_x * dist_x ) + ( dist_y * dist_y ) );
      }
      return( total / elms );
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

  float Sequence::Circularity( const Vector< float > &center, float radius, size_t begin, size_t end ) {
    try {
      double total = 0.0;
      for( size_t pos = begin; pos < end; ++pos ) {
        double elm_x = point[ pos ][ 0 ];
        double elm_y = point[ pos ][ 1 ];
        double dist_x = elm_x - center[ 0 ];
        double dist_y = elm_y - center[ 1 ];
        total += ( std::sqrt( ( dist_x * dist_x ) + ( dist_y * dist_y ) ) - radius );
      }
      return( total / radius );
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

  template< class O >
  O &operator<<( O &os, const Sequence &obj ) {
    obj.Print( os );
    return( os );
  }


#ifdef BIAL_EXPLICIT_DrawSequence

  template Sequence::Sequence( const Image< int > &label, size_t start_pxl, Adjacency &adj, const Color color );
  template void Sequence::SequenceCreator( const Image< int > &label, size_t start_pxl,
                                           Image< int > &visited, Adjacency &adj );
  template Sequence::Sequence( const Image< llint > &label, size_t start_pxl, Adjacency &adj, const Color color );
  template void Sequence::SequenceCreator( const Image< llint > &label, size_t start_pxl,
                                           Image< int > &visited, Adjacency &adj );
  template Sequence::Sequence( const Image< float > &label, size_t start_pxl, Adjacency &adj, const Color color );
  template void Sequence::SequenceCreator( const Image< float > &label, size_t start_pxl,
                                           Image< int > &visited, Adjacency &adj );
  template Sequence::Sequence( const Image< double > &label, size_t start_pxl, Adjacency &adj, const Color color );
  template void Sequence::SequenceCreator( const Image< double > &label, size_t start_pxl,
                                           Image< int > &visited, Adjacency &adj );
  template Sequence::Sequence( const Image< Color > &label, size_t start_pxl, Adjacency &adj, const Color color );
  template void Sequence::SequenceCreator( const Image< Color > &label, size_t start_pxl,
                                           Image< int > &visited, Adjacency &adj );

  template void Sequence::Print( OFile &os ) const;
  template void Sequence::Print( std::ostream &os ) const;
  template OFile &operator<<( OFile &os, const Sequence &obj );
  template std::ostream &operator<<( std::ostream &os, const Sequence &obj );

#endif

}

#endif

#endif
