#ifndef DISPLAYFORMAT_H
#define DISPLAYFORMAT_H

#include <QColor>
#include <QObject>
#include <QVector>
#include <array>

enum class Modality { BW2D = 1, RGB2D = 2, BW3D = 4 };
enum class Layout { GRID, HORIZONTAL, VERTICAL };
enum class Views { SHOW0 = 1, SHOW1 = 2, SHOW2 = 4, SHOW3 = 8, SHOW012 = 7, SHOW123 = 14, SHOW0123 = 15 };

class DisplayFormat : public QObject {
  Q_OBJECT
public:
  explicit DisplayFormat( QObject *parent );
  ~DisplayFormat( );

  Modality modality( ) const;

  Layout currentLayout( ) const;
  virtual void setCurrentLayout( const Layout &currentLayout );

  Views currentViews( ) const;
  virtual void setCurrentViews( const Views &currentViews );

  bool hasViewerControls( ) const;

  bool enableTools( ) const;

  bool rotateAll( ) const;

  bool rotateSingle( ) const;

  bool hasOverlay( ) const;

  bool showNiftiViews( ) const;

  bool showNiftiAxis( ) const;

  bool showOrientation( ) const;

  bool showPpmViews( ) const;

  bool showPpmChannels( ) const;

  bool overlay( ) const;

  void setOverlay( bool overlay );

  void toggleOverlay( );

  bool hasLayout( ) const;

  bool has3Views( ) const;

  bool has4Views( ) const;

  std::array< bool, 4 > getViews( );

  int getNumberOfViews( ) const;
  virtual void setNumberOfViews( int numberOfViews ) = 0;

  QColor overlayColor( ) const;
  void setOverlayColor( const QColor &overlayColor );

  size_t getMaximumNumberOfViews( ) const;

signals:
  void updated( );

protected:
  Modality m_modality;
  Layout m_currentLayout;
  Views m_currentViews;

  void loadSettings( );
  bool m_overlay;

  bool m_hasViewerControls;
  bool m_enableTools;
  bool m_rotateAll;
  bool m_rotateSingle;

  bool m_hasOverlay;
  bool m_hasLayout;
  bool m_has3Views;
  bool m_has4Views;

  bool m_showNiftiViews;
  bool m_showNiftiAxis;
  bool m_showOrientation;
  bool m_showPpmViews;
  bool m_showPpmChannels;

  Views defaultViews;
  int m_numberOfViews;
  size_t m_maximumNumberOfViews;
  QColor m_overlayColor;
};

class BW2DFormat : public DisplayFormat {
public:
  BW2DFormat( QObject *parent = 0 );
  void setNumberOfViews( int numberOfViews );
};

class BW3DFormat : public DisplayFormat {
public:
  BW3DFormat( QObject *parent = 0 );
  void setNumberOfViews( int numberOfViews );
};

class RGB2DFormat : public DisplayFormat {
public:
  RGB2DFormat( QObject *parent = 0 );
  void setNumberOfViews( int numberOfViews );
};

#endif /* DISPLAYFORMAT_H */
