#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>

#include "serialtransreceiver.h"
#include "graphicreadouts.h"

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

    void on_status_forceupdate_btn_clicked();

    void on_status_setpath_btn_clicked();


    void on_status_openfile_btn_clicked();

    void on_status_clearlog_btn_clicked();

    void on_cfg_settime_btn_clicked();

    void on_admin_btn_clicked();

    void on_cfg_setlocaltime_btn_clicked();

    void on_cfg_logout_btn_clicked();

    void on_cfg_always_check_stateChanged(int arg1);

    void on_status_graphics_btn_clicked();

private:
    Ui::MainWindow *ui;
    void status_update();
    QString logFilePath;
    bool password[8];
    GraphicReadouts *graphicReadouts;
};
#endif // MAINWINDOW_H
