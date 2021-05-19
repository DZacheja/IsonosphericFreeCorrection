#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <map>
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

private:
    Ui::MainWindow *ui;
    const QString F_DF = "#D_DIR";  //stała flaga dla ścieżki z danymi
    const QString F_RN = "#R_NAV#"; //stała flaga w pliku dla Rinexu navigacyjnego
    const QString F_RO = "#R_OBS#"; //stała flaga w pliku dla Rinexu obserwacyjnego
    const QString fileSettings = "settings.txt";
    QString PoprawnaNazwaPlikuNavigacyjnego; //zmienna sprawdz nazwe pliku navi
    QString Sciezka; //wybrana przezuzytkownika sciezka
    std::map<QString,QString> settings; //ustawienia wczytywane przy otwarciu pliku
    void Ustawienia(); //Sprawdz czy istnieje plik z ustawieniami
    void WczytajUstawienie(QString,QString);// Przypisz wczytane ustawienie
    void WczytajRinexObserwacyjny(QString); // Wczytatnie danych z rinxu navigacyjnego do interfejsu
    std::map<QString,int> DataPlikuObs; //Data pliku obserwacyjnego
    void ZapiszDoPlikuUstawien(QString,QString); //zapisanie ustawień do pliku
    void SprawdzStatusWczytaniaPlikow();
};
#endif // MAINWINDOW_H
