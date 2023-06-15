HEADERS += \
    $$PWD/desk_read.h \
    $$PWD/video_common.h \
    $$PWD/video_read.h

SOURCES += \
    $$PWD/desk_read.cpp \
    $$PWD/video_read.cpp


#使用 opencv 4.2.0

INCLUDEPATH += C:\Qt\opencv3.4.0\opencv3.4.0\OpenCV-MinGW-Build-OpenCV-3.4.5\include

LIBS +=C:\Qt\opencv3.4.0\opencv3.4.0\OpenCV-MinGW-Build-OpenCV-3.4.5\x86\mingw\bin\libopencv_*.dll
