#ifndef LABELSWIDGET_H
#define LABELSWIDGET_H

#include <QWidget>

#include "defaulttool.h"

namespace Ui {
  class LabelsWidget;
}

class LabelsWidget : public QWidget {
  Q_OBJECT

public:
  explicit LabelsWidget( QWidget *parent = 0 );
  ~LabelsWidget( );
  void setTool(Tool *sTool );

private slots:
  void on_labelSpinBox_editingFinished( );

  void on_labelHorizontalSlider_actionTriggered( int action );

  void on_solid_clicked( );

  void on_translucent_clicked( );

  void on_translucentSlider_actionTriggered( int action );

  void on_multiLabel_clicked( );

private:
  Ui::LabelsWidget *ui;
  DefaultTool *tool;
};

#endif /* LABELSWIDGET_H */
