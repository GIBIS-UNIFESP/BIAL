#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include "displayformat.h"
#include "graphicsscene.h"
#include "viewerinterface.h"
#include <QTime>
#include <QWidget>
#include <array>

class ImageWidget;
class QGridLayout;
class QGraphicsScene;
class Controller;

class ImageViewer : public QWidget, public ViewerInterface {
  Q_OBJECT
  std::array< ImageWidget*, 4 > views;
  QGridLayout *layout;
  Controller *m_controller;
  bool dragging;
  QTime timer;

  void getNewLayout( );

public:
  explicit ImageViewer( QWidget *parent = 0 );
  ~ImageViewer( );

  void setViewBgColor( const QColor &color );
  bool eventFilter( QObject *obj, QEvent *evt );
  GraphicsScene* getScene( size_t axis );
  void setController( Controller *value );
  Controller* controller( ) const;

signals:
  void mouseReleased( QPointF pt, Qt::MouseButtons buttons, size_t axis );
  void mouseClicked( QPointF pt, Qt::MouseButtons buttons, size_t axis );
  void mouseDragged( QPointF pt, Qt::MouseButtons buttons, size_t axis );
  void mouseMoved( QPointF pt, size_t axis );
  void dropImage( const QString &filePath );
  void dropFolder( const QString &folderPath );

private slots:
  void updateViews( );
  void changeImage( );

  void setLayoutType( Layout layout );
  void setViewMode( Views view );

  void setGridLayout( );
  void setHorizontalLayout( );
  void setVerticalLayout( );

  void hideViews( );
  void showViews( );

  void sliceChanged( size_t axis, size_t slice );

  /* QWidget interface */
protected:
  void resizeEvent( QResizeEvent* );
};

#endif /* IMAGEVIEWER_H */
