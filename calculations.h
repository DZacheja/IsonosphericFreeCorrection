#ifndef CALCULATIONS_H
#define CALCULATIONS_H
#include "RinexNavigacyjny.h"
#include "BrdcEphemeris.h"
#include <QVector>
#include <QTableWidget>
#include <QTimeEdit>
#include "RinexObserwacyjny.h"
#include <QString>
#include <MyTimeClass.h>
#include <QLabel>

class Calculations
{
private:
    RinexNavigacyjny *NavigationFIle; //obiekt RINEX navigacyjny
    RinexObserwacyjny *ObservationFile; //Obiekt RINEX obserwacyjny
    QList<QString> ZnajdzWspolneSatelity(QVector<QString>&, QVector<QString>&); //satelity wspólne dla obywdu plików
    std::map<QString,double> mWyniki; //mapa wyników

public:
    struct BladObliczen
    {
     QString informacja;
    };
    //Konstruktor
    Calculations(RinexNavigacyjny *nvi, RinexObserwacyjny *obs):
        NavigationFIle(nvi),
        ObservationFile(obs){}
    Calculations();
    void oblicz(std::map<QString,bool>,vector<QString>);
    void ZnajdzEpokiObliczen(QTime,QTime);
    void ZapiszWynikiDoTablicy(QTableWidget*);
    double ObliczPrzemieszczenie();
    void WpiszWspolrzedneOdbiornika(RinexObserwacyjny*, QLabel*,QLabel*,QLabel*); //zapisz wsp do tablicy

};

#endif // CALCULATIONS_H
