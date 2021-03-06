/****************************************************************************
** ClientSocket meta object code from reading C++ file 'httpd.h'
**
** Created: Wed Jan 23 19:09:23 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "httpd.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ClientSocket::className() const
{
    return "ClientSocket";
}

QMetaObject *ClientSocket::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ClientSocket( "ClientSocket", &ClientSocket::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ClientSocket::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ClientSocket", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ClientSocket::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ClientSocket", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ClientSocket::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QSocket::staticMetaObject();
    static const QUMethod slot_0 = {"readClient", 0, 0 };
    static const QUMethod slot_1 = {"connectionClosed", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "readClient()", &slot_0, QMetaData::Private },
	{ "connectionClosed()", &slot_1, QMetaData::Private }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"sendtext", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ 0, &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod signal_1 = {"updateDesktop", 1, param_signal_1 };
    static const QMetaData signal_tbl[] = {
	{ "sendtext(QString)", &signal_0, QMetaData::Private },
	{ "updateDesktop(assetData)", &signal_1, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"ClientSocket", parentObject,
	slot_tbl, 2,
	signal_tbl, 2,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ClientSocket.setMetaObject( metaObj );
    return metaObj;
}

void* ClientSocket::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ClientSocket" ) )
	return this;
    return QSocket::qt_cast( clname );
}

// SIGNAL sendtext
void ClientSocket::sendtext( QString t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL updateDesktop
void ClientSocket::updateDesktop( assetData t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 1 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

bool ClientSocket::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: readClient(); break;
    case 1: connectionClosed(); break;
    default:
	return QSocket::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ClientSocket::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: sendtext((QString)static_QUType_QString.get(_o+1)); break;
    case 1: updateDesktop((assetData)(*((assetData*)static_QUType_ptr.get(_o+1)))); break;
    default:
	return QSocket::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool ClientSocket::qt_property( int id, int f, QVariant* v)
{
    return QSocket::qt_property( id, f, v);
}

bool ClientSocket::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *SimpleServer::className() const
{
    return "SimpleServer";
}

QMetaObject *SimpleServer::metaObj = 0;
static QMetaObjectCleanUp cleanUp_SimpleServer( "SimpleServer", &SimpleServer::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString SimpleServer::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SimpleServer", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString SimpleServer::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SimpleServer", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* SimpleServer::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QServerSocket::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"socketText", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ 0, &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod slot_1 = {"socketUpdateDesktop", 1, param_slot_1 };
    static const QMetaData slot_tbl[] = {
	{ "socketText(QString)", &slot_0, QMetaData::Public },
	{ "socketUpdateDesktop(assetData)", &slot_1, QMetaData::Public }
    };
    static const QUMethod signal_0 = {"newConnect", 0, 0 };
    static const QUParameter param_signal_1[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_1 = {"copyText", 1, param_signal_1 };
    static const QUParameter param_signal_2[] = {
	{ 0, &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod signal_2 = {"sendupdateDesktop", 1, param_signal_2 };
    static const QUParameter param_signal_3[] = {
	{ 0, &static_QUType_ptr, "QHostAddress", QUParameter::In }
    };
    static const QUMethod signal_3 = {"recipConnect", 1, param_signal_3 };
    static const QMetaData signal_tbl[] = {
	{ "newConnect()", &signal_0, QMetaData::Public },
	{ "copyText(QString)", &signal_1, QMetaData::Public },
	{ "sendupdateDesktop(assetData)", &signal_2, QMetaData::Public },
	{ "recipConnect(QHostAddress)", &signal_3, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"SimpleServer", parentObject,
	slot_tbl, 2,
	signal_tbl, 4,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_SimpleServer.setMetaObject( metaObj );
    return metaObj;
}

void* SimpleServer::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "SimpleServer" ) )
	return this;
    return QServerSocket::qt_cast( clname );
}

// SIGNAL newConnect
void SimpleServer::newConnect()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

// SIGNAL copyText
void SimpleServer::copyText( QString t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 1, t0 );
}

// SIGNAL sendupdateDesktop
void SimpleServer::sendupdateDesktop( assetData t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 2 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL recipConnect
void SimpleServer::recipConnect( QHostAddress t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 3 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

bool SimpleServer::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: socketText((QString)static_QUType_QString.get(_o+1)); break;
    case 1: socketUpdateDesktop((assetData)(*((assetData*)static_QUType_ptr.get(_o+1)))); break;
    default:
	return QServerSocket::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool SimpleServer::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: newConnect(); break;
    case 1: copyText((QString)static_QUType_QString.get(_o+1)); break;
    case 2: sendupdateDesktop((assetData)(*((assetData*)static_QUType_ptr.get(_o+1)))); break;
    case 3: recipConnect((QHostAddress)(*((QHostAddress*)static_QUType_ptr.get(_o+1)))); break;
    default:
	return QServerSocket::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool SimpleServer::qt_property( int id, int f, QVariant* v)
{
    return QServerSocket::qt_property( id, f, v);
}

bool SimpleServer::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
