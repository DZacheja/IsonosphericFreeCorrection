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
class Calculations
{
private:
    RinexNavigacyjny *NavigationFIle;
    RinexObserwacyjny *ObservationFile;
    QList<QString> ZnajdzWspolneSatelity(QVector<QString>&, QVector<QString>&);
    QTimeEdit   *StartTime;
    QTimeEdit   *EndTime;
    std::vector<MyTimeClass> EpokiObliczen;
    std::map<QString,double> mWyniki;

public:
    Calculations(RinexNavigacyjny *nvi, RinexObserwacyjny *obs):
        NavigationFIle(nvi),
        ObservationFile(obs){}
    Calculations(RinexNavigacyjny&,QVector<QTime>);
    void oblicz();
    void ZnajdzEpokiObliczen(QTime,QTime);
    void ZapiszWynikiDoTablicy(QTableWidget*);
    double ObliczPrzemieszczenie();
    Calculations(QTableWidget*);
};

#endif // CALCULATIONS_H
