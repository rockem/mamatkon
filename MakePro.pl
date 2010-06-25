$app='mamatkon.pro';
$boost_dir = $ENV{'BOOSTDIR'};
$boost_dir =~ s/\\/\//g; #convert \ to /
$includepath = "\"include include/mamatkon/ui $boost_dir\"";
$config='"console qt debug"';
$libs='win32:LIBS+=""';
$os = ($^O =~ /MSWin32/) ? 'win32-msvc2003' : 'unix';	
$files='src/ resource/ include/mamatkon';
$qt='"sql network"';
$extra_targets='"check-syntax
check-syntax.commands=$(CXX) -c -nologo -Zm300 -Zi -MDd -GR -EHsc -W3 -w34100 -w34189 \
-DUNICODE -DWIN32 -DQT_LARGEFILE_SUPPORT -DQT_DLL -DQT_SQL_LIB \
-DQT_GUI_LIB -DQT_CORE_LIB -DQT_THREAD_SUPPORT \
-I"c:\Dev\qt-src-4.5.2\include\QtCore" \
-I"c:\Dev\qt-src-4.5.2\include\QtGui" \
-I"c:\Dev\qt-src-4.5.2\include\QtSql" \
-I"c:\Dev\qt-src-4.5.2\include" \
-I"include" -I"include\mamatkon\ui\qt" \
-I"." -I"c:\Dev\qt-src-4.5.2\include\ActiveQt" \
-I"debug" \
-I"c:\Dev\qt-src-4.5.2\mkspecs\win32-msvc2003" \
-Fodebug \
$(CHK_SOURCES)"';

print "$app config:\n";
print "====================\n";
print "FILES=$files\n";
print "LIBS=$libs\n";
print "INCLUDES=$includepath\n";
print "CONFIG=$config\n";
print "\n";

`qmake -project $files -nopwd -r $libs QMAKE_EXTRA_TARGETS+=$extra_targets QT+=$qt CONFIG+=$config INCLUDEPATH+=$includepath -o $app`;
`qmake $app -spec $os`;

print "Make files were succesfuly created :)\n";

`lrelease $app`;

print 'Translation files updated';