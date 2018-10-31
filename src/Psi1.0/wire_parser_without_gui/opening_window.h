#ifndef OPENING_WINDOW_H
#define OPENING_WINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QListWidget>
#include <iostream>
#include <fstream>
#include <sstream>
#include <QFileDialog>
#include <iostream>
#include <vector>
#include "QString"
#include <QTextBrowser>
#include <QTextEdit>
#include <QLabel>
#include <QList>
#include <QTreeWidget>
#include <QWidget>
#include <QVBoxLayout>
namespace Ui {
class opening_window;
}

class opening_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit opening_window(QWidget *parent = 0,std::string topLevelFolder="",std::string topLevelPath="",std::string listOfWires="");

private slots:
    void handleButton();
    void handleButton2();
    void select_file();
    void select_directory();

private:
    QString FileName;
    QPushButton *file_select_button;
    QPushButton *directory_select_button;
    QPushButton *m_button;
    QPushButton *get_wires;
    QListWidget *listWidget ;
    QTreeWidget *treeWidget ;
    QListWidget *listWidget2;
    QTextBrowser *textb;
    QTextEdit *toplevel;
    QLabel *label_toplevel;
    QTextEdit *verilog_path;
    QTextEdit *verilog_path2;
    QTextEdit *verilog_path3;
    QLabel *label_verilog_path;
    QLabel *label_verilog_path2;
    QLabel *label_verilog_path3;
    QPushButton* m_Button;
    QTreeWidget* m_TreeWidget;
    QWidget*	 m_centralWidget;
    QVBoxLayout* m_VBoxLayout;
    std::vector<std::string> wiresToExtern;
    std::vector<std::string> myvector;
    std::vector<std::string> ReadWires(QString toplevel,QString path);
    std::vector<std::string> ReadCells(QString toplevel,QString path);
    std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
    std::string exec(char* cmd);
    void readSignals(std::string path);
    void handleButton_test();
};

#endif // OPENING_WINDOW_H
