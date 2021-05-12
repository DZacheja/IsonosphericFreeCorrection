/*
 * KLASA POCHODNA OD RINEX, POWSTAŁY DWIE TAKIE KLASY - JEDNA DLA PLIKÓW PRECISE DRUGA BRODECAST
 * W TYM PLIKU ZNAJDUJA SIE WSZYSTKIE METODY POZWALAJACE NA WYZNACZENIE WSP SATELITÓW Z PLIKU
 * "*.brdc" NA PODSTAWIE DATY.
 */
#ifndef WSPBRDC_H
#define WSPBRDC_H
#include "RinexNavigacyjny.h"
#include <map>
#include <QString>
#include <armadillo>
#include <vector>
#include <QList>

class BrdcEphemeris final: public RinexNavigacyjny
{
private:
    long SecoundsOfTheWeek;

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
     void obliczWsp(int,int,int, FileDatas&); //Obliczenie wspolrzednych
     std::map<QString,FileDatas> Satellites;
     long CalculateSecoundsOfMonth(int,int,int);
     void znajdzNazwePliku(int,int,int) override; //funkcja określa nazwę pliku na podtsawie daty

public:

    void UzupelnijStrukture(); //znalienie wartosci efemeryd w pliku przypisanym do obiektu
    BrdcEphemeris(int, int, int); //konstruktor obiektu na podstawie daty
    BrdcEphemeris(QString, int, int); //konstruktor dla nazwy pliku
    ~BrdcEphemeris() override; //destruktor
    void WyznaczWspolrzedneSatelitow(int,int,int);
    std::vector<long double> WspolrzedneSatelity(QString, long double); //vektor wspolrzednych z wybranego satelity
    QVector<QString> ListaSatelitow(); //Lista sateltów dla wybranej godziny
    void WybraneSatelity(QList<QString>,std::map<QString,FileDatas>&); //Stworz mape tylko dla wspolnych satelitow
    friend class Calculations;

};

#endif // WSPBRDC_H
