#include "BrdcEphemeris.h"
#include "RinexNavigacyjny.h"
#include <QString>
#include <QList>
#include <math.h>
#include <string>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>

BrdcEphemeris::~BrdcEphemeris(){}
//konstktor: przypisuje zmienne do daty, określa nazwę pliku,
BrdcEphemeris::BrdcEphemeris(int dd, int mm, int yy)
{

    day = dd;
    month = mm;
    year = yy;

    znajdzNazwePliku(day, month, year);
    UzupelnijStrukture();
}
//wywołanie bezpośrednio nazwy pliku
BrdcEphemeris::BrdcEphemeris(QString Name, int h, int m)
{
    //NazwaPliku = Name;
    hour = h;
    minute = m;
    NazwaPliku = "E:\\WAT\\MAGISTERKA\\PROGRAM\\IonoFreeCorrection\\DANE\\brdc0020.21n";
    day = 2;
    month =1;
    year = 2021;
}
void BrdcEphemeris::znajdzNazwePliku(int d, int m, int y)
{
    QList<int> day_n = {31,28,31,30,31,30,31,31,30,31,30,31}; //the days in months on non leap year
    QList<int> day_p = {31,29,31,30,31,30,31,31,30,31,30,31}; //the days in months on leap year
    QStringList items;
    int days = 0;

    if((y%4==0 && y%100!=0) || y%400==0){
        for(int i = 0; i < m -2 ; i++){

            int a = day_p[i];
            days =  days + a;
        } days = days + d;
    }
    else {
        for(int i = 0; i <= m - 2; i++){
            int a = day_n[i];
            days =  days + a;
        } days = days + d;
    }
    QString rk;
    if(y >= 2000){
        y = y - 2000;}
    else{
        y = y - 1900;
    }
    if(y>10){
        rk = QString::number(y);
    }else {
        rk = "0";
        rk.append(QString::number(y));
    }
    QString dat;
    if (days < 100 && days >=10){
        dat = "0";
        dat.append(QString::number(days));
    }
    if(days < 10){
        dat = "00";
        dat.append(QString::number(days));
    }
    if(days>=100){
        dat = QString::number(days);
    }
    QString name = "brdc";
    name.append(dat);
    name.append("0.");
    name.append(rk);
    name.append("n");
    NazwaPliku = name;
}

/*
 * Funkcja znajdująca dane i zapisujące je w strukturze
 */
void BrdcEphemeris::UzupelnijStrukture(){


    //Sprawdzenie czy plik się otwierz
    QFile file(NazwaPliku);
    if (!file.open(QFile::ReadOnly|QFile::Text))
    {
        QMessageBox msgbox;
        msgbox.setText("Błąd otwarcia pliku!");
        msgbox.exec();

    }else{

    int NumerDoCzytania =0; //zmienna do Select/Case określająca linie danych
// zmienna określająca informacje czy szukac (0), znalazlo i zczytywac (1), zakonczyc petle(2) - dane wczytane
    int znalazlo = 0;

    QStringList lista; //lista przechowuje podzielone spacjami linie w osobnych elementach tablicy
    QTextStream in(&file);
    std::string zamiana;
    QString ObecnySatelita;
    FileDatas DanePliku; //przechowywane wartosci struktury satelity o danej godzinie
    bool header = true;
 // iteracja przez kolejne linie pliku
    while(!in.atEnd()){
        QString linia_txt = in.readLine();
        //Sprawdzenie nagłówka
        int EndOfHeaderT = linia_txt.indexOf("END OF HEADER");
        if (EndOfHeaderT > 0){header = false; continue;}
        if (header){continue;}
        //koniec sprawdzenia
        linia_txt.replace("-", " -"); //naprawa złączonych elementów
        linia_txt.replace("D ","D"); //usuniecie spacji z "D -"
        linia_txt.replace("D","e"); //zamiana "D" na "e"
        lista = linia_txt.split(" ",QString::SkipEmptyParts); //split po spacjach
        if(lista.count() >= 5){
            int CurrentHour = lista[4].toInt();
            if (CurrentHour %2 != 0 && CurrentHour != 0 ){CurrentHour++;}
            //sprawdzenie czy jest kolejna godzina
            if (CurrentHour == hour + 2){break;}
            //sprawdzenie czy zaczynają sie dane do wybranej godziny
            if (CurrentHour == hour)
            {
                znalazlo = 1;
                NumerDoCzytania = 1;
            }}

        if (znalazlo ==3){continue;}//pomiń 2 ostatnie wiersze
        if(znalazlo == 2)
        {
            DanePliku.Idot = std::stold(lista[0].toStdString());
            std::pair<QString,FileDatas> para{ObecnySatelita,DanePliku};
            Satellites.insert(para); //zapisz dane do mapy
            DanePliku = {}; //wyzeruj strukture
            znalazlo = 3;
        }
        if(znalazlo == 1){

            switch (NumerDoCzytania) {
            case 1:
               ObecnySatelita = lista[0];
               if(ObecnySatelita.length() >1){ObecnySatelita = "G" + ObecnySatelita;}
               else{ObecnySatelita = "G0" + ObecnySatelita;}

               DanePliku.SV = lista[0].toInt();
               DanePliku.hs = lista[4].toInt();
               DanePliku.ms = lista[5].toInt();
               DanePliku.ss = lista[6].toDouble();
               DanePliku.a0 = std::stold(lista[7].toStdString());
               DanePliku.a1 = std::stold(lista[8].toStdString());
               DanePliku.a2 = std::stold(lista[9].toStdString());
               NumerDoCzytania = 2;
                break;
            case 2:
                DanePliku.IODE = std::stold(lista[0].toStdString());
                DanePliku.Crs = std::stold(lista[1].toStdString());
                DanePliku.del_n = std::stold(lista[2].toStdString());
                DanePliku.M0 = std::stold(lista[3].toStdString());
                NumerDoCzytania = 3;
                break;
            case 3:
                DanePliku.Cuc = std::stold(lista[0].toStdString());
                DanePliku.e = std::stold(lista[1].toStdString());
                DanePliku.Cus = std::stold(lista[2].toStdString());
                DanePliku.HalfA = std::stold(lista[3].toStdString());
                NumerDoCzytania = 4;
                break;
            case 4:
                DanePliku.toe = std::stold(lista[0].toStdString());
                DanePliku.Cic = std::stold(lista[1].toStdString());
                DanePliku.Omega0 = std::stold(lista[2].toStdString());
                DanePliku.Cis = std::stold(lista[3].toStdString());
                NumerDoCzytania = 5;
                break;
            case 5:
                DanePliku.i0 = std::stold(lista[0].toStdString());
                DanePliku.Crc = std::stold(lista[1].toStdString());
                DanePliku.omega = std::stold(lista[2].toStdString());
                DanePliku.OmegaDot = std::stold(lista[3].toStdString());
                znalazlo = 2;
                break;
            }
        }
    }
    file.close();
}}
void BrdcEphemeris::WyznaczWspolrzedneSatelitow(int hh, int mm, int ss)
{
    //obliczenie wspolrzednych poprzez iteracje na mapie
    for(auto& elem : Satellites)
    {
        auto& [qstr,struk] = elem;
                obliczWsp(hh,mm,ss,struk);

    }
}
void BrdcEphemeris::obliczWsp(int hh, int mm, int ss, FileDatas& DanePliku)
{
    hour = hh;
    minute = mm;
    secounds = ss;

    long double tSV, SV,tt,toc,
            tk,A,A3, gma,n0,n,Mk,
            Ek,cos_vk,sin_vk,vk,
            Fk,du,dr,di,uk,rk,ik,
            XPk,YPk,Omega,Eo,t_tt,
            e_1, e_2;

    const double GM = 3.986005e+14;
    const double we = 7.2921151467e-5;

    SecoundsOfTheWeek = CalculateSecoundsOfMonth(day,month,year);
    SecoundsOfTheWeek *= 24 * 3600;
    toc = (DanePliku.hs*3600) + (DanePliku.ms*60) + DanePliku.ss + SecoundsOfTheWeek;
    tSV = (hour*3600) + (minute*60) + secounds + SecoundsOfTheWeek;

    t_tt = tSV - toc;
    SV = DanePliku.a0 + (DanePliku.a1*t_tt) + (DanePliku.a2*pow(t_tt,2));
    // ELEMENT DO OBLICZEŃ IONO FREE
    DanePliku.DtSi = SV;
    ////////////////////////////////
    tt = tSV - SV;
    tk = tt - DanePliku.toe;
    if(tk>302400L) tk -= 604800L;
    if (tk<-302400L) tk +=604800L;
//calculate medium anomally
    A = pow(DanePliku.HalfA,2);
    A3 = pow(A,3);
    gma = GM/A3;
    n0 = pow(gma,0.5);
    n = n0 + DanePliku.del_n;
    Mk = DanePliku.M0 + (n*tk);
    Ek = Mk;
    do
    {
        Eo= Ek;
        Ek = Mk + DanePliku.e * sin(Ek);
    } while (fabs(Ek - Eo) >= 1.0e-8);
//Calculation of the eccentric anomaly from Kepler's Law
       e_1 = cos(Ek) - DanePliku.e;
       e_2 = 1 - (DanePliku.e * (cos(Ek)));
       cos_vk = (e_1/e_2);

       e_1 = pow((1-pow(DanePliku.e,2)),0.5) * sin(Ek);
       e_2 = 1 - (DanePliku.e * cos(Ek));
       sin_vk = (e_1/e_2);

       vk = atan2(sin_vk,cos_vk);
//Calculate width argument
       Fk = vk + DanePliku.omega;

       du = (DanePliku.Cuc * (cos(2*Fk))) + (DanePliku.Cus* sin(2*Fk));
       dr = (DanePliku.Crc * (cos(2*Fk))) + (DanePliku.Crs* sin(2*Fk));
       di = (DanePliku.Cic * (cos(2*Fk))) + (DanePliku.Cis* sin(2*Fk));
       uk = Fk + du;
       rk = A * (1 - (DanePliku.e*cos(Ek))) + dr;
       ik = DanePliku.i0 + (DanePliku.Idot * tk) + di;

       XPk = rk * cos(uk);
       YPk = rk * sin(uk);
       Omega = DanePliku.Omega0 + ((DanePliku.OmegaDot - we)*tk) - (we * DanePliku.toe);

       X = (XPk * cos(Omega)) - ( YPk * sin(Omega) * cos(ik));
       Y = XPk * sin(Omega) + (YPk * cos(Omega) * cos(ik));
       Z = YPk * sin(ik);
       DanePliku.X = X;
       DanePliku.Y = Y;
       DanePliku.Z = Z;

}

long BrdcEphemeris::CalculateSecoundsOfMonth(int d, int m, int y)
{
    static int t[] = { 0, 3, 2, 5, 0, 3,
                       5, 1, 4, 6, 2, 4 };
    y -= m < 3;
    return ( y + y / 4 - y / 100 +
             y / 400 + t[m - 1] + d) % 7;
}

std::vector<long double> BrdcEphemeris::WspolrzedneSatelity(QString Numer, long double CxC)
{

    //Zapisanie wsp do wektora i jego zwrot
    FileDatas *dane;
    dane = &Satellites[Numer];
    std::vector<long double> Wsp;

    if (CxC != 0)
    {
        const double c = 299792458; //predkosc swiatla w m/s
        const long double V_obr_ziemi = 7.2921151467E-5;
        long double czas_przelotu = CxC/c;
        long double alfa = V_obr_ziemi * czas_przelotu;
        long double X,Y,Z;
        X = (dane->X * cos(alfa)) + (dane->Y * sin(alfa));
        Y = (dane->Y * cos(alfa)) + (dane->X * sin(alfa));
        Z = dane->Z;
        Wsp.push_back(X);
        Wsp.push_back(Y);
        Wsp.push_back(Z);
    }
    else
    {
        Wsp.push_back(dane->X);
        Wsp.push_back(dane->Y);
        Wsp.push_back(dane->Z);
    }
    return Wsp;
}

QVector<QString> BrdcEphemeris::ListaSatelitow()
{
   QVector<QString> rezultat;
   for(auto& elem :Satellites)
   {
       auto &[NumerS,param] = elem;

       rezultat.push_back(NumerS);
   }
   return rezultat;
}

void BrdcEphemeris::WybraneSatelity(QList<QString> lista, std::map<QString,FileDatas>& mapa)
{
    mapa.clear();
    FileDatas item;
    for(auto& elem: lista)
    {
        item = Satellites[elem];
        mapa.insert({elem,item});
    }
}
