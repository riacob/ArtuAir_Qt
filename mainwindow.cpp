#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QString>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QDesktopServices>
#include <QMessageBox>

#include "hdlc.h"
#include "loginprompt.h"
#include "graphicreadouts.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    logFilePath = "log.csv";
    graphicReadouts = new GraphicReadouts();
}

MainWindow::~MainWindow()
{
    delete graphicReadouts;
    delete ui;
}

// TODO
void MainWindow::on_cfg_settime_btn_clicked()
{
    // TODO
    // Send string containing time data in following format: 'DD:MM:YYYY hh:mm:ss'
    QString datetime = ui->cfg_datetimeedit->dateTime().toString("dd/MM/yyyy hh:mm:ss");
}

// TODO
void MainWindow::status_update()
{
    // TODO
    // Read data from serial port
    QString serialstr = "00/00/0000,00:00:00,1024.0,25.5,50.7,143";
    // Split the received string into useful data
    QStringList serialdat = serialstr.split(",");
    // Pressure in Pascal
    float pressure = serialdat.at(2).toFloat();
    graphicReadouts->pressure = pressure;
    // Temperature in Celsius
    float temperature = serialdat.at(3).toFloat();
    graphicReadouts->temperature = temperature;
    // Humidity (relative)
    float humidity = serialdat.at(4).toFloat();
    graphicReadouts->humidity = humidity;
    // Air Quality Index
    int aqi = serialdat.at(5).toInt();
    graphicReadouts->aqi = aqi;

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
    QString pressurestr = "Pressure: ";
    QString tempstr = "Temperature: ";
    QString humstr = "Relative Humidity: ";
    QString aqistr = "Air Quality Index: ";
    QString datestr = "Date: ";
    QString timestr = "Time: ";

    // Pressure and relative units formatting
    pressurestr.append(QString::number(pressure));
    if (ui->status_pascal_radio->isChecked()) {pressurestr.append(" Pa");}
    if (ui->status_psi_radio->isChecked()) {pressurestr.append(" PSI");}
    if (ui->status_bar_radio->isChecked()) {pressurestr.append(" BAR");}
    if (ui->status_torr_radio->isChecked()) {pressurestr.append(" mmHg");}
    if (ui->status_atm_radio->isChecked()) {pressurestr.append(" ATM");}

    // Temperature and relative units formatting
    tempstr.append(QString::number(temperature));
    if (ui->status_celsius_radio->isChecked()) {tempstr.append(" °C");}
    if (ui->status_fahrenheit_radio->isChecked()) {tempstr.append(" °F");}
    if (ui->status_kelvin_radio->isChecked()) {tempstr.append(" K");}

    // Humidity formatting
    humstr.append(QString::number(humidity));
    humstr.append(" %");
    aqistr.append(QString::number(aqi));

    // Air quality index formatting
    if (aqi>0) {ui->status_aqi_bg_btn->setStyleSheet("background-color:rgb(0,255,0); color:rgb(0,0,0)");ui->status_aqi_bg_btn->setText("AQI: Good");}
    if (aqi>50) {ui->status_aqi_bg_btn->setStyleSheet("background-color:rgb(255,255,0); color:rgb(0,0,0)");ui->status_aqi_bg_btn->setText("AQI: Average");}
    if (aqi>100) {ui->status_aqi_bg_btn->setStyleSheet("background-color:rgb(255,165,0); color:rgb(0,0,0)");ui->status_aqi_bg_btn->setText("AQI: Little Bad");}
    if (aqi>150) {ui->status_aqi_bg_btn->setStyleSheet("background-color:rgb(255,0,0); color:rgb(0,0,0)");ui->status_aqi_bg_btn->setText("AQI: Bad");}
    if (aqi>200) {ui->status_aqi_bg_btn->setStyleSheet("background-color:rgb(255,0,255); color:rgb(255,255,255)");ui->status_aqi_bg_btn->setText("AQI: Worse");}
    if (aqi>300) {ui->status_aqi_bg_btn->setStyleSheet("background-color:rgb(0,0,0); color:rgb(255,255,255)");ui->status_aqi_bg_btn->setText("AQI: Very Bad");}

    // Date and time formatting
    datestr.append(serialdat.at(0));
    timestr.append(serialdat.at(1));

    // Print formatted values
    ui->status_pressure_label->setText(pressurestr);
    ui->status_temperature_label->setText(tempstr);
    ui->status_humidity_label->setText(humstr);
    ui->status_aqi_label->setText(aqistr);
    ui->status_date_label->setText(datestr);
    ui->status_time_label->setText(timestr);
}

// DONE
void MainWindow::on_status_forceupdate_btn_clicked()
{
    // Forcefully request data from slave
    status_update();
}

// DONE
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

// DONE
void MainWindow::on_status_openfile_btn_clicked()
{
    // Open the CSV file with the default editor (presumably Excel or similar)
    if(!QDesktopServices::openUrl(QUrl("file:" + logFilePath))) {
        QMessageBox *msg = new QMessageBox("Error", "Log file does not exist.\nHas the data been read yet?", QMessageBox::Critical, QMessageBox::Ok,0,0);
        msg->exec();
        delete msg;
    }
}

// DONE
void MainWindow::on_status_clearlog_btn_clicked()
{
    QMessageBox *msg = new QMessageBox("Delete log file", "Confirm deletition of log file?", QMessageBox::Warning, QMessageBox::Yes, QMessageBox::No, 0);
    // If accepted delete log file
    if (msg->exec() == QMessageBox::Yes) {
        QFile file(logFilePath);
        file.remove();
    }
    delete msg;
}

// TODO
void MainWindow::on_admin_btn_clicked()
{
    // TODO
    // Open dialog to request passcode from user
    LoginPrompt* login = new LoginPrompt();
    login->exec();
    // Copy the password and NOT the pointer since the memory pointed to by the pointer would be destroyed at the destruction of the dialog window
    for (int i = 0; i < 8; i++) {
        password[i] = login->password[i];
    }
    delete login;
    // Send passcode to uC
    // uC replies with boolean true if the code is valid and false if it is not
    // setEnabled of QGroupBox should be set to 1 if the code is valid and should be set at 0 if the code is invalid
    // Note: the "unlock" bit is stored on the uC, so users can't forcefully send serial commands without logging in first
    ui->cfg_groupbox->setEnabled(1);
}

// TODO
void MainWindow::on_cfg_setlocaltime_btn_clicked()
{
    // Set the time and date of the QDateTimeEdit to the current date and time
    ui->cfg_datetimeedit->setTime(QTime::currentTime());
    ui->cfg_datetimeedit->setDate(QDate::currentDate());
    // TODO
    // Transmit the current date and time to the uC
    QString datetime = ui->cfg_datetimeedit->dateTime().toString("dd/MM/yyyy hh:mm:ss");
}

// TODO
void MainWindow::on_cfg_logout_btn_clicked()
{
    // TODO
    // Ask user logout confirmation using a QMessageBox
    QMessageBox *msg = new QMessageBox("Logout", "Do you wish to log out of Administrator mode?", QMessageBox::Warning, QMessageBox::Yes, QMessageBox::No, 0);
    // If accepted proceed
    // If cancelled abort
    msg->exec();
    delete msg;
    // Send logout cmd to uC
    // uC replies with ack
    // setEnabled of QGroupBox should be set to 0 if the cmd was acknowledged, or to 1 otherwise
    ui->cfg_groupbox->setEnabled(0);
}

// DONE
void MainWindow::on_cfg_always_check_stateChanged(int arg1)
{
    // If "always" is checked, disable the option of setting a time interval
    ui->cfg_timeedit_start->setEnabled(!arg1);
    ui->cfg_timeedit_stop->setEnabled(!arg1);
}

// DONE
void MainWindow::on_status_graphics_btn_clicked()
{
    // Open the graphics view
    graphicReadouts->exec();
}

