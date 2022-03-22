#include "mainwindow.h"
#include "ui_mainwindow.h"

#define usbtimeout 10000

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //connect(this, &MainWindow::recievedData, usbSettings, &UsbSettings::set_pte_usbdata);
    connect(usbSettings, &UsbSettings::SetPort, this, &MainWindow::setPort);
    connect(_thread, SIGNAL(DataReady()), this, SLOT(DisplayData()));
    connect(this, &MainWindow::SettingsReady, _thread, &SerialComm::SaveSettings);
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
    if (data->message == nullptr)
    {
        ui->pte_Data->insertPlainText("Time: " + QString(data->time) +
                                      " | Current State: " + QString::number(data->state) +
                                      " | Pumplvl: " + QString::number(data->pumplvl, 'f', 2) + "%" +
                                      " | Depth: " + QString::number(data->depth, 'f', 2) + "m" +
                                      " | Flow: " + QString::number(data->flow) +
                                      " | Fanspeed: " + QString::number(data->fanspeed, 'f', 2) + "%" +
                                      "\t\n");

    }
    else
    {
        ui->pte_Data->insertPlainText(data->message + "\t\n");
    }
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
    ui->sw_Test->setCurrentIndex(TSW_RUNNING);
    _thread->StartTest(portname);
}

void MainWindow::on_pb_back_start_clicked()
{
    ui->sw_main->setCurrentIndex(MSW_HOME);
}

void MainWindow::on_pb_files_clicked()
{
    //ui->sw_main->setCurrentIndex(MSW_FILES);

    const QString srcPath = QFileDialog::getOpenFileName(this, "Source file",
    "C:/Users/chris/OneDrive - HTL Wien 3 Rennweg/Schule/Schule_5BM/APM/Data_out", //Windows: C:/Users/chris/OneDrive - HTL Wien 3 Rennweg/Schule/Schule_5BM/APM/Data_out  //Raspi:/home/pi/Documents/Data_out/Pumptest.csv
    "All files (*.*)");
    if (srcPath.isNull()) // QFileDialog dialogs return null if user canceled
    {
        qDebug() << "User cancelled";
    }

    const QString dstPath = QFileDialog::getSaveFileName(this, "Destination file",
    "C:/Users/chris/OneDrive - HTL Wien 3 Rennweg/Schule/Schule_5BM/APM/temp/PumpTest.csv", //Windows: C:/Users/chris/OneDrive - HTL Wien 3 Rennweg/Schule/Schule_5BM/APM/temp/PumpTest.csv Raspi: /media/pi/PumpTest.csv
    "All files (*.*)"); // it asks the user for overwriting existing files
    if (dstPath.isNull())
    {
        qDebug() << "User cancelled";
    }

    if (QFile::exists(dstPath))
    {
        if (!QFile::remove(dstPath))
        {
            qDebug() << "couldn't delete File";
        }
    }

    QFile::copy(srcPath, dstPath);

}

void MainWindow::on_pb_back_files_clicked()
{
    ui->sw_main->setCurrentIndex(MSW_HOME);
}

void MainWindow::on_pb_usb_settings_clicked()
{
    usbSettings->setVisible(true);
}

void MainWindow::on_pb_startTest_clicked()
{
    ui->sw_Test->setCurrentIndex(TSW_RUNNING);
    _thread->StartTest(portname);
}

void MainWindow::on_btn_StopTest_clicked()
{
    ui->sw_Test->setCurrentIndex(MSW_HOME);
    ui->sw_main->setCurrentIndex(MSW_HOME);
    ui->pte_Data->clear();
    _thread->StopTest();
}

void MainWindow::on_pb_file_copy_clicked()
{
    const QString srcPath = QFileDialog::getOpenFileName(this, "Source file",
    "C:/Users/chris/OneDrive - HTL Wien 3 Rennweg/Schule/Schule_5BM/APM/Data_out", //Windows: C:/Users/chris/OneDrive - HTL Wien 3 Rennweg/Schule/Schule_5BM/APM/Data_out  //Raspi:/home/pi/Documents/Data_out/Pumptest.csv
    "All files (*.*)");
    if (srcPath.isNull()) // QFileDialog dialogs return null if user canceled
    {
        qDebug() << "User cancelled";
    }

    const QString dstPath = QFileDialog::getSaveFileName(this, "Destination file",
    "C:/Users/chris/OneDrive - HTL Wien 3 Rennweg/Schule/Schule_5BM/APM/temp/PumpTest.csv", //Windows: C:/Users/chris/OneDrive - HTL Wien 3 Rennweg/Schule/Schule_5BM/APM/temp/PumpTest.csv Raspi: /media/pi/PumpTest.csv
    "All files (*.*)"); // it asks the user for overwriting existing files
    if (dstPath.isNull())
    {
        qDebug() << "User cancelled";
    }

    if (QFile::exists(dstPath))
    {
        if (!QFile::remove(dstPath))
        {
            qDebug() << "couldn't delete File";
        }
    }

    QFile::copy(srcPath, dstPath);
}


void MainWindow::on_sb_measureRate_valueChanged(int arg1)
{
    _settings.measureRate = arg1;
}


void MainWindow::on_sb_deltaRate_valueChanged(int arg1)
{
    _settings.deltaRate = arg1;
}


void MainWindow::on_sb_stateSwitchTol_valueChanged(int arg1)
{
    _settings.stateSwitchTol = arg1;
}


void MainWindow::on_sb_depthLimit_valueChanged(int arg1)
{
    _settings.depthLimit = arg1;
}


void MainWindow::on_sb_depthOffset_valueChanged(int arg1)
{
    _settings.depthOffset = arg1;
}


void MainWindow::on_sb_flowOffset_valueChanged(int arg1)
{
    _settings.flowOffset = arg1;
}


void MainWindow::on_pb_saveSettings_clicked()
{
    emit SettingsReady(_settings);
    ui->sw_main->setCurrentIndex(MSW_HOME);
}

