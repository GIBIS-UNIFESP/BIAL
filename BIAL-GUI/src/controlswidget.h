#ifndef CONTROLSWIDGET_H
#define CONTROLSWIDGET_H

#include <QWidget>

namespace Ui {
  class ControlsWidget;
}

class ImageViewer;
class Controller;

class ControlsWidget : public QWidget {
  Q_OBJECT

public:
  explicit ControlsWidget( Controller *controller, QWidget *parent = 0 );
  ~ControlsWidget( );

private slots:
  void imageChanged( );
  void imageUpdated( );
  void updateRange( );

  void on_spinBoxSpeed_valueChanged( int arg1 );

  void on_buttonPlay_clicked( bool checked );

  void on_pushButton1View_clicked( );

  void on_pushButton3Views_clicked( );

  void on_pushButton4Views_clicked( );

  void on_pushButton_1RGB_clicked( );

  void on_pushButton_3RGB_clicked( );

  void on_pushButton_4RGB_clicked( );

  void on_pushButtonGrid_clicked( );

  void on_pushButtonHorizontal_clicked( );

  void on_pushButtonVertical_clicked( );

  void on_pushButtonAxial_clicked( );

  void on_pushButtonCoronal_clicked( );

  void on_pushButtonSagittal_clicked( );

  void on_pushButton_RGB_clicked( );

  void on_pushButton_R_clicked( );

  void on_pushButton_G_clicked( );

  void on_pushButton_B_clicked( );

  void on_pushButtonInterpolation_clicked( );

  void on_rotate_clicked( );

  void on_pushButtonHistogramNormalization_clicked( );

  void on_horizontalSliderContrast_valueChanged( int value );

  void on_horizontalSliderBrightness_valueChanged( int value );

  void on_resetContrast_clicked( );

  void on_resetBrightness_clicked( );

  void on_resetZoom_clicked( );

private:
  Ui::ControlsWidget *ui;
  Controller *controller;
  QTimer *timer;
};

#endif /* CONTROLSWIDGET_H */
