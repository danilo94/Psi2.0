#include "testewindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TesteWindow w;
    w.show();

    return a.exec();
}
