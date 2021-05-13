#include "MyTimeClass.h"
MyTimeClass::MyTimeClass(){}
MyTimeClass::MyTimeClass(int g,int m,double s)
{
    if(g==0 && m==0 & s==0){
        BlednyCzas blad;
        blad.informacja = "Aby obliczyć czas dla godziny 0:0:0 należy wczytać poprzedni plik naviigacyjny";
        throw blad;
    }
    godz = g;
    min = m;
    sec = s;
}
void MyTimeClass::addTime(int g, int m, double s)
{
   sec += s;
   if (sec >= 60){
       sec -= 60;
       min++;
   }
   min += m;
   if(min >=60)
   {
       min -= 60;
       godz++;
   }
   godz += g;
   if (godz >= 24)
   {
       BlednyCzas blad;
       blad.informacja = "Przekroczono czas obliczeń, zmiejsz zakres godzinowy!";
       throw blad;

   }
}
void MyTimeClass::addTime(int g, int m)
{
    min += m;
    if(min >=60)
    {
        min -= 60;
        godz++;
    }
    godz += g;
    if (godz >= 24)
    {
        BlednyCzas blad;
        blad.informacja = "Przekroczono czas obliczeń, zmiejsz zakres godzinowy!";
        throw blad;

    }

}
void MyTimeClass::addTime(int m, double s)
{
    sec += s;
    if (sec >= 60){
        sec -= 60;
        min++;
    }
    min += m;
    if(min >=60)
    {
        min -= 60;
        godz++;
    }
    if (godz >= 24)
    {
        BlednyCzas blad;
        blad.informacja = "Przekroczono czas obliczeń, zmiejsz zakres godzinowy!";
        throw blad;

    }
}
void MyTimeClass::addHours(int g)
{
    godz += g;
if (godz >= 24)
{
    BlednyCzas blad;
    blad.informacja = "Przekroczono czas obliczeń, zmiejsz zakres godzinowy!";
    throw blad;

}
}
void MyTimeClass::addMinutes(int m)
{
    min += m;
    if(min >=60)
    {
        min -= 60;
        godz++;
    }
    if (godz >= 24)
    {
        BlednyCzas blad;
        blad.informacja = "Przekroczono czas obliczeń, zmiejsz zakres godzinowy!";
        throw blad;

    }
}
void MyTimeClass::addSecounds(double s)
{
    sec += s;
    if (sec >= 60){
        sec -= 60;
        min++;
    }
    if(min >=60)
    {
        min -= 60;
        godz++;
    }
    if (godz >= 24)
    {
        BlednyCzas blad;
        blad.informacja = "Przekroczono czas obliczeń, zmiejsz zakres godzinowy!";
        throw blad;

    }
}

//Konwertuje bieżacy czas na QString
QString MyTimeClass::GetTimeQString()
{
    QString g,m,s;
    g=QString::number(godz);
        if(godz <10){g = "0" + g;}

    m=QString::number(min);
        if(min <10){m = "0" + m;}

    s=QString::number(sec);
        if(sec <10){s = "0" + s;}

    return g+":"+m+":"+s;
}
