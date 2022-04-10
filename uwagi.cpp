#include "uwagi.h"
#include "ui_uwagi.h"

Uwagi::Uwagi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Uwagi)
{
    ui->setupUi(this);
}

Uwagi::~Uwagi()
{
    delete ui;
}

void Uwagi::on_pushButton_clicked()
{
    delete this;
}

void Uwagi::Uzupelnij()
{
    for (auto& elem: *Informacje)
    {
        auto&[epoka,info] = elem;
                int row;
                row = ui->tableWidget_Uwagi->rowCount();
                ui->tableWidget_Uwagi->insertRow(row);
                ui->tableWidget_Uwagi->setItem(row,0,new QTableWidgetItem(epoka));
                ui->tableWidget_Uwagi->setItem(row,1,new QTableWidgetItem(info));
    }

}
