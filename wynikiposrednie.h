#ifndef WYNIKIPOSREDNIE_H
#define WYNIKIPOSREDNIE_H

#include <QMainWindow>
#include <map>
#include <QString>
namespace Ui {
class WynikiPosrednie;
}

class WynikiPosrednie : public QMainWindow
{
    Q_OBJECT

public:
    explicit WynikiPosrednie(QWidget *parent = nullptr);
    ~WynikiPosrednie();

    //wyniki pośrednie
    std::map<QString,double> *Dreal;
    std::map<QString,double> *WartTropo;
    std::map<QString,double> *WartIono;
    std::map<int,QString> *WartDxDyDz;
    std::map<QString,double> *WartDSV;
    std::map<QString,double> *WartPoprCzasu;
    std::map<QString,std::vector<long double>> *WspSatelitow;
    std::map<QString,double> *PoczCxC;
    std::map<QString,double> *KoncCxC;
    std::map<QString,QString> *PoprawionyCzas;
    std::map<QString,std::vector<long double>> *WspSatelitowBezKorekcji;
    std::map<QString,QString> *AzymutSatelitow;
    std::map<QString,QString> *KatElewacjiSatelitow;
    std::map<QString,double> *P1CxC; //zapisz odczyt czestotliwosci P1
    std::map<QString,QString> *P2CxC; //jeżeli iono-free zpaisz odczyt czestotliwosci P2
    std::map<QString,QString> *UwagiDoSatelitow; //informacje o ewentualnym odrzuceniu, lub innych błędach w przyszłosci
    void uzupelnijSatelity();
private slots:
    void on_Satelity_currentIndexChanged(const QString &arg1);

private:
    Ui::WynikiPosrednie *ui;
};

#endif // WYNIKIPOSREDNIE_H
