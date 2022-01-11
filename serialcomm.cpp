#include "serialcomm.h"

SerialCommWorker::SerialCommWorker(QObject* parent) : QObject(parent)
{

}

SerialCommWorker::~SerialCommWorker()
{
    delete _port;
}

void SerialCommWorker::Iteration()
{
    //qDebug() << "Iteration";

    if (_dtTimeout)
    {
        qDebug() << "dt Timeout";
        if (true) // depth then - depth now >= 5 or something
        {
            _currState++;

            if (_currState >= _msgTx.length())
            {
                _errcode = SerComm::Error::NONE;
                _currState = 0;
                _port->flush();
                emit TestEnded();
                return;
            }
        }
        else if (_currState == 0)
        {
            ++_depthFailCount;
        }

        _dtTimeout = false;
    }

    QString query =
            QString::number(_msgTx.at(_currState).val1) +
            ";" +
            QString::number(_msgTx.at(_currState).val2)+
            "\n";
    /*
    QString query =
            QString::number(H_BYTE(_msgTx.at(_currState).val1)) +
            QString::number(L_BYTE(_msgTx.at(_currState).val1)) +
            QString::number(H_BYTE(_msgTx.at(_currState).val2)) +
            QString::number(L_BYTE(_msgTx.at(_currState).val2));

    */

    if (!_port->isOpen())
    {
        _port->open(QIODevice::ReadWrite);

        if (!_port->isOpen())
        {
            qDebug() << "failed to open port";
            _errcode = SerComm::Error::PORT_OPEN_FAIL;
            emit TestEnded();
            return;
        }
    }

    //_port->write(query.toLocal8Bit().data());
    _port->write(query.toLocal8Bit());
    _msgRx_long = _port->readLine();
    //qDebug() << _msgRx_long;

    QString _msg = QString(_msgRx_long);

    QStringList _list = _msg.split(QLatin1Char(';'));

    for (uint8_t i = 0 ; i < _list.size(); ++i)
    {
        //qDebug() << _list.at(i);
        if(i == 0)
        {
            _depth = _list.at(i).toInt();
        } else if (i == 1)
        {
            QString _tempflow = _list.at(i);
            _tempflow.chop(2);
            _flow = _tempflow.toInt();
        } else
        {
            qDebug() << _list.at(i);
        }

    }
    qDebug() << "The depth is:" << _depth << "The flow is:" << _flow;

    //_port->read(_msgRx.a, 4);

    // TODO: convert to something useful like meters or hamburgers per bald eagle

    //int depth = TO_16_BIT(_msgRx.s.depthH, _msgRx.s.depthL);
    //int flow  = TO_16_BIT(_msgRx.s.flowH, _msgRx.s.flowL);

    // TODO: File stuff and data ready

    //qDebug() << depth << " " << flow;
    //return;
/*
    if (depth <= _depthLimit)
    {
        _errcode = SerComm::Error::MIN_DEPTH;
        emit TestEnded();
        return;
    }

    if (_depthFailCount >= 15)
    {
        _errcode = SerComm::Error::DEPTH_FAIL;
        emit TestEnded();
        return;
    }
*/


    return;
}

void SerialCommWorker::DT_Timeout()
{
    //qDebug() << "timeout";
    _dtTimeout = true;
}

// ---------------------------------------------------------------------------------

SerialComm::SerialComm(QObject* parent) : QObject(parent)
{
    _worker->moveToThread(_thread);
    _worker->Port()->moveToThread(_thread);


    _iterationTimer->setInterval(MEASURE_RATE);
    _iterationTimer->moveToThread(_thread);

    //removed singelshot
    _depthTimer->setInterval(DELTA_RATE); //old MINUTES(15)
    _depthTimer->moveToThread(_thread);

    _thread->connect(_worker, SIGNAL(TestEnded()), SLOT(quit()));

    _iterationTimer->connect(_thread, SIGNAL(started()), SLOT(start()));
    _iterationTimer->connect(_thread, SIGNAL(finished()), SLOT(stop()));

    _depthTimer->connect(_thread, SIGNAL(started()), SLOT(start()));
    _depthTimer->connect(_thread, SIGNAL(finished()), SLOT(stop()));

    _worker->connect(_iterationTimer, SIGNAL(timeout()), SLOT(Iteration()), Qt::DirectConnection);
    _worker->connect(_depthTimer, SIGNAL(timeout()), SLOT(DT_Timeout()), Qt::DirectConnection);

    _worker->Port()->setBaudRate(QSerialPort::Baud9600);
    _worker->Port()->setDataBits(QSerialPort::Data8);
    _worker->Port()->setStopBits(QSerialPort::OneStop);
    _worker->Port()->setParity(QSerialPort::NoParity);
    _worker->Port()->setFlowControl(QSerialPort::NoFlowControl);
}

SerialComm::~SerialComm()
{
    delete _thread;
    delete _worker;
    delete _iterationTimer;
    delete _depthTimer;
}

void SerialComm::StartTest(QString portName)
{
    _worker->Port()->setPortName(portName);
    qDebug() << "start";
    _thread->start();
}

void SerialComm::StopTest()
{
    _worker->Port()->close();
    qDebug() << "stop";
    _thread->quit();
}
