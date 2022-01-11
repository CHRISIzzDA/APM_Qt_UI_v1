
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

// 8-/16-Byte conversion

#define TO_16_BIT(vH, vL) (((vH) << 8) + (vL))

#define H_BYTE(val) ((val) >> 8)
#define L_BYTE(val)  ((val) & 0xFF)

// msec to time conversion

#define MINUTES(msecs) ((msecs) * 60000)

// Structs/Union for Message handling

typedef struct
{
    qint8 depthH;
    qint8 depthL;
    qint8 flowH;
    qint8 flowL;
} MsgRx_s;

typedef union
{
    MsgRx_s s;
    char a[4];
} MsgRx_t;

typedef struct
{
    qint16 val1;
    qint16 val2;
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

signals:
    void DataReady();
    void TestEnded();

private slots:
    void Iteration();
    void DT_Timeout();

private:
    QSerialPort* _port = new QSerialPort();

    MsgRx_t _msgRx;
    QList<Pair_16_t> _msgTx  = { {0, 0}, {100, 100}, {500, 500}, {1000, 1000}, {1024, 1024} };
    QByteArray _msgRx_long;

    int _currState = 0;
    int _depthLimit = 0;
    int _depthFailCount = 0;
    int _depth;
    int _flow;

    bool _dtTimeout;

    SerComm::Error _errcode = SerComm::Error::NONE;
};

// ---------------------------------------------------------------------------------

class SerialComm : public QObject
{
    Q_OBJECT

public:
    explicit SerialComm(QObject* parent = Q_NULLPTR);
    ~SerialComm();

    void StartTest(QString portName);
    void StopTest();

private:
    QThread* _thread = new QThread();
    SerialCommWorker* _worker = new SerialCommWorker();

    QTimer* _iterationTimer = new QTimer();
    QTimer* _depthTimer = new QTimer();
};

#endif // SERIALCOMM_H
