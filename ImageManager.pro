QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    main.cpp \
    fileslist.cpp \
    mainwindow.cpp \
    resizedialog.cpp \
    unitedialog.cpp

HEADERS += \
    fileslist.h \
    mainwindow.h \
    resizedialog.h \
    unitedialog.h

RESOURCES += \
    tools.qrc

INCLUDEPATH+=C:\\Qt\\Tools\\opencv_mingw492_32\\install\\include

LIBS += \
    -LC:\\Qt\\Tools\\opencv_mingw492_32\\install\\x86\\mingw\\lib\\libopencv_core2412.dll \
    -LC:\\Qt\\Tools\\opencv_mingw492_32\\install\\x86\\mingw\\lib\\libopencv_highgui2412.dll \
    -LC:\\Qt\\Tools\\opencv_mingw492_32\\install\\x86\\mingw\\lib\\libopencv_imgproc2412.dll
