TEMPLATE = lib
CONFIG += staticlib
TARGET = MaCore
include(../malibrary.pri)
CONFIG += exceptions
INCLUDEPATH += actionbuttons \
    recipelist \
    recipegrabber \
    recipeedit \
	finder \
    nuritionaldata \
    ui
SOURCES += db/*.cpp \
    ui/*.cpp \
    eventmanager.cpp \
    fraction.cpp \
    recipelist/*.cpp \
    recipeedit/*.cpp \
    actionbuttons/*.cpp \
    recipegrabber/*.cpp \
    finder/*.cpp \
    nuritionaldata/*.cpp
HEADERS += db/*.h \
    ui/*.h \
    eventmanager.h \
    controlbase.h \
    fraction.h \
    eventlistener.h \
    recipelist/*.h \
    recipeedit/*.h \
    actionbuttons/*.h \
    recipegrabber/*.h \
    finder/*.h \
    nuritionaldata/*.h
FORMS += actionbuttons/*.ui \
    recipelist/*.ui \
    recipegrabber/*.ui \
    ui/*.ui \
    recipeedit/*.ui \
    finder/*.ui \
    nuritionaldata/*.ui
RESOURCES += \
    actionbuttons/maimage.qrc \
    ui/qtres.qrc
TRANSLATIONS += $$IDE_SOURCE_TREE/resource/mamtkon_he.ts
