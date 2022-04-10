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
#include "MixedNavigationFile.h"



std::map<QString,Calculations> wyniki;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton_Oblicz->setEnabled(false);
    ui->progressBar->setVisible(false);
    ui->checkBoxObsOK->setChecked(false);
    ui->checkBoxNaviOK->setChecked(false);
    ui->PodsumowanieWynikow->hide();
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
                ElemLinii = linia.split("|",QString::SkipEmptyParts);
                settings.insert({ElemLinii[0],ElemLinii[1]}); // <- do mapy
                try {
                    WczytajUstawienie(ElemLinii[0],ElemLinii[1]); // <- wywołaj metodę i przypis ustawienie
                }catch(RinexNavigacyjny::Blad err){
                    ui->labelNviFilePath->setText("Nic nie wybrano!");
                    ui->checkBoxNaviOK->setChecked(false);
                    //zmien kolor na czerwony
                    QPalette pl = ui->labelNviFilePath->palette();
                    pl.setColor(QPalette::WindowText,QColor(Qt::red));
                    ui->labelNviFilePath->setPalette(pl);
                }catch(RinexObserwacyjny::Blad err)
                 {
                    ui->labelObsFilePath->setText("Nic nie wybrano!");
                    QPalette pal = ui->labelObsFilePath->palette();
                    pal.setColor(QPalette::WindowText,QColor(Qt::red));
                    ui->labelObsFilePath->setPalette(pal);
                    ui->checkBoxObsOK->setChecked(false);
                }
            }
            FileUst.close();
        }


    }
}
void MainWindow::on_ButtonReadObsFile_clicked()
{
    //wyczysc dane o nagłówku
    ui->labelWspX->setText("-");
    ui->labelWspY->setText("-");
    ui->labelWspZ->setText("-");
    ui->labelWspB->setText("-");
    ui->labelWspL->setText("-");
    ui->labelWspH->setText("-");
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
                                            "pliki RNX (*O.rnx);;inne pliki(*.??o)");
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
        this->ZapiszDoPlikuUstawien(F_DF,Sciezka);

        ui->checkBoxObsOK->setChecked(true);
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
                                            "Wybierz plik RINEX'u navigacyjnego",ObecnyFolder,
                                            "pliki RNX (*N.rnx);;GPS z CDDIS(brdc*.??n)");

    if(Filename != NULL)
    {

        QString File;
        int indOf;
        indOf = Filename.lastIndexOf("/") +1;
        File = Filename.mid(indOf);
        Sciezka = Filename.mid(0,indOf);

        try {
            BrdcEphemeris *br = new BrdcEphemeris;
            DataPlikuNavi = br->dataPliku(Filename);
            delete br;
            ui->labelNviFilePath->setText(Filename);
            ui->checkBoxNaviOK->setChecked(true);

            //zmien kolor na zielony
            QPalette pl = ui->labelNviFilePath->palette();
            pl.setColor(QPalette::WindowText,QColor(Qt::green));
            ui->labelNviFilePath->setPalette(pl);

            this->ZapiszDoPlikuUstawien(F_RN,Filename);
            this->ZapiszDoPlikuUstawien(F_DF,Sciezka);

        }catch(RinexNavigacyjny::Blad er)
        {
            QMessageBox msg;
            msg.setText("Błąd otwierania pliku nawigacyjnego!");
            msg.exec();
            Filename = "Nic nie wybrano!";
            ui->labelNviFilePath->setText(Filename);
            ui->checkBoxNaviOK->setChecked(false);
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

///przygotowaie obliczeń
void MainWindow::on_pushButton_Oblicz_clicked()
{
    //mapa przechowująca informacje o ewentualnych błędach
    map<QString,QString> BledyInfo = {};

    //czyszczenie mapy wyników pośrednich oraz tablicy
    //ze współrzędnymi poprawionymi
    wyniki.clear();
    ui->PodsumowanieWynikow->hide();
    //sprawdzenie daty
    if(!(DataPlikuNavi==DataPlikuObs))
    {
        QMessageBox msg;
        msg.setText("Błąd wyboru plików!");
        QString info = "Wybrane pliki są z różnych dat! \n"
                            "Data pliku nawigacyjnego: " + DataPlikuNavi +
                "\nData pliku obserwacyjnego: " + DataPlikuObs;
        msg.setInformativeText(info);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        return ;
    }
    //odczytanie wybranych czestotliwosci
    std::vector<QString> P1P2;
    P1P2.push_back(ui->comboBoxL1->currentText());
    P1P2.push_back(ui->comboBoxL2->currentText());
    P1P2.push_back(ui->comboBoxE1->currentText());
    P1P2.push_back(ui->comboBoxE2->currentText());

    //odczytanie wybranych poprawek
    std::map<QString,bool> WybranePoprawki;
    bool poprawka;
    poprawka = ui->Corr_IonoFree->isChecked();
    WybranePoprawki.insert({"iono_free",poprawka}); //kombinacja iono-free
    poprawka = ui->Corr_clockTime->isChecked();
    WybranePoprawki.insert({"eart_move",poprawka}); //poprawka ze względu na obrót ziemi
    poprawka = ui->Corr_relativistic->isChecked();
    WybranePoprawki.insert({"relativistic",poprawka}); //poprawka relatywistyczna
    poprawka = ui->Corr_tropo->isChecked();
    WybranePoprawki.insert({"tropo",poprawka}); //poprawka troposferyczna
    poprawka = ui->Corr_stdIono->isChecked();
    WybranePoprawki.insert({"std_iono",poprawka}); //poprawka jonosferyczna
    poprawka = ui->Corr_elevation->isChecked();
    WybranePoprawki.insert({"lowElevation",poprawka}); //usuwanie niskich satelitów

    ////////////////////////////////////////////

    ui->tableWyniki->setRowCount(0);
    //Przypisanie wybranego czasu do zmiennych klasy MyTimeClass
    QTime OdczytCzasu;
    OdczytCzasu = ui->StartTime->time();
    MyTimeClass StartCalc(OdczytCzasu.hour(),OdczytCzasu.minute(),OdczytCzasu.second());
    OdczytCzasu = ui->EndTime->time();
    MyTimeClass EndCalc(OdczytCzasu.hour(),OdczytCzasu.minute(),OdczytCzasu.second());

    //SPrawdzenie poprawności wybranych danych
    if(StartCalc>EndCalc)
    {
        QMessageBox Informacja;
        Informacja.setText("Błąd wyboru czasu obliczeń!");
        Informacja.setInformativeText("Czas końca nie może być wecześniejszy niż czas początku!");
        Informacja.setStandardButtons(QMessageBox::Ok);
        Informacja.exec();
        return;
    }
    //Instrukcje dla poprawnie wybranego czasu
    QString ObservationFileName;
    QString NavigationFileName;
    ObservationFileName = ui->labelObsFilePath->text();
    NavigationFileName = ui->labelNviFilePath->text();

    QString naviTypMX = NavigationFileName.right(6);
    QString naviTypBR = NavigationFileName.right(12);
    naviTypBR = naviTypBR.left(4);



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
        BrdcEphemeris brd;
        MixedNavigationFIle mix;
        ///////////////////////////
        for (auto& elem: PrzedzialObliczen)
        {
            ui->progressBar->setValue(IteratorElementu);
            QCoreApplication::processEvents();
            RinexOVer3 ObsFile(ObservationFileName,elem); //stworzenie obiektu z informacjami na daną epoke
            if (naviTypBR == "brdc"){

                brd = BrdcEphemeris(NavigationFileName,elem); //stworzenie obiektu z informacjami na daną epoke
                wskNavi = &brd;
            }else if (naviTypMX == "MN.rnx")
            {
                mix = MixedNavigationFIle(NavigationFileName,elem);
                wskNavi = &mix;
            }else
            {
                QMessageBox msg;
                msg.setText("Błąd pliku nawigacyjnego!");
                msg.setInformativeText("Musi mieć rozszerzenie *.??n lub MN.rnx!! Inne pliki nie są obsługiwane");
                msg.exec();
                ui->progressBar->setVisible(false);
                return;
            }
            wskObs = &ObsFile;
            Calculations obliczenia(wskNavi,wskObs);
            if (poprawka)
            {
                obliczenia.UstawMaksymalnyKatElewacji(ui->horizontalSlider->value());
            }
            int TableSize = ui->tableWyniki->rowCount();
            //Opracje dla zapisania wyników obliczeń
            try {
                ui->tableWyniki->insertRow(TableSize);
                ui->tableWyniki->setItem(TableSize,0,new QTableWidgetItem(elem.GetTimeQString()));
                obliczenia.oblicz_popr(WybranePoprawki,P1P2);
                obliczenia.ZapiszWynikiDoTablicy(ui->tableWyniki);

                obliczenia.UsunDanePlikow();
                wyniki.insert({elem.GetTimeQString(),obliczenia});
            }  catch (Calculations::BladObliczen er) {
                ui->tableWyniki->removeRow(TableSize);
                BledyInfo.insert({elem.GetTimeQString(),er.informacja});

            }
            catch(Calculations::BladGruby gr)
            {
                ui->tableWyniki->setRowCount(0);
                QMessageBox informacja;
                informacja.setText(gr.info);
                informacja.exec();
                ui->progressBar->setVisible(false);
                return;
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
    ObliczSrednie();
    if (BledyInfo.size()>0)
    {
        OknoUwag = new Uwagi(this);
        OknoUwag->Informacje = &BledyInfo;
        OknoUwag->Uzupelnij();
        OknoUwag->show();

    }
}
//funckja wczytuje wcześniej wybrane pliki
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
        try
        {
            BrdcEphemeris *br = new BrdcEphemeris;
            DataPlikuNavi = br->dataPliku(wartosc);
            delete br;
        }
        catch(RinexNavigacyjny::Blad er)
        {
            QString Filename = "Nic nie wybrano!";
            ui->labelNviFilePath->setText(Filename);
            ui->checkBoxNaviOK->setChecked(false);
        }

    }
    this->SprawdzStatusWczytaniaPlikow();
}

void MainWindow::WczytajRinexObserwacyjny(QString Filename)
{
    RinexOVer3 *rinex = new RinexOVer3(Filename);

    //wstawienie czestotliwosci do combobox'ów
    rinex->WstawCzestotliwosciDoComboBox(
                ui->comboBoxL1, ui->comboBoxL2,
                ui->comboBoxE1,ui->comboBoxE2);

    //Wpisanie współrzędnych odbiornika do odpowiednich etykiet

    Calculations *WspOdbiornika = new Calculations;
    WspOdbiornika->WpiszWspolrzedneOdbiornika(rinex,
                                              ui->labelWspX,ui->labelWspY,ui->labelWspZ,
                                              ui->labelWspB,ui->labelWspL,ui->labelWspH);
    delete  WspOdbiornika;

    //Znalezienie poprawnej daty pliku nawigacyjnego
    DataPlikuObs = rinex->getFileDate();
    delete rinex;
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
                //sprawdz spacje w pliku
                ElemLinii = linia.split("|",QString::SkipEmptyParts);

                if (ElemLinii[0] == key) //znalazo element w pliku z informacja o nazwie pliku
                {
                    //dane w pliku o rinexie są juz zapisane - czas na podmiane
                    if (ElemLinii[1] == FileName)
                    {
                        st.close();
                        return;
                    }
                    fs.seek(0); //ustaw wskaznik na poczatek
                    QString curLine = linia + "\n";
                    linia = fs.readAll(); //caly plik do zmiennej
                    linia.replace(curLine,""); //podmien pliki
                    linia = linia + key + "|" + FileName + "\n";
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
                    add << key + "|" + FileName + '\n';
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
                out << F_DF + "|"+ Sciezka + '\n';
                settings.insert({F_DF,Sciezka});
            }
            out << key + "|" + FileName + '\n';
            st.flush();
            st.close();
            settings.insert({key,FileName});
        }
    }
}
//sprawdza czy obydwa pliki są prawidłowo wczytane
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
//Obliczenie średnich i wstawienie ich do zestawienia wszystkich wyników
void MainWindow::ObliczSrednie()
{
    double X=0;
    double Y=0;
    double Z=0;
    for(int i = 0; i <= ui->tableWyniki->rowCount()-1;i++)
    {
        QString Xs,Ys,Zs;
        Xs = ui->tableWyniki->item(i,1)->text();
        Ys = ui->tableWyniki->item(i,2)->text();
        Zs = ui->tableWyniki->item(i,3)->text();
        X += Xs.toDouble();
        Y += Ys.toDouble();
        Z += Zs.toDouble();
    }
    X = X / ui->tableWyniki->rowCount();
    Y = Y / ui->tableWyniki->rowCount();
    Z = Z / ui->tableWyniki->rowCount();
    double B,L,H;
    double *wB = &B;
    double *wL = &L;
    double *wH = &H;
    PrzeliczNaWGS84(X,Y,Z,wB,wL,wH);
    //wstawianie uśrednionych wyników
    //ICRF
    ui->ICRF_X->setText(QString::number(X,'f',2));
    ui->ICRF_Y->setText(QString::number(Y,'f',2));
    ui->ICRF_Z->setText(QString::number(Z,'f',2));
    //WGS84
    ui->WGS_X->setText(QString::number(B,'f',10));
    ui->WGS_Y->setText(QString::number(L,'f',10));
    ui->WGS_Z->setText(QString::number(H,'f',10));

    ui->PodsumowanieWynikow->show();
}

//funcja zamienia współrzędne geocentryczne XYZ na elipsoidalne
//na podstawie parametrów elipsoidy GRS80
void MainWindow::PrzeliczNaWGS84(long double X, long double Y, long double Z, double *B, double *L, double *H)
{
    /* IN : wsp satelity w XYZ
     * OUT: wsp satelity w BLH
     */
    const long double pi_const = 3.14159265359;
    //parametry elipsoidy GRS84
    const double a = 6378137.0;
    const double b = 6356752.314245;
    const double e = sqrt((pow(a,2)-pow(b,2))/pow(a,2));
    double R;

    R = sqrt(pow(X,2)+pow(Y,2));
    double q = 0;
    double B0 = atan((Z+q)/R);
    double RN = a/sqrt(1-(pow(e,2)*pow(sin(B0),2)));
    q = RN * pow(e,2) * sin(B0);
    double Bi=0;
    Bi = atan((Z+q)/R);
    do{
        B0 = Bi;
        RN = a/sqrt(1-(pow(e,2)*pow(sin(Bi),2)));
        q = RN * pow(e,2) * sin(Bi);
        Bi = atan((Z+q)/R);
    }while (abs(B0-Bi)>0.00000000001);
    double Lrad = asin(Y/R);

    double del_r, del_z;
    del_r = R - RN * cos(Bi);
    del_z = Z - RN * (1-pow(e,2)) * sin(Bi);

    //Wyznaczenie wysokosci
    if(del_r < 0 || del_z < 0)
    {
        *H = -sqrt(pow(del_r,2) + pow(del_z,2));
    }else
    {
        *H = sqrt(pow(del_r,2) + pow(del_z,2));
    }

    *B = Bi * (180/pi_const);
    *L = Lrad *(180/pi_const);

}

//okno tabel kombinacji iono-free
void MainWindow::on_Informacje_clicked()
{
    infoWindow = new PanelInformacyjny(this);
    infoWindow->show();
}

///Obliczenie wpółrzędnych wgs84, wyświetlenie okna z pośrednimi wartościami
void MainWindow::on_tableWyniki_cellClicked(int row, int column)
{
    // jeżeli odznaczone w opcjach zakończ///
    if(ui->radioButtonPosrednie->isChecked())
    {

    /////////////////otworzenie okna z obliczeniami/////////////

    QString selectedItem = ui->tableWyniki->item(row,0)->text();
    oknoWynikowe = new WynikiPosrednie(this);
    Calculations *calc = &wyniki[ui->tableWyniki->item(row,0)->text()];
    oknoWynikowe->WartTropo = &calc->WartTropo;
    oknoWynikowe->WartIono = &calc->WartIono;
    oknoWynikowe->WartDxDyDz = &calc->WartDxDyDz;
    oknoWynikowe->WartDSV = &calc->WartDSV;
    oknoWynikowe->WartPoprCzasu = &calc->WartPoprCzasu;
    oknoWynikowe->PoczCxC = &calc->PoczCxC;
    oknoWynikowe->KoncCxC = &calc->KoncCxC;
    oknoWynikowe->WspSatelitow = &calc->WspSatelitow;
    oknoWynikowe->Dreal = &calc->WartPoprRelat;
    oknoWynikowe->WspSatelitowBezKorekcji = &calc->WspSatelitowBezKorekcji;
    oknoWynikowe->PoprawionyCzas = &calc->PoprawionyCzas;
    oknoWynikowe->AzymutSatelitow = &calc->AzymutSatelitow;
    oknoWynikowe->KatElewacjiSatelitow = &calc->KatElewacjiSatelitow;
    oknoWynikowe->P1CxC = &calc->P1CxC;
    oknoWynikowe->P2CxC = &calc->P2CxC;
    oknoWynikowe->UwagiDoSatelitow = &calc->UwagiDoSatelitow;
    oknoWynikowe->setWindowTitle("Epoka: "+selectedItem);
    oknoWynikowe->uzupelnijSatelity();
    oknoWynikowe->show();
    }
}



//Export danych
void MainWindow::on_pushButton_clicked()
{

    QMessageBox msg;
    msg.setText("Exportowanie danych do pliku txt");
    msg.setInformativeText("Czy wyexportować dane pośrednie?");
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int ret = msg.exec();
    bool exportP = false;
    if (ret == QMessageBox::Yes)
    {
        exportP = true;
    }
    QString fileExport = QFileDialog::getSaveFileName(this,
                                                      tr("Wybierz lokalizację pliku"), "",
                                                      tr("Txt (*.txt);;All Files (*)"));

    if (fileExport.isEmpty())
        return;

    else {
        QFile file(fileExport);
        if (!file.open(QIODevice::WriteOnly|QIODevice::Truncate)) {
            QMessageBox::information(this, tr("Nie można otworzyć pliku!"),
                                     file.errorString());
            return;
        }

        QTextStream data(&file);

        long Wyniki_size = ui->tableWyniki->rowCount()-1;
        QStringList strList;
        data << "LP\tEpoka\tX\tY\tZ\tdP\tB\tL\tH\n";
        if (exportP)
        {
            data << "Dane posrednie: \n";
            data << "SV\tdTSV\tdRel\tdT\tAzymut\tElewacja\tdTropo\tdIono\t"
            "PsD1\tPsD2\tpoczPsD\tkoncPsD\t"
            "Xp\tYp\tZp\tXk\tYk\tZk\n";

        }
        long linia = 1;
        for( int r = 0; r < Wyniki_size; ++r )
        {
            strList.clear();
            strList << QString::number(linia);
            linia++;
            for( int c = 0; c < 8; ++c )
            {   QTableWidgetItem* item = ui->tableWyniki->item(r,c);
                if (!item || item->text().isEmpty())
                {
                    ui->tableWyniki->setItem(r,c,new QTableWidgetItem());

                }
                strList << ui->tableWyniki->item(r,c)->text();
                strList.removeAll(QString(""));
            }
            if (exportP){
                data << ">" + strList.join("\t")+"\n";
                Calculations *calc = &wyniki[ui->tableWyniki->item(r,0)->text()];
                for(auto& elem:calc->AzymutSatelitow)
                {
                    data << elem.first + "\t";
                    data << QString::number(calc->WartDSV[elem.first],'e',4) + "\t";
                    data << QString::number(calc->WartPoprRelat[elem.first],'e',4) + "\t";
                    data << QString::number(calc->WartPoprCzasu[elem.first],'f') + "\t";
                    data << calc->AzymutSatelitow[elem.first] + "\t";
                    data << calc->KatElewacjiSatelitow[elem.first] + "\t";
                    data << QString::number(calc->WartTropo[elem.first],'f') + "\t";
                    data << QString::number(calc->WartIono[elem.first],'f') + "\t";
                    data << QString::number(calc->P1CxC[elem.first],'f',3) + "\t";
                    data << calc->P2CxC[elem.first].replace("Nie wyszukiwano.","-") + "\t";
                    data << QString::number(calc->PoczCxC[elem.first],'f',3) + "\t";
                    data << QString::number(calc->KoncCxC[elem.first],'f',3) + "\t";

                    std::vector<long double> XYZ = calc->WspSatelitowBezKorekcji[elem.first];
                    data << QString::number(XYZ[0],'f',3) +"\t";
                    data << QString::number(XYZ[1],'f',3) +"\t";
                    data << QString::number(XYZ[2],'f',3) +"\t";
                    XYZ = calc->WspSatelitow[elem.first];
                    data << QString::number(XYZ[0],'f',3) +"\t";
                    data << QString::number(XYZ[1],'f',3) +"\t";
                    data << QString::number(XYZ[2],'f',3) +"\t";
                    data << "\n";

                }

            }else{
                data << strList.join("\t")+"\n";
            }
        }
        file.close();

    }
}

//ustawienia zależności konterolek
void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    ui->wartEle->setText(QString::number(position)+ "°");
}


void MainWindow::on_Corr_elevation_stateChanged(int arg1)
{
    ui->wartEle->setEnabled(arg1);
    ui->horizontalSlider->setEnabled(arg1);
}


void MainWindow::on_Corr_IonoFree_stateChanged(int arg1)
{
    if (arg1)
    {
        ui->Corr_stdIono->setEnabled(false);
        ui->Corr_stdIono->setChecked(false);
    }else
    {
        ui->Corr_stdIono->setEnabled(true);
    }
}


void MainWindow::on_Corr_stdIono_stateChanged(int arg1)
{
    if (arg1){
        ui->Corr_IonoFree->setChecked(false);
    }
}

