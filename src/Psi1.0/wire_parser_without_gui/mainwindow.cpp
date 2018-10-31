#include "mainwindow.h"
#include "opening_window.h"
#include <QCheckBox>
#include <iterator>
#include <QFileDialog>
#include <QMessageBox>
#include <sstream>
#include <QtCore/QCoreApplication>
#include <algorithm>
#include<treeitem.h>
#include<tree_item_module.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <inputform.h>
#include <algorithm>
#include "Abs_network.h"
#include <fstream>
#include <string>
//#include <boost/filesystem.hpp>


//#include </usr/include/boost/algorithm/string/string.hpp>
#include <sstream>
using std::ifstream;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <iterator>
using std::istream_iterator;
#include <fstream>
#include <iostream>
#include <QGridLayout>
#include <QFormLayout>
using namespace std;




MainWindow::MainWindow(std::vector<std::string> wiresToExtern,QWidget *parent, std::string a, std::string path,std::string toplevels ): QMainWindow(parent){

    std::cout << wiresToExtern.size() << std::endl;
    signalsToExtern = wiresToExtern;
    FOLDER_BASE = path;
    vector<string> wires;
    string wire ("wireteste");
    wires.push_back(wire);
    //copy_from_to("/home/andre/Documentos/CLearning/simpletest_top.v","aaa",3,wires); //sempre linha-1
    //Tree
    treeView = new QTreeView(this);
    //setCentralWidget(treeView);
    standardModel = new QStandardItemModel ;
    QList<QStandardItem *> preparedRow =prepareRow("Type", "Wire Name", "Wire Width","Wire ID");
    item = standardModel->invisibleRootItem();
    // adding a row to the invisible root item produces a root element
    item->appendRow(preparedRow);
    QFile file("example.txt");
    file.open(QIODevice::ReadOnly);
    QString data = file.readAll();
    int tam_b = data.size();
    //
    //remove("Hierarchy.txt");
    file.close();
    QModelIndex auxxx = preparedRow.first()->index();
    global_index = auxxx;
    setupModelData(data.split(QString("\n")),preparedRow.first());
    treeView->setModel(standardModel);
    //treeView->expandAll();
    //selection changes shall trigger a slot
    QItemSelectionModel *selectionModel= treeView->selectionModel();
    /*connect(selectionModel, SIGNAL(selectionChanged (const QItemSelection &, const QItemSelection &)),
            this, SLOT(selectionChangedSlot(const QItemSelection &, const QItemSelection &)));*/
    //connect(treeView, SIGNAL(tree_crawler_test()),this, SLOT(selectionChangedSlot(const QItemSelection &, const QItemSelection &)));
    //----------------------------------------------------------------------
    //Components initializations.
  //  m_button = new QPushButton("Network Instance", this);
    get_wires = new QPushButton("Generate Verilog", this);
    toplevel = new QTextEdit(this);
    //textb = new QTextBrowser(this);
    label_toplevel = new QLabel("Toplevel Module:",this);
    listWidget = new QListWidget(this);
    m_centralWidget = new QWidget();
    treeWidget = new QTreeWidget(m_centralWidget);
    verilog_path = new QTextEdit(this);
    input_form = new InputForm(this);
    scroll_area = new QScrollArea(this);
   // verilog_path->setText("/home/andre/Documentos/CLearning");
    label_verilog_path = new QLabel("Verilog File Path:",this);
    //myvector = NULL;
    //--------------------------------------------------------------
    //Components geometry settings.
    label_verilog_path->setGeometry(QRect(QPoint(330, 25),QSize(200, 25)));
    verilog_path->setGeometry(QRect(QPoint(450, 25),QSize(400, 25)));
    label_toplevel->setGeometry(QRect(QPoint(330, 0),QSize(200, 25)));
    toplevel->setGeometry(QRect(QPoint(450, 0),QSize(400, 25)));
    //textb->setGeometry(QRect(QPoint(500,50),QSize(500,500)));
    //m_button->setGeometry(QRect(QPoint(450, 550),QSize(200, 50)));
    get_wires->setGeometry(QRect(QPoint(450, 550),QSize(200, 50)));
    //listWidget->setGeometry(QRect(QPoint(0,50),QSize(500,500)));
    treeView->setGeometry(QRect(QPoint(0,50),QSize(500,500)));
    scroll_area->setGeometry(QRect(QPoint(500,50),QSize(500,500)));
    scroll_area->setWidget(input_form);
    toplevel->setText(toplevels.c_str());
    toplevel->setDisabled("TRUE");
    verilog_path->setText(path.c_str());
    verilog_path->setDisabled("TRUE");
    this->resize(1000,600);
    //--------------------------------------------------------------
    connect(get_wires,SIGNAL(released()),this,SLOT(handleButton2()));
 //   connect(input_form->Assertions,SIGNAL(released()),this,SLOT(Assert_Manager()));

    handleButton2();
}
void MainWindow::Assert_Manager(){
    assert_GUI = new Assertion_Interface(0);
    //win_assert = new window_assert(0);
    //win_assert->show();
    assert_GUI->show();



}

void MainWindow::selectionChangedSlot(const QItemSelection & /*newSelection*/, const QItemSelection & /*oldSelection*/)
{
    //get the text of the selected item
    QModelIndex rootindex = treeView->rootIndex();
    QModelIndex index_aux = treeView->selectionModel()->currentIndex();
    //const QModelIndex index = global_index.child(index_aux.row(),0);
    const QModelIndex index = index_aux.parent().child(index_aux.row(),0);
    const QModelIndex index3 = index_aux.parent().child(index_aux.row(),2);
    const QModelIndex indexaux = index_aux.parent().child(index_aux.row(),1);
    const QModelIndex ID_ITEM = index_aux.parent().child(index_aux.row(),3);
    int index_row = index_aux.row();
    int index_column = index_aux.column();
    QString row = QString::number(index.row());
    QString column = QString::number(index.column());
    QModelIndex index2;
    if(index.child(0,0).isValid()){
        index2 = index.child(0,0);
    }else{
        index2 = index;
    }
    //qDebug("row "+row.toLatin1()+"column "+column.toLatin1());
    //    QModelIndex index2 = new QModelIndex(index.row(),0);
    QString selectedText = index.data(Qt::DisplayRole).toString();
    QString selectedText2 = indexaux.data(Qt::DisplayRole).toString();
    QString current_id = ID_ITEM.data(Qt::DisplayRole).toString();
    QStandardItem* treee;
    QStandardItem*treee2;
    treee2 = static_cast<QStandardItem*> (index3.internalPointer());
    treee = static_cast<QStandardItem*> (treeView->selectionModel()->currentIndex().internalPointer());
    //std::cout << "checado" << treee->data(Qt::DisplayRole).toString().toStdString() << "\n";
    std::string name_test = indexaux.data(Qt::DisplayRole).toString().toStdString();
    QStandardItem *treeit =  treee->child(10);
    QString wiresub("wiresub");
    if(!selectedText.toStdString().compare(wiresub.toStdString())){
    }else{
        treeit =  treeit->child(4);
        treeit =  treeit->child(0);
        treeit =  treeit->child(0,1);
        //treeit = treeit->child(10);
        int aaa = treeit->row();
        std::cout << "aaaa" <<treeit->data(Qt::DisplayRole).toString().toStdString() << "\n\n\n\n";
        if(treee->checkState() == 2){
            std::cout << "checado" << treee->data(Qt::DisplayRole).toString().toStdString() << "\n";
        }
        else{
            std::cout << " nao checado  " << treee->data(Qt::DisplayRole).toString().toStdString() << "\n";
        }
    }
    //find out the hierarchy level of the selected item
    int hierarchyLevel=1;
    QModelIndex seekRoot = index;
    while(seekRoot.parent() != QModelIndex())
    {
        seekRoot = seekRoot.parent();
        hierarchyLevel++;
    }
    QString showString = QString("%1, Level %2").arg(selectedText)
            .arg(hierarchyLevel);
    setWindowTitle(showString);
    /*text_s = textb->toPlainText();
    if(text_s.contains(selectedText2))
    {
        text_s.replace(selectedText2+",",QString(""));
    }
    else{
        text_s = text_s+selectedText2;
        text_s = text_s+",";
        textb->setText(text_s);
    }
    textb->setText(text_s);*/
}





string MainWindow::copy_from_to(std::string file_name_from,std::string file_name_to,int line, vector<string> wires){
    string file_copy_name = file_name_to;
    // Store the words from the two files into these two vectors
    //vector<string> lines_array;
    // Create two input streams, opening the named files in the process.
    // You only need to check for failure if you want to distinguish
    // between "no file" and "empty file". In this example, the two
    // situations are equivalent.
    std::ofstream myfile;
    myfile.open (file_name_to.c_str());
    ifstream file_from(file_name_from.c_str());
    std::string contents((std::istreambuf_iterator<char>(file_from)),
                         std::istreambuf_iterator<char>());
    //cout << contents.c_str();
    char aaa = '\n';
    vector<string> lines_array = split(contents,aaa);
    //wires outputed
    string comma (")");
    int b = line-1;
    while(1){
        try {
            // use ".at()" and catch the resulting exception if there is any
            // chance that the index is bogus. Since we are reading external files,
            // there is every chance that the index is bogus.
            std::string::size_type pos = lines_array.at(b).find(")");
            if(pos != std::string::npos)
            {
                string assigns;
                char dotcomma = ')';
                vector<string> line_endm = split(lines_array.at(b),dotcomma);
                for (int var = 0; var < wires.size(); ++var) {
                    line_endm.at(0) = line_endm[0] + std::string(", output wire ") + wires[var] +"_out";
                    assigns = assigns + string("assign ") + wires[var] +"_out" + " = " + wires[var] + ";" +"\n";
                   // assigns = assigns + "assign do["+var+ string ("]") + " = " + wires[var] + ";"+ "\n";
                    QString fio (wires[var].c_str());
                }
                std::cout << "teste aquiii" << line_endm.at(0) << "\n\n\n";

                if(line_endm.size() == 2){
                    lines_array[b] = line_endm[0] + std::string(")") + line_endm[1];
                }
                if(line_endm.size() == 1){
                    lines_array[b] = line_endm[0] + std::string(")");
                }
                lines_array.insert(lines_array.begin()+b+1,assigns);
                break;
            }
        } catch(...) {
            cout << "Data Unavailable\n";
        }
        b++;
    }
    int i = line;
    while(1){
        try {
            // use ".at()" and catch the resulting exception if there is any
            // chance that the index is bogus. Since we are reading external files,
            // there is every chance that the index is bogus.
            std::string::size_type pos = lines_array.at(i).find("endmodule");
            if(pos != std::string::npos)
            {
                myfile << lines_array.at(i).c_str();
                //myfile << "\n";
                cout<<lines_array.at(i)<<"\n";
                break;
            }
            else
            {
                myfile << lines_array.at(i).c_str();
                myfile << "\n";
                cout<<lines_array.at(i)<<"\n";
            }
        } catch(...) {
            // deal with error here. Maybe:
            //   the input file doesn't exist
            //   the ifstream creation failed for some other reason
            //   the string reads didn't work
            cout << "Data Unavailable\n";
        }
        i++;
    }
    return file_copy_name;
}
std::vector<std::string> MainWindow::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

void trace_information(string name,string width,string depth){
    ofstream xml;
    xml.open("TraceInfo.xml");
    xml << "<Name>\n";
    xml << "tracebuffer\n";
    xml << "</Name>\n";
    xml << "<FileName>\n";
    xml << name+"\n";
    xml << "</FileName>\n";
    xml << "<Width>\n";
    xml << width+"\n";
    xml << "</Width>\n";
    xml << "<Depth>\n";
    xml << depth+"\n";
    xml << "</Depth>\n";
}

void MainWindow::handleButton2(){
    char *generate_command; // Comando relacionado ao Vericonn
    char *command_veritrace; // Comando relacionado ao Veritrace
    string aux;
    string aux1;
    string aux2;
    tree_crawler2(0,0,aux1,aux,aux2);
}




void MainWindow::handleButton(){
    QWidget *item2;
    // change the text
    //m_button->setText("Example");
    // resize button
    int a = listWidget->count();
    QString text;
    text = "module generic(";
    for (int i = 0; i < a; ++i) {
        // listWidget->itemWidget(listWidget->item(i))->;
        if(listWidget->item(i)->checkState()){
            text = text+listWidget->item(i)->text();
            text = text+",";
        }
        else{
        }
    }
    int testando = text.size();
    //std::string s = std::to_string(testando);
    text.remove(text.size()-1,1);
    text = text+");";
    textb->setText(text);
}

std::vector<std::string> MainWindow::ReadCells(QString toplevel,QString path)
{
    using namespace std;
    std::vector<std::string> myvector;
    std::vector<std::string> myvector2;
    std::string aux = path.toStdString()+"/cells.txt";
    std::ifstream file(aux.c_str()); // pass file name as argment
    std::string linebuffer;
    std::string delimiter = "|";
    int i = 0;
    while (file && getline(file, linebuffer)){

        if (linebuffer.length() == 0)continue;

        if(linebuffer.find("toplevel")==false){

            size_t pos = 0;
            std::string token;
            while ((pos = linebuffer.find(delimiter)) != std::string::npos) {
                token = linebuffer.substr(0, pos);
                //std::cout << token << std::endl;
                myvector.push_back(token);
                linebuffer.erase(0, pos + delimiter.length());
            }
            myvector2.push_back(linebuffer);
        }
    }
    return myvector2;
}
std::vector<std::string> MainWindow::ReadWires(QString toplevel,QString path)
{
    std::vector<std::string> myvector;
    std::string aux = path.toStdString()+"/wires.txt";
    std::ifstream file(aux.c_str()); // pass file name as argment
    std::string linebuffer;
    std::string delimiter = "|";
    int i = 0;
    while (file && getline(file, linebuffer)){
        if (linebuffer.length() == 0)continue;
        if(linebuffer.find("toplevel")==false){
            size_t pos = 0;
            std::string token;
            while ((pos = linebuffer.find(delimiter)) != std::string::npos) {
                token = linebuffer.substr(0, pos);
                linebuffer.erase(0, pos + delimiter.length());
            }
            myvector.push_back(linebuffer);
        }
    }
    return myvector;
}

std::string MainWindow::exec(char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[10000];
    std::string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer,10000, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;
}

void MainWindow::tree_crawler_test()
{

    vector<std::string> wires;
    wires.push_back("a");
    wires.push_back("y");
    copy_from_to("/home/andre/Documentos/yosys-new/yosys/manual/test/absval_ref.v","/home/andre/Documentos/yosys-new/yosys/manual/test/absal_ref2.v",1,wires);
}

void MainWindow::tree_crawler2(int inputs,int outputs, string rede, string includes_names, string trace_name){
    QModelIndex rootindex = global_index;
    QModelIndex current_index;
    QModelIndex top_index;
    QModelIndex current_index_compare;
    QList<QModelIndex> index_stack;
    QList<QModelIndex*> wires_stack;
    QList<int> child_positions;
    std::string wire = "wire";
    QStandardItem *top_item;
    tree_item_module* current_module;
    top_item = static_cast<QStandardItem*> (rootindex.child(0,0).child(0,0).internalPointer());
    std::string location_top = top_item->accessibleDescription().toStdString();
    std::string name_item_top = rootindex.child(0,1).data(Qt::DisplayRole).toString().toStdString();
    std::string type_item_top = rootindex.child(0,0).data(Qt::DisplayRole).toString().toStdString();
    tree_item_module* top_item_module = new tree_item_module(name_item_top,type_item_top,location_top);
    if(rootindex.child(0,0).isValid()){
        current_index = rootindex.child(0,0);
        current_module = top_item_module;
        top_index = current_index;
        //guarda root
    }
    int i = 0;
    while(1){
        if(current_index.child(i,0).isValid()){
            current_index_compare = current_index.child(i,0);
            std::string type_item = current_index_compare.parent().child(i,0).data(Qt::DisplayRole).toString().toStdString();
            std::string name_item = current_index_compare.parent().child(i,1).data(Qt::DisplayRole).toString().toStdString();
            int id_item = std::atoi(current_index_compare.parent().child(i,3).data(Qt::DisplayRole).toString().toStdString().c_str());
            int item_size = std::atoi(current_index_compare.parent().child(i,2).data(Qt::DisplayRole).toString().toStdString().c_str());
            // TRATAMENTO WIRE INICIO
            if(wire.compare(type_item) == 0){
                index_stack << current_index;
                child_positions << i;
                current_index = current_index.child(i,0);
                QStandardItem *subwire_item;
                std::string name_subwire;
                QModelIndex subwire;
                for (i = 0; i < item_size; i++) {
                    subwire = current_index.child(i,0);
                    subwire = subwire.child(0,0);
                    name_subwire = current_index.child(i,1).data(Qt::DisplayRole).toString().toStdString();
                    subwire_item = static_cast<QStandardItem*> (subwire.internalPointer());
                    std::string location = subwire_item->accessibleDescription().toStdString();
                    if (findElementInList(name_subwire)){

                        items_tree.at(id_item)->push_selected_wire(name_subwire);
                        current_module->appendWirePrivate(name_subwire+"*"+"%"+"*"+location+"*"+"%");
                        std::cout << "subs: " << name_subwire << "\n";
                        recursive_module_searching(current_module,top_item_module,name_subwire);
                    }
                }
                current_index = index_stack.last();
                index_stack.pop_back();
                i = child_positions.last()+1;
                child_positions.pop_back();
            }
            // TRATAMENTO WIRE FIM
            //CASO SEJA UM MODULO, TEM QUE COLOCAR ELE COMO TOP DOS INDEX
            else{
                QStandardItem *current_item;
                current_item = static_cast<QStandardItem*> (current_index_compare.parent().child(i,0).child(0,0).internalPointer());
                std::string location_current = current_item->accessibleDescription().toStdString();
                tree_item_module *new_module = new tree_item_module(name_item,type_item,location_current,current_module);
                current_module->appendChild(new_module);
                current_module = new_module;

                index_stack << current_index;
                child_positions << i;
                current_index = current_index.child(i,0);
                i = 0;
            }
        }else{
            if(index_stack.size() == 0){
                break;
            }
            current_index = index_stack.last();
            std::string type_item2 = current_index_compare.parent().child(i,0).data(Qt::DisplayRole).toString().toStdString();
            std::string name_item2 = current_index_compare.parent().child(i,1).data(Qt::DisplayRole).toString().toStdString();
            int id_item2 = std::atoi(current_index_compare.parent().child(i,3).data(Qt::DisplayRole).toString().toStdString().c_str());
            int item_size2 = std::atoi(current_index_compare.parent().child(i,2).data(Qt::DisplayRole).toString().toStdString().c_str());
            index_stack.pop_back();
            current_module = current_module->parentItem();
            i = child_positions.last()+1;
            child_positions.pop_back();
        }
    }

    destiny_folder_global = create_folder(FOLDER_BASE);
    char *command = new char[10000];
    std::string comando = "cp -a ";
    //destiny_folder_global = "/home/andre/Documentos/yosys-new/yosys/manual/test/modified_0";
    depth_searching(top_item_module,inputs,outputs,rede,includes_names,trace_name);
    std::string includes = "";
    stringstream s0,s9;
    s0 << inputs;
    s9 << outputs;
    s0.str("");
    s9.str("");
    int includessize=0;
    string auxi= "";
    while (includessize<=includes_names.size()){
        if (includes_names[includessize]!='\n'){
            auxi = auxi + includes_names[includessize];
        }
        else {
            includes = includes +"\`include \""+auxi+"\"\n";
            auxi = "";
        }
        includessize++;
    }
    for (int var = 0; var < NewFiles.size(); ++var) {
        includes = includes +"\`include \""+NewFiles[var]+"\"\n";
      //  QString b (NewFiles[var].c_str());
    }
    command = new char[10000];
    std::string acent = "`";
    std::string acent2 = "\\`";
    replaceAll(includes,acent,acent2);
    acent = "\"";
    acent2 = "\\\"";
    replaceAll(includes,acent,acent2);
    comando = "echo \""+includes+"\\n$(cat "+top_file+")"+"\" > "+top_file;
    strcpy(command,comando.c_str());
    exec(command);
    std::string print_show = "";
    print_show = print_show + "Created files on "+destiny_folder_global+"\n";
    for (int i = 0; i < NewFiles.size(); ++i) {
        print_show = print_show+"\n"+NewFiles[i].c_str()+".v";
    }
    QString print_Q = QString::fromStdString(print_show);
    //textb->setText(print_Q);
    char *command2 = new char[10000];
    std::string BASE_AUX = FOLDER_BASE;
    std::string comando2 = "cp -a "+BASE_AUX+"/*.v "+destiny_folder_global+"/";
    strcpy(command2,comando2.c_str());
    exec(command2);
    comando2.clear();
    //comando2 = "cp /utils/Assertions/ "+assertion_strcture.at(0).file_name+" "+destiny_folder_global;
    //strcpy(command2,comando2.c_str());
   // exec(command2);
}

void MainWindow::EraseSpace (string& str){
    std::string::iterator end_pos = std::remove(str.begin(), str.end(), ' ');
    str.erase(end_pos, str.end());
}

string MainWindow::Find_Verilog_File ( QList<std::string>& wPrivates, QList<std::string>& wPublics, string& module_line){
    string verilog_file;
    QString info_aux;
    QStringList info_wire;
    if(wPrivates.size() > 0){
        info_aux = wPrivates[0].c_str();
        info_wire = info_aux.split("*", QString::SkipEmptyParts);
        info_aux = info_wire.at(2);
        QStringList split_file = info_aux.split(":", QString::SkipEmptyParts);
        verilog_file = split_file.at(0).toStdString();
        module_line = split_file.at(1).toStdString();
        cout << "aqui1";
    }else{
        info_aux = wPublics[0].c_str();
        info_wire = info_aux.split("*", QString::SkipEmptyParts);
        info_aux = info_wire.at(2);
        QStringList split_file = info_aux.split(":", QString::SkipEmptyParts);
        verilog_file = split_file.at(0).toStdString();
        cout << "aqui1";
    }
    return verilog_file;
}

void MainWindow::EraseBars (string& str){
    boost::erase_all(str,"[");
    boost::erase_all(str,"]");
}

int MainWindow::FindModuleModifyName (vector<string>& module_copy, string module_name, tree_item_module* &top_item_module, string father_name){
    int line_module_current = 0;
    string modified_name = module_name+"_"+father_name;
    EraseSpace(modified_name);
    string module_type;

    for (int i = 0; i < top_item_module->childCount(); ++i) {
        if(top_item_module->child(i)->getName().find(module_name) != std::string::npos){
            module_type = top_item_module->child(i)->getType();
        }
    }

    for (int i = 0; i < module_copy.size(); ++i) {
        string current_line = module_copy[i];
        EraseSpace(current_line);
        if(current_line.find(module_name) != std::string::npos){
            line_module_current = i;
            if(current_line.find(modified_name) != std::string::npos){

            }else{
                string line_replace = module_copy[i];
                replace(line_replace,module_type,modified_name);
                module_copy[i] = line_replace;
            }
            break;
        }
    }

    return line_module_current;
}

int MainWindow::FindLimitHeader(vector<string>& module_copy){
    int line_current = 0;
    string parencomma = ");";
    EraseSpace(parencomma);
    for (int i = 0; i < module_copy.size(); ++i) {
        string current_line = module_copy[i];
        EraseSpace(current_line);
        if(current_line.find(parencomma) != std::string::npos){
            line_current = i;
            break;
        }
    }

    return line_current;
}

int MainWindow::FindLineAssignInsert (vector<string>& module_copy, string line_search, string assign_string, int limit_header){
    int line_current = 0;
    EraseSpace(line_search);
    for (int i = 0; i < module_copy.size(); ++i) {
        string current_line = module_copy[i];
        EraseSpace(current_line);
        if(current_line.find(line_search) != std::string::npos){
            line_current = i;
            if(line_current > limit_header){
                module_copy.insert(module_copy.begin()+(i+1),assign_string);
            }else{
                module_copy.insert(module_copy.begin()+(limit_header+1),assign_string);
            }
        }
    }

    return line_current;
}

int MainWindow::IdentifyHeaderType (vector<string>& module_copy, int& limit_header){
    int type_header = 0;
    string comma = ",";
    string parenrightdotcomma = ");";
    string emptiness = "";
    string paren = "(";
    string copy_line_inputs = "";
    int limit_concat = 0;
    for (int i = limit_header; i > -1; --i) {
        if(limit_concat > 2) break;
        string current_line = module_copy[i];
        if(current_line.find(comma) != std::string::npos){
            if(current_line.find(parenrightdotcomma) != std::string::npos){
                replace(current_line,parenrightdotcomma,emptiness);
            }
            if(current_line.find(paren) != std::string::npos){
                QString line_qstring = QString::fromStdString(current_line);
                QStringList line_split = line_qstring.split("(", QString::SkipEmptyParts);
                current_line = line_split.at(1).toStdString();
                copy_line_inputs = copy_line_inputs + current_line;
                break;
            }
            copy_line_inputs = copy_line_inputs + current_line;
        }
    }

    QString line_qstring = QString::fromStdString(copy_line_inputs);
    line_qstring = line_qstring.simplified();
    QStringList line_split = line_qstring.split(",", QString::SkipEmptyParts);

    line_qstring = line_split.at(0);
    line_qstring = line_qstring.simplified();
    line_split = line_qstring.split(" ", QString::SkipEmptyParts);

    if(line_split.size() > 1){
        type_header = 2; //Output nome);
    }else{
        type_header = 1; // nome); output nome;
    }


    return type_header;
}

void MainWindow::ChangeModuleName(vector<string>& module_copy, string newModuleName, string module_name ){

    for (int i = 0; i < module_copy.size(); ++i) {
        string current_line = module_copy[i];
        EraseSpace(current_line);
        if(current_line.find(module_name) != std::string::npos){
            string line_replace = module_copy[i];
            replace(line_replace,module_name,newModuleName);
            module_copy[i] = line_replace;
            break;
        }
    }

}

void MainWindow::ModifyHeader (vector<string>& module_copy, vector<string>& outputs_extern_public, vector<string>& connect_wires_public, vector<string>& assigns_public, vector<string>& outputs_extern_private, vector<string>& assigns_private, int& limit_header, int& type_header, int toplevel){
    string comma = ",";
    string line_limit_header = module_copy[limit_header];
    string parenrightdotcomma = ");";
    string emptiness = "";
    EraseSpace(line_limit_header);
    if(type_header == 1){
        if(line_limit_header.size() == 2){
            /*
                fio
                );
                output fio;
                wire fio2;
                assign fio = fio2;
            */
            if(toplevel == 1){
                int qty_wires = outputs_extern_private.size()+outputs_extern_public.size();
                string previous_line = module_copy[limit_header-1];
                previous_line = previous_line + ",";
                module_copy[limit_header-1] = previous_line;
                string wire_output = "out_debug" ;
                module_copy.insert(module_copy.begin()+(limit_header),wire_output);
                limit_header++;

                string end_assign = "};";
                module_copy.insert(module_copy.begin()+(limit_header+1),end_assign);

                for (int i = 0; i < outputs_extern_public.size(); ++i) {
                    string wire_declare;
                    if((i == 0) /*|| (i == (outputs_extern_public.size()-1))*/){
                        wire_declare = connect_wires_public[i] + "\n";
                    }else{
                        wire_declare = connect_wires_public[i] + ",\n";
                    }
                    module_copy.insert(module_copy.begin()+(limit_header+1),wire_declare);

                }
                for (int i = 0; i < outputs_extern_private.size(); ++i) {
                    string wire_declare;
                    if((i == 0) && (outputs_extern_public.size() == 0)/*|| (i == (outputs_extern_public.size()-1))*/){
                        wire_declare = outputs_extern_private[i] + "\n";
                    }else{
                        wire_declare = outputs_extern_private[i] + ",\n";
                    }
                    module_copy.insert(module_copy.begin()+(limit_header+1),wire_declare);

                }
                string assign_begin = "assign out_debug = {";
                module_copy.insert(module_copy.begin()+(limit_header+1),assign_begin);

                for (int i = 0; i < outputs_extern_public.size(); ++i) {
                    string wire_declare;
                    wire_declare = "wire " + connect_wires_public[i] + ";\n";
                    module_copy.insert(module_copy.begin()+(limit_header+1),wire_declare);

                }
                for (int i = 0; i < outputs_extern_private.size(); ++i) {
                    string wire_declare;
                    wire_declare = "wire " + outputs_extern_private[i] + ";\n";
                    module_copy.insert(module_copy.begin()+(limit_header+1),wire_declare);

                }

                string wire_output_declare = "output["+boost::lexical_cast<string>(qty_wires)+":0]" + wire_output + ";";
                module_copy.insert(module_copy.begin()+(limit_header+1),wire_output_declare);

            }else{
                for (int i = 0; i < outputs_extern_private.size(); ++i) {
                    string previous_line = module_copy[limit_header-1];
                    previous_line = previous_line + ",";
                    module_copy[limit_header-1] = previous_line;
                    string wire_output = outputs_extern_private[i];
                    module_copy.insert(module_copy.begin()+(limit_header),wire_output);
                    limit_header++;

                    string wire_output_declare = "output " + outputs_extern_private[i] + "; \n";
                    module_copy.insert(module_copy.begin()+(limit_header+1),wire_output_declare);

                }
                for (int i = 0; i < outputs_extern_public.size(); ++i) {
                    string previous_line = module_copy[limit_header-1];
                    previous_line = previous_line + ",";
                    module_copy[limit_header-1] = previous_line;
                    string wire_output = outputs_extern_public[i];
                    module_copy.insert(module_copy.begin()+(limit_header),wire_output);
                    limit_header++;

                    string wire_assign = assigns_public[i];
                    module_copy.insert(module_copy.begin()+(limit_header+1),wire_assign);
                    string wire_declare = "wire " + connect_wires_public[i] + "; \n";
                    module_copy.insert(module_copy.begin()+(limit_header+1),wire_declare);
                    string wire_output_declare = "output " + outputs_extern_public[i] + "; \n";
                    module_copy.insert(module_copy.begin()+(limit_header+1),wire_output_declare);

                }
            }

        }else{
            /*
                    fio,fio);
                    output fio;
                    wire fio2;
                    assign fio = fio2;
            */

            /*
                    fio,
                    fio );
                    output fio;
                    wire fio2;
                    assign fio = fio2;
            */

            string line_replace_dotcomma = module_copy[limit_header];
            replace(line_replace_dotcomma,parenrightdotcomma,emptiness);
            module_copy[limit_header] = line_replace_dotcomma;
            module_copy.insert(module_copy.begin()+(limit_header+1),parenrightdotcomma);
            limit_header++;
            if(toplevel == 1){
                int qty_wires = outputs_extern_private.size()+outputs_extern_public.size();
                string previous_line = module_copy[limit_header-1];
                previous_line = previous_line + ",";
                module_copy[limit_header-1] = previous_line;
                string wire_output = "out_debug" ;
                module_copy.insert(module_copy.begin()+(limit_header),wire_output);
                limit_header++;

                string end_assign = "};";
                module_copy.insert(module_copy.begin()+(limit_header+1),end_assign);

                for (int i = 0; i < outputs_extern_public.size(); ++i) {
                    string wire_declare;
                    if((i == 0) /*|| (i == (outputs_extern_public.size()-1))*/){
                        wire_declare = connect_wires_public[i] + "\n";
                    }else{
                        wire_declare = connect_wires_public[i] + ",\n";
                    }
                    module_copy.insert(module_copy.begin()+(limit_header+1),wire_declare);

                }
                for (int i = 0; i < outputs_extern_private.size(); ++i) {
                    string wire_declare;
                    if((i == 0) && (outputs_extern_public.size() == 0)/*|| (i == (outputs_extern_public.size()-1))*/){
                        wire_declare = outputs_extern_private[i] + "\n";
                    }else{
                        wire_declare = outputs_extern_private[i] + ",\n";
                    }
                    module_copy.insert(module_copy.begin()+(limit_header+1),wire_declare);

                }
                string assign_begin = "assign out_debug = {";
                module_copy.insert(module_copy.begin()+(limit_header+1),assign_begin);

                for (int i = 0; i < outputs_extern_public.size(); ++i) {
                    string wire_declare;
                    wire_declare = "wire " + connect_wires_public[i] + ";\n";
                    module_copy.insert(module_copy.begin()+(limit_header+1),wire_declare);

                }
                for (int i = 0; i < outputs_extern_private.size(); ++i) {
                    string wire_declare;
                    wire_declare = "wire " + outputs_extern_private[i] + ";\n";
                    module_copy.insert(module_copy.begin()+(limit_header+1),wire_declare);

                }

                string wire_output_declare = "output["+boost::lexical_cast<string>(qty_wires)+":0]" + wire_output + ";";
                module_copy.insert(module_copy.begin()+(limit_header+1),wire_output_declare);

            }else{
                for (int i = 0; i < outputs_extern_private.size(); ++i) {
                    string previous_line = module_copy[limit_header-1];
                    previous_line = previous_line + ",";
                    module_copy[limit_header-1] = previous_line;
                    string wire_output = outputs_extern_private[i];
                    module_copy.insert(module_copy.begin()+(limit_header),wire_output);
                    limit_header++;

                    string wire_output_declare = "output " + outputs_extern_private[i] + "; \n";
                    module_copy.insert(module_copy.begin()+(limit_header+1),wire_output_declare);

                }
                for (int i = 0; i < outputs_extern_public.size(); ++i) {
                    string previous_line = module_copy[limit_header-1];
                    previous_line = previous_line + ",";
                    module_copy[limit_header-1] = previous_line;
                    string wire_output = outputs_extern_public[i];
                    module_copy.insert(module_copy.begin()+(limit_header),wire_output);
                    limit_header++;

                    string wire_assign = assigns_public[i];
                    module_copy.insert(module_copy.begin()+(limit_header+1),wire_assign);
                    string wire_declare = "wire " + connect_wires_public[i] + "; \n";
                    module_copy.insert(module_copy.begin()+(limit_header+1),wire_declare);
                    string wire_output_declare = "output " + outputs_extern_public[i] + "; \n";
                    module_copy.insert(module_copy.begin()+(limit_header+1),wire_output_declare);

                }
            }

        }
    }
    if(type_header == 2){
        if(line_limit_header.size() == 2){
            /*
                output fio
                );
                wire fio2;
            */
            if(toplevel == 1){
                int qty_wires = outputs_extern_private.size()+outputs_extern_public.size();
                string previous_line = module_copy[limit_header-1];
                previous_line = previous_line + ",";
                module_copy[limit_header-1] = previous_line;
                string wire_output = "output ["+boost::lexical_cast<string>(qty_wires)+":0]" + " out_debug";
                module_copy.insert(module_copy.begin()+(limit_header),wire_output);
                limit_header++;

                string end_assign = "};";
                module_copy.insert(module_copy.begin()+(limit_header+1),end_assign);

                for (int i = 0; i < outputs_extern_public.size(); ++i) {
                    string wire_declare;
                    if((i == 0) /*|| (i == (outputs_extern_public.size()-1))*/){
                        wire_declare = connect_wires_public[i] + "\n";
                    }else{
                        wire_declare = connect_wires_public[i] + ",\n";
                    }
                    module_copy.insert(module_copy.begin()+(limit_header+1),wire_declare);

                }
                for (int i = 0; i < outputs_extern_private.size(); ++i) {
                    string wire_declare;
                    if((i == 0) && (outputs_extern_public.size() == 0)/*|| (i == (outputs_extern_public.size()-1))*/){
                        wire_declare = outputs_extern_private[i] + "\n";
                    }else{
                        wire_declare = outputs_extern_private[i] + ",\n";
                    }
                    module_copy.insert(module_copy.begin()+(limit_header+1),wire_declare);

                }
                string assign_begin = "assign out_debug = {";
                module_copy.insert(module_copy.begin()+(limit_header+1),assign_begin);

                for (int i = 0; i < outputs_extern_public.size(); ++i) {
                    string wire_declare;
                    wire_declare = "wire " + connect_wires_public[i] + ";\n";
                    module_copy.insert(module_copy.begin()+(limit_header+1),wire_declare);

                }
                for (int i = 0; i < outputs_extern_private.size(); ++i) {
                    string wire_declare;
                    wire_declare = "wire " + outputs_extern_private[i] + ";\n";
                    module_copy.insert(module_copy.begin()+(limit_header+1),wire_declare);

                }

            }else{
                for (int i = 0; i < outputs_extern_private.size(); ++i) {
                    string previous_line = module_copy[limit_header-1];
                    previous_line = previous_line + ",";
                    module_copy[limit_header-1] = previous_line;
                    string wire_output = "output " + outputs_extern_private[i];
                    module_copy.insert(module_copy.begin()+(limit_header),wire_output);
                    limit_header++;

                }
                for (int i = 0; i < outputs_extern_public.size(); ++i) {
                    string previous_line = module_copy[limit_header-1];
                    previous_line = previous_line + ",";
                    module_copy[limit_header-1] = previous_line;
                    string wire_output = "output " + outputs_extern_public[i];
                    module_copy.insert(module_copy.begin()+(limit_header),wire_output);
                    limit_header++;

                    string wire_assign = assigns_public[i];
                    module_copy.insert(module_copy.begin()+(limit_header+1),wire_assign);
                    string wire_declare = "wire " + connect_wires_public[i] + "; \n";
                    module_copy.insert(module_copy.begin()+(limit_header+1),wire_declare);

                }
            }
        }else{
            /*
                    output fio,output fio);
                    wire fio2;
            */

            /*
                    output fio,
                    output fio );
                    wire fio2;
            */

            string line_replace_dotcomma = module_copy[limit_header];
            replace(line_replace_dotcomma,parenrightdotcomma,emptiness);
            module_copy[limit_header] = line_replace_dotcomma;
            module_copy.insert(module_copy.begin()+(limit_header+1),parenrightdotcomma);
            limit_header++;
            if(toplevel == 1){
                int qty_wires = outputs_extern_private.size()+outputs_extern_public.size();
                string previous_line = module_copy[limit_header-1];
                previous_line = previous_line + ",";
                module_copy[limit_header-1] = previous_line;
                string wire_output = "output ["+boost::lexical_cast<string>(qty_wires)+":0]" + " out_debug";
                module_copy.insert(module_copy.begin()+(limit_header),wire_output);
                limit_header++;

                string end_assign = "};";
                module_copy.insert(module_copy.begin()+(limit_header+1),end_assign);

                for (int i = 0; i < outputs_extern_public.size(); ++i) {
                    string wire_declare;
                    if((i == 0) /*|| (i == (outputs_extern_public.size()-1))*/){
                        wire_declare = connect_wires_public[i] + "\n";
                    }else{
                        wire_declare = connect_wires_public[i] + ",\n";
                    }
                    module_copy.insert(module_copy.begin()+(limit_header+1),wire_declare);

                }
                for (int i = 0; i < outputs_extern_private.size(); ++i) {
                    string wire_declare;
                    if((i == 0) && (outputs_extern_public.size() == 0)/*|| (i == (outputs_extern_public.size()-1))*/){
                        wire_declare = outputs_extern_private[i] + "\n";
                    }else{
                        wire_declare = outputs_extern_private[i] + ",\n";
                    }
                    module_copy.insert(module_copy.begin()+(limit_header+1),wire_declare);

                }
                string assign_begin = "assign out_debug = {";
                module_copy.insert(module_copy.begin()+(limit_header+1),assign_begin);

                for (int i = 0; i < outputs_extern_public.size(); ++i) {
                    string wire_declare;
                    wire_declare = "wire " + connect_wires_public[i] + ";\n";
                    module_copy.insert(module_copy.begin()+(limit_header+1),wire_declare);

                }
                for (int i = 0; i < outputs_extern_private.size(); ++i) {
                    string wire_declare;
                    wire_declare = "wire " + outputs_extern_private[i] + ";\n";
                    module_copy.insert(module_copy.begin()+(limit_header+1),wire_declare);

                }

            }else{
                for (int i = 0; i < outputs_extern_private.size(); ++i) {
                    string previous_line = module_copy[limit_header-1];
                    previous_line = previous_line + ",";
                    module_copy[limit_header-1] = previous_line;
                    string wire_output = "output " + outputs_extern_private[i];
                    module_copy.insert(module_copy.begin()+(limit_header),wire_output);
                    limit_header++;

                }
                for (int i = 0; i < outputs_extern_public.size(); ++i) {
                    string previous_line = module_copy[limit_header-1];
                    previous_line = previous_line + ",";
                    module_copy[limit_header-1] = previous_line;
                    string wire_output = "output " + outputs_extern_public[i];
                    module_copy.insert(module_copy.begin()+(limit_header),wire_output);
                    limit_header++;

                    string wire_assign = assigns_public[i];
                    module_copy.insert(module_copy.begin()+(limit_header+1),wire_assign);
                    string wire_declare = "wire " + connect_wires_public[i] + "; \n";
                    module_copy.insert(module_copy.begin()+(limit_header+1),wire_declare);

                }
            }


        }
    }
}

void MainWindow::FindTypeDeclareAddWire (vector<string>& module_copy, int line_module, string wire_before_parenthesis, string connection_wire){
    int i = line_module;
    int found_dot_paren = 0;
    string line;
    string line_old;
    string dot = ".";
    string comma = ",";
    string commajump = ",\n";
    string paren = "(";
    string dotcomma = ";";
    string parenright = ")";
    string parenrightdotcomma = ");";
    string emptiness = "";
    string line_replace;

    if(module_copy.size() > line_module){
        line = module_copy[i];
    }else{
        return;
    }

    do{
        if((line.find(dot) != std::string::npos) && (line.find(paren) != std::string::npos)){
            found_dot_paren = 1;
        }
        i++;
        line_old = line;
        line = module_copy[i];
    }while(!((line_old.find(dotcomma) != std::string::npos) && (line_old.find(parenright) != std::string::npos)));

    string wire_string;
    if(found_dot_paren == 1){
        wire_string = "."+wire_before_parenthesis+"("+connection_wire+")";
    }else{
        wire_string = connection_wire;
    }

    EraseSpace(line_old);
    string line_limit_header = line_old;
    int limit_header = i-1;
    if(line_limit_header.size() == 2){
        /*
            fio
            );
        */
        line_replace = module_copy[limit_header-1];
        line_replace = line_replace + ",";
        module_copy[limit_header-1] = line_replace;
        module_copy.insert(module_copy.begin()+(limit_header),wire_string);

    }else{
        /*
                fio,fio);
        */

        /*
                fio,
                fio );
        */
        string line_replace_dotcomma = module_copy[limit_header];
        replace(line_replace_dotcomma,parenrightdotcomma,emptiness);
        module_copy[limit_header] = line_replace_dotcomma;
        module_copy.insert(module_copy.begin()+(limit_header+1),parenrightdotcomma);
        line_replace = module_copy[limit_header];
        line_replace = line_replace + ",";
        module_copy[limit_header] = line_replace;
        module_copy.insert(module_copy.begin()+(limit_header+1),wire_string);
    }
    /*string line_replace;// = module_copy[i-1];
    //Here we check if there is a comma with ); because if so, we have to use a different method to replace it.
    if(line_replace.find(comma) != std::string::npos){
        line_replace = module_copy[i-1];
        replace(line_replace,parenrightdotcomma,commajump);
        module_copy[i-1] = line_replace;
        module_copy.insert(module_copy.begin()+(i),wire_string);
    }else{
        line_replace = module_copy[i-2];
        line_replace = line_replace + ",";
        module_copy[i-2] = line_replace;
        module_copy.insert(module_copy.begin()+(i-1),wire_string);
    }*/

    cout << "aqui";

}

void MainWindow::CleanComents(vector<string>& module_copy){
    string comment_line = "//";
    string comment_block_begin = "/*";
    string comment_block_end   = "*/";
    string emptiness = "";
    string current_line = "";
    string barleft = "/";
    QString line_aux;
    QStringList split_line;

    for (int i = 0; i < module_copy.size(); ++i) {
        current_line = module_copy[i];
        if(current_line.find(comment_line) != std::string::npos){
            size_t equals_idx = current_line.find_first_of('//');
            if (std::string::npos != equals_idx)
            {
                module_copy[i] = current_line.substr(0, equals_idx);
            }
            else
            {
                module_copy[i] = emptiness;
            }
        }

        current_line = module_copy[i];
        if(current_line.find(comment_block_begin) != std::string::npos){
            if(current_line.find(comment_block_end) != std::string::npos){

            }else{
                size_t equals_idx = current_line.find_first_of('/*');
                if (std::string::npos != equals_idx)
                {
                    string subaux = current_line.substr(0, equals_idx);
                    EraseSpace(subaux);
                    if((subaux.size() == 1) && (subaux.find(barleft) != std::string::npos)){
                        module_copy[i] = emptiness;
                    }else{
                        module_copy[i] = current_line.substr(0, equals_idx);
                    }

                }
                else
                {
                    module_copy[i] = emptiness;
                }
                i++;
                while(!(current_line.find(comment_block_end) != std::string::npos)){
                    module_copy[i] = emptiness;
                    i++;
                    current_line = module_copy[i];
                }

                equals_idx = current_line.find_first_of('*/');
                if (std::string::npos != equals_idx)
                {
                    string subaux = current_line.substr(equals_idx+1);
                    EraseSpace(subaux);
                    if((subaux.size() == 1) && (subaux.find(barleft) != std::string::npos)){
                        module_copy[i] = emptiness;
                    }else{
                        module_copy[i] = current_line.substr(equals_idx+1);
                    }
                }
                else
                {
                    module_copy[i] = emptiness;
                }
            }


        }
    }
}

bool MainWindow::findElementInList(std::string wire)
{
    if(std::find(signalsToExtern.begin(), signalsToExtern.end(), wire) != signalsToExtern.end())
    {
        return true;
      //elem exists in the vector
    }
    else{
        return false;
    }

}

void MainWindow::CopyModule (vector<string>& module_copy, string file, string line_module, vector<string>& lines){
    vector<string> module_copy2;

    int linecount = 0 ;
    std::string line ;
    std::ifstream infile(file.c_str());
    if ( infile ) {
        while ( getline( infile , line ) ) {
            lines.push_back(line);
            linecount++ ;
        }
    }else{
        return;
    }

    std::string endmodule("endmodule");
    std::string module("module");
    int line_copy_start = atoi(line_module.c_str());
    int line_module_start = 0;
    for (int j = line_copy_start-1; j >= 0; --j) {
        line = lines[j];
        std::size_t found = line.find(module);
        if (found!=std::string::npos){
            //module_copy.push_back(line);
            line_module_start = j;
            break;

        }else{
            //module_copy.push_back(line);

        }
    }

    for (int j = line_module_start; j < lines.size(); ++j) {
        line = lines[j];
        std::size_t found = line.find(endmodule);
        if (found!=std::string::npos){
            module_copy.push_back(line);
            break;

        }else{
            module_copy.push_back(line);

        }
    }




}

std::string MainWindow::create_folder (std::string base_folder){
    char *command = new char[500];
    int i = 0;
    std::string destiny_folder = base_folder+"/modified_"+boost::lexical_cast<std::string>(i);

    /*std::string::iterator end_pos = std::remove(destiny_folder.begin(), destiny_folder.end(), ' ');
    destiny_folder.erase(end_pos, destiny_folder.end());*/
    EraseSpace(destiny_folder);
    if (check_folder_existence(destiny_folder)){
        std::string comando = "rm -rf "+destiny_folder;
        strcpy(command,comando.c_str());
        exec(command);
    }
    while(check_folder_existence(destiny_folder)){
        i++;
        destiny_folder = base_folder+"/modified_"+boost::lexical_cast<std::string>(i);
        EraseSpace(destiny_folder);
    }
    if(i > 0){
        destiny_folder = base_folder+"/modified_"+boost::lexical_cast<std::string>(i);
        EraseSpace(destiny_folder);
        //boost::filesystem::create_directory(destiny_folder.c_str());
        std::string comando = "mkdir "+destiny_folder;
        strcpy(command,comando.c_str());
        exec(command);
        return destiny_folder;
    }else{
        //boost::filesystem::create_directory(destiny_folder.c_str());
        destiny_folder = base_folder+"/modified_0";
        EraseSpace(destiny_folder);
        std::string comando = "mkdir "+destiny_folder;
        strcpy(command,comando.c_str());
        exec(command);
        return destiny_folder;
    }
}



bool MainWindow::check_folder_existence (std::string folder){
    EraseSpace(folder);
    struct stat info;
    if( stat( folder.c_str(), &info ) != 0 ){
       // printf( "cannot access %s\n", folder.c_str() );
        return false;
    }
    else if( info.st_mode & S_IFDIR ){  // S_ISDIR() doesn't exist on my windows
       // printf( "existe", folder.c_str() );
        return true;
    }
    else{
       // printf( "Nao existe", folder.c_str() );
        return false;
    }
}

void MainWindow::depth_searching (tree_item_module* top_item_module,int inputs,int outputs,string rede, string includes_names, string trace_name){
    bool top = false;

    QList<std::string> wPrivates = top_item_module->getPrivateWires();
    QList<std::string> wPublics = top_item_module->getPublicWires();

    if((wPrivates.size() == 0) && (wPublics.size() == 0) ) return; //If there are no wires from inner modules (wPublics) or wires from current module (wPrivates), there is nothing to do here.

    //#############################MODIFYFILE###########################################################
    string out_assign = "";
    string module_name = top_item_module->getName();
    string module_type = top_item_module->getType();
    string module_location = top_item_module->getLocation();
    string name_father;
    string verilog_file;
    string file = "";
    cout << "aqui";

    QString info_aux;
    QStringList info_wire;
    string module_line;
    verilog_file = Find_Verilog_File(wPrivates,wPublics,module_line); //Find the Verilog file name for this module.
    QString file_qstring = QString::fromStdString(verilog_file);
    QStringList file_split = file_qstring.split("/", QString::SkipEmptyParts);
    verilog_file = file_split.last().toStdString();

    file = FOLDER_BASE+"/"+verilog_file;
    EraseSpace(file);
    vector<string> module_copy;

    vector<string> outputs_extern_public;
    vector<string> connect_wires_public;
    vector<string> assigns_public;

    vector<string> outputs_extern_private;
    vector<string> assigns_private; //model = <assign statement>:<line to look for, line where the wire was declared>
    vector<string> lines;
    CopyModule(module_copy,file,module_line, lines); //Copies current module content into "module_copy"
    CleanComents(module_copy); //Remove all comments from "module_copy" in order to avoid mismatches
    int limit_header = FindLimitHeader(module_copy); //Finds the line where the header finishes, used for knowing if a wire is a port or an internal wire.

    cout << "aqui";

    if(top_item_module->parentItem() != NULL){
        name_father = top_item_module->parentItem()->getName();
    }else{
        name_father = "toplevel";
    }

    //#############CHECK IF MODULE IS TOP LEVEL#######################

    string toplevel_aux = "topmodule";
    string topmodule_compare = module_type;
    int toplevel = 0;

    EraseSpace(toplevel_aux);
    EraseSpace(topmodule_compare);

    string fileNameWrite;
    string current_module_name;
    string new_module_name;

    if(topmodule_compare.find(toplevel_aux) != std::string::npos){
        toplevel = 1;
        current_module_name = module_name;
        new_module_name = current_module_name+"_"+name_father;
    }else{
        current_module_name = module_type;
        new_module_name = module_name+"_"+name_father;
    }
    //#############CHECK IF MODULE IS TOP LEVEL#######################

    for (int h = 0; h < wPublics.size(); ++h) {        

        info_aux = wPublics[h].c_str();
        info_wire = info_aux.split("*", QString::SkipEmptyParts);

        std::string wire_out_name = info_wire[0].toStdString();
        boost::erase_all(wire_out_name,"[");
        boost::erase_all(wire_out_name,"]");
        std::string wire_inner_name = info_wire[1].toStdString();
        boost::erase_all(wire_inner_name,"[");
        boost::erase_all(wire_inner_name,"]");

        info_aux = info_wire.at(3);
        QStringList split_module_name = info_aux.split(":", QString::SkipEmptyParts);
        string name_inner_module = split_module_name.at(1).toStdString();
        string modified_name = name_inner_module+"_"+module_name;

        string wire_before_parenthesis = wire_inner_name+"_out";
        string output_name_for_wire = wire_out_name+"_out";
        string connection_wire = wire_inner_name+"_"+modified_name+"_out";
        string wire_assign  = "assign "+output_name_for_wire+" = "+connection_wire+";";

        outputs_extern_public.push_back(output_name_for_wire);
        connect_wires_public.push_back(connection_wire);
        assigns_public.push_back(wire_assign);



        int line_module = FindModuleModifyName(module_copy,name_inner_module,top_item_module,module_name); //Finds module line and modifies name if not modified yet
        FindTypeDeclareAddWire(module_copy,line_module,wire_before_parenthesis,connection_wire); //Add the wire to a module instance which we modified the name on the previous line.
        cout << "aqui";



    }

    for (int h = 0; h < wPrivates.size(); ++h) {
        info_aux = wPrivates[h].c_str();
        info_wire = info_aux.split("*", QString::SkipEmptyParts);
        cout << "aqui";
        std::string wire_out_name = info_wire[0].toStdString();
        boost::erase_all(wire_out_name,"[");
        boost::erase_all(wire_out_name,"]");
        info_aux = info_wire.at(2);
        QStringList split_wire_location = info_aux.split(":", QString::SkipEmptyParts);
        string location_wire_on_file = split_wire_location.at(1).toStdString();
        EraseSpace(location_wire_on_file);
        int location_wire_num = atoi(location_wire_on_file.c_str()) - 1;


        string output_name_for_wire = wire_out_name+"_out";
        string connection_wire = info_wire[0].toStdString();
        string wire_assign  = "assign "+output_name_for_wire+" = "+connection_wire+";";
        if(toplevel == 1){
            wire_assign = wire_assign + "|" + lines[location_wire_num];
        }else{
            wire_assign = wire_assign + "|" + lines[location_wire_num];
        }
        //The model for the assign for private wires is <assign statement>|<line where wire was declared>

        outputs_extern_private.push_back(output_name_for_wire);
        assigns_private.push_back(wire_assign);

    }

    for (int i = 0; i < assigns_private.size(); ++i) {
        info_aux = assigns_private[i].c_str();
        QStringList split_assign = info_aux.split("|", QString::SkipEmptyParts);
        string line_assign_search = split_assign.at(1).toStdString();
        string assign_string = split_assign.at(0).toStdString();
        int line_assign = FindLineAssignInsert(module_copy,line_assign_search,assign_string,limit_header); // Inserts the wire below its declaration.
        cout << "aqui";
    }


    int type_header = IdentifyHeaderType(module_copy,limit_header);    

    ModifyHeader(module_copy,outputs_extern_public,connect_wires_public,assigns_public,outputs_extern_private,assigns_private,limit_header, type_header, toplevel); //Adds all the new ports to the header.

    //#############################MODIFYFILE###########################################################    

    fileNameWrite = new_module_name + ".v";
    EraseSpace(fileNameWrite);

    ChangeModuleName(module_copy,new_module_name,current_module_name); //Changes current module name.
    cout << "aqui";
    string Folder_Write = destiny_folder_global;
    WriteTofile(module_copy,fileNameWrite); //Write the final result to a new file inside the "modified" folder

    tree_item_module* current_module;
    if(top_item_module->childCount() > 0){
        for (int i = 0; i < top_item_module->childCount(); ++i) {
            depth_searching(top_item_module->child(i),inputs,outputs,rede,includes_names,trace_name);
        }
    }else{
        return;
    }

}

void MainWindow::WriteTofile(vector<string>& module_copy,string fileNameWrite){
    char *command = new char[2000] ;
    std::string comando = "";
    string file_write = destiny_folder_global+"/"+fileNameWrite;
    comando = "touch "+ file_write;
    strcpy(command,comando.c_str());
    exec(command);

    std::ofstream output_file(file_write.c_str());
    std::ostream_iterator<std::string> output_iterator(output_file, "\n");
    std::copy(module_copy.begin(), module_copy.end(), output_iterator);
}

void MainWindow::replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

bool MainWindow::replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}
/* std::string MainWindow::modify_private (QList<std::string> wPrivates){
   for (int j = 0; j < wPrivates.size(); ++j) {
        QString info_aux(wPrivates[j].c_str());
        QStringList info_wire = info_aux.split("*", QString::SkipEmptyParts);
        QStringList file_line = info_wire.at(2).split(":",QString::SkipEmptyParts);
        std::ofstream myfile;
        std::string file = FOLDER_BASE+"/"+file_line.at(0).toStdString();
         vector<string> lines;
         vector<string> module_copy;


        int linecount = 0 ;
        std::string line ;
        std::ifstream infile(file.c_str());
        if ( infile ) {
           while ( getline( infile , line ) ) {
          std::cout << linecount << ": " << line << '\n' ;//supposing '\n' to be line end
          lines.push_back(line);
          linecount++ ;
           }
        }

        int extra_line_count = 0;

        std::string endmodule("endmodule");
        int line_copy_start = atoi(file_line[1].toStdString().c_str());

        for (int j = line_copy_start-1; j < lines.size(); ++j) {
            line = lines[j];
            std::size_t found = line.find(endmodule);
              if (found!=std::string::npos){
                  module_copy.push_back(line);
                std::cout << "first 'needle' found at: " << found << '\n';
                  break;

              }else{
                  module_copy.push_back(line);

              }
        }


        infile.close( ) ;


    }

}*/

void MainWindow::recursive_module_searching (tree_item_module* current_module,tree_item_module* top_item_module,std::string wire_name){
    tree_item_module* recursive_module_search = current_module;
    QList<std::string> names_modules;
    QList<std::string> types_modules;
    names_modules << recursive_module_search->getName();
    types_modules << recursive_module_search->getType();
    std::string location = recursive_module_search->getLocation();
    recursive_module_search = recursive_module_search->parentItem();

    while(recursive_module_search != top_item_module->parentItem()){

        std::string name_wire = "";
        for (int l = 0; l < names_modules.size(); l++) {
            name_wire = name_wire + names_modules[l] + "_";

        }
        name_wire = name_wire + wire_name;
        name_wire = name_wire + "*";
        for (int l = 0; l < names_modules.size()-1; l++) {
            name_wire = name_wire + names_modules[l] + "_";

        }
        name_wire = name_wire + wire_name;
        name_wire = name_wire + "*";
        name_wire = name_wire + location;
        name_wire = name_wire + "*";
        name_wire = name_wire + types_modules.last();
        name_wire = name_wire + ":";
        name_wire = name_wire + names_modules.last();

        recursive_module_search->appendWirePublic(name_wire);
        location = recursive_module_search->getLocation();
        names_modules << recursive_module_search->getName();
        types_modules << recursive_module_search->getType();
        recursive_module_search = recursive_module_search->parentItem();
    }
}


//------------------------------------------------------------------------------------

//! [quoting modelview_b]

//! [quoting modelview_b]

/*void MainWindow::buttonn(const QItemSelection & /*newSelection, const QItemSelection & /*oldSelection)
{
    //get the text of the selected item

    const QModelIndex index_aux = treeView->selectionModel()->currentIndex();
    //const QModelIndex index = global_index.child(index_aux.row(),0);
    const QModelIndex index = index_aux.parent().child(index_aux.row(),0);
    const QModelIndex indexaux = index_aux.parent().child(index_aux.row(),1);



    QString row = QString::number(index.row());
    QString column = QString::number(index.column());
    const QModelIndex index2 = index.child(0,0);
    qDebug("row "+row.toLatin1()+"column "+column.toLatin1());
    //    QModelIndex index2 = new QModelIndex(index.row(),0);
    QString selectedText = index.data(Qt::DisplayRole).toString();
    //find out the hierarchy level of the selected item
    int hierarchyLevel=1;
    QModelIndex seekRoot = index;
    while(seekRoot.parent() != QModelIndex())
    {
        seekRoot = seekRoot.parent();
        hierarchyLevel++;
    }
    QString showString = QString("%1, Level %2").arg(selectedText)
            .arg(hierarchyLevel);
    setWindowTitle(showString);
}*/

QList<QStandardItem *> MainWindow::prepareRow(const QString &first,
                                              const QString &second,
                                              const QString &third,
                                              const QString &ID)
{
    std::string subwire = "wiresub";
    QList<QStandardItem *> rowItems;
    QStandardItem *fir = new QStandardItem(first);
    if(subwire.compare(first.toStdString())==0){
        fir->setFlags(fir->flags()| Qt::ItemIsUserCheckable);
        fir->setCheckState(Qt::Unchecked);
    }
    QStandardItem *sec = new QStandardItem(second);
    QStandardItem *thir = new QStandardItem(third);
    QStandardItem *forth = new QStandardItem(ID);
    fir->setWhatsThis(ID);
    sec->setWhatsThis(ID);
    thir->setWhatsThis(ID);
    forth->setWhatsThis(ID);
    rowItems << fir;
    rowItems << sec;
    rowItems << thir;
    rowItems << forth;
    /*rowItems << a;
    rowItems << new QStandardItem(second);
    rowItems << new QStandardItem(third);
    rowItems << new QStandardItem(ID);*/
    return rowItems;
}

void MainWindow::setupModelData(const QStringList &lines, QStandardItem *parent){
    string name = "";
    string type = "";
    int size = 0;
    int id = 0;
    int idfather = 0;


    QList<QStandardItem*> parents;
    QList<int> indentations;
    QList<int> fathers;

    parents << parent;
    fathers << 0;
    indentations << 0;

    int number = 0;
    QString numero_l = QString::number(lines.count());
   // qDebug("aqui: "+numero_l.toLatin1()+"\n");
    while (number < lines.count()) {
        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].mid(position, 1) != " ")
                break;
            position++;
        }

        QString lineData = lines[number].mid(position);

        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            QStringList columnStrings = lineData.split("|", QString::SkipEmptyParts);
            int tamanho = columnStrings.count();

            if (position > indentations.last()) {
                if(name!= string("wire")){
                    fathers << id;
                }else{
                    fathers << fathers.last();
                }


                // The last child of the current parent is now the new parent
                // unless the current parent has no children.
                int child_count = parents.last()->rowCount();


                if (child_count > 0) {
                    /*QStandardItem auxx = parents.last()->child(child_count);
                    parents << &auxx;*/
                    parents << parents.last()->child(child_count-1);
                    //std::cout << "aaaaaaa" << columnStrings[0].toStdString() << "\n";
                    indentations << position;


                }
            } else {
                while (position < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                    //if(fathers.size() > 0){
                    fathers.pop_back();
                    //   }
                }
            }

            // Append a new item to the current parent's list of children.
            QList<QStandardItem *> thirdRow ;
            std::string aaaa;
            std::string str1 = columnStrings[0].toStdString();//("green apple");
            std::string str2 ("wiresub");

            if(tamanho==2){
                treeitem* item;
                if(id == 0){
                    item = new treeitem(id, columnStrings[0].toStdString(), columnStrings[1].toStdString(),columnStrings[3].toStdString(),columnStrings[2].toInt(),-1);
                }
                else{
                    item = new treeitem(id, columnStrings[0].toStdString(), columnStrings[1].toStdString(),columnStrings[3].toStdString(),columnStrings[2].toInt(),fathers.last());
                }
                items_tree << item;
                thirdRow =prepareRow(columnStrings[0], columnStrings[1], "","");

            }
            else{
                treeitem* item;
                if(id == 0){
                    item = new treeitem(id, columnStrings[0].toStdString(), columnStrings[1].toStdString(),columnStrings[3].toStdString(),columnStrings[2].toInt(),-1);
                }
                else{
                    item = new treeitem(id, columnStrings[0].toStdString(), columnStrings[1].toStdString(),columnStrings[3].toStdString(),columnStrings[2].toInt(),fathers.last());
                }
                items_tree << item;
                thirdRow =prepareRow(columnStrings[0], columnStrings[1], columnStrings[2],QString::number(id));
                //std::cout << "What is this?" << item->getfather() << "\n";

            }

            /*if (str1.compare(str2) == 0){
                thirdRow.at(0)->setFlags(thirdRow.at(0)->flags()| Qt::ItemIsUserCheckable);
                thirdRow.at(0)->setCheckState(Qt::Unchecked);
            }
            thirdRow.at(0)->setFlags(thirdRow.at(0)->flags()| Qt::ItemIsUserCheckable);
            thirdRow.at(0)->setCheckState(Qt::Unchecked);*/

            // adding a row to an item starts a subtree
            //item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            //item->setCheckState(Qt::Unchecked);
            thirdRow[0]->setAccessibleDescription(columnStrings[3]);
            thirdRow[1]->setAccessibleDescription(columnStrings[3]);
            thirdRow[2]->setAccessibleDescription(columnStrings[3]);
            thirdRow[3]->setAccessibleDescription(columnStrings[3]);
            parents.last()->appendRow(thirdRow);
            if (str1.compare(str2) == 0){
                int child_count = parents.last()->rowCount();
                parents << parents.last()->child(child_count-1);
                thirdRow =prepareRow(columnStrings[0], columnStrings[1], columnStrings[2],QString::number(id));
                thirdRow.at(0)->setFlags(thirdRow.at(0)->flags()| Qt::ItemIsUserCheckable);
                thirdRow.at(0)->setCheckState(Qt::Unchecked);
                thirdRow[0]->setAccessibleDescription(columnStrings[3]);
                thirdRow[1]->setAccessibleDescription(columnStrings[3]);
                thirdRow[2]->setAccessibleDescription(columnStrings[3]);
                thirdRow[3]->setAccessibleDescription(columnStrings[3]);
                parents.last()->appendRow(thirdRow);
                parents.pop_back();
            }
            /*if(columnStrings[0] != QString("wire")){
                fathers <
            }*/
            /*if(columnStrings[0] != QString("wire")){
            idfather = id;
            }*/

            id++;
            name = columnStrings[0].toStdString();
            //std::cout << "aaaaaaa" << columnStrings[0].toStdString() << "\n";
            /*if(columnStrings[0] != QString("wire")){
           fathers << idfather;
           idfather = id;
           }*/

            //parents.last()->appendChild(new TreeItem(columnData, parents.last()));

        }

        ++number;
    }
}
