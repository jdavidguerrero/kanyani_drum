/****************************************************************************
** Meta object code from reading C++ file 'SensorController.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.10)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/controller/SensorController.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SensorController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.10. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SensorController_t {
    QByteArrayData data[11];
    char stringdata0[166];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SensorController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SensorController_t qt_meta_stringdata_SensorController = {
    {
QT_MOC_LITERAL(0, 0, 16), // "SensorController"
QT_MOC_LITERAL(1, 17, 19), // "sensorValuesChanged"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 17), // "thresholdsChanged"
QT_MOC_LITERAL(4, 56, 12), // "startReading"
QT_MOC_LITERAL(5, 69, 11), // "stopReading"
QT_MOC_LITERAL(6, 81, 16), // "calibrateSensors"
QT_MOC_LITERAL(7, 98, 21), // "saveThresholdSettings"
QT_MOC_LITERAL(8, 120, 21), // "loadThresholdSettings"
QT_MOC_LITERAL(9, 142, 10), // "thresholds"
QT_MOC_LITERAL(10, 153, 12) // "QVector<int>"

    },
    "SensorController\0sensorValuesChanged\0"
    "\0thresholdsChanged\0startReading\0"
    "stopReading\0calibrateSensors\0"
    "saveThresholdSettings\0loadThresholdSettings\0"
    "thresholds\0QVector<int>"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SensorController[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       1,   56, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,
       3,    0,   50,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
       4,    0,   51,    2, 0x02 /* Public */,
       5,    0,   52,    2, 0x02 /* Public */,
       6,    0,   53,    2, 0x02 /* Public */,
       7,    0,   54,    2, 0x02 /* Public */,
       8,    0,   55,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
       9, 0x80000000 | 10, 0x0049510b,

 // properties: notify_signal_id
       1,

       0        // eod
};

void SensorController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SensorController *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->sensorValuesChanged(); break;
        case 1: _t->thresholdsChanged(); break;
        case 2: _t->startReading(); break;
        case 3: _t->stopReading(); break;
        case 4: _t->calibrateSensors(); break;
        case 5: _t->saveThresholdSettings(); break;
        case 6: _t->loadThresholdSettings(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SensorController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SensorController::sensorValuesChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SensorController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SensorController::thresholdsChanged)) {
                *result = 1;
                return;
            }
        }
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<int> >(); break;
        }
    }

#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<SensorController *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QVector<int>*>(_v) = _t->getThresholds(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<SensorController *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setThresholds(*reinterpret_cast< QVector<int>*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject SensorController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_SensorController.data,
    qt_meta_data_SensorController,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SensorController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SensorController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SensorController.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SensorController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void SensorController::sensorValuesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SensorController::thresholdsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
