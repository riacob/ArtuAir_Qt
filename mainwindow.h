#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>

#include "serialtransreceiver.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    SerialTransreceiver serial;

private slots:
    void on_rtc_settime_btn_clicked();

    void on_status_forceupdate_btn_clicked();

    void on_status_setpath_btn_clicked();


    void on_status_openfile_btn_clicked();

    void on_status_clearlog_btn_clicked();

private:
    Ui::MainWindow *ui;
    void status_update();
    QString logFilePath;
};
#endif // MAINWINDOW_H
