#ifndef ASSERTION_INTERFACE_H
#define ASSERTION_INTERFACE_H

#include <vector>
#include <QFile>
#include <QDialog>
 #include <QModelIndex>
#include <QStandardItemModel>
#include <QListWidgetItem>
#include "assert_type.h"
#include "string"
namespace Ui {
class Assertion_Interface;
}

class Assertion_Interface : public QDialog
{
    Q_OBJECT

public:
    vector <asrt> assert_out;
    explicit Assertion_Interface(QWidget *parent = 0);
    ~Assertion_Interface();
private slots:
    void close();
    void select();
    void add_assert();
    void Ok();
private:
    int nrow;
    int ncol;
    QString selected_wire;
    QStandardItemModel *model;
    vector <asrt> Assertionss;
    Ui::Assertion_Interface *ui;
};

#endif // ASSERTION_INTERFACE_H
