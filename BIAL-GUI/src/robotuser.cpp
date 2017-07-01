#include "Adjacency.hpp"
#include "AdjacencyRound.hpp"
#include "FileImage.hpp"
#include "livewiretool.h"
#include "robotuser.h"

#include <AdjacencyIterator.hpp>
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QThread>

void find_next( const Bial::Image< int > &img, std::array< int, 8 > disp, int p_c, int d_pc, int &p_n, int &d_cn ) {
  int d_cp = ( d_pc + 4 ) % 8;
  // int width = img.size(0);
  // int height = img.size(1);
  for( int r_dir = 0; r_dir <= 7; ++r_dir ) {
    int d_e = ( d_cp + r_dir ) % 8;
    int d_i = ( d_cp + r_dir + 1 ) % 8;

    int p_e = p_c + disp[ d_e ];
    int p_i = p_c + disp[ d_i ];
    if( ( img[ p_e ] == 0 ) && ( img[ p_i ] > 0 ) ) {
      p_n = p_e;
      d_cn = d_e;
    }
  }
}

Path contour_following( const Bial::Image< int > &img ) {
  Bial::Image< int > contour_map( img.Dim( ) );
  Path contour;
  int label = 0;
  int width = img.size( 0 );
  int height = img.size( 1 );
  std::array< int, 8 > disp = { 1, 1 - width, -width, -width - 1, -1, width - 1, width, width + 1 };
  for( int y = 1; y < ( height - 1 ); ++y ) {
    int ywdt = y * width;
    for( int x = 1; x < ( width - 1 ); ++x ) {
      int start_pxl = ywdt + x;
      if( ( contour_map[ start_pxl ] == 0 ) && ( img[ start_pxl ] == 0 ) && ( img[ start_pxl + 1 ] > 0 ) ) {
        /*cout << start_pxl << ", " << label << endl;*/
        contour_map[ start_pxl ] = label;
        contour.push_back( start_pxl );
        int next_pxl;
        int dcn;
        for( dcn = 4; dcn < 7; ++dcn ) {
          next_pxl = start_pxl + disp[ dcn ];
          int other_pxl = start_pxl + disp[ ( dcn + 1 ) % 7 ];
          if( ( img[ next_pxl ] == 0 ) && ( img[ other_pxl ] > 0 ) ) {
            break;
          }
        }
        while( next_pxl != start_pxl ) {
          contour_map[ next_pxl ] = label;
          contour.push_back( next_pxl );
          find_next( img, disp, next_pxl, dcn, next_pxl, dcn );
        }
        ++label;
        if( contour.size( ) > 10 ) {
          return( contour );
        }
      }
    }
  }
  return( contour );
}


RobotUser::RobotUser( LiveWireTool &tool ) :
  m_tool( tool ) {

  QFileInfo finfo( m_tool.guiImage->fileName( ) );
  QDir dir = finfo.dir( );
  if( dir.cd( "segmentation" ) ) {
    QFileInfo gtFile( dir.absoluteFilePath( finfo.baseName( ) + ".pgm" ) );
    if( gtFile.exists( ) && gtFile.isFile( ) ) {
      m_groundTruth = Bial::Read< int >( gtFile.absoluteFilePath( ).toStdString( ) );
      if( m_groundTruth.size( ) != m_tool.m_grayImg.size( ) ) {
        throw std::runtime_error( BIAL_ERROR( "Images should have the same dimensions!" ) );
      }
      m_contour = contour_following( m_groundTruth );


    }
    else {
      throw( std::runtime_error( BIAL_ERROR( "Could not find the ground truth file!" ) ) );
    }
  }
  else {
    throw( std::runtime_error( BIAL_ERROR( "Could not find the ground truth file!" ) ) );
  }
}

void RobotUser::plotPath( const Path &path, QColor clr ) {
  clr.setAlpha( 100 );
  m_tool.m_res = m_tool.m_cache;
  for( size_t pxl : path ) {
    auto coords = m_groundTruth.Coordinates( pxl );
    m_tool.m_res.setPixelColor( coords[ 0 ], coords[ 1 ], clr );
  }
  emit m_tool.guiImage->imageUpdated( );
  qApp->processEvents( );
}

QPointF RobotUser::toPointF( int pxl ) {
  auto coord = m_groundTruth.Coordinates( pxl );
  return( QPointF( coord[ 0 ], coord[ 1 ] ) );
}

void draw( Bial::Image< int > &img, size_t pxl ) {
  Bial::Adjacency adj = Bial::AdjacencyType::Circular( 1.9 );
  Bial::AdjacencyIterator it( img, adj );
  size_t adj_px;
  for( size_t idx = 0; idx < adj.size( ); ++idx ) {
    if( it.AdjIdx( pxl, idx, adj_px ) ) {
      img[ adj_px ] = 1;
    }
  }
}

void RobotUser::run( ) {
//  size_t end = m_contour.size( ) - 1;
  qDebug( ) << "CONTOUR : " << m_contour.size( );
  Bial::Image< int > gt_map( m_groundTruth.Dim( ) );
  QPointF pt = toPointF( m_contour[ 0 ] );
  m_tool.addPoint( pt );
  m_tool.runLiveWire( );
  for( size_t pxl_idx = 0; pxl_idx < m_contour.size( ); ++pxl_idx ) {
    size_t best_length = 0;
    for( auto method : m_tool.m_methods ) {
      gt_map.Set( 0 );
      for( size_t pxl_idx2 = pxl_idx; pxl_idx2 < m_contour.size( ); ++pxl_idx2 ) {
        int pxl2 = m_contour[ pxl_idx2 ];
        draw( gt_map, pxl2 );

        Path path = method->updatePath( pxl2 );
        if( pxl_idx2 % 10 == 0 ) {
          plotPath( path, method->color );
        }
        int diff = 0;
        for( size_t pxl : path ) {
          if( gt_map[ pxl ] == 0 ) {
            diff++;
          }
        }
        if( diff > 0 ) {
          break;
        }
        else if( pxl_idx2 > best_length ) {
          best_length = pxl_idx2;
          m_tool.m_currentMethod = method->type( );
        }
      }
    }
    pxl_idx = best_length;
    if( pxl_idx < m_contour.size( ) - 1 ) {
      QPointF pt2 = toPointF( m_contour[ pxl_idx ] );
      m_tool.addPoint( pt2 );
      m_tool.runLiveWire( );
    }
  }
  m_tool.finishSegmentation( );
  emit m_tool.guiImage->imageUpdated( );
  qApp->processEvents( );
}
