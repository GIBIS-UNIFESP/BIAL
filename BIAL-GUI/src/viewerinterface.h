#ifndef VIEWERINTERFACE_H
#define VIEWERINTERFACE_H

class QColor;

class ViewerInterface {
public:
  virtual void setViewBgColor( const QColor &color ) = 0;
};

#endif /* VIEWERINTERFACE_H */
