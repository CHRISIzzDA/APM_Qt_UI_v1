#include "apmui.h"
#include "ui_apmui.h"

apmui::apmui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::apmui)
{
    ui->setupUi(this);
    startTimer(1000);
    s1->assignProperty(ui->stateLabel, "text", "State One");
    s2->assignProperty(ui->stateLabel, "text", "State Two");
    //s3->assignProperty(ui->stateLabel, "text", "Pressing...");

    timer->start(1000);
    timer2->start(1000);


    QSignalTransition *timedout = s1->addTransition(timer, &QTimer::timeout, s2);

    QSignalTransition *timeout2 = s2->addTransition(timer2, &QTimer::timeout, s1);

/*
    QEventTransition *enterTransistion = new QEventTransition(ui->pb_test, QEvent::Enter);
    enterTransistion->setTargetState(s2);
    s1->addTransition(enterTransistion);

    QEventTransition *leaveTransition = new QEventTransition(ui->pb_test, QEvent::Leave);
    leaveTransition->setTargetState(s1);
    s2->addTransition(leaveTransition);

    QEventTransition *pressTransition = new QEventTransition(ui->pb_test, QEvent::MouseButtonPress);
    pressTransition->setTargetState(s3);
    s2->addTransition(pressTransition);

    QEventTransition *releaseTransition = new QEventTransition(ui->pb_test, QEvent::MouseButtonRelease);
    releaseTransition->setTargetState(s2);
    s3->addTransition(releaseTransition);
*/
    machine->addState(s1);
    machine->addState(s2);
    //machine->addState(s3);

    machine->setInitialState(s1);
    machine->start();

}

apmui::~apmui()
{
    delete ui;

}

void apmui::on_pb_test_clicked()
{

}
