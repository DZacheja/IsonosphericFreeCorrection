#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "RinexOVer3.h"
//#include "crx2rnx.h"
#include <QString>
#include <QFileDialog>
#include <QDir>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_TestowyGuzik_clicked()
{
         QString filename = "CACE00ESP_R_20210020000_01D_30S_MO.crx";

        // crx2rnx *Plik;
        // Plik = new crx2rnx(filename);
        // delete Plik;
}

void MainWindow::on_ButtonReadObsFile_clicked()
{
    //wczytanie pliku i jego zapis do okienka
    QString ObecnyFolder = QDir::currentPath();
    int PozycjaSlesza = ObecnyFolder.lastIndexOf("/");
    ObecnyFolder = ObecnyFolder.mid(0,PozycjaSlesza) + "/IonoFreeCorrection/DANE";
    QString Filename;
    Filename = QFileDialog::getOpenFileName(this,
                                            "Wybierz plik RINEX'u obserwacyjnego",ObecnyFolder,
                                            "pliki RNX (*.rnx);;pliki CRX (*.crx)");
    if(Filename != NULL)
    {
       RinexOVer3 rinex(Filename);
       Filename = "<font color='green'>" + Filename + "</font>";
       ui->labelObsFilePath->setText(Filename);

       rinex.WstawCzestotliwosciDoComboBox(
                   ui->comboBoxL1, ui->comboBoxL2);

    }else
    {
        Filename = "<font color='red'>Nic nie wybrano!</font>";
        ui->labelObsFilePath->setText(Filename);

    }
}

void MainWindow::on_ButtonReadNaviFile_clicked()
{
    //wczytanie pliku i jego zapis do okienka
    QString ObecnyFolder = QDir::currentPath();
    int PozycjaSlesza = ObecnyFolder.lastIndexOf("/");
    ObecnyFolder = ObecnyFolder.mid(0,PozycjaSlesza) + "/IonoFreeCorrection/DANE";
    QString Filename;
    Filename = QFileDialog::getOpenFileName(this,
                                            "Wybierz plik RINEX'u navigacyjnego",ObecnyFolder);

    if(Filename != NULL)
    {
       Filename = "<font color='green'>" + Filename + "</font>";
       ui->labelNviFilePath->setText(Filename);

    }
    {
        Filename = "<font color='red'>Nic nie wybrano!</font>";
        ui->labelNviFilePath->setText(Filename);
    }
}
