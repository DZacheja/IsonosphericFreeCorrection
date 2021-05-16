#include "mainwindow.h"
#include <QApplication>
#include "BrdcEphemeris.h"
#include <QDebug>
#include <armadillo>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <QProcess>
#include <windows.h>
#include <string>
#include <QString>
//#include <crx2rnx.h>
#include <QStringList>
#include "RinexOVer3.h"
#include "calculations.h"
#include <QDesktopWidget>
#include <QRect>
#include "MyTimeClass.h"
using namespace  std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.show();
    QRect ScreenRect = QApplication::desktop()->screenGeometry();
    int height =static_cast<int>(0.9 * ScreenRect.height());
    w.resize(ScreenRect.width(),height);
    w.show();
/*    MyTimeClass asd;
    asd = MyTimeClass(0,1,10.12);
    MyTimeClass asd2(0,12,11);
*/  //  bool cowieksze = asd > asd2*/;
//    QString test = "asdasd";
//    BrdcEphemeris brodecast(test,0,0);
//      brodecast.UzupelnijStrukture();
//      brodecast.WyznaczWspolrzedneSatelitow(0,7,30);

//      RinexOVer3 Over3;
//      Over3.InHeader();
//      Over3.WyszukajParametryCzestotliwosi();
//      RinexNavigacyjny *wskNavigacyjny;
//      RinexObserwacyjny *wskObserwacyjny;
//      wskNavigacyjny = &brodecast;
//      wskObserwacyjny = &Over3;

//      Calculations TestoweObliczenia(wskNavigacyjny,wskObserwacyjny);
//      TestoweObliczenia.oblicz();
//    arma::mat matrix1;
//    matrix1.resize(1,2);
//    matrix1(0,0) = 123;
//    matrix1.print("M:");
//    matrix1.resize(3,3);
//    matrix1(2,2) = 11;
//    matrix1.print("M:");

    return a.exec();
}
