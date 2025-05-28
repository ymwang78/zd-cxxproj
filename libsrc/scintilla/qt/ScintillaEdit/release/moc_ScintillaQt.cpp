/****************************************************************************
** Meta object code from reading C++ file 'ScintillaQt.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ScintillaEditBase/ScintillaQt.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ScintillaQt.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN9Scintilla8Internal11ScintillaQtE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN9Scintilla8Internal11ScintillaQtE = QtMocHelpers::stringData(
    "Scintilla::Internal::ScintillaQt",
    "horizontalScrolled",
    "",
    "value",
    "verticalScrolled",
    "horizontalRangeChanged",
    "max",
    "page",
    "verticalRangeChanged",
    "notifyParent",
    "Scintilla::NotificationData",
    "scn",
    "notifyChange",
    "aboutToCopy",
    "QMimeData*",
    "data",
    "command",
    "Scintilla::uptr_t",
    "wParam",
    "Scintilla::sptr_t",
    "lParam",
    "onIdle",
    "execCommand",
    "QAction*",
    "action",
    "SelectionChanged"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN9Scintilla8Internal11ScintillaQtE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   80,    2, 0x06,    1 /* Public */,
       4,    1,   83,    2, 0x06,    3 /* Public */,
       5,    2,   86,    2, 0x06,    5 /* Public */,
       8,    2,   91,    2, 0x06,    8 /* Public */,
       9,    1,   96,    2, 0x06,   11 /* Public */,
      12,    0,   99,    2, 0x06,   13 /* Public */,
      13,    1,  100,    2, 0x06,   14 /* Public */,
      16,    2,  103,    2, 0x06,   16 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      21,    0,  108,    2, 0x08,   19 /* Private */,
      22,    1,  109,    2, 0x08,   20 /* Private */,
      25,    0,  112,    2, 0x08,   22 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    6,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    6,    7,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, 0x80000000 | 17, 0x80000000 | 19,   18,   20,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 23,   24,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject Scintilla::Internal::ScintillaQt::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ZN9Scintilla8Internal11ScintillaQtE.offsetsAndSizes,
    qt_meta_data_ZN9Scintilla8Internal11ScintillaQtE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN9Scintilla8Internal11ScintillaQtE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ScintillaQt, std::true_type>,
        // method 'horizontalScrolled'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'verticalScrolled'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'horizontalRangeChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'verticalRangeChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'notifyParent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::NotificationData, std::false_type>,
        // method 'notifyChange'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'aboutToCopy'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QMimeData *, std::false_type>,
        // method 'command'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::uptr_t, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::sptr_t, std::false_type>,
        // method 'onIdle'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'execCommand'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QAction *, std::false_type>,
        // method 'SelectionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void Scintilla::Internal::ScintillaQt::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ScintillaQt *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->horizontalScrolled((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->verticalScrolled((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->horizontalRangeChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 3: _t->verticalRangeChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 4: _t->notifyParent((*reinterpret_cast< std::add_pointer_t<Scintilla::NotificationData>>(_a[1]))); break;
        case 5: _t->notifyChange(); break;
        case 6: _t->aboutToCopy((*reinterpret_cast< std::add_pointer_t<QMimeData*>>(_a[1]))); break;
        case 7: _t->command((*reinterpret_cast< std::add_pointer_t<Scintilla::uptr_t>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<Scintilla::sptr_t>>(_a[2]))); break;
        case 8: _t->onIdle(); break;
        case 9: _t->execCommand((*reinterpret_cast< std::add_pointer_t<QAction*>>(_a[1]))); break;
        case 10: _t->SelectionChanged(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (ScintillaQt::*)(int );
            if (_q_method_type _q_method = &ScintillaQt::horizontalScrolled; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaQt::*)(int );
            if (_q_method_type _q_method = &ScintillaQt::verticalScrolled; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaQt::*)(int , int );
            if (_q_method_type _q_method = &ScintillaQt::horizontalRangeChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaQt::*)(int , int );
            if (_q_method_type _q_method = &ScintillaQt::verticalRangeChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaQt::*)(Scintilla::NotificationData );
            if (_q_method_type _q_method = &ScintillaQt::notifyParent; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaQt::*)();
            if (_q_method_type _q_method = &ScintillaQt::notifyChange; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaQt::*)(QMimeData * );
            if (_q_method_type _q_method = &ScintillaQt::aboutToCopy; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaQt::*)(Scintilla::uptr_t , Scintilla::sptr_t );
            if (_q_method_type _q_method = &ScintillaQt::command; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
    }
}

const QMetaObject *Scintilla::Internal::ScintillaQt::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Scintilla::Internal::ScintillaQt::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN9Scintilla8Internal11ScintillaQtE.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "ScintillaBase"))
        return static_cast< ScintillaBase*>(this);
    return QObject::qt_metacast(_clname);
}

int Scintilla::Internal::ScintillaQt::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void Scintilla::Internal::ScintillaQt::horizontalScrolled(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Scintilla::Internal::ScintillaQt::verticalScrolled(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Scintilla::Internal::ScintillaQt::horizontalRangeChanged(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Scintilla::Internal::ScintillaQt::verticalRangeChanged(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Scintilla::Internal::ScintillaQt::notifyParent(Scintilla::NotificationData _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Scintilla::Internal::ScintillaQt::notifyChange()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void Scintilla::Internal::ScintillaQt::aboutToCopy(QMimeData * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void Scintilla::Internal::ScintillaQt::command(Scintilla::uptr_t _t1, Scintilla::sptr_t _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_WARNING_POP
