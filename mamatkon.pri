defineReplace(cleanPath) {
    win32:1 ~= s|\\\\|/|g
    contains(1, ^/.*):pfx = /
    else:pfx =
    segs = $$split(1, /)
    out =
    for(seg, segs) {
        equals(seg, ..):out = $$member(out, 0, -2)
        else:!equals(seg, .):out += $$seg
    }
    return($$join(out, /, $$pfx))
}

defineReplace(targetPath) {
    win32:1 ~= s|/|\|g
    return($$1)
}

isEmpty(IDE_LIBRARY_BASENAME) {
    IDE_LIBRARY_BASENAME = lib
}

DEFINES += IDE_LIBRARY_BASENAME=\\\"$$IDE_LIBRARY_BASENAME\\\"

IDE_SOURCE_TREE = $$PWD
isEmpty(IDE_BUILD_TREE) {
    sub_dir = $$_PRO_FILE_PWD_
    sub_dir ~= s,^$$re_escape($$PWD),,
    IDE_BUILD_TREE = $$cleanPath($$OUT_PWD)
    IDE_BUILD_TREE ~= s,$$re_escape($$sub_dir)$,,
}
IDE_APP_PATH = $$IDE_BUILD_TREE/bin

macx {
    IDE_APP_TARGET   = Mamatkon
    IDE_LIBRARY_PATH = $$IDE_APP_PATH/$${IDE_APP_TARGET}.app/Contents/PlugIns
    IDE_PLUGIN_PATH  = $$IDE_LIBRARY_PATH
    IDE_LIBEXEC_PATH = $$IDE_APP_PATH/$${IDE_APP_TARGET}.app/Contents/Resources
    IDE_DATA_PATH    = $$IDE_APP_PATH/$${IDE_APP_TARGET}.app/Contents/Resources
    contains(QT_CONFIG, ppc):CONFIG += ppc x86
    copydata = 1
} else {
    win32 {
        #contains(TEMPLATE, vc.*)|contains(TEMPLATE_PREFIX, vc):vcproj = 1
        IDE_APP_TARGET   = mamatkon
    } else {
        IDE_APP_WRAPPER  = mamatkon
        IDE_APP_TARGET   = mamatkon.bin
    }
    IDE_LIBRARY_PATH = $$IDE_BUILD_TREE/$$IDE_LIBRARY_BASENAME/mamatkon
    IDE_PLUGIN_PATH  = $$IDE_LIBRARY_PATH/plugins
    IDE_LIBEXEC_PATH = $$IDE_APP_PATH # FIXME
    IDE_DATA_PATH    = $$IDE_BUILD_TREE/share/qtcreator
    !isEqual(IDE_SOURCE_TREE, $$IDE_BUILD_TREE):copydata = 1
}

CONFIG += console qt debug

INCLUDEPATH += \
    $$IDE_SOURCE_TREE/src/libs \
    $$(BOOSTDIR)

DEPENDPATH += \
    $$IDE_SOURCE_TREE/src/libs

#LIBS += $$IDE_LIBRARY_PATH/MaCore.lib
#    LIBS += $$IDE_LIBRARY_PATH/MaCored.lib

CONFIG(release, debug|release):LIBS += $$IDE_LIBRARY_PATH/MaCore.lib
CONFIG(debug, debug|release):LIBS += $$IDE_LIBRARY_PATH/MaCored.lib

QT += sql network



