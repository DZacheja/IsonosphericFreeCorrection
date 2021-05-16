#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "RinexOVer3.h"
//#include "crx2rnx.h"
#include <QString>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include "calculations.h"
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

    }else
    {
        Filename = "<font color='red'>Nic nie wybrano!</font>";
        ui->labelNviFilePath->setText(Filename);
    }
}

void MainWindow::on_pushButton_Oblicz_clicked()
{
    ui->tableWyniki->setRowCount(0);
    //Przypisanie wybranego czasu do zmiennych klasy MyTimeClass
    QTime OdczytCzasu;
    OdczytCzasu = ui->StartTime->time();
    MyTimeClass StartCalc(OdczytCzasu.hour(),OdczytCzasu.minute(),OdczytCzasu.second());
    OdczytCzasu = ui->EndTime->time();
    MyTimeClass EndCalc(OdczytCzasu.hour(),OdczytCzasu.minute(),OdczytCzasu.second());
    bool SprawdzenieCzasu = true;
    //SPrawdzenie poprawności wybranych danych
    if(StartCalc>EndCalc)
    {
        SprawdzenieCzasu = false;
        QMessageBox Informacja;
        Informacja.setText("Błąd wyboru czasu obliczeń!");
        Informacja.setInformativeText("Czas końca nie może być wecześniejszy niż czas początku!");
        Informacja.setStandardButtons(QMessageBox::Ok);
        Informacja.exec();

    }
    //Instrukcje dla poprawnie wybranego czasu
    if (SprawdzenieCzasu)
    {
        QString ObservationFileName;
        QString NavigationFileName;
        ObservationFileName = ui->labelObsFilePath->text();
        ObservationFileName.remove("<font color='green'>");
        ObservationFileName.remove("</font>");
        NavigationFileName = ui->labelNviFilePath->text();
        NavigationFileName.remove("<font color='green'>");
        NavigationFileName.remove("</font>");
        RinexOVer3 ObsFile(ObservationFileName);
        std::vector<MyTimeClass> PrzedzialObliczen = ObsFile.PrzedzialGodzinowy(StartCalc,EndCalc);
        //Wskaźniki -> przyśpieszą znacząco czas obliczeń
        RinexNavigacyjny *wskNavi;
        RinexObserwacyjny *wskObs;
        for (auto& elem: PrzedzialObliczen)
        {
            RinexOVer3 ObsFile(ObservationFileName,elem); //stworzenie obiektu z informacjami na daną epoke
            BrdcEphemeris NaviFile(NavigationFileName,elem); //stworzenie obiektu z informacjami na daną epoke
            wskNavi = &NaviFile;
            wskObs = &ObsFile;
            Calculations obliczenia(wskNavi,wskObs);
            int TableSize = ui->tableWyniki->rowCount();
            //Opracje dla zapisania wyników obliczeń
                ui->tableWyniki->insertRow(TableSize);
                ui->tableWyniki->setItem(TableSize,0,new QTableWidgetItem(elem.GetTimeQString()));
            obliczenia.oblicz();
            obliczenia.ZapiszWynikiDoTablicy(ui->tableWyniki);
        }


    }
}

void MainWindow::on_TestButton_clicked()
{
    int TableSize = ui->tableWyniki->rowCount();
    ui->tableWyniki->insertRow(TableSize);
    ui->tableWyniki->setItem(TableSize,0,new QTableWidgetItem("Test "+QString::number(TableSize)));
    Calculations test(ui->tableWyniki);

}
