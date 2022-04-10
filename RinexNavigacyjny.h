/*
 *KLASA OGÓLNA PLIKÓW RINEX DZIĘKI NIEJ BĘDZIE MOŻNA ZA POMOCĄ FUNKCJI WIRTUALNYCH
 * OKREŚLAĆ DOKŁANE INSTRUKCJE OBLICZANIA WSPÓŁRZĘDNYCH ORAZ PRZESYŁAĆ POPRZEZ
 * REFERENCJE OBIEKT KLASY POCHODNEJ I ODCZYTYWAC Z NIEGO WSP OBLICZONE Z PLIKOW
 * BRODECAST LUB PRECISE
 */
#ifndef RINEX_H
#define RINEX_H
#include <string>
#include <QString>
#include <map>
#include <vector>
#include "MyTimeClass.h"
#include <QList>
#include <math.h>
#include <string>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>

class RinexNavigacyjny
{
protected:
    struct FileDatas // struktura w której znajdują się znalezione wartosci
    {
       MyTimeClass CzasObliczenSatelity;
       long double X;
       long double Y;
       long double Z;
       QString SV;
       int hs;
       int ms;
       double ss;
       long double DtSi;
       long double toc;
       long double tSV;
       long double a0;
       long double a1;
       long double a2;
       long double IODE;
       long double Crs;
       long double del_n;
       long double M0;
       long double Cuc;
       long double e;
       long double Cus;
       long double HalfA;
       long double toe;
       long double Cic;
       long double Omega0;
       long double Cis;
       long double i0;
       long double Crc;
       long double omega;
       long double OmegaDot;
       long double Idot;
       long double TDG;
       long double dReal;
    };
    QString NazwaPliku; //nazwa pliku
    //Data dla wygenerowania nazwy pliku danego obiektu klasy pochodnej
    bool znalazloDate;
    MyTimeClass CzasObliczen;
    long SecoundsOfTheWeek;
    void obliczWsp(FileDatas*); //Obliczenie wspolrzednych
    long CalculateSecoundsOfMonth(int,int,int);

public:
/*
 * Obliczone wsp satelity w zależnosci od wybranej klasy pochodnej
 * dzięki temu że jest to klasa abstrakcyjna, można wysyłać do funkcji dowolną
 * klasę pochodną (BRDC, Mixed).
 */
    double brdcIonoPharam[8];
    std::map<QString,FileDatas> Satellites;
    struct Blad {QString info;}; //obsługa błędów
    RinexNavigacyjny(); //konstruktor
    RinexNavigacyjny(int,int,int,QString&);
    int day;
    int month;
    int year;
    //funkcje z wartościa
    QVector<QString> ListaSatelitow();
    std::vector<long double> WspolrzedneSatelity(QString, long double,bool);

    QString dataPliku(QString);
    QString getNazwaPliku();
    //funckje void
    void ZmienCzasObliczen(MyTimeClass);
    void WybraneSatelity(QList<QString>,std::map<QString,FileDatas*>*);



    //funkcje wirtualne
    virtual ~RinexNavigacyjny(); //destruktor
    virtual void OdejmijSekundyZCzasuObliczenDanegoSatelity(QString,long double) = 0;
    virtual void UzupelnijStrukture()=0;
    virtual void PoprawStruktureSatelity(FileDatas*,QString)=0;
    friend class Calculations;
};

#endif // RINEX_H
