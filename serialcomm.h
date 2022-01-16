
#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#include <QDebug>
#include <QThread>

#include <QObject>
#include <QSerialPort>
#include <QTimer>
#include <QElapsedTimer>

// measuring rate

#define MEASURE_RATE 1000

// Rate of depth delta comparison

#define DELTA_RATE 3000

#define MINUTES(msecs) ((msecs) * 60000)

// Structs/Union for Message handling

typedef struct
{
    qint32 state;
    qint32 pumplvl;
    qint32 depth;
    qint32 flow;
    qint32 fanspeed;
} MeasurementData_t;

typedef struct
{
    qint16 pumpPower;
    qint16 fanSpeed;
} Pair_16_t;

namespace SerComm
{
    enum class Error { NONE, PORT_OPEN_FAIL, MIN_DEPTH, DEPTH_FAIL };
};

class SerialCommWorker : public QObject
{
    Q_OBJECT

public:
    explicit SerialCommWorker(QObject* parent = Q_NULLPTR);
    ~SerialCommWorker();

    QSerialPort* Port() { return _port; }
    MeasurementData_t* GetData() { return &_data; }

signals:
    void DataReady();
    void TestEnded();

public slots:
    void Iteration();
    void DT_Timeout();

private:
    QSerialPort* _port = new QSerialPort();

    QList<Pair_16_t> _msgTx  = { {0, 0}, {100, 100}, {500, 500}, {1000, 1000}, {1024, 1024} };
    QByteArray _msgRx_long;
    MeasurementData_t _data;

    qint32 _currState = 0;
    qint32 _depthLimit = 0;
    qint32 _depthFailCount = 0;
    qint32 _depth;
    qint32 _flow;

    bool _dtTimeout;

    SerComm::Error _errcode = SerComm::Error::NONE;
};

// ---------------------------------------------------------------------------------

class SerialComm : public QObject
{
    Q_OBJECT

signals:
    void DataReady();


private slots:
    void SaveCopy();


public:
    explicit SerialComm(QObject* parent = Q_NULLPTR);
    ~SerialComm();

    MeasurementData_t* GetData() { return _data; }

    void StartTest(QString portName);
    void StopTest();

private:
    QThread* _thread = new QThread();
    SerialCommWorker* _worker = new SerialCommWorker();
    MeasurementData_t* _data;

    QTimer* _iterationTimer = new QTimer();
    QTimer* _depthTimer = new QTimer();
};

#endif // SERIALCOMM_H
