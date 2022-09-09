QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
QT_QPA_PLATFORM_PLUGIN_PATH=<QT_BASE>\plugins\platforms

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dropPlace.cpp \
    imageConverter.cpp \
    main.cpp \
    mainwindow.cpp \
    settings.cpp

HEADERS += \
    dropPlace.h \
    imageConverter.h \
    mainwindow.h \
    settings.h

FORMS += \
    mainwindow.ui

LIBS += \
    Qt5FontDatabaseSupport.lib \
    Qt5EventDispatcherSupport.lib \
    Qt5ThemeSupport.lib \
    Qt5PlatformCompositorSupport.lib \
    Qt5AccessibilitySupport.lib \
    Qt5WindowsUiAutomationSupport.lib \
    qwindows.lib \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
