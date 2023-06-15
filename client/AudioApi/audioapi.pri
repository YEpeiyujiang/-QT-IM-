QT += multimedia


LIBS += $$PWD/speex/lib/libspeex.lib\


HEADERS += \
    $$PWD/audio_common.h \
    $$PWD/audio_read.h \
    $$PWD/audio_write.h

SOURCES += \
    $$PWD/audio_read.cpp \
    $$PWD/audio_write.cpp
