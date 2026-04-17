/****************************************************************************
** Meta object code from reading C++ file 'InternetControlPlugin.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/InternetControlPlugin.h"
#include <QtCore/qmetatype.h>
#include <QtCore/qplugin.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'InternetControlPlugin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_InternetControlPlugin_t {
    uint offsetsAndSizes[2];
    char stringdata0[22];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_InternetControlPlugin_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_InternetControlPlugin_t qt_meta_stringdata_InternetControlPlugin = {
    {
        QT_MOC_LITERAL(0, 21)   // "InternetControlPlugin"
    },
    "InternetControlPlugin"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_InternetControlPlugin[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

Q_CONSTINIT const QMetaObject InternetControlPlugin::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_InternetControlPlugin.offsetsAndSizes,
    qt_meta_data_InternetControlPlugin,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_InternetControlPlugin_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<InternetControlPlugin, std::true_type>
    >,
    nullptr
} };

void InternetControlPlugin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

const QMetaObject *InternetControlPlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *InternetControlPlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_InternetControlPlugin.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "FeatureProviderInterface"))
        return static_cast< FeatureProviderInterface*>(this);
    if (!strcmp(_clname, "PluginInterface"))
        return static_cast< PluginInterface*>(this);
    if (!strcmp(_clname, "io.veyon.Veyon.Plugins.PluginInterface"))
        return static_cast< PluginInterface*>(this);
    if (!strcmp(_clname, "io.veyon.Veyon.FeatureProviderInterface"))
        return static_cast< FeatureProviderInterface*>(this);
    return QObject::qt_metacast(_clname);
}

int InternetControlPlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}

#ifdef QT_MOC_EXPORT_PLUGIN_V2
static constexpr unsigned char qt_pluginMetaDataV2_InternetControlPlugin[] = {
    0xbf, 
    // "IID"
    0x02,  0x78,  0x1e,  'i',  'o',  '.',  'v',  'e', 
    'y',  'o',  'n',  '.',  'V',  'e',  'y',  'o', 
    'n',  '.',  'P',  'l',  'u',  'g',  'i',  'n', 
    'I',  'n',  't',  'e',  'r',  'f',  'a',  'c', 
    'e', 
    // "className"
    0x03,  0x75,  'I',  'n',  't',  'e',  'r',  'n', 
    'e',  't',  'C',  'o',  'n',  't',  'r',  'o', 
    'l',  'P',  'l',  'u',  'g',  'i',  'n', 
    // "MetaData"
    0x04,  0xa8,  0x66,  'A',  'u',  't',  'h',  'o', 
    'r',  0x69,  'T',  'u',  ' ',  'N',  'o',  'm', 
    'b',  'r',  'e',  0x68,  'C',  'a',  't',  'e', 
    'g',  'o',  'r',  'y',  0x67,  'F',  'e',  'a', 
    't',  'u',  'r',  'e',  0x69,  'C',  'o',  'p', 
    'y',  'r',  'i',  'g',  'h',  't',  0x64,  '2', 
    '0',  '2',  '4',  0x6b,  'D',  'e',  's',  'c', 
    'r',  'i',  'p',  't',  'i',  'o',  'n',  0x78, 
    0x1c,  'C',  'o',  'n',  't',  'r',  'o',  'l', 
    ' ',  'd',  'e',  ' ',  'a',  'c',  'c',  'e', 
    's',  'o',  ' ',  'a',  ' ',  'I',  'n',  't', 
    'e',  'r',  'n',  'e',  't',  0x6a,  'I',  'n', 
    't',  'e',  'r',  'f',  'a',  'c',  'e',  's', 
    0x82,  0x6f,  'P',  'l',  'u',  'g',  'i',  'n', 
    'I',  'n',  't',  'e',  'r',  'f',  'a',  'c', 
    'e',  0x78,  0x18,  'F',  'e',  'a',  't',  'u', 
    'r',  'e',  'P',  'r',  'o',  'v',  'i',  'd', 
    'e',  'r',  'I',  'n',  't',  'e',  'r',  'f', 
    'a',  'c',  'e',  0x67,  'L',  'i',  'c',  'e', 
    'n',  's',  'e',  0x66,  'G',  'P',  'L',  'v', 
    '2',  '+',  0x64,  'N',  'a',  'm',  'e',  0x6f, 
    'I',  'n',  't',  'e',  'r',  'n',  'e',  't', 
    'C',  'o',  'n',  't',  'r',  'o',  'l',  0x67, 
    'V',  'e',  'r',  's',  'i',  'o',  'n',  0x63, 
    '1',  '.',  '0', 
    0xff, 
};
QT_MOC_EXPORT_PLUGIN_V2(InternetControlPlugin, InternetControlPlugin, qt_pluginMetaDataV2_InternetControlPlugin)
#else
QT_PLUGIN_METADATA_SECTION
Q_CONSTINIT static constexpr unsigned char qt_pluginMetaData_InternetControlPlugin[] = {
    'Q', 'T', 'M', 'E', 'T', 'A', 'D', 'A', 'T', 'A', ' ', '!',
    // metadata version, Qt version, architectural requirements
    0, QT_VERSION_MAJOR, QT_VERSION_MINOR, qPluginArchRequirements(),
    0xbf, 
    // "IID"
    0x02,  0x78,  0x1e,  'i',  'o',  '.',  'v',  'e', 
    'y',  'o',  'n',  '.',  'V',  'e',  'y',  'o', 
    'n',  '.',  'P',  'l',  'u',  'g',  'i',  'n', 
    'I',  'n',  't',  'e',  'r',  'f',  'a',  'c', 
    'e', 
    // "className"
    0x03,  0x75,  'I',  'n',  't',  'e',  'r',  'n', 
    'e',  't',  'C',  'o',  'n',  't',  'r',  'o', 
    'l',  'P',  'l',  'u',  'g',  'i',  'n', 
    // "MetaData"
    0x04,  0xa8,  0x66,  'A',  'u',  't',  'h',  'o', 
    'r',  0x69,  'T',  'u',  ' ',  'N',  'o',  'm', 
    'b',  'r',  'e',  0x68,  'C',  'a',  't',  'e', 
    'g',  'o',  'r',  'y',  0x67,  'F',  'e',  'a', 
    't',  'u',  'r',  'e',  0x69,  'C',  'o',  'p', 
    'y',  'r',  'i',  'g',  'h',  't',  0x64,  '2', 
    '0',  '2',  '4',  0x6b,  'D',  'e',  's',  'c', 
    'r',  'i',  'p',  't',  'i',  'o',  'n',  0x78, 
    0x1c,  'C',  'o',  'n',  't',  'r',  'o',  'l', 
    ' ',  'd',  'e',  ' ',  'a',  'c',  'c',  'e', 
    's',  'o',  ' ',  'a',  ' ',  'I',  'n',  't', 
    'e',  'r',  'n',  'e',  't',  0x6a,  'I',  'n', 
    't',  'e',  'r',  'f',  'a',  'c',  'e',  's', 
    0x82,  0x6f,  'P',  'l',  'u',  'g',  'i',  'n', 
    'I',  'n',  't',  'e',  'r',  'f',  'a',  'c', 
    'e',  0x78,  0x18,  'F',  'e',  'a',  't',  'u', 
    'r',  'e',  'P',  'r',  'o',  'v',  'i',  'd', 
    'e',  'r',  'I',  'n',  't',  'e',  'r',  'f', 
    'a',  'c',  'e',  0x67,  'L',  'i',  'c',  'e', 
    'n',  's',  'e',  0x66,  'G',  'P',  'L',  'v', 
    '2',  '+',  0x64,  'N',  'a',  'm',  'e',  0x6f, 
    'I',  'n',  't',  'e',  'r',  'n',  'e',  't', 
    'C',  'o',  'n',  't',  'r',  'o',  'l',  0x67, 
    'V',  'e',  'r',  's',  'i',  'o',  'n',  0x63, 
    '1',  '.',  '0', 
    0xff, 
};
QT_MOC_EXPORT_PLUGIN(InternetControlPlugin, InternetControlPlugin)
#endif  // QT_MOC_EXPORT_PLUGIN_V2

QT_WARNING_POP
QT_END_MOC_NAMESPACE
