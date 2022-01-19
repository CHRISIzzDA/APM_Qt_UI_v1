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
                                  " | Pumplvl: " + QString::number(data->pumplvl, 'f', 2) + "%" +
                                  " | Depth: " + QString::number(data->depth, 'f', 2) + "m" +
                                  " | Flow: " + QString::number(data->flow) +
                                  " | Fanspeed: " + QString::number(data->fanspeed, 'f', 2) + "%" +
                                  "\t\n");
    ui->pte_Data->ensureCursorVisible();

    switch (QString::number(data->state).toInt()) {
    case 0:
        ui->stateLabel->setText("Prepping...");
        break;
    case 1:
        ui->stateLabel->setText("Pumping at lvl.1");
        break;
    case 2:
        ui->stateLabel->setText("Pumping at lvl.2");
        break;
    case 3:
        ui->stateLabel->setText("Pumping at lvl.3");
        break;
    case 4:
        ui->stateLabel->setText("Wait till Full");
        break;
    default:
        ui->stateLabel->setText("No current State");
        break;
    }
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
    ui->pte_Data->clear();
    _thread->StopTest();
}
