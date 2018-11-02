/*
- Tirar diagonais e tratar pelo rótulo.
*/

#include "Adjacency.hpp"
#include "AdjacencyIterator.hpp"
#include "AdjacencyRound.hpp"
#include "ColorRGB.hpp"
#include "DrawLine.hpp"
#include "File.hpp"
#include "FileImage.hpp"
#include "GeoSumPathFunction.hpp"
#include "Geometrics.hpp"
#include "GradientMorphological.hpp"
#include "GradientSobel.hpp"
#include "IntensityGlobals.hpp"
#include "MaxPathFunction.hpp"
#include "MorphologyErosion.hpp"
#include "MultiImage.hpp"
#include "OrientedExternPathFunction.hpp"
#include "OrientedInternPathFunction.hpp"
#include "RealColor.hpp"
#include "RotatingBucketQueue.hpp"
#include "SegmentationFSum.hpp"
#include "SegmentationGeoStar.hpp"
#include "SegmentationWatershed.hpp"
#include "SimpleBucketQueue.hpp"
#include "SumPathFunction.hpp"
#include "guiimage.h"
#include "segmentationtool.h"
#include "segmentationtool.h"
#include "segmentationtool.h"
#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <QPointF>
#include <algorithm>
#include <tuple>
#include <QGraphicsEllipseItem>

double SegmentationTool::getAlpha( ) const {
  return( alpha );
}

void SegmentationTool::setAlpha( double value ) {
  alpha = value;
}

double SegmentationTool::getBeta( ) const {
  return( beta );
}

void SegmentationTool::setAnchors( int value ) {
  anchors = value;
}

void SegmentationTool::setBeta( double value ) {
  beta = value;
}

int SegmentationTool::getDrawType( ) const {
  return( drawType );
}

void SegmentationTool::setSeedsVisibility( bool vis ) {
  seedsVisible = vis;
  emit guiImage->imageUpdated( );
}

void SegmentationTool::setMaskVisibility( bool vis ) {
  maskVisible = vis;
  emit guiImage->imageUpdated( );
}

bool SegmentationTool::getSeedsVisible( ) const {
  return( seedsVisible );
}

bool SegmentationTool::getMaskVisible( ) const {
  return( maskVisible );
}

void SegmentationTool::setThickness( int value ) {
  thickness = value;
  COMMENT( "thickness set to" << thickness, 2 );
}

bool SegmentationTool::isInitiated( ) const {
  return( initiated );
}

Bial::Image< int > SegmentationTool::getMask( ) const {
  return( label[ 0 ] );
}

Bial::Image< int > SegmentationTool::getSeeds( ) const {
  return( seeds );
}

void SegmentationTool::setExtremity( ) {
  extremity_pxl.clear( );
}

void SegmentationTool::setLabelAdjacencies( ) {
  lbl_adj_list.clear( );
}

SegmentationTool::SegmentationTool( GuiImage *guiImage, ImageViewer *viewer ) try :
  Tool( guiImage, viewer ), seeds( guiImage->getDim( ) ), adj( ), queue( nullptr ), m_scene( viewer->getScene( 0 ) ) {
  COMMENT( "Initiating segmentation tool.", 0 );
  setObjectName( "SegmentationTool" );
  setHasLabel( true );
  setExtremity( );
  setLabelAdjacencies( );
  drawType = 1;
  drawing = false;
  alpha = 0;
  beta = 0.5;
  thickness = 0;
  seedsVisible = true;
  maskVisible = true;
  initiated = false;
  pf = 0; // geosum path-function
  int_ift[ 0 ] = nullptr;
  int_ift[ 1 ] = nullptr;
  flt_ift[ 0 ] = nullptr;
  flt_ift[ 1 ] = nullptr;
  int_path_func[ 0 ] = nullptr;
  int_path_func[ 1 ] = nullptr;
  flt_path_func[ 0 ] = nullptr;
  flt_path_func[ 1 ] = nullptr;
  grad_type = -1;
  moved_anchor = nullptr;
  moved_anchor_index = -1;
  COMMENT( "Finished constructor for segmentation tool.", 0 );
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

SegmentationTool::~SegmentationTool( ) {
  try {
    delete int_path_func[ 0 ];
    delete int_path_func[ 1 ];
    delete flt_path_func[ 0 ];
    delete flt_path_func[ 1 ];
    delete queue;
    delete int_ift[ 0 ];
    delete int_ift[ 1 ];
    delete flt_ift[ 0 ];
    delete flt_ift[ 1 ];
    int_path_func[ 0 ] = nullptr;
    int_path_func[ 1 ] = nullptr;
    flt_path_func[ 0 ] = nullptr;
    flt_path_func[ 1 ] = nullptr;
    queue = nullptr;
    int_ift[ 0 ] = nullptr;
    int_ift[ 1 ] = nullptr;
    flt_ift[ 0 ] = nullptr;
    flt_ift[ 1 ] = nullptr;
    for( auto pt : anchor_ellipse ) {
      m_scene->removeItem( ( QGraphicsItem* ) pt );
    }
    //qDeleteAll( anchor_ellipse );
    anchor_ellipse.clear( );
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

int SegmentationTool::type( ) {
  return( SegmentationTool::Type );
}

void SegmentationTool::mouseClicked( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {
  try {
    timer.start( );
    if( drawType == 3 ) {
      COMMENT( "Moving anchor.", 3 );
      int anchor_number = -1;
      moved_anchor = m_scene->itemAt( pt, QTransform( ) );
      COMMENT( "Checking if the clicked pixel refers to an anchor.", 3 );
      for( int pos = 0; pos < anchor_ellipse.size( ); ++pos ) {
        if( anchor_ellipse[ pos ] ==  moved_anchor){
          anchor_number = pos;
          COMMENT("anchor selected = " << pos , 0 );
        }
      }
      if( ( moved_anchor != nullptr ) && ( anchor_number >= 0) ) {
        COMMENT( "Clicked pixel does refer to an anchor.", 0 ) ;
        moved_anchor_index = anchor_number;
        size_t img_size = seed_img.size( );
        int previous_anchor_index = ( moved_anchor_index == 0 ) ? anchors : moved_anchor_index;
        int true_moved_anchor_index  = ( moved_anchor_index == 0) ? 1 : moved_anchor_index + 1;
        COMMENT( "Erasing current paths to run Livewire.", 0 );
        for( size_t pxl = 0; pxl < img_size; ++pxl ) {
          int contour_label = border[ pxl ];
          if( ( contour_label == previous_anchor_index ) || ( contour_label == true_moved_anchor_index ) ) {
            label[ 2 ][ pxl ] = 0;
            mask[ pxl ] = 0;    
            border[ pxl ] = 0; // Ensurance
          }
        }
        COMMENT( "Paths with labels " << previous_anchor_index << " and " << true_moved_anchor_index << " were updated.", 0 );
        emit guiImage->imageUpdated( );
      }
      else {
        COMMENT( "Clicked pixel does not refer to an anchor.", 0)
        moved_anchor = nullptr;
      }
    }
    else {
      COMMENT( "Drawing seeds.", 0 );
      drawing = true;
      seedsVisible = true;
      if( ( drawType == 1 ) || ( drawType == 2 ) ) {
        switch( buttons ) {
            case Qt::LeftButton:
            drawType = 1;
            break;
            case Qt::RightButton:
            drawType = 2;
            break;
        }
      }
      const Bial::FastTransform &transf = guiImage->getTransform( axis );
      lastPoint = transf( pt.x( ), pt.y( ), ( double ) guiImage->currentSlice( axis ) );
    }
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

void SegmentationTool::mouseMoved( QPointF pt, size_t axis ) {
  try {
    if( drawing ) {
      const Bial::FastTransform &transf = guiImage->getTransform( axis );
      Bial::Point3D current = transf( pt.x( ), pt.y( ), ( double ) guiImage->currentSlice( axis ) );
      if( lastPoint == current ) {
        return;
      }
      drawSeed( lastPoint, current );
      lastPoint = current;
    }
    if( timer.elapsed( ) > 30 ) {
      emit guiImage->imageUpdated( );
      timer.start( );
      qApp->processEvents( );
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

void SegmentationTool::mouseReleased( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {
  try {
    if( moved_anchor != nullptr ) {
      size_t x_size = seed_img.size( 0 );
      COMMENT( "Computing new anchor postion.", 0 );
      size_t pos = static_cast< size_t >( pt.x( ) ) + static_cast< size_t >( pt.y( ) ) * x_size;
      COMMENT( "Updating anchor position from " << anchor_position[ moved_anchor_index ] << " to " << pos, 0 );
      anchor_position[ moved_anchor_index ] = pos;
      Bial::Image< int > my_grad( grad.IntImage( ) );
      Bial::Intensity::Complement( my_grad );
      COMMENT("Running two LiveWire algorithms, the first one between the moved anchor and it's previous and the second one between the moved anchor and it's next.", 3 );
      Bial::Write( border, "/tmp/border_on_click.pgm" );
      if( moved_anchor_index == 0 ) {
        anchor_position[ anchors ] = pos - 1;
        COMMENT("First LiveWire, with ini_pxl = " << anchor_position[ anchors - 1 ] << " , and end_pxl = " << anchor_position[ 0 ] << ".", 0);
        SegmentationTool::LiveWire( my_grad, my_seed, seed_img, border, anchors, anchor_position[ anchors - 1 ], anchor_position[ 0 ], -1 );
      }
      else{ 
        COMMENT("First LiveWire, with ini_pxl = " << anchor_position[ moved_anchor_index - 1] << " , and end_pxl = " << anchor_position[ moved_anchor_index ] << ".", 3 );
        SegmentationTool::LiveWire( my_grad, my_seed, seed_img, border, moved_anchor_index, anchor_position[ moved_anchor_index - 1 ], anchor_position[ moved_anchor_index ], -1 );
      }
      int true_moved_anchor_index  = ( moved_anchor_index == 0) ? 1 : moved_anchor_index + 1;
      COMMENT("Second LiveWire, with ini_pxl = " << anchor_position[ moved_anchor_index ] << " , and end_pxl = " << anchor_position[ true_moved_anchor_index ] << ".", 3 );
      SegmentationTool::LiveWire( my_grad, my_seed, seed_img, border, true_moved_anchor_index, anchor_position[ moved_anchor_index ], anchor_position[ true_moved_anchor_index ], -1 );
      moved_anchor = nullptr;
      emit guiImage->imageUpdated( );
    }
    else if( drawing ) {
      const Bial::FastTransform &transf = guiImage->getTransform( axis );
      Bial::Point3D current = transf( pt.x( ), pt.y( ), ( double ) guiImage->currentSlice( axis ) );
      drawSeed( lastPoint, current );
      drawing = false;
      emit guiImage->imageUpdated( );
    }
    Q_UNUSED( buttons );
    Q_UNUSED( axis );
    Q_UNUSED( pt );
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

void SegmentationTool::mouseDragged( QPointF pt, Qt::MouseButtons buttons, size_t axis ) {
  Q_UNUSED( buttons );
  Q_UNUSED( axis );
  Q_UNUSED( pt );
  /* nothing happens */
}

void SegmentationTool::sliceChanged( size_t axis, size_t slice ) {
  try {
    Q_UNUSED( slice );
    needUpdate[ axis ] = true;
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

void SegmentationTool::drawSeed( Bial::Point3D last, Bial::Point3D current ) {
  try {
    Bial::Vector< float > vLast;
    Bial::Vector< float > vCurrent;
    size_t dims = guiImage->getDims( );
    if( dims == 3 ) {
      vLast = { { ( float ) last[ 0 ], ( float ) last[ 1 ], ( float ) last[ 2 ] } };
      vCurrent = { { ( float ) current[ 0 ], ( float ) current[ 1 ], ( float ) current[ 2 ] } };
    }
    else {
      vLast = { { ( float ) last[ 0 ], ( float ) last[ 1 ] } };
      vCurrent = { { ( float ) current[ 0 ], ( float ) current[ 1 ] } };
    }
    Bial::Line imgLine( vLast, vCurrent, Bial::Color( 255, 255, 255, 255 ) );
    imgLine.Draw( seeds );
    Bial::Adjacency adj( Bial::AdjacencyType::HyperSpheric( thickness, dims ) );
    Bial::AdjacencyIterator it( seeds, adj );
    size_t adj_size = adj.size( );
    size_t size = seeds.size( );
    size_t adj_px;
    for( size_t px = 0; px < size; ++px ) {
      if( seeds[ px ] == 255 ) {
        for( size_t idx = 0; idx < adj_size; ++idx ) {
          if( it.AdjIdx( px, idx, adj_px ) )
            seeds[ adj_px ] = drawType;
        }
      }
    }
    for( size_t i = 0; i < needUpdate.size( ); ++i ) {
      needUpdate[ i ] = true;
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

void SegmentationTool::setDrawType( int type ) {
  drawType = type;
}

void SegmentationTool::clearSeeds( ) {
  try {
    for( size_t i = 0; i < seeds.size( ); ++i ) {
      if( seeds[ i ] == 1 || seeds[ i ] == 2 )
        seeds[ i ] = 0;
    }
    for( size_t i = 0; i < needUpdate.size( ); ++i ) {
      needUpdate[ i ] = true;
    }
    DeleteEllipses( );
    
    maskVisible = false;
    emit guiImage->imageUpdated( );
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

void SegmentationTool::clearMask( ) {
  try {
    for( size_t i = 0; i < mask.size( ); ++i ) {
      mask[ i ] = 0;
    }
    emit guiImage->imageUpdated( );
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

void SegmentationTool::setEllipsesMovable( bool movable ) {
  if( anchor_ellipse.size() > 0 ){ 
    for( size_t idx = 0; idx < anchors; ++idx ) {
      auto ellipse = anchor_ellipse[ idx ];
      ellipse->setFlag( QGraphicsItem::ItemIsSelectable, movable );
      ellipse->setFlag( QGraphicsItem::ItemIsMovable, movable );
    }
  }
}

bool SegmentationTool::hasMask( ) {
  try {
    bool hasMask = false;
    for( auto pxl: mask ) {
      if( pxl > 0 ) {
        hasMask = true;
      }
    }
    return( hasMask );
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
void SegmentationTool::InitiateSeeds( size_t map_set,
                                      const Bial::Vector< size_t > &obj_seeds,
                                      const Bial::Vector< size_t > &bkg_seeds,
                                      Bial::Image< D > &grad ) {
  try {
    COMMENT( "Initialize seeds on map set:" << map_set, 0 );
    COMMENT( "Initialize object seeds. Seeds: " << obj_seeds.size( ), 0 );
    for( size_t elm = 0; elm < obj_seeds.size( ); ++elm ) {
      size_t obj_elm = obj_seeds[ elm ];
      label[ map_set ][ obj_elm ] = 1;
      grad[ obj_elm ] = 0;
      queue->Insert( obj_elm, grad[ obj_elm ] );
    }
    COMMENT( "Initialize background seeds. Seeds: " << bkg_seeds.size( ), 0 );
    for( size_t elm = 0; elm < bkg_seeds.size( ); ++elm ) {
      size_t bkg_elm = bkg_seeds[ elm ];
      label[ map_set ][ bkg_elm ] = 0;
      grad[ bkg_elm ] = 0;
      queue->Insert( bkg_elm, grad[ bkg_elm ] );
    }
    COMMENT( "Fininished initializing seeds.", 0 );
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

void SegmentationTool::GeodesicSum( Bial::Image< int > &img,
                                    const Bial::Vector< size_t > &obj_seeds,
                                    const Bial::Vector< size_t > &bkg_seeds,
                                    float alpha,
                                    float beta ) {
  try {
    qDebug( ) << "GEOSUM: pf: " << pf;
    if( ( initiated ) && ( ( pf != 0 ) || ( curr_alpha != alpha ) || ( curr_beta != beta ) ) ) {
      COMMENT( "Was running with other pathfunction. Restart required.", 0 );
      delete int_path_func[ 0 ];
      delete int_path_func[ 1 ];
      delete int_ift[ 0 ];
      delete int_ift[ 1 ];
      int_path_func[ 0 ] = nullptr;
      int_path_func[ 1 ] = nullptr;
      int_ift[ 0 ] = nullptr;
      int_ift[ 1 ] = nullptr;
      initiated = false;
    }
    COMMENT( "Getting initial parameters. pf:" << pf, 0 );
    pf = 0;
    size_t size = img.size( );
    if( !initiated ) {
      Bial::Image< int > &lgrad( grad.IntImage( ) );
      COMMENT( "Initialize maps.", 0 );
      pred[ 1 ] = Bial::Image< int >( img.Dim( ) );
      label[ 1 ] = Bial::Image< int >( img.Dim( ) );
      adj = Bial::AdjacencyType::HyperSpheric( 1.5, pred[ 1 ].Dims( ) );
      COMMENT( "Initialize first path function.", 0 );
      cost[ 0 ] = grad;
      cost[ 1 ] = grad;
      Bial::Image< int > &value( cost[ 1 ].IntImage( ) );
      int_path_func[ 1 ] =
        new Bial::GeodesicRestrictionPathFunction< int >( value, nullptr, &pred[ 1 ], false, lgrad, img,
                                                          adj, alpha, beta );
      COMMENT( "Initialize queue.", 0 );
      for( size_t elm = 0; elm < size; ++elm ) {
        value[ elm ] = std::numeric_limits< int >::max( );
      }
      queue = new Bial::GrowingBucketQueue( size, 1.0, true, true );
      COMMENT( "Initialize first IFT.", 0 );
      int_ift[ 1 ] = new Bial::ImageIFT< int >( value, adj, int_path_func[ 1 ], queue );
      Bial::Vector< size_t > no_seeds;
      InitiateSeeds( 1, obj_seeds, no_seeds, value );
      COMMENT( "Running first IFT.", 0 );
      int_ift[ 1 ]->Run( );

      COMMENT( "SECOND STEP. Running with background and foreground seeds.", 0 );
      COMMENT( "Setting seeds with background.", 0 );
      pred[ 0 ] = Bial::Image< int >( img.Dim( ) );
      label[ 0 ] = Bial::Image< int >( img.Dim( ) );
      Bial::Image< int > &value2( cost[ 0 ].IntImage( ) );
      for( size_t elm = 0; elm < size; ++elm ) {
        value2[ elm ] = std::numeric_limits< int >::max( );
        queue->State( elm, Bial::BucketState::NOT_VISITED );
      }
      InitiateSeeds( 0, obj_seeds, bkg_seeds, value2 );
      COMMENT( "Running oriented path function.", 0 );
      if( alpha >= 0.0 ) {
        COMMENT( "OrientedExternPathFunction.", 0 );
        int_path_func[ 0 ] = new Bial::OrientedExternPathFunction< int >( value2, label[ 0 ], &pred[ 0 ], false,
                                                                          lgrad, img, &pred[ 1 ], alpha );
      }
      else {
        COMMENT( "OrientedInternPathFunction.", 0 );
        int_path_func[ 0 ] = new Bial::OrientedInternPathFunction< int >( value2, label[ 0 ], &pred[ 0 ], false,
                                                                          lgrad, img, &pred[ 1 ], -alpha );
      }
      int_ift[ 0 ] = new Bial::ImageIFT< int >( value2, adj, int_path_func[ 0 ], queue );
      int_ift[ 0 ]->Run( );
      initiated = true;
      curr_alpha = alpha;
      curr_beta = beta;
    }
    else {
      COMMENT( "Continuing segmentation first.", 0 );
      int_path_func[ 1 ]->DifferentialPropagation( true );
      for( size_t elm = 0; elm < size; ++elm ) {
        queue->State( elm, Bial::BucketState::NOT_VISITED );
      }
      Bial::Vector< size_t > no_seeds;
      InitiateSeeds( 1, obj_seeds, no_seeds, cost[ 1 ].IntImage( ) );
      COMMENT( "Running first IFT.", 0 );
      int_ift[ 1 ]->Run( );

      COMMENT( "Continuing segmentation second .", 0 );
      int_path_func[ 0 ]->DifferentialPropagation( true );
      Bial::Image< int > &value( cost[ 0 ].IntImage( ) );
      for( size_t elm = 0; elm < size; ++elm ) {
        queue->State( elm, Bial::BucketState::NOT_VISITED );
        value[ elm ] = std::numeric_limits< int >::max( );
        label[ 0 ][ elm ] = 0;
      }
      InitiateSeeds( 0, obj_seeds, bkg_seeds, value );
      COMMENT( "Running second IFT.", 0 );
      int_ift[ 0 ]->Run( );
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

void SegmentationTool::GeodesicSum( Bial::Image< float > &img,
                                    const Bial::Vector< size_t > &obj_seeds,
                                    const Bial::Vector< size_t > &bkg_seeds,
                                    float alpha,
                                    float beta ) {
  try {
    qDebug( ) << "GEOSUM: pf: " << pf;
    if( ( initiated ) && ( ( pf != 0 ) || ( curr_alpha != alpha ) || ( curr_beta != beta ) ) ) {
      COMMENT( "Was running with other pathfunction. Restart required.", 0 );
      delete flt_path_func[ 0 ];
      delete flt_path_func[ 1 ];
      delete flt_ift[ 0 ];
      delete flt_ift[ 1 ];
      flt_path_func[ 0 ] = nullptr;
      flt_path_func[ 1 ] = nullptr;
      flt_ift[ 0 ] = nullptr;
      flt_ift[ 1 ] = nullptr;
      initiated = false;
    }
    COMMENT( "Getting initial parameters. pf:" << pf, 0 );
    pf = 0;
    size_t size = img.size( );
    if( !initiated ) {
      Bial::Image< float > &lgrad( grad.FltImage( ) );
      COMMENT( "Initialize maps.", 0 );
      pred[ 1 ] = Bial::Image< int >( img.Dim( ) );
      label[ 1 ] = Bial::Image< int >( img.Dim( ) );
      adj = Bial::AdjacencyType::HyperSpheric( 1.5, pred[ 1 ].Dims( ) );
      COMMENT( "Initialize first path function.", 0 );
      cost[ 0 ] = grad;
      cost[ 1 ] = grad;
      Bial::Image< float > &value( cost[ 1 ].FltImage( ) );
      flt_path_func[ 1 ] =
        new Bial::GeodesicRestrictionPathFunction< float >( value, nullptr, &pred[ 1 ], false, lgrad, img,
                                                            adj, alpha, beta );
      COMMENT( "Initialize queue.", 0 );
      for( size_t elm = 0; elm < size; ++elm ) {
        value[ elm ] = std::numeric_limits< float >::max( );
      }
      queue = new Bial::GrowingBucketQueue( size, 1.0, true, true );
      COMMENT( "Initialize first IFT.", 0 );
      flt_ift[ 1 ] = new Bial::ImageIFT< float >( value, adj, flt_path_func[ 1 ], queue );
      Bial::Vector< size_t > no_seeds;
      InitiateSeeds( 1, obj_seeds, no_seeds, value );
      COMMENT( "Running first IFT.", 0 );
      flt_ift[ 1 ]->Run( );

      COMMENT( "SECOND STEP. Running with background and foreground seeds.", 0 );
      COMMENT( "Setting seeds with background.", 0 );
      pred[ 0 ] = Bial::Image< int >( img.Dim( ) );
      label[ 0 ] = Bial::Image< int >( img.Dim( ) );
      Bial::Image< float > &value2( cost[ 0 ].FltImage( ) );
      for( size_t elm = 0; elm < size; ++elm ) {
        value2[ elm ] = std::numeric_limits< float >::max( );
        queue->State( elm, Bial::BucketState::NOT_VISITED );
      }
      InitiateSeeds( 0, obj_seeds, bkg_seeds, value2 );
      COMMENT( "Running oriented path function.", 0 );
      if( alpha >= 0.0 ) {
        COMMENT( "OrientedExternPathFunction.", 0 );
        flt_path_func[ 0 ] =
          new Bial::OrientedExternPathFunction< float >( value2, label[ 0 ], &pred[ 0 ], false, lgrad, img,
                                                         &pred[ 1 ], alpha );
      }
      else {
        COMMENT( "OrientedInternPathFunction.", 0 );
        flt_path_func[ 0 ] =
          new Bial::OrientedInternPathFunction< float >( value2, label[ 0 ], &pred[ 0 ], false, lgrad, img,
                                                         &pred[ 1 ], -alpha );
      }
      flt_ift[ 0 ] = new Bial::ImageIFT< float >( value2, adj, flt_path_func[ 0 ], queue );
      flt_ift[ 0 ]->Run( );
      initiated = true;
      curr_alpha = alpha;
      curr_beta = beta;
    }
    else {
      COMMENT( "Continuing segmentation first.", 0 );
      flt_path_func[ 1 ]->DifferentialPropagation( true );
      for( size_t elm = 0; elm < size; ++elm ) {
        queue->State( elm, Bial::BucketState::NOT_VISITED );
      }
      Bial::Vector< size_t > no_seeds;
      InitiateSeeds( 1, obj_seeds, no_seeds, cost[ 1 ].FltImage( ) );
      COMMENT( "Running first IFT.", 0 );
      flt_ift[ 1 ]->Run( );

      COMMENT( "Continuing segmentation second .", 0 );
      flt_path_func[ 0 ]->DifferentialPropagation( true );
      Bial::Image< float > &value( cost[ 0 ].FltImage( ) );
      for( size_t elm = 0; elm < size; ++elm ) {
        queue->State( elm, Bial::BucketState::NOT_VISITED );
        value[ elm ] = std::numeric_limits< float >::max( );
        label[ 0 ][ elm ] = 0;
      }
      InitiateSeeds( 0, obj_seeds, bkg_seeds, value );
      COMMENT( "Running second IFT.", 0 );
      flt_ift[ 0 ]->Run( );
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

// ---
void SegmentationTool::Watershed( Bial::Image< int > &img,
                                  const Bial::Vector< size_t > &obj_seeds,
                                  const Bial::Vector< size_t > &bkg_seeds ) {
  try {
    qDebug( ) << "Watershed: pf: " << pf;
    if( ( initiated ) && ( pf != 1 ) ) {
      COMMENT( "Was running with other pathfunction. Restart required.", 0 );
      delete int_path_func[ 0 ];
      delete int_path_func[ 1 ];
      delete int_ift[ 0 ];
      delete int_ift[ 1 ];
      int_path_func[ 0 ] = nullptr;
      int_path_func[ 0 ] = nullptr;
      int_ift[ 0 ] = nullptr;
      int_ift[ 1 ] = nullptr;
      initiated = false;
    }
    COMMENT( "Getting initial parameters.", 0 );
    pf = 1;
    size_t size = img.size( );
    if( !initiated ) {
      COMMENT( "Initialize maps.", 0 );
      Bial::Image< int > &lgrad( grad.IntImage( ) );
      pred[ 0 ] = Bial::Image< int >( img.Dim( ) );
      label[ 0 ] = Bial::Image< int >( img.Dim( ) );
      COMMENT( "Initialize path function.", 0 );
      cost[ 0 ] = grad;
      int_path_func[ 0 ] = new Bial::MaxPathFunction< Bial::Image, int >( cost[ 0 ].IntImage( ), &label[ 0 ],
                                                                          &pred[ 0 ], false, lgrad );
      COMMENT( "Initialize queue.", 0 );
      Bial::Image< int > &value( cost[ 0 ].IntImage( ) );
      for( size_t elm = 0; elm < size; ++elm ) {
        value[ elm ] = std::numeric_limits< int >::max( );
      }
      queue = new Bial::GrowingBucketQueue( size, 1.0, true, true );
      InitiateSeeds( 0, obj_seeds, bkg_seeds, value );
      COMMENT( "Initialize IFT.", 0 );
      adj = Bial::AdjacencyType::HyperSpheric( 1.0, label[ 0 ].Dims( ) );
      int_ift[ 0 ] = new Bial::ImageIFT< int >( value, adj, int_path_func[ 0 ], queue );
      initiated = true;
    }
    else {
      COMMENT( "Continuing segmentation.", 0 );
      ( int_path_func[ 0 ] )->DifferentialPropagation( true );
      for( size_t elm = 0; elm < size; ++elm ) {
        queue->State( elm, Bial::BucketState::NOT_VISITED );
      }
      InitiateSeeds( 0, obj_seeds, bkg_seeds, cost[ 0 ].IntImage( ) );
    }
    COMMENT( "Running IFT.", 0 );
    int_ift[ 0 ]->Run( );
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

void SegmentationTool::Watershed( Bial::Image< float > &img,
                                  const Bial::Vector< size_t > &obj_seeds,
                                  const Bial::Vector< size_t > &bkg_seeds ) {
  try {
    if( ( initiated ) && ( pf != 1 ) ) {
      COMMENT( "Was running with other pathfunction. Restart required.", 0 );
      delete flt_path_func[ 0 ];
      delete flt_path_func[ 1 ];
      delete flt_ift[ 0 ];
      delete flt_ift[ 1 ];
      flt_path_func[ 0 ] = nullptr;
      flt_path_func[ 1 ] = nullptr;
      flt_ift[ 0 ] = nullptr;
      flt_ift[ 1 ] = nullptr;
      initiated = false;
    }
    COMMENT( "Getting initial parameters.", 0 );
    pf = 1;
    size_t size = img.size( );
    if( !initiated ) {
      COMMENT( "Initialize maps.", 0 );
      Bial::Image< float > &lgrad( grad.FltImage( ) );
      pred[ 0 ] = Bial::Image< int >( img.Dim( ) );
      label[ 0 ] = Bial::Image< int >( img.Dim( ) );
      COMMENT( "Initialize path function.", 0 );
      cost[ 0 ] = grad;
      flt_path_func[ 0 ] = new Bial::MaxPathFunction< Bial::Image, float >( cost[ 0 ].FltImage( ), &label[ 0 ],
                                                                            &pred[ 0 ], false, lgrad );
      COMMENT( "Initialize queue.", 0 );
      Bial::Image< float > &value( cost[ 0 ].FltImage( ) );
      for( size_t elm = 0; elm < size; ++elm ) {
        value[ elm ] = std::numeric_limits< float >::max( );
      }
      queue = new Bial::GrowingBucketQueue( size, 1.0, true, true );
      InitiateSeeds( 0, obj_seeds, bkg_seeds, value );
      COMMENT( "Initialize IFT.", 0 );
      adj = Bial::AdjacencyType::HyperSpheric( 1.0, label[ 0 ].Dims( ) );
      flt_ift[ 0 ] = new Bial::ImageIFT< float >( value, adj, flt_path_func[ 0 ], queue );
      initiated = true;
    }
    else {
      COMMENT( "Continuing segmentation.", 0 );
      ( flt_path_func[ 0 ] )->DifferentialPropagation( true );
      for( size_t elm = 0; elm < size; ++elm ) {
        queue->State( elm, Bial::BucketState::NOT_VISITED );
      }
      InitiateSeeds( 0, obj_seeds, bkg_seeds, cost[ 0 ].FltImage( ) );
    }
    COMMENT( "Running IFT.", 0 );
    ( flt_ift[ 0 ] )->Run( );
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

void SegmentationTool::FSum( Bial::Image< int > &img,
                             const Bial::Vector< size_t > &obj_seeds,
                             const Bial::Vector< size_t > &bkg_seeds ) {
  try {
    qDebug( ) << "FSum: pf: " << pf;
    if( ( initiated ) && ( pf != 2 ) ) {
      COMMENT( "Was running with other pathfunction. Restart required.", 0 );
      delete int_path_func[ 0 ];
      delete int_path_func[ 1 ];
      delete int_ift[ 0 ];
      delete int_ift[ 1 ];
      int_path_func[ 0 ] = nullptr;
      int_path_func[ 0 ] = nullptr;
      int_ift[ 0 ] = nullptr;
      int_ift[ 1 ] = nullptr;
      initiated = false;
    }
    COMMENT( "Getting initial parameters.", 0 );
    pf = 2;
    size_t size = img.size( );
    if( !initiated ) {
      COMMENT( "Initialize maps.", 0 );
      Bial::Image< int > &lgrad( grad.IntImage( ) );
      pred[ 0 ] = Bial::Image< int >( img.Dim( ) );
      label[ 0 ] = Bial::Image< int >( img.Dim( ) );
      COMMENT( "Initialize path function.", 0 );
      cost[ 0 ] = grad;
      int_path_func[ 0 ] = new Bial::SumPathFunction< Bial::Image, int >( cost[ 0 ].IntImage( ), &label[ 0 ],
                                                                          &pred[ 0 ], false, lgrad );
      COMMENT( "Initialize queue.", 0 );
      Bial::Image< int > &value( cost[ 0 ].IntImage( ) );
      for( size_t elm = 0; elm < size; ++elm ) {
        value[ elm ] = std::numeric_limits< int >::max( );
      }
      queue = new Bial::GrowingBucketQueue( size, 1.0, true, true );
      InitiateSeeds( 0, obj_seeds, bkg_seeds, value );
      COMMENT( "Initialize IFT.", 0 );
      adj = Bial::AdjacencyType::HyperSpheric( 1.0, label[ 0 ].Dims( ) );
      int_ift[ 0 ] = new Bial::ImageIFT< int >( value, adj, int_path_func[ 0 ], queue );
      initiated = true;
    }
    else {
      COMMENT( "Continuing segmentation.", 0 );
      ( int_path_func[ 0 ] )->DifferentialPropagation( true );
      for( size_t elm = 0; elm < size; ++elm ) {
        queue->State( elm, Bial::BucketState::NOT_VISITED );
      }
      InitiateSeeds( 0, obj_seeds, bkg_seeds, cost[ 0 ].IntImage( ) );
    }
    COMMENT( "Running IFT.", 0 );
    int_ift[ 0 ]->Run( );
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

void SegmentationTool::FSum( Bial::Image< float > &img,
                             const Bial::Vector< size_t > &obj_seeds,
                             const Bial::Vector< size_t > &bkg_seeds ) {
  try {
    qDebug( ) << "FSum: pf: " << pf;
    if( ( initiated ) && ( pf != 2 ) ) {
      COMMENT( "Was running with other pathfunction. Restart required.", 0 );
      delete flt_path_func[ 0 ];
      delete flt_path_func[ 1 ];
      delete flt_ift[ 0 ];
      delete flt_ift[ 1 ];
      flt_path_func[ 0 ] = nullptr;
      flt_path_func[ 0 ] = nullptr;
      flt_ift[ 0 ] = nullptr;
      flt_ift[ 1 ] = nullptr;
      initiated = false;
    }
    COMMENT( "Getting initial parameters.", 0 );
    pf = 2;
    size_t size = img.size( );
    if( !initiated ) {
      COMMENT( "Initialize maps.", 0 );
      Bial::Image< float > &lgrad( grad.FltImage( ) );
      pred[ 0 ] = Bial::Image< int >( img.Dim( ) );
      label[ 0 ] = Bial::Image< int >( img.Dim( ) );
      COMMENT( "Initialize path function.", 0 );
      cost[ 0 ] = grad;
      flt_path_func[ 0 ] = new Bial::SumPathFunction< Bial::Image, float >( cost[ 0 ].FltImage( ), &label[ 0 ],
                                                                            &pred[ 0 ], false, lgrad );
      COMMENT( "Initialize queue.", 0 );
      Bial::Image< float > &value( cost[ 0 ].FltImage( ) );
      for( size_t elm = 0; elm < size; ++elm ) {
        value[ elm ] = std::numeric_limits< float >::max( );
      }
      queue = new Bial::GrowingBucketQueue( size, 1.0, true, true );
      InitiateSeeds( 0, obj_seeds, bkg_seeds, value );
      COMMENT( "Initialize IFT.", 0 );
      adj = Bial::AdjacencyType::HyperSpheric( 1.0, label[ 0 ].Dims( ) );
      flt_ift[ 0 ] = new Bial::ImageIFT< float >( value, adj, flt_path_func[ 0 ], queue );
      initiated = true;
    }
    else {
      COMMENT( "Continuing segmentation.", 0 );
      ( flt_path_func[ 0 ] )->DifferentialPropagation( true );
      for( size_t elm = 0; elm < size; ++elm ) {
        queue->State( elm, Bial::BucketState::NOT_VISITED );
      }
      InitiateSeeds( 0, obj_seeds, bkg_seeds, cost[ 0 ].FltImage( ) );
    }
    COMMENT( "Running IFT.", 0 );
    flt_ift[ 0 ]->Run( );
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

// Fourth parameter used ( > 3) only when re-segmentating the image by changing the amount of anchors manually.
int SegmentationTool::connect(int pf_type, double alpha, double beta, int new_anchors ) {
  if( grad_type == -1 ) {
    MorphologicalGradient( );
  }
  Bial::Vector< size_t > obj_seed;
  Bial::Vector< size_t > bkg_seed;
  maskVisible = true;
  for( size_t i = 0; i < seeds.size( ); ++i ) {
    if( seeds[ i ] == 1 ) {
      obj_seed.push_back( i );
    }
    else if( seeds[ i ] == 2 ) {
      bkg_seed.push_back( i );
    }
  }
  if( ( !obj_seed.empty( ) ) && ( !bkg_seed.empty( ) ) ) {
    switch( pf_type ) {
        case 0: {
        COMMENT( "Running Oriented Geodesic Path Function IFT.", 0 );
        switch( guiImage->getImageType( ) ) {
            case Bial::MultiImageType::int_img:
            GeodesicSum( guiImage->getIntImage( ), obj_seed, bkg_seed, alpha, beta );
            break;
            case Bial::MultiImageType::flt_img:
            GeodesicSum( guiImage->getFltImage( ), obj_seed, bkg_seed, alpha, beta );
            break;
            case Bial::MultiImageType::clr_img: {
            Bial::Image< int > img( Bial::ColorSpace::ARGBtoGraybyLuminosity< int >( guiImage->getClrImage( ) ) );
            GeodesicSum( img, obj_seed, bkg_seed, alpha, beta );
            break;
            }
            case Bial::MultiImageType::rcl_img: {
            Bial::Image< int > img( Bial::ColorSpace::ARGBtoGraybyLuminosity< int >( guiImage->getRclImage( ) ) );
            GeodesicSum( img, obj_seed, bkg_seed, alpha, beta );
            break;
          }
            default:
            qDebug( "Error: Segmented uninitialized image." );
        }
        break;
      }
        case 1: {
        COMMENT( "Running Max Path Function IFT.", 0 );
        switch( guiImage->getImageType( ) ) {
            case Bial::MultiImageType::int_img:
            Watershed( guiImage->getIntImage( ), obj_seed, bkg_seed );
            break;
            case Bial::MultiImageType::flt_img:
            Watershed( guiImage->getFltImage( ), obj_seed, bkg_seed );
            break;
            case Bial::MultiImageType::clr_img: {
            Bial::Image< int > img( Bial::ColorSpace::ARGBtoGraybyLuminosity< int >( guiImage->getClrImage( ) ) );
            Watershed( img, obj_seed, bkg_seed );
            break;
          }
            case Bial::MultiImageType::rcl_img: {
            Bial::Image< int > img( Bial::ColorSpace::ARGBtoGraybyLuminosity< int >( guiImage->getRclImage( ) ) );
            Watershed( img, obj_seed, bkg_seed );
            break;
          }
            default:
            qDebug( "Error: Segmented uninitialized image." );
        }
        break;
      }
        default: {
        COMMENT( "Running Sum Path Function IFT.", 0 );
        switch( guiImage->getImageType( ) ) {
            case Bial::MultiImageType::int_img:
            FSum( guiImage->getIntImage( ), obj_seed, bkg_seed );
            break; 
            case Bial::MultiImageType::flt_img:
            FSum( guiImage->getFltImage( ), obj_seed, bkg_seed );
            break;
            case Bial::MultiImageType::clr_img: {
            Bial::Image< int > img( Bial::ColorSpace::ARGBtoGraybyLuminosity< int >( guiImage->getClrImage( ) ) );
            FSum( img, obj_seed, bkg_seed );
            break;
          }
            case Bial::MultiImageType::rcl_img: {
            Bial::Image< int > img( Bial::ColorSpace::ARGBtoGraybyLuminosity< int >( guiImage->getRclImage( ) ) );
            FSum( img, obj_seed, bkg_seed );
            break;
          }
            default:
            qDebug( "Error: Segmented uninitialized image." );
        }
        break;
      }
    }
    mask = label[ 0 ] - Bial::Morphology::ErodeBin( label[ 0 ], Bial::AdjacencyType::Circular( 1.0 ) );
    emit guiImage->imageUpdated( );
    if( new_anchors > 3 ) {
      LiveWirePostProcessing( new_anchors );
      return( new_anchors );
    }
    else {
      bool use_border_length = ( ( label[ 0 ].Dims( ) == 2 ) && ( guiImage->getImageType( ) == Bial::MultiImageType::int_img ) ) ? true : false;
      int border_length = GetBorderLength( );
      if( use_border_length ) 
        LiveWirePostProcessing( border_length / 20 ); 
      else 
        LiveWirePostProcessing( 3 );
      // --
      propagated_paths = Bial::Image< int >( border );
      Bial::Adjacency adj( Bial::AdjacencyType::Circular( 1.1 ) );
      Bial::AdjacencyIterator adj_itr( label[ 0 ], adj );
      size_t img_size = label[ 0 ].size( );
      size_t adj_size = adj.size( );
      size_t adj_pxl;
      //Bial::Vector< bool > visited( img_size, false );
      bool retracing_path;
      int same_lbl_adjs;
      COMMENT( "Retracing propagated paths for later LiveWire comparison.", 4 );
      for( size_t pxl = 0; pxl < img_size; ++pxl ) {
        COMMENT( "Current pixel is already labeled.", 4 );
        if( propagated_paths[ pxl ] > 0 ) { 
          size_t src_pxl = pxl;
          int path_lbl = propagated_paths[ src_pxl ];
          retracing_path = true;
          while( retracing_path ) { 
            same_lbl_adjs = 0;
            for( size_t adj_idx = 0; adj_idx < adj_size; ++adj_idx ) {
              if( ( adj_itr.AdjIdx2( src_pxl, adj_idx, adj_pxl ) ) && ( pred[ 0 ][ src_pxl ] == static_cast< int >( adj_pxl ) ) /*&& ( propagated_paths[ adj_pxl ] == 0 )*/ ) {
                COMMENT( "Found a adjacent pixel that belongs to a path conquered with the same label " << path_lbl << ".", 4 );
                propagated_paths[ adj_pxl ] = path_lbl;
                ++same_lbl_adjs;
              }
            }
            if( same_lbl_adjs > 0 ) { 
              COMMENT( "Pixel adjacency has " << same_lbl_adjs << " neighbours with the same label, it's path must be propagated to the pixel's predecessor.", 4 );
              src_pxl = pred[ 0 ][ src_pxl ];
            }
            else {
              COMMENT( "Path retracing with label " << path_lbl << " reached it's end.", 4 );
              retracing_path = false;
            }
          }
        }
        else{
          COMMENT( "Current pixel isn't yet labeled. Saving a path from it to his first labeled predecessor.", 4 );
          size_t src_pxl = pxl;
          Bial::Vector< int > retraced_path;
          retracing_path = true;
          while( retracing_path ) {
            int local_pred = pred[ 0 ][ src_pxl ];
            int path_lbl = propagated_paths[ src_pxl ];
            if( path_lbl > 0 ) {
              COMMENT( "Found a labeled predecessor, it's label value must be propagated to the path used to reach it.", 4 );
              int path_size = retraced_path.size( );
              retracing_path = false;
              for( int idx = 0; idx < path_size; ++idx ) 
                propagated_paths[ retraced_path[ idx ] ] = path_lbl;
            } 
            else{
              COMMENT( "Predecessor still not labeled, keeping the search for a labeled one.", 4 );
              retraced_path.push_back( src_pxl );
              if( static_cast< int >( local_pred ) >= 0 )
                src_pxl = local_pred;
              else {
                retracing_path = false;
                int path_size = retraced_path.size( );
                for( int idx = 0; idx < path_size; ++idx ) 
                  propagated_paths[ retraced_path[ idx ] ] = path_lbl;
              }  
            }
          }
        }
      }
      /*
      - ok
      for( size_t pxl = 0; pxl < img_size; ++pxl ) {
        if( propagated_paths[ pxl ] > 0 ) { 
          size_t src_pxl = pxl;
          int path_lbl = propagated_paths[ src_pxl ];
          retracing_path = true;
          while( retracing_path ) { 
            same_lbl_adjs = 0;
            for( size_t adj_idx = 0; adj_idx < adj_size; ++adj_idx ) {
              if( ( adj_itr.AdjIdx2( src_pxl, adj_idx, adj_pxl ) ) && ( pred[ 0 ][ src_pxl ] == static_cast< int >( adj_pxl ) ) ) {
                COMMENT( "Found a adjacent pixel that belongs to a path conquered with the same label " << path_lbl << ".", 4 );
                propagated_paths[ adj_pxl ] = path_lbl;
                ++same_lbl_adjs;
              }
            }
            if( same_lbl_adjs > 0 ) { 
              COMMENT( "Pixel adjacency has " << same_lbl_adjs << " neighbours with the same label, it's path must be propagated to the pixel's predecessor.", 4 );
              src_pxl = pred[ 0 ][ src_pxl ];
            }
            else {
              COMMENT( "Path retracing with label " << path_lbl << " reached it's end.", 4 );
              retracing_path = false;
            }
          }
        }
      }
      */
      /*
      - ok
      for( size_t pxl = 0; pxl < img_size; ++pxl ) {
        if( ( propagated_paths[ pxl ] != 0 ) && ( border[ pxl ] != 0 ) ) { 
          size_t src_pxl = pxl;
          int path_lbl = propagated_paths[ src_pxl ];
          retracing_path = true;
          while( retracing_path ) { 
            same_lbl_adjs = 0;
            for( size_t adj_idx = 0; adj_idx < adj_size; ++adj_idx ) {
              if( ( adj_itr.AdjIdx2( src_pxl, adj_idx, adj_pxl ) ) && ( pred[ 0 ][ adj_pxl ] == static_cast< int >( src_pxl ) ) ) {
                COMMENT( "Found a adjacent pixel that belongs to a path conquered with the same label " << path_lbl << ".", 4 );
                propagated_paths[ adj_pxl ] = path_lbl;
                ++same_lbl_adjs;
              }
            }
            if( same_lbl_adjs > 0 ) { 
              COMMENT( "Pixel adjacency has " << same_lbl_adjs << " neighbours with the same label, it's path must be propagated to the pixel's predecessor.", 4 );
              src_pxl = pred[ 0 ][ src_pxl ];
            }
            else {
              COMMENT( "Path retracing with label " << path_lbl << " reached it's end.", 4 );
              retracing_path = false;
            }
          }
        }
      }
      */
      Bial::Write( propagated_paths, "/tmp/my_propagated_paths.pgm" );
      return use_border_length ? border_length / 20 : 3;
    }
  }
  else {
    throw std::runtime_error( "Seeds Missing." );
  }
}


QPixmap SegmentationTool::getLabel( size_t axis ) {
  try {
    const size_t xsz = guiImage->width( axis );
    const size_t ysz = guiImage->heigth( axis );
    if( !seedsVisible && !maskVisible ) {
      return( QPixmap( ) );
    }
    if( !needUpdate[ axis ] ) {
      return( pixmaps[ axis ] );
    }
    const Bial::FastTransform &transf = guiImage->getTransform( axis );
    QImage res( xsz, ysz, QImage::Format_ARGB32 );
    if( !seedsVisible ) {
      res.fill( qRgba( 0, 0, 0, 0 ) );
    }
    else {
#pragma omp parallel for firstprivate(axis, xsz, ysz)
      for( size_t y = 0; y < ysz; ++y ) {
        QRgb *scanLine = ( QRgb* ) res.scanLine( y );
        for( size_t x = 0; x < xsz; ++x ) {
          Bial::Point3D pos = transf( x, y, guiImage->currentSlice( axis ) );
          char pixel = seeds( pos.x, pos.y, pos.z );
          if( pixel == 1 ) {
            scanLine[ x ] = qRgb( 0, 255, 0 );
          }
          else if( pixel == 2 ) {
            scanLine[ x ] = qRgb( 0, 0, 255 );
          }
          else {
            scanLine[ x ] = qRgba( 0, 0, 0, 0 );
          }
        }
      }
    }
    if( maskVisible && ( mask.size( ) == seeds.size( ) ) ) {
#pragma omp parallel for firstprivate(axis, xsz, ysz)
      for( size_t y = 0; y < ysz; ++y ) {
        QRgb *scanLine = ( QRgb* ) res.scanLine( y );
        for( size_t x = 0; x < xsz; ++x ) {
          Bial::Point3D pos = transf( x, y, guiImage->currentSlice( axis ) );
          if( mask( pos.x, pos.y, pos.z ) ) {
            scanLine[ x ] = qRgb( 255, 0, 0 );
          }
        }
      }
    }
    pixmaps[ axis ] = QPixmap::fromImage( res );
    return( pixmaps[ axis ] );
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

void SegmentationTool::MorphologicalGradient( ) {
  try {
    switch( guiImage->getImageType( ) ) {
        case Bial::MultiImageType::int_img:
        grad = Bial::MultiImage( Bial::Gradient::Morphological( guiImage->getIntImage( ) ) );
        break;
        case Bial::MultiImageType::flt_img:
        grad = Bial::MultiImage( Bial::Gradient::Morphological( guiImage->getFltImage( ) ) );
        break;
        case Bial::MultiImageType::clr_img: {
        Bial::Image< int > img( Bial::ColorSpace::ARGBtoGraybyLuminosity< int >( guiImage->getClrImage( ) ) );
        grad = Bial::MultiImage( Bial::Gradient::Morphological( img ) );
        break;
      }
        case Bial::MultiImageType::rcl_img: {
        Bial::Image< int > img( Bial::ColorSpace::ARGBtoGraybyLuminosity< int >( guiImage->getRclImage( ) ) );
        grad = Bial::MultiImage( Bial::Gradient::Morphological( img ) );
        break;
      }
        default:
        qDebug( "Error: Segmenting uninitialized image." );
    }
    grad_type = 0;
    initiated = false;
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

int SegmentationTool::GetBorderLength( ) {
  COMMENT( "Find the higher energy point.", 0 );
  size_t img_size = label[ 0 ].size( );
  size_t contour_size = 0;
  Bial::Adjacency adj( Bial::AdjacencyType::Circular( 1.1 ) );
  Bial::AdjacencyIterator adj_itr( label[ 0 ], adj );
  size_t adj_size = adj.size( );
  Bial::Vector< char > my_border( label[ 0 ].size( ), 0 );
  size_t adj_pxl;
  COMMENT( "Computing the higest energy in the object contourn and the size of the contourn.", 0 );
  for( size_t src_pxl = 0; src_pxl < img_size; ++src_pxl ) {
    if( label[ 0 ][ src_pxl ] != 0 ) {
      for( size_t adj_idx = 0; adj_idx < adj_size; ++adj_idx ) {
        if( ( adj_itr.AdjIdx2( src_pxl, adj_idx, adj_pxl ) ) && ( label[ 0 ][ adj_pxl ] == 0 ) ) {
          ++contour_size;
          my_border[ src_pxl ] = 1;
          break;
        }
      }
    }
  }
  return( contour_size );
}

void SegmentationTool::SobelGradient( ) {
  try {
    switch( guiImage->getImageType( ) ) {
        case Bial::MultiImageType::int_img:
        grad = Bial::MultiImage( guiImage->getIntImage( ) );
        Bial::Gradient::Sobel( guiImage->getIntImage( ), &( grad.IntImage( ) ), nullptr );
        break;
        case Bial::MultiImageType::flt_img:
        grad = Bial::MultiImage( guiImage->getFltImage( ) );
        Bial::Gradient::Sobel( guiImage->getFltImage( ), &( grad.FltImage( ) ), nullptr );
        break;
        case Bial::MultiImageType::clr_img: {
        Bial::Image< int > img( Bial::ColorSpace::ARGBtoGraybyLuminosity< int >( guiImage->getClrImage( ) ) );
        grad = Bial::MultiImage( img );
        Bial::Gradient::Sobel( img, &( grad.IntImage( ) ), nullptr );
        break;
      }
        case Bial::MultiImageType::rcl_img: {
        Bial::Image< int > img( Bial::ColorSpace::ARGBtoGraybyLuminosity< int >( guiImage->getRclImage( ) ) );
        grad = Bial::MultiImage( img );
        Bial::Gradient::Sobel( img, &( grad.IntImage( ) ), nullptr );
        break;
      }
        default:
        qDebug( "Error: Segmenting uninitialized image." );
    }
    grad_type = 1;
    initiated = false;
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

void SegmentationTool::DeleteEllipses( ) {
  try {
    COMMENT( "Deleting old ellipses.", 0 );
    for( auto pt : anchor_ellipse )
      m_scene->removeItem( ( QGraphicsItem* ) pt );
    //qDeleteAll( anchor_ellipse );
    anchor_ellipse.clear( );
    COMMENT( "Done deleting old ellipses.", 0 );
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

void SegmentationTool::PrintAnchors( size_t x_size ) {
  try {
    COMMENT( "Printing " << anchor_position.size( ) - 1 << " anchors.", 0 );
    for( size_t idx = 0; idx < static_cast< size_t >( anchor_position.size( ) - 1 ); ++idx ) {
      size_t pos = anchor_position[ idx ];
      std::div_t pos_div_xsize = std::div( static_cast< int >( pos ), static_cast< int >( x_size ) );
      size_t x = pos_div_xsize.rem;
      size_t y = pos_div_xsize.quot;
      COMMENT( "Adding ellipse " << x << ", " << y << " to scene.", 0 );
      auto point = m_scene->addEllipse( QRectF( x - 2, y - 2, 4, 4 ), QPen( QColor( 0, 255, 0, 128 ), 1 ),
                                        QBrush( QColor( 0, 255, 0, 64 ) ) );
      //point->setFlag( QGraphicsItem::ItemIsMovable, true );
      //point->setFlag( QGraphicsItem::ItemIsSelectable, true );
      COMMENT( "Adding ellipse " << x << ", " << y << " to vector.", 0 );
      anchor_ellipse.append( point );
    }
    COMMENT( "Updating image.", 0 );
    emit guiImage->imageUpdated( );
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

// This function may be executed along with region growing algorithm. A seed label is required for that.
Bial::Image< int > SegmentationTool::ConnectSeeds( ) {  
  if( anchor_ellipse.size( ) != 0 )
    DeleteEllipses( );
  size_t img_size = seeds.size( );
  my_seed = Bial::Image< int >( seeds ); // In this function, negative values are used for background seeds and positive to object seeds.
  Bial::Vector< bool > visited( img_size, false );
  Bial::Adjacency adj( Bial::AdjacencyType::Circular( 1.7 ) );
  Bial::AdjacencyIterator adj_itr( label[ 0 ], adj );
  size_t adj_size = adj.size( );
  int obj_seed_lbl = 1; // Will eventually increase depending on the number of disconnected initial object seeds.
  int bkg_seed_lbl = -1; // Will eventually decrease depending on the number of disconnected initial background seeds.
  size_t adj_pxl;
  COMMENT( "Label seeds into connected components.", 0 );
  for( size_t src_pxl = 0; src_pxl < img_size; ++src_pxl ) {
    if( visited[ src_pxl ] )
      continue;
    visited[ src_pxl ] = true;
    if( seeds[ src_pxl ] != 0 ) { // If this is a seed pixel.
      if( seeds[ src_pxl ] == 1 ) {
        COMMENT( "If seed pixel, label it as a component and propagate the label to other connected seed pixels. ", 3 );
        my_seed[ src_pxl ] = obj_seed_lbl;
        ++obj_seed_lbl;
      }
      else {
        COMMENT( "If background pixel, label it as a component and propagate the label to other connected background pixels. ", 3 );
        my_seed[ src_pxl ] = bkg_seed_lbl;
        --bkg_seed_lbl;
      }
      std::queue< size_t > simple_queue;
      simple_queue.push( src_pxl );
      do {
        size_t seed_pxl = simple_queue.front( );
        simple_queue.pop( );
        for( size_t adj_idx = 1; adj_idx < adj_size; ++adj_idx ) {
          if( ( adj_itr.AdjIdx2( seed_pxl, adj_idx, adj_pxl ) ) && ( seeds[ adj_pxl ] == seeds[ seed_pxl ] ) 
            && ( !visited[ adj_pxl ] ) ) { 
            visited[ adj_pxl ] = true;
            my_seed[ adj_pxl ] = my_seed[ seed_pxl ];
            simple_queue.push( adj_pxl );
          }
        }
      } while( !simple_queue.empty( ) );
    }
  }
  Bial::Write( my_seed, "/tmp/my_seed_components.pgm" );
  COMMENT( "Entering seeds into bucket queue.", 0 );
  Bial::Image< int > &my_cost( cost[ 0 ].IntImage( ) );
  Bial::Image< int > my_pred( my_seed.Dim( ) );
  int max_cost = my_cost.Maximum( );
  Bial::SimpleBucketQueue bucket_queue( img_size, max_cost + 1 );
  for( size_t pxl = 0; pxl < img_size; ++pxl ) {
    if( seeds[ pxl ] != 0 ) {
      bucket_queue.Insert( pxl, 0 );
      my_pred[ pxl ] = -1;
    }
  }
  COMMENT( "Run fill algorithm starting from the seeds and detecting the first shock point between two object/background seed groups.", 0 );
  Bial::Image< int > my_label( my_seed );
  Bial::Matrix< int > obj_adj( obj_seed_lbl, obj_seed_lbl );
  Bial::Matrix< int > bkg_adj( -bkg_seed_lbl, -bkg_seed_lbl );
  obj_adj.Set( img_size );
  bkg_adj.Set( img_size );
  Bial::Vector< std::tuple< int, int > > obj_order;
  Bial::Vector< std::tuple< int, int > > bkg_order;
  while( !bucket_queue.Empty( ) ) {
    size_t src_pxl = bucket_queue.Remove( );
    for( size_t adj_idx = 1; adj_idx < adj_size; ++adj_idx ) {
      if( adj_itr.AdjIdx2( src_pxl, adj_idx, adj_pxl ) ) {
        if( ( my_label[ adj_pxl ] == 0 ) && ( label[ 0 ][ src_pxl ] == label[ 0 ][ adj_pxl ] ) ) {
          COMMENT( "Adjacent pixel belong to non-visited region with same label. Propagate.", 3 );
          bucket_queue.Insert( adj_pxl, my_cost[ adj_pxl ] );
          my_label[ adj_pxl ] = my_label[ src_pxl ];
          my_pred[ adj_pxl ] = src_pxl;
        }
        // Equality test to check if both pixels belong to object or background. Inequality test to check if seed components are distinct.
        else if( ( my_label[ src_pxl ] * my_label[ adj_pxl ] > 0 ) && ( my_label[ src_pxl ] != my_label[ adj_pxl ] ) ) {
          COMMENT( "Both object or background rooted pixels, but with distinct source seed groups.", 3 );
          if( ( my_label[ src_pxl ] > 0 ) && ( obj_adj( my_label[ src_pxl ], my_label[ adj_pxl ] ) == static_cast< int >( img_size ) ) ) {
            COMMENT( "First time these two object seed component labels touch each other.", 3 );
            obj_adj( my_label[ src_pxl ], my_label[ adj_pxl ] ) = src_pxl;
            obj_adj( my_label[ adj_pxl ], my_label[ src_pxl ] ) = adj_pxl;
            obj_order.push_back( std::make_tuple( my_label[ src_pxl ], my_label[ adj_pxl ] ) );
          }
          if( ( my_label[ src_pxl ] < 0 ) && ( bkg_adj( -my_label[ src_pxl ], -my_label[ adj_pxl ] ) == static_cast< int >( img_size ) ) ) {
            COMMENT( "First time these two background seed component labels touch each other.", 3 );
            bkg_adj( -my_label[ src_pxl ], -my_label[ adj_pxl ] ) = src_pxl;
            bkg_adj( -my_label[ adj_pxl ], -my_label[ src_pxl ] ) = adj_pxl;
            bkg_order.push_back( std::make_tuple( -my_label[ src_pxl ], -my_label[ adj_pxl ] ) );
          }
        }
      }
    }
  }
  Bial::Write( my_label, "/tmp/my_label_components.pgm" );
  COMMENT( "Connecting object seeds through object region.", 3 );
  Bial::Matrix< int > obj_conn_map( obj_seed_lbl, obj_seed_lbl );
  obj_conn_map.Set( 0 );
  for( size_t obj_idx = 0; obj_idx < obj_order.size( ); ++obj_idx ) {
    int obj_0 = std::get< 0 >( obj_order[ obj_idx ] );
    int obj_1 = std::get< 1 >( obj_order[ obj_idx ] );
    if( obj_conn_map( obj_0, obj_1 ) == 0 ) {
      //COMMENT( "Labels not connected between them. Make connection using predecessors.", 0 );
      obj_conn_map( obj_0, obj_1 ) = 1;
      obj_conn_map( obj_1, obj_0 ) = 1;
      for( size_t obj_map_idx = 0; obj_map_idx < static_cast< size_t >( obj_seed_lbl ); ++obj_map_idx ) {
        if( ( obj_conn_map( obj_0, obj_map_idx ) == 1 ) && ( obj_conn_map( obj_1, obj_map_idx ) == 0 ) ) {
          obj_conn_map( obj_1, obj_map_idx ) = 1;
          obj_conn_map( obj_map_idx, obj_1 ) = 1;
        }
        if( ( obj_conn_map( obj_1, obj_map_idx ) == 1 ) && ( obj_conn_map( obj_0, obj_map_idx ) == 0 ) ) {
          obj_conn_map( obj_0, obj_map_idx ) = 1;
          obj_conn_map( obj_map_idx, obj_0 ) = 1;
        }
      }
      //COMMENT( "Labeling path from object label 0.", 0 );
      size_t pxl = obj_adj( obj_0, obj_1 );
      while( my_pred[ pxl ] != -1 ) {
        my_seed[ pxl ] = 1;
        pxl = my_pred[ pxl ];
      }
      //COMMENT( "Labeling path from object label 1.", 0 );
      pxl = obj_adj( obj_1, obj_0 );
      while( my_pred[ pxl ] != -1 ) {
        my_seed[ pxl ] = 1;
        pxl = my_pred[ pxl ];
      }
    }
  }
  COMMENT( "Connecting background seeds through background region.", 0 );
  Bial::Matrix< int > bkg_conn_map( -bkg_seed_lbl, -bkg_seed_lbl );
  bkg_conn_map.Set( 0 );
  for( size_t bkg_idx = 0; bkg_idx < bkg_order.size( ); ++bkg_idx ) {
    int bkg_0 = std::get< 0 >( bkg_order[ bkg_idx ] );
    int bkg_1 = std::get< 1 >( bkg_order[ bkg_idx ] );
    if( bkg_conn_map( bkg_0, bkg_1 ) == 0 ) {
      //COMMENT( "Labels not connected between them. Make connection using predecessors.", 0 );
      bkg_conn_map( bkg_0, bkg_1 ) = 1;
      bkg_conn_map( bkg_1, bkg_0 ) = 1;
      //COMMENT( "Update the connectivity map.", 0 );
      for( size_t bkg_map_idx = 0; bkg_map_idx < static_cast< size_t >( -bkg_seed_lbl ); ++bkg_map_idx ) {
        if( ( bkg_conn_map( bkg_0, bkg_map_idx ) == 1 ) && ( bkg_conn_map( bkg_1, bkg_map_idx ) == 0 ) ) {
          bkg_conn_map( bkg_1, bkg_map_idx ) = 1;
          bkg_conn_map( bkg_map_idx, bkg_1 ) = 1;
        }
        if( ( bkg_conn_map( bkg_1, bkg_map_idx ) == 1 ) && ( bkg_conn_map( bkg_0, bkg_map_idx ) == 0 ) ) {
          bkg_conn_map( bkg_0, bkg_map_idx ) = 1;
          bkg_conn_map( bkg_map_idx, bkg_0 ) = 1;
        }
      }
      //COMMENT( "Labeling path from background label 0.", 0 );
      size_t pxl = bkg_adj( bkg_0, bkg_1 );
      while( my_pred[ pxl ] != -1 ) {
        my_seed[ pxl ] = -1;
        pxl = my_pred[ pxl ];
      }
      //COMMENT( "Labeling path from background label 1.", 0 );
      pxl = bkg_adj( bkg_1, bkg_0 );
      while( my_pred[ pxl ] != -1 ) {
        my_seed[ pxl ] = -1;
        pxl = my_pred[ pxl ];
      }
    }
  }
  COMMENT( "Labeling all seeds into 1 (object) and 2 (background)", 0 );
  for( size_t pxl = 0; pxl < img_size; ++pxl ) {
    if( my_seed[ pxl ] < 0 )
      my_seed[ pxl ] = 2;
    else if( my_seed[ pxl ] > 0 )
      my_seed[ pxl ] = 1;
  }
  return( my_seed );
}

void SegmentationTool::LiveWirePostProcessing( size_t anchors ) {
  try {
    if( ( label[ 0 ].Dims( ) != 2 ) || ( label[ 0 ].size( ) < 10 ) ||
        ( guiImage->getImageType( ) != Bial::MultiImageType::int_img ) ) {
      qDebug( "Warning: LiveWire Post Processing available only for 2D integer image." );
      return;
    }
    COMMENT( "Find the higher energy point.", 3 );
    size_t img_size = label[ 0 ].size( );
    double higher_eng_val = 0.0;
    size_t higher_pixel = 0;
    size_t contour_size = 0;

    Bial::Vector< size_t > high_eng_pxls;

    Bial::Adjacency adj( Bial::AdjacencyType::Circular( 1.1 ) );
    Bial::AdjacencyIterator adj_itr( label[ 0 ], adj );
    size_t adj_size = adj.size( );
    Bial::Image< int > my_grad( grad.IntImage( ) );
    Bial::Write( my_grad, "/tmp/my_grad.pgm" ); 
    Bial::Write( label[ 0 ], "/tmp/my_label.pgm" );
    Bial::Write( seeds, "/tmp/my_ini_seeds.pgm" );
    border = Bial::Image< int >( label[ 0 ].Dim( ) );
    size_t adj_pxl;
    COMMENT( "Computing the higest energy in the object contourn and the size of the contourn.", 3 );
    for( size_t src_pxl = 0; src_pxl < img_size; ++src_pxl ) {
      if( label[ 0 ][ src_pxl ] != 0 ) {
        for( size_t adj_idx = 0; adj_idx < adj_size; ++adj_idx ) {
          if( ( adj_itr.AdjIdx2( src_pxl, adj_idx, adj_pxl ) ) && ( label[ 0 ][ adj_pxl ] == 0 ) ) {
            ++contour_size;
            border[ src_pxl ] = 1;
            high_eng_pxls.push_back( src_pxl );
            if( higher_eng_val < my_grad[ src_pxl ] ) {
              higher_eng_val = my_grad[ src_pxl ];
              higher_pixel = src_pxl;
            }
            break;
          }
        }
      }
    }
    if( contour_size < 10 ) {
      qDebug( "Warning: Contour must be at least 10 pixels long." );
      return;
    }
    COMMENT( "Border tracing.", 0 );
    contour = Bial::Vector< size_t >( contour_size * 3 );
    contour[ 0 ] = higher_pixel;
    Bial::Adjacency trc_adj( 8, 2 ); // Sorted adjacency
    trc_adj( 0, 0 ) = 1; trc_adj( 0 , 1 ) = -1; // NE
    trc_adj( 1, 0 ) = 1; trc_adj( 1 , 1 ) = 0; // E
    trc_adj( 2, 0 ) = 1; trc_adj( 2 , 1 ) = 1; // SE
    trc_adj( 3, 0 ) = 0; trc_adj( 3 , 1 ) = 1; // S
    trc_adj( 4, 0 ) = -1; trc_adj( 4 , 1 ) = 1; // SW
    trc_adj( 5, 0 ) = -1; trc_adj( 5 , 1 ) = 0; // W
    trc_adj( 6, 0 ) = -1; trc_adj( 6 , 1 ) = -1; // NW
    trc_adj( 7, 0 ) = 0; trc_adj( 7 , 1 ) = -1; //N
    Bial::AdjacencyIterator trc_adj_itr( label[ 0 ], trc_adj );
    size_t trc_adj_size = trc_adj.size( );
    size_t curr_cntr_pxl = 1;
    size_t adj_idx = 0;
    ++border[ curr_cntr_pxl ];
    do {
      if( curr_cntr_pxl == contour_size * 3 ) {
        qDebug( "Error in contour tracking. Contour must be holeless and without pixel-wise width paths." );
        return;
      }
      COMMENT( "Raster the adjacent pixels in clockwise direction, starting from the first adjacent to opposite direction of the last movement.", 3 );
      COMMENT( "adj_idx: " << adj_idx << ", curr_cntr_pxl - 1: " << curr_cntr_pxl - 1 << ", contour[ " << curr_cntr_pxl - 1 << " ]: " <<
               ( contour[ curr_cntr_pxl - 1 ] ) % label[ 0 ].size( 0 ) << ", " << std::floor( ( contour[ curr_cntr_pxl - 1 ] ) / label[ 0 ].size( 0 ) ), 4 );
      adj_idx = ( adj_idx + 5 ) % trc_adj_size;
      do{
        if( ( trc_adj_itr.AdjIdx2( contour[ curr_cntr_pxl - 1 ], adj_idx, adj_pxl ) ) && ( border[ adj_pxl ] == 1 ) ) {
          contour[ curr_cntr_pxl ] = adj_pxl;
          ++border[ curr_cntr_pxl ];
          curr_cntr_pxl++;
          break;
        }
        adj_idx = ( adj_idx + 1 ) % trc_adj_size;
      } while( true );
    } while( ( curr_cntr_pxl < 22 ) || ( contour[ curr_cntr_pxl - 1 ] != contour[ 1 ] ) || ( contour[ curr_cntr_pxl - 2 ] != contour[ 0 ] ) );
    size_t last_pxl = curr_cntr_pxl - 3;
    contour[ last_pxl + 1 ] = img_size;
    COMMENT( "Getting the anchor pixels in n +- m intervals.", 3 );
    if( anchors > last_pxl / 10 ) // Ensure that there are at most 1/10th of the contour size anchors.
      anchors = std::max< int >( 3, last_pxl / 10 ); // Ensuring at least 2 anchors.
    double anchor_distance = last_pxl / static_cast< double >( anchors );
    size_t anchor_radius = std::floor( anchor_distance / 4.0 );
    anchor_position = Bial::Vector< size_t >( anchors + 1 );
    anchor_position[ 0 ] = higher_pixel;
    size_t ancr_idx = 1;
    COMMENT( "anchor_distance: " << anchor_distance << ", last_pxl: " << last_pxl, 3 );
    for( double brd_idx = anchor_distance; brd_idx < last_pxl - 2.5; brd_idx += anchor_distance ) {
      size_t cur_pos = static_cast< size_t >( std::round( brd_idx ) ) - anchor_radius;
      size_t max_pos = cur_pos + anchor_radius;
      int max_energy = my_grad[ cur_pos ];
      anchor_position[ ancr_idx ] = contour[ cur_pos ];
      COMMENT( "Initial anchor: " << contour[ cur_pos ], 4 );
      COMMENT( "cur_pos: " << cur_pos << ", max_pos: " << max_pos << ", max_energy: " << max_energy, 3 );
      while( cur_pos <= max_pos ) {
        ++cur_pos;
        COMMENT( "max_energy: " << max_energy << ", my_grad[ " << cur_pos << " ]: " << my_grad[ cur_pos ], 3 );
        if( max_energy < my_grad[ contour[ cur_pos ] ] ) {
          max_energy = my_grad[ contour[ cur_pos ] ];
          anchor_position[ ancr_idx ] = contour[ cur_pos ];
          COMMENT( "Selected anchor: " << contour[ cur_pos ], 4 );
        }
      }
      ++ancr_idx;
    }
    anchor_position[ anchors ] = contour[ last_pxl - 1 ]; // Error: It should be last_pxl, but it is not closing the last border segment with it.
    COMMENT( "Anchors: " << anchors + 1, 4 );
    for( size_t ancr_idx = 0; ancr_idx <= anchors; ++ancr_idx ) {
      COMMENT( "Anchor[ " << ancr_idx << " ]: " << anchor_position[ ancr_idx ], 3 );
    }
    COMMENT( "Labeling contour sectors. Used as limitation for livewire to avoid crossing the wrong borders.", 3 );
    ancr_idx = 0;
    for( size_t cntr_idx = 0; cntr_idx < last_pxl; ++cntr_idx ) {
      border[ contour[ cntr_idx ] ] = ancr_idx + 1;
      if( contour[ cntr_idx ] == anchor_position[ ancr_idx + 1 ] )
        ++ancr_idx;
    }
    Bial::Write( border, "/tmp/my_border.pgm" );
    COMMENT( "Linking all background/object seeds in order to avoid leaving them in/out of the contour.", 3 );
    Bial::Image< int > my_seed( ConnectSeeds( ) );
    Bial::Write( my_seed, "/tmp/my_conn_seeds.pgm" );
    COMMENT( "Labeling seed image to set allowed paths. Internal seeds are forbidden. " <<
             "Crossing single object seed line is forbidden.", 3 );
    Bial::Image< int > seed_bdr( my_seed - Bial::Morphology::ErodeBin( my_seed, Bial::AdjacencyType::Circular( 1.5f ) ) );
    COMMENT( "Detecting isolated point that are not necessary to the continuity of object seed borders." <<  
              "These are pixels with only one continuous adjacent object border segment. Labeled to 3.", 3 );
    for( size_t src_pxl = 0; src_pxl < img_size; ++src_pxl ) {
      if( seed_bdr[ src_pxl ] == 1 ) {
        bool initialized_adj = false;
        bool curr_is_adj_border = false;
        uchar borders = 0;
        uchar non_borders = 0;
        for( adj_idx = 0; adj_idx < trc_adj_size; ++adj_idx ) {
          if( trc_adj_itr.AdjIdx2( src_pxl, adj_idx, adj_pxl ) ) {
            if( ( seed_bdr[ adj_pxl ] == 1 ) || ( seed_bdr[ adj_pxl ] == 3 ) ) {
              if( !initialized_adj )
                curr_is_adj_border = true;
              if( !curr_is_adj_border ) {
                ++non_borders;
                curr_is_adj_border = true;
              }
            }
            else {
              if( !initialized_adj )
                curr_is_adj_border = false;
              if( curr_is_adj_border ) {
                ++borders;
                curr_is_adj_border = false;
              }
            }
            initialized_adj = true;
          }
        }
        if( ( borders < 2 ) && ( non_borders < 2 ) ){
          seed_bdr[ src_pxl ] = 3; 
          COMMENT("Extremity pixel detected. Store it.", 3 );
          extremity_pxl.push_back( src_pxl ); 
        }
      }
    }
    std::queue< size_t > seed_queue;
    Bial::Vector< bool > extremity( img_size, false);
    seed_img = Bial::Image< int >( my_grad.Dim( ) ); // -2: Forbidden pixel; -1: allowed pixel; 0: not visited; > 0: non-seed adjacent to seed border.
    int seed_side_lbl = 1; // Label assigned to each side of a single pixel line of seeds.
    COMMENT( "Building seed_img. It contains the forbidden regions with -2, allowed regions with -1. " <<
             "Values higher than 0 are set to non-seed pixels adjacent to object seed borders. " <<
             "These are used to prevent crossing object seeds.", 3 );
    for( size_t src_pxl = 0; src_pxl < img_size; ++src_pxl ) {
      if( seed_img[ src_pxl ] == 0 ) { // Testing wheather pixel was visited or not.
        if( seed_bdr[ src_pxl ] % 2 == 1 ) {
          COMMENT( "Border of object seed. Allow live-wire to pass over it.", 3 );
          seed_img[ src_pxl ] = -1;
        }
        else if( my_seed[ src_pxl ] != 0 ) {
          COMMENT( "Interior object seed or background seed. Forbidden.", 3 );
          seed_img[ src_pxl ] = -2;
        }
        else {
          COMMENT( "Non-seed pixel. Allow live-wire to pass over it. Must check if it is adjacent to object seed border pixel", 3 );
          seed_img[ src_pxl ] = -1;
          seed_queue.push( src_pxl );
          COMMENT( "Checking if src_pxl belongs to a border. If so, paint all border with the same color.", 3 );
          do {
            size_t brd_pxl = seed_queue.front( );
            seed_queue.pop( );
            COMMENT( "Cheking adjacents for any seed border pixels.", 3 );
            std::string adj_seed_type = "none";
            for( adj_idx = 0; adj_idx < trc_adj_size; ++adj_idx ) {
              if( ( trc_adj_itr.AdjIdx2( brd_pxl, adj_idx, adj_pxl ) ) && ( seed_bdr[ adj_pxl ] % 2 == 1 ) ) {
                COMMENT( "Adjacent to seed border pixel. Setting label.", 3 );
                if( seed_bdr[ adj_pxl ] == 3 ) {
                  adj_seed_type = "end_point";
                }
                else{
                  adj_seed_type = "middle_point";
                  break;
                }
              }
            }
            if( adj_seed_type == "middle_point" ) {
              seed_img[ brd_pxl ] = seed_side_lbl;
              COMMENT( "Inserting in queue all non-seed 4-adjacent pixels if it is non-seed pixel.", 3 );
              if( ( trc_adj_itr.AdjIdx2( brd_pxl, 1, adj_pxl ) ) && ( seed_img[ adj_pxl ] == 0 ) && ( my_seed[ adj_pxl ] == 0 )  ) {
                seed_img[ adj_pxl ] = -1;
                seed_queue.push( adj_pxl );
              }
              if( ( trc_adj_itr.AdjIdx2( brd_pxl, 3, adj_pxl ) ) && ( seed_img[ adj_pxl ] == 0 ) && ( my_seed[ adj_pxl ] == 0 ) ) {
                seed_img[ adj_pxl ] = -1;
                seed_queue.push( adj_pxl );
              }
              if( ( trc_adj_itr.AdjIdx2( brd_pxl, 5, adj_pxl ) ) && ( seed_img[ adj_pxl ] == 0 ) && ( my_seed[ adj_pxl ] == 0 ) ) {
                seed_img[ adj_pxl ] = -1;
                seed_queue.push( adj_pxl );
              }
              if( ( trc_adj_itr.AdjIdx2( brd_pxl, 7, adj_pxl ) ) && ( seed_img[ adj_pxl ] == 0 ) && ( my_seed[ adj_pxl ] == 0 ) ) {
                seed_img[ adj_pxl ] = -1;
                seed_queue.push( adj_pxl );
              }
            }
          } while( !seed_queue.empty( ) );
          if( seed_img[ src_pxl ] > 0 ) {
            COMMENT( "Increasing label if last pixel belonged to seed border.", 3 );
            ++seed_side_lbl;
          }
        }
      }
    }
    COMMENT( "Propagating extremity pixel's labels to non-visited regions.", 3 );
    size_t extremity_pxl_size = extremity_pxl.size( );
    for( size_t idx = 0; idx < extremity_pxl_size; ++idx ) {
      size_t pxl = extremity_pxl[ idx ];
      seed_img[ pxl ] = seed_side_lbl;
      for( adj_idx = 0; adj_idx < trc_adj_size; ++adj_idx ) {
        if( ( trc_adj_itr.AdjIdx2( pxl, adj_idx, adj_pxl ) ) && ( seed_bdr[ adj_pxl ] == 0 ) && ( seed_img[ adj_pxl ] < 1 ) ) 
          seed_img[ adj_pxl ] = seed_side_lbl;
      }
      ++seed_side_lbl;
    } 
    COMMENT( "Assigning neighbour labels to the label adjacency vector.", 3 );
    lbl_adj_list = Bial::Vector< Bial::Vector< size_t > >( seed_side_lbl );
    for( size_t idx = 0; idx < extremity_pxl_size; ++idx ) {
      size_t src_pxl = extremity_pxl[ idx ];
      int src_lbl = seed_img[ src_pxl ];
      for( adj_idx = 0; adj_idx < trc_adj_size; ++adj_idx ) {
        if( ( trc_adj_itr.AdjIdx2( src_pxl, adj_idx, adj_pxl ) ) && ( seed_img[ adj_pxl ] > 0 ) &&
          ( src_lbl != seed_img[ adj_pxl ] ) ) {
          size_t adj_lbl = seed_img[ adj_pxl ];
          size_t adj_lbl_idx = 0;
          size_t src_lbl_size = lbl_adj_list[ src_lbl ].size( );
          while( ( adj_lbl_idx < src_lbl_size ) && ( adj_lbl != lbl_adj_list[ src_lbl ][ adj_lbl_idx ] ) )
            ++adj_lbl_idx;
          /////////<<<<<<< HEAD - COMMIT...
          if( adj_lbl_idx == src_lbl_size ) {
            lbl_adj_list[ src_lbl ].push_back( adj_lbl );
            lbl_adj_list[ adj_lbl ].push_back( src_lbl );
          }
        }
      }
    }
    
    COMMENT( "Number of border labels:" << seed_side_lbl, 0 );
    label[ 2 ] = mask;
    COMMENT( "Adjusting higher value point pixels to their correct labels.", 3 );
    size_t hep_size = high_eng_pxls.size( );
    for( size_t idx = 0; idx < hep_size; ++idx ) {
      size_t pxl = high_eng_pxls[ idx ];
      if( border[ pxl ] == 1 ) {
        int diff_lbl_num = 0;
        int lbl = 0;
        for( adj_idx = 0; adj_idx < trc_adj_size; ++adj_idx ) {
          if( ( trc_adj_itr.AdjIdx2( pxl, adj_idx, adj_pxl ) ) && ( border[ adj_pxl ] != 0 ) ) {
            if( border[ adj_pxl ] != 1) { 
              COMMENT( "If pixel has border = 1 and is adjacent to more than two pixels with border != 1 it must be adjusted.", 3 );
              ++diff_lbl_num;
              lbl = border[ adj_pxl ];
            }
          }
        }
        if( diff_lbl_num >= 2 )
          border[ pxl ] = lbl; // Label adjust.
      }
    }
    Bial::Write( seed_bdr, "/tmp/my_seed_brd.pgm" );
    Bial::Write( seed_img, "/tmp/my_seed_img.pgm" );
    Bial::Write( border, "/tmp/my_border_lbls.pgm" );
    // ======= - COMMIT END ...
//           if( adj_lbl_idx == src_lbl_size )
//             lbl_adj_list[ src_lbl ].push_back( adj_lbl );
//           lbl_count++;
//         }
//       }
//     }
//     COMMENT( "Number of border labels:" << seed_side_lbl, 0 );
//     Bial::Write( seed_bdr, "/tmp/my_seed_brd.pgm" );
//     Bial::Write( seed_img, "/tmp/my_seed_img.pgm" );
//     COMMENT( "Computing gradient complement.", 4 );
// //    Bial::Intensity::Complement( my_grad );
// //    COMMENT( "Linking anchors with livewire.", 0 );
//     label[ 2 ] = mask;
// //    int brd_lbl = seed_img[ anchor_position[ 0 ] ];
// //    for( ancr_idx = 0; ancr_idx < anchors; ++ancr_idx )
// //      brd_lbl = LiveWire( my_grad, my_seed, seed_img, borders, ancr_idx + 1, anchor_position[ ancr_idx ], anchor_position[ ancr_idx + 1 ], brd_lbl );
// //    COMMENT( "Returning result.", 0 );
// //    for( size_t src_pxl = 0; src_pxl < img_size; ++src_pxl ) {
// //      if( seed_img[ src_pxl ] == -1 )
// //        seed_img[ src_pxl ] = 0;
// //      if( seed_img[ src_pxl ] == -2 )
// //        seed_img[ src_pxl ] = seed_side_lbl + 1;
// //    }
// //    Bial::Write( seed_img, "/tmp/my_seed_img.pgm" );
// //    mask = label[ 2 ];
// //    Bial::Write( mask, "/tmp/my_livewire.pgm" );
// >>>>>>> 650de6f435c6034e60876d3e7809804a4c96a5f4
    COMMENT( "Printing the anchors into the label.", 4 );
    PrintAnchors( my_grad.size( 0 ) );
    emit guiImage->imageUpdated( );
  
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

// my_grad: gradient complement of the image.
// my_seed: image containing connected bkg and obj seeds. 0: no seed; 1: obj seed; 2: background seed; 3: obj seed end point.
// seed_zone_img: image containning connected bkg seeds and contours around connected obj seeds. 0: no seed or contour; 1 to n-1: distinct obj contour; n: bkg seed.
// borders: pixels of the border of the segmented region by region growing divided in segments by the anchors. Each segment has a color. 0: no border; 1-n border segments.
// anchor: current anchor number, which is the same as one of the border segments.
// ini_pxl, end_pxl: anchor coordinates and last pixel in the path which is the point before next anchor.
// obj_seed_contour_lbl: 0: if initial pixel does no belong to contour of object seeds. > 0: if pixel belongs to contour of object seeds or lays on object seeds. In the last case,
//                       it has the label of the object seed contour from which the path reached seed pixels.

// seed_zone_img -> -2: Forbidden pixel; -1: allowed pixel; 0: not visited; > 0: non-seed adjacent to seed border.
int SegmentationTool::LiveWire( const Bial::Image< int > &my_grad, const Bial::Image< int > &my_seed, const Bial::Image< int > &seed_zone_img,
                                const Bial::Image< int > &borders, size_t anchor, size_t ini_pxl, size_t end_pxl, int brd_lbl ) {
  try {
    if( ( ini_pxl == 0 ) || ( end_pxl == 0 ) ) {
      std::cout << "ini_pxl = " << ini_pxl << ", end_pxl: " << end_pxl << std::endl;
    }
    size_t size = my_grad.size( );
    size_t x_size = my_grad.size( 0 );
    COMMENT( "Initiating maps and seeds.", 4 );
    Bial::Image< int > value( my_grad );
    Bial::Image< int > predecessor( my_grad.Dim( ), my_grad.PixelSize( ) );
    Bial::Image< int > seed_brd( value.Dim( ) );
    Bial::Adjacency adj( 8, 2 ); // Sorted adjacency
    adj( 0, 0 ) = 1; adj( 0 , 1 ) = -1; // NE
    adj( 1, 0 ) = 1; adj( 1 , 1 ) = 0; // E
    adj( 2, 0 ) = 1; adj( 2 , 1 ) = 1; // SE
    adj( 3, 0 ) = 0; adj( 3 , 1 ) = 1; // S
    adj( 4, 0 ) = -1; adj( 4 , 1 ) = 1; // SW
    adj( 5, 0 ) = -1; adj( 5 , 1 ) = 0; // W
    adj( 6, 0 ) = -1; adj( 6 , 1 ) = -1; // NW
    adj( 7, 0 ) = 0; adj( 7 , 1 ) = -1; //N
    Bial::RotatingBucketQueue queue( size, my_grad.Maximum( ) + 1 );
    for( size_t pxl = 0; pxl < size; ++pxl ) {
      if( ( label[ 2 ][ pxl ] != 0 ) || ( seed_zone_img[ pxl ] == -2 ) )
        value[ pxl ] = 0;
      else
        value[ pxl ] = std::numeric_limits< int >::max( );
    }
    value[ ini_pxl ] = 0;
    seed_brd[ ini_pxl ] = brd_lbl;
    queue.Insert( ini_pxl, 0 );
    predecessor[ ini_pxl ] = -1;
    COMMENT( "Computing IFT.", 4 );
    Bial::AdjacencyIterator adj_itr( value, adj );
    size_t adj_size = adj.size( );
    size_t src_pxl = ini_pxl;
    size_t adj_pxl;
    while( ( !queue.Empty( ) ) && ( src_pxl != end_pxl ) ) {
      src_pxl = queue.Remove( );
      COMMENT("Checking for forbidden segmentation border crossing.", 3 );
      queue.Finished( src_pxl );
      for( size_t adj_idx = 0; adj_idx < adj_size; ++adj_idx ) {
        if( ( adj_itr.AdjIdx2( src_pxl, adj_idx, adj_pxl ) ) &&
            ( queue.State( adj_pxl ) != Bial::BucketState::REMOVED ) &&
            // <<<<<<< HEAD - COMMIT...
            ( value[ src_pxl ] < value[ adj_pxl ] ) ) { 
          COMMENT( "Avoiding forbidden regions: background and internal object seeds.", 3 );
          if ( borders[ adj_pxl ] == -2 ){
            COMMENT( "Pixel " << adj_pxl << " avoided forbidden region.", 0 );
            continue;
          }            
          COMMENT( "Avoid crossing borders from other segments. If it is a diagonal direction and neighbor in the horizontal of this diagonal belongs to a border" <<
                   "and vertical and horizontal of the diagonal belong to the same border and the horizontal does not belong to current border segment.", 3 );
          /*
            if( ( propagated_paths[ adj_pxl ] != static_cast< int >( anchor ) )  || // border trocado por propagated_paths.
          */
          if( ( adj_idx % 2 == 0 ) && ( borders[ adj_pxl - adj( adj_idx, 0 ) ] != 0 ) &&
              ( borders[ adj_pxl - x_size * adj( adj_idx, 1 ) ] == borders[ adj_pxl - adj( adj_idx, 0 ) ] ) &&
              ( borders[ adj_pxl - adj( adj_idx, 0 ) ] != static_cast< int >( anchor ) ) ) {
            COMMENT( "Pixel " << adj_pxl << " avoided crossing borders from other segments.", 0 );
            continue; // In this case, it is not a valid adjacent to propagate the cost.
          }
          COMMENT( "Checking if adjacent is over forbidden region or If it is a diagonal and the vertical of the diagonal " <<
                   "direction is background seed and the horizontal of the diagonal direction is background seed.", 3 );
          if( ( seed_zone_img[ adj_pxl ] == -2 ) ||
              ( ( adj_idx % 2 == 0 ) && ( seed_zone_img[ adj_pxl - x_size * adj( adj_idx, 1 ) ] == -2 ) &&
                ( seed_zone_img[ adj_pxl - adj( adj_idx, 0 ) ] == -2 ) ) ) {
            COMMENT( "Pixel " << adj_pxl << " avoided crossing forbidden region or diagonal.", 0 );
            continue; // 
          }
          int src_seed_lbl = seed_brd[ src_pxl ];
          COMMENT( "Checking for ... something...", 3 ); // It might be a bug here.
          if( ( src_seed_lbl > 0 )  && ( seed_zone_img[ adj_pxl ] > 0 ) && ( seed_zone_img[ adj_pxl ] != src_seed_lbl ) ) {
            for( size_t lbl_idx = 0; lbl_idx < lbl_adj_list[ src_seed_lbl ].size( ); ++lbl_idx ) {
              if( seed_zone_img[ adj_pxl ] == static_cast< int >( lbl_adj_list[ src_seed_lbl ][ lbl_idx ] ) ) {
                COMMENT( "Pixel " << adj_pxl << " avoided...something.", 0 );
                continue;
              }
            }
          }
          // ======= END - COMMIT...
          //   ( value[ src_pxl ] < value[ adj_pxl ] ) ) {
          // COMMENT("Checking for forbidden segmentation border crossing.", 3);
          // if ( borders[adj_pxl] == -2 ) {
          //   COMMENT( "Avoiding forbidden regions: background and internal object seeds.", 3);
          //   continue;
          // }
          // /*
          // if ( ( borders[ adj_pxl ] != static_cast< int >( anchor ) ) // Avoid crossing borders from other segments.
          //     || ((adj_idx % 2 == 0) && (borders[ adj_pxl - adj( adj_idx, 0 ) ] != 0 ) && // If it is a diagonal direction and neighbor in the horizontal of this diagonal belongs to a border...
          //     ( borders[ adj_pxl - x_size * adj(adj_idx, 1)] == borders[ adj_pxl - adj(adj_idx, 0) ] ) && // and vertical and horizontal of the diagonal belong to the same border...
          //     ( borders[ adj_pxl - adj( adj_idx, 0 ) ] != static_cast< int >( anchor ) ) ) ) // and the horizontal does not belong to current border segment.
          //     continue; // In this case, it is not a valid adjacent to propagate the cost.
          // if ( ( seed_zone_img[adj_pxl] == -2) || // If adjacent is over forbidden region or...
          //     ((adj_idx % 2 == 0) && (seed_zone_img[adj_pxl - x_size * adj(adj_idx, 1)] == -2) && // If it is a diagonal and the vertical of the diagonal direction is background seed...
          //     (seed_zone_img[adj_pxl - adj(adj_idx, 0)] == -2))) // and horizontal of the diagonal direction is background seed.
          //     continue; // In this case, it is not a valid adjacent to propagate the cost.
          // int src_seed_lbl = seed_brd[ src_pxl ];
          // if( ( src_seed_lbl > 0)  && ( seed_zone_img[ adj_pxl ] > 0 ) && ( seed_zone_img[ adj_pxl ] != src_seed_lbl ) 
          //   && ( seed_zone_img[ adj_pxl ] != static_cast< int >( lbl_adj_list[ src_seed_lbl ][ 0 ] ) )
          //   && ( seed_zone_img[ adj_pxl ] != static_cast< int >( lbl_adj_list[ src_seed_lbl ][ 1 ] ) ) )
          //   continue;
          // >>>>>>> 650de6f435c6034e60876d3e7809804a4c96a5f4
          if( my_seed[ adj_pxl ] == 1 ) {
            bool prop_allowed = false;
            size_t adj_pxl2;
            for( size_t adj_idx2 = 0; adj_idx2 < adj_size; ++adj_idx2 ) {
              if( adj_itr.AdjIdx2( adj_pxl, adj_idx2, adj_pxl2 ) ) {
                // <<<<<<< HEAD - COMMIT...
                if( seed_zone_img[ adj_pxl2 ] == src_seed_lbl ) {
                  for( size_t lbl_idx = 0; lbl_idx < lbl_adj_list[ src_seed_lbl ].size( ); ++lbl_idx ) {
                    if( seed_zone_img[ adj_pxl2 ] == static_cast< int >( lbl_adj_list[ src_seed_lbl ][ lbl_idx ] ) ) {
                      prop_allowed = true;
                      continue;
                    }
                  }
                }
              }
            } 
            if ( !prop_allowed )
              continue;
          }
          // ======= END - COMMIT...
//                 if ( ( seed_zone_img[ adj_pxl2 ] == src_seed_lbl )  
//                   || ( seed_zone_img[ adj_pxl2 ] == static_cast< int >( lbl_adj_list[ src_seed_lbl ][ 0 ] ) )
//                   || ( seed_zone_img[ adj_pxl2 ] == static_cast< int >( lbl_adj_list[ src_seed_lbl ][ 1 ] ) ) ){
//                   prop_allowed = true;
//                 }
//               }
//             }
//             if ( !prop_allowed )
//               continue;
//           }
//           */
// >>>>>>> 650de6f435c6034e60876d3e7809804a4c96a5f4
          int previous_value = value[ adj_pxl ];
          int prop_value = value[ src_pxl ] + ( my_grad[ src_pxl ] + my_grad[ adj_pxl ] ) / 2;
          if( previous_value > prop_value ) { 
            value[ adj_pxl ] = prop_value;
            predecessor[ adj_pxl ] = src_pxl;
            if( my_seed[ adj_pxl ] == 0 )
              seed_brd[ adj_pxl ] = seed_img[ adj_pxl ];
            else
              seed_brd[ adj_pxl ] = seed_brd[ src_pxl ];
            queue.Update( adj_pxl, previous_value, prop_value );
          }
        }
      }
    }
    //<<<<<<< HEAD - COMMIT...
    COMMENT( "Painting final path and re-labelling pixels. ini_pxl: " << ini_pxl, 0 );
    for( src_pxl = end_pxl; src_pxl != ini_pxl; src_pxl = predecessor[ src_pxl ] ) {
      label[ 2 ][ src_pxl ] = 1;
      border[ src_pxl ] = anchor; // Re-labelling the pixel.
      if( src_pxl == 0 ){
        std::cout << "Error ocurred. " << std::endl;
        break;
      }
    }
    label[ 2 ][ ini_pxl ] = 1;
    mask = label[ 2 ];
    COMMENT(" LiveWire executed, label " << anchor << " was updated, from ini_pxl = " << ini_pxl << " to end_pxl = " << end_pxl << ".", 0 );
    Bial::Write( value, "/tmp/final_value.pgm" );
    Bial::Write( border, "/tmp/final_border_labels.pgm" );
    Bial::Write( label[ 2 ], "/tmp/final_label[2].pgm" );
    Bial::Write( predecessor, "/tmp/final_predecessor.pgm" );
    Bial::Write( seed_brd, "/tmp/final_seed_brd.pgm" );
    Bial::Write( my_seed, "/tmp/final_my_seed.pgm" );
    Bial::Write( seed_zone_img, "/tmp/final_seed_zone_img.pgm" );
    // ======= END - COMMIT...
//     COMMENT( "Painting final path.", 0 );
//     for( src_pxl = end_pxl; src_pxl != ini_pxl; src_pxl = predecessor[ src_pxl ] ) {
//       label[ 2 ][ src_pxl ] = 1;
//       border[ src_pxl ] = anchor; // Re-labelling the pixel
//     }
//     label[ 2 ][ ini_pxl ] = 1;
//     mask = label[ 2 ];
    
//     COMMENT(" LiveWire executed, label " << anchor << " was updated, from ini_pxl = " << ini_pxl << " to end_pxl = " << end_pxl << ".", 3);
// >>>>>>> 650de6f435c6034e60876d3e7809804a4c96a5f4
    return( seed_brd[ end_pxl ] );
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
