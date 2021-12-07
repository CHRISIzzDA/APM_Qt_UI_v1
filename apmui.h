#ifndef APMUI_H
#define APMUI_H

#include <QDialog>
#include <QTimer>
#include <QtDebug>
#include <QStateMachine>
#include <QEventTransition>


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
    QState *s3 = new QState();


public slots:

private slots:

    void on_pb_test_clicked();

private:
    Ui::apmui *ui;
};

#endif // APMUI_H
