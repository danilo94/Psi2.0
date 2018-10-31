#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <string>
#include <QMainWindow>
#include <QPushButton>
#include <QListWidget>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "QString"
#include <QTextBrowser>
#include "assert_interface.h"
#include "assertion_interface.h"
#include <QTextEdit>
#include <QLabel>
#include <QTreeWidget>
#include <QWidget>
#include <QVBoxLayout>
#include<QStandardItem>
#include<treeitem.h>
#include<QList>
#include<tree_item_module.h>
#include <inputform.h>
#include <QScrollArea>
#include <vector>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(std::vector<std::string> wiresToExtern,QWidget *parent = 0,std::string a = "",string path="",string toplevel="");
    QModelIndex global_index;
    QPushButton *Assertions;
    QString text_s;
    QList<std::string> NewFiles ;
    std::string top_file;
    std::string destiny_folder_global;
    std::string exec(char* cmd);
    //MainWindow(QWidget *parent = 0);
    //QModelIndex global_index;
   //MainWindow(QWidget *parent = 0);

private slots:
    void Assert_Manager();
    void handleButton();
    void handleButton2();
    void selectionChangedSlot(const QItemSelection & newSelection, const QItemSelection & oldSelection);
    QList<QStandardItem *> prepareRow(const QString &first,
                                      const QString &second,
                                      const QString &third,
                                      const QString &ID);
    void setupModelData(const QStringList &lines, QStandardItem *parent);
//std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);


private:
    vector <AssertionInterface> assertion_strcture;
    vector <asrt> assercoes;
    Assertion_Interface *assert_GUI;
    QStandardItem *item;
    QTreeView *treeView;
    QStandardItemModel *standardModel;
    QPushButton *m_button;
    QPushButton *get_wires;
    QListWidget *listWidget ;
    QTreeWidget *treeWidget ;
    QListWidget *listWidget2;
    QTextBrowser *textb;
    QTextEdit *toplevel;
    QLabel *label_toplevel;
    QTextEdit *verilog_path;
    QLabel *label_verilog_path;
    QList<treeitem*> items_tree;
    InputForm* input_form;
    QScrollArea *scroll_area;
    QPushButton* m_Button;
    QTreeWidget* m_TreeWidget;
    QWidget*	 m_centralWidget;
    QPushButton *Assertion_Button;


    std::vector <std::string> signalsToExtern;
   // QVBoxLayout* m_VBoxLayout;
    std::string FOLDER_BASE;

    std::vector<std::string> myvector;

    std::vector<std::string> ReadWires(QString toplevel,QString path);
    std::vector<std::string> ReadCells(QString toplevel,QString path);
    std::vector<std::string> split(const std::string &s, char delim);

    std::string copy_from_to(std::string file_name_from,std::string file_name_to,int line,std::vector<std::string> wires);
    std::string modify_private (QList<std::string> wPrivates);
    void handleButton_test();
    void tree_crawler();
    void tree_crawler2(int inputs, int outputs, string rede,string includes_names, string trace_name);
    void tree_crawler_test();
    void recursive_module_searching (tree_item_module* current_module, tree_item_module *top_item_module, string wire_name);
    void depth_searching (tree_item_module* top_item_module, int inputs, int outputs, string rede, string includes_names, string trace_name);
    std::string create_folder (std::string base_folder);
    void EraseSpace(string& str);
    void EraseBars(string& str);
    string Find_Verilog_File (QList<std::string>& wPrivates, QList<std::string>& wPublics, string &module_line);
    void CopyModule (vector<string>& module_copy, string file, string line_module,vector<string>& lines);
    int FindModuleModifyName (vector<string>& module_copy, string module_name, tree_item_module *&top_item_module, string father_name);
    void FindTypeDeclareAddWire (vector<string>& module_copy, int line_module, string wire_before_parenthesis, string connection_wire);
    void replaceAll(std::string& str, const std::string& from, const std::string& to) ;
    bool replace(std::string& str, const std::string& from, const std::string& to) ;
    bool check_folder_existence (std::string folder);
    int FindLineAssignInsert (vector<string>& module_copy, string line_search, string assign_string, int limit_header);
    int FindLimitHeader(vector<string>& module_copy);
    int IdentifyHeaderType (vector<string>& module_copy, int& limit_header);
    void ModifyHeader (vector<string>& module_copy, vector<string>& outputs_extern_public, vector<string>& connect_wires_public, vector<string>& assigns_public, vector<string>& outputs_extern_private, vector<string>& assigns_private, int& limit_header, int& type_header, int toplevel);
    void ChangeModuleName(vector<string>& module_copy, string newModuleName, string module_name );
    void WriteTofile(vector<string>& module_copy,string fileNameWrite);
    void CleanComents(vector<string>& module_copy);

    bool findElementInList(std::string wire);



};

#endif // MAINWINDOW_H
