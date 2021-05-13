#ifndef CALCULATIONS_H
#define CALCULATIONS_H
#include "RinexOVer3.h"
#include "BrdcEphemeris.h"
#include <QVector>
class Calculations
{
private:
    BrdcEphemeris Brodecast;
    RinexOVer3 ObservationFile;
    QList<QString> ZnajdzWspolneSatelity(QVector<QString>&, QVector<QString>&);
public:
    Calculations(BrdcEphemeris & brd, RinexOVer3 & rnx):
        Brodecast(brd),
        ObservationFile(rnx){}
    Calculations(RinexOVer3&,QVector<QTime>);
    void oblicz();
    void ZnajdzEpokiObliczen(QTime,QTime);
};

#endif // CALCULATIONS_H
