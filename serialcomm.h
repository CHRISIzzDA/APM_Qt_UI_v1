#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>
#include <QElapsedTimer>

#define TO_16_BIT(vH, vL) (((vH) << 8) + (vL))

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

    QSerialPort& port() { return *_port; }
    QVector<TestData_t>& data() { return _dataVec; }

    enum ErrorCodes { ERR_NONE, ERR_MIN_DEPTH, ERR_DEPTH_FAIL };

signals:
    void DataReady();
    void TestDone();

public slots:
    void StartTest();
    void StopTest();

private slots:
    void Test();
    void Init();

private:
    QSerialPort* _port = new QSerialPort();
    QVector<TestData_t> _dataVec;

    QElapsedTimer* _tickTimer = new QElapsedTimer();
    QElapsedTimer* _measuringTimer = new QElapsedTimer();
    QElapsedTimer* _depthTimer = new QElapsedTimer();

    QList<int> _pumpPower = { 0, 100, 500, 1000, 1024 };
    int _timeout = 15; // min
    int _samplerate = 15; //min
    int _currState = 0;
    int _depthlimit = 10;
    int _errcode = ERR_NONE;
    int _depthfailcount = 0;
};

#endif // SERIALCOMM_H
