#ifndef UWAGI_H
#define UWAGI_H

#include <QDialog>
#include <map>
#include <QString>
namespace Ui {
class Uwagi;
}

class Uwagi : public QDialog
{
    Q_OBJECT

public:
    explicit Uwagi(QWidget *parent = nullptr);
    ~Uwagi();
    std::map<QString,QString> *Informacje;
    void Uzupelnij();
private slots:
    void on_pushButton_clicked();

private:
    Ui::Uwagi *ui;
};

#endif // UWAGI_H
