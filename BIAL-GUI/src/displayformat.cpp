#include "Common.hpp"
#include "displayformat.h"

#include <QDebug>
#include <QSettings>

BW2DFormat::BW2DFormat( QObject *parent ) : DisplayFormat( parent ) {
  m_modality = Modality::BW2D;
  m_currentLayout = Layout::GRID;
  m_currentViews = Views::SHOW0;
  defaultViews = Views::SHOW0;
  m_numberOfViews = 1;
  m_hasViewerControls = false;
  m_enableTools = true;
  m_rotateAll = false;
  m_rotateSingle = true;
  m_showNiftiViews = false;
  m_showNiftiAxis = false;
  m_showOrientation = false;
  m_showPpmViews = false;
  m_showPpmChannels = false;
  m_hasOverlay = false;
  m_hasLayout = false;
  m_has3Views = false;
  m_has4Views = false;
  m_overlay = false;
  m_maximumNumberOfViews = 1;
  loadSettings( );
}

BW3DFormat::BW3DFormat( QObject *parent ) : DisplayFormat( parent ) {
  m_modality = Modality::BW3D;
  m_currentLayout = Layout::GRID;
  m_currentViews = Views::SHOW012;
  defaultViews = Views::SHOW0;
  m_numberOfViews = 3;
  m_hasViewerControls = true;
  m_enableTools = true;
  m_rotateAll = true;
  m_rotateSingle = false;
  m_showNiftiViews = true;
  m_showNiftiAxis = false;
  m_showOrientation = true;
  m_showPpmViews = false;
  m_showPpmChannels = false;
  m_hasOverlay = true;
  m_hasLayout = true;
  m_has3Views = true;
  m_has4Views = false;
  m_overlay = false;
  m_maximumNumberOfViews = 3;
  loadSettings( );
}

RGB2DFormat::RGB2DFormat( QObject *parent ) : DisplayFormat( parent ) {
  m_modality = Modality::RGB2D;
  m_currentLayout = Layout::GRID;
  m_currentViews = Views::SHOW0;
  defaultViews = Views::SHOW0;
  m_numberOfViews = 1;
  m_hasViewerControls = false;
  m_enableTools = false;
  m_rotateAll = true;
  m_rotateSingle = false;
  m_showNiftiViews = false;
  m_showNiftiAxis = false;
  m_showPpmViews = true;
  m_showOrientation = false;
  m_showPpmChannels = true;
  m_hasOverlay = true;
  m_hasLayout = false;
  m_has3Views = false;
  m_has4Views = true;
  m_overlay = false;
  m_maximumNumberOfViews = 4;
  loadSettings( );
}

DisplayFormat::DisplayFormat( QObject *parent ) : QObject( parent ) {
  m_overlayColor = Qt::green;
}

DisplayFormat::~DisplayFormat( ) {
  QSettings settings;
  settings.beginGroup( "DisplayFormat" );
  settings.beginGroup( QString( "Type%1" ).arg( ( int ) modality( ) ) );
  settings.setValue( "numberOfViews", m_numberOfViews );
  settings.setValue( "currentLayout", ( int ) m_currentLayout );
  settings.setValue( "currentViews", ( int ) m_currentViews );
  settings.setValue( "defaultViews", ( int ) defaultViews );
}

void DisplayFormat::loadSettings( ) {
  QSettings settings;
  settings.beginGroup( "DisplayFormat" );
  settings.beginGroup( QString( "Type%1" ).arg( ( int ) modality( ) ) );
  if( settings.contains( "numberOfViews" ) ) {
    try {
      setNumberOfViews( settings.value( "numberOfViews" ).toInt( ) );
    }
    catch( std::invalid_argument &e ) {
      return;
    }
  }
  if( settings.contains( "currentLayout" ) ) {
    try {
      setCurrentLayout( ( Layout ) settings.value( "currentLayout" ).toInt( ) );
    }
    catch( std::invalid_argument &e ) {
      return;
    }
  }
  if( settings.contains( "currentViews" ) ) {
    try {
      setCurrentViews( ( Views ) settings.value( "currentViews" ).toInt( ) );
    }
    catch( std::invalid_argument &e ) {
      return;
    }
  }
  if( settings.contains( "defaultViews" ) ) {
    defaultViews = ( Views ) settings.value( "defaultViews" ).toInt( );
  }
}
size_t DisplayFormat::getMaximumNumberOfViews( ) const {
  return( m_maximumNumberOfViews );
}

Modality DisplayFormat::modality( ) const {
  return( m_modality );
}
Layout DisplayFormat::currentLayout( ) const {
  return( m_currentLayout );
}
Views DisplayFormat::currentViews( ) const {
  return( m_currentViews );
}
bool DisplayFormat::hasViewerControls( ) const {
  return( m_hasViewerControls );
}
bool DisplayFormat::enableTools( ) const {
  return( m_enableTools );
}
bool DisplayFormat::rotateAll( ) const {
  return( m_rotateAll );
}
bool DisplayFormat::rotateSingle( ) const {
  return( m_rotateSingle );
}
bool DisplayFormat::hasOverlay( ) const {
  return( m_hasOverlay );
}
bool DisplayFormat::showNiftiViews( ) const {
  return( m_showNiftiViews );
}
bool DisplayFormat::showNiftiAxis( ) const {
  return( m_showNiftiAxis );
}
bool DisplayFormat::showOrientation( ) const {
  return( m_showOrientation );
}
bool DisplayFormat::showPpmViews( ) const {
  return( m_showPpmViews );
}
bool DisplayFormat::showPpmChannels( ) const {
  return( m_showPpmChannels );
}
bool DisplayFormat::hasLayout( ) const {
  return( m_hasLayout );
}
bool DisplayFormat::has3Views( ) const {
  return( m_has3Views );
}
bool DisplayFormat::has4Views( ) const {
  return( m_has4Views );
}

std::array< bool, 4 > DisplayFormat::getViews( ) {
  std::array< bool, 4 > views;

  views[ 0 ] = ( int ) currentViews( ) & ( int ) Views::SHOW0;
  views[ 1 ] = ( int ) currentViews( ) & ( int ) Views::SHOW1;
  views[ 2 ] = ( int ) currentViews( ) & ( int ) Views::SHOW2;
  views[ 3 ] = ( int ) currentViews( ) & ( int ) Views::SHOW3;

  return( views );
}
int DisplayFormat::getNumberOfViews( ) const {
  return( m_numberOfViews );
}

QColor DisplayFormat::overlayColor( ) const {
  return( m_overlayColor );
}

void DisplayFormat::setOverlayColor( const QColor &overlayColor ) {
  m_overlayColor = overlayColor;
  emit updated( );
}

bool DisplayFormat::overlay( ) const {
  return( m_overlay );
}

void DisplayFormat::setCurrentLayout( const Layout &currentLayout ) {
  COMMENT( "Layout set to " << ( int ) currentLayout, 0 );
  m_currentLayout = currentLayout;
  emit updated( );
}

void DisplayFormat::setCurrentViews( const Views &currentViews ) {
  COMMENT( "currentViews set to " << ( int ) currentViews, 0 );
  m_currentViews = currentViews;
  if( currentViews <= Views::SHOW3 ) {
    defaultViews = currentViews;
  }
  emit updated( );
}

void DisplayFormat::setOverlay( bool overlay ) {
  COMMENT( "Overlay set to " << overlay, 0 );
  if( m_hasOverlay ) {
    m_overlay = overlay;
    emit updated( );
  }
}

void DisplayFormat::toggleOverlay( ) {
  if(hasOverlay()){
    setOverlay( !overlay( ) );
  }
}

void BW2DFormat::setNumberOfViews( int numberOfViews ) {
  if( numberOfViews != 1 ) {
    throw std::invalid_argument( "The number of views cannot be changed!" );
  }
}

void BW3DFormat::setNumberOfViews( int numberOfViews ) {
  COMMENT( "Number of views set to " << numberOfViews << ".", 0 );
  if( ( numberOfViews != 1 ) && ( numberOfViews != 3 ) ) {
    throw std::invalid_argument( "Invalid number of views!" );
  }
  if( numberOfViews == 1 ) {
    m_currentViews = defaultViews;
    m_showNiftiAxis = true;
    m_showOrientation = false;
  }
  else {
    m_currentViews = Views::SHOW012;
    m_showNiftiAxis = false;
    m_showOrientation = true;
  }
  m_numberOfViews = numberOfViews;
  emit updated( );
}

void RGB2DFormat::setNumberOfViews( int numberOfViews ) {
  if( ( numberOfViews != 1 ) && ( numberOfViews != 4 ) ) {
    throw std::invalid_argument( "Invalid number of views!" );
  }
  if( numberOfViews == 1 ) {
    m_currentViews = defaultViews;
    m_showPpmChannels = true;
    m_showOrientation = false;
  }
  else {
    m_currentViews = Views::SHOW0123;
    m_showPpmChannels = false;
    m_showOrientation = true;
  }
  m_numberOfViews = numberOfViews;
  emit updated( );
}
