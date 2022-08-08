#include "mainwindow.h"
#include <QApplication>
#include <windows.h>
#include <QString>
#include <QWidget>
#include <QRect>
#include "MyTimeClass.h"
#include "MixedNavigationFile.h"
#include "RinexOVer3.h"
using namespace  std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(true);
    MainWindow w;
    w.showMaximized();
    return a.exec();
}
