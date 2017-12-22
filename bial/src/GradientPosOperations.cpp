/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/21 
 * @brief Operations to be executed after defining the gradient.
 */

#ifndef BIALGRADIENTPOSOPERATIONS_C
#define BIALGRADIENTPOSOPERATIONS_C

#include "GradientPosOperations.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_GradientPosOperations )
#define BIAL_EXPLICIT_GradientPosOperations
#endif
#if defined ( BIAL_EXPLICIT_GradientPosOperations ) || ( BIAL_IMPLICIT_BIN )

#include "AdjacencyGrayCode.hpp"
#include "AdjacencyIterator.hpp"
#include "AdjacencyRound.hpp"
#include "Bit.hpp"
#include "DrawSequence.hpp"
#ifdef BIAL_DEBUG
#include "FileImage.hpp"
#endif
#include "Image.hpp"
#include "SegmentationConnectedComponents.hpp"

namespace Bial {

  template< class D >
  Image< D > Gradient::CloseHoles( const Image< D > &canny ) {
    try {
      Image< D > res( canny );
      if( canny.Dims( ) == 2 ) {
        Adjacency adj_rel = AdjacencyType::Gray8( );
        AdjacencyIterator adj_itr( canny, adj_rel );
        size_t img_xsize = canny.size( 0 );
        size_t img_ysize = canny.size( 1 );
        size_t img_size = canny.size( );
        for( size_t elm = 0; elm < img_size; ++elm ) {
          div_t elm_by_imgx = std::div( static_cast< int >( elm ), static_cast< int >( img_xsize ) );
          if( ( elm_by_imgx.rem > 0 ) && ( elm_by_imgx.rem < static_cast< int >( img_xsize ) - 1 ) &&
              ( elm_by_imgx.quot > 0 ) && ( elm_by_imgx.quot < static_cast< int >( img_ysize ) - 1 ) &&
              ( canny[ elm ] == 0 ) ) {
            for( int adj_dir = 7; adj_dir != 4; adj_dir = ( adj_dir + 1 ) % 8 ) {
              size_t adj_elm = adj_itr( elm, adj_dir );
              if( canny[ adj_elm ] != 0 ) {
                COMMENT( "Three opposed directions.", 4 );
                size_t opp_dir = Bit::Flip( Bit::Flip( adj_dir, 0 ), 2 );
                size_t opp_elm = adj_itr( elm, opp_dir );
                if( canny[ opp_elm ] != 0 ) {
                  COMMENT( "0 and 2 elm: " << canny.Coordinates( elm ) << ", adj_elm: " << canny.Coordinates( adj_elm )
			   << ", opp_elm: " << canny.Coordinates(
								 opp_elm ) << ", adj_dir: " << adj_dir <<
                           ", opp_dir: " << opp_dir, 0 );
                  int side1_dir = Bit::Flip( Bit::Flip( adj_dir, 0 ), 1 );
                  int side2_dir = Bit::Flip( Bit::Flip( adj_dir, 2 ), 1 );
                  size_t side1_elm = adj_itr( elm, side1_dir );
                  size_t side2_elm = adj_itr( elm, side2_dir );
                  if( ( canny[ side1_elm ] == 0 ) && ( canny[ side2_elm ] == 0 ) ) {
                    res[ elm ] = canny[ adj_elm ];
                    break;
                  }
                }
                else {
                  opp_dir = Bit::Flip( opp_dir, 1 );
                  opp_elm = adj_itr( elm, opp_dir );
                  if( canny[ opp_elm ] != 0 ) {
                    COMMENT( "1, 2, 3 elm: " << canny.Coordinates( elm ) << ", adj_elm: " <<
                             canny.Coordinates( adj_elm ) << ", opp_elm: " << canny.Coordinates( opp_elm ) <<
                             ", adj_dir: " << adj_dir << ", opp_dir: " << opp_dir, 0 );
                    int side1_dir = Bit::Flip( adj_dir, Bit::IsZero( adj_dir, 1 ) ? 0 : 2 );
                    int side2_dir = Bit::Flip( side1_dir, 1 );
                    size_t side1_elm = adj_itr( elm, side1_dir );
                    size_t side2_elm = adj_itr( elm, side2_dir );
                    if( ( canny[ side1_elm ] == 0 ) && ( canny[ side2_elm ] == 0 ) ) {
                      res[ elm ] = canny[ adj_elm ];
                      break;
                    }
                  }
                  else {
                    opp_dir = Bit::Flip( adj_dir, Bit::IsZero( adj_dir, 1 ) ? 2 : 0 );
                    opp_elm = adj_itr( elm, opp_dir );
                    if( canny[ opp_elm ] != 0 ) {
                      COMMENT( "0 or 2 elm: " << canny.Coordinates( elm ) << ", adj_elm: " <<
                               canny.Coordinates( adj_elm ) << ", opp_elm: " << canny.Coordinates( opp_elm ) <<
                               ", adj_dir: " << adj_dir << ", opp_dir: " << opp_dir, 0 );
                      int side1_dir = Bit::Flip( adj_dir, 1 );
                      int side2_dir = Bit::Flip( side1_dir, Bit::IsZero( adj_dir, 1 ) ? 2 : 0 );
                      size_t side1_elm = adj_itr( elm, side1_dir );
                      size_t side2_elm = adj_itr( elm, side2_dir );
                      if( ( canny[ side1_elm ] == 0 ) && ( canny[ side2_elm ] == 0 ) ) {
                        res[ elm ] = canny[ adj_elm ];
                        break;
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
      else {
        std::string msg( BIAL_ERROR( "CloseHoles not implemented for 3D images yet!" ) );
        throw( std::logic_error( msg ) );
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

  template< class D >
  void Gradient::RemoveRedundantPoint( Image< D > &canny ) {
    try {
      if( canny.Dims( ) != 2 ) {
        std::string msg( BIAL_ERROR( "Requires 2D Canny image. Given: " + std::to_string( canny.Dims( ) ) ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Creating redundancy image and adjacency relation.", 0 );
      Image< int > redundant( canny.Dim( ) );
      redundant.Set( 0 );
      Adjacency adj = AdjacencyType::Circular( 1.45 );
      AdjacencyIterator adj_itr( canny, adj );
      size_t adj_size = adj.size( );
      size_t img_size = canny.size( );
      size_t adj_pxl;
      COMMENT( "Computing redundant pixels and removing them from canny.", 0 );
      for( size_t itr = 0; itr < 2; ++itr ) {
        for( size_t pxl = 0; pxl < img_size; ++pxl ) {
          if( canny[ pxl ] ) {
            char l1 = 0, l2 = 0, l3 = 0, l4 = 0, ngbs = 0;
            for( size_t pos = 1; pos < adj_size; ++pos ) {
              if( ( adj_itr.AdjIdx( pxl, pos, adj_pxl ) ) && ( canny[ adj_pxl ] ) ) {
                switch( pos ) {
                case 1: {
                  --l4; ++ngbs;
                  break;
                }
                case 2: {
                  ++l1; ++l2; --l3; --l4; ++ngbs;
                  break;
                }
                case 3: {
                  --l3; ++ngbs;
                  break;
                }
                case 4: {
                  ++l1; --l2; ++l3; --l4; ++ngbs;
                  break;
                }
                case 5: {
                  --l1; ++l2; --l3; ++l4; ++ngbs;
                  break;
                }
                case 6: {
                  --l2; ++ngbs;
                  break;
                }
                case 7: {
                  --l1; --l2; ++l3; ++l4; ++ngbs;
                  break;
                }
                case 8: {
                  --l1; ++ngbs;
                  break;
                }
                }
              }
            }
            if( ( ( l1 == 2 ) || ( l2 == 2 ) || ( l3 == 2 ) || ( l4 == 2 ) ) ||
                ( ( std::abs( l1 ) == 1 ) && ( std::abs( l2 ) == 1 ) &&
                  ( std::abs( l3 ) == 1 ) && ( std::abs( l4 ) == 1 ) && ( ngbs >= 3 ) ) ||
                ( ( std::abs( l1 ) == 0 ) && ( std::abs( l2 ) == 0 ) &&
                  ( std::abs( l3 ) == 0 ) && ( std::abs( l4 ) == 0 ) && ( ngbs == 4 ) ) ) {
              redundant[ pxl ] = 1;
              canny[ pxl ] = 0;
            }
          }
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
  void Gradient::RemoveBifurcation( Image< D > &canny ) {
    try {
      if( canny.Dims( ) != 2 ) {
        std::string msg( BIAL_ERROR( "Requires 2D Canny image. Given: " + std::to_string( canny.Dims( ) ) ) );
        throw( std::logic_error( msg ) );
      }
      Adjacency adj_rel( AdjacencyType::Gray8( ) );
      AdjacencyIterator adj_itr( canny, adj_rel );
      size_t adj_size = adj_rel.size( );
      size_t img_size = canny.size( );
      size_t adj_pxl;
      COMMENT( "Removing bifurcation points from 3 point bifurcation with dominant direction.", 1 );
      for( size_t elm = 0; elm < img_size; ++elm ) {
        if( canny[ elm ] != 0 ) {
          COMMENT( "Counting canny adjacent pixels.", 4 );
          Vector< int > adjs;
          for( size_t adj_dir = 0; adj_dir < 8; ++adj_dir ) {
            adj_pxl = adj_itr( elm, adj_dir );
            if( canny[ adj_pxl ] != 0 )
              adjs.push_back( adj_dir );
          }
          COMMENT( "Checking if pixel has three canny adjacents.", 4 );
          if( adj_size == 3 ) {
            COMMENT( "Verifying opposed directions.", 4 );
            int opp_dir = Bit::Flip( Bit::Flip( adjs[ 0 ], 0 ), 2 );
            if( opp_dir == adjs[ 1 ] ) {
              COMMENT( "0 opposed to 1. Removing 2.", 4 );
              adj_pxl = adj_itr( elm, adjs[ 2 ] );
              canny[ adj_pxl ] = 0;
              continue;
            }
            if( opp_dir == adjs[ 2 ] ) {
              COMMENT( "0 opposed to 2. Removing 1.", 4 );
              adj_pxl = adj_itr( elm, adjs[ 1 ] );
              canny[ adj_pxl ] = 0;
              continue;
            }
            opp_dir = Bit::Flip( Bit::Flip( adjs[ 1 ], 0 ), 2 );
            if( opp_dir == adjs[ 2 ] ) {
              COMMENT( "1 opposed to 2. Removing 0.", 4 );
              adj_pxl = adj_itr( elm, adjs[ 0 ] );
              canny[ adj_pxl ] = 0;
              continue;
            }
            COMMENT( "Diagonal opposed directions.", 4 );
            int side1_dir = Bit::Flip( adjs[ 0 ], Bit::IsZero( adjs[ 0 ], 1 ) ? 2 : 0 );
            int side2_dir = Bit::Flip( Bit::Flip( Bit::Flip( adjs[ 0 ], 0 ), 1 ), 2 );
            if( ( adjs[ 1 ] == side1_dir ) || ( adjs[ 1 ] == side2_dir ) ) {
              COMMENT( "0 side-opposed to 1. Removing 2.", 4 );
              adj_pxl = adj_itr( elm, adjs[ 2 ] );
              canny[ adj_pxl ] = 0;
              continue;
            }
            if( ( adjs[ 2 ] == side1_dir ) || ( adjs[ 2 ] == side2_dir ) ) {
              COMMENT( "0 side-opposed to 2. Removing 1.", 4 );
              adj_pxl = adj_itr( elm, adjs[ 1 ] );
              canny[ adj_pxl ] = 0;
              continue;
            }
            side1_dir = Bit::Flip( adjs[ 1 ], Bit::IsZero( adjs[ 1 ], 1 ) ? 2 : 0 );
            side2_dir = Bit::Flip( Bit::Flip( Bit::Flip( adjs[ 1 ], 0 ), 1 ), 2 );
            if( ( adjs[ 2 ] == side1_dir ) || ( adjs[ 2 ] == side2_dir ) ) {
              COMMENT( "1 side-opposed to 2. Removing 0.", 4 );
              adj_pxl = adj_itr( elm, adjs[ 0 ] );
              canny[ adj_pxl ] = 0;
            }
          }
        }
      }
      COMMENT( "Removing the other bifurcations.", 1 );
      for( size_t pxl = 0; pxl < img_size; ++pxl ) {
        if( canny[ pxl ] != 0 ) {
          unsigned int neighbors = 0;
          for( size_t pos = 0; pos < adj_size; ++pos ) {
            if( ( adj_itr.AdjIdx( pxl, pos, adj_pxl ) ) && ( canny[ pxl ] == canny[ adj_pxl ] ) )
              ++neighbors;
          }
          if( neighbors >= 3 )
            canny[ pxl ] = 0;
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

  Vector< size_t > Gradient::Extremities( const Image< int > &label ) {
    try {
      if( label.Dims( ) != 2 ) {
        std::string msg( BIAL_ERROR( "Requires 2D labeled Canny image. Given: " + std::to_string( label.Dims( ) ) ) );
        throw( std::logic_error( msg ) );
      }
      Vector< size_t > extremities( label.Maximum( ) + 1, label.size( ) );
      Adjacency adj( AdjacencyType::Circular( 1.47 ) );
      AdjacencyIterator adj_itr( label, adj );
      size_t adj_size = adj.size( );
      size_t img_size = label.size( );
      size_t adj_pxl;
      COMMENT( "Detecting extremities.", 1 );
      for( size_t pxl = 0; pxl < img_size; ++pxl ) {
        if( label[ pxl ] != 0 ) {
          unsigned int neighbors = 0;
          for( size_t pos = 1; pos < adj_size; ++pos ) {
            if( ( adj_itr.AdjIdx( pxl, pos, adj_pxl ) ) && ( label[ pxl ] == label[ adj_pxl ] ) )
              ++neighbors;
          }
          if( neighbors == 1 ) {
            if( extremities[ label[ pxl ] ] == img_size )
              extremities[ label[ pxl ] ] = pxl;
          }
        }
      }
      return( extremities );
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

  /* Parei aqui. Colocar parâmetros de validação. */
  template< class D >
  void Gradient::RemoveSmallSegments( Image< D > &canny, size_t min_seg_size ) {
    try {
      if( canny.Dims( ) != 2 ) {
        std::string msg( BIAL_ERROR( "Requires 2D Canny image. Given: " + std::to_string( canny.Dims( ) ) ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Preparing image borders, removing redundant points and bifurcations.", 0 );
      Gradient::RemoveRedundantPoint( canny );
      // DEBUG_WRITE( canny, "redundand_rm", 0 );
      Gradient::RemoveBifurcation( canny );
      // DEBUG_WRITE( canny, "bifurc_rm", 0 );
      COMMENT( "Identifying connected segments.", 0 );
      Adjacency adj = AdjacencyType::Circular( 1.45 );
      Image< int > label( Segmentation::ConnectedComponents( canny, adj ) );
      COMMENT( "Initial candidate segments: " << label.Maximum( ) << ". Reserving memory.", 0 );
      size_t label_max = static_cast< size_t >( label.Maximum( ) );
      COMMENT( "Vector holds the coordinates of each segment.", 0 );
      Vector< Sequence > segment;
      segment.reserve( label_max );
      COMMENT( "Finding extremities of segments.", 0 );
      Vector< size_t > extremities( Gradient::Extremities( label ) );
      for( size_t lb = 0; lb <= label_max; ++lb ) {
        if( extremities[ lb ] < label.size( ) ) {
          COMMENT( "SegmentOrder uses an adjacency to navigate trough the segment pixels" <<
                   " and stores the pixel coordinates sorted in the \"segments\" vector.", 3 );
          segment.push_back( Sequence( label, extremities[ lb ], adj ) );
        }
      }
      COMMENT( "Extremities found: " << segment.size( ) << ". Validating the segments.", 0 );
      Vector< Sequence > valid_segments;
      for( size_t sgm = 0; sgm < segment.size( ); ++sgm ) {
        if( segment[ sgm ].ValidateSize( min_seg_size ) ) {
          valid_segments.push_back( segment[ sgm ] );
        }
      }
      segment.swap( valid_segments );
      COMMENT( "Writing the " << segment.size( ) << " resultant segment(s) into the return image.", 0 );
      Image< D > res( canny );
      res.Set( 0 );
      for( size_t sgm = 0; sgm < segment.size( ); ++sgm ) {
        for( size_t elm = 0; elm < segment[ sgm ].point.size( ); ++elm ) {
          res( segment[ sgm ].point[ elm ][ 0 ], segment[ sgm ].point[ elm ][ 1 ] ) = 1;
        }
      }
      canny = res;
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

#ifdef BIAL_EXPLICIT_GradientPosOperations

  template Image< int > Gradient::CloseHoles( const Image< int > &canny );
  template void Gradient::RemoveRedundantPoint( Image< int > &canny );
  template void Gradient::RemoveBifurcation( Image< int > &canny );
  template void Gradient::RemoveSmallSegments( Image< int > &canny, size_t min_seg_size );

  template Image< llint > Gradient::CloseHoles( const Image< llint > &canny );
  template void Gradient::RemoveRedundantPoint( Image< llint > &canny );
  template void Gradient::RemoveBifurcation( Image< llint > &canny );
  template void Gradient::RemoveSmallSegments( Image< llint > &canny, size_t min_seg_size );

  template Image< float > Gradient::CloseHoles( const Image< float > &canny );
  template void Gradient::RemoveRedundantPoint( Image< float > &canny );
  template void Gradient::RemoveBifurcation( Image< float > &canny );
  template void Gradient::RemoveSmallSegments( Image< float > &canny, size_t min_seg_size );

  template Image< double > Gradient::CloseHoles( const Image< double > &canny );
  template void Gradient::RemoveRedundantPoint( Image< double > &canny );
  template void Gradient::RemoveBifurcation( Image< double > &canny );
  template void Gradient::RemoveSmallSegments( Image< double > &canny, size_t min_seg_size );

#endif

}

#endif

#endif
