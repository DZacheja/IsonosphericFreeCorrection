/*
 * KLASA POCHODNA OD RINEX, POWSTAŁY DWIE TAKIE KLASY - JEDNA DLA PLIKÓW PRECISE DRUGA BRODECAST
 * W TYM PLIKU ZNAJDUJA SIE WSZYSTKIE METODY POZWALAJACE NA WYZNACZENIE WSP SATELITÓW Z PLIKU
 * "*.brdc" NA PODSTAWIE DATY.
 */
#ifndef WSPBRDC_H
#define WSPBRDC_H
#include "RinexNavigacyjny.h"

class BrdcEphemeris final: public RinexNavigacyjny
{
private:
     QString znajdzNazwePliku(int,int,int); //funkcja określa nazwę pliku na podtsawie daty
     void UzupelnijStrukture() override; //znalienie wartosci efemeryd w pliku przypisanym do obiektu
public:
     //KONSTRUKTORY
     BrdcEphemeris();
     BrdcEphemeris(int, int, int,QString&); //konstruktor obiektu na podstawie daty
     BrdcEphemeris(QString, MyTimeClass); //konstruktor dla nazwy pliku
     ~BrdcEphemeris() override; //destruktor

    //FUNKCJE
    void PoprawStruktureSatelity(FileDatas*,QString) override;
    void OdejmijSekundyZCzasuObliczenDanegoSatelity(QString ,long double) override;
    friend class Calculations;
};

#endif // WSPBRDC_H
