#include "RinexOVer3.h"
#include <QString>
#include <vector>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QChar>
#include <cmath>
#include <map>
#include <QDebug>

RinexOVer3::RinexOVer3()
{

}
RinexOVer3::RinexOVer3(QString ff)
{
    FileName = ff;
    //FileName = testowaSciezka;
    InHeader();

}

RinexOVer3::RinexOVer3(QString nazwa, MyTimeClass epoka)
{
    FileName = nazwa;
    EpokaObliczen = epoka;
    this->InHeader();
    this->WyszukajParametryCzestotliwosi();
}

void RinexOVer3::OtworzPlik()
{

}

void RinexOVer3::WyszukajParametryCzestotliwosi()
{
    // obliczany czas
    int HH, MM;
    double SS;
    HH = EpokaObliczen.getHour();
    MM = EpokaObliczen.getMinutes();
    SS = EpokaObliczen.getSecounds();
    //plik
    QString filenn = FileName;
    QFile file(filenn);

    //wzorując się na Aaron Boda ..
    const QString EndOfHeaderT = "END OF HEADER";

    bool inHeader = true;

    if (!file.open(QFile::ReadOnly|QFile::Text))
    {
        Blad err;
        err.info ="Błąd otwiercia pliku podczas wyszukiwania częstotliwości w pliku obserwacyjnym!";
        throw err;
    }
    else
    {
        QTextStream in(&file);


        QStringList ElementyLinii;
        QString time;
        bool NewTimeData = true;
        bool SzukanyCzas = false;
        //iteracja przez plik
        while(!in.atEnd())
        {
            QString linia = in.readLine();
            if(inHeader)
            {
                int isInHeader = linia.indexOf(EndOfHeaderT);

                if ((isInHeader == -1) && inHeader)
                {
                    continue;
                }
                else if (isInHeader > 0)
                {
                    inHeader = false;
                    continue;
                }
            }

            ElementyLinii.clear();
            int isNewTime = linia.indexOf(">");
            if(isNewTime >= 0)
            {
                if (SzukanyCzas) {break;}
                ElementyLinii = linia.split(" ",QString::SkipEmptyParts);
                int Hours,Minutes;
                double Secounds;
                Hours = ElementyLinii[4].toInt();
                Minutes = ElementyLinii[5].toInt();
                Secounds = ElementyLinii[6].toDouble();
                if(Hours == HH && Minutes == MM && Secounds == SS)
                {
                    SzukanyCzas = true;
                }
                time = ElementyLinii[4] + ":" + ElementyLinii[5] + ":" + ElementyLinii[6].mid(0,3);
                NewTimeData = true;
                continue;
            }
            //Jeżeli znalazło szukaną epokę zapisz wszystkie jej pomiary
            if(SzukanyCzas)
            {
                QString NumerSatelity = linia.mid(0,3);
                if(linia[0] != "G"){continue;} //obecnie obliczamy tylko sat. GPS
                int poczatek = 4;
                double IleIteracji;
                IleIteracji = (linia.length() - 5)/16;
                //Dzielenie liniii na poszczegolne elementy
                for(int i=0; i <= ceil(IleIteracji); i++)
                {
                    ElementyLinii.push_back(linia.mid(poczatek,16).trimmed());
                    poczatek += 16;
                }

                map<QString,long double> CurrentLine;
                for(int i = 0; i < ElementyLinii.count(); i++)
                {
                    QString item = PeriodicityOrder[i+2];
                    long double wartosc;
                    if (ElementyLinii[i]=="")
                    {
                        wartosc = 0;
                    }
                    else{
                        wartosc = stold(ElementyLinii[i].toStdString());
                    }
                    CurrentLine.insert({item,wartosc});
                }

                SatellitesPeriodicity.insert({NumerSatelity,CurrentLine});

            }

            if (!NewTimeData){continue;}
        }
    }
}

void RinexOVer3::InHeader(){
    //QString filenn = testowaSciezka + "\\" + testowaNazwaPliku;
    QString filenn = FileName;
    QFile file(filenn);
    QChar LSatSign = SateliteTypeAndNumber[0];
    bool FounAll = false;
    int PeriodicitySize;
    bool first = true;
    //wzorując się na Aaron Boda ..
    if (!file.open(QFile::ReadOnly|QFile::Text))
    {
        Blad txt;
        txt.info = "Błąd otwarcia pliku przy przeglądniu nagłówka w pliku Obserwacyjnym!";
        throw txt;
    }
    else
    {

        // linia w tablicy wielowymiarowej typu vector - po stworzeniu całej linii dodaje
        // się ja do kontenera jako nowy element.
        std::vector<QString> AddedLine;

        QTextStream in(&file);

        while(!in.atEnd())
        {

            QString ll = in.readLine();
            QStringList QSlist;

            int isEnd = ll.indexOf("END OF HEADER");
            if(isEnd > 0){break;}
            //tokeny
            int isXYZ = ll.indexOf("APPROX POSITION XYZ"); //flaga wspolrzednych odbiornika
            int isSys = ll.indexOf("SYS / # / OBS TYPES"); //flaga czestotliwosci
            int isStartTime = ll.indexOf("TIME OF FIRST OBS"); //falaga poczatku obs z pliku
            int isEndTime = ll.indexOf("TIME OF LAST OBS"); //flaga konca obserwacji z pliku
            if (isXYZ > 0){
                QSlist = ll.split(" ",QString::SkipEmptyParts);
                header.PozX = QSlist[0].toDouble();
                header.PozY = QSlist[1].toDouble();
                header.PozZ = QSlist[2].toDouble();
                continue;
            }

            if (isStartTime > 0){
                QSlist = QSlist = ll.split(" ",QString::SkipEmptyParts);
                header.rok = QSlist[0].toInt();
                header.mies = QSlist[1].toInt();
                header.dzien = QSlist[2].toInt();
                continue;
            }

            if (isEndTime > 0){
                QSlist = QSlist = ll.split(" ",QString::SkipEmptyParts);
                KoniecObserwacjiPliku = MyTimeClass(QSlist[0].toInt(),QSlist[1].toInt(),QSlist[2].toDouble());
                continue;
            }


            /* Jeżeli pierwszy znak linii jest spacją oznacza, że w dalszym ciągu dodajemy
* częstotliwośći satelity z poprzedniego wiersza. Jeżeli pierwszy znak nie jest
* spacją oznacza, że definiujemy nową listę częstotliwości więc kontener z obecnymi
* wartościami należy "dołożyć" do kontenera głównego w klasie (PeriodicityOrder)
* a następnie go wyczyścić w celu przygotowania do nowych danych.
*/

            else if(isSys >0)
            {
                if(PeriodicitySize == static_cast<int>(PeriodicityOrder.size())){continue;}
                QChar SPC = ll[0];
                if(SPC == LSatSign) {FounAll = true;}


                if(!(SPC.isSpace()))
                {
                    if(!(PeriodicityOrder.empty()))
                    {
                        if(FounAll){continue;;}
                    }

                }
                ll.remove("SYS / # / OBS TYPES");
                QSlist = ll.split(" ",QString::SkipEmptyParts); //utworzenie tablicy z linii, z pominieciem spacji
                //dodanie wszystkich elementow tablicy do kontenera
                if(first)
                {
                    PeriodicitySize = QSlist[1].toInt() + 2;
                    first = false;
                }

                for(auto elem : QSlist)
                {
                    PeriodicityOrder.push_back(elem);
                }
            }//elseif
        }//while
    }//1.else - plik ok
}//funckja

vector<MyTimeClass> RinexOVer3::PrzedzialGodzinowy(MyTimeClass poczatek, MyTimeClass koniec)
{
    StartCalculations = poczatek;
    EndCalculations = koniec;
    vector<MyTimeClass> wyniki;
    //Wczytanie pliku ...
    QString filenn = FileName;
    QFile file(filenn);
    if (!file.open(QFile::ReadOnly|QFile::Text))
    {
        QMessageBox msgbox;
        msgbox.setText("Błąd otwarcia pliku przy przeglądniu przedziału czasu w pliku obserwacyjnym!");
        msgbox.exec();
    }
    else
    {

        //zapisanie wszystkich czasow do vector'a.
        QTextStream in(&file);
        while(!in.atEnd())
        {

            QString ll = in.readLine();
            if (ll[0] == ">")
            {
                QStringList ListaElementowLinii;
                ListaElementowLinii = ll.split(" ",QString::SkipEmptyParts);
                int h = ListaElementowLinii[4].toInt();
                int m = ListaElementowLinii[5].toInt();
                int s = ListaElementowLinii[6].toDouble();
                MyTimeClass CurrentReadTime(h,m,s);
                //porównanie możliwe dzięki przeładowaniu operatora "<"
                if(CurrentReadTime >= StartCalculations && CurrentReadTime <= EndCalculations)
                {
                    wyniki.push_back(CurrentReadTime);
                    continue;
                }
                if(CurrentReadTime > EndCalculations)
                {
                    break;
                }

            }

        }

    }

    return wyniki;
}
