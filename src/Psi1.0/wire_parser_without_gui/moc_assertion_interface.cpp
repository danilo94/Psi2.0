/****************************************************************************
** Meta object code from reading C++ file 'assertion_interface.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "assertion_interface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'assertion_interface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Assertion_Interface_t {
    QByteArrayData data[6];
    char stringdata0[48];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Assertion_Interface_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Assertion_Interface_t qt_meta_stringdata_Assertion_Interface = {
    {
QT_MOC_LITERAL(0, 0, 19), // "Assertion_Interface"
QT_MOC_LITERAL(1, 20, 5), // "close"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 6), // "select"
QT_MOC_LITERAL(4, 34, 10), // "add_assert"
QT_MOC_LITERAL(5, 45, 2) // "Ok"

    },
    "Assertion_Interface\0close\0\0select\0"
    "add_assert\0Ok"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Assertion_Interface[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x08 /* Private */,
       3,    0,   35,    2, 0x08 /* Private */,
       4,    0,   36,    2, 0x08 /* Private */,
       5,    0,   37,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Assertion_Interface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Assertion_Interface *_t = static_cast<Assertion_Interface *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->close(); break;
        case 1: _t->select(); break;
        case 2: _t->add_assert(); break;
        case 3: _t->Ok(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject Assertion_Interface::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Assertion_Interface.data,
      qt_meta_data_Assertion_Interface,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Assertion_Interface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Assertion_Interface::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Assertion_Interface.stringdata0))
        return static_cast<void*>(const_cast< Assertion_Interface*>(this));
    return QDialog::qt_metacast(_clname);
}

int Assertion_Interface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
