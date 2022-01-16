#include "mainwindow.h"
#include "ui_mainwindow.h"

#define usbtimeout 10000

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //connect(this, &MainWindow::recievedData, usbSettings, &UsbSettings::set_pte_usbdata);
    connect(usbSettings, &UsbSettings::SetPort, this, &MainWindow::setPort);
    connect(_thread, SIGNAL(DataReady()), this, SLOT(DisplayData()));
}

MainWindow::~MainWindow()
{
    delete usbSettings;
    delete ui;
}

void MainWindow::setPort(QString usbtext)
{
    portname = usbtext;
}

void MainWindow::DisplayData()
{
    MeasurementData_t* data = _thread->GetData();
    ui->pte_Data->insertPlainText("Current State: " + QString::number(data->state) +
                                  " | Pumplvl: " + QString::number(data->pumplvl) +
                                  " | Depth: " + QString::number(data->flow) +
                                  " | Flow: " + QString::number(data->flow) +
                                  " | Fanspeed: " + QString::number(data->fanspeed)+
                                  "\t\n");
}

void MainWindow::on_pb_settings_clicked()
{
    ui->sw_main->setCurrentIndex(MSW_SETTINGS);
}

void MainWindow::on_pb_back_settings_clicked()
{
    ui->sw_main->setCurrentIndex(MSW_HOME);
}

void MainWindow::on_pb_start_clicked()
{
    ui->sw_main->setCurrentIndex(MSW_TEST);
}

void MainWindow::on_pb_back_start_clicked()
{
    ui->sw_main->setCurrentIndex(MSW_HOME);
}

void MainWindow::on_pb_files_clicked()
{
    ui->sw_main->setCurrentIndex(MSW_FILES);
}

void MainWindow::on_pb_back_files_clicked()
{
    ui->sw_main->setCurrentIndex(MSW_HOME);
}

void MainWindow::on_pb_usb_settings_clicked()
{
    usbSettings->setVisible(true);
}

void MainWindow::on_pushButton_clicked()
{
    ui->sw_Test->setCurrentIndex(TSW_RUNNING);
    _thread->StartTest(portname);
}

void MainWindow::on_btn_StopTest_clicked()
{
    ui->sw_Test->setCurrentIndex(MSW_HOME);
    _thread->StopTest();
}
