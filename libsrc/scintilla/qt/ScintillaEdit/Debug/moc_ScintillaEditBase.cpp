/****************************************************************************
** Meta object code from reading C++ file 'ScintillaEditBase.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ScintillaEditBase/ScintillaEditBase.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ScintillaEditBase.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN17ScintillaEditBaseE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN17ScintillaEditBaseE = QtMocHelpers::stringData(
    "ScintillaEditBase",
    "horizontalScrolled",
    "",
    "value",
    "verticalScrolled",
    "horizontalRangeChanged",
    "max",
    "page",
    "verticalRangeChanged",
    "notifyChange",
    "linesAdded",
    "Scintilla::Position",
    "aboutToCopy",
    "QMimeData*",
    "data",
    "styleNeeded",
    "position",
    "charAdded",
    "ch",
    "savePointChanged",
    "dirty",
    "modifyAttemptReadOnly",
    "key",
    "doubleClick",
    "line",
    "updateUi",
    "Scintilla::Update",
    "updated",
    "modified",
    "Scintilla::ModificationFlags",
    "type",
    "length",
    "text",
    "Scintilla::FoldLevel",
    "foldNow",
    "foldPrev",
    "macroRecord",
    "Scintilla::Message",
    "message",
    "Scintilla::uptr_t",
    "wParam",
    "Scintilla::sptr_t",
    "lParam",
    "marginClicked",
    "Scintilla::KeyMod",
    "modifiers",
    "margin",
    "textAreaClicked",
    "needShown",
    "painted",
    "userListSelection",
    "uriDropped",
    "uri",
    "dwellStart",
    "x",
    "y",
    "dwellEnd",
    "zoom",
    "hotSpotClick",
    "hotSpotDoubleClick",
    "callTipClick",
    "autoCompleteSelection",
    "autoCompleteCancelled",
    "focusChanged",
    "focused",
    "notify",
    "Scintilla::NotificationData*",
    "pscn",
    "command",
    "buttonPressed",
    "QMouseEvent*",
    "event",
    "buttonReleased",
    "keyPressed",
    "QKeyEvent*",
    "resized",
    "scrollHorizontal",
    "scrollVertical",
    "notifyParent",
    "Scintilla::NotificationData",
    "scn",
    "event_command"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN17ScintillaEditBaseE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      41,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      37,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  260,    2, 0x06,    1 /* Public */,
       4,    1,  263,    2, 0x06,    3 /* Public */,
       5,    2,  266,    2, 0x06,    5 /* Public */,
       8,    2,  271,    2, 0x06,    8 /* Public */,
       9,    0,  276,    2, 0x06,   11 /* Public */,
      10,    1,  277,    2, 0x06,   12 /* Public */,
      12,    1,  280,    2, 0x06,   14 /* Public */,
      15,    1,  283,    2, 0x06,   16 /* Public */,
      17,    1,  286,    2, 0x06,   18 /* Public */,
      19,    1,  289,    2, 0x06,   20 /* Public */,
      21,    0,  292,    2, 0x06,   22 /* Public */,
      22,    1,  293,    2, 0x06,   23 /* Public */,
      23,    2,  296,    2, 0x06,   25 /* Public */,
      25,    1,  301,    2, 0x06,   28 /* Public */,
      28,    8,  304,    2, 0x06,   30 /* Public */,
      36,    3,  321,    2, 0x06,   39 /* Public */,
      43,    3,  328,    2, 0x06,   43 /* Public */,
      47,    2,  335,    2, 0x06,   47 /* Public */,
      48,    2,  340,    2, 0x06,   50 /* Public */,
      49,    0,  345,    2, 0x06,   53 /* Public */,
      50,    0,  346,    2, 0x06,   54 /* Public */,
      51,    1,  347,    2, 0x06,   55 /* Public */,
      53,    2,  350,    2, 0x06,   57 /* Public */,
      56,    2,  355,    2, 0x06,   60 /* Public */,
      57,    1,  360,    2, 0x06,   63 /* Public */,
      58,    2,  363,    2, 0x06,   65 /* Public */,
      59,    2,  368,    2, 0x06,   68 /* Public */,
      60,    0,  373,    2, 0x06,   71 /* Public */,
      61,    2,  374,    2, 0x06,   72 /* Public */,
      62,    0,  379,    2, 0x06,   75 /* Public */,
      63,    1,  380,    2, 0x06,   76 /* Public */,
      65,    1,  383,    2, 0x06,   78 /* Public */,
      68,    2,  386,    2, 0x06,   80 /* Public */,
      69,    1,  391,    2, 0x06,   83 /* Public */,
      72,    1,  394,    2, 0x06,   85 /* Public */,
      73,    1,  397,    2, 0x06,   87 /* Public */,
      75,    0,  400,    2, 0x06,   89 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      76,    1,  401,    2, 0x0a,   90 /* Public */,
      77,    1,  404,    2, 0x0a,   92 /* Public */,
      78,    1,  407,    2, 0x0a,   94 /* Public */,
      81,    2,  410,    2, 0x0a,   96 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    6,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    6,    7,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11,   10,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void, 0x80000000 | 11,   16,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void, QMetaType::Bool,   20,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   22,
    QMetaType::Void, 0x80000000 | 11, 0x80000000 | 11,   16,   24,
    QMetaType::Void, 0x80000000 | 26,   27,
    QMetaType::Void, 0x80000000 | 29, 0x80000000 | 11, 0x80000000 | 11, 0x80000000 | 11, QMetaType::QByteArray, 0x80000000 | 11, 0x80000000 | 33, 0x80000000 | 33,   30,   16,   31,   10,   32,   24,   34,   35,
    QMetaType::Void, 0x80000000 | 37, 0x80000000 | 39, 0x80000000 | 41,   38,   40,   42,
    QMetaType::Void, 0x80000000 | 11, 0x80000000 | 44, QMetaType::Int,   16,   45,   46,
    QMetaType::Void, 0x80000000 | 11, QMetaType::Int,   24,   45,
    QMetaType::Void, 0x80000000 | 11, 0x80000000 | 11,   16,   31,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   52,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   54,   55,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   54,   55,
    QMetaType::Void, QMetaType::Int,   57,
    QMetaType::Void, 0x80000000 | 11, 0x80000000 | 44,   16,   45,
    QMetaType::Void, 0x80000000 | 11, 0x80000000 | 44,   16,   45,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11, QMetaType::QString,   16,   32,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   64,
    QMetaType::Void, 0x80000000 | 66,   67,
    QMetaType::Void, 0x80000000 | 39, 0x80000000 | 41,   40,   42,
    QMetaType::Void, 0x80000000 | 70,   71,
    QMetaType::Void, 0x80000000 | 70,   71,
    QMetaType::Void, 0x80000000 | 74,   71,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, 0x80000000 | 79,   80,
    QMetaType::Void, 0x80000000 | 39, 0x80000000 | 41,   40,   42,

       0        // eod
};

Q_CONSTINIT const QMetaObject ScintillaEditBase::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractScrollArea::staticMetaObject>(),
    qt_meta_stringdata_ZN17ScintillaEditBaseE.offsetsAndSizes,
    qt_meta_data_ZN17ScintillaEditBaseE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN17ScintillaEditBaseE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ScintillaEditBase, std::true_type>,
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
        // method 'notifyChange'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'linesAdded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::Position, std::false_type>,
        // method 'aboutToCopy'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QMimeData *, std::false_type>,
        // method 'styleNeeded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::Position, std::false_type>,
        // method 'charAdded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'savePointChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'modifyAttemptReadOnly'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'key'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'doubleClick'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::Position, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::Position, std::false_type>,
        // method 'updateUi'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::Update, std::false_type>,
        // method 'modified'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::ModificationFlags, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::Position, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::Position, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::Position, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QByteArray &, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::Position, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::FoldLevel, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::FoldLevel, std::false_type>,
        // method 'macroRecord'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::Message, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::uptr_t, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::sptr_t, std::false_type>,
        // method 'marginClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::Position, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::KeyMod, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'textAreaClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::Position, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'needShown'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::Position, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::Position, std::false_type>,
        // method 'painted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'userListSelection'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'uriDropped'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'dwellStart'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'dwellEnd'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'zoom'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'hotSpotClick'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::Position, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::KeyMod, std::false_type>,
        // method 'hotSpotDoubleClick'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::Position, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::KeyMod, std::false_type>,
        // method 'callTipClick'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'autoCompleteSelection'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::Position, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'autoCompleteCancelled'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'focusChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'notify'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::NotificationData *, std::false_type>,
        // method 'command'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::uptr_t, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::sptr_t, std::false_type>,
        // method 'buttonPressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QMouseEvent *, std::false_type>,
        // method 'buttonReleased'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QMouseEvent *, std::false_type>,
        // method 'keyPressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QKeyEvent *, std::false_type>,
        // method 'resized'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'scrollHorizontal'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'scrollVertical'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'notifyParent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::NotificationData, std::false_type>,
        // method 'event_command'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::uptr_t, std::false_type>,
        QtPrivate::TypeAndForceComplete<Scintilla::sptr_t, std::false_type>
    >,
    nullptr
} };

void ScintillaEditBase::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ScintillaEditBase *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->horizontalScrolled((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->verticalScrolled((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->horizontalRangeChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 3: _t->verticalRangeChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 4: _t->notifyChange(); break;
        case 5: _t->linesAdded((*reinterpret_cast< std::add_pointer_t<Scintilla::Position>>(_a[1]))); break;
        case 6: _t->aboutToCopy((*reinterpret_cast< std::add_pointer_t<QMimeData*>>(_a[1]))); break;
        case 7: _t->styleNeeded((*reinterpret_cast< std::add_pointer_t<Scintilla::Position>>(_a[1]))); break;
        case 8: _t->charAdded((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 9: _t->savePointChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 10: _t->modifyAttemptReadOnly(); break;
        case 11: _t->key((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 12: _t->doubleClick((*reinterpret_cast< std::add_pointer_t<Scintilla::Position>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<Scintilla::Position>>(_a[2]))); break;
        case 13: _t->updateUi((*reinterpret_cast< std::add_pointer_t<Scintilla::Update>>(_a[1]))); break;
        case 14: _t->modified((*reinterpret_cast< std::add_pointer_t<Scintilla::ModificationFlags>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<Scintilla::Position>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<Scintilla::Position>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<Scintilla::Position>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<Scintilla::Position>>(_a[6])),(*reinterpret_cast< std::add_pointer_t<Scintilla::FoldLevel>>(_a[7])),(*reinterpret_cast< std::add_pointer_t<Scintilla::FoldLevel>>(_a[8]))); break;
        case 15: _t->macroRecord((*reinterpret_cast< std::add_pointer_t<Scintilla::Message>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<Scintilla::uptr_t>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<Scintilla::sptr_t>>(_a[3]))); break;
        case 16: _t->marginClicked((*reinterpret_cast< std::add_pointer_t<Scintilla::Position>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<Scintilla::KeyMod>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3]))); break;
        case 17: _t->textAreaClicked((*reinterpret_cast< std::add_pointer_t<Scintilla::Position>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 18: _t->needShown((*reinterpret_cast< std::add_pointer_t<Scintilla::Position>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<Scintilla::Position>>(_a[2]))); break;
        case 19: _t->painted(); break;
        case 20: _t->userListSelection(); break;
        case 21: _t->uriDropped((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 22: _t->dwellStart((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 23: _t->dwellEnd((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 24: _t->zoom((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 25: _t->hotSpotClick((*reinterpret_cast< std::add_pointer_t<Scintilla::Position>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<Scintilla::KeyMod>>(_a[2]))); break;
        case 26: _t->hotSpotDoubleClick((*reinterpret_cast< std::add_pointer_t<Scintilla::Position>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<Scintilla::KeyMod>>(_a[2]))); break;
        case 27: _t->callTipClick(); break;
        case 28: _t->autoCompleteSelection((*reinterpret_cast< std::add_pointer_t<Scintilla::Position>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 29: _t->autoCompleteCancelled(); break;
        case 30: _t->focusChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 31: _t->notify((*reinterpret_cast< std::add_pointer_t<Scintilla::NotificationData*>>(_a[1]))); break;
        case 32: _t->command((*reinterpret_cast< std::add_pointer_t<Scintilla::uptr_t>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<Scintilla::sptr_t>>(_a[2]))); break;
        case 33: _t->buttonPressed((*reinterpret_cast< std::add_pointer_t<QMouseEvent*>>(_a[1]))); break;
        case 34: _t->buttonReleased((*reinterpret_cast< std::add_pointer_t<QMouseEvent*>>(_a[1]))); break;
        case 35: _t->keyPressed((*reinterpret_cast< std::add_pointer_t<QKeyEvent*>>(_a[1]))); break;
        case 36: _t->resized(); break;
        case 37: _t->scrollHorizontal((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 38: _t->scrollVertical((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 39: _t->notifyParent((*reinterpret_cast< std::add_pointer_t<Scintilla::NotificationData>>(_a[1]))); break;
        case 40: _t->event_command((*reinterpret_cast< std::add_pointer_t<Scintilla::uptr_t>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<Scintilla::sptr_t>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (ScintillaEditBase::*)(int );
            if (_q_method_type _q_method = &ScintillaEditBase::horizontalScrolled; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)(int );
            if (_q_method_type _q_method = &ScintillaEditBase::verticalScrolled; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)(int , int );
            if (_q_method_type _q_method = &ScintillaEditBase::horizontalRangeChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)(int , int );
            if (_q_method_type _q_method = &ScintillaEditBase::verticalRangeChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)();
            if (_q_method_type _q_method = &ScintillaEditBase::notifyChange; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)(Scintilla::Position );
            if (_q_method_type _q_method = &ScintillaEditBase::linesAdded; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)(QMimeData * );
            if (_q_method_type _q_method = &ScintillaEditBase::aboutToCopy; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)(Scintilla::Position );
            if (_q_method_type _q_method = &ScintillaEditBase::styleNeeded; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)(int );
            if (_q_method_type _q_method = &ScintillaEditBase::charAdded; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 8;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)(bool );
            if (_q_method_type _q_method = &ScintillaEditBase::savePointChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 9;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)();
            if (_q_method_type _q_method = &ScintillaEditBase::modifyAttemptReadOnly; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 10;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)(int );
            if (_q_method_type _q_method = &ScintillaEditBase::key; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 11;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)(Scintilla::Position , Scintilla::Position );
            if (_q_method_type _q_method = &ScintillaEditBase::doubleClick; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 12;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)(Scintilla::Update );
            if (_q_method_type _q_method = &ScintillaEditBase::updateUi; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 13;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)(Scintilla::ModificationFlags , Scintilla::Position , Scintilla::Position , Scintilla::Position , const QByteArray & , Scintilla::Position , Scintilla::FoldLevel , Scintilla::FoldLevel );
            if (_q_method_type _q_method = &ScintillaEditBase::modified; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 14;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)(Scintilla::Message , Scintilla::uptr_t , Scintilla::sptr_t );
            if (_q_method_type _q_method = &ScintillaEditBase::macroRecord; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 15;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)(Scintilla::Position , Scintilla::KeyMod , int );
            if (_q_method_type _q_method = &ScintillaEditBase::marginClicked; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 16;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)(Scintilla::Position , int );
            if (_q_method_type _q_method = &ScintillaEditBase::textAreaClicked; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 17;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)(Scintilla::Position , Scintilla::Position );
            if (_q_method_type _q_method = &ScintillaEditBase::needShown; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 18;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)();
            if (_q_method_type _q_method = &ScintillaEditBase::painted; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 19;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)();
            if (_q_method_type _q_method = &ScintillaEditBase::userListSelection; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 20;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)(const QString & );
            if (_q_method_type _q_method = &ScintillaEditBase::uriDropped; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 21;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)(int , int );
            if (_q_method_type _q_method = &ScintillaEditBase::dwellStart; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 22;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)(int , int );
            if (_q_method_type _q_method = &ScintillaEditBase::dwellEnd; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 23;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)(int );
            if (_q_method_type _q_method = &ScintillaEditBase::zoom; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 24;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)(Scintilla::Position , Scintilla::KeyMod );
            if (_q_method_type _q_method = &ScintillaEditBase::hotSpotClick; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 25;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)(Scintilla::Position , Scintilla::KeyMod );
            if (_q_method_type _q_method = &ScintillaEditBase::hotSpotDoubleClick; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 26;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)();
            if (_q_method_type _q_method = &ScintillaEditBase::callTipClick; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 27;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)(Scintilla::Position , const QString & );
            if (_q_method_type _q_method = &ScintillaEditBase::autoCompleteSelection; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 28;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)();
            if (_q_method_type _q_method = &ScintillaEditBase::autoCompleteCancelled; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 29;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)(bool );
            if (_q_method_type _q_method = &ScintillaEditBase::focusChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 30;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)(Scintilla::NotificationData * );
            if (_q_method_type _q_method = &ScintillaEditBase::notify; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 31;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)(Scintilla::uptr_t , Scintilla::sptr_t );
            if (_q_method_type _q_method = &ScintillaEditBase::command; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 32;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)(QMouseEvent * );
            if (_q_method_type _q_method = &ScintillaEditBase::buttonPressed; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 33;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)(QMouseEvent * );
            if (_q_method_type _q_method = &ScintillaEditBase::buttonReleased; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 34;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)(QKeyEvent * );
            if (_q_method_type _q_method = &ScintillaEditBase::keyPressed; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 35;
                return;
            }
        }
        {
            using _q_method_type = void (ScintillaEditBase::*)();
            if (_q_method_type _q_method = &ScintillaEditBase::resized; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 36;
                return;
            }
        }
    }
}

const QMetaObject *ScintillaEditBase::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ScintillaEditBase::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN17ScintillaEditBaseE.stringdata0))
        return static_cast<void*>(this);
    return QAbstractScrollArea::qt_metacast(_clname);
}

int ScintillaEditBase::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractScrollArea::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 41)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 41;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 41)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 41;
    }
    return _id;
}

// SIGNAL 0
void ScintillaEditBase::horizontalScrolled(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ScintillaEditBase::verticalScrolled(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ScintillaEditBase::horizontalRangeChanged(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ScintillaEditBase::verticalRangeChanged(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ScintillaEditBase::notifyChange()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void ScintillaEditBase::linesAdded(Scintilla::Position _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void ScintillaEditBase::aboutToCopy(QMimeData * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void ScintillaEditBase::styleNeeded(Scintilla::Position _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void ScintillaEditBase::charAdded(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void ScintillaEditBase::savePointChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void ScintillaEditBase::modifyAttemptReadOnly()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void ScintillaEditBase::key(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void ScintillaEditBase::doubleClick(Scintilla::Position _t1, Scintilla::Position _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void ScintillaEditBase::updateUi(Scintilla::Update _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void ScintillaEditBase::modified(Scintilla::ModificationFlags _t1, Scintilla::Position _t2, Scintilla::Position _t3, Scintilla::Position _t4, const QByteArray & _t5, Scintilla::Position _t6, Scintilla::FoldLevel _t7, Scintilla::FoldLevel _t8)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t5))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t6))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t7))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t8))) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void ScintillaEditBase::macroRecord(Scintilla::Message _t1, Scintilla::uptr_t _t2, Scintilla::sptr_t _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void ScintillaEditBase::marginClicked(Scintilla::Position _t1, Scintilla::KeyMod _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void ScintillaEditBase::textAreaClicked(Scintilla::Position _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void ScintillaEditBase::needShown(Scintilla::Position _t1, Scintilla::Position _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void ScintillaEditBase::painted()
{
    QMetaObject::activate(this, &staticMetaObject, 19, nullptr);
}

// SIGNAL 20
void ScintillaEditBase::userListSelection()
{
    QMetaObject::activate(this, &staticMetaObject, 20, nullptr);
}

// SIGNAL 21
void ScintillaEditBase::uriDropped(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 21, _a);
}

// SIGNAL 22
void ScintillaEditBase::dwellStart(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 22, _a);
}

// SIGNAL 23
void ScintillaEditBase::dwellEnd(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 23, _a);
}

// SIGNAL 24
void ScintillaEditBase::zoom(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 24, _a);
}

// SIGNAL 25
void ScintillaEditBase::hotSpotClick(Scintilla::Position _t1, Scintilla::KeyMod _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 25, _a);
}

// SIGNAL 26
void ScintillaEditBase::hotSpotDoubleClick(Scintilla::Position _t1, Scintilla::KeyMod _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 26, _a);
}

// SIGNAL 27
void ScintillaEditBase::callTipClick()
{
    QMetaObject::activate(this, &staticMetaObject, 27, nullptr);
}

// SIGNAL 28
void ScintillaEditBase::autoCompleteSelection(Scintilla::Position _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 28, _a);
}

// SIGNAL 29
void ScintillaEditBase::autoCompleteCancelled()
{
    QMetaObject::activate(this, &staticMetaObject, 29, nullptr);
}

// SIGNAL 30
void ScintillaEditBase::focusChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 30, _a);
}

// SIGNAL 31
void ScintillaEditBase::notify(Scintilla::NotificationData * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 31, _a);
}

// SIGNAL 32
void ScintillaEditBase::command(Scintilla::uptr_t _t1, Scintilla::sptr_t _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 32, _a);
}

// SIGNAL 33
void ScintillaEditBase::buttonPressed(QMouseEvent * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 33, _a);
}

// SIGNAL 34
void ScintillaEditBase::buttonReleased(QMouseEvent * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 34, _a);
}

// SIGNAL 35
void ScintillaEditBase::keyPressed(QKeyEvent * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 35, _a);
}

// SIGNAL 36
void ScintillaEditBase::resized()
{
    QMetaObject::activate(this, &staticMetaObject, 36, nullptr);
}
QT_WARNING_POP
