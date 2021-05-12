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
RinexOVer3::RinexOVer3(QString ff, QString sat)
{
    FileName = ff;
    SateliteTypeAndNumber = sat;

}

void RinexOVer3::OtworzPlik()
{
    vector<pair<QString,long double>> Para = Periodicity["00:00:0.0"];
    map<QString,long double> OK;
    long double test;
    QString cc = "C1C";
    for (auto& ass : Para)
    {
        auto [frs,sec] = ass;
                if(frs == cc)
                {
                    test = sec;
                    break;
                }
    }



}

void RinexOVer3::WyszukajParametryCzestotliwosi()
{
    // tstowy czas
    int HH, MM;
    double SS;
    HH = 0;
    MM = 7;
    SS = 30.00;
    //testowa sciezka
    QString filenn = testowaSciezka + "\\" + testowaNazwaPliku;
    QFile file(filenn);

    //wzorując się na Aaron Boda ..
    const QString EndOfHeaderT = "END OF HEADER";

    bool inHeader = true;

  if (!file.open(QFile::ReadOnly|QFile::Text))
  {
        QMessageBox msgbox;
        msgbox.setText("Błąd otwarcia pliku!");
        msgbox.exec();
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
                 wartosc = NULL;
                }
                else{
                 wartosc = stold(ElementyLinii[i].toStdString());
                }
                CurrentLine.insert({item,wartosc});
            }

            SatellitesPeriodicity.insert({NumerSatelity,CurrentLine});

        }

        if (!NewTimeData){continue;}

        int isThatSatelite = linia.indexOf(SateliteTypeAndNumber);

        if(isThatSatelite >= 0)
        {
/*
            int poczatek = 4;
            double IleIteracji;
            IleIteracji = (linia.length() - 5)/16;

            for(int i=0; i <= ceil(IleIteracji); i++)
            {
                ElementyLinii.push_back(linia.mid(poczatek,16).trimmed());
                poczatek += 16;
            }
*/
            map<QString,long double> CurrentLine;

            for(int i = 0; i < ElementyLinii.count(); i++)
            {
                QString item = PeriodicityOrder[i+2];
                long double wartosc;
                if (ElementyLinii[i]=="")
                {
                 wartosc = NULL;
                }
                else{
                 wartosc = stold(ElementyLinii[i].toStdString());
                }
                CurrentLine.insert(pair<QString,long double>(item,wartosc));
            }
            vector<pair<QString,long double>> vect;
            for (auto& it : CurrentLine)
            {
                auto& [str,dbl] = it;
                vect.push_back(pair<QString,long double>(str,dbl));
            }
                        pair<QString,vector<pair<QString,long double>>> para{time,vect};
            Periodicity.insert(para);
            NewTimeData = false;
        }
    }
  }
}

void RinexOVer3::InHeader(){
    QString filenn = testowaSciezka + "\\" + testowaNazwaPliku;
    QFile file(filenn);
    QChar LSatSign = SateliteTypeAndNumber[0];
    bool FounAll = false;

    //wzorując się na Aaron Boda ..
  if (!file.open(QFile::ReadOnly|QFile::Text))
  {
        QMessageBox msgbox;
        msgbox.setText("Błąd otwarcia pliku!");
        msgbox.exec();
  }
  else
  {

    const QString EndOfHeaderT = "END OF HEADER";

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
    int isXYZ = ll.indexOf("APPROX POSITION XYZ");
    int isSys = ll.indexOf("SYS / # / OBS TYPES");
    if (isXYZ > 0){
        QSlist = ll.split(" ",QString::SkipEmptyParts);
        header.PozX = QSlist[0].toDouble();
        header.PozY = QSlist[1].toDouble();
        header.PozZ = QSlist[2].toDouble();
    }

/* Jeżeli pierwszy znak linii jest spacją oznacza, że w dalszym ciągu dodajemy
* częstotliwośći satelity z poprzedniego wiersza. Jeżeli pierwszy znak nie jest
* spacją oznacza, że definiujemy nową listę częstotliwości więc kontener z obecnymi
* wartościami należy "dołożyć" do kontenera głównego w klasie (PeriodicityOrder)
* a następnie go wyczyścić w celu przygotowania do nowych danych.
*/

    else if(isSys >0)
    {
        QChar SPC = ll[0];
        if(SPC == LSatSign) {FounAll = true;}


            if(!(SPC.isSpace()))
            {
                if(!(PeriodicityOrder.empty()))
                {
                    if(FounAll){break;}
                }

            }

            ll.remove("SYS / # / OBS TYPES");
            QSlist = ll.split(" ",QString::SkipEmptyParts); //utworzenie tablicy z linii, z pominieciem spacji
            //dodanie wszystkich elementow tablicy do kontenera

            for(auto elem : QSlist)
            {
                PeriodicityOrder.push_back(elem);
            }
}//elseif
}//while
}//1.else - plik ok
}//funckja

