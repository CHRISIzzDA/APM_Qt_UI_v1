#include "apmui.h"
#include "ui_apmui.h"

apmui::apmui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::apmui)
{
    ui->setupUi(this);
    connect(getdata, &QState::entered, this, &apmui::gettingData);
    connect(pAC, &QState::entered, this, &apmui::prepAndCheck);
    connect(this, &apmui::accepted, this, &apmui::stop);
    connect(this, &apmui::rejected, this, &apmui::stop);

    getdata->assignProperty(ui->stateLabel, "text", "Getting Data");

    pAC->assignProperty(ui->stateLabel, "text", "Preparing and Checking");


    QSignalTransition *timedout = getdata->addTransition(timer, &QTimer::timeout, pAC);
    QSignalTransition *timeout2 = pAC->addTransition(timer2, &QTimer::timeout, getdata);


    machine->addState(getdata);
    machine->addState(pAC);

    machine->setInitialState(getdata);
}

apmui::~apmui()
{
    delete ui;
    delete getdata;
    delete pAC;
    delete timer;
    delete timer2;
    delete machine;
}

void apmui::startup()
{
    machine->start();
    qDebug() << "Starting up";
}

void apmui::usbData(const QByteArray &data)
{
    ui->pte_Data->insertPlainText(data);
}

void apmui::gettingData()
{
    //Data Query
    qDebug() << "<P:0;T;D;L:0>";   //Set Pumplvl. 0; get Depth Data; get Throughput Data; Set Fan
    emit SendData("<P:0;T;D;L:0>\n");
    //Populate Plaintextedit
    timer->setSingleShot(true);
    timer->start(250);
}

void apmui::prepAndCheck()
{

    c = ui->pte_Data->textCursor();
    c.movePosition(QTextCursor::End);
    ui->pte_Data->setTextCursor(c);


    timer2->setSingleShot(true);
    timer2->start(1000);
}

void apmui::stop()
{
    machine->stop();
    qDebug() << "Stoped";
}
