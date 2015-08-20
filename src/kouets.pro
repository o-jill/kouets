QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = kouets
TEMPLATE = app

SOURCES += decorate.cpp \
    kouetsapp.cpp \
    kouetshash.cpp \
    main.cpp \
    mainwindow.cpp \
    projectfile.cpp

HEADERS += decorate.h \
    kouetsapp.h \
    kouetshash.h \
    mainwindow.h \
    projectfile.h

FORMS += \
    mainwindow.ui


win32 {
    RC_FILE = kouets.rc
#    LIBS += -lole32
#    LIBS += -luser32 -lshell32 -lole32
    DEFINES += _CRT_SECURE_NO_WARNINGS WINDOWS
    QMAKE_LFLAGS += /map
}

# Create source about git commit hash
updategithash.target = ../src/kouetshash.cpp
updategithash.depends = ../.git/index
win32:updategithash.commands =pushd .&&cd ../src&&gen_git_hash.bat H kouetshash&&gen_git_hash.bat CPP kouetshash&&popd
QMAKE_EXTRA_TARGETS += updategithash

#updategithashhook.depends = updategithash
#CONFIG(debug,debug|release):updategithashhook.target = Makefile.Debug
#CONFIG(release,debug|release):updategithashhook.target = Makefile.Release
#QMAKE_EXTRA_TARGETS += updategithashhook


# Create our custom updatever target.
#win32:updatever.commands =pushd .&&cd ../src&&copy version.cpp +&&popd
#QMAKE_EXTRA_TARGETS += updatever

#updateverhook.depends = updatever
#CONFIG(debug,debug|release):updateverhook.target = Makefile.Debug
#CONFIG(release,debug|release):updateverhook.target = Makefile.Release
#QMAKE_EXTRA_TARGETS += updateverhook


RESOURCES += \
    kouets.qrc
