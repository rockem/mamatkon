include(../mamatkon.pri)

TEMPLATE = app
TARGET = tests
DESTDIR = $$IDE_APP_PATH
CONFIG += ordered

win32:POST_TARGETDEPS = $$IDE_LIBRARY_PATH/MaCored.lib
unix:POST_TARGETDEPS = $$IDE_LIBRARY_PATH/libMaCore.a

INCLUDEPATH += . ext/include

win32:LIBS += ext/lib/vc/gmock.lib
unix:LIBS += ext/lib/gcc/libgmock.a

SOURCES+= *.cpp \
    db/*.cpp

HEADERS+= \
    *.h \
    mock/*.h


