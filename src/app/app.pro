include(../../mamatkon.pri)
TEMPLATE = app
TARGET = $$IDE_APP_TARGET
DESTDIR = $$IDE_APP_PATH
CONFIG += ordered
win32:POST_TARGETDEPS = $$IDE_LIBRARY_PATH/MaCored.lib
unix:POST_TARGETDEPS = $$IDE_LIBRARY_PATH/libMaCore.a
SOURCES += main.cpp
