#include "MixedNavigationFile.h"
//konstruktory
MixedNavigationFIle::MixedNavigationFIle(){}
MixedNavigationFIle::MixedNavigationFIle(QString fn, MyTimeClass c)
{
    NazwaPliku = fn;
    CzasObliczen = c;
    UzupelnijStrukture();
}

void MixedNavigationFIle::UzupelnijStrukture()
{
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

    int SzukanyCzasGalileo[2];
    int SzukanaGodzinaGalileo;
    SzukanaGodzinaGalileo = CzasObliczen.getHour();

    int szukaneMinuty = CzasObliczen.getMinutes()/10;
    szukaneMinuty *= 10;
    szukaneMinuty += 10;
    if (szukaneMinuty >=60)
    {
        SzukanaGodzinaGalileo++;
        szukaneMinuty -= 60;
        if(SzukanaGodzinaGalileo>=24)
        {
            SzukanaGodzinaGalileo = 23;
            szukaneMinuty = 50;
        }
    }
    SzukanyCzasGalileo[0] = SzukanaGodzinaGalileo;
    SzukanyCzasGalileo[1] = szukaneMinuty;

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
        QString OznSat;
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
            linia_txt.replace("D","e"); //zamiana D na e
            linia_txt.replace("E","e"); //zamiana E na e
            linia_txt.replace("e ","e"); //usuniecie spacji z "e -"
            lista = linia_txt.split(" ",QString::SkipEmptyParts); //split po spacjach
            if(lista.count() >= 5){
                int CurrentHour = lista[4].toInt();
                int E_h, E_m;
                E_h = lista[4].toInt();
                E_m = lista[5].toInt();
                //sprawdz system satelitarny - dane z GPS i Galileo zapisz
                ObecnySatelita = lista[0].replace("e","E");
                OznSat = ObecnySatelita.mid(0,1);

                if (CurrentHour %2 != 0 && CurrentHour != 0 ){CurrentHour++;}

                //sprawdzenie czy zaczynają sie dane do wybranej godziny
                if (CurrentHour - 2 == hour && (OznSat == "G"))
                {
                    if(!znalazloDate)
                    {
                        year = lista[1].toInt();
                        month = lista[2].toInt();
                        day = lista[3].toInt();
                        znalazloDate = true;
                    }
                    znalazlo = 1;
                    NumerDoCzytania = 1;
                }else if (OznSat == "E" && E_h == SzukanyCzasGalileo[0] && E_m == SzukanyCzasGalileo[1])
                {
                    if(!znalazloDate)
                    {
                        year = lista[1].toInt();
                        month = lista[2].toInt();
                        day = lista[3].toInt();
                        znalazloDate = true;
                    }
                    znalazlo = 1;
                    NumerDoCzytania = 1;
                }
            }

            if (znalazlo ==3){continue;}//pomiń 1 ostatni wiersz
            if(znalazlo == 2)
            {
                DanePliku.dReal = 0.0;
                DanePliku.CzasObliczenSatelity = CzasObliczen;
                DanePliku.SV = ObecnySatelita;
                std::pair<QString,FileDatas> para{ObecnySatelita,DanePliku};
                Satellites.insert(para); //zapisz dane do mapy
                DanePliku = {}; //wyzeruj strukture
                znalazlo = 3;
            }
            if(znalazlo == 1){

                switch (NumerDoCzytania) {
                case 1:
                    ObecnySatelita = lista[0];
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
                    NumerDoCzytania = 6;
                    break;
                case 6:
                    DanePliku.Idot = std::stold(lista[0].toStdString());
                    NumerDoCzytania =7;
                    break;
                case 7:
                    if(OznSat == "G"){
                        DanePliku.TDG = std::stold(lista[2].toStdString());
                    }else
                    {
                        DanePliku.TDG = 0.0;
                    }
                    znalazlo = 2;
                    break;
                }
            }
        }
        file.close();
    }
}

//Popraw dane konkretnego satelity
void MixedNavigationFIle::PoprawStruktureSatelity(FileDatas* dane,QString SV)
{
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

    int SzukanyCzasGalileo[3];
    int SzukanaGodzinaGalileo;
    SzukanaGodzinaGalileo = CzasObliczen.getHour();

    int szukaneMinuty = CzasObliczen.getMinutes()/10;
    szukaneMinuty *= 10;
    szukaneMinuty += 10;
    if (szukaneMinuty >=60)
    {
        SzukanaGodzinaGalileo++;
        szukaneMinuty -= 60;
        if(SzukanaGodzinaGalileo>=24)
        {
            SzukanaGodzinaGalileo = 23;
            szukaneMinuty = 50;
        }
    }
    SzukanyCzasGalileo[0] = SzukanaGodzinaGalileo;
    SzukanyCzasGalileo[1] = szukaneMinuty;

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
        QString OznSat;
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
            linia_txt.replace("D","e"); //zamiana D na e
            linia_txt.replace("E","e"); //zamiana E na e
            linia_txt.replace("e ","e"); //usuniecie spacji z "e -"
            lista = linia_txt.split(" ",QString::SkipEmptyParts); //split po spacjach
            if(lista.count() >= 5){
                lista = linia_txt.split(" ",QString::SkipEmptyParts); //split po spacjach
                int CurrentHour = lista[4].toInt();

                int E_h, E_m;
                E_h = lista[4].toInt();
                E_m = lista[5].toInt();
                //sprawdz system satelitarny - dane z GPS i Galileo zapisz
                ObecnySatelita = lista[0].replace("e","E");
                OznSat = ObecnySatelita.mid(0,1);
                if (CurrentHour %2 != 0 && CurrentHour != 0 ){CurrentHour++;}

                //sprawdzenie czy jest kolejna godzina
                if (CurrentHour == hour + 6){break;}

                //sprawdzenie czy zaczynają sie dane do wybranej godziny
                if (CurrentHour + 2 == hour &&(OznSat == "G"))
                {
                    ObecnySatelita = lista[0];
                    if (ObecnySatelita == SV) {
                        znalazlo = 1;
                        NumerDoCzytania = 1;
                    }
                }else if(OznSat == "E" && E_h == SzukanyCzasGalileo[0] && E_m == SzukanyCzasGalileo[1]){
                    ObecnySatelita = lista[0];
                    if (ObecnySatelita == SV) {
                        znalazlo = 1;
                        NumerDoCzytania = 1;
                    }
                }

            }

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
    }
}


//Odjęcie sekund od obecnego czasu
void MixedNavigationFIle::OdejmijSekundyZCzasuObliczenDanegoSatelity(QString SV,long double s)
{
    QString TypSatelity = SV.mid(0,1);
    FileDatas *dane;
    dane = &Satellites[SV];
    MyTimeClass *Czas;
    Czas = &dane->CzasObliczenSatelity;

    //sprawdz obcena godzine danych efemeryd sie zgadza
    int hour=0;
    int min=0;
    if (TypSatelity == "G"){
        if(Czas->getHour()%2 == 0)
        {
            hour = Czas->getHour();
        }else
        {
            hour = Czas->getHour() -1;
        }
        if (hour > 22)
        {
            hour = 20;
        }
    }else if(TypSatelity == "E")
    {
        hour = Czas->getHour();
        min = Czas->getMinutes()/10;
        min *= 10;
        min += 10;
        if (min>= 60)
        {
            hour++;
            min -=60;
        }
        if (hour>=24)
        {
            hour = 23;
            min = 50;
        }
    }

    //odejmij sekundy od obecnego czasu
    Czas->subSecounds(s);

    //sprawdz godzine danych efemeryd po korekcie
    int newHour=0;
    int newMinutes=0;

    if(TypSatelity == "G"){
        if(Czas->getHour()%2 == 0)
        {
            newHour = Czas->getHour();
        }else
        {
            newHour = Czas->getHour() -1;
        }
        if (newHour >= 22)
        {
            newHour = 20;
        }
    }else if(TypSatelity =="E")
    {
        newHour = Czas->getHour();
        newMinutes = Czas->getMinutes()/10;
        newMinutes *= 10;
        if (newMinutes>= 60)
        {
            newHour++;
            min -=60;
            if (newHour>=24)
            {
                newHour = 23;
                newMinutes = 50;
            }
        }

    }
    //jeżeli dane się różnią to wyzukaj strukturę od nowa!
    if (TypSatelity == "G"){
        if(hour != newHour)
        {
            this->PoprawStruktureSatelity(dane,SV);
        }
    }else if(TypSatelity == "E")
    {
        if(hour != newHour || min != newMinutes)
        {
            this->PoprawStruktureSatelity(dane,SV);
        }
    }
}
