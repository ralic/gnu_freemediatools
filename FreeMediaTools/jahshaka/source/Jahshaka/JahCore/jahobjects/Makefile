#############################################################################
# Makefile for building: libjahobjects.a
# Generated by qmake (1.07a) (Qt 3.3.7) on: Sat Jan 26 09:53:32 2008
# Project:  jahobjects.pro
# Template: lib
# Command: $(QMAKE) -o Makefile jahobjects.pro
#############################################################################

####### Compiler, tools and options

CC       = gcc
CXX      = g++
LEX      = flex
YACC     = yacc
CFLAGS   = -pipe -Wall -W -g -D_REENTRANT  -DJAHPREFIX=\"/mnt/sda10/mdupont/experiments/video/js/experiments/video/js/openlibraries/install\" -D_THREAD_SAFE=true -DHAVE_GL_GLEW_H -DNEWOPENOBJECTLIB -DJAHTHEMES -DJAHGIFT -DNVIDIA_GPU -DNEWOPENOBJECTLIB -DOPENIMAGELIB_PLUGINS=\"/home/mdupont/experiments/video/js/openlibraries/trunk/src/openimagelib/\" -DOPENMEDIALIB_PLUGINS=\"/home/mdupont/experiments/video/js/openlibraries/trunk/src/openmedialib/\" -DOPENEFFECTSLIB_PLUGINS=\"/home/mdupont/experiments/video/js/openlibraries/trunk/src/openeffectslib/\" -DOPENOBJECTLIB_PLUGINS=\"/home/mdupont/experiments/video/js/openlibraries/trunk/src/openobjectlib/\" -DJAHSCRIPT -DJAHLINUX -DQT_THREAD_SUPPORT -DQT_SHARED -DQT_TABLET_SUPPORT
CXXFLAGS = -pipe -Wall -W -g -D_REENTRANT  -DJAHPREFIX=\"/mnt/sda10/mdupont/experiments/video/js/experiments/video/js/openlibraries/install\" -D_THREAD_SAFE=true -DHAVE_GL_GLEW_H -DNEWOPENOBJECTLIB -DJAHTHEMES -DJAHGIFT -DNVIDIA_GPU -DNEWOPENOBJECTLIB -DOPENIMAGELIB_PLUGINS=\"/home/mdupont/experiments/video/js/openlibraries/trunk/src/openimagelib/\" -DOPENMEDIALIB_PLUGINS=\"/home/mdupont/experiments/video/js/openlibraries/trunk/src/openmedialib/\" -DOPENEFFECTSLIB_PLUGINS=\"/home/mdupont/experiments/video/js/openlibraries/trunk/src/openeffectslib/\" -DOPENOBJECTLIB_PLUGINS=\"/home/mdupont/experiments/video/js/openlibraries/trunk/src/openobjectlib/\" -DJAHSCRIPT -DJAHLINUX -DQT_THREAD_SUPPORT -DQT_SHARED -DQT_TABLET_SUPPORT
LEXFLAGS = 
YACCFLAGS= -d
INCPATH  = -I/usr/share/qt3/mkspecs/default -I. -I. -I../../../AuxiliaryLibraries/glew -I../../../AuxiliaryLibraries/FTGL -I../../../AuxiliaryLibraries/particle -I../../../Jahshaka/JahWidgets/keyframes -I../../../Jahshaka/JahWidgets/nodes -I../../../OpenLibraries/opencore -I../../../OpenLibraries/openassetlib -I../../../OpenLibraries/opengpulib -I../../../OpenLibraries/openobjectlib -I../../../OpenLibraries/openmedialib -I../../../OpenLibraries/openimagelib -I../../../OpenLibraries/openmedialib/mediaobject -I../../JahCore/jahobjects -I../../JahCore/jahrender -I../../JahCore/jahworld -I../../JahWidgets/interfaceobjs -I../../JahLibraries -I../../JahLibraries/jahtracer -I../../JahLibraries/jahtimer -I../../JahLibraries/jahkeyframes -I../../JahLibraries/jahpreferences -I../../JahLibraries/jahformatter -I../../JahLibraries/jahtranslate -I../../JahLibraries/jahdataio -I../../JahLibraries/jahplugins -I../../JahLibraries/jahglcore -I/usr/include/freetype2 -I/usr/local/include -I/usr/include -I../../../../../openlibraries/trunk/src -I/usr/include/qt3 -I/usr/X11R6/include -I/usr/X11R6/include
AR       = ar cqs
RANLIB   = 
MOC      = /usr/share/qt3/bin/moc
UIC      = /usr/share/qt3/bin/uic
QMAKE    = qmake
TAR      = tar -cf
GZIP     = gzip -9f
COPY     = cp -f
COPY_FILE= $(COPY)
COPY_DIR = $(COPY) -r
INSTALL_FILE= $(COPY_FILE)
INSTALL_DIR = $(COPY_DIR)
DEL_FILE = rm -f
SYMLINK  = ln -sf
DEL_DIR  = rmdir
MOVE     = mv -f
CHK_DIR_EXISTS= test -d
MKDIR    = mkdir -p

####### Output directory

OBJECTS_DIR = ./

####### Files

HEADERS = coreheaders.h \
		coreobject.h \
		coreeffects.h \
		corestructures.h
SOURCES = coreobject.cpp \
		coreeffects.cpp \
		jahasset.cpp \
		jahutility.cpp \
		jahtexture.cpp \
		corestructures.cpp
OBJECTS = coreobject.o \
		coreeffects.o \
		jahasset.o \
		jahutility.o \
		jahtexture.o \
		corestructures.o
FORMS = 
UICDECLS = 
UICIMPLS = 
SRCMOC   = 
OBJMOC = 
DIST	   = ../../../../config.pro \
		../../../../Settings.pro \
		jahobjects.pro
QMAKE_TARGET = jahobjects
DESTDIR  = 
TARGET   = libjahobjects.a

first: all
####### Implicit rules

.SUFFIXES: .c .o .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

####### Build rules

all: Makefile $(TARGET) 

staticlib: $(TARGET)

$(TARGET):  $(UICDECLS) $(OBJECTS) $(OBJMOC) $(OBJCOMP) 
	-$(DEL_FILE) $(TARGET)
	$(AR) $(TARGET) $(OBJECTS) $(OBJMOC)


mocables: $(SRCMOC)
uicables: $(UICDECLS) $(UICIMPLS)

$(MOC): 
	( cd $(QTDIR)/src/moc && $(MAKE) )

Makefile: jahobjects.pro  /usr/share/qt3/mkspecs/default/qmake.conf ../../../../config.pro \
		../../../../Settings.pro \
		/usr/share/qt3/lib/libqt-mt.prl
	$(QMAKE) -o Makefile jahobjects.pro
qmake: 
	@$(QMAKE) -o Makefile jahobjects.pro

dist: 
	@mkdir -p .tmp/jahobjects && $(COPY_FILE) --parents $(SOURCES) $(HEADERS) $(FORMS) $(DIST) .tmp/jahobjects/ && ( cd `dirname .tmp/jahobjects` && $(TAR) jahobjects.tar jahobjects && $(GZIP) jahobjects.tar ) && $(MOVE) `dirname .tmp/jahobjects`/jahobjects.tar.gz . && $(DEL_FILE) -r .tmp/jahobjects

mocclean:

uiclean:

yaccclean:
lexclean:
clean:
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) $(TARGET)


FORCE:

####### Compile

coreobject.o: coreobject.cpp coreobject.h \
		coreheaders.h \
		corestructures.h \
		coreeffects.h

coreeffects.o: coreeffects.cpp coreeffects.h

jahasset.o: jahasset.cpp coreobject.h \
		coreheaders.h \
		corestructures.h \
		coreeffects.h

jahutility.o: jahutility.cpp coreobject.h \
		coreheaders.h \
		corestructures.h \
		coreeffects.h

jahtexture.o: jahtexture.cpp coreobject.h \
		coreheaders.h \
		corestructures.h \
		coreeffects.h

corestructures.o: corestructures.cpp corestructures.h

####### Install

install:  

uninstall:  

