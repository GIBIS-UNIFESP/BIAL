#ifndef DEFAULTTOOL_H
#define DEFAULTTOOL_H

#include "tool.h"

enum class LabelType : char {
  none,
  solid,
  translucent,
  multilabel
};

class DefaultTool : public Tool {

public:
  DefaultTool( GuiImage *guiImage, ImageViewer *viewer );
  enum { Type = 0 };
  static const int supportedFormats = ( ( int ) Modality::BW2D | ( int ) Modality::BW3D | ( int ) Modality::RGB2D );

  /* Tool interface */
public:
  int type( );

  void mouseReleased( QPointF pt, Qt::MouseButtons buttons, size_t axis );
  void mouseClicked( QPointF pt, Qt::MouseButtons buttons, size_t axis );
  void mouseDragged( QPointF pt, Qt::MouseButtons buttons, size_t axis );
  void mouseMoved( QPointF pt, size_t axis );
  void sliceChanged( size_t axis, size_t slice );
  void updateOverlay( QPointF pt, size_t axis );
  bool addLabel( QString filename );
  void removeLabel( );
  QPixmap getLabel( size_t axis );

  float getFactor( ) const;
  void setTranslucentFactor( float m_factor );

  LabelType getLabelType( ) const;
  void setLabelType( const LabelType &value );

private:
  /**
   *
   * @brief changeOthersSlices is called when the mouse is clicked over
   *  an 3d image view (axial, sagittal and coronal).
   * @param posF is the mouse pointer position in the scene.
   * @param view is the number of the view's axis.
   *
   */
  void changeOtherSlices( QPointF posF, size_t view );

  Bial::Image< int > m_label;
  int m_max;
  std::array< QPixmap, 4 > pixmaps;
  std::array< bool, 4 > needUpdate;
  LabelType labelType;
  float m_factor;
};

#endif /* DEFAULTTOOL_H */
