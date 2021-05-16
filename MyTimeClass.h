/*
 * Autor: inż. Damian Zacheja
 * Opis:
 * Ponieważ wyznaczenie wymaga bardzo dokładnego określenia czasu
 * poniżej 1ms należało stworzyć własną klasę czasu wraz ze wszystkimi
 * potrzebnymi metodami do jej obsługi oraz przeładować wyamagane
 * operatory w celu ułatwienia dalszej pracy
 *
 * Klasa może być przydatna w wielu następpnych opracowaniach, dlatego
 * warto ją gdzieś zapisać.
 */
#ifndef MYTIMECLASS_H
#define MYTIMECLASS_H
#include <QString>

class MyTimeClass
{
private:
    int godz;
    int min;
    double sec;
public:
    struct BlednyCzas{
        QString informacja;
    };
    MyTimeClass();
    MyTimeClass(int, int, double);
    void addTime(int,int,double);
    void addTime(int,int);
    void addTime(int, double);
    void addSecounds(double);
    void addMinutes(int);
    void addHours(int);
    int getHour();
    int getMinutes();
    double getSecounds();
    double getSecoundsOfTheDay();
    QString GetTimeQString();
    bool operator>(MyTimeClass&);
    bool operator<(MyTimeClass&);
    bool operator>=(MyTimeClass&);
    bool operator<=(MyTimeClass&);
};

#endif // MYTIMECLASS_H
