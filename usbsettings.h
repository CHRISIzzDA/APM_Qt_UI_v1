#ifndef USBSETTINGS_H
#define USBSETTINGS_H

#include <QDialog>
#include <QSerialPortInfo>
#include <QTimer>

namespace Ui {
class UsbSettings;
}

class UsbSettings : public QDialog
{
    Q_OBJECT

public:
    explicit UsbSettings(QWidget *parent = nullptr);
    ~UsbSettings();

    QSerialPortInfo SelectedPort();

private slots:
    void set_pte_usbdata(const QByteArray &data);

    void timerEvent(QTimerEvent* evt);

    void on_pb_status_led_on_clicked();

    void on_pb_status_led_off_clicked();

    void on_comboBox_activated(int index);

signals:
    void SendData(const char* data);
    void SetPort(QString text);

private:
    Ui::UsbSettings *ui;
    QList<QSerialPortInfo> serialPorts;
};



#endif // USBSETTINGS_H
