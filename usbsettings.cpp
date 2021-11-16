#include "usbsettings.h"
#include "ui_usbsettings.h"


UsbSettings::UsbSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UsbSettings)
{

    ui->setupUi(this);
    startTimer(1000);    
}


UsbSettings::~UsbSettings()
{
    delete ui;
}

QSerialPortInfo UsbSettings::SelectedPort()
{

    return serialPorts.at(ui->comboBox->currentIndex());
}

void UsbSettings::set_pte_usbdata(const QByteArray &data)
{

}

void UsbSettings::timerEvent(QTimerEvent *evt)
{
    serialPorts = QSerialPortInfo::availablePorts();
    QString last = ui->comboBox->currentText();
    ui->comboBox->clear();

    for (int i = 0; i < serialPorts.size(); i++)
    {
        ui->comboBox->addItem(serialPorts.at(i).portName() +
        " | " + serialPorts.at(i).description() +
        " | " + serialPorts.at(i).manufacturer());
    }
    ui->comboBox->setCurrentIndex(ui->comboBox->findText(last));
}




void UsbSettings::on_pb_status_led_on_clicked()
{
    emit SendData("on\n");
}


void UsbSettings::on_pb_status_led_off_clicked()
{
    emit SendData("off\n");
}

void UsbSettings::on_comboBox_activated(int index)
{
    emit SetPort(serialPorts.at(index).portName());
}

