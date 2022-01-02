#include "serialcomm.h"

SerialComm::SerialComm(QObject *parent) : QObject(parent)
{
    connect(_timer, &QTimer::timeout, this, &SerialComm::Test);
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
    _timer->stop();
    if (_port->isOpen()) _port->close();
}

void SerialComm::Test()
{
    while(running)
    {

    }
    // while senden (abhängig vom state) bis es passt

    // lesen

    // signal DataReady1

    // speichern in file
}
