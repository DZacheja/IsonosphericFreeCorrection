#include "RinexNavigacyjny.h"
RinexNavigacyjny::RinexNavigacyjny()
{}
RinexNavigacyjny::~RinexNavigacyjny()
{}
/*
 *
 */
void RinexNavigacyjny::ZmienCzasObliczen(MyTimeClass czas)
{
    CzasObliczen = czas;
}

QString RinexNavigacyjny::getNazwaPliku()
{
    return NazwaPliku;
}


/*
*Obliczanie wspołrzędnych dla wybranej struktury na dany czas;
*/
void RinexNavigacyjny::obliczWsp(FileDatas* DanePliku)
{
    MyTimeClass czas;
    czas = DanePliku->CzasObliczenSatelity;

    const double GM = 3.986005e+14;
    const double we = 7.2921151467e-5;

    int hour,minute;
    double secounds;

    hour = czas.getHour();
    minute = czas.getMinutes();
    secounds = czas.getSecounds();

    long double tSV, SV,tt,toc,
            tk,A,A3, gma,n0,n,Mk,
            Ek,cos_vk,sin_vk,vk,
            Fk,du,dr,di,uk,rk,ik,
            XPk,YPk,Omega,Eo,t_tt,
            e_1, e_2;
    double X;
    double Y;
    double Z;


    SecoundsOfTheWeek = CalculateSecoundsOfMonth(day,month,year);
    SecoundsOfTheWeek *= 24 * 3600;
    toc = (DanePliku->hs*3600) + (DanePliku->ms*60) + DanePliku->ss + SecoundsOfTheWeek;
    tSV = (hour*3600) + (minute*60) + secounds + SecoundsOfTheWeek;

    t_tt = tSV - toc;

    SV = DanePliku->a0 + (DanePliku->a1*t_tt) + (DanePliku->a2*pow(t_tt,2));

    // ELEMENT DO OBLICZEŃ IONO FREE
    DanePliku->DtSi = SV;
    ////////////////////////////////

    tt = tSV - SV;
    tk = tt - DanePliku->toe;
    if(tk>302400L) tk -= 604800L;
    if (tk<-302400L) tk +=604800L;
    //calculate medium anomally
    A = pow(DanePliku->HalfA,2);
    A3 = pow(A,3);
    gma = GM/A3;
    n0 = pow(gma,0.5);
    n = n0 + DanePliku->del_n;
    Mk = DanePliku->M0 + (n*tk);
    Ek = Mk;
    do
    {
        Eo= Ek;
        Ek = Mk + DanePliku->e * sin(Ek);
    } while (fabs(Ek - Eo) >= 1.0e-8);

    //Calculation of the eccentric anomaly from Kepler's Law
    e_1 = cos(Ek) - DanePliku->e;
    e_2 = 1 - (DanePliku->e * (cos(Ek)));
    cos_vk = (e_1/e_2);

    e_1 = pow((1-pow(DanePliku->e,2)),0.5) * sin(Ek);
    e_2 = 1 - (DanePliku->e * cos(Ek));
    sin_vk = (e_1/e_2);

    vk = atan2(sin_vk,cos_vk);
    //Calculate width argument
    Fk = vk + DanePliku->omega;

    du = (DanePliku->Cuc * (cos(2*Fk))) + (DanePliku->Cus* sin(2*Fk));
    dr = (DanePliku->Crc * (cos(2*Fk))) + (DanePliku->Crs* sin(2*Fk));
    di = (DanePliku->Cic * (cos(2*Fk))) + (DanePliku->Cis* sin(2*Fk));
    uk = Fk + du;
    rk = A * (1 - (DanePliku->e*cos(Ek))) + dr;
    ik = DanePliku->i0 + (DanePliku->Idot * tk) + di;

    XPk = rk * cos(uk);
    YPk = rk * sin(uk);
    Omega = DanePliku->Omega0 + ((DanePliku->OmegaDot - we)*tk) - (we * DanePliku->toe);

    X = (XPk * cos(Omega)) - ( YPk * sin(Omega) * cos(ik));
    Y = XPk * sin(Omega) + (YPk * cos(Omega) * cos(ik));
    Z = YPk * sin(ik);
    DanePliku->X = X;
    DanePliku->Y = Y;
    DanePliku->Z = Z;

    //DanePliku->dReal = 2 * (sqrt(GM*A)/c)*DanePliku->e*sin(Ek);
    long double F = -4.442807633e-10;
    DanePliku->dReal = F * DanePliku->e * DanePliku->HalfA * sin(Ek);
}

long RinexNavigacyjny::CalculateSecoundsOfMonth(int d, int m, int y)
{
    static int t[] = { 0, 3, 2, 5, 0, 3,
                       5, 1, 4, 6, 2, 4 };
    y -= m < 3;
    return ( y + y / 4 - y / 100 +
             y / 400 + t[m - 1] + d) % 7;
}

std::vector<long double> RinexNavigacyjny::WspolrzedneSatelity(QString Numer, long double CxC, bool poprawka)
{

    //Zapisanie wsp do wektora i jego zwrot
    FileDatas *dane;
    dane = &Satellites[Numer];
    this->obliczWsp(dane);
    std::vector<long double> Wsp;

    if (poprawka)
    {
        const double c = 299792458; //predkosc swiatla w m/s
        const long double V_obr_ziemi = 7.2921151467E-5;
        long double czas_przelotu = CxC/c;
        long double alfa = V_obr_ziemi * czas_przelotu;
        long double X,Y,Z;
        X = (dane->X * cos(alfa)) + (dane->Y * sin(alfa));
        Y = (dane->Y * cos(alfa)) - (dane->X * sin(alfa));
        Z = dane->Z;
        Wsp.push_back(X);
        Wsp.push_back(Y);
        Wsp.push_back(Z);
        Wsp.push_back(dane->DtSi);
    }
    else
    {
        Wsp.push_back(dane->X);
        Wsp.push_back(dane->Y);
        Wsp.push_back(dane->Z);
        Wsp.push_back(dane->DtSi);
    }
    return Wsp;
}

void RinexNavigacyjny::WybraneSatelity(QList<QString> lista, std::map<QString,FileDatas*>* mapa)
{
mapa->clear();
FileDatas *item;
    for(auto& elem: lista)
    {

        item = &Satellites[elem];
        mapa->insert({elem,item});
    }
}

QVector<QString> RinexNavigacyjny::ListaSatelitow()
{
   QVector<QString> rezultat;
      for(auto& elem :Satellites)
        {
           rezultat.push_back(elem.first);
         }
             return rezultat;
}

QString RinexNavigacyjny::dataPliku(QString f)
{
    QString data;
    QFile file(f);
    if (!file.open(QFile::ReadOnly|QFile::Text))
    {
        Blad err;
        err.info = "Błąd otwarcia pliku nawigacyjnego podczas wczytywania";
        throw err;
    }else{
        QStringList lista; //lista przechowuje podzielone spacjami linie w osobnych elementach tablicy
        QTextStream in(&file);
        bool header = true;
        while(!in.atEnd())
        {
            QString linia_txt = in.readLine();
            int EndOfHeaderT = linia_txt.indexOf("END OF HEADER");
            if (EndOfHeaderT > 0){header = false; continue;}
            if (!header)
            {
              lista = linia_txt.split(" ",Qt::SkipEmptyParts); //split po spacjach
              int rok = lista[1].toInt();
              if (rok < 1000){rok += 2000;}
              data = QString::number(rok)+"-" +
                     QString::number(lista[2].toInt())+"-" +
                     QString::number(lista[3].toInt());
              break;
            }
        }
        file.close();
    }
    return data;
}
