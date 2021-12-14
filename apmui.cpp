#include "apmui.h"
#include "ui_apmui.h"

apmui::apmui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::apmui)
{
    ui->setupUi(this);
    connect(s1, &QState::entered, this, &apmui::entereds1);
    connect(s2, &QState::entered, this, &apmui::entereds2);
    connect(this, &apmui::accepted, this, &apmui::stop);
    connect(this, &apmui::rejected, this, &apmui::stop);

    s1->assignProperty(ui->stateLabel, "text", "One");

    s2->assignProperty(ui->stateLabel, "text", "Two");


    QSignalTransition *timedout = s1->addTransition(timer, &QTimer::timeout, s2);
    QSignalTransition *timeout2 = s2->addTransition(timer2, &QTimer::timeout, s1);

    machine->addState(s1);
    machine->addState(s2);

    machine->setInitialState(s1);

}

apmui::~apmui()
{
    delete ui;
    delete s1;
    delete s2;
    delete timer;
    delete timer2;
    delete machine;
}

void apmui::startup()
{
    machine->start();
    qDebug() << "Starting up";
}

void apmui::entereds1()
{
    qDebug()<< "Entered State 1 with timer remaining" << timer->remainingTime();
    timer->setSingleShot(true);
    timer->start(1000);
}

void apmui::entereds2()
{
    qDebug()<< "Entered State 2 with timer2 remaining" << timer2->remainingTime();
    timer2->setSingleShot(true);
    timer2->start(1000);
}

void apmui::stop()
{
    machine->stop();
    qDebug() << "Stoped";
}
