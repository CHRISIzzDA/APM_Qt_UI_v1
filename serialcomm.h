#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>
#include <thread>

typedef struct
{
    int pumpPower;
    int depth;
    int flow;
    int fanSpeed;
} TestData_t;

class SerialComm : public QObject
{
    Q_OBJECT

public:
    explicit SerialComm(QObject *parent = nullptr);
    ~SerialComm();

    QSerialPort* port() { return _port; }
    QVector<TestData_t>* data() { return &_dataVec; }

signals:
    void DataReady();

public slots:
    void StartTest();
    void StopTest();

private slots:
    void Test();
    void Init();

private:
    QSerialPort* _port = new QSerialPort();
    QTimer* _timer = new QTimer();
    QVector<TestData_t> _dataVec;
};

#endif // SERIALCOMM_H
