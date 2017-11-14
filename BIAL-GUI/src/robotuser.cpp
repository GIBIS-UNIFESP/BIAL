#include "Adjacency.hpp"
#include "AdjacencyRound.hpp"
#include "FileImage.hpp"
#include "activeContourTool.h"
#include "robotuser.h"

#include "GradientMorphological.hpp"
#include "MorphologyErosion.hpp"
#include "linepathmethod.h"
#include <AdjacencyIterator.hpp>
#include <ConnPathFunction.hpp>
#include <FastIncreasingFifoBucketQueue.hpp>
#include <ImageIFT.hpp>
#include <LocalMaxPathFunction.hpp>
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QThread>
#include <SegmentationConnectedComponents.hpp>

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
//          auto coord = contour_map.Coordinates( next_pxl );
//          qDebug( ) << next_pxl << coord[ 0 ] << coord[ 1 ];
          contour_map[ next_pxl ] = 255;
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

//Path contour_following( const Bial::Image< int > &img ) {
//  Bial::Adjacency adj( Bial::AdjacencyType::Circular( 1.25 ) );
//  Bial::Image< int > grad = img - Bial::Morphology::Erode( img );
//  Bial::Image< int > pred( grad.Dim( ) );
//  Bial::LocalMaxPathFunction< Bial::Image, int > connection_function( grad, nullptr, &pred, false );
//  size_t size = grad.size( );
//  size_t st_pxl = 0;
//  Bial::FastIncreasingFifoBucketQueue queue( size, 0, 2 );
//  for( size_t pxl = 0; pxl < size; ++pxl ) {//    if( grad[ pxl ] > 0 ) {
//      queue.Insert( pxl, grad[ pxl ] );
//      st_pxl = pxl;
//    }
//  }
//  Bial::ImageIFT< int > ift( grad, adj, &connection_function, &queue );
////  ift.EnableStopElement( pxl );
//  ift.Run( );
//  int pxl = pred[ st_pxl ];
//  Path path = { st_pxl };
//  while( pxl != st_pxl && pred[ pxl ] > 0 ) {
//    path.push_back( pxl );
//    pxl = pred[ pxl ];
//  }
//  return( path );
//}

RobotUser::RobotUser( ActiveContourTool &tool ) :
  m_tool( tool ) {

  QFileInfo finfo( m_tool.getGuiImage( )->fileName( ) );
  QDir dir = finfo.dir( );
  if( dir.cd( "segmentation" ) ) {
    QFileInfo gtFile( dir.absoluteFilePath( finfo.baseName( ) + ".pgm" ) );
    if( gtFile.exists( ) && gtFile.isFile( ) ) {
      m_groundTruth = Bial::Read< int >( gtFile.absoluteFilePath( ).toStdString( ) );
      if( m_groundTruth.size( ) != m_tool.getGuiImage( )->getSize( ) ) {
        BIAL_WARNING( "Images should have the same dimensions!" );
      }
      m_contour = contour_following( m_groundTruth );
    }
    else {
      BIAL_WARNING( "Could not find the ground truth file!" );
    }
  }
  else {
    BIAL_WARNING( "Could not find the ground truth file!" );
  }
}

void RobotUser::plotPath( const Path &path, QColor clr ) {
  clr.setAlpha( 100 );
  QImage res = m_tool.getCache( );
  for( size_t pxl : path ) {
    auto coords = m_groundTruth.Coordinates( pxl );

    res.setPixelColor( coords[ 0 ], coords[ 1 ], clr );
  }
  m_tool.setRes( res );
  emit m_tool.getGuiImage( )->imageUpdated( );
  qApp->processEvents( );
}

QPointF RobotUser::toPointF( int pxl ) {
  auto coord = m_groundTruth.Coordinates( pxl );
  return( QPointF( coord[ 0 ], coord[ 1 ] ) );
}

void draw( Bial::Image< int > &img, size_t pxl ) {
  Bial::Adjacency adj = Bial::AdjacencyType::Circular( 1.5 );
  Bial::AdjacencyIterator it( img, adj );
  size_t adj_px;
  for( size_t idx = 0; idx < adj.size( ); ++idx ) {
    if( it.AdjIdx( pxl, idx, adj_px ) ) {
      img[ adj_px ] = 1;
    }
  }
}

void RobotUser::report( const std::string &methodName, size_t total_errors ) {
  std::cout << "The robot has finished the segmentation!" << std::endl;
  std::cout << "Image: " << m_tool.getGuiImage( )->fileName( ).toStdString( ) << std::endl;
  std::cout << "Method: " << methodName << std::endl;
  std::cout << "Total seeds: " << m_tool.getSelectedMethods( ).size( ) << std::endl;
  std::cout << "Total changes: " << total_errors << std::endl;
  std::cout << "Contour size: " << m_tool.getCurrentPath( ).size( ) << std::endl;
  std::cout << "Order: " << m_tool.getSelectedMethods( ) << std::endl;
}

void RobotUser::runSingle( std::shared_ptr< ActiveContourMethod > method ) {
  if( m_contour.empty( ) ) {
    BIAL_WARNING( "Empty contour!" );
    return;
  }
  Bial::Image< int > gt_map( m_groundTruth.Dim( ) );
  QPointF pt = toPointF( m_contour[ 0 ] );
  m_tool.clear( );
  m_tool.setCurrentMethod( method->type( ) );
  m_tool.addPoint( pt );
  m_tool.runLiveWire( );
  size_t best_pxl = 0;
  size_t errors = 0;
  size_t total_errors = 0;
  size_t last_anchor = 0;
  for( size_t pxl_idx = 0; pxl_idx < m_contour.size( ); ++pxl_idx ) {
    m_tool.setCurrentMethod( method->type( ) );
    draw( gt_map, m_contour[ pxl_idx ] );
    int pxl = m_contour[ pxl_idx ];
    Path path = method->updatePath( pxl );
//    if( pxl_idx ) {
//    }
    int diff = 0;
    for( size_t pxl : path ) {
      if( gt_map[ pxl ] == 0 ) {
        diff++;
      }
    }
//    plotPath( path, method->color );
    if( diff > 0 ) {
      errors++;
      if( errors >= 10 ) {
        best_pxl = pxl_idx;
        m_tool.setCurrentMethod( LinePathMethod::Type );
        total_errors++;
      }
      if( ( best_pxl != last_anchor ) && ( pxl_idx < m_contour.size( ) - 1 ) ) {
        last_anchor = best_pxl;
        errors = 0;
        QPointF pt2 = toPointF( m_contour[ best_pxl ] );
        m_tool.addPoint( pt2 );
        m_tool.runLiveWire( );
//        Bial::Write( gt_map, "/tmp/" + method->name( ) + ".pgm" );
        qApp->processEvents( );
      }
    }
    else if( pxl_idx > best_pxl ) {
      best_pxl = pxl_idx;
    }
    emit m_tool.getGuiImage( )->imageUpdated( );
    qApp->processEvents( );
  }
  m_tool.finishSegmentation( );
  report( method->name( ), total_errors );
  QFileInfo finfo( m_tool.getGuiImage( )->fileName( ) );
  QString fname = "/tmp/" + finfo.baseName( ) + "_" +
                  QString::fromStdString( method->name( ) ) + ".pgm";

  qDebug( ) << fname;
  gt_map.Set( 0 );
  Path currentPath = m_tool.getCurrentPath( );
  for( size_t pxl : currentPath ) {
    gt_map[ pxl ] = 255;
  }
  Bial::Write( gt_map, fname.toStdString( ) );
//
  emit m_tool.getGuiImage( )->imageUpdated( );
  qApp->processEvents( );
}

void RobotUser::run( ) {
  m_tool.clear( );
  size_t total_changes = 0;
  size_t nextMethod = m_tool.getCurrentMethod( );
  const size_t STEP = 1;
//  size_t end = m_contour.size( ) - 1;
  Bial::Image< int > gt_map( m_groundTruth.Dim( ) );
  QPointF pt = toPointF( m_contour[ 0 ] );
  m_tool.addPoint( pt );
  m_tool.runLiveWire( );
  for( size_t pxl_idx = 0; pxl_idx < m_contour.size( ); pxl_idx += STEP ) {
    size_t best_length = 0;
    for( auto method : m_tool.getMethods( ) ) {
      gt_map.Set( 0 );
      for( size_t pxl_idx2 = pxl_idx; pxl_idx2 < m_contour.size( ); ++pxl_idx2 ) {
        int pxl2 = m_contour[ pxl_idx2 ];
        draw( gt_map, pxl2 );

        Path path = method->updatePath( pxl2 );
//        plotPath( path, method->color );
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
          nextMethod = method->type( );
        }
      }
      emit m_tool.getGuiImage( )->imageUpdated( );
      qApp->processEvents( );
    }
    pxl_idx = best_length;
    if( pxl_idx < m_contour.size( ) - STEP ) {
      if( m_tool.getCurrentMethod( ) != static_cast< int >( nextMethod ) ) {
        total_changes++;
        m_tool.setCurrentMethod( nextMethod );
      }
      QPointF pt2 = toPointF( m_contour[ pxl_idx ] );
      m_tool.addPoint( pt2 );
      m_tool.runLiveWire( );
      qApp->processEvents( );
    }
  }
  m_tool.finishSegmentation( );

  report( "Mixed", total_changes );
  QFileInfo finfo( m_tool.getGuiImage( )->fileName( ) );
  QString fname = "/tmp/" + finfo.baseName( ) + ".pgm";
  Bial::Write( gt_map, fname.toStdString( ) );

  fname = "/tmp/" + finfo.baseName( ) + "_Mixed.pgm";
  qDebug( ) << fname;
  gt_map.Set( 0 );
  Path currentPath = m_tool.getCurrentPath( );
  for( size_t pxl : currentPath ) {
    gt_map[ pxl ] = 255;
  }
  Bial::Write( gt_map, fname.toStdString( ) );
  emit m_tool.getGuiImage( )->imageUpdated( );
  qApp->processEvents( );
}

void RobotUser::train( ) {
//  size_t end = m_contour.size( ) - 1;
  qDebug( ) << "CONTOUR : " << m_contour.size( );
  QPointF pt = toPointF( m_contour[ 0 ] );
  m_tool.addPoint( pt );
  m_tool.runLiveWire( );
  Bial::Image< int > gt_map( m_groundTruth.Dim( ) );
  for( size_t pxl_idx = 0; pxl_idx < m_contour.size( ); ++pxl_idx ) {
    size_t best_length = 0;

//#pragma omp parallel for default ( none ) firstprivate( gt_map, m_contour, pxl_idx ) shared( m_tool, best_length )
    for( int m = 0; m < m_tool.getMethods( ).size( ); ++m ) {
      auto method = m_tool.getMethods( )[ m ];
      gt_map.Set( 0 );
      for( size_t pxl_idx2 = pxl_idx; pxl_idx2 < m_contour.size( ); ++pxl_idx2 ) {
        int pxl2 = m_contour[ pxl_idx2 ];
        draw( gt_map, pxl2 );

        Path path = method->updatePath( pxl2 );
//        if( pxl_idx2 % 10 == 0 ) {
//          plotPath( path, method->color );
//        }
        int diff = 0;
        for( size_t pxl : path ) {
          if( gt_map[ pxl ] == 0 ) {
            diff++;
          }
        }
        if( diff > 0 ) {
          break;
        }
//#pragma omp critical
        if( pxl_idx2 > best_length ) {
          best_length = pxl_idx2;
          m_tool.setCurrentMethod( method->type( ) );
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
  emit m_tool.getGuiImage( )->imageUpdated( );
  qApp->processEvents( );
}
