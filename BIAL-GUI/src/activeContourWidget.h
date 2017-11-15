#ifndef LIVEWIREWIDGET_H
#define LIVEWIREWIDGET_H

#include "activeContourTool.h"
#include "robotuser.h"
#include <QWidget>

namespace Ui {
  class LiveWireWidget;
}

class ImageViewer;

class ActiveContourWidget : public QWidget {
  Q_OBJECT
  Controller * m_controller;
public:
  explicit ActiveContourWidget( ImageViewer *viewer, QWidget *parent = 0 );
  ~ActiveContourWidget( );
  void setTool( Tool *sTool );

  Controller* controller( ) const;
  void setController( Controller *controller );

private slots:

  void on_pushButtonClear_clicked( );

  void on_pushButtonRobotUser_clicked( );

  void on_pushButtonSave_clicked( );

  void on_pushButtonProcessAll_clicked( );

  void on_pushButtonClassifier_clicked( );

  void on_pushButtonTest1_clicked();

  void on_pushButtonTest2_clicked();

private:
  Ui::LiveWireWidget *ui;
  ActiveContourTool *m_tool;

  CvSVM* m_SVM;

  ImageViewer *m_viewer;

};

#endif /* LIVEWIREWIDGET_H */
