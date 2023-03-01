#include "graphicreadouts.h"
#include "ui_graphicreadouts.h"

GraphicReadouts::GraphicReadouts(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphicReadouts)
{
    ui->setupUi(this);
}

GraphicReadouts::~GraphicReadouts()
{
    delete ui;
}
