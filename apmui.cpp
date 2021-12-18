#include "apmui.h"
#include "ui_apmui.h"

APMui::APMui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::APMui)
{
    ui->setupUi(this);

    connect(getdata, &QState::entered, this, &APMui::gettingData);
    connect(pAC, &QState::entered, this, &APMui::prepAndCheck);
    connect(this, &APMui::accepted, this, &APMui::stop);
    connect(this, &APMui::rejected, this, &APMui::stop);

    getdata->assignProperty(ui->stateLabel, "text", "Getting Data");
    pAC->assignProperty(ui->stateLabel, "text", "Preparing and Checking");


    getdata->addTransition(timer, &QTimer::timeout, pAC);
    pAC->addTransition(timer2, &QTimer::timeout, getdata);

    machine->addState(getdata);
    machine->addState(pAC);

    machine->setInitialState(getdata);
}

APMui::~APMui()
{
    delete ui;
    delete getdata;
    delete pAC;
    delete timer;
    delete timer2;
    delete machine;
}

void APMui::startup()
{
    machine->start();
    qDebug() << "Starting up";
}

void APMui::usbData(const QByteArray &data)
{
    ui->pte_Data->insertPlainText(data);
}

void APMui::gettingData()
{
    //Data Query
    qDebug() << "<P:0;T;D;L:0>";   //Set Pumplvl. 0; get Depth Data; get Throughput Data; Set Fan
    emit SendData("<P:0;T;D;L:0>\n");
    //Populate Plaintextedit
    timer->setSingleShot(true);
    timer->start(250);
}

void APMui::prepAndCheck()
{

    c = ui->pte_Data->textCursor();
    c.movePosition(QTextCursor::End);
    ui->pte_Data->setTextCursor(c);


    timer2->setSingleShot(true);
    timer2->start(1000);
}

void APMui::stop()
{
    machine->stop();
    qDebug() << "Stoped";
}
