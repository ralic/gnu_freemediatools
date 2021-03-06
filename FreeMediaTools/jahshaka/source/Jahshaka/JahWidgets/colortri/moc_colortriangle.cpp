/****************************************************************************
** ColorTriangle meta object code from reading C++ file 'colortriangle.h'
**
** Created: Sat Jan 26 08:33:47 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "colortriangle.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ColorTriangle::className() const
{
    return "ColorTriangle";
}

QMetaObject *ColorTriangle::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ColorTriangle( "ColorTriangle", &ColorTriangle::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ColorTriangle::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ColorTriangle", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ColorTriangle::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ColorTriangle", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ColorTriangle::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QFrame::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "col", &static_QUType_varptr, "\x0a", QUParameter::In }
    };
    static const QUMethod slot_0 = {"setColor", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "col", &static_QUType_varptr, "\x0a", QUParameter::In }
    };
    static const QUMethod slot_1 = {"setColorSilently", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "col", &static_QUType_varptr, "\x0a", QUParameter::In }
    };
    static const QUMethod slot_2 = {"setColorSilentlyConstantHue", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "hueshift", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"setCorrectorHueShift", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "sat", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"setCorrectorSaturation", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "angle", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"setCorrectorHueOffset", 1, param_slot_5 };
    static const QUParameter param_slot_6[] = {
	{ "rad", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_6 = {"setCorrectorHueStrength", 1, param_slot_6 };
    static const QUParameter param_slot_7[] = {
	{ "rad", &static_QUType_double, 0, QUParameter::In }
    };
    static const QUMethod slot_7 = {"setCorrectorHueStrength", 1, param_slot_7 };
    static const QUParameter param_slot_8[] = {
	{ "pos", &static_QUType_ptr, "DoublePoint", QUParameter::In }
    };
    static const QUMethod slot_8 = {"moveSelector", 1, param_slot_8 };
    static const QMetaData slot_tbl[] = {
	{ "setColor(const QColor&)", &slot_0, QMetaData::Public },
	{ "setColorSilently(const QColor&)", &slot_1, QMetaData::Public },
	{ "setColorSilentlyConstantHue(const QColor&)", &slot_2, QMetaData::Public },
	{ "setCorrectorHueShift(int)", &slot_3, QMetaData::Public },
	{ "setCorrectorSaturation(int)", &slot_4, QMetaData::Public },
	{ "setCorrectorHueOffset(int)", &slot_5, QMetaData::Public },
	{ "setCorrectorHueStrength(int)", &slot_6, QMetaData::Public },
	{ "setCorrectorHueStrength(double)", &slot_7, QMetaData::Public },
	{ "moveSelector(DoublePoint)", &slot_8, QMetaData::Public }
    };
    static const QUParameter param_signal_0[] = {
	{ "col", &static_QUType_varptr, "\x0a", QUParameter::In }
    };
    static const QUMethod signal_0 = {"colorChangedInternal", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ "col", &static_QUType_varptr, "\x0a", QUParameter::In }
    };
    static const QUMethod signal_1 = {"colorChangedExternal", 1, param_signal_1 };
    static const QUParameter param_signal_2[] = {
	{ "correctorHueShift", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_2 = {"correctorHueShiftChanged", 1, param_signal_2 };
    static const QUParameter param_signal_3[] = {
	{ "correctorHueOffset", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_3 = {"correctorHueOffsetChanged", 1, param_signal_3 };
    static const QUParameter param_signal_4[] = {
	{ "correctorHueStrength", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_4 = {"correctorHueStrengthChanged", 1, param_signal_4 };
    static const QUParameter param_signal_5[] = {
	{ "offset", &static_QUType_int, 0, QUParameter::In },
	{ "strength", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_5 = {"correctorWheelChanged", 2, param_signal_5 };
    static const QMetaData signal_tbl[] = {
	{ "colorChangedInternal(const QColor&)", &signal_0, QMetaData::Public },
	{ "colorChangedExternal(const QColor&)", &signal_1, QMetaData::Public },
	{ "correctorHueShiftChanged(int)", &signal_2, QMetaData::Public },
	{ "correctorHueOffsetChanged(int)", &signal_3, QMetaData::Public },
	{ "correctorHueStrengthChanged(int)", &signal_4, QMetaData::Public },
	{ "correctorWheelChanged(int,int)", &signal_5, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"ColorTriangle", parentObject,
	slot_tbl, 9,
	signal_tbl, 6,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ColorTriangle.setMetaObject( metaObj );
    return metaObj;
}

void* ColorTriangle::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ColorTriangle" ) )
	return this;
    return QFrame::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL colorChangedInternal
void ColorTriangle::colorChangedInternal( const QColor& t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_varptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL colorChangedExternal
void ColorTriangle::colorChangedExternal( const QColor& t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 1 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_varptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL correctorHueShiftChanged
void ColorTriangle::correctorHueShiftChanged( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 2, t0 );
}

// SIGNAL correctorHueOffsetChanged
void ColorTriangle::correctorHueOffsetChanged( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 3, t0 );
}

// SIGNAL correctorHueStrengthChanged
void ColorTriangle::correctorHueStrengthChanged( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 4, t0 );
}

// SIGNAL correctorWheelChanged
void ColorTriangle::correctorWheelChanged( int t0, int t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 5 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_int.set(o+1,t0);
    static_QUType_int.set(o+2,t1);
    activate_signal( clist, o );
}

bool ColorTriangle::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: setColor((const QColor&)*((const QColor*)static_QUType_ptr.get(_o+1))); break;
    case 1: setColorSilently((const QColor&)*((const QColor*)static_QUType_ptr.get(_o+1))); break;
    case 2: setColorSilentlyConstantHue((const QColor&)*((const QColor*)static_QUType_ptr.get(_o+1))); break;
    case 3: setCorrectorHueShift((int)static_QUType_int.get(_o+1)); break;
    case 4: setCorrectorSaturation((int)static_QUType_int.get(_o+1)); break;
    case 5: setCorrectorHueOffset((int)static_QUType_int.get(_o+1)); break;
    case 6: setCorrectorHueStrength((int)static_QUType_int.get(_o+1)); break;
    case 7: setCorrectorHueStrength((double)static_QUType_double.get(_o+1)); break;
    case 8: moveSelector((DoublePoint)(*((DoublePoint*)static_QUType_ptr.get(_o+1)))); break;
    default:
	return QFrame::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ColorTriangle::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: colorChangedInternal((const QColor&)*((const QColor*)static_QUType_ptr.get(_o+1))); break;
    case 1: colorChangedExternal((const QColor&)*((const QColor*)static_QUType_ptr.get(_o+1))); break;
    case 2: correctorHueShiftChanged((int)static_QUType_int.get(_o+1)); break;
    case 3: correctorHueOffsetChanged((int)static_QUType_int.get(_o+1)); break;
    case 4: correctorHueStrengthChanged((int)static_QUType_int.get(_o+1)); break;
    case 5: correctorWheelChanged((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    default:
	return QFrame::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool ColorTriangle::qt_property( int id, int f, QVariant* v)
{
    return QFrame::qt_property( id, f, v);
}

bool ColorTriangle::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
