#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort>
#include <QTimer>
#include <QDebug>
#include <QFileDialog>

#include <usbsettings.h>
#include <serialcomm.h>



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

    void setPort(QString usbtext);

    void DisplayData();

    void on_pb_settings_clicked();

    void on_pb_back_settings_clicked();

    void on_pb_start_clicked();

    void on_pb_back_start_clicked();

    void on_pb_files_clicked();

    void on_pb_back_files_clicked();

    void on_pb_usb_settings_clicked();

    void on_pb_startTest_clicked();

    void on_btn_StopTest_clicked();

    void on_pb_file_copy_clicked();

    void on_sb_measureRate_valueChanged(int arg1);

    void on_sb_deltaRate_valueChanged(int arg1);

    void on_pb_saveSettings_clicked();

    void on_sb_stateSwitchTol_valueChanged(int arg1);

    void on_sb_depthLimit_valueChanged(int arg1);

    void on_sb_depthOffset_valueChanged(int arg1);

    void on_sb_flowOffset_valueChanged(int arg1);

signals:
    void SettingsReady(Settings_t settings);

private:
    Ui::MainWindow *ui;

    UsbSettings* usbSettings = new UsbSettings();
    SerialComm* _thread = new SerialComm();

    Settings_t _settings = {1000, 3000, 256, 17, 0, 0};

    QString portname = "COM3";

    enum MainStackedWidget { MSW_HOME, MSW_SETTINGS, MSW_TEST, MSW_FILES };
    enum TestStackedWidget { TSW_MENU, TSW_RUNNING };
};

#endif // MAINWINDOW_H
