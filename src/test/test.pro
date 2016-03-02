#
# test suite
#

QT += testlib # core # gui network opengl phonon sql svg xml webkit qt3support

# greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

HEADERS += AutoTest.h \
           fileconfig_test.h \
           ../fileconfig.h

SOURCES += testmain.cpp \
           fileconfig_test.cpp
