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
class RinexNavigacyjny
{
protected:
    QString NazwaPliku; //nazwa pliku
    //Data dla wygenerowania nazwy pliku danego obiektu klasy pochodnej
    int day;
    int month;
    int year;
    int hour;
    int minute;
    int secounds;

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
};

#endif // RINEX_H
