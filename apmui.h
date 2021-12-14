#ifndef APMUI_H
#define APMUI_H

#include <QDialog>
#include <QTimer>
#include <QtDebug>
#include <QStateMachine>
#include <QEventTransition>
#include <QFinalState>
#include <QTextCursor>


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

    QState *getdata = new QState();
    QState *pAC = new QState();

    QTimer *timer = new QTimer();
    QTimer *timer2 = new QTimer();



public slots:

    void startup();

    void usbData(const QByteArray &data);

private slots:

    void gettingData();

    void prepAndCheck();

    void stop();

signals:
    void SendData(const char* data);

private:
    Ui::apmui *ui;
    QTextCursor c;
};

#endif // APMUI_H
