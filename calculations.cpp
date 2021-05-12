#include "calculations.h"
#include <armadillo>
#include <cmath>
#include <math.h>
#include <time.h>
using namespace std;
void Calculations::oblicz()
{
    /*
     * Znalezienie wspólnych satelitów dla obydwu pliku
     * wyselekcjonowanie z nich tylko tych, które maja dane o częstotliwości
     * wyselekcjonowanie "kawałka" mapy do obliczen z pliku navigacyjnego na podstawie
     *      listy satelitów wspolnych ktore maja czestotliowosci
     */

    bool KontynuowaniePetliObliczen = true;
    QVector<QString> sat_z_pliku_obs = ObservationFile.ListaSatelitow();
    QVector<QString> sat_z_pliku_nawi = Brodecast.ListaSatelitow();
    QList<QString> wspolne_satelity;
    wspolne_satelity = ZnajdzWspolneSatelity(sat_z_pliku_nawi, sat_z_pliku_obs);
    map<QString,double> Czestotliwosci = ObservationFile.ZnajdzCzestotliwosc("C1C","C2W",wspolne_satelity);
    map<QString,BrdcEphemeris::FileDatas> Satelity;
    Brodecast.WybraneSatelity(wspolne_satelity,Satelity);

    //MACIERZE WYNIKOWE i wartosci wynikowe//
    arma::mat L;
    arma::mat G;
    arma::mat X;
    /////////////////////
    const double c = 299792458; //predkosc swiatla w m/s



    vector<long double> wspO; //współrzedne z nagłówka
    wspO = ObservationFile.PolozenieOdbiornika();
/*
 * OBLICZENIE wiersza macierzy L dla każdego satelity i zapisanie wyników do mapy
 * Obliczenie wiersza macierzy G dla każdego satelity
 */
do{
        map<QString,long double> D0i; //obliczone odl. z roznucy wsp + korekcja predkosci swiatla
        map<QString,vector<long double>> UiXYZ; //obliczone elemeny macierzy
        L.clear();
        X.clear();
        G.clear();
    for(auto& elem: Satelity)
    {
        // [Numer Satelity | Struktura FileDatas]
        auto& [qstr,fdclass] = elem;

                long double d0;
                std::vector<long double> wspS; //wspołrzedne satelitów
                long double OdczytCxC = Czestotliwosci[qstr];
                wspS = Brodecast.WspolrzedneSatelity(qstr,OdczytCxC);
                long double dX2 = pow((wspS[0]-wspO[0]),2);
                long double dY2 = pow((wspS[1]-wspO[1]),2);
                long double dZ2 = pow((wspS[2]-wspO[2]),2);
                long double odl; // <- ODLEGŁOŚĆ Z WSP SATELITY I WSP Z ODBIORNIKA
                odl = sqrt((dX2 + dY2 + dZ2));
                d0 = odl + (c*fdclass.DtSi);

                D0i.insert({qstr,d0}); //zapis wyniku D0i do mapy

               //ELementy potencjalenj macierzy G
                long double uX,uY,uZ;
                uX = -( (wspS[0]- wspO[0]) ) / d0;
                uY = -( (wspS[1]- wspO[1]) ) / d0;
                uZ = -( (wspS[2]- wspO[2]) ) / d0;
                vector<long double> linia_macierzy_G {uX,uY,uZ};
                UiXYZ.insert({qstr,linia_macierzy_G});
    }
                int WielkoscMacierzy = 1;

                //Stworzenie macierzy L i G
                for (auto& elem : Czestotliwosci)
                {
                    long double Wi;
                    auto&[Numer,Wartosc] = elem;
                    Wi = Wartosc - D0i[Numer];
                    vector<long double> Linia_G = UiXYZ[Numer];
                    L.resize(WielkoscMacierzy,1);
                    int ElementMacierzy = WielkoscMacierzy - 1;

                    L(ElementMacierzy,0) = Wi;
                    G.resize(WielkoscMacierzy,4);
                    G(ElementMacierzy,0) = Linia_G[0];
                    G(ElementMacierzy,1) = Linia_G[1];
                    G(ElementMacierzy,2) = Linia_G[2];
                    G(ElementMacierzy,3) = c;
                    WielkoscMacierzy++;
                }
                arma::mat G_t = G.t();
                X = arma::powmat((G_t*G),-1)*(G_t * L);
                X.print("X: ");
                long double dx,dy,dz;
                dx = X(0,0);
                dy = X(1,0);
                dz = X(2,0);
                wspO[0] += dx;
                wspO[1] += dy;
                wspO[2] += dz;
                if(abs(dx)<0.001 && abs(dy)<0.001 && abs(dz)<0.001)
                    {
                        KontynuowaniePetliObliczen = false;
                    }
                } while (KontynuowaniePetliObliczen);

}

QList<QString> Calculations::ZnajdzWspolneSatelity(QVector<QString>& Navi, QVector<QString>& Obs)
{
    int CzyJest;
    QList<QString> rezults;
    for(int i = 0 ; i < Obs.size(); i++)
    {
        CzyJest = Navi.indexOf(Obs[i]);
        if (CzyJest != -1) {rezults.push_back(Obs[i]); }
    }
    return rezults;
}
