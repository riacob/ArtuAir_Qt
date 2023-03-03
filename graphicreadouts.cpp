#include "graphicreadouts.h"
#include "ui_graphicreadouts.h"

GraphicReadouts::GraphicReadouts(float temp, float press, float hum, int aqi, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphicReadouts)
{
    ui->setupUi(this);

    // Save data
    temperature = temp;
    pressure = press;
    humidity = hum;
    _aqi = aqi;

    // Air Quality index
    ui->aqi_lcd->setDecMode();
    ui->aqi_lcd->display(_aqi);
    if (aqi>0) {ui->aqi_lcd->setStyleSheet("background-color:rgb(0,255,0); color:rgb(0,0,0)");ui->aqi_lcd->display(aqi);}
    if (aqi>50) {ui->aqi_lcd->setStyleSheet("background-color:rgb(255,255,0); color:rgb(0,0,0)");ui->aqi_lcd->display(aqi);}
    if (aqi>100) {ui->aqi_lcd->setStyleSheet("background-color:rgb(255,165,0); color:rgb(0,0,0)");ui->aqi_lcd->display(aqi);}
    if (aqi>150) {ui->aqi_lcd->setStyleSheet("background-color:rgb(255,0,0); color:rgb(0,0,0)");ui->aqi_lcd->display(aqi);}
    if (aqi>200) {ui->aqi_lcd->setStyleSheet("background-color:rgb(255,0,255); color:rgb(255,255,255)");ui->aqi_lcd->display(aqi);}
    if (aqi>300) {ui->aqi_lcd->setStyleSheet("background-color:rgb(0,0,0); color:rgb(255,255,255)");ui->aqi_lcd->display(aqi);}
}

GraphicReadouts::~GraphicReadouts()
{
    delete ui;
}
