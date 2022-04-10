#include "calculations.h"
#include <armadillo>
#include <cmath>
#include <math.h>
#include <time.h>

using namespace std;
Calculations::Calculations(){}


/*
 * Znalezienie satelitów do obliczeń
 */
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

/*
 * Zapisanie rezultatów
 */
void Calculations::ZapiszWynikiDoTablicy(QTableWidget *tablica)
{
        int wiersz = tablica->rowCount();
        double B,L,H;
        PrzeliczNaWGS84(mWyniki["Popr_X"],mWyniki["Popr_Y"],mWyniki["Popr_Z"],&B,&L,&H);
        wiersz--;
        tablica->setItem(wiersz,1,new QTableWidgetItem(QString::number(mWyniki["Popr_X"],'f',2)));
        tablica->setItem(wiersz,2,new QTableWidgetItem(QString::number(mWyniki["Popr_Y"],'f',2)));
        tablica->setItem(wiersz,3,new QTableWidgetItem(QString::number(mWyniki["Popr_Z"],'f',2)));
        QString odleglosc = QString::number(this->ObliczPrzemieszczenie(),'f',2);
        tablica->setItem(wiersz,4,new QTableWidgetItem(odleglosc));
        tablica->setItem(wiersz,5,new QTableWidgetItem(QString::number(B,'f',10)));
        tablica->setItem(wiersz,6,new QTableWidgetItem(QString::number(L,'f',10)));
        tablica->setItem(wiersz,7,new QTableWidgetItem(QString::number(H,'f',10)));
}

/*
 * Obliczenie przemieszczenia względem początku
 */
double Calculations::ObliczPrzemieszczenie()
{
        double wynik;
        double dx,dy,dz;
        dx = mWyniki["Pocz_X"] - mWyniki["Popr_X"];
        dy = mWyniki["Pocz_Y"] - mWyniki["Popr_Y"];
        dz = mWyniki["Pocz_Z"] - mWyniki["Popr_Z"];
        wynik = sqrt(pow(dx,2)+pow(dy,2)+pow(dz,2));
        return wynik;
}

/*
 * Wpisanie wsp odbiornika do tablicy
 */
void Calculations::WpiszWspolrzedneOdbiornika(RinexObserwacyjny *rnx, QLabel *X, QLabel *Y, QLabel *Z, QLabel *B, QLabel *L, QLabel *H)
{
    std::vector<long double> wspOdbiornika;
    wspOdbiornika = rnx->PolozenieOdbiornika();
    double Bx,Lx,Hx;
    PrzeliczNaWGS84(wspOdbiornika[0],wspOdbiornika[1],wspOdbiornika[2],&Bx,&Lx,&Hx);
    X->setText(QString::number(wspOdbiornika[0],'f',3));
    Y->setText(QString::number(wspOdbiornika[1],'f',3));
    Z->setText(QString::number(wspOdbiornika[2],'f',3));
    B->setText(QString::number(Bx,'f',10));
    L->setText(QString::number(Lx,'f',10));
    H->setText(QString::number(Hx,'f',3));

}
//ustaw maksymalny kat elewacji wybrany przez uzytkownika
void Calculations::UstawMaksymalnyKatElewacji(int kat)
{
    MaxElevation = static_cast<double>(kat);
}

//Wykonywanie obliczeń
void Calculations::oblicz_popr(map<QString,bool>poprawki,vector<QString> P1P2)
{
    /*
     * Znalezienie wspólnych satelitów dla obydwu pliku
     * wyselekcjonowanie z nich tylko tych, które maja dane o częstotliwości
     * wyselekcjonowanie "kawałka" mapy do obliczen z pliku navigacyjnego na podstawie
     *      listy satelitów wspolnych ktore maja czestotliowosci
     *
     *      IN: informacje jakie porpawki uwzględnić
     *      IN: wybrane częstotliwości obliczeń
     *      Out: poprawione współrzędne
     */

    // sprawdzenie rodzaju pliku
    QString naviTypBR = NavigationFIle->NazwaPliku.right(12);
    naviTypBR = naviTypBR.left(4);

    long double dTr=0; // - poprawka odbiornika wyliczana w trakcie iteracji
    bool KontynuowaniePetliObliczen = true;
    QVector<QString> sat_z_pliku_obs = ObservationFile->ListaSatelitow();
    QVector<QString> sat_z_pliku_nawi = NavigationFIle->ListaSatelitow();
    QList<QString> wspolne_satelity;
    wspolne_satelity = ZnajdzWspolneSatelity(sat_z_pliku_nawi, sat_z_pliku_obs);
    map<QString,long double> Czestotliwosci;
    map<QString,long double> CzestotliwosciP1;
    map<QString,long double> CzestotliwosciP2;
    vector<QString> P2P1 = {P1P2[1],P1P2[0],P1P2[3],P1P2[2]};
    //sprawdzenie poprawnego wybrania czestotliwosci
    try{
    Czestotliwosci = ObservationFile->ZnajdzCzestotliwosc(&P1P2,wspolne_satelity,poprawki["iono_free"]);
    if(poprawki["iono_free"]){
    CzestotliwosciP1 = ObservationFile->ZnajdzCzestotliwosc(&P1P2,wspolne_satelity,false);
    CzestotliwosciP2 = ObservationFile->ZnajdzCzestotliwosc(&P2P1,wspolne_satelity,false);
    }
    }
    catch(RinexObserwacyjny::BladG er)
    {
        BladGruby erO;
        erO.info = er.info;
        throw  erO;
    }
    map<QString,BrdcEphemeris::FileDatas*> Satelity;
    NavigationFIle->WybraneSatelity(wspolne_satelity, &Satelity);
    if( Satelity.size() < 4)
    {
        BladObliczen err;
        err.informacja = "Zbyt mało obserwacji do ułożenia układu!";
        throw err;
    }

    //MACIERZE WYNIKOWE i wartosci wynikowe//
    arma::mat L;
    arma::mat G;
    arma::mat X;
    /////////////////////
    const double c = 299792458; //predkosc swiatla w m/s


    vector<QString> DoUsuniecia; // satelity nisko zawieszone nad horyzontem
    vector<long double> wspO; //współrzedne z nagłówka
    vector<long double> PoczwspO; //współrzedne z nagłówka
    wspO = ObservationFile->PolozenieOdbiornika();

    PoczwspO = wspO;
    /*
 * OBLICZENIE wiersza macierzy L dla każdego satelity i zapisanie wyników do mapy
 * Obliczenie wiersza macierzy G dla każdego satelity
 */
    int petlaObliczen = 1;
    bool PierwszaIteracja = true;
    bool DrugaIteracja = false;
    long double DtSi;
    do{
        map<QString,long double> ro0; //obliczone odl. z roznucy wsp + korekcja predkosci swiatla
        map<QString,long double> D0i;
        map<QString,vector<long double>> UiXYZ; //obliczone elemeny macierzy
        L.clear();
        X.clear();
        G.clear();
        ro0.clear();
        D0i.clear();
        UiXYZ.clear();
        for(auto& elem: Satelity)
        {
            // [Numer Satelity | Struktura FileDatas]
            auto& [qstr,fdclass] = elem;

                    std::vector<long double> wspS; //wspołrzedne satelitów + DtSi
                    long double OdczytCxC = Czestotliwosci[qstr];

                    if (PierwszaIteracja){
                //zapisz wyniki///////////////////////////
                    PoczCxC.insert({qstr,OdczytCxC});//
                    KoncCxC.insert({qstr,OdczytCxC});//
                    UwagiDoSatelitow.insert({qstr,"Brak uwag."});
                ////////////zapis częstotliwości z pliku////////////////////
                    if (poprawki["iono_free"])
                    {
                        P1CxC.insert({qstr,CzestotliwosciP1[qstr]});
                        P2CxC.insert({qstr,QString::number(CzestotliwosciP2[qstr],'f',4)});
                    }else{
                        P1CxC.insert({qstr,CzestotliwosciP1[qstr]});
                        P2CxC.insert({qstr,"Nie wyszukiwano."});
                    }

                    long double PoprawkaCzasuObliczen;
                    PoprawkaCzasuObliczen = OdczytCxC/c;
                    //pierwsze obliczenie wspolrzednych

                    wspS = NavigationFIle->WspolrzedneSatelity(qstr,OdczytCxC,false);

                    //zapisz wspolrzedne bez korekcji do pliku///
                    WspSatelitowBezKorekcji.insert({qstr,wspS});
                    /////////////////////////////////////////////

                    //czy uwzględnić poprawkę relatywistyczną??
                    if(!poprawki["relativistic"])
                    {
                        fdclass->dReal = 0;
                        fdclass->TDG = 0;
                    }
//                    long double KolejnaPoprawkaCzasu;

                    PoprawkaCzasuObliczen = PoprawkaCzasuObliczen +fdclass->DtSi - fdclass->TDG + fdclass->dReal;
                    NavigationFIle->OdejmijSekundyZCzasuObliczenDanegoSatelity(qstr,PoprawkaCzasuObliczen);

                    //obliczeie wspolrzednych poprawionych
                    wspS = NavigationFIle->WspolrzedneSatelity(qstr,OdczytCxC,poprawki["eart_move"]);

                    ////////////poprawiony czas//////////////////////////
                    QString pTime;
                    pTime = fdclass->CzasObliczenSatelity.GetTimeQString();
                    PoprawionyCzas.insert({qstr,pTime});
                    //////////////////zmienne, nie adresy/////////////////
                    double dtsi = fdclass->DtSi;
                    double dreal = fdclass->dReal;
                    ///////////////zapisz wyniki////////////////////////
                    WartPoprCzasu.insert({qstr,PoprawkaCzasuObliczen});
                    WartDSV.insert({qstr,dtsi});
                    WartPoprRelat.insert({qstr,dreal});//
                    //zapisz współrzędne do mapy aby pominąć obliczenia
                    WspSatelitow.insert({qstr,wspS});
                    ////////////////////////////////////////////////////


                    if(poprawki["lowElevation"]){
                    /// usuń nisko zawieszone zatelity///
                    double Azymut;
                    double KatElewacji;
                    bool DaneOK;
                    calcAzEl(wspS,wspO,&Azymut,&KatElewacji,&DaneOK);

                    //zapisz azymut i kat elewacji
                    const long double pi_const = 3.14159265359;

                    //sprawdz kat elewacji, jeżeli jest za mały usuń dane
                    double eleSt = KatElewacji*(180/pi_const);
                    //usuwanie odznaczone - generowało większe błędy?
                    if (eleSt <= MaxElevation || !DaneOK)
                    {
                        DoUsuniecia.push_back(elem.first);
                        UwagiDoSatelitow[qstr] = "Satelita pominięty z obliczeń. Zbyt mały kąt elewacji.";
                        KatElewacjiSatelitow[qstr] = QString::number(KatElewacji,'f',6);
                    }
                    }
                    ////////////////////////////////////////////////////////////

            }
                    wspS = WspSatelitow[qstr];
                    if(DrugaIteracja)
                    {
                      //poprawka troposferyczna
                    double Azymut;
                    double KatElewacji;
                    bool DaneDoPoprawkiTropo;
                    calcAzEl(wspS,wspO,&Azymut,&KatElewacji,&DaneDoPoprawkiTropo);

                    //zapisz azymut i kat elewacji
                    const long double pi_const = 3.14159265359;

                    QString RoboczyDoAziEle;
                    RoboczyDoAziEle = QString::number(Azymut*(180/pi_const),'f',6) + "°"; //zamiana rad na st i dodanie znaku st
                    AzymutSatelitow.insert({qstr,RoboczyDoAziEle});
                    RoboczyDoAziEle = QString::number(KatElewacji*(180/pi_const),'f',6) + "°"; //zamiana rad na sr i dodanie znaku st
                    KatElewacjiSatelitow.insert({qstr,RoboczyDoAziEle});
                    ////////////////////////////
                    long double dTropo = 0;
                    double dIono = 0;

                    //czy uwzględnić poprakę troposferyczną
                    if(poprawki["tropo"]){
                    dTropo = 2.312/ sin(sqrt((KatElewacji*KatElewacji)+1.904e-3))+0.084/(sin(sqrt((KatElewacji*KatElewacji)+0.6854e-3)));
                    }
//                    //////////////zapis wynikow///////////
                        WartTropo.insert({qstr,dTropo});
                        WartTropo.insert({qstr,dIono});///
//                    /////////////////////////////////////

                    //poprawka jonosferyczna - tylko jeżeli nie ma kombinacji oraz jest plik z CDDIS (brdc)
                    if (naviTypBR == "brdc" && !poprawki["iono_free"] && poprawki["std_iono"]){
                        double dIonoSec;
                        double B,L,H,Tr;
                        double *wskDIono = &dIonoSec;
                        double *wB = &B;
                        double *wL = &L;
                        double *wH = &H;
                    XYZ_BLH(wspO[0],wspO[1],wspO[2],wB,wL,wH);
                    Tr = (fdclass->CzasObliczenSatelity.getHour()*3600) +
                            fdclass->CzasObliczenSatelity.getMinutes()*60 +
                            fdclass->CzasObliczenSatelity.getSecounds() +
                            NavigationFIle->SecoundsOfTheWeek;
                    ionocorr(NavigationFIle->brdcIonoPharam,B,L,Azymut,KatElewacji,Tr,wskDIono);
                    dIono = dIonoSec*c;

//                    ////// podmien wyniki//////
                        WartIono[qstr] = dIono;
//                    ///////////////////////////
                    }
                    //poprawa wartosci czestotliwosci
                      long double dTclock = fdclass->dReal - fdclass->TDG;
                      long double poprC = OdczytCxC + (c*dTclock) -dTropo + dTr*c -dIono;
                      Czestotliwosci[qstr] = poprC;

//                      // podmien wyniki/////
                          KoncCxC[qstr] = Czestotliwosci[qstr];
//                      /////////////////////
                    }

                    if(!PierwszaIteracja)
                    {
                      Czestotliwosci[qstr] = Czestotliwosci[qstr] + dTr*c;
                      //// podmien wyniki/////
                      KoncCxC[qstr] = Czestotliwosci[qstr];
                      ///////////////////////
                    }
                    long double dX2 = (wspS[0]-wspO[0]);
                    long double dY2 = (wspS[1]-wspO[1]);
                    long double dZ2 = (wspS[2]-wspO[2]);
                    dX2 = pow(dX2,2);
                    dY2 = pow(dY2,2);
                    dZ2 = pow(dZ2,2);
                    long double odl; // <- ODLEGŁOŚĆ Z WSP SATELITY I WSP Z ODBIORNIKA
                    odl = sqrt((dX2 + dY2 + dZ2));
                    DtSi = wspS[3];

                    ro0.insert({qstr,odl}); //zapis wyniku D0i do mapy
                    D0i.insert({qstr,DtSi});

            //ELementy potencjalenj macierzy G
            long double uX,uY,uZ;
            uX = -( (wspS[0]- wspO[0]) ) / odl;
            uY = -( (wspS[1]- wspO[1]) ) / odl;
            uZ = -( (wspS[2]- wspO[2]) ) / odl;
            vector<long double> linia_macierzy_G {uX,uY,uZ};
            UiXYZ.insert({qstr,linia_macierzy_G});
        }

//Usuwanie błędnych satelitów - pominięto
        if(poprawki["lowElevation"]){
        for (auto & elem:DoUsuniecia)
        {
            Satelity.erase(elem);
            Czestotliwosci.erase(elem);
            if( Satelity.size() < 4)
            {
                BladObliczen err;
                err.informacja = "Zbyt mało obserwacji po wyeliminowaniu nisko zawieszonych satelitów!";
                throw err;
            }
        }
        DoUsuniecia.clear();
        }

        int WielkoscMacierzy = 1;

        //Stworzenie macierzy L i G
        for (auto& elem : Czestotliwosci)
        {
            long double Wi;
            auto&[Numer,Wartosc] = elem; //Numer = Numer satelity, Wartosc = odczyt pseudoodległości (skorygowany)
                    Wi =  Wartosc - ro0[Numer] + c*(dTr + D0i[Numer]);
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
                    dTr = X(3,0);

                    wspO[0] += dx;
                    wspO[1] += dy;
                    wspO[2] += dz;


                    ////zapis wynikow posrednich////
                    //Zapisywanie wynikow pośrednich//
                    QString dxdydz;
                    dxdydz =QString::number(petlaObliczen) + ";" +
                            QString::number(dx,'f',4) + ";" +
                            QString::number(dy,'f',4) + ";" +
                            QString::number(dz,'f',4) + ";" +
                            QString::number(dTr,'e',3);


                    WartDxDyDz.insert({petlaObliczen,dxdydz});
                ////////////////////////////////////////////////

                    //poprawienie współrzędnych odbiornika o dx,dy,dz
                    if(!PierwszaIteracja)
                    {

                    if(abs(dx)<0.001 && abs(dy)<0.001 && abs(dz)<0.001)
            {
                KontynuowaniePetliObliczen = false;
                mWyniki.insert({"Popr_X",wspO[0]});
                mWyniki.insert({"Popr_Y",wspO[1]});
                mWyniki.insert({"Popr_Z",wspO[2]});
                mWyniki.insert({"Pocz_X",PoczwspO[0]});
                mWyniki.insert({"Pocz_Y",PoczwspO[1]});
                mWyniki.insert({"Pocz_Z",PoczwspO[2]});
            }
            /*ponieważ część danych, z niewiadomych mi przyczyn wykonuje nieskończoną pętlę
             * postanowiłem zrobić "stoper" po 20 iteracjach
             */
            petlaObliczen ++;
            if (petlaObliczen > 200)
            {
                BladObliczen info;
                info.informacja = "Zbyt duzo petli obliczen!";
                throw info;

            }
            }
            if (PierwszaIteracja)
            {
                DrugaIteracja = true;
            }else{DrugaIteracja = false;}
            PierwszaIteracja = false;

        } while (KontynuowaniePetliObliczen);

}

    /*****************************************************************************
    * Procedura wyznaczenia azymutu i wysokości satelity
    * dla obliczenie poprawek iono i tropo sferycznych
    * IN: Xs – macierz położenia satelity czyli X,Y,Z satelity
    * IN: Xu – macierz położenia odbiornika (przybliżona) czyli X,Y,Z użytkownika
    ******************************************************************************/
    void Calculations::calcAzEl(vector<long double> Xs, vector<long double> Xu, double *Az, double *El, bool *stat)
    {
        /* IN: 	współrzędne SV ECEF: XYZ*/
        /* IN: 	położenie użytkownika: XYZ*/
        /* OUT: azymut [rad]*/
        /* OUT: wysokość - elevation [rad]*/
        /* OUT: poprawność obliczeń: stat = true*/
        long double R, p, x, y, z, s;
        long double e[3][3];
        long i, k;
        long double d[3];
        const long double pi_const = 3.14159265359;

        x = Xu[0];
        y = Xu[1];
        z = Xu[2];
        p = sqrt(x * x + y * y);
        if (p == 0.0)
                *stat = false;
        else
                *stat = true;

        if (!*stat) return;

        R = sqrt(x * x + y * y + z * z);
        e[0][0] = -(y / p);
        e[0][1] = x / p;
        e[0][2] = 0.0;
        e[1][0] = -(x * z / (p * R));
        e[1][1] = -(y * z / (p * R));
        e[1][2] = p / R;
        e[2][0] = x / R;
        e[2][1] = y / R;
        e[2][2] = z / R;

        /* mnożenie macierzy przez wektor użytkownika */
        for (k = 0; k <= 2; k++)
        {
                d[k] = 0.0;
                for (i = 0; i <= 2; i++)
            {
                    d[k] += (Xs[i] - Xu[i]) * e[k][i];
            }
        }
        s = d[2] / sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
        if (s == 1.0)
        {
                *El = 0.5 * pi_const;
        }
        else
        {
                *El = atan(s / sqrt(1.0 - s * s));
        }

        if (d[1] == 0.0 && d[0] > 0.0)
        {
                *Az = 0.5 * pi_const;
        }
        else if (d[1] == 0.0 && d[0] < 0.0)
        {
                *Az = 1.5 * pi_const;
        }
        else
        {
            *Az = atan(d[0] / d[1]);
            if (d[1] < 0.0)
            {
                    *Az += pi_const;
            }
            else if (d[1] > 0.0 && d[0] < 0.0)
            {
                    *Az += 2.0 * pi_const;
            }

        }
    }  /*koniec calcAzEl*/


void Calculations::ionocorr
(
    double *ion, double Latu, double Lonu, double Az, double El, double Ttr, double *dTiono
)
{
    /*ion 		współczynniki korekcji ionosferycznej ze zbiory nawigacyjnego RINEX lub danych nav */
    /*Latu		szerokość geograficzne użytkownika - latitude [rad]*/
    /*Lonu		d³ugoœæ geograficzna użytkownika - longitude [rad]*/
    /*Az		azymut SV [rad] wyznaczony w proc calcAzEl*/
    /*El		wysokość SV [rad] wyznaczona w proc calcAzEl*/
    /*Ttr		czas transmisji sygna³u z SV [sec]*/
    /*dTiono	opó¿nienie Jonosrefyczne [sec]*/
    double phi, Lati, Loni, Latm, T, F_, x, per, amp, a0, a1, a2, a3, b0, b1, b2, b3;

    const long double pi_const = 3.14159265359;
    /*dla jasności co jest co w tablicy*/
    a0 = ion[0];
    a1 = ion[1];
    a2 = ion[2];
    a3 = ion[3];
    b0 = ion[4];
    b1 = ion[5];
    b2 = ion[6];
    b3 = ion[7];

    /*zamiana z radianów na cykle - semicircles*/
    Latu /= pi_const;
    Lonu /= pi_const;
    Az /= pi_const;
    El /= pi_const;

    /*obliczenia*/
    phi = 0.0137 / (El + 0.11) - 0.022;
    Lati = Latu + phi * cos(Az * pi_const);
    if (Lati > 0.416)
    {
            Lati = 0.416;
    }
    else if (Lati < -0.416)
    {
            Lati = -0.416;
    }
    Loni = Lonu + phi * sin(Az * pi_const) / cos(Lati * pi_const);
    Latm = Lati + 0.064 * cos((Loni - 1.617) * pi_const);
    T = 4.32e+4 * Loni + Ttr;
    if (T >= 86400L)
    {
            T -= 86400L;
    }
    else if (T < 0)
    {
            T += 86400L;
    }
    F_ = 1.0 + 16.0 * (0.53 - El) * (0.53 - El) * (0.53 - El);
    per = b0 + b1 * Latm + b2 * Latm * Latm + b3 * Latm * Latm * Latm;
    if (per < 72000.0)
    {
            per = 72000.0;
    }
    x = 2 * pi_const * (T - 50400.0) / per;
    amp = a0 + a1 * Latm + a2 * Latm * Latm + a3 * Latm * Latm * Latm;
    if (amp < 0.0)
    {
            amp = 0.0;
    }
    if (fabs(x) >= 1.57)
    {
            *dTiono = F_ * 5.0e-9;
    }
    else
    {
            *dTiono = F_ * (5.0e-9 + amp * (1.0 - x * x / 2.0 + x * x * x * x / 24.0));
    }
}

void Calculations::XYZ_BLH(long double X, long double Y, long double Z, double *B, double *L, double *H)
{
    /* IN : wsp satelity w XYZ
     * OUT: wsp satelity w BLH
     */
    //parametry elipsoidy GRS84
    const double a = 6378137.0;
    const double b = 6356752.314245;
    const double e = sqrt((pow(a,2)-pow(b,2))/pow(a,2));
    double R;

    R = sqrt(pow(X,2)+pow(Y,2));
    double q = 0;
    double B0 = atan((Z+q)/R);
    double RN = a/sqrt(1-(pow(e,2)*pow(sin(B0),2)));
    q = RN * pow(e,2) * sin(B0);
    double Bi=0;
    Bi = atan((Z+q)/R);
        do{
            B0 = Bi;
            RN = a/sqrt(1-(pow(e,2)*pow(sin(Bi),2)));
            q = RN * pow(e,2) * sin(Bi);
            Bi = atan((Z+q)/R);
        }while (abs(B0-Bi)>0.0000000001);
    double Lrad = asin(Y/R);

    double del_r, del_z;
    del_r = R - RN * cos(Bi);
    del_z = Z - RN * (1-pow(e,2)) * sin(Bi);

    //Wyznaczenie wysokosci
    if(del_r < 0 || del_z < 0)
    {
        *H = -sqrt(pow(del_r,2) + pow(del_z,2));
    }else
    {
        *H = sqrt(pow(del_r,2) + pow(del_z,2));
    }

    *B = Bi;
    *L = Lrad;
}

void Calculations::UsunDanePlikow()
{
    NavigationFIle = nullptr;
    ObservationFile = nullptr;
}


//funcja zamienia współrzędne geocentryczne XYZ na elipsoidalne
//na podstawie parametrów elipsoidy GRS80
void Calculations::PrzeliczNaWGS84(long double X, long double Y, long double Z, double *B, double *L, double *H)
{
    /* IN : wsp satelity w XYZ
     * OUT: wsp satelity w BLH
     */
    const long double pi_const = 3.14159265359;
    //parametry elipsoidy GRS84
    const double a = 6378137.0;
    const double b = 6356752.314245;
    const double e = sqrt((pow(a,2)-pow(b,2))/pow(a,2));
    double R;

    R = sqrt(pow(X,2)+pow(Y,2));
    double q = 0;
    double B0 = atan((Z+q)/R);
    double RN = a/sqrt(1-(pow(e,2)*pow(sin(B0),2)));
    q = RN * pow(e,2) * sin(B0);
    double Bi=0;
    Bi = atan((Z+q)/R);
        do{
            B0 = Bi;
            RN = a/sqrt(1-(pow(e,2)*pow(sin(Bi),2)));
            q = RN * pow(e,2) * sin(Bi);
            Bi = atan((Z+q)/R);
        }while (abs(B0-Bi)>0.00000000001);
    double Lrad = asin(Y/R);

    double del_r, del_z;
    del_r = R - RN * cos(Bi);
    del_z = Z - RN * (1-pow(e,2)) * sin(Bi);

    //Wyznaczenie wysokosci
    if(del_r < 0 || del_z < 0)
    {
        *H = -sqrt(pow(del_r,2) + pow(del_z,2));
    }else
    {
        *H = sqrt(pow(del_r,2) + pow(del_z,2));
    }

    *B = Bi * (180/pi_const);
    *L = Lrad *(180/pi_const);

}
