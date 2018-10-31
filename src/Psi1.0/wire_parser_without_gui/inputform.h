#ifndef INPUTFORM_H
#define INPUTFORM_H

#include <QDialog>
#include <QFormLayout>
#include <vector>
#include <QTextEdit>
#include <QLabel>
#include <QComboBox>
#include <network.h>
class QLineEdit;
class QDateEdit;
class QPushButton;
class QDialogButtonBox;
using namespace std;

class InputForm : public QDialog {
    Q_OBJECT
public:
    QPushButton *Assertions;
    explicit InputForm(QWidget* parent = 0);
    void   updateUi();
    QFormLayout* layout;
    int accept( vector<string> &comando);
    vector<network*> redes;
protected slots:

    void chooseColor();

private slots:
        void interface_control();
private:
    QLabel *lbl_input;
    QLabel *lbl_output;
    QLabel *lbl_networks_types;
    QLabel *lbl_stages;
    QLabel *lbl_type;
    QLabel *lbl_radix;
    QLabel *lbl_widht;
    QLabel *lbl_depth;
    QLabel *trace1;
    QLabel *trace2;
    QComboBox *networks_types;
    QColor m_color;
    network *selected_net;
    QLineEdit *radix;
    QLineEdit *input_network;
    QLineEdit *output_network;
    QLineEdit *stages;
    QLineEdit *type;
    QLineEdit *Trace_widht;
    QLineEdit *Trace_depth;
    QPushButton* m_colorButton;
    QDialogButtonBox* m_buttons;
};

#endif // INPUTFORM_H
