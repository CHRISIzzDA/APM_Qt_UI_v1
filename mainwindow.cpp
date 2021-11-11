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

    connect(usbSettings, &UsbSettings::SendData, this, &MainWindow::ExchangeData);
    connect(usbSettings, &UsbSettings::SetPort, this, &MainWindow::SetPort);
}

MainWindow::~MainWindow()
{
    if (port->isOpen()) port->close();

    delete ui;
    delete usbSettings;
    delete port;
}

void MainWindow::ExchangeData(const char* ch)
{
    //QTime start = QTime::currentTime(); //used for raspi UART read
    port->flush();
    port->write(ch);
    qDebug() << ch;

    //recieving answer
    /*
     * codeblock via
     * https://stackoverflow.com/questions/50481765/qt-serialport-data-reception
     */

    port->flush();

    char buffer[50];
    qDebug() << port->read(buffer, 50);
    qDebug() << buffer;
    /*for (;;) {
        numRead  = port->read(buffer, 50);

        qDebug() << buffer;
        // Do whatever with the array

        numReadTotal += numRead;
        if (numRead == 0 && !port->waitForReadyRead())
            break;
    }
        qDebug() << "done";
        qDebug() << buffer;
    */
    /*
    port->waitForBytesWritten();
    buf.clear();
    while(!buf.endsWith("\n"))
    {
        if(!port->waitForReadyRead(50))
            break;

        buf.append(port->readAll());
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);


       int elapsed = start.msecsTo(QTime::currentTime());
        qDebug() << elapsed;
        if(elapsed > usbtimeout)
        {
            buf.clear();
            port->clear();
            break;
        }
    }
    qDebug() << buf.trimmed();
    */
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


