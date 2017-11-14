#include "functionalwidget.h"
#include "ui_functionalwidget.h"

FunctionalWidget::FunctionalWidget( QWidget *parent ) : QWidget( parent ), ui( new Ui::FunctionalWidget ) {
  ui->setupUi( this );
}

FunctionalWidget::~FunctionalWidget( ) {
  delete ui;
}

void FunctionalWidget::on_pushButtonInvertColors_clicked( ) {
  emit invertColors( );
}

void FunctionalWidget::on_pushButtonInvertColors_2_clicked( ) {
  emit brightness( );
}
