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

INCLUDEPATH+= /usr/local/include
LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc
