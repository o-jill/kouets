#
# test suite
#

QT += testlib core gui xml # network opengl phonon sql svg webkit qt3support

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

HEADERS += AutoTest.h \
           fileconfig_test.h \
           projectfile_test.h \
           projectxml_test.h \
           decorate_test.h \
           ../decorate.h \
           ../fileconfig.h \
           ../projectfile.h \
           ../projectxml.h


SOURCES += testmain.cpp \
           fileconfig_test.cpp \
           projectfile_test.cpp \
           projectxml_test.cpp \
           decorate_test.cpp \
           ../decorate.cpp \
           ../projectfile.cpp \
           ../projectxml.cpp

DEFINES += __KOUETS_TEST__
