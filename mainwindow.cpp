#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QString>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QDesktopServices>
#include <QMessageBox>

#include "hdlc.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    logFilePath = "log.csv";
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_rtc_settime_btn_clicked()
{
    // Send string containing time data in following format: 'DD:MM:YYYY hh:mm:ss'
    QString datetime = ui->rtc_datetimeedit->dateTime().toString("dd/MM/yyyy hh:mm:ss");
    qDebug() << datetime;
    serial.writeDataEncodeHDLC(55,55,datetime.toLocal8Bit());
}

void MainWindow::status_update()
{
    // Read data from serial port
    QString serialstr = "00/00/0000,00:00:00,1024.0,25.5,50.7,143";
    QStringList serialdat = serialstr.split(",");
    // Pressure in Pascal
    float pressure = serialdat.at(2).toFloat();
    // Temperature in Celsius
    float temperature = serialdat.at(3).toFloat();
    // Humidity (relative)
    float humidity = serialdat.at(4).toFloat();
    // Air Quality Index
    int aqi = serialdat.at(5).toInt();

    // Data logging (before any conversion)
    if (ui->status_log_check->isChecked()) {
        QFile logfile(logFilePath);
        // If the file was successfully opened in append mode
        if(logfile.open(QIODevice::Append)) {
            // if the file is empty, initialize it with a CSV header
            if(logfile.size() == 0) {
                logfile.write("Day Month Year,Hours Minutes Seconds,Pressure [Pa],Temperature [C],Relative Humidity [%],AQI\n");
            }
            // Append the log to the file
        logfile.write(serialstr.append("\n").toLocal8Bit());
        // Close the file since there is no point keeping it open for >= 30s
        logfile.close();
        qDebug() << "Data written to log file";
        } else {
            qDebug() << "Unable to open log file";
        }
    }

    // Temperature conversion
    // Fahrenheit
    if (ui->status_fahrenheit_radio->isChecked()) {temperature = (temperature*(9.0/5.0))+32.0;}
    // Kelvin
    if (ui->status_kelvin_radio->isChecked()) {temperature += 273.15;}

    // Pressure conversion
    // PSI
    if (ui->status_psi_radio->isChecked()) {pressure /= 6894.8;}
    // BAR
    if (ui->status_bar_radio->isChecked()) {pressure /= 100000.0;}
    // Torr
    if (ui->status_torr_radio->isChecked()) {pressure /= 133.3;}
    // ATM
    if (ui->status_atm_radio->isChecked()) {pressure /= 101325.0;}

    //Printing values
    // Formatting
    QString pressurestr = "Pressure: ";
    QString tempstr = "Temperature: ";
    QString humstr = "Relative Humidity: ";
    QString aqistr = "Air Quality Index: ";
    pressurestr.append(QString::number(pressure));
    // Pressure units
    if (ui->status_pascal_radio->isChecked()) {pressurestr.append(" Pa");}
    if (ui->status_psi_radio->isChecked()) {pressurestr.append(" PSI");}
    if (ui->status_bar_radio->isChecked()) {pressurestr.append(" BAR");}
    if (ui->status_torr_radio->isChecked()) {pressurestr.append(" mmHg");}
    if (ui->status_atm_radio->isChecked()) {pressurestr.append(" ATM");}
    tempstr.append(QString::number(temperature));
    // Temperature units
    if (ui->status_celsius_radio->isChecked()) {tempstr.append(" °C");}
    if (ui->status_fahrenheit_radio->isChecked()) {tempstr.append(" °F");}
    if (ui->status_kelvin_radio->isChecked()) {tempstr.append(" K");}
    humstr.append(QString::number(humidity));
    // Humidity units
    humstr.append(" %");
    aqistr.append(QString::number(aqi));
    // AQI formatting according to indexes and tooltips found in BME680 datasheet
    if (aqi>0) {ui->status_aqi_bg_btn->setStyleSheet("background-color:rgb(0,255,0); color:rgb(0,0,0)");ui->status_aqi_bg_btn->setText("AQI: Good");}
    if (aqi>50) {ui->status_aqi_bg_btn->setStyleSheet("background-color:rgb(255,255,0); color:rgb(0,0,0)");ui->status_aqi_bg_btn->setText("AQI: Average");}
    if (aqi>100) {ui->status_aqi_bg_btn->setStyleSheet("background-color:rgb(255,165,0); color:rgb(0,0,0)");ui->status_aqi_bg_btn->setText("AQI: Little Bad");}
    if (aqi>150) {ui->status_aqi_bg_btn->setStyleSheet("background-color:rgb(255,0,0); color:rgb(0,0,0)");ui->status_aqi_bg_btn->setText("AQI: Bad");}
    if (aqi>200) {ui->status_aqi_bg_btn->setStyleSheet("background-color:rgb(255,0,255); color:rgb(255,255,255)");ui->status_aqi_bg_btn->setText("AQI: Worse");}
    if (aqi>300) {ui->status_aqi_bg_btn->setStyleSheet("background-color:rgb(0,0,0); color:rgb(255,255,255)");ui->status_aqi_bg_btn->setText("AQI: Very Bad");}
    // Print values
    ui->status_pressure_label->setText(pressurestr);
    ui->status_temperature_label->setText(tempstr);
    ui->status_humidity_label->setText(humstr);
    ui->status_aqi_label->setText(aqistr);
}


void MainWindow::on_status_forceupdate_btn_clicked()
{
    // Forcefully request data from slave
    status_update();
}


void MainWindow::on_status_setpath_btn_clicked()
{
    // Open a QFileDialog to select the path of the log file
    QFileDialog* dialog = new QFileDialog();
    dialog->setFileMode(QFileDialog::Directory);
    logFilePath = dialog->getExistingDirectory(0, "Select log file destination", QDir::currentPath());
    logFilePath.append("/log.csv");
    qDebug() << logFilePath;
    delete dialog;
}


void MainWindow::on_status_openfile_btn_clicked()
{
    // Open the CSV file with the default editor (presumably Excel or similar)
    QDesktopServices::openUrl(QUrl("file:" + logFilePath));
}


void MainWindow::on_status_clearlog_btn_clicked()
{
    QMessageBox *msg = new QMessageBox("Delete log file", "Confirm deletition of log file?", QMessageBox::Warning, QMessageBox::Yes, QMessageBox::No, 0);
    if(QMessageBox::Accepted == msg->exec())
    {
        QFile::remove(logFilePath);
    }
    delete msg;
}

