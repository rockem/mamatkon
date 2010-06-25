include(../../mamatkon.pri)

win32 {
    # DLLDESTDIR = $$IDE_APP_PATH
}

DESTDIR = $$IDE_LIBRARY_PATH

TARGET = $$qtLibraryTarget($$TARGET)

# contains(QT_  CONFIG, reduce_exports):CONFIG += hide_symbols

unix:!macx {
    target.path = /$$IDE_LIBRARY_BASENAME/qtcreator
    INSTALLS += target
}
