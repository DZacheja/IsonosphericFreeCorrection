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
using namespace  std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    QRect ScreenRect = QApplication::desktop()->screenGeometry();
    int height =static_cast<int>(0.9 * ScreenRect.height());
    w.resize(ScreenRect.width(),height);
    w.setWindowState(Qt::WindowMaximized);
    w.showMaximized();
    QString asd = "asd";
      BrdcEphemeris brodecast(asd,0,0);
      brodecast.UzupelnijStrukture();
      brodecast.WyznaczWspolrzedneSatelitow(0,7,30);

      RinexOVer3 Over3;
      Over3.InHeader();
      Over3.WyszukajParametryCzestotliwosi();
      Calculations TestoweObliczenia(brodecast,Over3);
      TestoweObliczenia.oblicz();
//    arma::mat matrix1;
//    matrix1.resize(1,2);
//    matrix1(0,0) = 123;
//    matrix1.print("M:");
//    matrix1.resize(3,3);
//    matrix1(2,2) = 11;
//    matrix1.print("M:");

    return a.exec();
}
