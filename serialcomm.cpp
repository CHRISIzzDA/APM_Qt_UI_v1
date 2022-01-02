#include "serialcomm.h"

SerialComm::SerialComm(QObject *parent) : QObject(parent)
{
}

SerialComm::~SerialComm()
{

}


void SerialComm::StartTest()
{
    _port->open(QIODevice::ReadWrite);
    // file erstellen, header rein
    Test();
}

void SerialComm::StopTest()
{
    if (_port->isOpen()) _port->close();
}

void SerialComm::Test()
{
    char* response = NULL;
    response = (char*) malloc(4 * sizeof(char));

    _measuringTimer->start();
    while (true)
    {
        _tickTimer->start();
        _depthTimer->start();

        QByteArray query = QString::number(_pumpPower.at(_currState)).toLocal8Bit();
        _port->write(query);

        _port->read(response, 4);

        // TODO: convert to something useful like meters or hamburgers per bald eagles
        int depth = TO_16_BIT(response[0], response[1]);
        int flow  = TO_16_BIT(response[2], response[3]);

        // File stuff and data ready

        if (depth <= _depthlimit)
        {
            _errcode = ERR_MIN_DEPTH;
            break;
        }

        if (_depthfailcount >= 15)
        {
            _errcode = ERR_DEPTH_FAIL;
            break;
        }

        if (_depthTimer->elapsed() >= (_samplerate * 60000))
        {
            if (true) //depth then - depth now >= 5 or somthing
            {
                ++_currState;

                if (_currState >= _pumpPower.length())
                {
                    break;
                }

                _depthTimer->start();
            }

            else if (_currState == 0)
            {
                ++_depthfailcount;
            }
        }

        while (_tickTimer->elapsed() < 50);
    }

    free(response);
    emit TestDone();
}
