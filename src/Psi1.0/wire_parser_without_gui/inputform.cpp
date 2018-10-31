#include "inputform.h"
#include <QFormLayout>
#include <QDateEdit>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QMessageBox>
#include <QColorDialog>
#include <QSettings>
#include <QVariant>
#include <QPalette>
#include <QLabel>
#include <network.h>
#include <vector>
#include <mainwindow.h>

InputForm::InputForm(QWidget* parent)
    : QDialog(parent) {
    network *Clos = new network;
    /*Clos->init_parse("a");
    cout << "NOME\n\nNOME" << Clos->getName().c_str();*/
    /*Clos->setName("Clos");
    Clos->setCommand("gen_clos");*/

    /*option mand1;
    mand1.setOption_name("Input Number");
    mand1.setRelated_option("-i");

    option mand2;
    mand2.setOption_name("Output Number");
    mand2.setRelated_option("-o");

    option mand3;
    mand3.setOption_name("Stage Number");
    mand3.setRelated_option("-r");*/

    /*Clos->addMandatory_Option(mand1);
    Clos->addMandatory_Option(mand2);
    Clos->addMandatory_Option(mand3);*/

    /*network *Mux_Tree = new network;
    Mux_Tree->setName("Mux Tree");
    Mux_Tree->setCommand("gen_mux");
    Mux_Tree->addMandatory_Option(mand1);
    Mux_Tree->addMandatory_Option(mand2);*/

    redes = Clos->init_parse("/home/lunk/Documents/PackPsi/Psi/utils/xmlnetwork.xml");
    QSettings s;

    setWindowTitle("Networks");
    QVariant v = s.value("color/favorite", QVariant());
    if (!v.isNull()) m_color =  v.value<QColor>();

    //start id="form"
    //clos_types = new QComboBox(this);
    Assertions = new QPushButton("Assertions", this);
    m_colorButton = new QPushButton(tr("Choose"));
    m_colorButton->setAutoFillBackground(true);
    QVBoxLayout* vbox = new QVBoxLayout;
    layout = new QFormLayout;
    networks_types = new QComboBox(this);
    // Labels
    lbl_radix = new QLabel ("Radix:");
    lbl_input = new QLabel("Inputs:",this);
    lbl_output = new QLabel("Outputs:",this);
    lbl_networks_types = new QLabel("Select The Network:");
    lbl_stages = new QLabel("Network Stages:");
    lbl_type = new QLabel("Type:");
    lbl_widht = new QLabel("Trace Buffer Widht:");
    lbl_depth = new QLabel ("Trace Buffer Depth:");
    trace1 = new QLabel ("Trace Buffer Options:");
    trace2 = new QLabel("");
    // End Labels
    // Data In
    radix = new QLineEdit(this);
    input_network = new QLineEdit(this);
    output_network = new QLineEdit(this);
    stages = new QLineEdit(this);
    type = new QLineEdit(this);
    Trace_depth = new QLineEdit(this);
    Trace_widht = new QLineEdit(this);
    // End Data In
    QStringList lists;
    network *aux;
    network *selected;
    lists.push_back("                  -");
    for (int var=0; var<redes.size(); var++){
       aux = redes.at(var);
       lists.push_back(aux->getName().c_str());
    }
    networks_types->addItems(lists);
    layout = new QFormLayout;
    layout->addRow(lbl_networks_types,networks_types);
    layout->addRow(lbl_input,input_network);
    layout->addRow(lbl_output,output_network);
    layout->addRow(lbl_stages,stages);
    layout->addRow(lbl_type,type);
    layout->addRow(lbl_radix,radix);
    layout->addRow(trace2,trace2);
    layout->addRow(trace1,trace2);
    layout->addRow(lbl_depth,Trace_depth);
    layout->addRow(lbl_widht,Trace_widht);
    layout->addRow(Assertions);
    input_network->setDisabled("true");
    output_network->setDisabled("true");
    stages->setDisabled("true");
    type->setDisabled("true");
    radix->setDisabled("true");
    Trace_depth->setDisabled("true");
    Trace_widht->setDisabled("true");
    vbox->addLayout(layout);
    Q_ASSERT(vbox->parent() == 0);
    setLayout(vbox);
    Q_ASSERT(vbox->parent() == this);
    updateUi();
    connect(networks_types,SIGNAL(currentIndexChanged(int)), this, SLOT(interface_control()));
}
void InputForm::interface_control(){
    network *aux;
    string network;
    int shift=-1;
    int next=0;
    input_network->clear();
    output_network->clear();
    stages->clear();
    type->clear();
    radix->clear();
    input_network->setDisabled("true");
    output_network->setDisabled("true");
    stages->setDisabled("true");
    type->setDisabled("true");
    radix->setDisabled("true");
    Trace_depth->setDisabled("true");
    Trace_widht->setDisabled("true");
    for (int var=0; var<redes.size(); var++){
        aux = redes.at(var);
        if (aux->getName()==networks_types->currentText().toUtf8().constData()){
            shift = aux->getId();
            selected_net = aux;
            break;
        }
    }
    while (shift!=-1 && shift>0){
        Trace_widht->setEnabled("true");
        Trace_depth->setEnabled("true");
        switch (next){
        case 0:
            if (shift%2==1){
                input_network->setEnabled("true");
            }
            next++;
            break;
        case 1:
            if (shift%2==1){
                output_network->setEnabled("true");
            }
            next++;
            break;
        case 2:
            if (shift%2==1){
                stages->setEnabled("true");
            }
            next++;
            break;
        case 3:
            if (shift%2==1){
                type->setEnabled("true");
            }
            next++;
            break;
        case 4:
            if (shift%2==1){
                radix->setEnabled("true");
            }
            next++;
            break;
        }
        shift = shift >> 1;
    }

}



int InputForm::accept(vector <string> &c) {
    network *aux = selected_net;
    string comando_net;
    string comando_trace;
    string inputs = input_network->text().toUtf8().constData();
    string outputs = output_network->text().toUtf8().constData();
    string typee =  type->text().toUtf8().constData();
    string stagess = stages->text().toUtf8().constData();
    string radixx = radix->text().toUtf8().constData();
    string buffer_width = Trace_widht->text().toUtf8().constData();
    string buffer_depth = Trace_depth->text().toUtf8().constData();
    int width = atoi(buffer_width.c_str());
    int depth = atoi(buffer_depth.c_str());
    if (aux->getCommand().empty() || inputs.empty() || outputs.empty() || (aux->getCommand()=="clos" && typee.empty())){
        //QMessageBox::information(this,tr("Error"),tr("Please Insert the Mandatory Options"));
        return 0;
    }
    else {
    comando_net = "vericonn -n "+aux->getCommand()+" -i "+inputs+" -o "+outputs+" ";
    }
    if (buffer_width.empty() || buffer_depth.empty() || width<=0 || depth<=0){
        return 0;
    }
    else {
        comando_trace = "veritrace "+buffer_depth+" "+buffer_width+"";
    }

    if (aux->getCommand() == "clos"){
        comando_net = comando_net + " -t "+typee+" ";
    }
    if (!stagess.empty()){
        comando_net = comando_net + " -s "+stagess+" ";
    }
    if (!radixx.empty()){
        comando_net = comando_net + " -r "+radixx+" ";
    }

    c.push_back(comando_net);
    c.push_back(inputs);
    c.push_back(outputs);
    c.push_back(aux->getName());
    c.push_back(comando_trace);
    c.push_back(buffer_depth);
    c.push_back(buffer_width);
    c.push_back("tracebuffer_"+buffer_depth+"x"+buffer_width+".v\n");
    c.push_back("tracebuffer");
    return 1;
}

void InputForm::chooseColor() {
    m_color = QColorDialog::getColor(m_color, this);
    updateUi();
}

void InputForm::updateUi() {
    QPalette p = m_colorButton->palette();
    p.setColor(QPalette::Button, m_color);
    m_colorButton->setPalette(p);


}
