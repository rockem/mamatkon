include(../mamatkon.pri)

TEMPLATE = app
TARGET = tests
DESTDIR = $$IDE_APP_PATH
CONFIG += ordered

POST_TARGETDEPS = $$IDE_LIBRARY_PATH/MaCored.lib

INCLUDEPATH += . ext/include

win32:LIBS += ext/vslib/gmock.lib

SOURCES+= *.cpp \
    db/*.cpp

HEADERS+= \
    *.h \
    mock/*.h


