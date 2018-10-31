#include "opening_window.h"
#include "mainwindow.h"
#include <QCheckBox>
#include <iterator>
#include <QMessageBox>
#include <sstream>
#include <QtCore/QCoreApplication>
#include <algorithm>
#include <QTreeWidget>
#include <vector>
opening_window::opening_window(QWidget *parent,std::string topLevelFolder,std::string topLevelPath,std::string listOfWires)
    : QMainWindow(parent)
{
    //QFileDialog::getOpenFileName;
    /*
    //Components initializations.
    QMessageBox::information(this,tr("INSTRUCTIONS"), tr("- Include all of your design verilog files on the same folder. \n\n - The path should follow the model \"path/to/my/design\" \n\n - Toplevel file is \"Path/to/my/design/file.v\" \n\n - The toplevel file should have one include for each file, no other files can have includes. \n\n - If your compiler works with includes, ok, if not, remove the includes from the modfied toplevel file before compiling. \n\n"));
    file_select_button = new QPushButton("...",this);
    directory_select_button = new QPushButton("...",this);
    m_button = new QPushButton("Generate Hyerarchy", this);
    toplevel = new QTextEdit(this);
    toplevel->setText("/home/danilo/Downloads/natalius_8bit_risc/trunk/processor_core/natalius_processor.v");
    label_toplevel = new QLabel("TopLevel File:",this);
    verilog_path = new QTextEdit(this);
    verilog_path->setText("/home/danilo/Downloads/natalius_8bit_risc/trunk/processor_core/");
    //verilog_path->setText("/home/andre/Documentos/yosys-new/yosys/manual/test");
    //verilog_path->setText("/home/andre/Documentos/mips");

    label_verilog_path = new QLabel("Verilog path:",this);
    */
    /*
    std::string topLevel = "/home/danilo/Downloads/natalius_8bit_risc/trunk/processor_core/natalius_processor.v";
    std::string topLevelDir = "/home/danilo/Downloads/natalius_8bit_risc/trunk/processor_core/";
    readSignals("list.txt");
    */
    std::string topLevel = topLevelPath;
    std::string topLevelDir = topLevelFolder;
    readSignals(listOfWires);

    char *command = new char[10000];

    std::string comando = "yosys -Ql test0.log_new -m hierarchy_cmd.so -p 'my_cmd foo bar' "
        +topLevel;
    //std::string comando = "echo ";
    strcpy(command,comando.c_str());
    cout << "PATH\nPATH\nPATH\nPATH\nPATH\n" << command << "\n";
    comando = exec(command);

    this->close();
    std::string b ="";
    free(command);
    //wiresToExtern;


    MainWindow *windows_main = new MainWindow(wiresToExtern,0,topLevel,topLevelDir,topLevel);




}

void opening_window::handleButton()
{

}


void opening_window::select_file(){
    FileName = QFileDialog::getOpenFileName(this,
        tr("Select the top level file"), "/home", tr("Verilog Files (*.v)"));
    toplevel->setText(FileName);
    toplevel->setDisabled("true");
}
void opening_window::select_directory(){
    FileName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),"/home",QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
    verilog_path->setText(FileName);
    verilog_path->setDisabled("true");
}

std::vector<std::string> opening_window::ReadCells(QString toplevel,QString path)
{
    using namespace std;
    std::vector<std::string> myvector;
    std::vector<std::string> myvector2;
    std::string aux = path.toStdString()+"/cells.txt";
    std::ifstream file(aux.c_str()); // pass file name as argment
    std::string linebuffer;
    std::string delimiter = "/";
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

std::vector<std::string> opening_window::ReadWires(QString toplevel,QString path)
{
    std::vector<std::string> myvector;
    std::string aux = path.toStdString()+"/wires.txt";
    std::ifstream file(aux.c_str()); // pass file name as argment
    std::string linebuffer;
    std::string delimiter = "/";
    int i = 0;
    while (file && getline(file, linebuffer)){

        if (linebuffer.length() == 0)continue;

        if(linebuffer.find("toplevel")==false){
            size_t pos = 0;
            std::string token;
            while ((pos = linebuffer.find(delimiter)) != std::string::npos) {
                token = linebuffer.substr(0, pos);
                //std::cout << token << std::endl;

                linebuffer.erase(0, pos + delimiter.length());
            }

            myvector.push_back(linebuffer);

        }

    }

    return myvector;
}

std::string opening_window::exec(char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;
}

void opening_window::readSignals(string path)
{
    ifstream file;
    file.open(path);
    string linha;
    while (getline(file,linha)){
        wiresToExtern.push_back(linha);
        std::cout << linha << std::endl;
    }
}

void opening_window::handleButton2()
{
    char *command = new char[1000] ;
    std::string comando = "yosys -Ql test0.log_new -m /home/lunk/Documents/PackPsi/hierarchy_cmd.so -p 'my_cmd foo bar' "+
            verilog_path->toPlainText().toStdString()+"/"+toplevel->toPlainText().toStdString()+".v";
    std::string bug = verilog_path->toPlainText().toStdString()+"/"+toplevel->toPlainText().toStdString()+".v";
    strcpy(command,comando.c_str());
    comando = exec(command);
    std::cout << comando;
    //textb->setText(command);
    myvector = ReadCells(toplevel->toPlainText(),verilog_path->toPlainText());

    QListWidgetItem *item;
    std::string aux;
    for (int i = 0; i < myvector.size(); ++i) {
        item = new QListWidgetItem();
        aux = myvector[i];
        item->setText(QString::fromStdString(aux));
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        listWidget->addItem(item);
    }
}

