QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ckernel.cpp \
    logindialog.cpp \
    main.cpp \
    demodialog.cpp \
    roomdialog.cpp \
    setuserdialog.cpp \
    usershow.cpp \
    workthread.cpp

HEADERS += \
    ckernel.h \
    demodialog.h \
    logindialog.h \
    roomdialog.h \
    setuserdialog.h \
    usershow.h \
    workthread.h

FORMS += \
    demodialog.ui \
    logindialog.ui \
    roomdialog.ui \
    setuserdialog.ui \
    usershow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

include(./uiapi/uiapi.pri)
include(./netapi/netapi.pri)
include(./MD5/MD5.pri)
include(./VideoApi/videoapi.pri)
include(./AudioApi/audioapi.pri)
INCLUDEPATH += ./AudioApi/
INCLUDEPATH += ./VideoApi/
###  -I
INCLUDEPATH += ./netapi/\
        ./MD5
        ./uiapi/

RESOURCES += \
    resouse.qrc

INCLUDEPATH += C:\Qt\opencv3.4.0\opencv3.4.0\OpenCV-MinGW-Build-OpenCV-3.4.5\include

LIBS +=C:\Qt\opencv3.4.0\opencv3.4.0\OpenCV-MinGW-Build-OpenCV-3.4.5\x86\mingw\bin\libopencv_*.dll
