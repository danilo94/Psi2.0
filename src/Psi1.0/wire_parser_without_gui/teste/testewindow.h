#ifndef TESTEWINDOW_H
#define TESTEWINDOW_H

#include <QMainWindow>

namespace Ui {
class TesteWindow;
}

class TesteWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TesteWindow(QWidget *parent = 0);
    ~TesteWindow();

private:
    Ui::TesteWindow *ui;
};

#endif // TESTEWINDOW_H
