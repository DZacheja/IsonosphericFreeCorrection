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
    QString GetTimeQString();
};

#endif // MYTIMECLASS_H
