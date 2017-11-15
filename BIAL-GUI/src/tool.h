#ifndef TOOL_H
#define TOOL_H

#include "guiimage.h"
#include "imageviewer.h"

#include <QPointF>
#include <QString>

class Tool : public QObject {
  Q_OBJECT
protected:
  GuiImage *guiImage;
  ImageViewer *viewer;
  bool m_visible;
  bool m_hasLabel = false;

public:
  Tool( GuiImage *guiImage, ImageViewer *viewer );

  virtual int type( ) = 0;
  virtual void mouseReleased( QPointF pt, Qt::MouseButtons buttons, size_t axis ) = 0;
  virtual void mouseClicked( QPointF pt, Qt::MouseButtons buttons, size_t axis ) = 0;
  virtual void mouseDragged( QPointF pt, Qt::MouseButtons buttons, size_t axis ) = 0;
  virtual void mouseMoved( QPointF pt, size_t axis ) = 0;
  virtual void sliceChanged( size_t axis, size_t slice ) = 0;
  virtual void enter( );
  virtual void leave( );
  void setVisible( bool value );
  virtual QPixmap getLabel( size_t axis );

  bool visible( ) const;
  bool hasLabel( ) const;

  GuiImage* getGuiImage( ) const;

  static Tool* buildTool( int toolType, GuiImage *img, ImageViewer *viewer );
  static Tool* setImageTool( int tool_type, GuiImage *img, ImageViewer *viewer );
protected:
  void setHasLabel( bool sHasLabel );
};

#endif /* TOOL_H */
