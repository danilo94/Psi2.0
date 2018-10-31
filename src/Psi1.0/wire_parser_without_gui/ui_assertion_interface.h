/********************************************************************************
** Form generated from reading UI file 'assertion_interface.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ASSERTION_INTERFACE_H
#define UI_ASSERTION_INTERFACE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Assertion_Interface
{
public:
    QPushButton *OkButton;
    QPushButton *CancelButton;
    QLabel *label;
    QListView *Wires;
    QLabel *label_2;
    QComboBox *Assertions;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *Expression;
    QCheckBox *UseAssert;
    QLineEdit *SelectedWire;
    QLabel *label_5;
    QLineEdit *Sel;

    void setupUi(QDialog *Assertion_Interface)
    {
        if (Assertion_Interface->objectName().isEmpty())
            Assertion_Interface->setObjectName(QStringLiteral("Assertion_Interface"));
        Assertion_Interface->resize(402, 349);
        OkButton = new QPushButton(Assertion_Interface);
        OkButton->setObjectName(QStringLiteral("OkButton"));
        OkButton->setGeometry(QRect(250, 20, 91, 27));
        CancelButton = new QPushButton(Assertion_Interface);
        CancelButton->setObjectName(QStringLiteral("CancelButton"));
        CancelButton->setGeometry(QRect(250, 60, 91, 27));
        label = new QLabel(Assertion_Interface);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(70, 0, 71, 31));
        Wires = new QListView(Assertion_Interface);
        Wires->setObjectName(QStringLiteral("Wires"));
        Wires->setGeometry(QRect(10, 30, 201, 311));
        Wires->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        Wires->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        Wires->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContentsOnFirstShow);
        label_2 = new QLabel(Assertion_Interface);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(250, 90, 91, 20));
        Assertions = new QComboBox(Assertion_Interface);
        Assertions->setObjectName(QStringLiteral("Assertions"));
        Assertions->setGeometry(QRect(230, 170, 121, 31));
        QFont font;
        font.setPointSize(8);
        Assertions->setFont(font);
        label_3 = new QLabel(Assertion_Interface);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(250, 150, 91, 20));
        label_4 = new QLabel(Assertion_Interface);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(250, 210, 91, 20));
        Expression = new QLineEdit(Assertion_Interface);
        Expression->setObjectName(QStringLiteral("Expression"));
        Expression->setGeometry(QRect(232, 230, 121, 27));
        UseAssert = new QCheckBox(Assertion_Interface);
        UseAssert->setObjectName(QStringLiteral("UseAssert"));
        UseAssert->setGeometry(QRect(230, 260, 131, 22));
        SelectedWire = new QLineEdit(Assertion_Interface);
        SelectedWire->setObjectName(QStringLiteral("SelectedWire"));
        SelectedWire->setGeometry(QRect(230, 120, 121, 27));
        label_5 = new QLabel(Assertion_Interface);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(240, 280, 121, 17));
        Sel = new QLineEdit(Assertion_Interface);
        Sel->setObjectName(QStringLiteral("Sel"));
        Sel->setGeometry(QRect(230, 300, 121, 27));

        retranslateUi(Assertion_Interface);

        QMetaObject::connectSlotsByName(Assertion_Interface);
    } // setupUi

    void retranslateUi(QDialog *Assertion_Interface)
    {
        Assertion_Interface->setWindowTitle(QApplication::translate("Assertion_Interface", "Dialog", 0));
        OkButton->setText(QApplication::translate("Assertion_Interface", "Ok", 0));
        CancelButton->setText(QApplication::translate("Assertion_Interface", "Cancel", 0));
        label->setText(QApplication::translate("Assertion_Interface", "      Wires", 0));
        label_2->setText(QApplication::translate("Assertion_Interface", "Selected Wire", 0));
        label_3->setText(QApplication::translate("Assertion_Interface", "Assertion Type", 0));
        label_4->setText(QApplication::translate("Assertion_Interface", "    Expression", 0));
        UseAssert->setText(QApplication::translate("Assertion_Interface", "Use as Assertion", 0));
        label_5->setText(QApplication::translate("Assertion_Interface", "          Selected", 0));
    } // retranslateUi

};

namespace Ui {
    class Assertion_Interface: public Ui_Assertion_Interface {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ASSERTION_INTERFACE_H
