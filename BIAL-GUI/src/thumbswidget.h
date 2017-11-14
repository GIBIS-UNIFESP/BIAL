#ifndef THUMBSWIDGET_H
#define THUMBSWIDGET_H

#include "thumbnail.hpp"

#include <QWidget>

namespace Ui {
  class ThumbsWidget;
}

class Controller;

class ThumbsWidget : public QWidget {
  Q_OBJECT

public:
  explicit ThumbsWidget( Controller *controller, QWidget *parent = 0 );
  ~ThumbsWidget( );

  void clear( );

  void removeAt( int pos );

  void addThumbnail( GuiImage *image );

public slots:
  void currentImageChanged( );

private:
  Ui::ThumbsWidget *ui;
  QVector< Thumbnail* > thumbs;
  Controller *controller;
};

#endif /* THUMBSWIDGET_H */
