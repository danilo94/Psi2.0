#include "treeitem.h"

using namespace std;
treeitem::treeitem(int ID, string type, string name,string file, int size, int father)
{
    this->ID_item = ID;
    this->type_item = type;
    this->name_item = name;
    this->file_item = file;
    this->size_item = size;
    this->father_item = father;

}
string treeitem::gettype(){
    return this->type_item;
}
int treeitem::getfather(){
    return this->father_item;
}
QList<QStandardItem *> treeitem::getitem(){
    return this->inter_item;
}
string treeitem::getname(){
    return this->name_item;
}
int treeitem::getsize(){
    return this->size_item;
}
string treeitem::getfile(){
    return this->file_item;
}
int treeitem::getID(){
    return this->ID_item;
}
void treeitem::push_selected_wire(std::string wire){
    out_wires << wire;
}
