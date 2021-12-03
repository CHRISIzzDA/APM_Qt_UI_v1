#ifndef APMUI_H
#define APMUI_H

#include <QDialog>
#include <QTimer>
#include <QtDebug>

namespace Ui {
class apmui;
}

class apmui : public QDialog
{
    Q_OBJECT

public:
    explicit apmui(QWidget *parent = nullptr);
    ~apmui();

public slots:

private slots:

private:
    Ui::apmui *ui;
};

#endif // APMUI_H
