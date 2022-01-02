#include "mainwindow.h"
#include "ui_mainwindow.h"

#define usbtimeout 10000

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(usbSettings, &UsbSettings::SendData, this, &MainWindow::writeData);
    connect(usbSettings, &UsbSettings::SetPort, this, &MainWindow::SetPort);
    connect(port, &QSerialPort::readyRead, this, &MainWindow::readData);
    connect(this, &MainWindow::recievedData, usbSettings, &UsbSettings::set_pte_usbdata);

    getdata->assignProperty(ui->stateLabel, "text", "Getting Data");
    pAC->assignProperty(ui->stateLabel, "text", "Preparing and Checking");


    getdata->addTransition(timer, &QTimer::timeout, pAC);
    pAC->addTransition(timer2, &QTimer::timeout, getdata);

    machine->addState(getdata);
    machine->addState(pAC);

    machine->setInitialState(getdata);
}


MainWindow::~MainWindow()
{
    if (port->isOpen()) port->close();

    delete port;
    delete usbSettings;

    delete machine;
    delete getdata;
    delete pAC;

    delete timer;
    delete timer2;

    delete ui;
}

void MainWindow::writeData(const QByteArray &data)
{
    port->write(data);
    //qDebug() << data;

}

void MainWindow::readData()
{
    const QByteArray data = port->readAll();
    emit recievedData(data);
    //qDebug() << data;
}

void MainWindow::SetPort(QString usbtext)
{
    qDebug() << usbtext;

   port->setPortName(usbtext);
   port->setBaudRate(QSerialPort::Baud9600);
   port->setDataBits(QSerialPort::Data8);
   port->setStopBits(QSerialPort::OneStop);
   port->setParity(QSerialPort::NoParity);
   port->setFlowControl(QSerialPort::NoFlowControl);


   qDebug() << "Port Set to " + usbtext;
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
    machine->start();
    qDebug() << "Starting up";
}


void MainWindow::on_btn_StopTest_clicked()
{
    machine->stop();
    qDebug() << "Stoped";
}
