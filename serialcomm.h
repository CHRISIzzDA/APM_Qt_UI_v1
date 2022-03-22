
#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#include <QDebug>
#include <QThread>

#include <QObject>
#include <QSerialPort>
#include <QTimer>
#include <QElapsedTimer>
#include <QList>
#include <QStringList>
#include <QDir>
#include <QFile>
#include <QDateTime>

// measuring rate

#define MEASURE_RATE 1000

// Rate of depth delta comparison

#define DELTA_RATE 3000

#define MINUTES_TO_MS(min) ((min) * 60000)


typedef struct
{
    QString time;
    qint32 state;
    double pumplvl;
    double depth;
    double flow;
    double fanspeed;
    QString message;
} MeasurementData_t;

typedef struct
{
    qint16 pumpPower;
    qint16 fanSpeed;
} Pair_16_t;

typedef struct
{
    qint32 measureRate;
    qint32 deltaRate;
    qint32 depthLimit;
    qint32 stateSwitchTol;
    qint32 depthOffset;
    qint32 flowOffset;
} Settings_t;

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

    void setDepthLimit(qint32 depthLimit) { _depthLimit = depthLimit; }
    void setStateSwitchTol(qint32 stateSwitchTol) { _stateSwitchTol = stateSwitchTol; }
    void setDepthOffset(qint32 depthOffset) { _depthOffset = depthOffset; }
    void setFlowOffset(qint32 flowOffset) { _flowOffset = flowOffset; }

signals:
    void DataReady();
    void TestEnded();

public slots:
    void Iteration();
    void DT_Timeout();

private:
    QSerialPort* _port = new QSerialPort();

    QList<Pair_16_t> _msgTx  = { {0, 0}, {100, 100}, {500, 500}, {900, 900}, {0, 1024} };
    QByteArray _msgRx_long;
    MeasurementData_t _data;
    Settings_t _settings;

    qint32 _currState = 0;
    qint32 _depthLimit = 256;       //256 Equates to ~15m
    qint32 _stateSwitchTol = 17;    //17 Equates to ~1m
    qint32 _depthFailCount = 0;
    qint32 _depthOffset = 0;
    qint32 _flowOffset = 0;

    qint32 _depth = 0;
    qint32 _flow = 0;
    qint32 _depthThen = -1024;

    QString filename = "C:/Users/chris/OneDrive - HTL Wien 3 Rennweg/Schule/Schule_5BM/APM/Data_out/PumpTest.csv";
    QFile _file;


    bool _fileHeaderSet = 0;
    bool _dtTimeout;
    bool _testEnded = 0;


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

public slots:
    void SaveSettings(Settings_t settings);

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

    qint32 _measureRate = 1000;
    qint32 _deltaRate = 3000;

    QTimer* _iterationTimer = new QTimer();
    QTimer* _depthTimer = new QTimer();

};

#endif // SERIALCOMM_H
