#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "crx2rnx.h"
#include <QString>

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
