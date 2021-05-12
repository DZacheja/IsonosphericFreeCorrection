#include "RinexObserwacyjny.h"
#include <string>
RinexObserwacyjny::RinexObserwacyjny(){}
RinexObserwacyjny::~RinexObserwacyjny(){}

std::vector<long double> RinexObserwacyjny::PolozenieOdbiornika()
{
    vector<long double> wsp;
    wsp.push_back(header.PozX);
    wsp.push_back(header.PozY);
    wsp.push_back(header.PozZ);
    return wsp;
}

QVector<QString> RinexObserwacyjny::ListaSatelitow()
{
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
   map<QString, double> rezults;
   for(auto & value: Znalezione)
   {
       map<QString,long double> Item;
       Item = SatellitesPeriodicity[value];
       long double Wartosc1 = Item[szukana1];
       long double Wartosc2 = Item[szukana2];
       if(Wartosc1 != NULL && Wartosc2 != NULL) {
           long double L1L2 = (2.546 * Wartosc1) + (-1.546* Wartosc2);
           rezults.insert({value,L1L2});
       }else{Znalezione.removeAll(value);}

   }
   return rezults;
}
