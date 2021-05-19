QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

QMAKE_CXXFLAGS_RELEASE += 2
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    MyTimeClass.cpp \
    main.cpp \
    mainwindow.cpp \
    BrdcEphemeris.cpp \
    calculations.cpp \
    RinexNavigacyjny.cpp \
    RinexObserwacyjny.cpp \
    RinexOVer3.cpp

HEADERS += \
    BrdcEphemeris.h \
    MyTimeClass.h \
    calculations.h \
    mainwindow.h \
    RinexNavigacyjny.h \
    RinexObserwacyjny.h \
    RinexOVer3.h

INCLUDEPATH += D:\BIBLIOTEKI\armadillo-10.4.0\include

LIBS += \
    -LC:\armadillo\examples\lib_win64 \
    -llapack_win64_MT \
    -lblas_win64_MT

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc
