#ifndef CALCULATIONS_H
#define CALCULATIONS_H
#include "RinexOVer3.h"
#include "BrdcEphemeris.h"

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
    void oblicz();
};

#endif // CALCULATIONS_H
