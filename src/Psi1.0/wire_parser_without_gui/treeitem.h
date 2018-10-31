#ifndef TREEITEM_H
#define TREEITEM_H
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <QTreeWidget>
#include <QList>
#include <QStandardItem>
using namespace std;
class treeitem
{
public:
    treeitem(int ID, string type, string name, string file, int size, int father);
    string gettype();
    string getname();
    int getsize();
    int getfather();
    QList<QStandardItem *> getitem();
    string getfile();
    int getID();
    void push_selected_wire(std::string wire);

private:
    QList<std::string> out_wires;
    string type_item;
    string name_item;
    string file_item;
    int ID_item;
    int size_item;
    int father_item;
    QList<QStandardItem *> inter_item;

};

#endif // TREEITEM_H
