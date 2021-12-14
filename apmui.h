#ifndef APMUI_H
#define APMUI_H

#include <QDialog>
#include <QTimer>
#include <QtDebug>
#include <QStateMachine>
#include <QEventTransition>
#include <QFinalState>


namespace Ui {
class apmui;
}

class apmui : public QDialog
{
    Q_OBJECT

public:
    explicit apmui(QWidget *parent = nullptr);
    ~apmui();

    QStateMachine *machine = new QStateMachine(this);
    QState *s1 = new QState();
    QState *s2 = new QState();
    //QState *s3 = new QState();

    QTimer *timer = new QTimer();
    QTimer *timer2 = new QTimer();



public slots:

private slots:

    void entereds1();

    void entereds2();

private:
    Ui::apmui *ui;
};

#endif // APMUI_H
