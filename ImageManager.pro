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

unix{
    INCLUDEPATH+= /usr/local/include
    LIBS += -L/usr/local/lib \
    -lopencv_core \
    -lopencv_imgcodecs \
    -lopencv_highgui \
    -lopencv_imgproc
}

win32{
    INCLUDEPATH += C:/Qt/Tools/opencv_mingw/install/include
    LIBS += -L"C:/Qt/Tools/opencv_mingw/install/x86/mingw/lib" \
    -lopencv_core310 \
    -lopencv_highgui310 \
    -lopencv_imgproc310 \
    -lopencv_imgcodecs310
}
