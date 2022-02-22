#ifndef USBSETTINGS_H
#define USBSETTINGS_H

#include <QDialog>
#include <QSerialPortInfo>
#include <QTextCursor>
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

public slots:

private slots:

    void timerEvent(QTimerEvent* evt);

    void on_comboBox_activated(int index);

signals:
    void SendData(const char* data);
    void SetPort(QString text);

private:
    Ui::UsbSettings *ui;
    QList<QSerialPortInfo> serialPorts;
    QTextCursor c;
};



#endif // USBSETTINGS_H
