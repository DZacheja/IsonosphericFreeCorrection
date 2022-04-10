#include "RinexObserwacyjny.h"
#include <string>
#include <math.h>
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


map<QString,long double> RinexObserwacyjny::ZnajdzCzestotliwosc(vector<QString> *szukane, QList<QString>& Znalezione, bool poprawka)
{
    //porownaj ze soba plik OBS i NAVI oraz sprawdz czy wystepuja wybrane czestotliwosci dla satelity
    map<QString, long double> rezults;
    std::vector<QString> DoUsuniecia; //elementy do usuniecia z listy
    QString szukana1, szukana2;
    for(auto & value: Znalezione)
    {
        if(value.mid(0,1)=="G")
        {
            szukana1 = (*szukane)[0];
            szukana2 = (*szukane)[1];
        }else if(value.mid(0,1)=="E")
        {
            szukana1 = (*szukane)[2];
            szukana2 = (*szukane)[3];
        }
        map<QString,long double> Item;
        Item = SatellitesFrequency[value];
        long double Wartosc1 = Item[szukana1];

        //jezeli znalazlo wartosc1 oraz wartosc 2 to dodaj satelite do listy
        if(poprawka){
            long double Wartosc2 = Item[szukana2];
            if(Wartosc1 != 0 && Wartosc2 != 0) {
                long double L1L2;
                if(value.mid(0,1)=="G")
                {
                    L1L2 = KombinacjaIonoFreeGPS(szukana1,szukana2,Wartosc1,Wartosc2);
                }else if(value.mid(0,1)=="E")
                {
                    L1L2 = KombinacjaIonoFreeGalileo(szukana1,szukana2,Wartosc1,Wartosc2);
                }
                rezults.insert({value,L1L2});
            }else{DoUsuniecia.push_back(value);} // jeżeli nie znaleziono usun z lity znalezionych
        }else
        {
            if(Wartosc1 != 0) {
                rezults.insert({value,Wartosc1});
            }else{DoUsuniecia.push_back(value);} // jeżeli nie znaleziono usun z lity znalezionych
        }
    }
    /*wywalenie zbednych elementow
     *
     */
    for(auto & elem:DoUsuniecia)
    {
        Znalezione.removeAll(elem);
    }
    return rezults;
}

void RinexObserwacyjny::WstawCzestotliwosciDoComboBox(QComboBox *L1, QComboBox *L2, QComboBox *E1, QComboBox *E2)
{
    //GPS
    L1->clear();
    L2->clear();
    for(int i =2; i < static_cast<int>(FrequencyOrderGPS.size()); i++)
    {
        L1->addItem(FrequencyOrderGPS[i]);
        L2->addItem(FrequencyOrderGPS[i]);
    }
    //Galileo
    E1->clear();
    E2->clear();
    for(int i =2; i < static_cast<int>(FrequencyOrderGalileo.size()); i++)
    {
        E1->addItem(FrequencyOrderGalileo[i]);
        E2->addItem(FrequencyOrderGalileo[i]);
    }
}

QVector<QString> RinexObserwacyjny::ListaSatelitow()
{
    //wstaw liste satelitow do wektora
    QVector<QString> rezults;
    for(auto& elem: SatellitesFrequency)
    {
    rezults.push_back(elem.first);
    }

    return rezults;
}

QString RinexObserwacyjny::getFileDate()
{
        QString results;
        results =  QString::number(header.rok) +
                "-" + QString::number(header.mies) +
                "-" + QString::number(header.dzien);

        return results;
}

long double RinexObserwacyjny::KombinacjaIonoFreeGPS(QString S1, QString S2, long double L1, long double L2)
{
            QString NameL1 = S1.mid(0,2);
            QString NameL2 = S2.mid(0,2);
            long double f1 = pow(1575.420,2);
            long double f2 = pow(1227.600,2);
            long double f5 = pow(1176.450,2);

            long double wynik;
            if (NameL1 == "C1" && NameL2 == "C2")
            {
                long double a1 = f1/(f1-f2);
                long double a2 = -f2/(f1-f2);
                wynik = (a1*L1) + (a2*L2);
            }else if (NameL1 == "C1" && NameL2 == "C5")
            {
                long double a1 = f1/(f1-f5);
                long double a2 = -f5/(f1-f5);
                wynik = (a1*L1) + (a2*L2);
            }
            else if (NameL1 == "C2" && NameL2 == "C5")
            {
                long double a1 = f2/(f2-f5);
                long double a2 = -f5/(f2-f5);
                wynik = (a1*L1) + (a2*L2);
            }else
            {
                BladG error;
                error.info = "Wybrano niepoprawną kombinację Iono-Free!";
                throw  error;
            }

            return wynik;
}


long double RinexObserwacyjny::KombinacjaIonoFreeGalileo(QString S1, QString S2, long double L1, long double L2)
{
            QString NameL1 = S1.mid(0,2);
            QString NameL2 = S2.mid(0,2);
            long double wynik;
            long double f1 = pow(1575.420,2); //E1 -- C1
            long double f5 = pow(1176.450,2); //E5a -- C5
            long double f7 = pow(1207.140,2); //E5b -- C7
            if (NameL1 == "C1" && NameL2 == "C7")
            {
                long double a1 = f1/(f1-f7);
                long double a2 = -f7/(f1-f7);
                wynik = (a1*L1) + (a2*L2);

            }else if (NameL1 == "C1" && NameL2 == "C5")
            {
                long double a1 = f1/(f1-f5);
                long double a2 = -f5/(f1-f5);
                wynik = (a1*L1) + (a2*L2);
            }
            else if (NameL1 == "C7" && NameL2 == "C5")
            {
//                long double a1 = f5/(f5-f7);
//                long double a2 = -f7/(f5-f7);
                long double a1 = f7/(f7-f5);
                long double a2 = -f5/(f7-f5);
                wynik = (a1*L1) + (a2*L2);
            }
            else
            {
                BladG error;
                error.info = "Wybrano niepoprawną kombinację Iono-Free!";
                throw  error;
            }

            return wynik;
}
