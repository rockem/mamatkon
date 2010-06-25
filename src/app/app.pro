include(../../mamatkon.pri)
TEMPLATE = app
TARGET = $$IDE_APP_TARGET
DESTDIR = $$IDE_APP_PATH
CONFIG += ordered
POST_TARGETDEPS = $$IDE_LIBRARY_PATH/libMaCored.a
SOURCES += main.cpp
