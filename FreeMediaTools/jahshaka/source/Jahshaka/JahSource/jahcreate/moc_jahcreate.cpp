/****************************************************************************
** JahControl meta object code from reading C++ file 'jahcreate.h'
**
** Created: Sat Jan 26 08:33:38 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "jahcreate.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *JahControl::className() const
{
    return "JahControl";
}

QMetaObject *JahControl::metaObj = 0;
static QMetaObjectCleanUp cleanUp_JahControl( "JahControl", &JahControl::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString JahControl::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "JahControl", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString JahControl::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "JahControl", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* JahControl::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"switchMode", 0, 0 };
    static const QUMethod slot_1 = {"splitterChanged", 0, 0 };
    static const QUMethod slot_2 = {"switchDesktop", 0, 0 };
    static const QUMethod slot_3 = {"GetDesktop", 0, 0 };
    static const QUMethod slot_4 = {"GetDesktopKey", 0, 0 };
    static const QUMethod slot_5 = {"ReturnDesktop", 0, 0 };
    static const QUParameter param_slot_6[] = {
	{ 0, &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod slot_6 = {"addImageToDesktop", 1, param_slot_6 };
    static const QUParameter param_slot_7[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_7 = {"addStringToDesktop", 1, param_slot_7 };
    static const QUParameter param_slot_8[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_8 = {"updateDesktopLabel", 1, param_slot_8 };
    static const QUMethod slot_9 = {"getClipFromLibrary", 0, 0 };
    static const QUParameter param_slot_10[] = {
	{ "modname", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_10 = {"switchModule", 1, param_slot_10 };
    static const QUParameter param_slot_11[] = {
	{ "modname", &static_QUType_ptr, "Module", QUParameter::In }
    };
    static const QUMethod slot_11 = {"switchModule", 1, param_slot_11 };
    static const QUParameter param_slot_12[] = {
	{ "modname", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_12 = {"switchModule", 1, param_slot_12 };
    static const QUMethod slot_13 = {"slotSmartLoader", 0, 0 };
    static const QUMethod slot_14 = {"slotSmartSaver", 0, 0 };
    static const QUParameter param_slot_15[] = {
	{ "fn", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_15 = {"loadModuleFile", 1, param_slot_15 };
    static const QUParameter param_slot_16[] = {
	{ "mod", &static_QUType_ptr, "Module", QUParameter::In }
    };
    static const QUMethod slot_16 = {"raiseMenu", 1, param_slot_16 };
    static const QUParameter param_slot_17[] = {
	{ "mod", &static_QUType_ptr, "Module", QUParameter::In }
    };
    static const QUMethod slot_17 = {"selectMenuDropdown", 1, param_slot_17 };
    static const QUParameter param_slot_18[] = {
	{ "fn", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_18 = {"loadModelFile", 1, param_slot_18 };
    static const QUMethod slot_19 = {"GrabClipEffects", 0, 0 };
    static const QUMethod slot_20 = {"GrabClipEdit", 0, 0 };
    static const QUMethod slot_21 = {"AddClipEdit", 0, 0 };
    static const QUMethod slot_22 = {"ReturnClipEdit", 0, 0 };
    static const QUMethod slot_23 = {"GrabClipAnim", 0, 0 };
    static const QUMethod slot_24 = {"GrabClipAnimKey", 0, 0 };
    static const QUMethod slot_25 = {"GrabClipCG", 0, 0 };
    static const QUMethod slot_26 = {"GrabClipColorize", 0, 0 };
    static const QUMethod slot_27 = {"GrabClipPaint", 0, 0 };
    static const QUMethod slot_28 = {"GrabClipKeyer", 0, 0 };
    static const QUMethod slot_29 = {"GrabClipKeyerBase", 0, 0 };
    static const QUMethod slot_30 = {"GrabClipTracker", 0, 0 };
    static const QUParameter param_slot_31[] = {
	{ "item", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_31 = {"renameDesktopAsset", 1, param_slot_31 };
    static const QUMethod slot_32 = {"resetGlobalClipNumber", 0, 0 };
    static const QUMethod slot_33 = {"draggedAImage", 0, 0 };
    static const QUMethod slot_34 = {"setJahTheme", 0, 0 };
    static const QUParameter param_slot_35[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_35 = {"recordTheme", 1, param_slot_35 };
    static const QUParameter param_slot_36[] = {
	{ 0, &static_QUType_int, 0, QUParameter::Out }
    };
    static const QUMethod slot_36 = {"getJahThemeColor", 1, param_slot_36 };
    static const QUParameter param_slot_37[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_37 = {"hidetheUI", 1, param_slot_37 };
    static const QUMethod slot_38 = {"changeModeDesktop", 0, 0 };
    static const QUMethod slot_39 = {"changeModePlayer", 0, 0 };
    static const QUParameter param_slot_40[] = {
	{ 0, &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod slot_40 = {"syncMediaTable", 1, param_slot_40 };
    static const QUMethod slot_41 = {"notifyPlayerOfChange", 0, 0 };
    static const QUMethod slot_42 = {"notifyPlayerOfNextItem", 0, 0 };
    static const QUMethod slot_43 = {"notifyEncoderOfChange", 0, 0 };

    static const QUMethod slot_44 = {"changeModeNetworkNet", 0, 0 };
    static const QUMethod slot_45 = {"changeModeNetworkCom", 0, 0 };
    static const QUMethod slot_46 = {"changeModeNetworkP2P", 0, 0 };
    static const QUMethod slot_47 = {"changeModeNetworkHttpd", 0, 0 };

    static const QUMethod slot_48 = {"changeModeEncoding", 0, 0 };
    static const QUMethod slot_49 = {"changeModeVideoIO", 0, 0 };
    static const QUMethod slot_50 = {"changeModeBroadcasting", 0, 0 };
    static const QUMethod slot_51 = {"changeModeColorize", 0, 0 };
    static const QUMethod slot_52 = {"changeModePref", 0, 0 };
    static const QUMethod slot_53 = {"changeModePrefReturn", 0, 0 };
    static const QUMethod slot_54 = {"changeModeEffects", 0, 0 };
    static const QUMethod slot_55 = {"changeModeAnimation", 0, 0 };
    static const QUMethod slot_56 = {"changeModeEditing", 0, 0 };
    static const QUMethod slot_57 = {"changeModePaint", 0, 0 };
    static const QUMethod slot_58 = {"changeModeText", 0, 0 };
    static const QUMethod slot_59 = {"changeModeLibrary", 0, 0 };
    static const QUMethod slot_60 = {"changeModeTracker", 0, 0 };
    static const QUMethod slot_61 = {"changeModeKeyer", 0, 0 };
    static const QUMethod slot_62 = {"checkClearPlayer", 0, 0 };
    static const QUMethod slot_63 = {"checkDesktopButton", 0, 0 };
    static const QUMethod slot_64 = {"stopAnimation", 0, 0 };
    static const QUMethod slot_65 = {"saveSplitterValues", 0, 0 };
    static const QUMethod slot_66 = {"nameClip", 0, 0 };
    static const QUMethod slot_67 = {"saveAndUpdateColor", 0, 0 };
    static const QUMethod slot_68 = {"setstyleGrey", 0, 0 };
    static const QUMethod slot_69 = {"setstyleBlue", 0, 0 };
    static const QUMethod slot_70 = {"setstyleBlack", 0, 0 };
    static const QUMethod slot_71 = {"setstyleWhite", 0, 0 };
    static const QUParameter param_slot_72[] = {
	{ "size", &static_QUType_int, 0, QUParameter::In },
	{ "font", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_72 = {"styleFont", 2, param_slot_72 };
    static const QUMethod slot_73 = {"setStoragePath", 0, 0 };
    static const QUMethod slot_74 = {"getStoragePath", 0, 0 };
    static const QUParameter param_slot_75[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_75 = {"setGlobalRenderQuality", 1, param_slot_75 };
    static const QUMethod slot_76 = {"getGlobalPreferences", 0, 0 };
    static const QUMethod slot_77 = {"setModulesRenderQuality", 0, 0 };
    static const QUMethod slot_78 = {"setGlobalPositionSize", 0, 0 };
    static const QUMethod slot_79 = {"setVideoHeadOption", 0, 0 };
    static const QUMethod slot_80 = {"setStyleOption", 0, 0 };
    static const QUMethod slot_81 = {"setWidescreenOption", 0, 0 };
    static const QUMethod slot_82 = {"setDualHeadOption", 0, 0 };
    static const QUMethod slot_83 = {"setMenuOption", 0, 0 };
    static const QUParameter param_slot_84[] = {
	{ "renval", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_84 = {"setRenderer", 1, param_slot_84 };
    static const QUParameter param_slot_85[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_85 = {"setLanguage", 1, param_slot_85 };
    static const QUParameter param_slot_86[] = {
	{ "renval", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_86 = {"setResolution", 1, param_slot_86 };
    static const QUParameter param_slot_87[] = {
	{ 0, &static_QUType_ptr, "JahHeadable", QUParameter::In }
    };
    static const QUMethod slot_87 = {"showInHead", 1, param_slot_87 };
    static const QUMethod slot_88 = {"setQuickStart", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "switchMode()", &slot_0, QMetaData::Public },
	{ "splitterChanged()", &slot_1, QMetaData::Public },
	{ "switchDesktop()", &slot_2, QMetaData::Public },
	{ "GetDesktop()", &slot_3, QMetaData::Public },
	{ "GetDesktopKey()", &slot_4, QMetaData::Public },
	{ "ReturnDesktop()", &slot_5, QMetaData::Public },
	{ "addImageToDesktop(assetData)", &slot_6, QMetaData::Public },
	{ "addStringToDesktop(QString)", &slot_7, QMetaData::Public },
	{ "updateDesktopLabel(QString)", &slot_8, QMetaData::Public },
	{ "getClipFromLibrary()", &slot_9, QMetaData::Public },
	{ "switchModule(QString)", &slot_10, QMetaData::Public },
	{ "switchModule(Module)", &slot_11, QMetaData::Public },
	{ "switchModule(int)", &slot_12, QMetaData::Public },
	{ "slotSmartLoader()", &slot_13, QMetaData::Public },
	{ "slotSmartSaver()", &slot_14, QMetaData::Public },
	{ "loadModuleFile(QString)", &slot_15, QMetaData::Public },
	{ "raiseMenu(Module)", &slot_16, QMetaData::Public },
	{ "selectMenuDropdown(Module)", &slot_17, QMetaData::Public },
	{ "loadModelFile(QString)", &slot_18, QMetaData::Public },
	{ "GrabClipEffects()", &slot_19, QMetaData::Public },
	{ "GrabClipEdit()", &slot_20, QMetaData::Public },
	{ "AddClipEdit()", &slot_21, QMetaData::Public },
	{ "ReturnClipEdit()", &slot_22, QMetaData::Public },
	{ "GrabClipAnim()", &slot_23, QMetaData::Public },
	{ "GrabClipAnimKey()", &slot_24, QMetaData::Public },
	{ "GrabClipCG()", &slot_25, QMetaData::Public },
	{ "GrabClipColorize()", &slot_26, QMetaData::Public },
	{ "GrabClipPaint()", &slot_27, QMetaData::Public },
	{ "GrabClipKeyer()", &slot_28, QMetaData::Public },
	{ "GrabClipKeyerBase()", &slot_29, QMetaData::Public },
	{ "GrabClipTracker()", &slot_30, QMetaData::Public },
	{ "renameDesktopAsset(QString)", &slot_31, QMetaData::Public },
	{ "resetGlobalClipNumber()", &slot_32, QMetaData::Public },
	{ "draggedAImage()", &slot_33, QMetaData::Public },
	{ "setJahTheme()", &slot_34, QMetaData::Public },
	{ "recordTheme(const QString&)", &slot_35, QMetaData::Public },
	{ "getJahThemeColor()", &slot_36, QMetaData::Public },
	{ "hidetheUI(bool)", &slot_37, QMetaData::Public },
	{ "changeModeDesktop()", &slot_38, QMetaData::Public },
	{ "changeModePlayer()", &slot_39, QMetaData::Public },
	{ "syncMediaTable(assetData)", &slot_40, QMetaData::Public },
	{ "notifyPlayerOfChange()", &slot_41, QMetaData::Public },
	{ "notifyPlayerOfNextItem()", &slot_42, QMetaData::Public },
	{ "notifyEncoderOfChange()", &slot_43, QMetaData::Public },

	{ "changeModeNetworkNet()", &slot_44, QMetaData::Public },
	{ "changeModeNetworkCom()", &slot_45, QMetaData::Public },
	{ "changeModeNetworkP2P()", &slot_46, QMetaData::Public },
	{ "changeModeNetworkHttpd()", &slot_47, QMetaData::Public },

	{ "changeModeEncoding()", &slot_48, QMetaData::Public },
	{ "changeModeVideoIO()", &slot_49, QMetaData::Public },
	{ "changeModeBroadcasting()", &slot_50, QMetaData::Public },
	{ "changeModeColorize()", &slot_51, QMetaData::Public },
	{ "changeModePref()", &slot_52, QMetaData::Public },
	{ "changeModePrefReturn()", &slot_53, QMetaData::Public },
	{ "changeModeEffects()", &slot_54, QMetaData::Public },
	{ "changeModeAnimation()", &slot_55, QMetaData::Public },
	{ "changeModeEditing()", &slot_56, QMetaData::Public },
	{ "changeModePaint()", &slot_57, QMetaData::Public },
	{ "changeModeText()", &slot_58, QMetaData::Public },
	{ "changeModeLibrary()", &slot_59, QMetaData::Public },
	{ "changeModeTracker()", &slot_60, QMetaData::Public },
	{ "changeModeKeyer()", &slot_61, QMetaData::Public },
	{ "checkClearPlayer()", &slot_62, QMetaData::Public },
	{ "checkDesktopButton()", &slot_63, QMetaData::Public },
	{ "stopAnimation()", &slot_64, QMetaData::Public },
	{ "saveSplitterValues()", &slot_65, QMetaData::Public },
	{ "nameClip()", &slot_66, QMetaData::Public },
	{ "saveAndUpdateColor()", &slot_67, QMetaData::Public },
	{ "setstyleGrey()", &slot_68, QMetaData::Public },
	{ "setstyleBlue()", &slot_69, QMetaData::Public },
	{ "setstyleBlack()", &slot_70, QMetaData::Public },
	{ "setstyleWhite()", &slot_71, QMetaData::Public },
	{ "styleFont(int,QString)", &slot_72, QMetaData::Public },
	{ "setStoragePath()", &slot_73, QMetaData::Public },
	{ "getStoragePath()", &slot_74, QMetaData::Public },
	{ "setGlobalRenderQuality(int)", &slot_75, QMetaData::Public },
	{ "getGlobalPreferences()", &slot_76, QMetaData::Public },
	{ "setModulesRenderQuality()", &slot_77, QMetaData::Public },
	{ "setGlobalPositionSize()", &slot_78, QMetaData::Public },
	{ "setVideoHeadOption()", &slot_79, QMetaData::Public },
	{ "setStyleOption()", &slot_80, QMetaData::Public },
	{ "setWidescreenOption()", &slot_81, QMetaData::Public },
	{ "setDualHeadOption()", &slot_82, QMetaData::Public },
	{ "setMenuOption()", &slot_83, QMetaData::Public },
	{ "setRenderer(int)", &slot_84, QMetaData::Public },
	{ "setLanguage(const QString&)", &slot_85, QMetaData::Public },
	{ "setResolution(int)", &slot_86, QMetaData::Public },
	{ "showInHead(JahHeadable*)", &slot_87, QMetaData::Public },
	{ "setQuickStart()", &slot_88, QMetaData::Public }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"addProgressSteps", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_1 = {"creatingModule", 1, param_signal_1 };
    static const QUMethod signal_2 = {"updateProgress", 0, 0 };
    static const QUParameter param_signal_3[] = {
	{ 0, &static_QUType_varptr, "\x0b", QUParameter::In }
    };
    static const QUMethod signal_3 = {"emitStyleColor", 1, param_signal_3 };
    static const QUParameter param_signal_4[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod signal_4 = {"hideUI", 1, param_signal_4 };
    static const QMetaData signal_tbl[] = {
	{ "addProgressSteps(int)", &signal_0, QMetaData::Public },
	{ "creatingModule(const QString&)", &signal_1, QMetaData::Public },
	{ "updateProgress()", &signal_2, QMetaData::Public },
	{ "emitStyleColor(QPalette)", &signal_3, QMetaData::Public },
	{ "hideUI(bool)", &signal_4, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"JahControl", parentObject,
	slot_tbl, 89,
	signal_tbl, 5,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_JahControl.setMetaObject( metaObj );
    return metaObj;
}

void* JahControl::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "JahControl" ) )
	return this;
    return QObject::qt_cast( clname );
}

// SIGNAL addProgressSteps
void JahControl::addProgressSteps( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

// SIGNAL creatingModule
void JahControl::creatingModule( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 1, t0 );
}

// SIGNAL updateProgress
void JahControl::updateProgress()
{
    activate_signal( staticMetaObject()->signalOffset() + 2 );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL emitStyleColor
void JahControl::emitStyleColor( QPalette t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 3 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_varptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL hideUI
void JahControl::hideUI( bool t0 )
{
    activate_signal_bool( staticMetaObject()->signalOffset() + 4, t0 );
}

bool JahControl::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: switchMode(); break;
    case 1: splitterChanged(); break;
    case 2: switchDesktop(); break;
    case 3: GetDesktop(); break;
    case 4: GetDesktopKey(); break;
    case 5: ReturnDesktop(); break;
    case 6: addImageToDesktop((assetData)(*((assetData*)static_QUType_ptr.get(_o+1)))); break;
    case 7: addStringToDesktop((QString)static_QUType_QString.get(_o+1)); break;
    case 8: updateDesktopLabel((QString)static_QUType_QString.get(_o+1)); break;
    case 9: getClipFromLibrary(); break;
    case 10: switchModule((QString)static_QUType_QString.get(_o+1)); break;
    case 11: switchModule((Module)(*((Module*)static_QUType_ptr.get(_o+1)))); break;
    case 12: switchModule((int)static_QUType_int.get(_o+1)); break;
    case 13: slotSmartLoader(); break;
    case 14: slotSmartSaver(); break;
    case 15: loadModuleFile((QString)static_QUType_QString.get(_o+1)); break;
    case 16: raiseMenu((Module)(*((Module*)static_QUType_ptr.get(_o+1)))); break;
    case 17: selectMenuDropdown((Module)(*((Module*)static_QUType_ptr.get(_o+1)))); break;
    case 18: loadModelFile((QString)static_QUType_QString.get(_o+1)); break;
    case 19: GrabClipEffects(); break;
    case 20: GrabClipEdit(); break;
    case 21: AddClipEdit(); break;
    case 22: ReturnClipEdit(); break;
    case 23: GrabClipAnim(); break;
    case 24: GrabClipAnimKey(); break;
    case 25: GrabClipCG(); break;
    case 26: GrabClipColorize(); break;
    case 27: GrabClipPaint(); break;
    case 28: GrabClipKeyer(); break;
    case 29: GrabClipKeyerBase(); break;
    case 30: GrabClipTracker(); break;
    case 31: renameDesktopAsset((QString)static_QUType_QString.get(_o+1)); break;
    case 32: resetGlobalClipNumber(); break;
    case 33: draggedAImage(); break;
    case 34: setJahTheme(); break;
    case 35: recordTheme((const QString&)static_QUType_QString.get(_o+1)); break;
    case 36: static_QUType_int.set(_o,getJahThemeColor()); break;
    case 37: hidetheUI((bool)static_QUType_bool.get(_o+1)); break;
    case 38: changeModeDesktop(); break;
    case 39: changeModePlayer(); break;
    case 40: syncMediaTable((assetData)(*((assetData*)static_QUType_ptr.get(_o+1)))); break;
    case 41: notifyPlayerOfChange(); break;
    case 42: notifyPlayerOfNextItem(); break;
    case 43: notifyEncoderOfChange(); break;

#ifdef USE_NETWORK
    case 44: changeModeNetworkNet(); break;
    case 45: changeModeNetworkCom(); break;
    case 46: changeModeNetworkP2P(); break;
    case 47: changeModeNetworkHttpd(); break;
#endif
    case 48: changeModeEncoding(); break;
    case 49: changeModeVideoIO(); break;
    case 50: changeModeBroadcasting(); break;
    case 51: changeModeColorize(); break;
    case 52: changeModePref(); break;
    case 53: changeModePrefReturn(); break;
    case 54: changeModeEffects(); break;
    case 55: changeModeAnimation(); break;
    case 56: changeModeEditing(); break;
    case 57: changeModePaint(); break;
    case 58: changeModeText(); break;
    case 59: changeModeLibrary(); break;
    case 60: changeModeTracker(); break;
    case 61: changeModeKeyer(); break;
    case 62: checkClearPlayer(); break;
    case 63: checkDesktopButton(); break;
    case 64: stopAnimation(); break;
    case 65: saveSplitterValues(); break;
    case 66: nameClip(); break;
    case 67: saveAndUpdateColor(); break;
    case 68: setstyleGrey(); break;
    case 69: setstyleBlue(); break;
    case 70: setstyleBlack(); break;
    case 71: setstyleWhite(); break;
    case 72: styleFont((int)static_QUType_int.get(_o+1),(QString)static_QUType_QString.get(_o+2)); break;
    case 73: setStoragePath(); break;
    case 74: getStoragePath(); break;
    case 75: setGlobalRenderQuality((int)static_QUType_int.get(_o+1)); break;
    case 76: getGlobalPreferences(); break;
    case 77: setModulesRenderQuality(); break;
    case 78: setGlobalPositionSize(); break;
    case 79: setVideoHeadOption(); break;
    case 80: setStyleOption(); break;
    case 81: setWidescreenOption(); break;
    case 82: setDualHeadOption(); break;
    case 83: setMenuOption(); break;
    case 84: setRenderer((int)static_QUType_int.get(_o+1)); break;
    case 85: setLanguage((const QString&)static_QUType_QString.get(_o+1)); break;
    case 86: setResolution((int)static_QUType_int.get(_o+1)); break;
    case 87: showInHead((JahHeadable*)static_QUType_ptr.get(_o+1)); break;
    case 88: setQuickStart(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool JahControl::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: addProgressSteps((int)static_QUType_int.get(_o+1)); break;
    case 1: creatingModule((const QString&)static_QUType_QString.get(_o+1)); break;
    case 2: updateProgress(); break;
    case 3: emitStyleColor((QPalette)(*((QPalette*)static_QUType_ptr.get(_o+1)))); break;
    case 4: hideUI((bool)static_QUType_bool.get(_o+1)); break;
    default:
	return QObject::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool JahControl::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool JahControl::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
