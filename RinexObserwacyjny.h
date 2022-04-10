/*
 *  KLASA ABSTAKYJNA DEFINIUJĄCA GŁOWNE PARAMETRY PLIKÓW OBSERWACYJNYCH TAKIE JAK:
 *  NAZWA PLIKU, VEKTOR PRZEHCOWUJACY MAPY SATELITOW PRZECHOWUJACE WARTOSCI OBSERWACJI
 * *POMYSŁ? KAZDY SATELITA BEDZIE OSOBNYM OBIEKTEM ABY ZAOSZECZĘDZIĆ PAMIĘĆ - OBIEKTY BĘDA TWORZONE
 * TYLKO DLA WYBRANYCH SATELITÓW.
 */
#ifndef RINEXOBS_H
#define RINEXOBS_H
#include <string>
#include <vector>
#include <map>
#include <QString>
#include <QVector>
#include <QList>
#include <QComboBox>
#include "MyTimeClass.h"
using namespace  std;
class RinexObserwacyjny
{
protected:
    struct Sheader
    {
      QString RType;
      double PozX;
      double PozY;
      double PozZ;
      int dzien;
      int mies;
      int rok;
    };
    QString FileName; //plik do danych

    QString SateliteTypeAndNumber = "G07"; //numer i typ satelity
    Sheader header;
    map<QString,vector<pair<QString,long double>>> Frequency; //zapisywanie obcenego rekordu
    map<QString,map<QString, long double>> SatellitesFrequency; //zapisywanie obcenego rekordu
    vector<QString> FrequencyOrder; //Kolejnosc czestotliwosci
    vector<QString> FrequencyOrderGPS; //Kolejnosc czestotliwosci
    vector<QString> FrequencyOrderGalileo; //Kolejnosc czestotliwosci
    virtual void DaneNaglowka()=0;
    MyTimeClass StartCalculations; //zmienne do stworzenia vektora obliczeń
    MyTimeClass EndCalculations; //zmienna do stworzenia vektora obliczeń
    MyTimeClass EpokaObliczen; //Epoka obcenych obliczeń
    long double KombinacjaIonoFreeGPS(QString, QString, long double, long double);
    long double KombinacjaIonoFreeGalileo(QString, QString, long double,long double);
public:

    struct BladG{QString info;}; //informacja o błędzie
    RinexObserwacyjny();
    RinexObserwacyjny(QString);
    RinexObserwacyjny(QString,MyTimeClass);
    virtual ~RinexObserwacyjny();
    virtual void OtworzPlik() =0;
    virtual void WyszukajParametryCzestotliwosi()=0;
    virtual vector<MyTimeClass> PrzedzialGodzinowy(MyTimeClass,MyTimeClass)=0;
    vector<long double> PolozenieOdbiornika();
    QVector<QString> ListaSatelitow();
    map<QString,long double> ZnajdzCzestotliwosc(vector<QString>*, QList<QString>&,bool); //Wyszukanie odpowiednich czestot.
    void WstawCzestotliwosciDoComboBox(QComboBox*,QComboBox*,QComboBox*,QComboBox*); //uzuepłnienie comboBoxa
    struct Blad {QString info;}; //obsługa błędów
    QString getFileDate();
};

#endif // RINEXOBS_H
