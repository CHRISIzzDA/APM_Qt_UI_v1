#include "mainwindow.h"
#include "ui_mainwindow.h"

#define usbtimeout 10000

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    port = new QSerialPort();
    ui->setupUi(this);

    usbSettings = new UsbSettings();
    apmUi = new apmui();

    connect(usbSettings, &UsbSettings::SendData, this, &MainWindow::writeData);
    connect(usbSettings, &UsbSettings::SetPort, this, &MainWindow::SetPort);
    connect(port, &QSerialPort::readyRead, this, &MainWindow::readData);
    connect(this, &MainWindow::recievedData, usbSettings, &UsbSettings::set_pte_usbdata);
    connect(this, &MainWindow::startStateMachine, apmUi, &apmui::startup);
    connect(apmUi, &apmui::SendData, this, &MainWindow::writeData);
    connect(this, &MainWindow::recievedData, apmUi, &apmui::usbData);
}


MainWindow::~MainWindow()
{
    if (port->isOpen()) port->close();

    delete ui;
    delete usbSettings;
    delete port;
    delete apmUi;
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

   port->open(QIODevice::ReadWrite);

   qDebug() << "Port Set to " + usbtext;
}
void MainWindow::on_pb_settings_clicked()
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+1);
}


void MainWindow::on_pb_back_settings_clicked()
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()-1);
}


void MainWindow::on_pb_start_clicked()
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+2);
}


void MainWindow::on_pb_back_start_clicked()
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()-2);
}


void MainWindow::on_pb_files_clicked()
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+3);
}


void MainWindow::on_pb_back_files_clicked()
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()-3);
}


void MainWindow::on_pb_usb_settings_clicked()
{
    usbSettings->setVisible(true);

}

void MainWindow::on_pushButton_clicked()
{
    apmUi->setVisible(true);
    emit startStateMachine();
}

