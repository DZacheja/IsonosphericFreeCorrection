#ifndef MIXEDNAVIGATIONFILE_H
#define MIXEDNAVIGATIONFILE_H
#include "RinexNavigacyjny.h"

class MixedNavigationFIle final: public RinexNavigacyjny
{
private:
     void UzupelnijStrukture() override;
public:
    MixedNavigationFIle(QString,MyTimeClass);
    MixedNavigationFIle();

    //Funckje
    void PoprawStruktureSatelity(FileDatas*,QString) override;
    void OdejmijSekundyZCzasuObliczenDanegoSatelity(QString ,long double) override;
    friend class Calculations;
};

#endif // MIXEDNAVIGATIONFILE_H
