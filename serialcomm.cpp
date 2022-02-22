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

    //qDebug() << _depthLimit;
    //qDebug() << _stateSwitchTol;

    //Write query
    QString query =
            QString::number(_msgTx.at(_currState).pumpPower) +
            ";" +
            QString::number(_msgTx.at(_currState).fanSpeed)+
            "\n";

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
    _port->write(query.toLocal8Bit());

    //Read and save Response
    _msgRx_long = _port->readLine();

    QString _msg = QString(_msgRx_long);

    QStringList _list = _msg.split(QLatin1Char(';'));

    for (uint8_t i = 0 ; i < _list.size(); ++i)
    {

        if(i == 0)
        {
            _depth = (_list.at(i).toInt() + _depthOffset);
        } else if (i == 1)
        {
            QString _tempflow = _list.at(i);
            _tempflow.chop(2);
            _flow = (_tempflow.toInt() + _flowOffset);
        } else
        {
            qDebug() << _list.at(i);
        }
    }

    //Switch states
    if (_dtTimeout)
    {
        qDebug() << _depthThen << " | " << _depth;

        if (((_depthThen - _depth) > (-_stateSwitchTol)) && ((_depthThen - _depth) < _stateSwitchTol))
        {
            qDebug() << "Switch";
            _currState++;

            if (_currState >= _msgTx.length())
            {
                _port->flush();
                _currState = 0;
                qDebug() << "Test Ended";
                _testEnded = 1;
                _fileHeaderSet = 0;
                emit TestEnded();
            }
        }
        else if (_currState == 0)
        {
            ++_depthFailCount;
        }

        //Set new ref. depth
        _depthThen = _depth;
        _dtTimeout = false;
    }

    // TODO: convert to something usesnful like meters or hamburgers per bald eagle
    //Done for everything except for Flow

    //Send Data to MainWindow
    _data.time =  QTime::currentTime().toString("hh:mm:ss");
    _data.state = _currState;
    _data.pumplvl = (QString::number(_msgTx.at(_currState).pumpPower).toDouble() * 0.09765625); //PumpPower in %
    _data.depth = (_depth * 0.05859375); //Depth in meter
    _data.flow = _flow; //convert this
    _data.fanspeed = (QString::number(_msgTx.at(_currState).fanSpeed).toDouble() * 0.09765625); //Fanspeed in %
    if (!_fileHeaderSet && !_testEnded)
    {
        _data.message = "Test Started";
    }
    else if (_testEnded)
    {
        _data.message = "Test Ended";

    }
    else
    {
        _data.message = nullptr;
    }
    emit DataReady();



    //Create & write Data to file
    _file.setFileName(filename);

    if (_file.open(QIODevice::ReadWrite | QIODevice::Append))
    {
        QTextStream stream(&_file);
        if (!_fileHeaderSet && !_testEnded)
        {
            stream << "Time of Measurement" << ';'
                   << "Current State" << ';' << "PumpLvl in %"
                   << ';' << "Depth in m" << ';' << "Flow in x * l/min"
                   << ';' << "Fanspeed in %" << '\n';
            _fileHeaderSet = 1;
        }
        else
        {
        stream  << QString(_data.time) << ';'
                << QString::number(_data.state) << ';' << QString::number(_data.pumplvl, 'g', 3) << ';'
                << QString::number(_data.depth, 'g', 3) << ';' << QString::number(_data.flow, 'g', 3) << ';'
                << QString::number(_data.fanspeed, 'g', 3) << '\n';
        }
        if (_testEnded)
        {
            stream << "Test Ended" << '\n';
            _testEnded = 0;
        }
    }
    else
    {
        qDebug() << "file open Error";
    }
    _file.close();

    //depth Limit reached
    if (_depth <= _depthLimit && _currState != 0)
    {
        _errcode = SerComm::Error::MIN_DEPTH;
        //Set to endstate
        _currState	= 4;
        qDebug() << "Depth Limit";
    }

    //Failed to achieve equilibrium
    if (_depthFailCount >= 15)
    {
        _errcode = SerComm::Error::DEPTH_FAIL;
        //End Test due to insuffiecent Starting condition
        emit TestEnded();
    }



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

    _iterationTimer->setInterval(_measureRate);
    _iterationTimer->moveToThread(_thread);


    _depthTimer->setInterval(_deltaRate); //old MINUTES(15)
    _depthTimer->moveToThread(_thread);

    connect(_worker, SIGNAL(DataReady()), this, SLOT(SaveCopy()));

    connect(_worker, SIGNAL(TestEnded()), _thread, SLOT(quit()));

    connect(_thread, SIGNAL(started()), _iterationTimer, SLOT(start()));
    connect(_thread, SIGNAL(finished()), _iterationTimer, SLOT(stop()));

    connect(_thread, SIGNAL(started()), _depthTimer, SLOT(start()));
    connect(_thread, SIGNAL(finished()), _depthTimer, SLOT(stop()));

    connect(_thread, SIGNAL(started()), _depthTimer, SLOT(start()));
    connect(_thread, SIGNAL(finished()), _depthTimer, SLOT(stop()));

    connect(_iterationTimer, &QTimer::timeout, _worker, &SerialCommWorker::Iteration, Qt::DirectConnection);
    connect(_depthTimer, &QTimer::timeout, _worker, &SerialCommWorker::DT_Timeout, Qt::DirectConnection);

    _worker->Port()->setBaudRate(QSerialPort::Baud115200); //changed Baud from 9600 to 115200
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

void SerialComm::SaveCopy()
{
    _data = _worker->GetData();
    emit DataReady();
}

void SerialComm::SaveSettings(Settings_t settings)
{
    _worker->setDepthLimit(settings.depthLimit);
    _worker->setStateSwitchTol(settings.stateSwitchTol);
    _worker->setDepthOffset(settings.depthOffset);
    _worker->setFlowOffset(settings.flowOffset);


    _measureRate = settings.measureRate;
    qDebug() << _measureRate;
    _iterationTimer->setInterval(_measureRate);

    _deltaRate = settings.deltaRate;
    qDebug() << _deltaRate;
    _depthTimer->setInterval(_deltaRate);

}
