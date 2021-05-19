#include "RinexNavigacyjny.h"

RinexNavigacyjny::RinexNavigacyjny()
{}
RinexNavigacyjny::~RinexNavigacyjny()
{}
void RinexNavigacyjny::ZmienCzasObliczen(MyTimeClass czas)
{
    CzasObliczen = czas;
}

QString RinexNavigacyjny::getNazwaPliku()
{
    return NazwaPliku;
}
