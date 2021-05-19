#include "MyTimeClass.h"
MyTimeClass::MyTimeClass(){}
MyTimeClass::MyTimeClass(int g,int m,double s)
{
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

int MyTimeClass::getHour()
{
    return godz;
}

int MyTimeClass::getMinutes()
{
    return min;
}
double MyTimeClass::getSecounds()
{
    return sec;
}
double MyTimeClass::getSecoundsOfTheDay()
{
    double results;
    results = (godz * 3600) + (min *60) + sec;
    return results;
}
bool MyTimeClass::operator>(MyTimeClass &obj)
{
    double compareSesc;
    double thisSecs;
    compareSesc = obj.getSecoundsOfTheDay();
    thisSecs = this->getSecoundsOfTheDay();

    if (thisSecs > compareSesc){
        return true;
    }else{
        return false;}

}

bool MyTimeClass::operator<(MyTimeClass &obj)
{
    double compareSesc;
    double thisSecs;
    compareSesc = obj.getSecoundsOfTheDay();
    thisSecs = this->getSecoundsOfTheDay();

    if (thisSecs < compareSesc){
        return true;
    }else{
        return false;}

}
bool MyTimeClass::operator>=(MyTimeClass &obj)
{
    double compareSesc;
    double thisSecs;
    compareSesc = obj.getSecoundsOfTheDay();
    thisSecs = this->getSecoundsOfTheDay();

    if (thisSecs >= compareSesc){
        return true;
    }else{
        return false;}

}

bool MyTimeClass::operator<=(MyTimeClass &obj)
{
    double compareSesc;
    double thisSecs;
    compareSesc = obj.getSecoundsOfTheDay();
    thisSecs = this->getSecoundsOfTheDay();

    if (thisSecs <= compareSesc){
        return true;
    }else{
        return false;}

}
