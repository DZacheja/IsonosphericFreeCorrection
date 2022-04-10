#include "panelinformacyjny.h"
#include "ui_panelinformacyjny.h"

PanelInformacyjny::PanelInformacyjny(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PanelInformacyjny)
{
    ui->setupUi(this);
}

PanelInformacyjny::~PanelInformacyjny()
{
    delete ui;
}
