#include "wynikiposrednie.h"
#include "ui_wynikiposrednie.h"
#include <QStringList>

WynikiPosrednie::WynikiPosrednie(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WynikiPosrednie)
{
    ui->setupUi(this);
}

WynikiPosrednie::~WynikiPosrednie()
{
    delete ui;
}

void WynikiPosrednie::uzupelnijSatelity()
{
    ui->tableWidget->setRowCount(0);

    for(auto& elem: *PoczCxC)
    {
                ui->Satelity->addItem(elem.first);
    }

    for(auto& elem: *WartDxDyDz)
        {
                 QStringList str;
                 str = elem.second.split(";");
                 int row;
                 row = ui->tableWidget->rowCount();
                 ui->tableWidget->insertRow(row);
                 ui->tableWidget->setItem(row,0,new QTableWidgetItem(str[0]));
                 ui->tableWidget->setItem(row,1,new QTableWidgetItem(str[1]));
                 ui->tableWidget->setItem(row,2,new QTableWidgetItem(str[2]));
                 ui->tableWidget->setItem(row,3,new QTableWidgetItem(str[3]));
                 ui->tableWidget->setItem(row,4,new QTableWidgetItem(str[4]));
        }
}
/*
void WynikiPosrednie::on_Satelity_currentIndexChanged(const QString &arg1)
{
    ///wstawienie warrtości dla danego satelity
    ui->dSV->setText(QString::number((*WartDSV)[arg1],'e',5));
    ui->dPopCzasu->setText(QString::number((*WartPoprCzasu)[arg1],'f',5));
    ui->dTropo->setText(QString::number((*WartTropo)[arg1],'f',5));
    ui->dIono->setText(QString::number((*WartIono)[arg1],'f',5));
    ui->dPoczCxc->setText(QString::number((*PoczCxC)[arg1],'f',6));
    ui->dKoncCxc->setText(QString::number((*KoncCxC)[arg1],'f',6));
    ui->dRealValue->setText(QString::number((*Dreal)[arg1],'e',5));
    ui->dPopCzasSV->setText((*PoprawionyCzas)[arg1]);
    ui->Azymut->setText((*AzymutSatelitow)[arg1]);
    ui->Elewacja->setText((*KatElewacjiSatelitow)[arg1]);
    ui->P1->setText(QString::number((*P1CxC)[arg1],'f',3));
    ui->P2->setText((*P2CxC)[arg1]);
    ui->Uwagi->setText((*UwagiDoSatelitow)[arg1]);
    std::vector<long double> wsp;
    wsp = (*WspSatelitow)[arg1];
    ui->wspX->setText(QString::number(wsp[0],'f',3));
    ui->wspY->setText(QString::number(wsp[1],'f',3));
    ui->wspZ->setText(QString::number(wsp[2],'f',3));

    wsp = (*WspSatelitowBezKorekcji)[arg1];
    ui->wspPX->setText(QString::number(wsp[0],'f',3));
    ui->wspPY->setText(QString::number(wsp[1],'f',3));
    ui->wspPZ->setText(QString::number(wsp[2],'f',3));


}
*/

void WynikiPosrednie::on_Satelity_currentTextChanged(const QString &arg1)
{
    ///wstawienie warrtości dla danego satelity
    ui->dSV->setText(QString::number((*WartDSV)[arg1],'e',5));
    ui->dPopCzasu->setText(QString::number((*WartPoprCzasu)[arg1],'f',5));
    ui->dTropo->setText(QString::number((*WartTropo)[arg1],'f',5));
    ui->dIono->setText(QString::number((*WartIono)[arg1],'f',5));
    ui->dPoczCxc->setText(QString::number((*PoczCxC)[arg1],'f',6));
    ui->dKoncCxc->setText(QString::number((*KoncCxC)[arg1],'f',6));
    ui->dRealValue->setText(QString::number((*Dreal)[arg1],'e',5));
    ui->dPopCzasSV->setText((*PoprawionyCzas)[arg1]);
    ui->Azymut->setText((*AzymutSatelitow)[arg1]);
    ui->Elewacja->setText((*KatElewacjiSatelitow)[arg1]);
    ui->P1->setText(QString::number((*P1CxC)[arg1],'f',3));
    ui->P2->setText((*P2CxC)[arg1]);
    ui->Uwagi->setText((*UwagiDoSatelitow)[arg1]);
    std::vector<long double> wsp;
    wsp = (*WspSatelitow)[arg1];
    ui->wspX->setText(QString::number(wsp[0],'f',3));
    ui->wspY->setText(QString::number(wsp[1],'f',3));
    ui->wspZ->setText(QString::number(wsp[2],'f',3));

    wsp = (*WspSatelitowBezKorekcji)[arg1];
    ui->wspPX->setText(QString::number(wsp[0],'f',3));
    ui->wspPY->setText(QString::number(wsp[1],'f',3));
    ui->wspPZ->setText(QString::number(wsp[2],'f',3));
}

