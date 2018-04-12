/****************************************************************************
** Meta object code from reading C++ file 'hriboard.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../HriBoardLib/hriboard.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'hriboard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HriBoard_t {
    QByteArrayData data[12];
    char stringdata0[149];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HriBoard_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HriBoard_t qt_meta_stringdata_HriBoard = {
    {
QT_MOC_LITERAL(0, 0, 8), // "HriBoard"
QT_MOC_LITERAL(1, 9, 20), // "syncVarsListReceived"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 19), // "QList<SyncVarBase*>"
QT_MOC_LITERAL(4, 51, 8), // "syncVars"
QT_MOC_LITERAL(5, 60, 14), // "syncVarUpdated"
QT_MOC_LITERAL(6, 75, 12), // "SyncVarBase*"
QT_MOC_LITERAL(7, 88, 3), // "var"
QT_MOC_LITERAL(8, 92, 23), // "streamedSyncVarsUpdated"
QT_MOC_LITERAL(9, 116, 4), // "time"
QT_MOC_LITERAL(10, 121, 12), // "streamedVars"
QT_MOC_LITERAL(11, 134, 14) // "onReceivedData"

    },
    "HriBoard\0syncVarsListReceived\0\0"
    "QList<SyncVarBase*>\0syncVars\0"
    "syncVarUpdated\0SyncVarBase*\0var\0"
    "streamedSyncVarsUpdated\0time\0streamedVars\0"
    "onReceivedData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HriBoard[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       5,    1,   37,    2, 0x06 /* Public */,
       8,    2,   40,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    0,   45,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, QMetaType::Double, 0x80000000 | 3,    9,   10,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void HriBoard::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        HriBoard *_t = static_cast<HriBoard *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->syncVarsListReceived((*reinterpret_cast< const QList<SyncVarBase*>(*)>(_a[1]))); break;
        case 1: _t->syncVarUpdated((*reinterpret_cast< SyncVarBase*(*)>(_a[1]))); break;
        case 2: _t->streamedSyncVarsUpdated((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< const QList<SyncVarBase*>(*)>(_a[2]))); break;
        case 3: _t->onReceivedData(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (HriBoard::*_t)(const QList<SyncVarBase*> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HriBoard::syncVarsListReceived)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (HriBoard::*_t)(SyncVarBase * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HriBoard::syncVarUpdated)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (HriBoard::*_t)(double , const QList<SyncVarBase*> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HriBoard::streamedSyncVarsUpdated)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject HriBoard::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_HriBoard.data,
      qt_meta_data_HriBoard,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *HriBoard::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HriBoard::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HriBoard.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int HriBoard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void HriBoard::syncVarsListReceived(const QList<SyncVarBase*> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void HriBoard::syncVarUpdated(SyncVarBase * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void HriBoard::streamedSyncVarsUpdated(double _t1, const QList<SyncVarBase*> & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
