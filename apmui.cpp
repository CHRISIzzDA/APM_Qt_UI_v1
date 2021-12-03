#include "apmui.h"
#include "ui_apmui.h"

apmui::apmui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::apmui)
{
    ui->setupUi(this);
    startTimer(1000);
}

apmui::~apmui()
{
    delete ui;
}

///bashfkjhasiufhgsdzjfgsugfzsgezfgseugf
