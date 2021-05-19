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
#include "MyTimeClass.h"

class BrdcEphemeris final: public RinexNavigacyjny
{
private:
    long SecoundsOfTheWeek;
     void obliczWsp(int,int,int, FileDatas*); //Obliczenie wspolrzednych
     std::map<QString,FileDatas> Satellites;
     long CalculateSecoundsOfMonth(int,int,int);
     QString znajdzNazwePliku(int,int,int) override; //funkcja określa nazwę pliku na podtsawie daty
     MyTimeClass CzasObliczen;
public:

    void UzupelnijStrukture(); //znalienie wartosci efemeryd w pliku przypisanym do obiektu
    BrdcEphemeris(int, int, int,QString&); //konstruktor obiektu na podstawie daty
    BrdcEphemeris(QString, MyTimeClass); //konstruktor dla nazwy pliku
    ~BrdcEphemeris() override; //destruktor
    void WyznaczWspolrzedneSatelitow(int,int,double);
    std::vector<long double> WspolrzedneSatelity(QString, long double, bool) override; //vektor wspolrzednych z wybranego satelity
    QVector<QString> ListaSatelitow() override; //Lista sateltów dla wybranej godziny
    void WybraneSatelity(QList<QString>,std::map<QString,FileDatas>&) override; //Stworz mape tylko dla wspolnych satelitow
    friend class Calculations;

};

#endif // WSPBRDC_H
