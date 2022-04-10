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
    double MaxElevation=0;
public:
    struct BladObliczen
    {
     QString informacja;
    };
    struct BladGruby
    {
     QString info;
    };
    //wyniki
        map<QString,double> WartPoprRelat;
        map<QString,double> WartTropo;
        map<QString,double> WartIono;
        map<int,QString> WartDxDyDz;
        map<QString,double> WartDSV;
        map<QString,double> WartPoprCzasu;
        map<QString,std::vector<long double>> WspSatelitow;
        map<QString,std::vector<long double>> WspSatelitowBezKorekcji;
        map<QString,double> PoczCxC; //częstotliwość po kombinacji iono-free lub tylko wybrana
        map<QString,double> KoncCxC; //częstotliwość po uwzględnieniu metody klobuchara
        map<QString,QString> PoprawionyCzas;
        map<QString,QString> AzymutSatelitow;
        map<QString,QString> KatElewacjiSatelitow;
        map<QString,QString> UwagiDoSatelitow;
        map<QString,double> P1CxC; //zapisz odczyt czestotliwosci P1
        map<QString,QString> P2CxC; //jeżeli iono-free zpaisz odczyt czestotliwosci P2

    //Konstruktor
    Calculations(RinexNavigacyjny *nvi, RinexObserwacyjny *obs):
        NavigationFIle(nvi),
        ObservationFile(obs){}
    Calculations();
    void XYZ_BLH(long double,long double,long double,double*,double*,double*);
    void ionocorr(double *ion, double, double, double, double, double, double*);
    void oblicz(std::map<QString,bool>,vector<QString>);
    void oblicz_popr(std::map<QString,bool>,vector<QString>);
    void ZnajdzEpokiObliczen(QTime,QTime);
    void ZapiszWynikiDoTablicy(QTableWidget*);
    double ObliczPrzemieszczenie();
    void WpiszWspolrzedneOdbiornika(RinexObserwacyjny*, QLabel*,QLabel*,QLabel*,QLabel*,QLabel*,QLabel*); //zapisz wsp do tablicy
    void calcAzEl(vector<long double> Xs, vector<long double> Xu, double*, double*, bool*);
    void UsunDanePlikow();
    void PrzeliczNaWGS84(long double,long double,long double,double*,double*,double*);
    void UstawMaksymalnyKatElewacji(int);
};

#endif // CALCULATIONS_H
