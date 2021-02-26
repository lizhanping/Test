 
#QT       += core gui webenginewidgets
QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET =Exam
CONFIG += c++11
win32:QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"
win32: LIBS    +=  -luser32 -lgdi32

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    callbackhandler.cpp \
    customlabel.cpp \
    exitwindow.cpp \
    hook.cpp \
    main_win.cpp \
    mainwindow.cpp \
    simple_app.cc \
    simple_handler.cc \
    utils.cpp \
    global.cpp \

HEADERS += \
    callbackhandler.h \
    customlabel.h \
    exitwindow.h \
    hook.h \
    mainwindow.h \
    simple_app.h \
    simple_handler.h \
    utils.h \
    global.h \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rsc.qrc

RC_FILE=icon.rc

DISTFILES += \
    icon.rc

#cef lib
win32: LIBS += -L$$PWD/third_party/libs/cef/ -llibcef
win32: LIBS += -L$$PWD/third_party/libs/cef/ -llibcef_dll_wrapper
win32: PRE_TARGETDEPS += $$PWD/third_party/libs/cef/libcef_dll_wrapper.lib

win32:INCLUDEPATH += $$PWD/third_party/include/cef/
win32:DEPENDPATH += $$PWD/third_party/include/cef/
