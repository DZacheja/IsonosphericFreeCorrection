#include "RinexObserwacyjny.h"
#include <string>
RinexObserwacyjny::RinexObserwacyjny(){}
RinexObserwacyjny::~RinexObserwacyjny(){}

std::vector<long double> RinexObserwacyjny::PolozenieOdbiornika()
{
    //wstaw polozenie odbiornika do wektora z nagłowka pliku
    vector<long double> wsp;
    wsp.push_back(header.PozX);
    wsp.push_back(header.PozY);
    wsp.push_back(header.PozZ);
    return wsp;
}

QVector<QString> RinexObserwacyjny::ListaSatelitow()
{
    //wstaw liste satelitow do wektora
    QVector<QString> rezults;
    for(auto& elem: SatellitesPeriodicity)
    {
        auto& [NumerS,sec] = elem;
                rezults.push_back(NumerS);

    }

    return rezults;
}
map<QString,double> RinexObserwacyjny::ZnajdzCzestotliwosc(QString szukana1, QString szukana2, QList<QString>& Znalezione)
{
            //porownaj ze soba plik OBS i NAVI oraz sprawdz czy wystepuja wybrane czestotliwosci dla satelity
   map<QString, double> rezults;
   for(auto & value: Znalezione)
   {
       map<QString,long double> Item;
       Item = SatellitesPeriodicity[value];
       long double Wartosc1 = Item[szukana1];
       long double Wartosc2 = Item[szukana2];
       //jezeli znalazlo wartosc1 oraz wartosc 2 to dodaj satelite do listy
       if(Wartosc1 != NULL && Wartosc2 != NULL) {
           long double L1L2 = (2.546 * Wartosc1) + (-1.546* Wartosc2);
           rezults.insert({value,L1L2});
       }else{Znalezione.removeAll(value);} // jeżeli nie znaleziono usun z lity znalezionych

   }
   return rezults;
}

void RinexObserwacyjny::WstawCzestotliwosciDoComboBox(QComboBox *L1, QComboBox *L2)
{

//   for(auto& elem:PeriodicityOrder)
//   {
//    L1->addItem(elem);
//    L2->addItem(elem);
//   }
   for(int i =2; i < static_cast<int>(PeriodicityOrder.size()); i++)
   {
       L1->addItem(PeriodicityOrder[i]);
       L2->addItem(PeriodicityOrder[i]);
   }
}
