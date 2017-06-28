#include "livewirewidget.h"
#include "ui_livewirewidget.h"

LiveWireWidget::LiveWireWidget( QWidget *parent ) :
  QWidget( parent ),
  ui( new Ui::LiveWireWidget ) {
  ui->setupUi( this );
}

LiveWireWidget::~LiveWireWidget( ) {
  delete ui;
}

void LiveWireWidget::setTool( Tool *sTool ) {
  tool = dynamic_cast< LiveWireTool* >( sTool );
  setEnabled( tool );

  ui->rbCostMap->setChecked( tool->getCostVisible( ) );
  ui->rbPredMap->setChecked( tool->getGradVisible( ) );
}

void LiveWireWidget::on_rbCostMap_clicked( ) {
  tool->setCostVisibility( ui->rbCostMap->isChecked( ) );
}

void LiveWireWidget::on_rbPredMap_clicked( ) {
  tool->setGradVisibility( ui->rbPredMap->isChecked( ) );
}
