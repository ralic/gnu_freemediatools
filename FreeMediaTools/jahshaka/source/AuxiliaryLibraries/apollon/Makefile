#############################################################################
# Makefile for building: libapollon.a
# Generated by qmake (1.07a) (Qt 3.3.7) on: Sat Jan 26 09:53:29 2008
# Project:  apollon.pro
# Template: lib
# Command: $(QMAKE) -o Makefile apollon.pro
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
INCPATH  = -I/usr/share/qt3/mkspecs/default -I. -I. -I../gift -I/usr/include/qt3 -I/usr/X11R6/include -I/usr/X11R6/include
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

HEADERS = apollonlistview.h \
		apollonsearchviewitem.h \
		apollonsearchlistview.h \
		apollontransferviewitem.h \
		apollontransfertab.h \
		apollontab.h \
		apollonutils.h \
		apollonqueryview.h
SOURCES = apollonlistview.cpp \
		apollonsearchlistview.cpp \
		apollonsearchviewitem.cpp \
		apollonqueryview.cpp \
		apollonutils.cpp \
		apollontransferviewitem.cpp \
		apollontab.cpp \
		apollontransfertab.cpp
OBJECTS = apollonlistview.o \
		apollonsearchlistview.o \
		apollonsearchviewitem.o \
		apollonqueryview.o \
		apollonutils.o \
		apollontransferviewitem.o \
		apollontab.o \
		apollontransfertab.o
FORMS = 
UICDECLS = 
UICIMPLS = 
SRCMOC   = moc_apollonsearchlistview.cpp \
		moc_apollontransfertab.cpp \
		moc_apollontab.cpp \
		moc_apollonqueryview.cpp
OBJMOC = moc_apollonsearchlistview.o \
		moc_apollontransfertab.o \
		moc_apollontab.o \
		moc_apollonqueryview.o
DIST	   = ../../../config.pro \
		../../../Settings.pro \
		apollon.pro
QMAKE_TARGET = apollon
DESTDIR  = 
TARGET   = libapollon.a

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

Makefile: apollon.pro  /usr/share/qt3/mkspecs/default/qmake.conf ../../../config.pro \
		../../../Settings.pro \
		/usr/share/qt3/lib/libqt-mt.prl
	$(QMAKE) -o Makefile apollon.pro
qmake: 
	@$(QMAKE) -o Makefile apollon.pro

dist: 
	@mkdir -p .tmp/apollon && $(COPY_FILE) --parents $(SOURCES) $(HEADERS) $(FORMS) $(DIST) .tmp/apollon/ && ( cd `dirname .tmp/apollon` && $(TAR) apollon.tar apollon && $(GZIP) apollon.tar ) && $(MOVE) `dirname .tmp/apollon`/apollon.tar.gz . && $(DEL_FILE) -r .tmp/apollon

mocclean:
	-$(DEL_FILE) $(OBJMOC)
	-$(DEL_FILE) $(SRCMOC)

uiclean:

yaccclean:
lexclean:
clean: mocclean
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) $(TARGET)


FORCE:

####### Compile

apollonlistview.o: apollonlistview.cpp apollonlistview.h

apollonsearchlistview.o: apollonsearchlistview.cpp apollonsearchlistview.h \
		apollonsearchviewitem.h \
		apollonlistview.h

apollonsearchviewitem.o: apollonsearchviewitem.cpp apollonsearchlistview.h \
		apollonsearchviewitem.h \
		apollonutils.h \
		apollonlistview.h

apollonqueryview.o: apollonqueryview.cpp apollonqueryview.h \
		apollonsearchviewitem.h \
		apollonutils.h \
		apollonlistview.h \
		apollonsearchlistview.h

apollonutils.o: apollonutils.cpp apollonutils.h

apollontransferviewitem.o: apollontransferviewitem.cpp apollontransferviewitem.h \
		apollonutils.h

apollontab.o: apollontab.cpp apollontab.h

apollontransfertab.o: apollontransfertab.cpp apollontransfertab.h \
		apollontransferviewitem.h \
		apollonutils.h \
		apollontab.h \
		apollonlistview.h

moc_apollonsearchlistview.o: moc_apollonsearchlistview.cpp  apollonsearchlistview.h apollonlistview.h

moc_apollontransfertab.o: moc_apollontransfertab.cpp  apollontransfertab.h apollontab.h \
		apollonlistview.h

moc_apollontab.o: moc_apollontab.cpp  apollontab.h 

moc_apollonqueryview.o: moc_apollonqueryview.cpp  apollonqueryview.h apollonlistview.h \
		apollonsearchlistview.h \
		apollonsearchviewitem.h

moc_apollonsearchlistview.cpp: $(MOC) apollonsearchlistview.h
	$(MOC) apollonsearchlistview.h -o moc_apollonsearchlistview.cpp

moc_apollontransfertab.cpp: $(MOC) apollontransfertab.h
	$(MOC) apollontransfertab.h -o moc_apollontransfertab.cpp

moc_apollontab.cpp: $(MOC) apollontab.h
	$(MOC) apollontab.h -o moc_apollontab.cpp

moc_apollonqueryview.cpp: $(MOC) apollonqueryview.h
	$(MOC) apollonqueryview.h -o moc_apollonqueryview.cpp

####### Install

install:  

uninstall:  

