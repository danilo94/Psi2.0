#include "assertion_interface.h"
#include "ui_assertion_interface.h"
#include "iostream"
Assertion_Interface::Assertion_Interface(QWidget *parent) :QDialog(parent),ui(new Ui::Assertion_Interface){
    remove(".01.txt");
    ui->setupUi(this);
    this->setWindowTitle("Assertion Manager");
    asrt assertt;
    string buffer1,buffer2;
    char buffer;
    QString line;
    int i=0;
    ui->SelectedWire->setEnabled(false);
    QFile file("Hierarchy.txt");
    QFile file2 ("utils/Assertions.data");
    file.open(QIODevice::ReadOnly);
    file2.open(QIODevice::ReadOnly);
    while (!file.atEnd()){
        line = file.readLine();
        buffer1 = line.toUtf8().constData();
        buffer2 = line.toUtf8().constData();
        buffer1.erase(0,8);
        buffer1.erase(7,buffer1.size()-1);
        if (buffer1.compare("wiresub")==0){
         buffer2.erase(0,16);
         while (buffer != ']'){
            i++;
            buffer = buffer2[i];
         }
        i++;
        buffer2.erase(i,buffer2.size()-1);
        assertt.name=buffer2;
        assertt.value= "0";
        Assertionss.push_back(assertt);
        i=0;
        buffer='0';
        }
    }

    QStringList aux;
    aux.push_back("");
    while (!file2.atEnd()){
        line.clear();
        line = file2.readLine();
        line = line.remove('\n');
        aux.push_back(line);
    }
    ui->Assertions->addItems(aux);
    ui->Sel->setEnabled(false);
    file.close();
    file2.close();
    connect(ui->OkButton,SIGNAL(released()), this, SLOT(Ok()));
    connect(ui->CancelButton,SIGNAL(released()), this, SLOT(close()));
    connect(ui->Wires,SIGNAL(clicked(QModelIndex)),this,SLOT(select()));
    connect(ui->UseAssert, SIGNAL(clicked(bool)),this,SLOT(add_assert()));
    ncol = 1;
    nrow = Assertionss.size()-1;
    model = new QStandardItemModel(nrow,3,this);
    for (i=0; i<nrow; i++){
        QStandardItem *Item = new QStandardItem(Assertionss.at(i).name.c_str());
        //Item->setIcon(QIcon(QPixmap::fromImage("Icons/cancel.png")));
        Item->setCheckable( false );
        Item->setEditable(false);
        model->setItem(i, 0, Item);
    }
    ui->Wires->setModel(model);
    ui->Assertions->setEnabled(false);
    ui->Expression->setEnabled(false);
    ui->UseAssert->setEnabled(false);
}


void Assertion_Interface::Ok(){
    //this->setEnabled(false);
    this->setVisible(false);
    QFile file(".01.txt");
    file.open(QFile::WriteOnly);

    for (int i=0; i<assert_out.size(); i++){
        string writer;
        writer = assert_out.at(i).name;
        file.write(writer.c_str());
        file.write("\n");
        writer = assert_out.at(i).assertion_type;
        file.write(writer.c_str());
        file.write("\n");
        writer = assert_out.at(i).value;
        file.write(writer.c_str());
        file.write("\n");
    }
    file.close();
}


void Assertion_Interface::select(){
    asrt assert;
    ui->UseAssert->setChecked(false);
    ui->Expression->setText("");
    ui->Assertions->setEnabled(true);
    ui->Expression->setEnabled(true);
    ui->UseAssert->setEnabled(true);
    QModelIndexList list = ui->Wires->selectionModel()->selectedIndexes();
      QStringList slist;
      foreach(const QModelIndex &index, list){
          slist.append( index.data(Qt::DisplayRole ).toString());
      }
      selected_wire = slist.at(0);
      ui->SelectedWire->setText(selected_wire);

      for (int i=0; i<assert_out.size(); i++){
          if (assert_out.at(i).name.compare(selected_wire.toUtf8().constData())==0){
              assert = assert_out.at(i);
              break;
          }
      }
      ui->Expression->setText(QString::fromStdString(assert.value));
      ui->UseAssert->setChecked(assert.selected);
     // ui->Sel->setText(std::to_string(assert_out.size()));
      //ui->Assertions->setItemText(QString::fromStdString(assert.assertion_type));
      slist.clear();
}

void Assertion_Interface::add_assert(){
    asrt assert;
    int i=0;
    if (ui->UseAssert->isChecked()){
        assert.name = selected_wire.toUtf8().constData();
        assert.selected = true;
        if (ui->Expression->text().isEmpty()){
            assert.selected = false;
            ui->UseAssert->setChecked(false);
        }
        else {
            assert.value = ui->Expression->text().toUtf8().constData();
            assert.assertion_type = ui->Assertions->currentText().toUtf8().constData();
            assert_out.push_back(assert);
        }
    }
    else{
        for (i=0 ; i<assert_out.size(); i++){
            string comp = selected_wire.toUtf8().constData();
            if (comp.compare(assert_out.at(i).name)==0){
                break;
            }
        }
        assert_out.erase(assert_out.begin()+i);
    }


}
void Assertion_Interface::close(){
    assert_out.clear();
    Assertionss.clear();

    this->destroy();
}



Assertion_Interface::~Assertion_Interface()
{
    delete ui;
}
