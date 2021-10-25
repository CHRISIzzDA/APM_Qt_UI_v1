#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort>
#include <QTimer>
#include <QDebug>

#include "usbsettings.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void ExchangeData(const char* ch);

    void SetPort(QString usbtext);

    void on_pb_settings_clicked();

    void on_pb_back_settings_clicked();

    void on_pb_start_clicked();

    void on_pb_back_start_clicked();

    void on_pb_files_clicked();

    void on_pb_back_files_clicked();

    void on_pb_usb_settings_clicked();

private:
    Ui::MainWindow *ui;
    UsbSettings* usbSettings = Q_NULLPTR;
    QSerialPort* port = Q_NULLPTR;
    QString buf;

};
#endif // MAINWINDOW_H