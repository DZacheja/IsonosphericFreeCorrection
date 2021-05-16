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
class RinexNavigacyjny
{
protected:
    struct FileDatas // struktura w której znajdują się znalezione wartosci
    {
       long double X;
       long double Y;
       long double Z;
       int SV;
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
    };
    QString NazwaPliku; //nazwa pliku
    //Data dla wygenerowania nazwy pliku danego obiektu klasy pochodnej
    int day;
    int month;
    int year;
    MyTimeClass CzasObliczen;
public:
/*
 * Obliczone wsp satelity w zależnosci od wybranej klasy pochodnej
 * dzięki temu że jest to klasa abstrakcyjna, można wysyłać do funkcji dowolną
 * klasę pochodną (BRDC, PRECISE).
 */
    double X;
    double Y;
    double Z;
    RinexNavigacyjny(); //konstruktor

   // virtual void wczytajDane() =0;
    virtual void znajdzNazwePliku(int,int,int) =0; //Funkcja wirtualna do szukania nazwy pliku
    virtual ~RinexNavigacyjny(); //destruktor
    virtual QVector<QString> ListaSatelitow() =0;
    virtual void WybraneSatelity(QList<QString>,std::map<QString,FileDatas>&)=0;
    virtual std::vector<long double> WspolrzedneSatelity(QString, long double)=0;
    void ZmienCzasObliczen(MyTimeClass);
};

#endif // RINEX_H
