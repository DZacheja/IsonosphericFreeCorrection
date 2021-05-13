#ifndef RINEXOBS3_H
#define RINEXOBS3_H
#include "RinexObserwacyjny.h"
#include <vector>
#include <QString>
#include <QStringList>
#include "MyTimeClass.h"
class RinexOVer3 final : public RinexObserwacyjny
{
private:
    QString testowaNazwaPliku = "WRO_OKROJONY.rnx";
    QString testowaSciezka = R"(E:\WAT\MAGISTERKA\PROGRAM\IonoFreeCorrection\DANE)";
    MyTimeClass PoczatekObserwacjiPliku;
    MyTimeClass KoniecObserwacjiPliku;
public:
    RinexOVer3();
    RinexOVer3(QString);

    void OtworzPlik() override;
    void WyszukajParametryCzestotliwosi() override;
    void InHeader() override; //odczytanie informacji z nagłówka i zapisanie koljnosci zapisywanych czestotliwosci
};

#endif // RINEXOBS3_H
