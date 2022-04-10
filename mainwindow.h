#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <map>
#include <QVector>
#include "wynikiposrednie.h"
#include "panelinformacyjny.h"
#include "uwagi.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ButtonReadObsFile_clicked(); //wczytanie pliku obserwacyjnego

    void on_ButtonReadNaviFile_clicked(); //wczytanie pliku navigacyjnego

    void on_pushButton_Oblicz_clicked(); //Wykonaj obliczenia

    void on_Informacje_clicked(); //panel informacyjny

    void on_tableWyniki_cellClicked(int row, int column);

    void on_pushButton_clicked(); //exportowanie danych

    void on_horizontalSlider_sliderMoved(int position);

    void on_Corr_elevation_stateChanged(int arg1);

    void on_Corr_IonoFree_stateChanged(int arg1);

    void on_Corr_stdIono_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
    WynikiPosrednie *oknoWynikowe;
    PanelInformacyjny *infoWindow;
    Uwagi *OknoUwag;
    QVector<WynikiPosrednie *> okna;
    const QString F_DF = "#D_DIR";  //stała flaga dla ścieżki z danymi
    const QString F_RN = "#R_NAV#"; //stała flaga w pliku dla Rinexu navigacyjnego
    const QString F_RO = "#R_OBS#"; //stała flaga w pliku dla Rinexu obserwacyjnego
    const QString fileSettings = "settings.txt";
    QString DataPlikuNavi; //Data pliku obserwacyjnego
    QString DataPlikuObs; //Data pliku obserwacyjnego
    QString Sciezka; //wybrana przezuzytkownika sciezka
    std::map<QString,QString> settings; //ustawienia wczytywane przy otwarciu pliku
    ///Funkcje
    void Ustawienia(); //Sprawdz czy istnieje plik z ustawieniami
    void WczytajUstawienie(QString,QString);// Przypisz wczytane ustawienie
    void WczytajRinexObserwacyjny(QString); // Wczytatnie danych z rinxu navigacyjnego do interfejsu
    void ZapiszDoPlikuUstawien(QString,QString); //zapisanie ustawień do pliku
    void SprawdzStatusWczytaniaPlikow();
    void ObliczSrednie();
    void PrzeliczNaWGS84(long double,long double,long double,double*,double*,double*);
};
#endif // MAINWINDOW_H
