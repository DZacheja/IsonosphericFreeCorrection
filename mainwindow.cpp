#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "RinexOVer3.h"
//#include "crx2rnx.h"
#include <QString>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include "calculations.h"
#include "RinexObserwacyjny.h"
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include <QColorDialog>
#include <QPalette>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton_Oblicz->setEnabled(false);
    ui->progressBar->setVisible(false);
    ui->checkBoxObsOK->setChecked(false);
    ui->checkBoxNaviOK->setChecked(false);
    this->Ustawienia();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Ustawienia()
{
    //sprawdzenie czy istnieje plik z ustawieniami
    if(QFile::exists(fileSettings)){
        //jeżeli istnieje - wczytaj go i zapisz dane do mpay
        QFile FileUst(fileSettings);
        if(FileUst.open(QFile::ReadOnly|QFile::Text)){
            QTextStream in(&FileUst);
            QString linia;
            QStringList ElemLinii;
            while(!in.atEnd()){
                linia = in.readLine();
                ElemLinii = linia.split(" ",QString::SkipEmptyParts);
                settings.insert({ElemLinii[0],ElemLinii[1]}); // <- do mapy
                WczytajUstawienie(ElemLinii[0],ElemLinii[1]); // <- wywołaj metodę i przypis ustawienie
            }
            FileUst.close();
        }


    }
}
void MainWindow::on_ButtonReadObsFile_clicked()
{
    //wczytanie pliku i jego zapis do okienka
    QString ObecnyFolder = QDir::currentPath();
    int PozycjaSlesza = ObecnyFolder.lastIndexOf("/");
    ObecnyFolder = ObecnyFolder.mid(0,PozycjaSlesza) + "/IonoFreeCorrection/DANE";
    if (Sciezka != "")
    {
        ObecnyFolder = Sciezka;
    }
    QString Filename;
    Filename = QFileDialog::getOpenFileName(this,
                                            "Wybierz plik RINEX'u obserwacyjnego",ObecnyFolder,
                                            "pliki RNX (*.rnx);;pliki CRX (*.crx)");
    if(Filename != NULL)
    {
        WczytajRinexObserwacyjny(Filename);
        ui->labelObsFilePath->setText(Filename);
        //Zmiana koloru na zielony
        QPalette pal = ui->labelObsFilePath->palette();
        pal.setColor(QPalette::WindowText,QColor(Qt::green));
        ui->labelObsFilePath->setPalette(pal);

        PozycjaSlesza = Filename.lastIndexOf("/");
        Sciezka = Filename.mid(0,PozycjaSlesza);

        this->ZapiszDoPlikuUstawien(F_RO,Filename);
        ui->checkBoxObsOK->setChecked(true);

/////////////////////////////////////////////////////////////////////////////////
        // SPRAWDZENIE CZY ISTENIEJE ODPOWIEDNI PLIK NAVIGACYJNY //
/////////////////////////////////////////////////////////////////////////////////

        //pelna sziecka do pliku navigacyjnego
        QString fullPath = Sciezka + "/" + PoprawnaNazwaPlikuNavigacyjnego;

        //jeżeli kolor jest czerwony - można zadać pytanie - zielony plik juz wczytany
        if(!(ui->checkBoxNaviOK->isChecked())){

            //jezeli plik istnieje zapytaj czy wczytać
            if(QFile::exists(fullPath))
            {
                QMessageBox msg;
                msg.setText("Znaleziono odpowiedni plik navigacyjny w tym samym folderze");
                msg.setInformativeText("Czy chcesz go wczytać?");
                msg.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
                int ms = msg.exec();
                if(ms == QMessageBox::Yes)
                {

                    ui->labelNviFilePath->setText(fullPath);
                    //zapisanie wyboru do pliku ustawien
                    this->ZapiszDoPlikuUstawien(F_RN,fullPath);
                    ui->checkBoxNaviOK->setChecked(true);
                    //zmien kolor
                    QPalette pal = ui->labelObsFilePath->palette();
                    pal.setColor(QPalette::WindowText,QColor(Qt::green));
                    ui->labelNviFilePath->setPalette(pal);
                }
                }
        }

    }else
    {   //gdy wybrano zly plik obserwacyjny
        Filename = "Nic nie wybrano!";
        ui->labelObsFilePath->setText(Filename);
        QPalette pal = ui->labelObsFilePath->palette();
        pal.setColor(QPalette::WindowText,QColor(Qt::red));
        ui->labelObsFilePath->setPalette(pal);
        ui->checkBoxObsOK->setChecked(false);

    }
    this->SprawdzStatusWczytaniaPlikow();

}

void MainWindow::on_ButtonReadNaviFile_clicked()
{
    //wczytanie pliku i jego zapis do okienka
    QString ObecnyFolder = QDir::currentPath();
    int PozycjaSlesza = ObecnyFolder.lastIndexOf("/");
    ObecnyFolder = ObecnyFolder.mid(0,PozycjaSlesza) + "/IonoFreeCorrection/DANE";
    if (Sciezka != "")
    {
        ObecnyFolder = Sciezka;
    }

    QString Filename;
    Filename = QFileDialog::getOpenFileName(this,
                                            "Wybierz plik RINEX'u navigacyjnego",ObecnyFolder);

    if(Filename != NULL)
    {

        QString File;
        int indOf;
        indOf = Filename.lastIndexOf("/") +1;
        File = Filename.mid(indOf);
        Sciezka = Filename.mid(0,indOf);
        this->ZapiszDoPlikuUstawien(F_DF,Sciezka);


        if (File != PoprawnaNazwaPlikuNavigacyjnego)
        {
            QMessageBox msg;
            msg.setText("Wybrane pliki nie pasują do siebie!");
            msg.exec();
            ui->labelNviFilePath->setText("Wybrano nieprawidłowy plik!");

            //zmien kolor na czerwony
            QPalette pl = ui->labelNviFilePath->palette();
            pl.setColor(QPalette::WindowText,QColor(Qt::red));
            ui->labelNviFilePath->setPalette(pl);

        }else{
            ui->labelNviFilePath->setText(Filename);
            ui->checkBoxNaviOK->setChecked(true);

        //zmien kolor na zielony
        QPalette pl = ui->labelNviFilePath->palette();
        pl.setColor(QPalette::WindowText,QColor(Qt::green));
        ui->labelNviFilePath->setPalette(pl);
        this->ZapiszDoPlikuUstawien(F_RN,Filename);
        }
    }else
    {
        Filename = "Nic nie wybrano!";
        ui->labelNviFilePath->setText(Filename);
        ui->checkBoxNaviOK->setChecked(false);
        //zmien kolor na czerwony
        QPalette pl = ui->labelNviFilePath->palette();
        pl.setColor(QPalette::WindowText,QColor(Qt::red));
        ui->labelNviFilePath->setPalette(pl);
    }
    this->SprawdzStatusWczytaniaPlikow();
}

void MainWindow::on_pushButton_Oblicz_clicked()
{
    //odczytanie wybranych czestotliwosci
    std::vector<QString> P1P2;
    P1P2.push_back(ui->comboBoxL1->currentText());
    P1P2.push_back(ui->comboBoxL2->currentText());
    //odczytanie wybranych poprawek
    std::map<QString,bool> WybranePoprawki;
    bool poprawka;
    poprawka = ui->Corr_IonoFree->isChecked();
    WybranePoprawki.insert({"iono_free",poprawka});
    poprawka = ui->Corr_clockTime->isChecked();
    WybranePoprawki.insert({"clock",poprawka});
    poprawka = ui->Corr_relativistic->isChecked();
    WybranePoprawki.insert({"relativistic",poprawka});
    ////////////////////////////////////////////

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
        NavigationFileName = ui->labelNviFilePath->text();
        //otwieranie plikow
        try {
            RinexOVer3 ObsFile(ObservationFileName);
            std::vector<MyTimeClass> PrzedzialObliczen = ObsFile.PrzedzialGodzinowy(StartCalc,EndCalc);
            //Wskaźniki -> przyśpieszą znacząco czas obliczeń
            RinexNavigacyjny *wskNavi;
            RinexObserwacyjny *wskObs;
            //progress bar
            int ilosc_obliczen = PrzedzialObliczen.size();
            ui->progressBar->setMaximum(ilosc_obliczen);
            int IteratorElementu = 1;
            ui->progressBar->setVisible(true);
            ///////////////////////////
            for (auto& elem: PrzedzialObliczen)
            {
                ui->progressBar->setValue(IteratorElementu);

                RinexOVer3 ObsFile(ObservationFileName,elem); //stworzenie obiektu z informacjami na daną epoke
                BrdcEphemeris NaviFile(NavigationFileName,elem); //stworzenie obiektu z informacjami na daną epoke
                wskNavi = &NaviFile;
                wskObs = &ObsFile;
                Calculations obliczenia(wskNavi,wskObs);
                int TableSize = ui->tableWyniki->rowCount();
                //Opracje dla zapisania wyników obliczeń
                try {
                    ui->tableWyniki->insertRow(TableSize);
                    ui->tableWyniki->setItem(TableSize,0,new QTableWidgetItem(elem.GetTimeQString()));
                    obliczenia.oblicz(WybranePoprawki,P1P2);
                    obliczenia.ZapiszWynikiDoTablicy(ui->tableWyniki);
                }  catch (Calculations::BladObliczen er) {
                    QMessageBox informacja;
                    informacja.setText("Błąd w epokce :" + elem.GetTimeQString() + ", " + er.informacja);
                    informacja.exec();
                }


                //aktualizacja status bar
                IteratorElementu++;
                ui->progressBar->setValue(IteratorElementu);

            }
            //ukryj progress bar
            ui->progressBar->setVisible(false);

        }catch(RinexObserwacyjny::Blad err)
        {
            QMessageBox msgbox;
            msgbox.setText(err.info);
            msgbox.exec();
            return;
        }catch(RinexNavigacyjny::Blad err)
        {
            QMessageBox msgbox;
            msgbox.setText(err.info);
            msgbox.exec();
            return;
        }
    }
}

void MainWindow::WczytajUstawienie(QString typ, QString wartosc)
{
    if(typ == F_DF)
    {
        Sciezka = wartosc;
    }else if(typ == F_RO){
        ui->labelObsFilePath->setText(wartosc);

        //zmien kolor po wczytaniu na zielony
        QPalette pal = ui->labelObsFilePath->palette();
        pal.setColor(QPalette::WindowText,QColor(Qt::green));
        ui->labelObsFilePath->setPalette(pal);
        this->WczytajRinexObserwacyjny(wartosc);
        ui->checkBoxObsOK->setChecked(true);

    }else if(typ == F_RN)
    {
        ui->labelNviFilePath->setText(wartosc);

        //zmien kolor po wczytaniu na zielony
        QPalette pal = ui->labelNviFilePath->palette();
        pal.setColor(QPalette::WindowText,QColor(Qt::green));
        ui->labelNviFilePath->setPalette(pal);
        ui->checkBoxNaviOK->setChecked(true);
    }
    this->SprawdzStatusWczytaniaPlikow();
}

void MainWindow::WczytajRinexObserwacyjny(QString Filename)
{
    RinexOVer3 rinex(Filename);

    //wstawienie czestotliwosci do combobox'ów
    rinex.WstawCzestotliwosciDoComboBox(
                ui->comboBoxL1, ui->comboBoxL2);

    //Wpisanie współrzędnych odbiornika do odpowiednich etykiet
    RinexObserwacyjny *wskObs;
    wskObs = &rinex;
    Calculations *WspOdbiornika = new Calculations;
    WspOdbiornika->WpiszWspolrzedneOdbiornika(wskObs,ui->labelWspX,ui->labelWspY,ui->labelWspZ);
    delete  WspOdbiornika;

    //Znalezienie poprawnej nazwy pliku navigacyjnego
    DataPlikuObs = wskObs->getFileDate();
    BrdcEphemeris NazwaPlikuDoSprawdzenia(DataPlikuObs["dzien"],DataPlikuObs["miesiac"],DataPlikuObs["rok"],PoprawnaNazwaPlikuNavigacyjnego);

}

void MainWindow::ZapiszDoPlikuUstawien(QString key, QString FileName)
{
    /////////////////////////////////////////////////////////////////////////////////
            //Zapisanie wyboru do pliku Settings
    /////////////////////////////////////////////////////////////////////////////////
            if(QFile::exists(fileSettings))
            {
                QFile st(fileSettings);
                if(st.open(QFile::ReadWrite|QFile::Text)){
                    QTextStream fs(&st);
                    QStringList ElemLinii; // kontener kazdej linii
                    QString linia;
                    bool znalazlo = false;
                    while(!fs.atEnd())
                    {
                        linia = fs.readLine();
                        ElemLinii = linia.split(" ",QString::SkipEmptyParts);
                        if (ElemLinii[0] == key) //znalazo element w pliku z informacja o nazwie pliku
                        {
                            //dane w pliku o rinexie są juz zapisane - czas na podmiane
                            fs.seek(0); //ustaw wskaznik na poczatek
                            linia = fs.readAll(); //caly plik do zmiennej
                            linia.replace(settings[key],FileName); //podmien pliki
                            st.close(); //zamknij plik
                            st.open(QFile::WriteOnly|QFile::Truncate);
                            st.seek(0); //plik na poczatek
                            st.write(linia.toUtf8()); //zapisz podmieniony plik
                            st.close();
                            settings[key] = FileName;
                            znalazlo = true;
                            break; //koniec petli
                        }
                    }
                    if (!znalazlo){
                        st.close();
                        if(st.open(QIODevice::Append|QIODevice::Text))
                         {
                        QTextStream add(&st);
                        add << key + " " + FileName + '\n';
                        settings.insert({key,FileName});
                        st.close();
                        }
                    }
                }

            }else //plik nie istnieje
            {
                QFile st(fileSettings);
                if(st.open(QFile::WriteOnly|QFile::Text)){
                    QTextStream out(&st);
                    if (!(key == F_DF)){
                    out << F_DF + " "+ Sciezka + '\n';
                    settings.insert({F_DF,Sciezka});
                    }
                    out << key + " " + FileName + '\n';
                    st.flush();
                    st.close();
                    settings.insert({key,FileName});
                }
            }
}

void MainWindow::SprawdzStatusWczytaniaPlikow()
{
    //Sprawdzenie wczytania plikow
    if(ui->checkBoxNaviOK->isChecked() && ui->checkBoxObsOK->isChecked())
    {
        ui->pushButton_Oblicz->setEnabled(true);
    }else
    {
        ui->pushButton_Oblicz->setEnabled(false);
    }
}
