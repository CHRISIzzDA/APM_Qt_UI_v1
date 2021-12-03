QT       += core gui serialport #added Serialport for µC communication

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    apmui.cpp \
    main.cpp \
    mainwindow.cpp \
    usbsettings.cpp

HEADERS += \
    apmui.h \
    mainwindow.h \
    usbsettings.h

FORMS += \
    apmui.ui \
    mainwindow.ui \
    usbsettings.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
