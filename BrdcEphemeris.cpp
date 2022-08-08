#include "BrdcEphemeris.h"
#include "RinexNavigacyjny.h"

/*
 * Pusty konstruktor
 */
BrdcEphemeris::BrdcEphemeris(){}
BrdcEphemeris::~BrdcEphemeris(){}
//konstktor: przypisuje zmienne do daty, określa nazwę pliku,
BrdcEphemeris::BrdcEphemeris(int dd, int mm, int yy, QString &filen)
{

    day = dd;
    month = mm;
    year = yy;

    filen = znajdzNazwePliku(day, month, year);

}
//wywołanie bezpośrednio nazwy pliku
BrdcEphemeris::BrdcEphemeris(QString Name,MyTimeClass czas)
{
    NazwaPliku = Name;
    CzasObliczen = czas;
    znalazloDate = false;
    this->UzupelnijStrukture();

}

/*
/Znalezienie poprawnej nazy pliku
*/
QString BrdcEphemeris::znajdzNazwePliku(int d, int m, int y)
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
    return name;
}

/*
 * Funkcja znajdująca dane i zapisujące je w strukturze
 */
void BrdcEphemeris::UzupelnijStrukture(){
    Satellites.clear(); //wyczyść strukturę!

    int hour;
    if(CzasObliczen.getHour()%2 == 0)
    {
        hour = CzasObliczen.getHour();
    }else
    {
        hour = CzasObliczen.getHour() -1;
    }

    if (hour > 22)
    {
        hour = 20;
    }
    //Sprawdzenie czy plik się otwierz
    QFile file(NazwaPliku);
    if (!file.open(QFile::ReadOnly|QFile::Text))
    {
        Blad err;
        err.info = "Błąd otwarcia pliku nawigacyjnego podczas Uzupełniania struktury";
        throw err;
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
            int IONa = linia_txt.indexOf("ION ALPHA");
            int IONb = linia_txt.indexOf("ION BETA");
            if (IONa > 0)
            {
                linia_txt.replace("D","e"); //zamiana "D" na "e"
                lista = linia_txt.split(" ",Qt::SkipEmptyParts); //split po spacjach
                brdcIonoPharam[0] = lista[0].toDouble();
                brdcIonoPharam[1] = lista[1].toDouble();
                brdcIonoPharam[2] = lista[2].toDouble();
                brdcIonoPharam[3] = lista[3].toDouble();
            }
            if (IONb > 0)
            {
                linia_txt.replace("D","e"); //zamiana "D" na "e"
                lista = linia_txt.split(" ",Qt::SkipEmptyParts); //split po spacjach
                brdcIonoPharam[4] = lista[0].toDouble();
                brdcIonoPharam[5] = lista[1].toDouble();
                brdcIonoPharam[6] = lista[2].toDouble();
                brdcIonoPharam[7] = lista[3].toDouble();
            }
            if (EndOfHeaderT > 0){header = false; continue;}
            if (header){continue;}
            //koniec sprawdzenia
            linia_txt.replace("-", " -"); //naprawa złączonych elementów
            linia_txt.replace("D ","D"); //usuniecie spacji z "D -"
            linia_txt.replace("D","e"); //zamiana "D" na "e"
            lista = linia_txt.split(" ",Qt::SkipEmptyParts); //split po spacjach
            if(lista.count() >= 5){
                int CurrentHour = lista[4].toInt();
                if (CurrentHour %2 != 0 && CurrentHour != 0 ){CurrentHour++;}
                //sprawdzenie czy jest kolejna godzina
                if (CurrentHour == hour + 4){break;}
                //sprawdzenie czy zaczynają sie dane do wybranej godziny
                if ((CurrentHour - 2) == hour)
                {
                    if(!znalazloDate)
                    {
                        year = 2000 + lista[1].toInt();
                        month = lista[2].toInt();
                        day = lista[3].toInt();
                        znalazloDate = true;
                    }
                    znalazlo = 1;
                    NumerDoCzytania = 1;
                }}

            if (znalazlo ==3){continue;}//pomiń 1 ostatni wiersz
            if(znalazlo == 2)
            {
                DanePliku.dReal = 0.0;
                DanePliku.CzasObliczenSatelity = CzasObliczen;
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

                    DanePliku.SV = ObecnySatelita;
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
                    NumerDoCzytania = 6;
                    break;
                case 6:
                    DanePliku.Idot = std::stold(lista[0].toStdString());
                    NumerDoCzytania =7;
                    break;
                case 7:
                    DanePliku.TDG = std::stold(lista[2].toStdString());
                    znalazlo = 2;
                    break;
                }
            }
        }
        file.close();
    }}


/*
 * Funkcja znajdująca dane konkretnego satelity i poprawia je w strukturze
 */
void BrdcEphemeris::PoprawStruktureSatelity(FileDatas *dane, QString SV){

    MyTimeClass czas = dane->CzasObliczenSatelity;
    int hour;
    if(czas.getHour()%2 == 0)
    {
        hour = czas.getHour();
    }else
    {
        hour = czas.getHour() -1;
    }

    if (hour > 22)
    {
        hour = 20;
    }
    //Sprawdzenie czy plik się otwiera
    QFile file(NazwaPliku);
    if (!file.open(QFile::ReadOnly|QFile::Text))
    {
        Blad err;
        err.info = "Błąd otwarcia pliku nawigacyjnego podczas Uzupełniania struktury";
        throw err;
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
            lista = linia_txt.split(" ",Qt::SkipEmptyParts); //split po spacjach
            if(lista.count() >= 5){
                int CurrentHour = lista[4].toInt();
                if (CurrentHour %2 != 0 && CurrentHour != 0 ){CurrentHour++;}
                //sprawdzenie czy jest kolejna godzina
                if (CurrentHour == hour + 4){break;}
                //sprawdzenie czy zaczynają sie dane do wybranej godziny
                if (CurrentHour - 2 == hour)
                {
                    ObecnySatelita = lista[0];
                    if(ObecnySatelita.length() >1){ObecnySatelita = "G" + ObecnySatelita;}
                    else{ObecnySatelita = "G0" + ObecnySatelita;}
                    if (ObecnySatelita == SV) {
                    znalazlo = 1;
                    NumerDoCzytania = 1;
                    }
                }}

            if (znalazlo ==3){break;}//pomiń 1 ostatni wiersz
            if(znalazlo == 2)
            {
                dane->dReal = 0.0;
                znalazlo = 3;
            }
            if(znalazlo == 1){

                switch (NumerDoCzytania) {
                case 1:
                    dane->hs = lista[4].toInt();
                    dane->ms = lista[5].toInt();
                    dane->ss = lista[6].toDouble();
                    dane->a0 = std::stold(lista[7].toStdString());
                    dane->a1 = std::stold(lista[8].toStdString());
                    dane->a2 = std::stold(lista[9].toStdString());
                    NumerDoCzytania = 2;
                    break;
                case 2:
                    dane->IODE = std::stold(lista[0].toStdString());
                    dane->Crs = std::stold(lista[1].toStdString());
                    dane->del_n = std::stold(lista[2].toStdString());
                    dane->M0 = std::stold(lista[3].toStdString());
                    NumerDoCzytania = 3;
                    break;
                case 3:
                    dane->Cuc = std::stold(lista[0].toStdString());
                    dane->e = std::stold(lista[1].toStdString());
                    dane->Cus = std::stold(lista[2].toStdString());
                    dane->HalfA = std::stold(lista[3].toStdString());
                    NumerDoCzytania = 4;
                    break;
                case 4:
                    dane->toe = std::stold(lista[0].toStdString());
                    dane->Cic = std::stold(lista[1].toStdString());
                    dane->Omega0 = std::stold(lista[2].toStdString());
                    dane->Cis = std::stold(lista[3].toStdString());
                    NumerDoCzytania = 5;
                    break;
                case 5:
                    dane->i0 = std::stold(lista[0].toStdString());
                    dane->Crc = std::stold(lista[1].toStdString());
                    dane->omega = std::stold(lista[2].toStdString());
                    dane->OmegaDot = std::stold(lista[3].toStdString());
                    NumerDoCzytania = 6;
                    break;
                case 6:
                    dane->Idot = std::stold(lista[0].toStdString());
                    NumerDoCzytania =7;
                    break;
                case 7:
                    dane->TDG = std::stold(lista[2].toStdString());
                    znalazlo = 2;
                    break;
                }
            }
        }
        file.close();
    }}

//Odjęcie sekund od obecnego czasu
void BrdcEphemeris::OdejmijSekundyZCzasuObliczenDanegoSatelity(QString SV,long double s)
{
    FileDatas *dane;
    dane = &Satellites[SV];
    MyTimeClass *Czas;
    Czas = &dane->CzasObliczenSatelity;
    //sprawdz obcena godzine danych efemeryd
     int hour;
     if(Czas->getHour()%2 == 0)
     {
      hour = Czas->getHour();
     }else
     {
      hour = Czas->getHour() -1;
     }

     //odejmij sekundy od obecnego czasu
     Czas->subSecounds(s);

     //sprawdz godzine danych efemeryd po korekcie
     int newHour;
     if(Czas->getHour()%2 == 0)
     {
      newHour = Czas->getHour();
     }else
     {
      newHour = Czas->getHour() -1;
     }

     //jeżeli dane się różnią to wyzukaj strukturę od nowa!
     if(hour != newHour)
     {
         this->PoprawStruktureSatelity(dane,SV);
     }

}
