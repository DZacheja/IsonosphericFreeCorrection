#ifndef PANELINFORMACYJNY_H
#define PANELINFORMACYJNY_H

#include <QMainWindow>

namespace Ui {
class PanelInformacyjny;
}

class PanelInformacyjny : public QMainWindow
{
    Q_OBJECT

public:
    explicit PanelInformacyjny(QWidget *parent = nullptr);
    ~PanelInformacyjny();

private:
    Ui::PanelInformacyjny *ui;
};

#endif // PANELINFORMACYJNY_H
